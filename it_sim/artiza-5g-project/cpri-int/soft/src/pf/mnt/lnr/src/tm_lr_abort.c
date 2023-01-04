/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_lr_abort.c
 *  @brief   Call cm_Abort	
 *  @date   2008/07/28  FFCS)zhengmh Create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_LNR
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "tm_lr_header.h"			/*LNR task header					 	*/

/********************************************************************************************************************/
/**
 *  @brief     Call cm_Abort
 *  @note     Call cm_Abort
 *  @param	  a_inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/

VOID tm_lr_Abort(UINT * a_inf_p)
{
	cm_MAbort(CMD_ALMCD_EVENT,"tm_lr_Abort","abnormal event",lrw_eventno,(UINT)a_inf_p,lrw_tststat);
}
/* @} */
