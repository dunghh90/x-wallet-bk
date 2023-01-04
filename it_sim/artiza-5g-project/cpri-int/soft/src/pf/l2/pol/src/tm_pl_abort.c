/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_Abort.c
 *  @brief  Abnormal process
 *  @date   2008/07/23 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/


/*! @addtogroup RRH_L2_POLLING
 * @{ */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file           			*/
#include "tm_pl_header.h"			/* POLLING task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  Abnormal process.
 *  @note   Output abnormal log.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/23 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
VOID tm_pl_Abort(UINT *inf_p)
{
	cm_MAbort(CMD_ALMCD_EVENT, 
	"tm_pl_Abort", 
	"event-state abnormal", 
	plw_eventno, 
	plw_stateno, 
	CMD_NUM0);
}

/* @} */
