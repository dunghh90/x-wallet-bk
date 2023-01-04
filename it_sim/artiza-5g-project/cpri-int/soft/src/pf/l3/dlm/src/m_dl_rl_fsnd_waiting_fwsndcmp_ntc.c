/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsnd_waiting_fwsndcmp_ntc.c
 * @brief  the processing for "ファイルデータ送信完了報告通知" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/17 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update
 * @date   2015/10/23 TDIPS)sasaki Update
 * @date   2015/11/25 TDIPS)sasaki Update
 * @date   2015/11/26 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "rrhApi_File.h"
/*!
 *  @brief  the processing for "ファイルデータ送信完了報告通知" receiving at the ファイルデータ送信(FW)待ち REC state.
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @Bug_No M-RRU-ZSYS-01853   
 *  @Bug_No M-RRU-ZSYS-02007
 *  @Bug_No M-RRU-ZSYS-02011
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/09/25 TDIPS)Lay M-RRU-ZSYS-01853 不要処理削除
 *  @date   2015/10/02 TDIPS)sasaki Update MKレビュー指摘No.253対応
 *  @date   2015/10/17 TDIPS)sasaki Update IT2問処No.103対処
 *  @date   2015/10/20 TDIPS)sasaki Update ハソ-QA-031対応
 *  @date   2015/10/23 TDIPS)sasaki Update IT2問処N0.132対処 水平展開
 *  @date   2015/11/25 TDIPS)sasaki M-RRU-ZSYS-02007 Update IT2問処No.194対処 使用中タイマ情報を無視してファームウェアファイル報告確認(更新用)タイマ停止処理を実行
 *  @date   2015/11/26 TDIPS)sasaki M-RRU-ZSYS-02011 Update IT2問処No.198対処 受信イベントを更新中のリンクに対してのみ入力するよう修正
 */
