/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_Abort.c
 *  @brief   Abort process	 
 *  @date   2008/07/25 FFCS)zhengmh create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/*  TRA common head file							*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/

/********************************************************************************************************************/
/**
 *  @brief     Abort process
 *  @note     Abort process
 *  @param  inf_p      [in]   Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 */
/********************************************************************************************************************/

VOID m_mk_Abort(UINT *inf_p)
{
	cm_MAbort(CMD_ALMCD_EVENT, 
	"m_mk_Abort", 
	"event-state abnormal", 
	mkw_eventno, 
	mkw_stateno, 
	CMD_NUM0);
}
/* @} */
