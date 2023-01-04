/*!
 * @skip   $Id$
 * @file   m_dl_fhm_finfrsp_waiting_refw_stop_req.c
 * @brief  Event: REファイル更新中止要求(L3/DLM←RE/REC) RE status 5 DLD_RESTA_FILEINF_WAIT
 * @date   2015/08/22 FPT)Khiemnv1 Create
 * @date   2015/11/19 TDIPS)sasaki Update

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
 *  @brief  Event: Event: REファイル更新中止要求(L3/DLM←RE/REC) RE status 5 DLD_RESTA_FILEINF_WAIT
 *  @note   7.5.17
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/11/19 TDIPS)sasaki Update REC CNT-N/E切替問題対応
 */

VOID m_dl_fhm_finfrsp_waiting_refw_stop_req(VOID* bufp, USHORT cpri_no)
{
	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fhm_finfrsp_waiting_refw_stop_req] ENTER");
	m_dl_RELinkDisconect(bufp, cpri_no, ((CMT_TSKIF_REFILNEWSTP *)bufp)->systerm);
}
/* @} */