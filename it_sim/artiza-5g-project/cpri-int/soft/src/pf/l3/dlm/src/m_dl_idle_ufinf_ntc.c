/*!
 * @skip   $Id$
 * @file   m_dl_idle_ufinf_ntc.c
 * @brief  the processing for "運用中ファイル情報報告通知" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/09/29 TDIPS)sasaki Update
 * @date   2015/10/02 TDIPS)sasaki Update
 * @date   2015/10/17 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update
 * @date   2015/11/25 TDIPS)sasaki Update

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
 *  @brief  the processing for "運用中ファイル情報報告通知" receiving at the アイドル REC state.
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01846
 *  @Bug_No M-RRU-ZSYS-01847
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/09/22 FPT)DuongCD M-RRU-ZSYS-01846 IT update change to: gt_dlw_dlm_tbl.dlstart_factor = DLD_FACTOR_REC; 
 *  @date   2015/09/22 FPT)DuongCD M-RRU-ZSYS-01847 change to: a_timid = CMD_TIMID_USEFIRMWARESND;   
 *  @date   2015/09/29 TDIPS)sasaki Update MKレビュー指摘No.160対応
 *  @date   2015/10/02 TDIPS)sasaki Update MKレビュー指摘No.223,224,225対応
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.227対応
 */
VOID m_dl_idle_ufinf_ntc(VOID* bufp, USHORT cpri_no)
{
	USHORT							lw_data	= CMD_NUM0;					/* 対象データ			*/
	CMT_TSKIF_CPRIRCV_UFILEINFREQ*	a_cpridat_p;
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_idle_ufinf_ntc] ENTER" );
	
	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_UFILEINFREQ*)bufp;
	
	/* 対象データを取得 */
	lw_data = a_cpridat_p->cpridat_ufileinfreq.data;
	
	/* 対象データがファームならファイル情報報告応答(正常)を送信 */
	if(CMD_FRM == lw_data)
	{
		m_dl_UFimPro(bufp);
	}
	
	/* 対象データがシスパラ、ファームウェア以外のとき	*/
	else if(lw_data != CMD_FRM)
	{
		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "PARAMETER NG(date=%d)", lw_data);
		
		/* (LTE)(0x2521)運用中ファイル情報報告応答(51:処理NG(RE)) */
		m_dl_cm_CRUSFileInfRspSnd(CMD_HDL_NG);
		
		/* 1: アイドル */
		gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;

	}
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_idle_ufinf_ntc] RETURN" );
	
	return ;
}
/*!
 *  @brief  the processing for "運用中ファイル情報報告通知" receiving at the アイドル REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01791
 *  @date   2015/10/08 FPT)Duong Create ( MKレビュー指摘No.227対応)
 *  @date   2015/10/17 TDIPS)sasaki M-RRU-ZSYS-01791 Update IT2問処No.96対処 配下への転送不要と判明した時点の DL開始要因初期化処理を削除 全配下REからOK応答なしのルートで 応答結果223を返すよう修正
 *  @date   2015/10/20 TDIPS)sasaki Update ハソ-QA-031対応
 *  @date   2015/11/25 TDIPS)sasaki Update IT3問題水平展開(DL中のCPRIリンク断にて、DLが2回行われる)
 *  @date   2015/11/25 TDIPS)sasaki Update IT2問処No.195対処
 *  @date   2017/08/22 FJT)ohashi   17A(3201-16B-TS92)対応 RE DL用報告結果情報全初期化処理追加
 */
VOID m_dl_UFimPro(VOID *bufp)
{
	USHORT							a_data			= CMD_NUM0;					/* 対象データ			*/
	UINT							a_size			= CMD_NUM0;					/* トータルサイズ		*/
	UINT							a_timid			= CMD_NUM0;					/* タイマID格納変数		*/
	USHORT							a_tim_val		= CMD_NUM0;					/* タイマ値格納変数		*/
	CMT_TSKIF_CPRIRCV_UFILEINFREQ	*a_cpridat_p	= NULL;						/* CPRI信号データ部	*/
	UINT							a_rtn __attribute__((unused)) = CMD_RES_OK; /* 戻り値格納変数	*/
	INT								ret				= CMD_OK;
	INT								errcd;
	USHORT							lwNamechk;
	USHORT							lwLoop;

	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_UFILEINFREQ*)bufp;

	/* Processing for file information report notification receiving and the target data is FRM. */
	/* 対象データを取得 */
	a_data = a_cpridat_p->cpridat_ufileinfreq.data;
	/* トータルデータサイズを取得 */
	a_size = a_cpridat_p->cpridat_ufileinfreq.size;
	
	/* if the RE file size comes from S3G connection,the size's range is 0~16M */
	/* ファイルサイズ範囲外 */
	if( ( a_size > DLD_RCV_S3G_NUM ) || ( a_size == CMD_NUM0 ))
	{
		/* (LTE)(0x2521)運用中ファイル情報報告応答(51:処理NG(RE)) */
		m_dl_cm_CRUSFileInfRspSnd(CMD_HDL_NG);
			
		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "RCV TOTALSIZE NG(date=%d,size=%d)", a_data, a_size);

