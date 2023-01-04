/*!
 * @skip   $Id$
 * @file   m_dl_fl_refw_stop_req.c
 * @brief  Event: REファイル更新中止要求(L3/DLM←RE/REC) REC status EC_DLM_MAIN_STN_FHMDL 2
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
 *  @brief  Event: REファイル更新中止要求(L3/DLM←RE/REC) REC status EC_DLM_MAIN_STN_FHMDL 2
 *  @note   -------7.5.17-------------------
			--1
			"対RE状態遷移(FHM自律)へ入力する
			対RE状態が全てアイドル=>1
			else =>2"
			call m_dl_fhm_main
			  >>xxx
			....>>7.5.17
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 */

VOID m_dl_fl_refw_stop_req(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_refw_stop_req] ENTER" );

	USHORT lw_all_RE_idle = 0;

	m_dl_fhm_main(bufp, cpri_no);

	lw_all_RE_idle = all_RE_idle();
	if (lw_all_RE_idle)
	{
		set_REC_status(EC_DLM_MAIN_STN_IDLE);
	}
	else
	{
		set_REC_status(EC_DLM_MAIN_STN_FHMDL);
	}
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,	"[m_dl_fl_refw_stop_req] RETURN" );
}
/* @} */