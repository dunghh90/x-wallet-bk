/*!
 * @skip   $Id$
 * @file   m_dl_rl_ufsnd_waiting_ufwsndcmp_ntc.c
 * @brief  the processing for "運用中ファイルデータ送信完了報告通知" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/17 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update
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
 *  @brief  the processing for "運用中ファイルデータ送信完了報告通知" receiving at the 運用中ファイルデータ送信(FW)待ち REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-02007
 *  @Bug_No M-RRU-ZSYS-02011
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/10/02 TDIPS)sasaki Update MKレビュー指摘No.253対応
 *  @date   2015/10/17 TDIPS)sasaki Update IT2問処No.103対処
 *  @date   2015/10/20 TDIPS)sasaki Update ハソ-QA-031対応
 *  @date   2015/11/25 TDIPS)sasaki M-RRU-ZSYS-02007 Update IT2問処No.194対処 使用中タイマ情報を無視してファームウェアファイル報告確認(更新用)タイマ停止処理を実行
 *  @date   2015/11/26 TDIPS)sasaki M-RRU-ZSYS-02011 Update IT2問処No.198対処 受信イベントを更新中のリンクに対してのみ入力するよう修正
 */
VOID m_dl_rl_ufsnd_waiting_ufwsndcmp_ntc(VOID* bufp, USHORT cpri_no)
{
	INT		errcd;
	UINT	a_rtn = CMD_RES_OK;
	UINT	ldw_time = CMD_NUM0;
	USHORT  lwTimerId;
	USHORT	lw_tgtdat = CMD_NUM0;
	USHORT	lw_chksum = CMD_NUM0;
	USHORT	lw_cpri;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	
	printf( "[%d]%s lw_tgtdat[%d], lw_chksum[%d], use_timer[%d], filedata_trans_rslt[%d]\n",__LINE__,__FUNCTION__,
					((CMT_TSKIF_CPRIRCV_FILSNDFINNTC *)bufp)->cpridat_filsndfinntc.tgtdat,
						((CMT_TSKIF_CPRIRCV_FILSNDFINNTC *)bufp)->cpridat_filsndfinntc.chksum,
							gt_dlw_rec_tbl.use_timer,
								gt_dlw_rec_tbl.filedata_trans_rslt);
	
	#endif

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsnd_waiting_ufwsndcmp_ntc] ENTER" );
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "USE fileDatSndFinNtc Start");
		
	/* 対象データがREファイルなら対RE状態遷移(REC主導)へ入力する */
	/* check file name */
	if(DLD_FILENAME_RE == gw_dl_FileType)
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
	lw_tgtdat = ((CMT_TSKIF_CPRIRCV_UFILSNDFINNTC *)bufp)->cpridat_ufilsndfinntc.tgtdat;
	/* チェックサムを取得 */
	lw_chksum = ((CMT_TSKIF_CPRIRCV_UFILSNDFINNTC *)bufp)->cpridat_ufilsndfinntc.chksum;
	
	/*フラッシュ書込フラグON */
	gt_dlw_dlm_tbl.write_flg = CMD_ON;
	
	/* ファームウェア送信確認(更新用)タイマ起動済み */
//	if(DLD_RECRETIM_SND_UPDATE == gt_dlw_rec_tbl.use_timer)
	{
		lwTimerId = E_RRH_TIMID_USEFIRMWARESND;
		/* ファームウェア送信確認(更新用)タイマ停止 */
		a_rtn = cm_TStop(lwTimerId,&errcd);
		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "[m_dl_rl_ufsnd_waiting_ufwsndcmp_ntc] Timer Stop" );			
		
		/* タイマ管理テーブルのタイマ起動情報をタイマ未起動に設定 */
		gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_NONE;
	}

	/* 対REC情報テーブルのファイルデータ送信完了報告結果チェック */
	/* 3:タイムアウト */
	if (gt_dlw_rec_tbl.filedata_trans_rslt == DLD_RESLT_TO)
	{
		/* (LTE)(0x2621)運用中ファイルデータ送信完了報告応答(61:タイムアウト) */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "ufwsndcmp:TO(%d) NG=%d", gt_dlw_rec_tbl.filedata_trans_rslt, CMD_TIMOUT_S3G);
		
		m_dl_cm_CRUSDataEndRspSnd(CMD_TIMOUT_S3G);
		
		/* 再ダウンロード準備 */
		m_dl_cm_USDlRetry();
		
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return ;
	}
	
	/************************************************************************/
	/* 運用中ファイルデータ送信完了報告通知パラメータチェック               */
	/************************************************************************/
	/* 対象データが更新なし */
	if(lw_tgtdat == CMD_FRM_NONEW)
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "fwsndcmp:Firmware No Update(%d)", lw_tgtdat);
		
		/* FWファイル削除API発行 */
		m_dl_cm_DlEnd(CMD_SYS_S3G);

		/* LED設定処理 */
		m_cm_WriteLEDReg();