VOID m_dl_rl_fsnd_waiting_fwsndcmp_ntc(VOID* bufp, USHORT cpri_no)
{
	INT		errcd;
	UINT	a_rtn = CMD_RES_OK; 				/* 戻り値格納変数	*/
	UINT	ldw_time		= CMD_NUM0;			/* T.B.D */
	USHORT  lwTimerId;
	USHORT	lw_tgtdat		= CMD_NUM0;			/* 対象データ		*/
	USHORT	lw_chksum		= CMD_NUM0;			/* チェックサム		*/
	USHORT	lw_systype = ((CMT_TSKIF_CPRIRCV_FILSNDFINNTC*)bufp)->cpridat_filsndfinntc.signal_kind & CMD_SYS_CHK_CPRID;
	USHORT	lw_cpri;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	
	printf( "[%d]%s lw_tgtdat[%d], lw_chksum[%d], use_timer[%d], filedata_trans_rslt[%d]\n",__LINE__,__FUNCTION__,
					((CMT_TSKIF_CPRIRCV_FILSNDFINNTC *)bufp)->cpridat_filsndfinntc.tgtdat,
						((CMT_TSKIF_CPRIRCV_FILSNDFINNTC *)bufp)->cpridat_filsndfinntc.chksum,
							gt_dlw_rec_tbl.use_timer,
								gt_dlw_rec_tbl.filedata_trans_rslt);
	
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fsnd_waiting_fwsndcmp_ntc] ENTER" );

	if(lw_systype == CMD_SYS_3G)
	{
		m_dl_cm_CRDataEndRspSnd(CMD_REFW_NG, lw_systype);

		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* 対象データがREファイルなら対RE状態遷移(REC主導)へ入力する */
	if (DLD_FILENAME_RE == gw_dl_FileType)
	{
		for(lw_cpri = D_RRH_CPRINO_RE_MIN; lw_cpri <= D_RRH_CPRINO_RE_MAX; lw_cpri++)
		{
			/* 更新中リンク情報ONの場合 */
			if(gt_dlw_dlm_tbl.link_inf & (CMD_ON << (lw_cpri - 1)))
			{
				m_dl_rec_main(bufp, lw_cpri);
			}
		}
	}

	/* 対象データを取得 */
	lw_tgtdat = ((CMT_TSKIF_CPRIRCV_FILSNDFINNTC *)bufp)->cpridat_filsndfinntc.tgtdat;
	/* チェックサムを取得 */
	lw_chksum = ((CMT_TSKIF_CPRIRCV_FILSNDFINNTC *)bufp)->cpridat_filsndfinntc.chksum;
	
	/*フラッシュ書込フラグON */
	gt_dlw_dlm_tbl.write_flg = CMD_ON;
	
	/* タイマ管理テーブルのタイマ起動情報がタイマ起動中のとき */
//	if(DLD_RECRETIM_SND == gt_dlw_rec_tbl.use_timer)
	{
		lwTimerId = CMD_TIMID_FIRMWARESND_S3G;
		/* ファームウェア送信確認タイマ停止 */
		a_rtn = cm_TStop(lwTimerId,&errcd);
		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "[m_dl_rl_fsnd_waiting_fwsndcmp_ntc] Timer Stop" );
		
		/* タイマ管理テーブルのタイマ起動情報をタイマ未起動に設定 */
		gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_NONE;
	}

	/* 3:タイムアウト */
	if (DLD_RESLT_TO == gt_dlw_rec_tbl.filedata_trans_rslt)
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "fwsndcmp:TO(%d) NG=%d", gt_dlw_rec_tbl.filedata_trans_rslt, CMD_TIMOUT_S3G);
		
		/* (LTE)(0x2321)ファイルデータ送信完了報告応答(61:タイムアウト) */		
		m_dl_cm_CRDataEndRspSnd_S3G(CMD_TIMOUT_S3G, CMD_SYS_S3G, dlw_prisys_tbl);
		
		/* 再ダウンロード準備 */
		m_dl_cm_DlRetry(CMD_SYS_S3G);
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		
		return ;
	}
	/***********************************************************************/
	/* パラメータチェック処理                                              */
	/***********************************************************************/
	/* 対象データがファームウェア更新なしのとき */
	if(lw_tgtdat == CMD_FRM_NONEW)
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "fwsndcmp:Firmware No Update(%d)", lw_tgtdat);
		
		/* 待機面Flashイレース */
		m_dl_cm_FlashProSnd(CMD_SYS_S3G,CMD_ERASE_TYP,CMD_STAT_TYP);
		
		/* ダウンロード終了処理を起動 */
		m_dl_cm_DlEnd(CMD_SYS_S3G);
		
		/* LED設定処理 */
		m_cm_WriteLEDReg();

		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return ;
	}
	
	/*======================================================================*/
	/* ファイル情報報告テーブルの分割数と分割番号が不一致					*/
	/* ファイル情報報告テーブルの総データサイズと受信データサイズが不一致	*/
	/* 対象データが不一致のいづれかの場合									*/
	/*======================================================================*/
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s div_num[%d], file_size[%d], lw_tgtdat[%d], rcv_div_no[%d], rcv_data_size[%d]  \n",__LINE__,__FUNCTION__, 
		gt_dlw_mnt_reinf[CMD_NUM0].div_num,
			gt_dlw_mnt_reinf[CMD_NUM0].file_size,
				lw_tgtdat,
					gt_dlw_rec_tbl.rcv_div_no,
						gt_dlw_rec_tbl.rcv_data_size);
	#endif
	if((gt_dlw_mnt_reinf[CMD_NUM0].div_num != gt_dlw_rec_tbl.rcv_div_no) ||
		(gt_dlw_mnt_reinf[CMD_NUM0].file_size != gt_dlw_rec_tbl.rcv_data_size) ||
//		(gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg < 'A' || gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg > 'Z') ||
		((gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg < 'A' || gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg > 'Z') &&
		 (gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg < 'a' || gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg > 'z')) || /* 18B:小文字(a-z)も許容 */
		( CMD_FRM != lw_tgtdat))
	{
		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "DIVNO SIZE DATA NG(lw_tgtdat=%d/div_num=%d/rcv_div_no=%d)", lw_tgtdat, gt_dlw_mnt_reinf[CMD_NUM0].div_num, gt_dlw_rec_tbl.rcv_div_no);
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "DIVNO SIZE DATA NG(file_size=%d/rcv_data_size=%d)", gt_dlw_mnt_reinf[CMD_NUM0].file_size, gt_dlw_rec_tbl.rcv_data_size);

		/********************/
		/* CPRI信号送信処理 */
		/********************/
		/* ファイルデータ送信完了報告応答送信処理起動 */
		m_dl_cm_CRDataEndRspSnd_S3G(CMD_REFW_NG, CMD_SYS_S3G, dlw_prisys_tbl);
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		
		return ;
	}
	
	/************************************************/
	/*ファイルデータチェック処理                     */
	/************************************************/
	
	/* チェックサム判定処理 */
	/* call API to unload shared memory */
	a_rtn = rrhApi_File_Fsm_unLoadMemory(dlw_handle, gt_dlw_rec_tbl.rcv_data_size, lw_chksum);
	#ifdef FHM_DLM_FOR_DEBUG_IT1	
	printf( "[%d]%s a_rtn[%d],dlw_handle[%x], rcv_data_size[%d], lw_chksum[%d] \n",__LINE__,__FUNCTION__, a_rtn, dlw_handle, gt_dlw_rec_tbl.rcv_data_size, lw_chksum);
	#endif
	
	/* チェックサムNG */
	if(a_rtn != E_API_RCD_OK)
	{
		if(a_rtn == E_API_RCD_CHECKSUMNG)
		{
			a_rtn = CMD_CHKSUM_NG_S3G;
		}
		else
		{
			a_rtn = CMD_REFW_NG;
		}
		
		/* ファイルデータ送信完了報告応答送信処理起動 */
		m_dl_cm_CRDataEndRspSnd_S3G(a_rtn, CMD_SYS_S3G, dlw_prisys_tbl);
		
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "rrhApi_File_Fsm_unLoadMemory return NG(%d)", a_rtn);
		
		/* 再ダウンロード準備 */
		m_dl_cm_DlRetry(CMD_SYS_S3G);
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return ;
	}
	/* Implemented in function m_dl_rl_finfrsp_waiting_fwsnd() in m_dl_rl_finfrsp_waiting_fwsnd.c */
