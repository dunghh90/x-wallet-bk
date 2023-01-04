/*!
 * @skip   $Id$
 * @file   m_dl_idle_refw_stop_req.c
 * @brief  the processing for "REファイル更新中止要求" receiving.
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
 *  @brief  the processing for "REファイル更新中止要求" receiving at the アイドル REC state
 *  @note   
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return Void.
 *  @retval -
 *  @date   2015/11/18 TDIPS)sasaki Create
 */

VOID m_dl_idle_refw_stop_req(VOID* bufp, USHORT cpri_no)
{
	cm_Assert(CMD_ASL_NOLEVEL, cpri_no, "[m_dl_idle_refw_stop_req] ENTER");

	/* 当該リンクの対RE状態テーブル初期化 */
	m_dl_RELinkDisconect(bufp, cpri_no, ((CMT_TSKIF_REFILNEWSTP *)bufp)->systerm);
}
/* @} */
