/*!
 *  @skip   $Id:$
 *  @file   l2_main_abort.c
 *  @brief  Abnormal process
 *  @date   2013/11/14 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

 /*!
 * @addtogroup RRH_L2_MAIN
 * @{
 */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "l2_com_inc.h"
#include "l2_main_inc.h"

/********************************************************************************************************************/
/*!
 *  @brief  Abnormal process.
 *  @note   Output abnormal log.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/14 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
VOID l2_main_abort(UINT *inf_p)
{
	cm_MAbort(CMD_ALMCD_EVENT, 
		"l2_main_Abort", 
		"event-state abnormal", 
		l2_mainw_eventno, 
		l2_mainw_stateno, 
		CMD_NUM0);
 }

/* @} */
