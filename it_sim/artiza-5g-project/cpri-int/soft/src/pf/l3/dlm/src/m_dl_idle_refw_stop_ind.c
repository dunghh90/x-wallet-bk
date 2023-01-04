/*!
 * @skip   $Id$
 * @file   m_dl_idle_refw_stop_ind.c
 * @brief  the processing for "REファイル更新中止指示" receiving.
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
 *  @brief  the processing for "REファイル更新中止指示" receiving at the アイドル on 対RE状態遷移(FHM自律) matrix
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/11/18 TDIPS)sasaki Create
 */

VOID m_dl_idle_refw_stop_ind(VOID* bufp, USHORT cpri_no)
{
	USHORT	lwCpriNo;

	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_idle_refw_stop_ind] ENTER");

	/* 全リンクの対RE状態テーブル初期化 */
	for(lwCpriNo = D_RRH_CPRINO_RE_MIN; lwCpriNo <= D_RRH_CPRINO_RE_MAX; lwCpriNo++)
	{
		m_dl_RELinkDisconect(bufp, lwCpriNo, ((CMT_TSKIF_RENEWSTPNTC *)bufp)->systerm);
	}
}
/* @} */
