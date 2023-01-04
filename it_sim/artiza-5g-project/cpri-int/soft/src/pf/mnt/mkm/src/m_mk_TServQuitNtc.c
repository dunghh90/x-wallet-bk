/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_TServQuitNtc.c
 *  @brief   process TCP server stop notice
 *  @date   2008/08/20 FFCS)zhengmh create
 *  @date   2013/11/23 FFCS)zhaodx modify for zynq
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

																		
#include "m_cm_header.h"			/* 	TRA  common head file				*/
#include "m_mk_header.h"			/*	 MKM task header 	*/
#include "rrhApi_Com.h"
#include "rrhApi_Dbg.h"
#include "rrhApi_Cpri.h"

/********************************************************************************************************************/
/**
 *  @brief     process TCP server stop notice
 *  @note     1.Release CPRI-MT/RE-MT resource	
 		     2.Init TCP server	
 *  @param  inf_p      	[in]   Pointer of received buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/08/20 FFCS)zhengmh create
 *  @date     2013/11/23 FFCS)zhaodx modify for zynq
 *  @date     2015/10/09 TDI)satou cmw_tblClientInfoの配列番号変更に対応
 */
/********************************************************************************************************************/

VOID m_mk_TServQuitNtc( UINT *inf_p)
{
    T_API_COM_RSP_DATA	    rsp_data;
	UINT					linkno;
	INT						ret;
	INT						wtime;
	INT                     errcd;
	CMT_MTADDR_INFO			*mtaddr_p;			/* Pointer of MT Address info */
	CMT_TSKIF_TCPSRVSTRREQ *buf_tsin_p;	/* Pointer of TCP Server 初期化要求			*/
	UINT					ret_buf;	/* Return value of buffer request*/

	/************************************************/
	cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC    ].conn       = CMD_CONN_N;
	cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC    ].tcplinkhdl = CMD_TCPHDL_NULL;
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].conn       = CMD_CONN_N;
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].tcplinkhdl = CMD_TCPHDL_NULL;
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].conn       = CMD_CONN_N;
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].tcplinkhdl = CMD_TCPHDL_NULL;
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].conn       = CMD_CONN_N;       /* pgr0012 pgr0860 */
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].tcplinkhdl = CMD_TCPHDL_NULL;  /* pgr0012 pgr0860 */
	/************************************************/
	cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC    ].conn       = CMD_CONN_N;       /* pgr0012 pgr0860 */
	cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC    ].tcplinkhdl = CMD_TCPHDL_NULL;  /* pgr0012 pgr0860 */
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].conn       = CMD_CONN_N;
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].tcplinkhdl = CMD_TCPHDL_NULL;
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].conn       = CMD_CONN_N;
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].tcplinkhdl = CMD_TCPHDL_NULL;
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].conn       = CMD_CONN_N;       /* pgr0012 pgr0860 */
    cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].tcplinkhdl = CMD_TCPHDL_NULL;  /* pgr0012 pgr0860 */

	wtime = 0;

	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++) {
        /* call api to stop monitor mode  */
        rrhApi_Dbg_Mnt_StopCpriMonitor(D_RRH_PROCQUE_MT, wtime, D_RRH_NULL, linkno);
		/* call api to unsubscribe lapb link state  */
		m_mk_unsubl2state(linkno);
	}


	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++) {
        rrhApi_Dbg_Mqr_StopCpriSimu(D_RRH_PROCQUE_API_STOPSIMU,
                                     wtime,
                                     &rsp_data,
                                     linkno );
	}

	/************************************************/
	/* 	Clear TRA file download used status 				*/
	/************************************************/
	/* When TRA file download timer is running	*/
	if(mkw_timmng_tbl[CMD_NUM0] != CMD_NUM0)
	{
		/* Stop download timer			*/
		ret = cm_TStop(mkw_timmng_tbl[CMD_NUM0], &errcd);
		/* Stop timer error,Assert 		*/
		if(ret != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM0], "file download Timer Stop NG" );
		}
		/* Set download timer flag 		*/
		mkw_timmng_tbl[CMD_NUM0] = CMD_NUM0;
	}
	/* Stop TRA write timer */
	if(mkw_timmng_tbl[CMD_NUM1] != CMD_NUM0)
	{
		/* Stop TRA write timer */
		ret = cm_TStop(mkw_timmng_tbl[CMD_NUM1], &errcd);
		/* Stop timer error,Assert 		*/
		if(ret != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, mkw_timmng_tbl[CMD_NUM1], "TRA write timer Stop NG" );
		}
		/* Set download timer flag 		*/
		mkw_timmng_tbl[CMD_NUM1] = CMD_NUM0;
	}
	/* Clear status table */
	m_mk_cm_DLEnd();
	/* Set RE MT Client disconnection	*/
	cmw_tblClientInfo[CMD_TCPCONN_NUM_FHMMT].conn       = CMD_CONN_N;       /* pgr0012 pgr0860 */
	cmw_tblClientInfo[CMD_TCPCONN_NUM_FHMMT].tcplinkhdl = CMD_TCPHDL_NULL;  /* pgr0012 pgr0860 */
	mkw_REMTConnect = CMD_CONN_N;

	/************************************************/
	/* 	send TCP server init request to LNR				*/
	/************************************************/
	
	mtaddr_p = &(cmw_mtsetmng.mtaddr_sav[CMD_SYS_3G]);
	 /* Buffer request	*/
	 ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPSRVSTRREQ), (VOID **)&buf_tsin_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
		(CHAR *)"m_mk_TServQuitNtc", 
		(CHAR *)"Buffer request error", 
		ret_buf, 
		sizeof(CMT_TSKIF_TCPSRVSTRREQ), 
		CMD_NUM0);
		return;
	}

	/* タスク間ヘッダ部編集	*/
	buf_tsin_p->head.uiLength = sizeof(CMT_TSKIF_TCPSRVSTRREQ);

	buf_tsin_p->ipaddr[CMD_NUM0] = mtaddr_p->ipadd_1;
	buf_tsin_p->ipaddr[CMD_NUM1] = mtaddr_p->ipadd_2;
	buf_tsin_p->ipaddr[CMD_NUM2] = mtaddr_p->ipadd_3;
	buf_tsin_p->ipaddr[CMD_NUM3] = mtaddr_p->ipadd_4;

	buf_tsin_p->ipmask[CMD_NUM0] = mtaddr_p->sbnetmsk_1;
	buf_tsin_p->ipmask[CMD_NUM1] = mtaddr_p->sbnetmsk_2;
	buf_tsin_p->ipmask[CMD_NUM2] = mtaddr_p->sbnetmsk_3;
	buf_tsin_p->ipmask[CMD_NUM3] = mtaddr_p->sbnetmsk_4;

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


	/* Task state change into "TCP server init wait" state	*/
	mkw_stateno = MKD_ST_TSIW;

	buf_tsin_p->head.uiLength = sizeof(CMT_TSKIF_TCPSNDREQ);
	buf_tsin_p->head.uiEventNo = CMD_TSKIF_TCPSRVSTRREQ;
	buf_tsin_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
	buf_tsin_p->head.uiDstTQueueID = CMD_TSKID_LNR;
	/* Send TCP data tx request(CPRI monitor stop respo */
	/* TCP Server 初期化要求をMT LAN受信タスクに送信する			*/
	ret_buf = cm_Enter(CMD_TSKID_LNR, CMD_QRB_NORMAL, CMD_TSKIF_TCPSRVSTRREQ, buf_tsin_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
	}


}

/* @}*/
