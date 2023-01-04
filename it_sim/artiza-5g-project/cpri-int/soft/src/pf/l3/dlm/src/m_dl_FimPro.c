/*!
 * @skip   $Id$
 * @file   m_dl_FimPro.c
 * @brief  the processing for file information report notification receiving and the target data is FRM.
 * @date   2008/07/30 FFCS)Wuh Create for eNB-008-001.
 * @date   2008/08/05 FFCS)Wuh Modify the judgement of the size for 3G,S3G.
 * @date   2009/04/13  FFCS)Wuh modify for S3G PRC CR-xxx-xxx (CPRI spec V1.07)
 * @date   2009/04/30  FFCS)Wuh modify for CR-xxx-xxx
 *                      move the process of FLASH erasing to file data sending
 * @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) 
 * @date   2010/05/06  FJT)Otani  CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 * @date   2011/02/08  FJT)Tokunaga  CeNB-F-062-009(DCM) RE用FWファイルのメーカ識別子について
 * @date   2015/09/29  TDIPS)sasaki Update 
 * @date   2015/10/17 TDIPS)sasaki Update
 * @date   2015/10/20 TDIPS)sasaki Update
 * @date   2015/11/25 TDIPS)sasaki Update
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */
					
#include "m_cm_header.h"			            /* MDIF common head file			*/												
#include "m_dl_header.h"                        /* download management task head file */

/*!
 *  @brief  the processing for checking file name .
 *  @note   
 *  @param  a_file_name		 	[in]  file name
 *  @retval -
 *  @date   2015/08/13 FPT)DuongCD Create.
 *  @date   2021/03/01 M&C)Tri.hn Update based on No.27 (seq 7-1) - Add checking file name for Intermediate Deivce
 */
USHORT m_dl_cm_chkfilname(VOID* a_file_name )
{
	UCHAR                   lwfile_name_assert[20] = "RE********E*****FE*";
	UCHAR					lwfile_name[20];
	USHORT					lwLoop;
	USHORT					lwNamechk;
	
	
	if (a_file_name == NULL)
	{
		return DLD_FILENAME_INVALID;
	}
	/* ファイル名 */
	cm_MemCpy(&lwfile_name[CMD_NUM0], a_file_name, sizeof(lwfile_name));
	
	/* ファイル名異常 */
	for( lwLoop = 0; lwLoop < CMD_NUM20; lwLoop++ )
	{
		if (lwfile_name_assert[lwLoop] == '*')
		{
			continue;
		}
		else if(lwfile_name_assert[lwLoop]  != lwfile_name[lwLoop])
		{
			lwNamechk = DLD_ERR_INF_S3G_ON;
			break;
		}
	}
	/* Invalid file name */
	if (DLD_ERR_INF_S3G_ON == lwNamechk)
	{
		lwNamechk = DLD_FILENAME_INVALID;
	}
	/* File name: RE___FHM_EMMmm.FEX */
	else if ((lwfile_name[CMD_NUM6] == 'F') && (lwfile_name[CMD_NUM7] == 'H') && (lwfile_name[CMD_NUM8] == 'M'))
	{
		/* FHM file */
		lwNamechk = DLD_FILENAME_FHM;
		gw_dl_FileType = DLD_FILENAME_FHM;
	}
	/* File name: RE___FHIC_EMMmm.FEX */
	else if ((lwfile_name[CMD_NUM6] == 'F') && (lwfile_name[CMD_NUM7] == 'H') && (lwfile_name[CMD_NUM8] == 'I') && (lwfile_name[CMD_NUM9] == 'C'))
	{
		/* Intermediate device file */
		lwNamechk = DLD_FILENAME_INTDEV;
		gw_dl_FileType = DLD_FILENAME_INTDEV;
	}
	/* File name: RE_______EMMmm.FEX */
	else
	{
		/* RE file */
		lwNamechk = DLD_FILENAME_RE;
		gw_dl_FileType = DLD_FILENAME_RE;
	}

	return  lwNamechk;
}

