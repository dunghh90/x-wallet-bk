/*!
 * @skip   $Id$
 * @file   m_dl_fl_fwrep_req.c
 * @brief  the processing for " ファームウェアファイル報告要求" receiving.
 * @date   2015/11/18 TDIPS)sasaki Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */

/*!
 *  @brief  the processing for "ファームウェアファイル報告要求" receiving at the FHM自律DL処理中 REC state.
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/11/18 TDIPS)sasaki Create REC CNT-N/E切替問題対応
 */

VOID m_dl_fl_fwrep_req(VOID* bufp, USHORT cpri_no)
{
	UINT			a_sigchk;
	USHORT			lwSystemType;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fl_fwrep_req] ENTER");

	a_sigchk = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind, &lwSystemType);
	if(a_sigchk != CMD_RES_OK)
	{
		m_dl_cm_CRFirmRspSnd(CMD_MAKER_RENG, 0, 0, lwSystemType);
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_ERROR, "validate signal NG(ret=%d,sys=%d)", a_sigchk, lwSystemType);
		return;
	}

	/* 3Gシステムからの要求の場合は、3G優先でも「ファームウェアファイルなし」応答する */
	if(lwSystemType == CMD_SYS_3G)
	{
		BPF_COM_LOG_ASSERT_DWL(D_RRH_LOG_AST_LV_INFO, "No firmware rsp(sys=%d)", lwSystemType);
		m_dl_cm_CRFirmRspSnd(CMD_FRM_NOFILE, 0, 0, CMD_SYS_3G); 		
		return;
	}

	/****************************************************************************/
	/* 以下、LTEの場合のみ														*/
	/****************************************************************************/
	/* 対RE状態遷移(FHM自律)へ入力する */
	m_dl_fhm_main(bufp, cpri_no);

	/*更新中リンク情報初期化 */
	gt_dlw_dlm_tbl.link_inf = 0x0000;

	/* 実行中DL開始要因に0:未実行を設定*/
	gt_dlw_dlm_tbl.dlstart_factor = DLD_UPDATE_FACTOR_NONE;

	/* 対RE状態遷移(REC主導)へ入力する */
	m_dl_rec_main(bufp, cpri_no);

	/* 実行中DL開始要因に1:RECを設定 */
	gt_dlw_dlm_tbl.dlstart_factor = DLD_FACTOR_REC;

	/* 対REC状態をファームウェア報告応答集約中へ遷移 */
	gt_dlw_rec_tbl.rec_sta = EC_DLM_MAIN_STN_FW_REP_CON;
}
/* @} */