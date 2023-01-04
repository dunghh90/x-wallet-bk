/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_DataEndNtcRsp.c
 *  @brief   Download end response
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
 *  @brief     Download end response
 *  @note     Download end response
 *  @param  func_id			[in]	Function ID
 *  @param  ng_code			[in]	NG code 
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/22 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/


VOID m_mk_DataEndNtcRsp(UINT func_id, UINT ng_code)
{
	MKT_TCPIF_REMTFLDLCMPRSP *a_TcpDlCmpRsp_p 	= NULL;			/* File download start response pointer */
	UINT 					a_ret 				= CMD_RES_OK;	/* Return result 						*/
	CMT_TSKIF_TCPSNDREQ 	*a_sndmsg_p			= NULL;			/* Pointer of TCP data send req IF		*/
	
	/************************************************/
	/*	 Hunt buffer for download end response	*/
	/************************************************/
	
	/* Hunt buffer for file download end response */
	a_ret = cm_BReq(CMD_BUFCA_MON, sizeof(MKT_TCPIF_REMTFLDLCMPRSP), (VOID **)&a_TcpDlCmpRsp_p);

	/* Return result check */
	if(a_ret != CMD_RES_OK)
	{
		/* Hunt buffer error, abort */
		cm_MAbort(CMD_ALMCD_BUFGET, 
		"m_mk_DataEndNtcRsp", 
		"Buffer request error",
		a_ret, 
		sizeof(MKT_TCPIF_REMTFLDLCMPRSP), 
		CMD_NUM0);
	}

	/************************/
	/*		Fill buffer		*/
	/************************/
	/* Function id */
	a_TcpDlCmpRsp_p->fldlcmprsp.FunctionID = func_id;
	/* NG code */
	a_TcpDlCmpRsp_p->fldlcmprsp.NGCode = ng_code;
	/* NG code check */	
	if(ng_code == CMD_NML)
	{
		/* Normal response */
		a_TcpDlCmpRsp_p->fldlcmprsp.Result = CMD_OK;
	}
	else
	{
		/* NG response */
		a_TcpDlCmpRsp_p->fldlcmprsp.Result = CMD_NG;
	}


	/* head and data byte */
	a_TcpDlCmpRsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(MKT_REMTFLDLCMPRSP);
	/* Set TCP data for transmission */
	m_mk_cm_SetSndMsg(MKD_REMTFLDLCMPRSP, a_TcpDlCmpRsp_p, &a_sndmsg_p);

	/* Send TCP data tx request (CPRI data send request) to LAN tx task	*/
	a_ret = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, a_sndmsg_p);
	if(a_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
	}

	return;
	
}
/* @} */
