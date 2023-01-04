/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_cm_mondatproc.c
 *  @brief   Monitor data receiving processing	 
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/22 FFCS)zhaodx modify for zynq
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
 *  @brief     Monitor data receiving processing
 *  @note     Monitor data receiving processing
 *  @param  rxevent  [in] Received event
 *  @param  inf_p    [in] Pointer of buffer
 *  @param  link_num [in] CPRIリンク番号
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/22 FFCS)zhaodx modify for zynq
 *  @date     2015/09/24 TDI)satou 引数にlink_numを追加し、対応するtcplinkhdlを返却するように
 */
/********************************************************************************************************************/


VOID m_mk_cm_MonDatProc(UINT rxevent, VOID *inf_p, USHORT link_num)
{
    CMT_TSKIF_MKMTMONDAT *mondat_p;	/* Maker MT monitor data header 		*/
	UINT		len_mkmthead;		/* Length of Maker MT header			*/
	UINT		msglen;				/* 送信メッセージの長さ					*/
	INT			buf_ret;
	T_CMD_TCPCONN_NUM tcpconn;
	
	mondat_p = (CMT_TSKIF_MKMTMONDAT *)inf_p;
	len_mkmthead = sizeof( CMT_TSKIF_MKMTHEAD);
	msglen = (UINT)(mondat_p->head.uiLength - sizeof(CMT_TSKIF_HEAD) - len_mkmthead + sizeof(UINT) * CMD_NUM2);
	tcpconn = mnt_com_get_tcpconn_monitor(link_num);
	
	mondat_p->mkmt_head.msglen     = msglen;
	mondat_p->mkmt_head.msgevent   = rxevent;
	mondat_p->mkmt_head.msgbody    = NULL;
	mondat_p->mkmt_head.linkhandle = cmw_tblClientInfo[tcpconn].tcplinkhdl;
	
	/* send the received message to LAN directly */
	mkw_needfrmsg = MKD_FALSE;	/* Should not free the received message buffer	*/

	mondat_p->head.uiEventNo = CMD_TSKIF_TCPSNDREQ;
	mondat_p->head.uiSrcPQueueID = 0;
	mondat_p->head.uiDstPQueueID = 0;
	mondat_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
	mondat_p->head.uiDstTQueueID = CMD_TSKID_LNS;
	/* TCP データ送信通知(Send the received message to LAN)				*/
	buf_ret = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, inf_p);
	if(buf_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
	}

	return;
}
/* @} */
