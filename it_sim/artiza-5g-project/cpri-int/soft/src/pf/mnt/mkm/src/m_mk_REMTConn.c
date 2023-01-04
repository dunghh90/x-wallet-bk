/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_REMTConn.c
 *  @brief  RE-MT connect start
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/12/03 FFCS)zhaodx modify for zynq
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
 *  @brief     RE-MT connect start	 
 *  @note     RE-MT connect start	 
 *  @param  inf_p		[in]	Pointer of  buffer
 *  @param  link_flag	[in]	link flag
 *  @param  linkhandle  [in]    TCP link handle
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/12/03 FFCS)zhaodx modify for zynq
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_REMTConn(UINT *inf_p, UINT link_flag, INT linkhandle)
{
	UINT						a_ret_buf = CMD_RES_OK;				/* Return value of buffer request		*/
	UINT 						a_ngcode = CMD_RES_OK;				/* Ng code 								*/
	UINT						a_okng = CMD_OK;					/* Ok/ng flag							*/
	MKT_TCPIF_REMTCONSTRRSP	  	*a_tcp_remtstrrsp_p = NULL;			/* TCPIF RE-MT start response pointer		*/
	CMT_TSKIF_TCPSNDREQ 		*a_sndmsg_p			= NULL;			/* Pointer of TCP data send req IF		*/
	
	if (mkw_REMTConnect == CMD_CONN_Y )
	{	
		if ( link_flag == MKD_TCPDATA )
		{
			/* More than one RE-MT client connect to TRA */
			a_ngcode = MKD_RES_MDIFMULTICON;
		}
		else
		{
			return;
			/* Phase 3*/
		}
		a_okng = CMD_NG;
	}
	else
	{
		/* RE-MT client connected */
		mkw_REMTConnect = CMD_CONN_Y;
	}
	
	/* Link flag judge */
	if (link_flag == MKD_TCPDATA )
	{
		/* RE-MT start response buffer request */
		a_ret_buf = cm_BReq(CMD_BUFCA_MON, sizeof(MKT_TCPIF_REMTCONSTRRSP), (VOID **)&a_tcp_remtstrrsp_p);
		/* Return value judge */
		if(a_ret_buf  != CMD_RES_OK)
		{
			/* Buffer request error,abort */
			cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_remtConn", 
			"Buffer request error",
			a_ret_buf, 
			sizeof(MKT_TCPIF_REMTCONSTRRSP), 
			CMD_NUM0);
		}
		/* Message body part */
		a_tcp_remtstrrsp_p->constrrsp.Result = a_okng;
		a_tcp_remtstrrsp_p->constrrsp.NGCode = a_ngcode;	
		/* Head data byte length */
		a_tcp_remtstrrsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(MKT_REMTCONSTRRSP);
		/* Set TCP data for transmission */
		m_mk_cm_SetSndMsg(MKD_REMTCONSTRRSP, a_tcp_remtstrrsp_p, &a_sndmsg_p);
		a_sndmsg_p->linkhandle = linkhandle;

		/* Send TCP data tx request (CPRI data send request) to LAN tx task	*/
		a_ret_buf = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, a_sndmsg_p);
		if(a_ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret_buf, "cm_Enter NG" );
		}
	}
	else
	{
		/* Phase 3*/
	}
	return;
}
/* @} */
