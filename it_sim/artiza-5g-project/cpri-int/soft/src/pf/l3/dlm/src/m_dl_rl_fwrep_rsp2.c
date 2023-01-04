/*!
 * @skip   $Id$
 * @file   m_dl_rl_fwrep_rsp2.c
 * @brief  the processing for "ファームウェアファイル報告応答" receiving.
 * @date   2015/08/18 FPT)DuongCD Create
 * @date   2015/09/28 TDIPS)sasaki Update
 * @date   2015/10/27 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファームウェアファイル報告応答" receiving at the ファームウェア報告応答集約中 REC state.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @Bug_No M-RRU-ZSYS-01914
 *  @date   2015/08/18 FPT)DuongCD Create
 *  @date   2015/09/28 TDIPS)sasaki Update MKレビュー指摘No.220, No221対応
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.214対応
 *  @date   2015/10/09 FPT)Yen update メッセージサイズ
 *  @date   2015/10/27 TDIPS)sasaki M-RRU-ZSYS-01914 Update IT2問処No.147水平展開
 *  @ A_function
 */
VOID m_dl_rl_fwrep_rsp2(VOID* bufp, USHORT cpri_no)
{
	UINT 								ldwret_breq;						/* Return value of buffer request */
	CMT_TSKIF_CPRISND_FIRMFILERES2 		*lt_firmfile_p	= NULL;
	USHORT								lwLoop, reLoop;
	UINT								ldwSnd_ret;
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
	#endif
	
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_rl_fwrep_rsp2] ENTER" );
	
	/* 全てのREをループする (16 RE)*/
	for( lwLoop = CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		if(DLD_RESTA_IDLE != gt_dlw_refw_tbl[lwLoop ].re_sta)
		{
			#ifdef FHM_DLM_FOR_DEBUG_IT1
			printf( "[%d]%s re_sta[%d]: [%d] \n",__LINE__,__FUNCTION__, lwLoop , gt_dlw_refw_tbl[lwLoop ].re_sta);
			
			printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
			#endif
			return;
		}
	}
	
	/* 対REC状態をアイドルへ遷移 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_IDLE;
	
	/************************/
	/* 送信バッファ取得処理 */
	/************************/
	/* MSG送信用のバッファを取得 */
	ldwret_breq = CMD_NUM0;
	ldwret_breq = cm_L2BfGet( CMD_BUFCA_LAPSND, 
						   sizeof( CMT_TSKIF_CPRISND_FIRMFILERES2 ), 
						   CMD_NUM1, 
						   (VOID**)&lt_firmfile_p );
	
	/* If buffer request NG		*/
	if(ldwret_breq != CMD_RES_OK)
	{
		cm_MAbort(	CMD_ALMCD_BUFGET, 
					"m_ex_ExtDataRcv", 
					"Buffer request error", 
					ldwret_breq, 
					sizeof(CMT_TSKIF_CPRISND_FIRMFILERES2), 
					CMD_NUM0);
		
		#ifdef FHM_DLM_FOR_DEBUG_IT1
		printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/********************************************/
	/* ファームウェアファイル報告応答2作成 		*/
	/********************************************/
	/* CPRI信号データ部 */
	 /*-----the S3G CPRI signal-----*/
	lt_firmfile_p->cpridat_firmfileres2.signal_kind = CMD_CPRID_FIRMFILERES2 | CMD_SYS_S3G;

	lt_firmfile_p->cpridat_firmfileres2.verinf_num = 1;

	/* 応答結果 */
	lt_firmfile_p->cpridat_firmfileres2.result = CMD_NML;
	
	/* ファームウェアファイル情報群	 */
	lt_firmfile_p->cpridat_firmfileres2.firmfile[CMD_NUM0].makername 	= gt_dlw_dlm_tbl.mk_jdg;
	lt_firmfile_p->cpridat_firmfileres2.firmfile[CMD_NUM0].mjr_ver		= gt_dlw_dlm_tbl.usmjr_ver;
	lt_firmfile_p->cpridat_firmfileres2.firmfile[CMD_NUM0].mir_ver 		= gt_dlw_dlm_tbl.usmin_ver;
	
	/* Loop(RE#1～16) */
	for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		if(DLD_RESLT_NML == gt_dlw_refw_tbl[lwLoop].fw_rep_rslt)
		{
			/* 応答結果 */
			lt_firmfile_p->cpridat_firmfileres2.firmfile[lt_firmfile_p->cpridat_firmfileres2.verinf_num].makername = gt_dlw_refw_tbl[lwLoop].mk_jdg;
			lt_firmfile_p->cpridat_firmfileres2.firmfile[lt_firmfile_p->cpridat_firmfileres2.verinf_num].mjr_ver = gt_dlw_refw_tbl[lwLoop].usmjr_ver;
			lt_firmfile_p->cpridat_firmfileres2.firmfile[lt_firmfile_p->cpridat_firmfileres2.verinf_num].mir_ver = gt_dlw_refw_tbl[lwLoop].usmin_ver;
			
			lt_firmfile_p->cpridat_firmfileres2.verinf_num++;
		}
	}
	BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "fwrrsp2:Num=%d, FHM :%x=V%02d%02d", 
							lt_firmfile_p->cpridat_firmfileres2.verinf_num, lt_firmfile_p->cpridat_firmfileres2.firmfile[CMD_NUM0].makername, 
							lt_firmfile_p->cpridat_firmfileres2.firmfile[0].mjr_ver, lt_firmfile_p->cpridat_firmfileres2.firmfile[0].mir_ver);
	for(lwLoop = 0; lwLoop < 4; lwLoop++)
	{
		reLoop = lwLoop*4;
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "         RE%2d:%x=V%02d%02d(rtn=%d), RE%2d:%x=V%02d%02d(rtn=%d), RE%2d:%x=V%02d%02d(rtn=%d), RE%2d:%x=V%02d%02d(rtn=%d)", 
								reLoop+1, gt_dlw_refw_tbl[reLoop].mk_jdg, gt_dlw_refw_tbl[reLoop].usmjr_ver, gt_dlw_refw_tbl[reLoop].usmin_ver, gt_dlw_refw_tbl[reLoop].fw_rep_rslt, 
								reLoop+2, gt_dlw_refw_tbl[reLoop+1].mk_jdg, gt_dlw_refw_tbl[reLoop+1].usmjr_ver, gt_dlw_refw_tbl[reLoop+1].usmin_ver, gt_dlw_refw_tbl[reLoop+1].fw_rep_rslt, 
								reLoop+3, gt_dlw_refw_tbl[reLoop+2].mk_jdg, gt_dlw_refw_tbl[reLoop+2].usmjr_ver, gt_dlw_refw_tbl[reLoop+2].usmin_ver, gt_dlw_refw_tbl[reLoop+2].fw_rep_rslt, 
								reLoop+4, gt_dlw_refw_tbl[reLoop+3].mk_jdg, gt_dlw_refw_tbl[reLoop+3].usmjr_ver, gt_dlw_refw_tbl[reLoop+3].usmin_ver, gt_dlw_refw_tbl[reLoop+3].fw_rep_rslt);
	}
	/* タスク間ヘッダ部 */
	lt_firmfile_p->head.uiLength = 
			sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_CPRISIGSND_INF ) +
			CMD_NUM3 * sizeof(USHORT) + 
			lt_firmfile_p->cpridat_firmfileres2.verinf_num * sizeof( FIRM_FILE_INF );
	/******************************************************/
	/* (LTE)(0x2131)ファームウェアファイル報告応答2 */
	/******************************************************/
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s verinf_num[%d]\n, result[%d]\n, firmfile[CMD_NUM0].makername[%d]\n, firmfile[CMD_NUM0].mjr_ver[%d]\n, firmfile[CMD_NUM0].mir_ver[%d] \n",__LINE__,__FUNCTION__,
			lt_firmfile_p->cpridat_firmfileres2.verinf_num,
			lt_firmfile_p->cpridat_firmfileres2.result,
			lt_firmfile_p->cpridat_firmfileres2.firmfile[CMD_NUM0].makername,
			lt_firmfile_p->cpridat_firmfileres2.firmfile[CMD_NUM0].mjr_ver,
			lt_firmfile_p->cpridat_firmfileres2.firmfile[CMD_NUM0].mir_ver);
	#endif
	
	ldwSnd_ret = m_cm_L3MsgSend_REC( lt_firmfile_p );
	
	if(ldwSnd_ret != CMD_L3_OK)
	{
		cm_Assert(CMD_ASL_USELOW, CMD_NUM1, 
								"m_dl_rl_fwrep_rsp2 MSGSndNG");
	}
	
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return ;
}

/* @} */

