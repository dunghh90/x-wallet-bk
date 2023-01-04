/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_tx_Abort.c
 *  @brief  Abnormal process
 *  @date   2008/07/15 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/


/*! @addtogroup RRH_L2_TXS
 * @{ */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRIF common head file     			*/
#include "tm_tx_header.h"			/* TXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  Abnormal process.
 *  @note   Output abnormal log.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/15 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
VOID tm_tx_Abort(UINT *inf_p)
{
	cm_MAbort(CMD_ALMCD_EVENT, 
	"tm_tx_Abort", 
	"event-state abnormal", 
	txw_eventno, 
	txw_stateno, 
	CMD_NUM0);
 }

/* @} */