#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return ;
	}
	/*	ファイルが有効な場合フラグに3を設定する	*/
	cmw_filedatsnd_flag = 3;
	
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
	if((gt_dlw_mnt_reinf[CMD_NUM0].div_num 		!= gt_dlw_rec_tbl.rcv_div_no) 		||
		(gt_dlw_mnt_reinf[CMD_NUM0].file_size	!= gt_dlw_rec_tbl.rcv_data_size)	||
//		(gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg < 'A' || gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg > 'Z') ||
		((gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg < 'A' || gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg > 'Z') &&
		 (gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg < 'a' || gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg > 'z')) || /* 18B:小文字(a-z)も許容 */
		(( CMD_FRM != lw_tgtdat) && ( CMD_FRM_NONEW != lw_tgtdat)))
	{
		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "DIVNO SIZE DATA NG(lw_tgtdat=%d/div_num=%d/rcv_div_no=%d)", lw_tgtdat, gt_dlw_mnt_reinf[CMD_NUM0].div_num, gt_dlw_rec_tbl.rcv_div_no);
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "DIVNO SIZE DATA NG(file_size=%d/rcv_data_size=%d)", gt_dlw_mnt_reinf[CMD_NUM0].file_size, gt_dlw_rec_tbl.rcv_data_size);
		
		/* (LTE)(0x2621)運用中ファイルデータ送信完了報告応答(221:ダウンロードファイル異常) */
		m_dl_cm_CRUSDataEndRspSnd(CMD_REFW_NG);
		
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return ;
	}
	
	/************************************************************************/
	/* ファイルデータチェック処理                                           */
	/************************************************************************/
	
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
		
		/* (LTE)(0x2621)運用中ファイルデータ送信完了報告応答(60:チェックサムNG) */
		m_dl_cm_CRUSDataEndRspSnd(a_rtn);
		
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "rrhApi_File_Fsm_unLoadMemory return NG(ret=%d)", a_rtn);
		
		/* 再ダウンロード準備 */
		m_dl_cm_USDlRetry();
		
#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return ;
	}
	
//	/***************************************************************************
//	* RECからの転送データ格納用領域(FHM保持REファイル情報[0])に以下の情報を保存
//	* ・メーカ識別子
//	* ・バージョン情報
//	****************************************************************************/
//
//	/* メーカ識別子 */
//	gt_dlw_mnt_reinf[CMD_NUM0].mk_jdg	|= gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM19];
//	
//	/* マイナーバージョン */
//	gt_dlw_mnt_reinf[CMD_NUM0].min_ver	|= gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM10];
//	gt_dlw_mnt_reinf[CMD_NUM0].min_ver	= (gt_dlw_mnt_reinf[CMD_NUM0].min_ver << DLD_BIT8) | gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM11];
//	
//	/* メジャーバージョン */
//	gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver	= gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM12];
//	gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver	= (gt_dlw_mnt_reinf[CMD_NUM0].mjr_ver << DLD_BIT8) | gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM13];
	
	/*------------------------------*/
	/* ファームウェア設定処理を起動 */
	/*------------------------------*/
	/* FWファイル(FHM-FW)保存API発行 */
	a_rtn = m_dl_FirmWareSet(CMD_SYS_S3G,lw_chksum,ldw_time);
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "USE fileDatSndFinNtc End");
	
	/* Save RE File Request(FHM-FW) TBD*/
	//fhmApi_Save_FW_File();
	/* Save RE File Response(FHM-FW) TBD*/
	
	/* REC主導運用中ファイルデータ送信完了報告応答 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_USE_FILEDATATRANS_FINREP_WAIT;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_ufsnd_waiting_ufwsndcmp_ntc] RETURN" );
	
	return ;
}

/* @} */

