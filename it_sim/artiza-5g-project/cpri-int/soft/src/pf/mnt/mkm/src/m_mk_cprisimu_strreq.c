/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_CpriSimu_StrReq.c
 *  @brief  Stop cpri link and send CPRI simulator mode start to l2 process
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/22 FFCS)zhaodx  modify for zynq
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
#include "rrhApi_Dbg.h"
#include "rrhApi_Cpri.h"
/********************************************************************************************************************/
/**
 *  @brief    Stop cpri link and send CPRI simulator mode start to l2 process
 *  @note     Stop cpri link and send CPRI simulator mode start to l2 process
 *  @param    inf_p			[in]	Pointer of buffer
 *  @param    linkhandle    [in]    TCP link handle
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/22 FFCS)zhaodx  modify for zynq
 *  @date     2015/09/15 TDI)satou CPRIリンク番号を要求から取り出すように修正
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_CpriSimu_StrReq(UINT *inf_p, INT linkhandle)
{
	T_API_COM_RSP_DATA	    rsp_data;
	MKT_CPRISIMUSTRREQ 		*simstrreq_p;	/* Pointer of  CPRI simulator start request */
	CMT_TSKIF_TCPSNDREQ 	*sndmsg_p;	/* Pointer of TCP data send request IF	*/
	CMT_TSKIF_CPRISIMSRTRSP *a_cprisimustrrsp_p;	/* response buffer pointer */

	USHORT 					simmode;		/* Simulator mode					*/
	INT                     wtime;
	INT                     ret;
	UINT                    linkno;         /* CPRIリンク番号 */
	UINT                    linkno_internal;
	T_CMD_TCPCONN_NUM       tcpconn;

	simstrreq_p     = (MKT_CPRISIMUSTRREQ *)inf_p;
	linkno          = mnt_com_swap_USHORT(simstrreq_p->linkno );
	simmode         = mnt_com_swap_USHORT(simstrreq_p->simmode);
	linkno_internal = mnt_com_conv_linkno_simu_to_internal(linkno, linkhandle);

	switch(simmode)
	{
		case CMD_SIMMODE_COMMON:
		case CMD_SIMMODE_L3TEST:
			break;
		default:
			cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_CpriSimu_StrReq] Mode NG");
			return;
	}

	/* If not in simulator connection			*/
	tcpconn = mnt_com_get_tcpconn_simulator(linkno_internal);
	if(cmw_tblClientInfo[tcpconn].conn != CMD_CONN_Y)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM1, "[m_mk_CpriSimu_StrReq] Connect mode NG");
		return;
	}
#ifndef OPT_RRH_ZYNQ_REC
	wtime = 1000;
	/************************************************************/
	/* call api to stop cpri link			*/
	/************************************************************/
	ret = rrhApi_Cpri_Mnt_DisableLink(linkno_internal, D_RRH_PROCQUE_MT, wtime,D_RRH_NULL);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_CpriSimu_StrReq] Stop cpri link NG");
	}

    /************************************************************/
	/* call api to start cpri simulator mode					*/
	/************************************************************/
	ret = rrhApi_Dbg_Mqr_StartCpriSimu(D_RRH_PROCQUE_API_STARTSIMU,
	                             wtime,
	                             &rsp_data,
	                             linkno_internal,
	                             D_API_DBG_SIMU_MODE_ON_L3 );
	if(ret != E_API_RCD_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, ret, "[m_mk_CpriSimu_StrReq] rrhApi_Dbg_Mqr_StartCpriSimu NG");
	}
	else
	{
		if(rsp_data.result != CMD_OK)
		cm_Assert(CMD_ASL_DBGLOW, rsp_data.result, rsp_data.errMsg);
	}
#else
	ret = 0;
#endif

	/*Hunter buffer for CPRI simu start response*/
	if  (cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_CPRISIMSRTRSP), (void**)&a_cprisimustrrsp_p)  != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "m_mk_CpriSimu_StrReq",
			"buffer hunt fail", 0, sizeof(CMT_TSKIF_CPRISIMSRTRSP), CMD_NUM0);
	}

	/*For MDIF side, linknumber is ignored*/
	a_cprisimustrrsp_p->head.uiLength = sizeof(CMT_TSKIF_CPRISIMSRTRSP);
	a_cprisimustrrsp_p->linkno = mnt_com_swap_USHORT(linkno);
	a_cprisimustrrsp_p->result = ret;

	/* Set TCP data transmission message 	*/
	m_mk_cm_SetSndMsg(MKD_CPRISIMSTRRSP, a_cprisimustrrsp_p, &sndmsg_p);
	sndmsg_p->linkhandle = linkhandle;

	/* Send TCP data tx request(CPRI simulator start response) to LAN tx task*/
	ret = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, sndmsg_p);
	if(ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret, "cm_Enter NG" );
	}

	return;
}
/* @} */
