/*!
 * @skip   $Id$
 * @file   m_dl_fhm_idle_refw_dl_req.c
 * @brief  Event: REファイル更新要求(L3/DLM←RE/REC) RE state IDLE
 * @date   2015/08/22 FPT)Khiemnv1 Create
 * @date   2015/10/12 TDIPS)sasaki Update

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"							/* MDIF common head file			*/
#include "m_dl_header.h"							/* download management task head file */
#include "m_rc_header.h"

/*!
 *  @brief  Event: REファイル更新要求(L3/DLM←RE/REC) RE state IDLE
 *  @note  
 *		7-5-1 ファームウェアファイル報告要求(LTE)
 *		7.5.1 FW file report request
 *		REファイル更新要求(更新対象CPRI#)
 *		REFileUpdateReqRcv.c
 *		ref m_dl_CRUSFirmFileReqRcv.c
 *  @param  bufp 	[in]  receive message pointer REファイル更新要求 CMT_TSKIF_REFILNEWREQ
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.243対応
 *  @date   2015/10/12 TDIPS)sasaki Update MKレビュー指摘No.281対応
 *  @date   2015/11/04 TDIPS)sasaki Update IT3問処No.49対処
 */

VOID m_dl_fhm_idle_refw_dl_req(VOID* bufp, USHORT cpri_no)
{
	CMT_TSKIF_REFILNEWRES	lt_rsp;
	T_RRH_LAYER3			lt_layer3_sta;

	cm_Assert(CMD_ASL_NOLEVEL, CMD_NUM0, "[m_dl_fhm_idle_refw_dl_req] ENTER");

	if(CMD_SYS_3G == ((CMT_TSKIF_REFILNEWREQ *)bufp)->sys_type)
	{
		/* Send (0x4001)RE Forwarding Notice(ファームウェアファイル報告要求(3G)) */
		m_dl_FWFileInfoRequestSnd_3g(cpri_no);
		return;
	}

	/* REファイル更新応答メッセージ初期化 */
	memset(&lt_rsp, 0, sizeof(lt_rsp));

	/* DL開始要因がREC主導の場合 */
	if (gt_dlw_dlm_tbl.dlstart_factor == DLD_FACTOR_REC)
	{
		/* REファイル更新応答(NG) */
		lt_rsp.head.uiEventNo = CMD_TSKIF_REFIRM_DL_RSP;
		lt_rsp.cpri_no = cpri_no;
		lt_rsp.result_code = CMD_NG;
		lt_rsp.dummy = CMD_SYS_LTE;
		RE_forwarding_notice(MSG_RE_FILE_UPDATE_RESPOND, &lt_rsp);
		return;
	}

	/* 当該対RE-FW情報を初期化 */
	init_REFW(cpri_no-1);

	/* REC接続状態チェック */
	f_cmn_com_layer3_get(CMD_SYS_LTE, &lt_layer3_sta);
	switch(lt_layer3_sta.layer3_rec)
	{
		case D_L3_STA_REC_IDLE:		/* REC未接続 */
			/* 更新開始要因#nにRE起動(REC未接続)を設定 */
			gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_RESTART_REC_UNCON;
			break;
		case D_L3_STA_REC_USING:	/* REC接続済 */
			/* 更新開始要因#nにRE起動(REC接続済)を設定 */
			gt_dlw_refw_tbl[cpri_no-1].start_factor = DLD_UPDATE_FACTOR_RESTART_REC_CON;
			break;
	}

	/* ファームウェアファイル報告要求送信 */
	m_dl_FWFileInfoRequestSnd(bufp, cpri_no);
	return;
}
/** @} */
