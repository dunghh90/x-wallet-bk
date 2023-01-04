/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_ls_ram.c
 *  @brief   declare variable for LAN Message Sending Task
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
#include "tm_ls_header.h"			/* LNS task header					*/

/********************************************************************************************************************/
/**
 *  @brief     declare variable in LAN Message Sending Task
 *  @note     declare variable in LAN Message Sending Task
 *  @param  None
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/



UINT lsw_tststat ;/*Task state*/
UINT lsw_eventno ;/*The received event number*/

LST_ERRINFO lsw_errinfo ;/*Table to record error info returned by sockert function*/
/* @} */

