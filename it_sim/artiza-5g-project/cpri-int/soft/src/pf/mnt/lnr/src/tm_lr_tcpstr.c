/********************************************************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		tm_lr_TcpStr.c
 *  @brief		process TCP server start request
 *  @date		2008/07/28  FFCS)zhengmh Create
 *  @date       2013/11/22 FFCS)zhaodx modify for zynq
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
#include <sys/select.h>
#include <linux/tcp.h>
#include <linux/socket.h>
#include <fcntl.h>
#include "m_mk_def.h" 
#include "m_mk_typ.h" 
#include "m_mk_ext.h" 

extern int tm_lr_cm_ResetFd(int *,int , fd_set *);

/********************************************************************************************************************/
/**
 *  @brief     process TCP server start request 
 *  @note      1.Accept client TCP link connection, then,make message [TCP connection notification],and send it to MKM task		
 		      2.Read client data,then,make message [TCP data receiving notification],and send it to MKM task		 	 
		      3.Detect client disconnection,make message [TCP relese notification],and send it to  MKM task		 					
 *  @param    a_inf_p [in] pointer of received message
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 *  @date     2011/01/17  FJT)Tokunaga M-S3G-eNBSYS-02198 ; ☆ST_ENB☆TS085項【10B】;LREに対してeNB-MTの接続確認中にLREがALMに遷移
 *  @date     2015/10/24 TDIPS)sasaki Warning対処
 */
