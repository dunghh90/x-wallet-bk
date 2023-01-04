/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_abort.c
 *  @brief  Abnormal process
 *  @date   2013/12/03 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

 /*!
 * @addtogroup RRH_L2_DBG
 * @{
 */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "l2_dbg_inc.h"

/********************************************************************************************************************/
/*!
 *  @brief  Abnormal process.
 *  @note   Output abnormal log.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/12/03 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
#if 0
VOID l2_dbg_abort(UINT *inf_p)
{

	cm_MAbort(CMD_ALMCD_EVENT, 
		"l2_dbg_Abort", 
		"event abnormal", 
		l2_dbgw_eventno, 
		CMD_NUM0, 
		CMD_NUM0);

 }
 #endif

/* @} */
