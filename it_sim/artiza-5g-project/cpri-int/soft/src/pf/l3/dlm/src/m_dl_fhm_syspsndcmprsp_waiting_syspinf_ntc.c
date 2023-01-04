/*!
 * @skip   $Id$
 * @file   m_dl_fhm_syspsndcmprsp_waiting_syspinf_ntc.c
 * @brief  the processing for ファイル情報報告通知(対象データ：シスパラ) receiving.
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
 *  @brief  the processing for ファイル情報報告通知(対象データ：シスパラ), status: ファイルデータ送信完了報告応答(シスパラ)待ち
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/11/18 TDIPS)sasaki Create REC CNT-N/E切替問題対応
 */

VOID m_dl_fhm_syspsndcmprsp_waiting_syspinf_ntc(VOID* bufp, USHORT cpri_no)
{
	INT			llErrcd;
	UINT		lwSysType = ((CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;
	UINT		lwTimerId;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fhm_syspsndcmprsp_waiting_syspinf_ntc] ENTER");

	/* システムパラメータ送信完了タイマ停止 */
	lwTimerId = CMD_TIMID_SYSPRM_SNDFIN + (cpri_no - 1) + D_RRH_CPRINO_RE_MAX * lwSysType;
	cm_TStop(lwTimerId, &llErrcd);

	/* RECスレッドへREシステムパラメータ更新応答(NG)送信 */
	m_dl_rl_sysp_dl_rsp(cpri_no, CMD_NG, lwSysType);

	/* 当該対RE-FW情報初期化 */
	REFW_init(&gt_dlw_refw_all_tbl[lwSysType][cpri_no-1]);
}
/* @} */