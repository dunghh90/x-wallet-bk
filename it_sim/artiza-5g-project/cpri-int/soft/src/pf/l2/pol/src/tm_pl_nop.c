																		
/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_Nop.c
 *  @brief  No operation.
 *  @date   2008/07/25 FFCS)Zouw Create 
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
 *  @brief  No operation.
 *  @note  No operation.
 *  @param  *inf_p [in] Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned.
 *  @retval -
 *  @date   2008/07/25 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
VOID tm_pl_Nop(UINT *inf_p)
{
	cm_Assert( CMD_ASL_DBGLOW, (UINT)inf_p, 
							"Event-state NG");
	return;
}

/* @} */
