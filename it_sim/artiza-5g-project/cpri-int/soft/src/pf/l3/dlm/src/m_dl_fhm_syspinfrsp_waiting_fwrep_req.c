/*!
 * @skip   $Id$
 * @file   m_dl_fhm_syspinfrsp_waiting_fwrep_req.c
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
 *  @brief  the processing for ファームウェアファイル報告要求, status: ファイル情報報告応答(シスパラ)待ち.
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/11/18 TDIPS)sasaki Create REC CNT-N/E切替問題対応
 */

VOID m_dl_fhm_syspinfrsp_waiting_fwrep_req(VOID* bufp, USHORT cpri_no)
{
	INT			ldwerrcd;
	UINT		lwSysType = ((CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID;
	UINT		lwTimerId;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fhm_syspinfrsp_waiting_fwrep_req] ENTER");

	/* システムパラメータ情報報告確認タイマ停止 */
	lwTimerId = CMD_TIMID_SYSPRM_INF + (cpri_no - 1) + D_RRH_CPRINO_RE_MAX * lwSysType;
	cm_TStop(lwTimerId, &ldwerrcd);

	/* RECスレッドへREシステムパラメータ更新応答(NG)送信 */
	m_dl_rl_sysp_dl_rsp(cpri_no, CMD_NG, lwSysType);

	/* 当該対RE-FW情報初期化 */
	REFW_init(&gt_dlw_refw_all_tbl[lwSysType][cpri_no-1]);
}

/* @} */