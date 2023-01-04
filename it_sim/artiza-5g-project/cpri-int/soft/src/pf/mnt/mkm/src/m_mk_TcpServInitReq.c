/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_TcpServInitReq.c
 *  @brief   Get related address infomation, and begin to init TCP server
 *  @date   2008/07/25  FFCS)zhengmh Create
 *  @date   2013/11/22  FFCS)zhaodx modify for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "m_mk_header.h"			/*	Maker MT management task header 	*/

/********************************************************************************************************************/
/**
 *  @brief     Get related address infomation, and begin to init TCP server
 *  @note     Get related address infomation, and begin to init TCP server
 *  @param  inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2008/07/25  FFCS)zhengmh Create
 *  @date     2013/11/22  FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_mk_TcpServInitReq(UINT *inf_p)
{
	UINT					buf_ret;		/* Return value of buffer request	*/
	CMT_TSKIF_TCPSRVSTRREQ *buf_tsin_p;	/* Pointer of TCP server init req IF*/
	CMT_TSKIF_TCPRELNTC    *tcprel_p;
	
	buf_ret = CMD_NUM0;			/* Return value of buffer request	*/
	buf_tsin_p = NULL;		/* Pointer of TCP server init req IF*/
	tcprel_p = NULL;

	if (mkw_stateno == MKD_ST_TSIREQRXW)
	{
		/* Buffer request	*/
		buf_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPSRVSTRREQ), (VOID **)&buf_tsin_p);

		if(buf_ret != CMD_RES_OK)	/* If buffer request NG		*/
		{
			cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_TcpServInitReq", 
			"Buffer request error", 
			buf_ret, 
			sizeof(CMT_TSKIF_TCPSRVSTRREQ), 
			CMD_NUM0);
		}

		buf_tsin_p->monport = MKD_PORT_MON;
		buf_tsin_p->monport_re1 = MKD_PORT_MON_RE1;
		buf_tsin_p->monport_re2 = MKD_PORT_MON_RE2;
		buf_tsin_p->monport_re3 = MKD_PORT_MON_RE3;
		buf_tsin_p->simuport = MKD_PORT_SIM;
		buf_tsin_p->simuport_re1 = MKD_PORT_SIM_RE1;
		buf_tsin_p->simuport_re2 = MKD_PORT_SIM_RE2;
		buf_tsin_p->simuport_re3 = MKD_PORT_SIM_RE3;
		buf_tsin_p->remtport = MKD_PORT_REMT;

		buf_tsin_p->obj = CMD_MTTYPE_MK;		/* Object MT is Maker MT	*/
		buf_tsin_p->head.uiLength =
					sizeof(CMT_TSKIF_TCPSRVSTRREQ);
		buf_tsin_p->head.uiEventNo = CMD_TSKIF_TCPSRVSTRREQ;
		buf_tsin_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
		buf_tsin_p->head.uiDstTQueueID = CMD_TSKID_LNR;


		/* Task state change into "TCP server init wait" state	*/
		mkw_stateno = MKD_ST_TSIW;
		
		/* Send TCP server start request to LAN RX task			*/
		buf_ret = cm_Enter(CMD_TSKID_LNR, CMD_QRB_NORMAL, CMD_TSKIF_TCPSRVSTRREQ, buf_tsin_p);
		if(buf_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
		}

	}
	else if (mkw_stateno == MKD_ST_USE)
	{
		/* Buffer request	*/
		buf_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC), (VOID **)&tcprel_p);

		if(buf_ret != CMD_RES_OK)	/* If buffer request NG		*/
		{
			cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_TcpServInitReq", 
			"Buffer request error", 
			buf_ret, 
			sizeof(CMT_TSKIF_TCPRELNTC), 
			CMD_NUM0);
		}
		
		tcprel_p->linkhandle = mkw_linkhandle;
		tcprel_p->head.uiLength =
							sizeof(CMT_TSKIF_TCPRELNTC);
		tcprel_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
		tcprel_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
		tcprel_p->head.uiDstTQueueID = CMD_TSKID_LNS;
		
		/*change task state to TCP server init request */
		mkw_stateno = MKD_ST_TSIREQRXW;
		/*Set MKM MT address reset flag as ON	*/
		cmw_mkaddr_reset = CMD_ON;
		
		/* Send TCP server release request to LAN RX task			*/
		buf_ret = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPRELNTC,tcprel_p );
		if(buf_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
		}
	}
}

/* @}*/
