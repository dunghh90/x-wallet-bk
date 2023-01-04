/*!
 * @skip   $Id$
 * @file   m_dl_idle_refw_dl_req.c
 * @brief  Event: REファイル更新要求(L3/DLM←RE/REC) REC state IDLE
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
 *  @brief  Event: REファイル更新要求(L3/DLM←RE/REC) REC state IDLE
 *  @note   -------7.5.1--------------------
			--1
			"=>2
			対RE状態遷移(FHM自律)へ入力する"
			call m_dl_fhm_main()
			  >>m_dl_fhm_idle_refw_dl_req()
 *  @param  bufp 	[in]  receive message pointer
 *  @param  cpri_no [in]  the CPRI no
 *  @return VOID
 *  @retval -
 *  @date   2015/08/22 FPT)Khiemnv1 Create
 *  @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.243対応
 */

VOID m_dl_idle_refw_dl_req(VOID* bufp, USHORT cpri_no)
{
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_idle_refw_dl_req] ENTER" );

	m_dl_fhm_main(bufp, cpri_no);

	//DLMスレッドメイン(対REC状態遷移) => 2
	if( CMD_SYS_3G == ((CMT_TSKIF_REFILNEWREQ *)bufp)->sys_type )
	{
		gt_dlw_rec_3g_tbl.rec_sta = EC_DLM_MAIN_STN_FHMDL;
	}
	else
	{
		set_REC_status(EC_DLM_MAIN_STN_FHMDL);
	}
	cm_Assert( CMD_ASL_NOLEVEL, CMD_NUM0,"[m_dl_idle_refw_dl_req] RETURN" );
	
}
/** @} */
