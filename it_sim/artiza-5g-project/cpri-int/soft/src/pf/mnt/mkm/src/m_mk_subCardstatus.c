/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_subCardstatus.c
 *  @brief  Subscribe or unsubscribe card status
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
#include "rrhApi_Svp.h"
/********************************************************************************************************************/
/**
 *  @brief    Subscribe card status
 *  @note     Subscribe card status from PF process
 *  @param    -
 *  @return   None
 *  @retval   -
 *  @date     2013/11/21 FFCS)zhaodx create for zynq
 */
/********************************************************************************************************************/
extern UINT f_recv_addMsgMap(UINT , UINT , UINT );
extern UINT f_recv_delMsgMap(UINT , UINT , UINT );

VOID m_mk_subCardstatus()
{
	INT ret;
	INT wtime;

	/* call api of subscribing card state */
	cm_MemClr(mkw_cardStat,sizeof(mkw_cardStat));
	cm_MemClr(&mkw_cardcnt_tbl,sizeof(mkw_cardcnt_tbl));
	
	f_recv_addMsgMap(D_API_MSGID_SVP_STATE_CHG_NTC,0,CMD_TSKID_MKM);

	wtime = 0;
	ret = rrhApi_Svp_Mpr_SubscribeCardState(D_RRH_PROCQUE_MT, wtime, D_RRH_NULL);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_subCardstatus] Subscribe card state NG");
		f_recv_delMsgMap(D_API_MSGID_SVP_STATE_CHG_NTC,0,CMD_TSKID_MKM);
	}

	return;
}

VOID m_mk_unSubCardstatus()
{
	INT ret;
	INT wtime;

	/* call api of subscribing card state */
	cm_MemClr(mkw_cardStat,sizeof(mkw_cardStat));
	cm_MemClr(&mkw_cardcnt_tbl,sizeof(mkw_cardcnt_tbl));
	
	f_recv_delMsgMap(D_API_MSGID_SVP_STATE_CHG_NTC,0,CMD_TSKID_MKM);

	wtime = 0;
	ret = rrhApi_Svp_Mnt_UnsubscribeCardState(D_RRH_PROCQUE_MT, wtime, D_RRH_NULL);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_unSubCardstatus] unSubscribe card state NG");
	}

	return;
}

/* @} */
