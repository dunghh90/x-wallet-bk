/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_lr_TcpInit.c
 *  @brief   process TCP server init request
 *  @date   2008/07/28  FFCS)zhengmh Create
 *  @date   2013/11/22 FFCS)zhaodx modify for zynq
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
#include <sys/types.h>
#include <sys/socket.h>

/********************************************************************************************************************/
/**
 *  @brief     process TCP server init request 
 *  @note      1. Bring LAN up															
		     2. Init TCP server for simulator/monitor									
		     3. Make messasge TCP server,and send it to Maker MT task			 					
 *  @param  a_inf_p	[in]		point of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/


VOID tm_lr_TcpInit(UINT * a_inf_p)
{
	UINT						a_ret;/*Information returned by function*/
	CMT_TSKIF_TCPSRVSTRREQ*	a_tcpsrvinitreq_p;/*Buffer addr for task IF*/
	USHORT						a_simuport;/*Port for simulator data transfer*/
	USHORT						a_simuport_re1;/*Port for simulator RE1 data transfer*/
	USHORT						a_simuport_re2;/*Port for simulator RE2 data transfer*/
	USHORT						a_simuport_re3;/*Port for simulator RE3 data transfer*/
	USHORT						a_monport;/*Port for monitor data transfer*/	
	USHORT						a_monport_re1;/*Port for monitor RE1 data transfer*/	
	USHORT						a_monport_re2;/*Port for monitor RE2 data transfer*/	
	USHORT						a_monport_re3;/*Port for monitor RE3 data transfer*/	
	USHORT						a_remtport;/*Port for RE-MT data transfer*/	

	CMT_TSKIF_TCPSRVSTRRSP*	a_tcpsrvinitrsp_p;/*Buffer addr for task IF*/

	/*Type conversion*/
	a_tcpsrvinitreq_p = (CMT_TSKIF_TCPSRVSTRREQ*)a_inf_p;
	a_simuport = a_tcpsrvinitreq_p->simuport;/*Get port for simulator data transfer*/
	a_simuport_re1 = a_tcpsrvinitreq_p->simuport_re1;/*Get port for simulator RE1 data transfer*/
	a_simuport_re2 = a_tcpsrvinitreq_p->simuport_re2;/*Get port for simulator RE2 data transfer*/
	a_simuport_re3 = a_tcpsrvinitreq_p->simuport_re3;/*Get port for simulator RE3 data transfer*/
	a_monport = a_tcpsrvinitreq_p->monport;/*Get port for monitor data transfer*/
	a_monport_re1 = a_tcpsrvinitreq_p->monport_re1;/*Get port for monitor RE1 data transfer*/
	a_monport_re2 = a_tcpsrvinitreq_p->monport_re2;/*Get port for monitor RE2 data transfer*/
	a_monport_re3 = a_tcpsrvinitreq_p->monport_re3;/*Get port for monitor RE3 data transfer*/

	a_remtport = a_tcpsrvinitreq_p->remtport;/*Get port for OPT MT data transfer*/

	
		
	/*Hunter buffer for task IF*/
	a_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPSRVSTRRSP),(void**)&a_tcpsrvinitrsp_p);
	/*Fail to get buffer*/
	if(a_ret != CMD_RES_OK)
	{
		/*Free buffer for task IF*/
		cm_MAbort(CMD_ALMCD_BUFGET, "tm_lr_TcpInit", 
					"Buffer for CMT_TSKIF_TCPSRVSTRRSP is null ",
					a_ret,(UINT)(sizeof(CMT_TSKIF_TCPRELNTC)), CMD_NUM0);

		return;
	}
	
	a_tcpsrvinitrsp_p->obj = a_tcpsrvinitreq_p->obj;/*Get MT obj*/
	
	a_tcpsrvinitrsp_p->head.uiLength = sizeof(CMT_TSKIF_TCPSRVSTRRSP);
	a_tcpsrvinitrsp_p->head.uiEventNo = CMD_TSKIF_TCPSRVSTRRSP;
	a_tcpsrvinitrsp_p->head.uiSrcTQueueID = CMD_TSKID_LNR;
	a_tcpsrvinitrsp_p->head.uiDstTQueueID = CMD_TSKID_MKM;
	/*Call tm_lr_cm_SrvSocketInit to initialize tcp server for simulator*/
	a_ret = tm_lr_cm_SrvSocketInit(&lrw_srvsocket[LRD_SRVSIMULATOR],
									a_simuport);
	/*Fail to initialize Tcp server,so that send message to Maker MT task*/
	if(a_ret != CMD_OK)
	{
		a_tcpsrvinitrsp_p->result = -1;
		
		/*Send message to Maker MT task */
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}

		return ;
	}

	/*Call tm_lr_cm_SrvSocketInit to initialize tcp server for simulator RE1*/
	a_ret = tm_lr_cm_SrvSocketInit(&lrw_srvsocket[LRD_SRVSIMULATOR_RE1],
									a_simuport_re1);
	/*Fail to initialize Tcp server,so that send message to Maker MT task*/
	if(a_ret != CMD_OK)
	{
		a_tcpsrvinitrsp_p->result = -1;
		
		/*Send message to Maker MT task */
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}

		return ;
	}

	/*Call tm_lr_cm_SrvSocketInit to initialize tcp server for simulator RE2*/
	a_ret = tm_lr_cm_SrvSocketInit(&lrw_srvsocket[LRD_SRVSIMULATOR_RE2],
									a_simuport_re2);
	/*Fail to initialize Tcp server,so that send message to Maker MT task*/
	if(a_ret != CMD_OK)
	{
		a_tcpsrvinitrsp_p->result = -1;
		
		/*Send message to Maker MT task */
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}

		return ;
	}

	/*Call tm_lr_cm_SrvSocketInit to initialize tcp server for simulator RE3*/
	a_ret = tm_lr_cm_SrvSocketInit(&lrw_srvsocket[LRD_SRVSIMULATOR_RE3],
									a_simuport_re3);
	/*Fail to initialize Tcp server,so that send message to Maker MT task*/
	if(a_ret != CMD_OK)
	{
		a_tcpsrvinitrsp_p->result = -1;
		
		/*Send message to Maker MT task */
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}

		return ;
	}

	/*Call tm_lr_cm_SrvSocketInit to initialize tcp server for monitor*/
	a_ret = tm_lr_cm_SrvSocketInit(&lrw_srvsocket[LRD_SRVMONITOR],
									a_monport);
	/*Fail to initialize Tcp server,so that send message to Maker MT task*/
	if(a_ret != CMD_OK)
	{
		a_tcpsrvinitrsp_p->result = -1;
		/*Send message to Maker MT task */
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}

		return ;
	}

	/*Call tm_lr_cm_SrvSocketInit to initialize tcp server for monitor RE1*/
	a_ret = tm_lr_cm_SrvSocketInit(&lrw_srvsocket[LRD_SRVMONITOR_RE1],
									a_monport_re1);
	/*Fail to initialize Tcp server,so that send message to Maker MT task*/
	if(a_ret != CMD_OK)
	{
		a_tcpsrvinitrsp_p->result = -1;
		/*Send message to Maker MT task */
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}

		return ;
	}

	/*Call tm_lr_cm_SrvSocketInit to initialize tcp server for monitor RE2*/
	a_ret = tm_lr_cm_SrvSocketInit(&lrw_srvsocket[LRD_SRVMONITOR_RE2],
									a_monport_re2);
	/*Fail to initialize Tcp server,so that send message to Maker MT task*/
	if(a_ret != CMD_OK)
	{
		a_tcpsrvinitrsp_p->result = -1;
		/*Send message to Maker MT task */
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}

		return ;
	}

	/*Call tm_lr_cm_SrvSocketInit to initialize tcp server for monitor RE3*/
	a_ret = tm_lr_cm_SrvSocketInit(&lrw_srvsocket[LRD_SRVMONITOR_RE3],
									a_monport_re3);
	/*Fail to initialize Tcp server,so that send message to Maker MT task*/
	if(a_ret != CMD_OK)
	{
		a_tcpsrvinitrsp_p->result = -1;
		/*Send message to Maker MT task */
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}

		return ;
	}

	/*Call tm_lr_cm_SrvSocketInit to initialize tcp server for monitor*/
	a_ret = tm_lr_cm_SrvSocketInit(&lrw_srvsocket[LRD_SRVOPTMT],
									a_remtport);
	/*Fail to initialize Tcp server,so that send message to Maker MT task*/
	if(a_ret != CMD_OK)
	{
		a_tcpsrvinitrsp_p->result = -1;
		/*Send message to Maker MT task */
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}

		return ;
	}
	
	/*Initialization is OK, so that, send message to Maker MT task*/
	a_tcpsrvinitrsp_p->head.uiLength = sizeof(CMT_TSKIF_TCPSRVSTRRSP);
	a_tcpsrvinitrsp_p->head.uiEventNo = CMD_TSKIF_TCPSRVSTRRSP;
	a_tcpsrvinitrsp_p->head.uiSrcTQueueID = CMD_TSKID_LNR;
	a_tcpsrvinitrsp_p->head.uiDstTQueueID = CMD_TSKID_MKM;
	a_tcpsrvinitrsp_p->result = lrw_srvsocket[LRD_SRVSIMULATOR];

	/* Send message to Maker MT task */
	a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,CMD_TSKIF_TCPSRVSTRRSP,
				(void*)a_tcpsrvinitrsp_p);
	if(a_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
	}
	
	/* start TCP server */
	tm_lr_TcpStr(0);
	
	return;		
}
/* @} */


