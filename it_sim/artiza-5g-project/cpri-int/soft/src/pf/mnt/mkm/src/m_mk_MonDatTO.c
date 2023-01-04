/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_MonDatTO.c
 *  @brief   Send local saved messages to LAN, if any
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
 *  @brief     Send local saved messages to LAN, if any 
 *  @note     Send local saved messages to LAN, if any 
 *  @param    buf_p		[in]	Pointer of  buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 */
/********************************************************************************************************************/



VOID m_mk_MonDatTO(UINT *buf_p)
{
    CMT_TSKIF_HEAD* head_p;
	INT				buf_ret;
	if(mkw_mdatsav_p != NULL)	/* If monitor data saving buffer address is not null	*/
	{
		/* TCP データ送信通知(Send local saved messages to LAN)		*/
		head_p = (CMT_TSKIF_HEAD*)buf_p;
	    head_p->uiEventNo = CMD_TSKIF_TCPSNDREQ;
		head_p->uiSrcPQueueID = CMD_TSKID_MKM;
		head_p->uiDstPQueueID = CMD_TSKID_LNS;
		
		buf_ret = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, mkw_mdatsav_p);
		if(buf_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
		}
	}

	mkw_mdatsav_p = NULL;	
}
/* @} */
