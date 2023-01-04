/*!
 * @skip   $Id$
 * @file   m_dl_SysPro.c
 * @brief  the processing for file information report notification receiving and the target data is FIRM.
 * @date   2008/07/30 FFCS)Wuh Create for eNB-008-001.
 * @date   2009/04/30  FFCS)Wuh modify for CR-xxx-xxx
 *                      move the process of FLASH erasing to file data sending
 * @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) 
 * @date   2015/11/19 TDIPS)sasaki Update
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															
#include "m_dl_header.h"
/*!
 *  @brief  the processing for file information report notification receiving and the target data is FIRM.
 *  @note   This function is processed as follows.
 *          1) Judge whether the file information suit the firmware setting condition
 *          2) table setting
 *  @param  a_rcvmsg_p		 	[in]  pointer of receving message address
 *  @param  a_sys_typ		 	[in]  the type of system,3G or S3G
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01862
 *  @date   2008/07/30 FFCS)Wuh Create.
 *  @date   2008/08/05 FFCS)Wuh Miss the parameter of function m_dl_cm_DlEnd.
 *  @date   2008/02/02 FFCS)Wuh M-S3G-eNBPF-01256
 *  @date   2015/10/12 TDIPS)Lay M-RRU-ZSYS-01862 Update value for 2 fields of variable 
 *  @date   2013/11/15 FFCS)linb Modify for DCM Zynq
 *  @date   2015/11/19 TDIPS)sasaki Update REC CNT-N/E切替問題対応
 */
VOID m_dl_SysPro(VOID *a_rcvmsg_p,USHORT a_sys_typ)
{
	USHORT	a_data		= CMD_NUM0;					/* 対象データ			*/
	UINT	a_size		= CMD_NUM0;					/* トータルサイズ		*/
	UINT	a_rtn __attribute__((unused))		= CMD_RES_OK;				/* 戻り値格納変数		*/
//	UINT	a_timid		= CMD_NUM0;					/* タイマID格納変数		*/
//	USHORT	a_tim_val	= CMD_NUM0;					/* タイマ値格納変数		*/
//	USHORT	a_pri_typ	= CMD_NUM0; 				/* primary system */
	CMT_TSKIF_CPRIRCV_FILEINFREQ *a_cpridat_p = NULL;/* CPRI信号データ部	*/
//	INT		errcd;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_SysPro] ENTER" );

	/******************/
	/* データ取得処理 */
	/******************/
	/* CPRI信号データ部取得 */
	a_cpridat_p = (CMT_TSKIF_CPRIRCV_FILEINFREQ*)a_rcvmsg_p;
	/* 対象データを取得 */
	a_data = a_cpridat_p->cpridat_fileinfreq.data;
	/* トータルデータサイズを取得 */
	a_size = a_cpridat_p->cpridat_fileinfreq.size;
#if 0
	/* read current valid system */
	a_pri_typ = cmw_sys_mng_tbl[a_sys_typ].pri_sys_info;

	/* ファイル情報報告テーブルのENフラグがONのとき */
	if(cmw_file_inf_tbl[a_sys_typ].en_flg == CMD_ON)
	{
		/* change the download type to NONE */
		if(dlw_downtyp_tbl != DLD_TSK_TYP_NONE) 
		{
			/* if connect type is not CPRI share */
			if(cmw_connectinfo != CMD_CONN_CPRI_SHARED)
			{
				/* set the download type to none */
				dlw_downtyp_tbl = DLD_TSK_TYP_NONE;
			}
			/* connect type is CPRI share */
			else
			{
				if(a_pri_typ == a_sys_typ)
				{
					/* set the download type to none */
					dlw_downtyp_tbl = DLD_TSK_TYP_NONE;
				}
			}
		}
		
		/* retry systemparameter download again */
		/******************/
		/* タイマ停止処理 */
		/******************/
		/* タイマ管理テーブルのタイマ起動情報がタイマ起動中のとき */
		if(dlw_timmng_tbl[a_sys_typ] != CMD_NUM0)
		{
			/* タイマ停止処理 */
			a_rtn = cm_TStop(dlw_timmng_tbl[a_sys_typ],&errcd);
			cm_Assert( CMD_ASL_DBGLOW, dlw_timmng_tbl[a_sys_typ],	"[m_dl_SysPro] Timer Stop" );										
		}
			
		/* 現在ダウンロード中のものを強制終了する */
		m_dl_cm_DlEnd(a_sys_typ);

		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "RETRY SYSDOWN(data=%d<-%d)", a_data, cmw_file_inf_tbl[a_sys_typ].target_data);
	}