/********************************************************************************************************************/
VOID tm_lr_TcpStr(UINT * a_inf_p)
{
	INT						a_sel;/*Information returned by function*/
	INT						a_ret;/*Information returned by function*/
	INT						a_opt;/*Information returned by function*/
	fd_set					a_newfs;/*The later file descriptor*/
	fd_set					a_errfs;/*The file descriptor used for error*/
	fd_set					a_curfs;/*The file descriptor used currently*/
	INT						a_maxfd;/*The max value of tcp link handle*/
	UINT					a_msglen;/*The msg length of arrival data*/
	CHAR*					a_msgbuff_p;/*The msg body of arrival data*/
	T_MSGHEAD				a_cardhead; /*common header between TRA and PC*/
	CMT_TSKIF_TCPCONNNTC*	a_tcpconnntc_p;/*Buffer for Task IF to Maker MT task*/
	CMT_TSKIF_TCPRELNTC*	a_tcprelntc_p;/*Buffer for Task IF to Maker MT task*/
	CMT_TSKIF_TCPDATNTC*	a_tcpdatntc_p;/*Buffer for Task IF to Maker MT task*/
	INT						a_lingeropt[CMD_NUM2]={CMD_NUM1,CMD_NUM0};
	struct sockaddr			a_addr;		/*Client socket address	*/
	UINT					a_addrlen;  /*Client socket address length*/
	UINT					a_i;/*It is used in loop*/
	UINT					a_j;/*It is used in loop*/
							/*The table saving server socket handle and established tcp link*/
	INT						a_fdtbl[LRD_MAXFD] = {SOCK_INVALID};
	INT						a_newsocket;/*The new established tcp link*/
	INT						a_hasdat = CMD_OFF;/*flag to check link is on/off*/	
	INT						a_mode[LRD_MAXFD] = {0};
	
	a_maxfd = SOCK_VALID;
	/*Call socket macro to initiaze a_newfs and a_curfs*/
	FD_ZERO(&a_newfs);
	FD_ZERO(&a_curfs);
	FD_ZERO(&a_errfs);

	/*Call socket macro to add server socke handle for simulator to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVSIMULATOR],&a_newfs);
	a_maxfd = lrw_srvsocket[LRD_SRVSIMULATOR];
	/*Call socket macro to add server socke handle for simulator RE1 to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVSIMULATOR_RE1],&a_newfs);
	if(lrw_srvsocket[LRD_SRVSIMULATOR_RE1] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVSIMULATOR_RE1];
	/*Call socket macro to add server socke handle for simulator RE2 to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVSIMULATOR_RE2],&a_newfs);
	if(lrw_srvsocket[LRD_SRVSIMULATOR_RE2] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVSIMULATOR_RE2];
	/*Call socket macro to add server socke handle for simulator RE3 to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVSIMULATOR_RE3],&a_newfs);
	if(lrw_srvsocket[LRD_SRVSIMULATOR_RE3] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVSIMULATOR_RE3];
	/*Call socket macro to add server socke handle for monitor to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVMONITOR],&a_newfs);
	if(lrw_srvsocket[LRD_SRVMONITOR] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVMONITOR];
	/*Call socket macro to add server socke handle for monitor RE1 to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVMONITOR_RE1],&a_newfs);
	if(lrw_srvsocket[LRD_SRVMONITOR_RE1] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVMONITOR_RE1];
	/*Call socket macro to add server socke handle for monitor RE2 to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVMONITOR_RE2],&a_newfs);
	if(lrw_srvsocket[LRD_SRVMONITOR_RE2] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVMONITOR_RE2];
	/*Call socket macro to add server socke handle for monitor RE3 to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVMONITOR_RE3],&a_newfs);
	if(lrw_srvsocket[LRD_SRVMONITOR_RE3] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVMONITOR_RE3];
	FD_SET(lrw_srvsocket[LRD_SRVOPTMT],&a_newfs);
	if(lrw_srvsocket[LRD_SRVOPTMT] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVOPTMT];

	cm_Assert( CMD_ASL_DBGLOW, lrw_srvsocket[LRD_SRVSIMULATOR], "tcp server startup" );

	/*An unlimited loop*/
	for(;;)
	{
		a_errfs = a_curfs = a_newfs;
		/*Call socket function:select to detect event:tcp */
		a_sel = select(a_maxfd + CMD_NUM1,&a_curfs,NULL,&a_errfs,NULL);
		/* detect MT address setting request*/
		if(cmw_mkaddr_reset != CMD_OFF)
		{
			cm_Assert(CMD_ASL_DBGLOW, cmw_mkaddr_reset, "cmw_mkaddr_reset != CMD_OFF");
			break;/* jump out*/
		}
	
		if(a_sel < SOCK_SEL_TO)
		{
			if(errno == EBADF  /* invalid fd*/)
			{
				a_maxfd = tm_lr_cm_ResetFd(a_fdtbl, LRD_MAXFD,&a_newfs);
				continue;
			}
			lrw_errinfo.lr_select = errno;
			/*Set Alarm*/
			cm_MAbort(CMD_ALMCD_RTOS, "tm_lr_TcpStr", 
								"select return err",
								a_maxfd, lrw_errinfo.lr_select, CMD_NUM0);
		}
		/*In the below loop, the process of accepting new tcp link connection is performed*/
		a_hasdat = CMD_OFF;
		for(a_i = CMD_NUM0;a_i < LRD_SRVCOUNT;a_i++)
		{
			if(mkw_tblClientInfo[a_i].conn == MKD_CONN_Y)
			{
				continue;
			}

			/*Call socket  macro: to decide whether there is new connection*/
			if(FD_ISSET(lrw_srvsocket[a_i],&a_curfs))
			{

				a_hasdat = CMD_ON;
				a_addrlen = sizeof(struct sockaddr);
				cm_MemClr(&a_addr,a_addrlen);
				
				a_newsocket  = accept(lrw_srvsocket[a_i],&a_addr,&a_addrlen);

				if(a_newsocket <= SOCK_VALID)
				{
					cm_Assert( CMD_ASL_DBGLOW, (UINT)a_newsocket, "accept error1" );
					lrw_errinfo.lr_accept = errno;
					continue;
				}

				a_ret = fcntl(a_newsocket,F_SETFL,O_NONBLOCK);
					
				a_ret |= setsockopt( a_newsocket, (INT)SOL_SOCKET, SO_LINGER, (char*)a_lingeropt, sizeof(a_lingeropt) );
				a_opt = 1;
				a_ret |= setsockopt(a_newsocket,(INT)SOL_SOCKET,SO_KEEPALIVE,(char*)&a_opt,sizeof(a_opt));
				a_opt = CMD_NUM5;			
				a_ret |= setsockopt(a_newsocket,(INT)SOL_TCP,TCP_KEEPIDLE,(char*)&a_opt,sizeof(a_opt));
				a_opt = CMD_NUM3;
				a_ret |= setsockopt(a_newsocket,(INT)SOL_TCP,TCP_KEEPINTVL,(char*)&a_opt,sizeof(a_opt));
				a_opt = CMD_NUM3;
				a_ret |= setsockopt(a_newsocket,(INT)SOL_TCP,TCP_KEEPCNT,(char*)&a_opt,sizeof(a_opt));

				if(a_ret != 0)
					cm_Assert( CMD_ASL_DBGLOW, (UINT)a_newsocket, "tcp socket option ng" );
				
		
				/*Find an blank in table a_fdtbl*/
				for(a_j = CMD_NUM0;a_j < LRD_MAXFD;a_j++)
				{
					if(a_fdtbl[a_j] <= SOCK_VALID)
					{
						break;
					}						
				}
				/*There is not blank,so that , close this tcplink in initial action*/
				if(a_j >= LRD_MAXFD)
				{
					cm_Assert( CMD_ASL_DBGLOW, (UINT)a_j, "accept error3" );
					a_ret = close(a_newsocket);/*Disconnect TCP link in initiative */
					continue;
				}

				/*Call common function to get buffer for task if:*/
				a_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPCONNNTC),(void**)&a_tcpconnntc_p);
				if( a_ret != CMD_RES_OK)
				{
					cm_Assert( CMD_ASL_DBGLOW, (UINT)a_ret, "accept error4" );
					a_ret = close(a_newsocket);/*Disconnect TCP link in initiative */

					cm_MAbort(CMD_ALMCD_BUFGET, "tm_lr_TcpStr", 
								"Buffer for CMT_TSKIF_TCPCONNNTC is null ",
								a_ret,(UINT)(sizeof(CMT_TSKIF_TCPCONNNTC)),
								(UINT)a_newsocket);

					continue;
				}

				/*Set linktype parameter according to server socket */
				if(a_i == LRD_SRVSIMULATOR)
				{
					a_tcpconnntc_p->linktype = CMD_LINKTYPE_MKSIMU;
					a_tcpconnntc_p->sim_mon_no = CMD_TCPCONN_NUM_SIMNO_REC;
				}
				else if(a_i == LRD_SRVSIMULATOR_RE1)
				{
					a_tcpconnntc_p->linktype = CMD_LINKTYPE_MKSIMU;
					a_tcpconnntc_p->sim_mon_no = CMD_TCPCONN_NUM_SIMNO_RE01_06;
				}
				else if(a_i == LRD_SRVSIMULATOR_RE2)
				{
					a_tcpconnntc_p->linktype = CMD_LINKTYPE_MKSIMU;
					a_tcpconnntc_p->sim_mon_no = CMD_TCPCONN_NUM_SIMNO_RE07_12;
				}
				else if(a_i == LRD_SRVSIMULATOR_RE3)
				{
					a_tcpconnntc_p->linktype = CMD_LINKTYPE_MKSIMU;
					a_tcpconnntc_p->sim_mon_no = CMD_TCPCONN_NUM_SIMNO_RE13_16;
				}
				else if(a_i == LRD_SRVMONITOR)
				{
					a_tcpconnntc_p->linktype = CMD_LINKTYPE_MKMON;
					a_tcpconnntc_p->sim_mon_no = CMD_TCPCONN_NUM_MONNO_REC;
				}
				else if(a_i == LRD_SRVMONITOR_RE1)
				{
					a_tcpconnntc_p->linktype = CMD_LINKTYPE_MKMON;
					a_tcpconnntc_p->sim_mon_no = CMD_TCPCONN_NUM_MONNO_RE01_06;
				}
				else if(a_i == LRD_SRVMONITOR_RE2)
				{
					a_tcpconnntc_p->linktype = CMD_LINKTYPE_MKMON;
					a_tcpconnntc_p->sim_mon_no = CMD_TCPCONN_NUM_MONNO_RE07_12;
				}
				else if(a_i == LRD_SRVMONITOR_RE3)
				{
					a_tcpconnntc_p->linktype = CMD_LINKTYPE_MKMON;
					a_tcpconnntc_p->sim_mon_no = CMD_TCPCONN_NUM_MONNO_RE13_16;
				}
				else
				{
					a_tcpconnntc_p->linktype = CMD_LINKTYPE_RE;
					a_tcpconnntc_p->sim_mon_no = CMD_TCPCONN_NUM_FHMMT;
				}

				/*Set new established link handle to linkhandle*/
				a_tcpconnntc_p->linkhandle = a_newsocket;
				/*Send message TCP接続通知」to Maker MT task*/
				a_tcpconnntc_p->head.uiLength = sizeof(CMT_TSKIF_TCPCONNNTC);
				a_tcpconnntc_p->head.uiEventNo = CMD_TSKIF_TCPCONNNTC;
				a_tcpconnntc_p->head.uiSrcTQueueID = CMD_TSKID_LNR;
				a_tcpconnntc_p->head.uiDstTQueueID = CMD_TSKID_MKM;
				a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,
						CMD_TSKIF_TCPCONNNTC,a_tcpconnntc_p);
				if(a_ret != CMD_RES_OK)
				{
					cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
				}

				cm_Assert( CMD_ASL_NOLEVEL, a_newsocket, "new socket is accept" );
				/*Add the established link:a_newsocket to file descriptor*/
				a_fdtbl[a_j] = a_newsocket;
				a_mode[a_j] = a_i;
				FD_SET(a_newsocket,&a_newfs);
				if(a_newsocket > a_maxfd)
				{
					a_maxfd = a_newsocket;
				}
			}
		}

		/*
		In the below loop, the process of detecting arrival data or disconnection request
		in established tcp link
		*/
		for(a_i = CMD_NUM0;a_i < LRD_MAXFD;a_i++)
		{
			if(a_fdtbl[a_i] <= SOCK_VALID)
			{
				continue;
			}

			/*Call socket  macro: to decide whether new data is arrival*/
			if(FD_ISSET(a_fdtbl[a_i] ,&a_curfs))
			{
				a_hasdat = CMD_ON;
				/*Call tm_lr_cm_TcpRead to read message length info*/
				a_ret = tm_lr_cm_TcpRead(a_fdtbl[a_i],
					(CHAR*)&a_cardhead,sizeof(a_cardhead),LRD_MAXDURATION);
				
				/*
				Disconnection is detected, so that, make and send  TCP解放通知」
				to Maker MT task
				*/
				if(a_ret == LRD_TCPRD_ERROR)
				{
					cm_Assert( CMD_ASL_NOLEVEL, (UINT)a_fdtbl[a_i], "receive NG0" );
					cm_Assert( CMD_ASL_NOLEVEL, (UINT)a_maxfd, "receive NG0 a_maxfd" );
					a_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC),(void**)&a_tcprelntc_p);
					if(a_ret != CMD_RES_OK)
					{
						cm_MAbort(CMD_ALMCD_BUFGET, "tm_lr_TcpStr", 
								"Buffer for CMT_TSKIF_TCPRELNTC is null ",
								a_ret, (UINT)(sizeof(CMT_TSKIF_TCPRELNTC)),
								(UINT)a_fdtbl[a_i]);

						continue;
					}
					
					a_tcprelntc_p->linkhandle = a_fdtbl[a_i];/*TCP link handle setting*/
					a_tcprelntc_p->head.uiLength = sizeof(CMT_TSKIF_TCPRELNTC);
					a_tcprelntc_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
					a_tcprelntc_p->head.uiSrcTQueueID = CMD_TSKID_LNR;
					a_tcprelntc_p->head.uiDstTQueueID = CMD_TSKID_MKM;
					a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_EMERGENCY,
						CMD_TSKIF_TCPRELNTC,a_tcprelntc_p);
					if(a_ret != CMD_RES_OK)
					{
						cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
					}
					/*Remove the established link from file descriptor*/
					a_ret = close(a_fdtbl[a_i]);

					FD_CLR(a_fdtbl[a_i],&a_newfs);
					a_fdtbl[a_i] = SOCK_INVALID;
					
					continue;
				}
				
				a_msglen = mnt_com_swap_UINT(a_cardhead.allMsgLen);
				
				/*Duration expires. so that,close this tcplink in initial action*/
				if(a_ret == LRD_TCPRD_TIMEOUT)
				{
					cm_Assert( CMD_ASL_DBGLOW, (UINT)a_ret, "receive NG1" );
					a_ret = shutdown(a_fdtbl[a_i] ,SOCK_SHUT_RD);/*Disconnect TCP link in initiative */
					continue;
				}

				/*Hunter buffer for task IF:*/
				a_ret = cm_BReq(CMD_BUFCA_TSKIF, a_msglen + sizeof(CMT_TSKIF_HEAD) + CMD_NUM5 * sizeof(INT) ,
								(VOID**)&a_tcpdatntc_p);
				if( a_ret != CMD_RES_OK)
				{
					a_ret = close(a_fdtbl[a_i]);

					cm_MAbort(CMD_ALMCD_BUFGET, "tm_lr_TcpStr", 
							"Buffer for CMT_TSKIF_TCPDATNTC is null ",
							a_ret, 
							(UINT)(a_msglen + sizeof(CMT_TSKIF_HEAD) + CMD_NUM3 * sizeof(INT)),
							(UINT)a_fdtbl[a_i]);

					continue;
				}
				
				a_msgbuff_p = (CHAR*)(a_tcpdatntc_p->msgbody);/*Buffer addr setting*/
				a_tcpdatntc_p->linkhandle = a_fdtbl[a_i];/*TCP link handle setting*/
				/*If not heart beat request,process as follow    */
				if (a_cardhead.msgid != mnt_com_swap_UINT(0x520b0000))
				{
					/*Call tm_lr_cm_TcpRead to read message body*/
					a_ret = tm_lr_cm_TcpRead(a_fdtbl[a_i],
						a_msgbuff_p,(INT)a_msglen ,LRD_MAXDURATION);

					if(a_ret == LRD_TCPRD_ERROR)
					{
						cm_Assert( CMD_ASL_DBGLOW, (UINT)a_ret, "receive NG2" );
						a_tcpdatntc_p->head.uiLength = a_msglen + sizeof(CMT_TSKIF_HEAD) + CMD_NUM5 * sizeof(INT);
						a_tcpdatntc_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
						a_tcpdatntc_p->head.uiSrcTQueueID = CMD_TSKID_LNR;
						a_tcpdatntc_p->head.uiDstTQueueID = CMD_TSKID_MKM;
						a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_EMERGENCY,
							CMD_TSKIF_TCPRELNTC,a_tcpdatntc_p);
						if(a_ret != CMD_RES_OK)
						{
							cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
						}
						/*Remove the established link from file descriptor*/
						a_ret = close(a_fdtbl[a_i]);

						FD_CLR(a_fdtbl[a_i],&a_newfs);
						a_fdtbl[a_i] = SOCK_INVALID;
						continue;
					}
					/*Duration expires. so that,close this tcplink in initial action*/
					if(a_ret == LRD_TCPRD_TIMEOUT)
					{
						cm_Assert( CMD_ASL_DBGLOW, (UINT)a_ret, "receive NG3" );
						a_ret = shutdown(a_fdtbl[a_i] ,SOCK_SHUT_RD);/*Disconnect TCP link in initiative */
						continue;
					}
				}
				/*Send message TCPデータ受信通知」to Maker MT task*/
				a_tcpdatntc_p->msglen = a_msglen;
				a_tcpdatntc_p->msgevent = a_cardhead.msgid;
				a_tcpdatntc_p->linkhandle = a_fdtbl[a_i];
				a_tcpdatntc_p->head.uiLength = a_msglen + sizeof(CMT_TSKIF_HEAD) + CMD_NUM5 * sizeof(INT);
				a_tcpdatntc_p->head.uiEventNo = CMD_TSKIF_TCPDATNTC;
				a_tcpdatntc_p->head.uiSrcTQueueID = CMD_TSKID_LNR;
				a_tcpdatntc_p->head.uiDstTQueueID = CMD_TSKID_MKM;

				a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_NORMAL,
						CMD_TSKIF_TCPDATNTC,a_tcpdatntc_p);
				if(a_ret != CMD_RES_OK)
				{
					cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
				}
				
			}
			else if(FD_ISSET(a_fdtbl[a_i] ,&a_errfs))
			{
				/*Check whether error is existent*/
				a_hasdat = CMD_ON;

				/*Because of error occurence, close socket*/
				a_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC),(void**)&a_tcprelntc_p);
				if(a_ret != CMD_RES_OK)
				{
					cm_MAbort(CMD_ALMCD_BUFGET, "tm_lr_TcpStr", 
								"Buffer for CMT_TSKIF_TCPRELNTC is null ",
								a_ret, (UINT)(sizeof(CMT_TSKIF_TCPRELNTC)),
								(UINT)a_fdtbl[a_i]);

					continue;
				}
					
				cm_Assert( CMD_ASL_DBGLOW, (UINT)a_i, "receive NG4" );
				a_tcprelntc_p->linkhandle = a_fdtbl[a_i];/*TCP link handle setting*/
				a_tcprelntc_p->head.uiLength = sizeof(CMT_TSKIF_TCPRELNTC);
				a_tcprelntc_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
				a_tcprelntc_p->head.uiSrcTQueueID = CMD_TSKID_LNR;
				a_tcprelntc_p->head.uiDstTQueueID = CMD_TSKID_MKM;
				a_ret = cm_Enter(CMD_TSKID_MKM,CMD_QRB_EMERGENCY,
						CMD_TSKIF_TCPRELNTC,a_tcprelntc_p);
				if(a_ret != CMD_RES_OK)
				{
					cm_Assert(CMD_ASL_DBGHIGH, a_ret, "cm_Enter NG" );
				}

				/*Remove the established link from file descriptor*/
				a_ret = close(a_fdtbl[a_i]);

				FD_CLR(a_fdtbl[a_i],&a_newfs);
				a_fdtbl[a_i] = SOCK_INVALID;
			}
		}
		
		if(a_hasdat == CMD_ON)
		{
			continue;
		}
		if(a_sel  != SOCK_SEL_TO)
		{
			lrw_errinfo.lr_select = -CMD_NUM1;
			lrw_errinfo.lr_listen++;
			cm_MWait(CMD_NUM2);
		}
		else
		{
			lrw_errinfo.lr_select = CMD_NUM0;
			lrw_errinfo.lr_listen = CMD_NUM0;
		}		
	}
	
	return;
}
/* @} */


