/*!
 * @skip   $Id$
 * @file   m_dl_fhm_fwreprsp_waiting_refw_stop_ind.c
 * @brief  Event: REファイル更新中止指示(L3/DLM←L3/RCT) RE status 3 DLD_RESTA_FWREP_WAIT
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
 *  @brief  Event: REファイル更新中止指示(L3/DLM←L3/RCT) RE status 3 DLD_RESTA_FWREP_WAIT
 *  @note   7.5.17
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/11/19 TDIPS)sasaki Update REC CNT-N/E切替問題対応
 */

VOID m_dl_fhm_fwreprsp_waiting_refw_stop_ind(VOID* bufp, USHORT cpri_no)
{
	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_fhm_fwreprsp_waiting_refw_stop_ind] ENTER");
	m_dl_RELinkDisconect(bufp, cpri_no, ((CMT_TSKIF_RENEWSTPNTC *)bufp)->systerm);
}

/* @} */