#ifdef FHM_DLM_FOR_DEBUG_IT1
printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return ;
	}

	/* すでにファイル送信完了通知受信済でFLASH 書き込み中*/
	/* フラッシュ書込フラグがON */
	if(CMD_ON == gt_dlw_dlm_tbl.write_flg)
	{
		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "FLASH WRITE NO CMP(flg=%d)", gt_dlw_dlm_tbl.write_flg);
		
		/* (LTE)(0x2521)運用中ファイル情報報告応答(51:処理NG(RE)) */
		m_dl_cm_CRUSFileInfRspSnd(CMD_HDL_NG);
		
		/* set the ERR control for file information report notification ON */
		//m_dl_cm_ErrCtl(DLD_ERR_INF_S3G_ON);
		
#ifdef FHM_DLM_FOR_DEBUG_IT1
printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/* check file name */
	lwNamechk = m_dl_cm_chkfilname(a_cpridat_p->cpridat_ufileinfreq.filename);
	/* ファイル名異常 */
	if( DLD_FILENAME_INVALID == lwNamechk)
	{
		/* (LTE)(0x2521)運用中ファイル情報報告応答(51:処理NG(RE)) */
		m_dl_cm_CRUSFileInfRspSnd(CMD_HDL_NG);

		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "RCV S3GMAKERNAME INF NG(filename=%s)", a_cpridat_p->cpridat_ufileinfreq.filename);

		/* set the ERR control for file information report notification ON */
		//m_dl_cm_ErrCtl(DLD_ERR_INF_S3G_ON);

#ifdef FHM_DLM_FOR_DEBUG_IT1
printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return ;
	}

	/*******************************************************************************/
	/* RECからの転送データ格納用領域(FHM保持REファイル情報[0])に以下の情報を保存   */
	/*******************************************************************************/

	/* ファイル名 */
	cm_MemCpy(&gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM0], 
				&a_cpridat_p->cpridat_ufileinfreq.filename[CMD_NUM0], 
				sizeof(a_cpridat_p->cpridat_ufileinfreq.filename));

	/* ファイルサイズ */
	gt_dlw_mnt_reinf[CMD_NUM0].file_size = a_size;

	/* 分割数設定 */
	gt_dlw_mnt_reinf[CMD_NUM0].div_num = (a_size + CMD_FILEDAT_MAX - 1) / CMD_FILEDAT_MAX;

	/* 受信済みトータルデータサイズ初期化 */
	gt_dlw_rec_tbl.rcv_div_no = CMD_NUM0;

	/* 受信済み分割数初期化 */
	gt_dlw_rec_tbl.rcv_data_size = CMD_NUM0;
	
	/*実行中DL開始要因に1:RECを設定 */
	gt_dlw_dlm_tbl.dlstart_factor = DLD_FACTOR_REC;

	/* (LTE)(0x2521)運用中ファイル情報報告応答(OK) */
	m_dl_cm_CRUSFileInfRspSnd(CMD_NML);

	/* REC向けファイルデータ送信完了報告応答結果設定 223:全配下RE応答NG */
	gt_dlw_rec_tbl.filedata_trans_rslt = CMD_ALLRE_NG;

	/* RE DL用報告結果情報全初期化 TS92対応 */
	if (DLD_FILENAME_RE == gw_dl_FileType)
	{
		for (lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++)
		{
			gt_dlw_refw_tbl[lwLoop].trx_div_no = 0;
			gt_dlw_refw_tbl[lwLoop].trx_data_size = 0;
			gt_dlw_refw_tbl[lwLoop].fileinf_rep_rslt = DLD_RESLT_IDLE;
			gt_dlw_refw_tbl[lwLoop].filedata_trans_rslt = DLD_RESLT_IDLE;
			gt_dlw_refw_tbl[lwLoop].fileinf_rep_tc = 0;
			gt_dlw_refw_tbl[lwLoop].filedata_trans_tc = 0;
		}
	}

	/* REC主導運用中ファイルデータ転送先決定待ち */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_USE_FILEDATATRANS_DES_WAIT;

	/* LED設定処理 */
	m_cm_WriteLEDReg();

	/* タイマIDにファームウェア送信確認タイマIDを設定 */
	a_timid = CMD_TIMID_USEFIRMWARESND;

	/* タイマ値設定 */
	a_tim_val = cmw_sys_mng_tbl[CMD_SYS_S3G].firm_sndtimer;

	/* タイマ管理テーブルのタイマ起動情報を設定 */
	gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_SND_UPDATE;

	/* ファームウェア送信確認(更新用)タイマ開始 */
	a_rtn = cm_TStat(a_timid, TIMVAL_GET(a_tim_val), CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC,CMD_TSKID_DLM,&errcd);

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"[FimPro] Timer Start(ID=%d, value=%d)",
						a_timid, a_tim_val);

	/* save the handle of shared memory */
	ret = rrhApi_File_Fsm_loadMemory((INT *)&dlw_handle);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_USELOW, ret, "rrhApi_File_Fsm_loadMemory return NG");
#ifdef FHM_DLM_FOR_DEBUG_IT1
printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	if(DLD_FILENAME_FHM == gw_dl_FileType)
	{
		/* send erasing flash request to pf_fmng */
		/* FWファイル削除API発行 */
		m_dl_cm_FlashProSnd(CMD_SYS_S3G,CMD_ERASE_TYP,CMD_USE_TYP);
		
	}
	/* transfer the download type to download in USE state */
	dlw_downtyp_tbl = DLD_TSK_TYP_USE;

}
/* @} */

