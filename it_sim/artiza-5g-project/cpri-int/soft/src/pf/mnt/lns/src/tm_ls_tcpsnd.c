/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_ls_TcpSnd.c
 *  @brief   Transmit data to client through established TCP link
 *  @date   2008/07/28  FFCS)zhengmh Create
 *  @date   2013/11/22  FFCS)zhaodx modify for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_LNS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "tm_ls_header.h"			/* LNS task header					*/
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

/********************************************************************************************************************/
/**
 *  @brief     Transmit data to client through established TCP link
 *  @note     Transmit data to client through established TCP link
 *  @param  a_inf_p 	[in]	point of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/

VOID tm_ls_TcpSnd(UINT * a_inf_p)
{
	CMT_TSKIF_TCPSNDREQ*	a_tcpsndreq_p;/*Buffer for task IF*/
	CMT_TSKIF_HEAD*			a_msgheader_p;/*Buffer for task IF*/
	INT						a_ret;/*Information returned by function*/		
	INT 					a_sockhandle;/*socket handle */	
	CHAR*					a_buffaddr_p;/*Buffer to transmit */
	INT						a_buffsize;/*The length of buffer to transmit*/
	CMT_TSKIF_TCPRELNTC*	a_tcpsndrsp_p;/*Buffer for task IF*/
	T_MSGHEAD				a_cardhead; /*common header between TRA and PC*/

	a_tcpsndreq_p = (CMT_TSKIF_TCPSNDREQ*)a_inf_p;	/*Type conversion*/

	if(a_tcpsndreq_p == NULL)
	{
		return;
	}
	
	a_sockhandle = a_tcpsndreq_p->linkhandle;	/*Get socket handle*/

	/* Fill card common header */
	a_cardhead.msgid = mnt_com_swap_UINT(a_tcpsndreq_p->msgevent);
	a_cardhead.allMsgLen = mnt_com_swap_UINT(a_tcpsndreq_p->msglen);
	a_cardhead.msgLen = (USHORT)mnt_com_swap_UINT(a_tcpsndreq_p->msglen);
	a_cardhead.srcFbNo = CMD_NUM0;
	a_cardhead.next = CMD_NUM0;
	a_cardhead.replyId = CMD_NUM0;
	a_cardhead.headSeqNo = CMD_NUM0;
	a_cardhead.nowSeqNo = CMD_NUM0;

	if((a_sockhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC    ].tcplinkhdl)
	|| (a_sockhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].tcplinkhdl)
	|| (a_sockhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].tcplinkhdl)
	|| (a_sockhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].tcplinkhdl)) /* pgr0012 pgr0860 */
	{
		a_cardhead.dstFbNo = mnt_com_swap_UINT(CMD_FBNO_CPRI_SIM);
	}
	else if(
	   (a_sockhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC    ].tcplinkhdl)  /* pgr0012 pgr0860 */
	|| (a_sockhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE01_06].tcplinkhdl)  /* pgr0012 pgr0860 */
	|| (a_sockhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE07_12].tcplinkhdl)  /* pgr0012 pgr0860 */
	|| (a_sockhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE13_16].tcplinkhdl)) /* pgr0012 pgr0860 */
	{
		a_cardhead.dstFbNo = mnt_com_swap_UINT(CMD_FBNO_CPRI_MON);
	}
	else if(a_sockhandle == cmw_tblClientInfo[CMD_TCPCONN_NUM_FHMMT].tcplinkhdl) /* pgr0012 pgr0860 */
	{
		a_cardhead.dstFbNo = mnt_com_swap_UINT(CMD_FBNO_REMT);
	}
	
	/*First tranmit the common header*/
	a_ret = tm_ls_cm_send(a_sockhandle,(char*)&a_cardhead,sizeof(T_MSGHEAD));
		
		/*Fail to transmit the data,so that,to make and send message to  Maker MT task*/
	if(a_ret <= SOCK_RES_OK)
	{
			/*Hunter buffer for task IF*/
		a_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC),(void**)&a_tcpsndrsp_p);
		if( a_ret != CMD_RES_OK)
		{
				/*Error log*/
			cm_MAbort(CMD_ALMCD_BUFGET, "tm_ls_TcpSnd", 
						"Buffer for CMT_TSKIF_TCPSNDRSP is null:1 ",a_ret,
						 (UINT)(sizeof(CMT_TSKIF_TCPRELNTC)), CMD_NUM0);

		}

		a_tcpsndrsp_p->linkhandle = a_sockhandle;
			
		a_tcpsndrsp_p->head.uiLength = sizeof(CMT_TSKIF_TCPRELNTC);
		a_tcpsndrsp_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
		a_tcpsndrsp_p->head.uiSrcTQueueID = CMD_TSKID_LNS;
		a_tcpsndrsp_p->head.uiDstTQueueID = CMD_TSKID_MKM;
		a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_EMERGENCY,
				CMD_TSKIF_TCPRELNTC,(void*)a_tcpsndrsp_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
		}
		
		cm_Assert(CMD_ASL_DBGLOW, a_sockhandle, "SOCK_SD_ZERO");
		
		/*Free buffer for msg body*/
		if(a_tcpsndreq_p->msgbody != NULL)
		{
			a_ret = (INT)cm_BFree(a_tcpsndreq_p->msgbody);
			if(a_ret != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_ls_TcpSnd",
						  "Free buffer for taskIF  NG:3", a_ret, (UINT)a_tcpsndreq_p->msgbody, CMD_NUM0);
			}
		}
		/*Free buffer for task IF*/
		a_ret = (INT)cm_BFree(a_inf_p);
		if(a_ret != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "tm_ls_TcpSnd",
					  "Free buffer for taskIF  NG:4", a_ret, (UINT)a_inf_p, CMD_NUM0);
		}
		return;				
	}

	/*Next tranmit the body*/		
	if(a_tcpsndreq_p->msgbody == NULL)
	{
//‹[Ž—ŠÂ‹« 20210312
//		a_buffaddr_p = (CHAR*)&(a_tcpsndreq_p->rtc_rsp);	/*Get buffer to transmit*/
		// \‘¢‘ÌCMT_TSKIF_MKMTHEAD‚Ìpadding‘Î‰ž‚Ì‚½‚ß4byteˆÚ“®
		a_buffaddr_p = (CHAR*)&(a_tcpsndreq_p->rtc_rsp.rsp.cause);	/*Get buffer to transmit*/
//‹[Ž—ŠÂ‹« 20210312
		
		a_buffsize = a_tcpsndreq_p->msglen;		/*Get buffer length*/
				
		/*Call socket function:send to tranmit the data*/
		if(a_buffsize > CMD_NUM0)
		{
			a_ret = tm_ls_cm_send(a_sockhandle,(char*)a_buffaddr_p,a_buffsize);
		}
		else
		{
			/*forcely,set a_ret OK*/
			a_ret = CMD_NUM1;
		}

		/*Fail to transmit the data,so that,to make and send message to  Maker MT task*/
		if(a_ret <= SOCK_RES_OK)
		{
			/*Hunter buffer for task IF*/
			a_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC),(void**)&a_tcpsndrsp_p);
			if( a_ret != CMD_RES_OK)
			{
				/*Error log*/
				cm_MAbort(CMD_ALMCD_BUFGET, "tm_ls_TcpSnd", 
						"Buffer for CMT_TSKIF_TCPSNDRSP is null:1 ",a_ret,
						 (UINT)(sizeof(CMT_TSKIF_TCPRELNTC)), CMD_NUM0);

			}

			a_tcpsndrsp_p->linkhandle = a_sockhandle;
			
			a_tcpsndrsp_p->head.uiLength = sizeof(CMT_TSKIF_TCPRELNTC);
			a_tcpsndrsp_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
			a_tcpsndrsp_p->head.uiSrcTQueueID = CMD_TSKID_LNS;
			a_tcpsndrsp_p->head.uiDstTQueueID = CMD_TSKID_MKM;
			a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_EMERGENCY,
				CMD_TSKIF_TCPRELNTC,(void*)a_tcpsndrsp_p);
			if(a_ret != CMD_RES_OK)
			{
				cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
			}

			cm_Assert(CMD_ASL_DBGLOW, a_sockhandle, "SOCK_SD_ZERO");
		}
	}
	else
	{
//‹[Ž—ŠÂ‹« 20210312
//		a_buffaddr_p = (CHAR*)&(a_tcpsndreq_p->rtc_rsp);/*Get data#1 to transmit*/
		// \‘¢‘ÌCMT_TSKIF_MKMTHEAD‚Ìpadding‘Î‰ž‚Ì‚½‚ß4byteˆÚ“®
		a_buffaddr_p = (CHAR*)&(a_tcpsndreq_p->rtc_rsp.rsp.cause);/*Get data#1 to transmit*/
//‹[Ž—ŠÂ‹« 20210312
		/*Type conversion for data#2*/
		a_msgheader_p = (CMT_TSKIF_HEAD*)(a_tcpsndreq_p->msgbody);
		/*Caclue the data#1 length*/
		a_buffsize = a_tcpsndreq_p->msglen - (a_msgheader_p->uiLength - sizeof(CMT_TSKIF_HEAD)) ;		

		if(a_buffsize > CMD_NUM0)
		{
			/*Call socket function:send to tranmsit data#1*/
			a_ret = tm_ls_cm_send(a_sockhandle,(char*)a_buffaddr_p,a_buffsize);
		}
		else
		{
			/*forcely,set a_ret OK*/
			a_ret = CMD_NUM1;
		}
		/*
		Fail to transmit the data,so that,so that to make and send message to  Maker MT task
		*/
		if(a_ret <= SOCK_RES_OK)
		{
			a_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC),(void**)&a_tcpsndrsp_p);
			if(a_ret != CMD_RES_OK)
			{
				/*Error log*/
				cm_MAbort(CMD_ALMCD_BUFGET, "tm_ls_TcpSnd", 
						"Buffer for CMT_TSKIF_TCPSNDRSP is null:2 ",a_ret,
						 (UINT)(sizeof(CMT_TSKIF_TCPRELNTC)), CMD_NUM0);
			}

			a_tcpsndrsp_p->linkhandle = a_sockhandle;
			
			a_tcpsndrsp_p->head.uiLength = sizeof(CMT_TSKIF_TCPRELNTC);
			a_tcpsndrsp_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
			a_tcpsndrsp_p->head.uiSrcTQueueID = CMD_TSKID_LNS;
			a_tcpsndrsp_p->head.uiDstTQueueID = CMD_TSKID_MKM;
			a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_EMERGENCY,
				CMD_TSKIF_TCPRELNTC,(void*)a_tcpsndrsp_p);
			if(a_ret != CMD_RES_OK)
			{
				cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
			}
			
			cm_Assert(CMD_ASL_DBGLOW, a_sockhandle, "SOCK_SD_ZERO");
			
			a_ret = (INT)cm_BFree(a_tcpsndreq_p->msgbody);
			if(a_ret != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_ls_TcpSnd",
						  "Free buffer for taskIF  NG:1", a_ret, (UINT)a_tcpsndreq_p->msgbody, CMD_NUM0);
			}
			a_ret = (INT)cm_BFree(a_inf_p);
			if(a_ret != CMD_RES_OK)
			{
				cm_MAbort(CMD_ALMCD_BUFREL, "tm_ls_TcpSnd",
						  "Free buffer for taskIF  NG:2", a_ret, (UINT)a_inf_p, CMD_NUM0);
			}
			
			return;
		}

		/*Get data#2to transmit*/
		a_buffaddr_p = (CHAR*)(a_tcpsndreq_p->msgbody) + sizeof(CMT_TSKIF_HEAD);
		a_buffsize = a_msgheader_p->uiLength - sizeof(CMT_TSKIF_HEAD);/*Caclue the data#2 length*/

		/*Call socket function:send to tranmsit data#2*/
		a_ret = tm_ls_cm_send(a_sockhandle,(char*)a_buffaddr_p,a_buffsize);

		/*
		Fail to transmit the data,so that,so that to make and send message to Maker MT task
		*/
		if(a_ret <= SOCK_RES_OK)
		{
			a_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC),(void**)&a_tcpsndrsp_p);
			if(a_ret != CMD_RES_OK)
			{
				/*Error log*/
				cm_MAbort(CMD_ALMCD_BUFGET, "tm_ls_TcpSnd", 
						"Buffer for CMT_TSKIF_TCPSNDRSP is null:3 ",
						a_ret, (UINT)(sizeof(CMT_TSKIF_TCPRELNTC)), CMD_NUM0);

			}

			a_tcpsndrsp_p->linkhandle = a_sockhandle;

			cm_Assert(CMD_ASL_DBGLOW, a_sockhandle, "SOCK_SD_ZERO");
			
			a_tcpsndrsp_p->head.uiLength = sizeof(CMT_TSKIF_TCPRELNTC);
			a_tcpsndrsp_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
			a_tcpsndrsp_p->head.uiSrcTQueueID = CMD_TSKID_LNS;
			a_tcpsndrsp_p->head.uiDstTQueueID = CMD_TSKID_MKM;
			a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_EMERGENCY,
				CMD_TSKIF_TCPRELNTC,(void*)a_tcpsndrsp_p);
			if(a_ret != CMD_RES_OK)
			{
				cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
			}

		}		
		
	}

	/*Free buffer for msg body*/
	if(a_tcpsndreq_p->msgbody != NULL)
	{
		a_ret = (INT)cm_BFree(a_tcpsndreq_p->msgbody);
		if(a_ret != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL, "tm_ls_TcpSnd",
					  "Free buffer for taskIF  NG:3", a_ret, (UINT)a_tcpsndreq_p->msgbody, CMD_NUM0);
		}
	}
	/*Free buffer for task IF*/
	a_ret = (INT)cm_BFree(a_inf_p);
	if(a_ret != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFREL, "tm_ls_TcpSnd",
				  "Free buffer for taskIF  NG:4", a_ret, (UINT)a_inf_p, CMD_NUM0);
	}
	return;
}
/* @} */