#endif


	if(a_size != dlr_syssize_settbl[a_sys_typ])
	{
		/********************/
		/* CPRI信号送信処理 */
		/********************/
		/* 情報報告NGでファイル情報報告応答送信処理起動 */
		if(a_sys_typ == CMD_SYS_3G)
		{
			m_dl_cm_CRFileInfRspSnd(CMD_INFRPT_NG,a_sys_typ);	   
		}
		else
		{
			m_dl_cm_CRFileInfRspSnd(CMD_HDL_NG,a_sys_typ);		
		}
			
		/* アサート出力 */
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "RCV TOTALSIZE NG(date=%d/size=%d<->%d)", a_data, a_size, dlr_syssize_settbl[a_sys_typ]);

		/* set the ERR control for file information report notification ON */
		if(a_sys_typ == CMD_SYS_3G)
		{
			m_dl_cm_ErrCtl(DLD_ERR_INF_ON);
		}
		else
		{
			m_dl_cm_ErrCtl(DLD_ERR_INF_S3G_ON);
		}
		return ;
	}
#if 0
	/********************/
	/* テーブル設定処理 */
	/********************/
	/*==========================*/
	/* ファイル情報報告テーブル */
	/*==========================*/
	/* ファイル情報報告テーブルのENフラグ設定 */
	cmw_file_inf_tbl[a_sys_typ].en_flg = CMD_ON;
	
	/* ファイル情報報告テーブルの対象データ設定 */
	cmw_file_inf_tbl[a_sys_typ].target_data = a_data;

	/*==========================*/
	/* ダウンロード管理テーブル */
	/*==========================*/
	/* ダウンロード管理テーブルのトータルデータサイズ設定 */
	dlw_dlmng_tbl[a_sys_typ].total_size = a_size;
	
	/* ファイル情報報告テーブルの分割数に1を固定で設定 */
	cmw_file_inf_tbl[a_sys_typ].division_num = CMD_NUM1;
	
	/* ファイルデータ送信完了報告応答のNG要因領域の初期化 */
	cmw_file_inf_tbl[a_sys_typ].ng_fct = CMD_NUM0;
#endif

	gt_dlw_mnt_all_reinf[a_sys_typ][CMD_NUM0].div_num = 1;
	gt_dlw_mnt_all_reinf[a_sys_typ][CMD_NUM0].file_size = a_size;

	gt_dlw_rec_all_tbl[a_sys_typ]->rcv_div_no = 0;
	gt_dlw_rec_all_tbl[a_sys_typ]->rcv_data_size = 0;

//	/* ダウンロード管理テーブルのトータルデータサイズ設定 */
//	gt_dlw_mnt_reinf[CMD_NUM0].file_size = ((CMT_TSKIF_CPRIRCV_FILEINFREQ*)a_rcvmsg_p)->cpridat_fileinfreq.size;

	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(a_rcvmsg_p, DLD_ALL_RE);

	/* タイマ起動 システムパラメータ情報報告確認タイマ */
	/* all Re start 1 timer -> fix CpriNo = 1 */
	m_dl_fhm_sysp_fileinfo_timersta( CMD_NUM1, a_sys_typ );

	/* 対REC状態をREC主導ファイル情報報告応答(シスパラ)集約中へ遷移 */
	(*gt_dlw_rec_all_tbl[a_sys_typ]).rec_sta = EC_DLM_MAIN_STN_FILEINF_REP_SYSPARA_CON;

	cm_Assert( CMD_ASL_RETURN, CMD_NUM0,	"[m_dl_SysPro] RETURN" );
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return;
}

/* @} */