/*!
 *  @brief  the processing for file information report notification receiving and the target data is FRM.
 *  @note   This function is processed as follows.
 *          1) Judge whether the file information suit the systemparameter setting condition
 *          2) table setting
 *  @param  bufp		 	[in]  pointer of receving message address
 *  @param  cpri_no		 	[in]  Cpri No
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01791
 *  @date   2008/07/30 FFCS)Wuh Create.
 *  @date   2010/05/06  FJT)Otani  CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *  @date   2011/02/08  FJT)Tokunaga  CeNB-F-062-009(DCM) RE用FWファイルのメーカ識別子について
 *  @date   2015/08/18 FPT)DuongCD Update for FHM
 *  @date   2015/09/29 TDIPS)sasaki Update MKレビュー指摘No.160対応
 *  @date   2015/10/17 TDIPS)sasaki M-RRU-ZSYS-01791 Update IT2問処No.96対処 配下への転送不要と判明した時点の DL開始要因初期化処理を削除 全配下REからOK応答なしのルートで 応答結果223を返すよう修正
 *  @date   2015/10/20 TDIPS)sasaki Update ハソ-QA-031対応
 *  @date   2015/11/25 TDIPS)sasaki Update IT3問題対処(DL中のCPRIリンク断にて、DLが2回行われる)
 *  @date   2015/11/25 TDIPS)sasaki Update IT2問処No.195対処
 *  @date   2017/08/22 FJT)ohashi   17A(3201-16B-TS92)対応 RE DL用報告結果情報全初期化処理追加
 */