/** @addtogroup RRH_MNT_LNR
 * @{ */


/********************************************************************************************************************/
/**
 *  @brief     Init TCP server with specified port	
 *  @note     Init TCP server with specified port				 					
 *  @param  a_port				[in]		TCP port number
 *  @param  a_srvsocket_p		[in]		Tcp link handle
 *  @return   None
 *  @retval   CMD_OK		0:ok
 *  @retval   CMD_NG		1:NG
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/
UINT tm_lr_cm_SrvSocketInit(INT*a_srvsocket_p,USHORT a_port)
{
	INT				a_ret;		/*	Return information				*/
	struct sockaddr_in			a_iaddr;	/*	Server socket address			*/
	INT				a_socdes;	/*	Socket()'s ret val,save srvsocket*/

	/*Confirm thata_srvsocket_p is not null*/
	if(a_srvsocket_p == NULL)
	{
		cm_Assert( CMD_ASL_DBGLOW, (UINT)a_srvsocket_p, "a_srvsocket_p NULL" );
		return CMD_NG;
	}
	/*Call socket function:socket to create sever socket*/
	a_socdes = socket( PF_INET, SOCK_STREAM, 0);
	if(a_socdes < SOCK_VALID)
	{
		lrw_errinfo.lr_socket = errno;
		cm_Assert( CMD_ASL_DBGLOW, lrw_errinfo.lr_socket, "socket error" );

		return CMD_NG;
	}
	a_iaddr.sin_family = AF_INET;	
	a_iaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	a_iaddr.sin_port = htons(a_port);

	/*Call socket function:bind to bind server socket to specified port*/
	a_ret = bind( a_socdes, (struct sockaddr *)&a_iaddr, sizeof(a_iaddr) );
	/*Fail to bind,so that, close server socket and return NG*/
	if( a_ret != SOCK_RES_OK ) 
	{
		/*Close server socket(2:  close read and write)*/
		a_ret = close(a_socdes);
		lrw_errinfo.lr_bind = errno;

		cm_Assert( CMD_ASL_DBGLOW, lrw_errinfo.lr_bind, "bind error" );

		return CMD_NG;
	}

	/*
	Call socket function:listen to places a socket a state where it is listening for 
	an incoming connection.
	*/
	a_ret = listen( a_socdes,SOCK_DEFBACKLOG);
	/*Fail to listen,so that, close server socket and return NG*/
	if( a_ret != SOCK_RES_OK ) 
	{
		a_ret = close(a_socdes);
		lrw_errinfo.lr_listen = errno;

		cm_Assert( CMD_ASL_DBGLOW, lrw_errinfo.lr_listen, "listen error" );

		return CMD_NG;
	}

	/*Return the created server socket*/
	*a_srvsocket_p = a_socdes;

	return CMD_OK;
}
/* @} */

