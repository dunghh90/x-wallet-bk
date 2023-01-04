/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_lr_ram.c
 *  @brief   declare variable for LNR task
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
#include "tm_lr_header.h"			/* LNR task header					*/

/********************************************************************************************************************/
/**
 *  @brief     declare variable for LNR task
 *  @note     declare variable for LNR task
 *  @param  None
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/

UINT lrw_tststat ;/*Task task*/
UINT lrw_eventno ;/*The received event number*/

/*server tcp handle for simulator/monitor data transfer*/
INT lrw_srvsocket[LRD_SRVCOUNT] ;
LRT_ERRINFO lrw_errinfo ;/*Table to record err info returned by socket function*/
/* @} */