//	/*****************************************************
//	* RECからの転送データ格納用領域
//	* (FHM保持REファイル情報[0])に以下の情報を保存
//	* ・メーカ識別子
//	* ・バージョン情報
//	******************************************************/
//
//	/* メーカ識別子 */
//	gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg	|= gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM18];
//	
//	/* マイナーバージョン */
//	gt_dlw_mnt_reinf[CMD_NUM0].min_ver	|= gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM10];
//	gt_dlw_mnt_reinf[CMD_NUM0].min_ver	= (gt_dlw_mnt_reinf[CMD_NUM0].min_ver << DLD_BIT8) | gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM11];
//	
//	/* メジャーバージョン */
//	gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver	= gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM12];
//	gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver	= (gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver << DLD_BIT8) | gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM13];
	
	/******************/
	/* データ設定処理 */
	/******************/
	/*------------------------------*/
	/* ファームウェア設定処理を起動 */
	/*------------------------------*/
	/* FWファイル(FHM-FW)保存API発行 */
	a_rtn = m_dl_FirmWareSet(CMD_SYS_S3G,lw_chksum,ldw_time);
	
	/* Save RE File Request(FHM-FW) TBD*/
	//fhmApi_Save_FW_File();
	/* Save RE File Response(FHM-FW) TBD*/
	
	/*REC主導ファイルデータ送信完了報告応答 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_FINREP_WAIT;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	
	return ;
}

/* @} */

