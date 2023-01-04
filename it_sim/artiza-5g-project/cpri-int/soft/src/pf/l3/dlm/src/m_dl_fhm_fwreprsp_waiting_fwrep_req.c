/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fwreprsp_waiting_fwrep_req.c
 * @brief  the processing for ファームウェアファイル報告要求 receiving.
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
 *  @brief  the processing for ファームウェアファイル報告要求, status: ファームウェアファイル報告応答待ち.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/11/18 TDIPS)sasaki Create REC CNT-N/E切替問題対応
 */

VOID    m_dl_fhm_fwreprsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no)
{
	INT			llErrcd;
	UINT		lwTimerId;
	UINT		lwSysType = ((CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fhm_fwreprsp_waiting_fwrep_req] ENTER");

	/* ファームウェアファイル報告確認タイマ停止 */
	lwTimerId = CMD_TIMID_FIRMFILE_REP_3G + (cpri_no - 1) + D_RRH_CPRINO_RE_MAX * lwSysType;
	cm_TStop(lwTimerId, &llErrcd);

	/* RECスレッドへREファイル更新応答(NG)送信 */
	m_dl_UpdateExpandREDowloadingFile(bufp, cpri_no);

	/* 当該対RE-FW情報初期化 */
	REFW_init(&gt_dlw_refw_all_tbl[lwSysType][cpri_no-1]);
}
/* @} */