VOID m_dl_FimPro(VOID *bufp, USHORT cpri_no)
{
	USHORT							a_data			= CMD_NUM0;					/* 対象データ			*/
	UINT							a_size			= CMD_NUM0;					/* トータルサイズ		*/
	UINT							a_timid			= CMD_NUM0;					/* タイマID格納変数		*/
	USHORT							a_tim_val		= CMD_NUM0;					/* タイマ値格納変数		*/
	CMT_TSKIF_CPRIRCV_FILEINFREQ	*a_cpridat_p	= NULL;						/* CPRI信号データ部	*/
	UINT							a_rtn __attribute__((unused)) = CMD_RES_OK; /* 戻り値格納変数	*/
	USHORT							lwNamechk;
	INT								errcd;
	USHORT							lwLoop;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	
	printf( "[%d]%s a_data[%d], a_size[%d] \n",__LINE__,__FUNCTION__, a_data, a_size);
	#endif
		
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_FimPro] ENTER" );			

	/* Processing for file information report notification receiving and the target data is FRM. */
	/******************/
	/* データ取得処理 */
	/******************/
	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_FILEINFREQ*)bufp;
	/* 対象データを取得 */
	a_data = a_cpridat_p->cpridat_fileinfreq.data;
	/* トータルデータサイズを取得 */
	a_size = a_cpridat_p->cpridat_fileinfreq.size;

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s a_data[%d], a_size[%d] \n",__LINE__,__FUNCTION__, a_data, a_size);
	#endif
	/* if the target data is CMD_FRM judge the primary system and CPRI signal is the same type */
	/* read current valid system */
	//a_pri_typ = cmw_sys_mng_tbl[CMD_SYS_S3G].pri_sys_info;
	
	/* if the RE file size comes from S3G connection,the size's range is 0~16M */
	/* ファイルサイズ範囲外 */
	if( ( a_size > DLD_RCV_S3G_NUM ) || ( a_size == CMD_NUM0 ))
	{
		/* 情報報告NGでファイル情報報告応答送信処理起動 */
		m_dl_cm_CRFileInfRspSnd(CMD_HDL_NG,CMD_SYS_S3G);
			
		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "RCV TOTALSIZE NG(data=%d,sys=%d)", a_data, a_size);

		return ;
	}

	/* すでにファイル送信完了通知受信済でFLASH 書き込み中*/
	/* フラッシュ書込フラグがON */
	if(CMD_ON == gt_dlw_dlm_tbl.write_flg)
	{
		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR,"FLASH WRITE NO CMP(flg=%d)", gt_dlw_dlm_tbl.write_flg);
		
		/* 情報報告NGでファイル情報報告応答送信処理起動 */
		m_dl_cm_CRFileInfRspSnd(CMD_HDL_NG, CMD_SYS_S3G);
		
		/* set the ERR control for file information report notification ON */
		//m_dl_cm_ErrCtl(DLD_ERR_INF_S3G_ON);
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* check file name */
	lwNamechk = m_dl_cm_chkfilname(a_cpridat_p->cpridat_fileinfreq.filename);
	
	/* ファイル名異常 */
	if( DLD_FILENAME_INVALID == lwNamechk)
	{
		/* メーカ名NGでファイル情報報告応答送信処理起動 */
		m_dl_cm_CRFileInfRspSnd(CMD_HDL_NG,CMD_SYS_S3G);

		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "RCV S3GMAKERNAME INF NG(filenane=%s)", a_cpridat_p->cpridat_fileinfreq.filename);

		/* set the ERR control for file information report notification ON */
		m_dl_cm_ErrCtl(DLD_ERR_INF_S3G_ON);
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return ;
	}

	/*******************************************************************************/
	/* RECからの転送データ格納用領域(FHM保持REファイル情報[0])に以下の情報を保存   */
	/*******************************************************************************/
	
	/* ファイル名 */
	cm_MemCpy(&gt_dlw_mnt_reinf[CMD_NUM0].file_name[CMD_NUM0], 
				&a_cpridat_p->cpridat_fileinfreq.filename[CMD_NUM0], 
				sizeof(a_cpridat_p->cpridat_fileinfreq.filename));
	
	/* ファイルサイズ */
	gt_dlw_mnt_reinf[CMD_NUM0].file_size = a_size;

	/* 分割数設定 */
	gt_dlw_mnt_reinf[CMD_NUM0].div_num = (a_size + CMD_FILEDAT_MAX - 1) / CMD_FILEDAT_MAX;

	/* save the handle of shared memory */
	a_rtn = rrhApi_File_Fsm_loadMemory((INT *)&dlw_handle);
	if (a_rtn != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_USELOW, a_rtn, "rrhApi_File_Fsm_loadMemory return NG");
		return;
	}

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s dlw_handle = %16llx\n",__LINE__,__FUNCTION__, dlw_handle);
	#endif

	/* 受信済みトータルデータサイズ初期化 */
	gt_dlw_rec_tbl.rcv_div_no = CMD_NUM0;

	/* 受信済み分割数初期化 */
	gt_dlw_rec_tbl.rcv_data_size = CMD_NUM0;
	
	/* 実行中DL開始要因に1:RECを設定 */
	gt_dlw_dlm_tbl.dlstart_factor = DLD_FACTOR_REC;
	
	/* 正常でファイル情報報告応答送信処理起動 */
	m_dl_cm_CRFileInfRspSnd(CMD_NML, CMD_SYS_S3G);

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

	/* 対REC状態をファイルデータ転送先決定待ちへ遷移 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_FILEDATATRANS_DES_WAIT;
	
	/* ファームウェア送信確認タイマ */
	gt_dlw_rec_tbl.use_timer = DLD_RECRETIM_SND;

	/* LED設定処理 */
	m_cm_WriteLEDReg();

	/* タイマIDにファームウェア送信確認タイマIDを設定 */
	a_timid = CMD_TIMID_FIRMWARESND_S3G;   /* time id for S3G */
	
	/* タイマ値設定 */
	a_tim_val = cmw_sys_mng_tbl[CMD_SYS_S3G].firm_timer;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s a_tim_val[%d] \n",__LINE__,__FUNCTION__, TIMVAL_GET(a_tim_val));
	#endif
	/* ファームウェア送信確認タイマ開始*/
	a_rtn = cm_TStat(a_timid, TIMVAL_GET(a_tim_val), CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC,CMD_TSKID_DLM,&errcd);

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"[FimPro] Timer Start(ID=%d, value=%d)",
						a_timid, a_tim_val);

	if (DLD_FILENAME_FHM == lwNamechk)
	{
		/* send erasing flash request to pf_fmng */
		/* FWファイル削除API発行 */
		m_dl_cm_FlashProSnd(CMD_SYS_S3G,CMD_ERASE_TYP,CMD_STAT_TYP);

		/* transfer the download type to download in startup state */
		dlw_downtyp_tbl = DLD_TSK_TYP_STUP;

		cm_Assert( CMD_ASL_RETURN, CMD_NUM0, "[m_dl_FimPro] RETURN" );
	}

	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s div_num[%d], file_size[%d] \n",__LINE__,__FUNCTION__, gt_dlw_mnt_reinf[CMD_NUM0].div_num, gt_dlw_mnt_reinf[CMD_NUM0].file_size);
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
}

/* @} */


