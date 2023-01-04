/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_subl2state.c
 *  @brief  Subscribe or unsubscribe l2 state
 *  @date   2013/11/21 FFCS)zhaodx create for zynq
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/*  RRH common head file							*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/
#include "rrhApi_L2.h"
/********************************************************************************************************************/
/**
 *  @brief    Subscribe l2 state
 *  @note     Subscribe l2 state from l2 process
 *  @param    -
 *  @return   None
 *  @retval   -
 *  @date     2013/11/21 FFCS)zhaodx create for zynq
 */
/********************************************************************************************************************/

VOID m_mk_subl2state(USHORT linkno)
{
	INT ret;
	INT wtime;

	/* call api of subscribing l2 state */
	wtime = 0;
	ret = rrhApi_L2_Mpr_SubscribeLinkState(linkno, D_RRH_PROCQUE_MT, wtime,D_RRH_NULL);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, ret, "[m_mk_subl2state] Subscribe l2 state NG");
	}
	return;
}

/********************************************************************************************************************/
/**
 *  @brief    Unsubscribe l2 state
 *  @note     Unsbscribe l2 state from l2 process
 *  @param    -
 *  @return   None
 *  @retval   -
 *  @date     2013/11/21 FFCS)zhaodx create for zynq
 */
/********************************************************************************************************************/

VOID m_mk_unsubl2state(USHORT linkno)
{
	INT ret;
	INT wtime;

	/* call api of unsubscribing l2 state */
	wtime = 0;
	ret = rrhApi_L2_Mnt_UnsubscribeLinkState(linkno, D_RRH_PROCQUE_MT, wtime,D_RRH_NULL);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_unsubl2state] Unsbscribe l2 state NG");
	}
	return;
}
/* @} */
