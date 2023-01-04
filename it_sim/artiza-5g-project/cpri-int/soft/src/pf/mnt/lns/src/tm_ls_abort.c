/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_ls_nop.c
 *  @brief   Call cm_Abort	
 *  @date   2008/07/28  FFCS)zhengmh Create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_LNS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "tm_ls_header.h"			/*LNS task header					 	*/

/********************************************************************************************************************/
/**
 *  @brief     Call cm_Abort
 *  @note     Call cm_Abort
 *  @param  a_inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/


VOID tm_ls_Abort(UINT * a_inf_p)
{
	cm_MAbort(CMD_ALMCD_EVENT,"tm_ls_Abort","abnormal event",lsw_eventno,(UINT)a_inf_p,lsw_tststat);
}

/* @} */
