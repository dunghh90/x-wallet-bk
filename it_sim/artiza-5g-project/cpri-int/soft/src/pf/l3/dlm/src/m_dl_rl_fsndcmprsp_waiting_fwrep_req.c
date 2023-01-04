/*!
 * @skip   $Id$
 * @file   m_dl_rl_fsndcmprsp_waiting_fwrep_req.c
 * @brief  the processing for "ファームウェアファイル報告要求" receiving.
 * @date   2015/11/18 TDIPS)sasaki Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */
#include "rrhApi_File.h"

/*!
 *  @brief  the processing for "ファームウェアファイル報告要求" receiving at the "REC主導ファイルデータ送信完了報告応答(REC←FHM) 送信待ち".
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/11/18 TDIPS)sasaki Create REC CNT-N/E切替問題対応
 */

VOID m_dl_rl_fsndcmprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_CPRISND_FIRMFILERES2*	pRsp = NULL;
	UINT 							ldwret_breq;
	UINT							ldwSnd_ret;
	USHORT							lwLoop, reLoop;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_rl_fsndcmprsp_waiting_fwrep_req] ENTER");

	/* MSG送信用のバッファを取得 */
	ldwret_breq = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRISND_FIRMFILERES2), CMD_NUM1, (VOID**)&pRsp);
	if(ldwret_breq != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
					"m_dl_rl_fsndcmprsp_waiting_fwrep_req",
					"Buffer request error",
					ldwret_breq,
					sizeof(CMT_TSKIF_CPRISND_FIRMFILERES2), 
					CMD_NUM0);
		return;
	}

	/********************************************/
	/* ファームウェアファイル報告応答2作成 		*/
	/********************************************/
	pRsp->cpridat_firmfileres2.signal_kind = CMD_CPRID_FIRMFILERES2 | CMD_SYS_S3G;
	pRsp->cpridat_firmfileres2.verinf_num = 1;
	pRsp->cpridat_firmfileres2.result = CMD_NML;

	/* ファームウェアファイル情報群 */
	pRsp->cpridat_firmfileres2.firmfile[CMD_NUM0].makername = gt_dlw_dlm_tbl.mk_jdg;
	pRsp->cpridat_firmfileres2.firmfile[CMD_NUM0].mjr_ver = gt_dlw_dlm_tbl.usmjr_ver;
	pRsp->cpridat_firmfileres2.firmfile[CMD_NUM0].mir_ver = gt_dlw_dlm_tbl.usmin_ver;

	/* Loop(RE#1～16) */
	for(lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++)
	{
		if(DLD_RESLT_NML == gt_dlw_refw_tbl[lwLoop].fw_rep_rslt)
		{
			pRsp->cpridat_firmfileres2.firmfile[pRsp->cpridat_firmfileres2.verinf_num].makername = gt_dlw_refw_tbl[lwLoop].mk_jdg;
			pRsp->cpridat_firmfileres2.firmfile[pRsp->cpridat_firmfileres2.verinf_num].mjr_ver = gt_dlw_refw_tbl[lwLoop].usmjr_ver;
			pRsp->cpridat_firmfileres2.firmfile[pRsp->cpridat_firmfileres2.verinf_num].mir_ver = gt_dlw_refw_tbl[lwLoop].usmin_ver;
			pRsp->cpridat_firmfileres2.verinf_num++;
		}
	}
	BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "fwrrsp2:Num=%d, FHM :%x=V%02d%02d", 
							pRsp->cpridat_firmfileres2.verinf_num, pRsp->cpridat_firmfileres2.firmfile[CMD_NUM0].makername, 
							pRsp->cpridat_firmfileres2.firmfile[0].mjr_ver, pRsp->cpridat_firmfileres2.firmfile[0].mir_ver);
	for(lwLoop = 0; lwLoop < 4; lwLoop++)
	{
		reLoop = lwLoop*4;
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "         RE%2d:%x=V%02d%02d(rtn=%d), RE%2d:%x=V%02d%02d(rtn=%d), RE%2d:%x=V%02d%02d(rtn=%d), RE%2d:%x=V%02d%02d(rtn=%d)", 
								reLoop+1, gt_dlw_refw_tbl[reLoop].mk_jdg, gt_dlw_refw_tbl[reLoop].mjr_ver, gt_dlw_refw_tbl[reLoop].min_ver, gt_dlw_refw_tbl[reLoop].fw_rep_rslt, 
								reLoop+2, gt_dlw_refw_tbl[reLoop+1].mk_jdg, gt_dlw_refw_tbl[reLoop+1].mjr_ver, gt_dlw_refw_tbl[reLoop+1].min_ver, gt_dlw_refw_tbl[reLoop+1].fw_rep_rslt, 
								reLoop+3, gt_dlw_refw_tbl[reLoop+2].mk_jdg, gt_dlw_refw_tbl[reLoop+2].mjr_ver, gt_dlw_refw_tbl[reLoop+2].min_ver, gt_dlw_refw_tbl[reLoop+2].fw_rep_rslt, 
								reLoop+4, gt_dlw_refw_tbl[reLoop+3].mk_jdg, gt_dlw_refw_tbl[reLoop+3].mjr_ver, gt_dlw_refw_tbl[reLoop+3].min_ver, gt_dlw_refw_tbl[reLoop+3].fw_rep_rslt);
	}

	/* タスク間ヘッダ部 */
	pRsp->head.uiLength = 
			sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) +
			CMD_NUM3 * sizeof(USHORT) + 
			pRsp->cpridat_firmfileres2.verinf_num * sizeof(FIRM_FILE_INF);

	/************************************************/
	/* (0x2131)ファームウェアファイル報告応答2送信	*/
	/************************************************/
	ldwSnd_ret = m_cm_L3MsgSend_REC(pRsp);
	if(ldwSnd_ret != CMD_L3_OK)
	{
		cm_Assert(CMD_ASL_USELOW, ldwSnd_ret, "MSG Snd NG");
	}

	return;
}
/* @} */