/** @addtogroup RRH_MNT_LNR
 * @{ */


/********************************************************************************************************************/
/**
 *  @brief     Read TCP data with specified length in specified duration time
 *  @note     Read TCP data with specified length in specified duration time
 *  @param  a_sockethandle		[in]		Tcp link handle
 *  @param  a_buff_p			[in]		the buffer to save the read data
 *  @param  a_bufflen			[in]		the count of data to read
 *  @param  a_duration			[in]		duration time	
 *  @return   result code is return 
 *  @retval   LRD_TCPRD_ERROR		-1:Fail to receive tcp data
 *  @retval   LRD_TCPRD_TIMEOUT	0  :Timerout occurence 
 *  @retval   LRD_TCPRD_OK		1  :Success in receiving tcp data 
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/
INT tm_lr_cm_TcpRead(INT a_sockethandle, CHAR * a_buff_p, INT a_bufflen, 
	INT a_duration)
{
	INT				a_ret;	/*Information returned by function*/
	INT				a_count;/*The count of read data*/
	INT				a_wait;
	fd_set			a_newfs;/*The later file descriptor*/
	struct timeval tv;

	/*Confirm a_buff_p is not null*/
	if(a_buff_p == NULL)
	{
		cm_Assert( CMD_ASL_DBGLOW, (UINT)a_buff_p, "tm_lr_cm_TcpRead NULL NG" );
		
		return LRD_TCPRD_ERROR;
	}
	if(a_bufflen > 10270 ) /* max is CMT_CPRIF_FILEDATSND or CMT_CPRIF_UFILEDATSND + head of MKT_CPRISIMUDATRCV */
	{
		cm_Assert( CMD_ASL_DBGLOW, (UINT)a_bufflen, "tm_lr_cm_TcpRead length is very big" );
		
		return LRD_TCPRD_ERROR;
	}
	
	a_count = CMD_NUM0;
	a_wait  = CMD_NUM0;
	
	for(;;)
	{
		tv.tv_sec = 0;
		tv.tv_usec = 100*1000;
		FD_ZERO(&a_newfs);
		FD_SET(a_sockethandle,&a_newfs);

		a_ret = select(a_sockethandle + 1,&a_newfs,NULL,NULL,&tv);
		if(a_ret < SOCK_SEL_TO) 
		{
			lrw_errinfo.lr_recv = errno;
			cm_Assert( CMD_ASL_DBGLOW, (UINT)( a_bufflen << 16) |lrw_errinfo.lr_recv, "tm_lr_cm_TcpRead select error" );
		
			return LRD_TCPRD_ERROR;
		}
		if(a_ret == SOCK_SEL_TO)
		{
			if(a_wait++ >= 50)
			{
				cm_Assert( CMD_ASL_DBGLOW,(UINT)( a_bufflen << 16) |lrw_errinfo.lr_recv, "tm_lr_cm_TcpRead timerout" );
				
				return LRD_TCPRD_TIMEOUT;
			}
			continue;
		}

		/*Call socket function to read arrival data*/
		a_ret = recv(a_sockethandle,(char*)(a_buff_p + a_count),
			a_bufflen - a_count,CMD_NUM0);
		/*Fail to read data, perhaps the peer disonnect this link*/
		if(a_ret < SOCK_RD_OK)
		{
			lrw_errinfo.lr_recv = errno;
			cm_Assert( CMD_ASL_NOLEVEL, (UINT)( a_bufflen << 16) |lrw_errinfo.lr_recv , "tm_lr_cm_TcpRead recv error" );

			return LRD_TCPRD_ERROR;
		}
		/*The count of read data match the expected, this routine is ended*/
		if((a_ret + a_count) >= a_bufflen)
		{
			break;
		}
	
		a_count += a_ret;/*The count increment*/
	}

	return LRD_TCPRD_OK;	
}
/* @} */

