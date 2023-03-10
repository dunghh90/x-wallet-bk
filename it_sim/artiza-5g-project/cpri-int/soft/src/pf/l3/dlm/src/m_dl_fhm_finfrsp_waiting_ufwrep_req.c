/*!
 * @skip   $Id$
 * @file   m_dl_fhm_finfrsp_waiting_ufwrep_req.c
 * @brief  Event: 運用中ファームウェアファイル報告要求, Status: ファイル情報報告応答(FW)待ち
 * @date   2015/08/22 FPT)Khiemnv1 Create

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
 *  @brief  Event: 運用中ファームウェアファイル報告要求, Status: ファイル情報報告応答(FW)待ち
 *  @note   7.6.1
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/09/24 FPT)Yen update
 */

VOID m_dl_fhm_finfrsp_waiting_ufwrep_req(VOID* bufp, USHORT cpri_no)
{
	INT			ldwerrcd;
	UINT		lwTimerId;
	USHORT 		lw_REFW_index = 0;

	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fhm_finfrsp_waiting_ufwrep_req] ENTER" );

	lw_REFW_index = convert_CPRI_no_REFW_index(cpri_no);

	/* ファームウェアファイル情報報告確認タイマ停止 */
	lwTimerId = CMD_TIMID_FIRMFILE_INF + cpri_no - CMD_NUM1;
	cm_TStop( lwTimerId, &ldwerrcd );

	/* RECスレッドへREファイル更新応答(NG)送信 */
	m_dl_UpdateExpandREDowloadingFile(bufp, cpri_no);

	set_RE_status(lw_REFW_index, DLD_RESTA_IDLE);
}

/* @} */