/** @addtogroup RRH_MNT_LNS
 * @{ */


/********************************************************************************************************************/
/**
 *  @brief     Write TCP data in loop
 *  @note     Write TCP data in loop
 *  @param  sock 	[in]	Tcp link handle	
 *  @param  pbuf 	[in]	the buffer to save the write data
 *  @param  len 	[in]	the count of data to write
 *  @return   result code is return 
 *  @retval   len
 *  @retval   CMD_NUM_MINUS1			-1:minimum number 
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/

int tm_ls_cm_send(int sock,char* pbuf,int len)
{
	INT 			sndlen;
	INT 			ret;
	INT				a_wait;
		
	a_wait = CMD_NUM0;
	sndlen = CMD_NUM0;

	while(sndlen < len)
	{
		ret = send(sock,(char*)&pbuf[sndlen],len - sndlen,CMD_NUM0);
		if(ret <= CMD_NUM0)
		{
			if(errno != EAGAIN)
			{
				ret = shutdown(sock,2);
				ret = close(sock);

				cm_Assert( CMD_ASL_DBGLOW, (UINT)errno, "tm_ls_cm_send Error" );
				return CMD_NUM_MINUS1;
			}
			usleep(100000);
			if(a_wait++ >= 20)
			{
				ret = shutdown(sock,2);
				ret = close(sock);
				
				cm_Assert( CMD_ASL_DBGLOW, (UINT)len, "tm_ls_cm_send Error" );
				return CMD_NUM_MINUS1;
			}		

			continue;
		}		

		sndlen += ret;	
	}
	
	return len; 
}
/* @} */
