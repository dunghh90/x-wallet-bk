/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_cm_SetSndMsg.c
 *  @brief   Set TCP data tx message	 
 *  @date   2008/07/25 FFCS)zhengmh create
 *          2013/11/22 FFCS)zhaodx modify for zynq
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
 *  @brief     Set TCP data tx message
 *  @note     Set TCP data tx message
 *  @param  rxevent		[in]	Received event
 *  @param  inf_p			[in]	Pointer of buffer
 *  @param  sndmsg_pp		[in]	Double pointer of TCP tx data
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/22 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/

VOID m_mk_cm_SetSndMsg(UINT rxevent, VOID *inf_p, 
									CMT_TSKIF_TCPSNDREQ **sndmsg_pp)
{
	CMT_TSKIF_HEAD 		*head_p;	/* Pointer of task common head		*/
	UINT 				ret_buf;	/* Return value of buffer request	*/
	CMT_TSKIF_TCPSNDREQ *sndmsg_p;	/* Single pointer of TCP data send request IF*/

	head_p = NULL;
	ret_buf = CMD_NUM0;
	sndmsg_p = NULL;

	/* Buffer request	*/
	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPSNDREQ), (VOID **)sndmsg_pp);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
		"m_mk_cm_SetSndMsg", 
		"Buffer request error", 
		ret_buf, 
		sizeof(CMT_TSKIF_TCPSNDREQ), 
		CMD_NUM0);
	}

	sndmsg_p = *sndmsg_pp;
	sndmsg_p->msglen = CMD_NUM0;		/* Init message length				*/

	if(inf_p)					/* If buffer pointer is not NULL	*/
	{
		head_p = (CMT_TSKIF_HEAD *)inf_p;
		sndmsg_p->msgbody = inf_p;
		sndmsg_p->msglen += head_p->uiLength - sizeof(CMT_TSKIF_HEAD);
	}
	else
	{
		sndmsg_p->msgbody = (VOID *)NULL;
	}

	sndmsg_p->msgevent = rxevent;
	/*sndmsg_p->msglen += sizeof(UINT);*/

	switch(rxevent)
	{
		case MKD_CPRIMONSTRRSP:			/* CPRI monitor start response	*/
		case MKD_CPRIMONSTPRSP:			/* CPRI monitor stop response	*/
			sndmsg_p->rtc_rsp.rsp.result = CMD_OK;	/* Response result OK (0)	*/
			sndmsg_p->rtc_rsp.rsp.cause = CMD_NUM0;		/* NG code NULL				*/
			sndmsg_p->msglen += (sizeof(UINT) * CMD_NUM2);
			break;
		default:
			break;
	}

	sndmsg_p->head.uiLength = sizeof(CMT_TSKIF_TCPSNDREQ);
	sndmsg_p->head.uiEventNo = CMD_TSKIF_TCPSNDREQ;
	sndmsg_p->head.uiSrcPQueueID = 0;
	sndmsg_p->head.uiDstPQueueID = 0;
	sndmsg_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
	sndmsg_p->head.uiDstTQueueID = CMD_TSKID_LNS;

	return;
}
/* @} */
