/*!
 * @skip    $Id$
 * @file    tm_lb_abort.c
 * @brief   タスクアボート処理関数(tm_lb_Abort
 * @date    2008.07.22 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/

/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"
#include "tm_lb_header.h"







/*!
 *  @brief   タスクアボート処理関数(tm_lb_Abort
 *  @note    アボート処理
 *  @param   a_source		[in]  a_source
 *  @param   a_inf_p		[in]  a_inf_p
 *  @param   a_event		[in]  a_event
 *  @return  None
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
void	tm_lb_Abort(UINT a_source,UINT a_event,UINT * a_inf_p)
{
	cm_MAbort(CMD_ALMCD_EVENT, "tm_lb_Abort",
					"CMD_TSKIF RCV NG_1", a_source, a_event, CMD_NUM0);
	return;
}


/* @} */