/********************************************************************************************************************/
/**
 *  @brief    Read tcp handle table	
 *  @note     Perform the process of checking active tcp connection
 *  @param    tcphdl	[in]		tcp handle set
 *  @param    count		[in]		tcp handle set size
 *  @return   result code is return 
 *  @retval   CMD_RES_OK	0  :active tcp is existent
 *  @retval   CMD_RES_NG	1  :none of active tcp is existent
 *  @date     2013/11/33  FFCS)Niemsh Create
 */
/********************************************************************************************************************/
UINT tm_lr_cm_ChkActTcp(int *tcphdl,int count)
{
	int 	a_count;
	
	for(a_count = CMD_NUM0;a_count < count;a_count++)
	{
		if(tcphdl[a_count] > SOCK_VALID)
		{
			return CMD_RES_OK;
		}						
	}
	
	return CMD_RES_NG;
}

int tm_lr_cm_ResetFd(int *tcphdl,int count, fd_set *fs)
{
	int 						a_count;
	INT						a_maxfd;/*The max value of tcp link handle*/
	fd_set					a_newfs;/*The later file descriptor*/

	for(a_count = CMD_NUM0;a_count < count;a_count++)
	{
		if(tcphdl[a_count] > SOCK_VALID)
		{
			close(tcphdl[a_count]);
			tcphdl[a_count] = SOCK_INVALID;
		}						
	}

	a_maxfd = 0;
	FD_ZERO(&a_newfs);

	FD_SET(lrw_srvsocket[LRD_SRVSIMULATOR],&a_newfs);
	a_maxfd = lrw_srvsocket[LRD_SRVSIMULATOR];
	FD_SET(lrw_srvsocket[LRD_SRVSIMULATOR_RE1],&a_newfs);
	if(lrw_srvsocket[LRD_SRVSIMULATOR_RE1] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVSIMULATOR_RE1];
	FD_SET(lrw_srvsocket[LRD_SRVSIMULATOR_RE2],&a_newfs);
	if(lrw_srvsocket[LRD_SRVSIMULATOR_RE2] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVSIMULATOR_RE2];
	FD_SET(lrw_srvsocket[LRD_SRVSIMULATOR_RE3],&a_newfs);
	if(lrw_srvsocket[LRD_SRVSIMULATOR_RE3] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVSIMULATOR_RE3];
	/*Call socket macro to add server socke handle for monitor to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVMONITOR],&a_newfs);
	if(lrw_srvsocket[LRD_SRVMONITOR] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVMONITOR];
	/*Call socket macro to add server socke handle for monitor RE1 to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVMONITOR_RE1],&a_newfs);
	if(lrw_srvsocket[LRD_SRVMONITOR_RE1] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVMONITOR_RE1];
	/*Call socket macro to add server socke handle for monitor RE2 to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVMONITOR_RE2],&a_newfs);
	if(lrw_srvsocket[LRD_SRVMONITOR_RE2] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVMONITOR_RE2];
	/*Call socket macro to add server socke handle for monitor RE3 to file descriptor*/
	FD_SET(lrw_srvsocket[LRD_SRVMONITOR_RE3],&a_newfs);
	if(lrw_srvsocket[LRD_SRVMONITOR_RE3] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVMONITOR_RE3];
	FD_SET(lrw_srvsocket[LRD_SRVOPTMT],&a_newfs);
	if(lrw_srvsocket[LRD_SRVOPTMT] > a_maxfd)
	a_maxfd = lrw_srvsocket[LRD_SRVOPTMT];

	memcpy(fs,&a_newfs,sizeof(a_newfs));

	cm_Assert(CMD_ASL_DBGLOW, a_maxfd, "reset socket handle");

	return a_maxfd;
}

