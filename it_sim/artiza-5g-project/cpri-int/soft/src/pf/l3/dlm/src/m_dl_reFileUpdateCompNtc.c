/*!
 * @skip   $Id$
 * @file   m_dl_reFileUpdateCompNtc.c
 * @brief  the processing RE file update completion
 * @date   2015/08/20 FPT)VietCV Create

 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2010
 */

/*!
 * @addtogroup RRH_L3_DLM
 * @{
 */

#include "m_cm_header.h"															/* MDIF common head file			*/
#include "m_dl_header.h"															/* download management task head file */

/*!
 *  @brief  the processing RE file update completion
 *  @note   
 *  @param  cpri_no [in]  the CPRI no
 *	@param  a_result [in]  the result of response message received
 *  @return Void.
 *  @retval -
 *  @date   2015/08/20 FPT)VietCV Create
 *  @date   2015/09/17 FPT)VietCV update
 */

VOID m_dl_reFileUpdateCompNtc(USHORT cpri_no, USHORT a_result)
{
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s start cpri_no = %d, a_result = %d\n",__LINE__,__FUNCTION__, cpri_no, a_result);
	#endif
	CMT_TSKIF_UREFILNEWFINRES	ltFileInfRes;
	
	/* 更新中リンク情報の当該CPRI#ビットOFF */
	gt_dlw_dlm_tbl.link_inf &= ~(CMD_ON << (cpri_no-1));
	
	/* 運用中REファイル更新完了通知(結果(OK or NG) */
	ltFileInfRes.cpri_no = cpri_no;
	ltFileInfRes.result_code = a_result;
	ltFileInfRes.head.uiEventNo = CMD_TSKIF_REFILE_UPDATE_COMP_NTC;
	
	l3_com_sendMsg(CMD_TSKID_DLM, D_RRH_PROCQUE_RE, 0, &ltFileInfRes, sizeof( CMT_TSKIF_UREFILNEWFINRES));
	
	/* change status of re status */
	gt_dlw_refw_tbl[cpri_no-1].re_sta = DLD_RESTA_IDLE;
	#ifdef FHM_DLM_FOR_DEBUG_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
	#endif
	return;
}
/** @} */
