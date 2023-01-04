/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_CpriSimu_StpReq.c
 *  @brief  Send CPRI simulator mode stop to l2 process and start cpri link
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
 *  @brief    Send CPRI simulator mode stop to l2 process and start cpri link
 *  @note     Send CPRI simulator mode stop to l2 process and start cpri link
 *  @param    inf_p      [in] Pointer of buffer
 *  @param    linkhandle [in] TCP link handle
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/22 FFCS)zhaodx  modify for zynq
 *  @date     2015/09/03 TDIPS)ikeda 17リンク対応..
 *  @date     2015/09/16 TDI)satou requestからリンク番号を取り出すように
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_CpriSimu_StpReq(UINT *inf_p, INT linkhandle)
{
	T_API_COM_RSP_DATA	    rsp_data;
	INT                     wtime;
	INT                     ret = 0;
	CMT_TSKIF_TCPSNDREQ 	*sndmsg_p;	/* Pointer of TCP data send request IF	*/
	CMT_TSKIF_CPRISIMSTPRSP *a_cprisimustprsp_p;	/* response buffer pointer */
	UINT linkno;
	UINT linkno_internal;
	UINT linkno_min, linkno_max;
	CMT_TSKIF_CPRISIMSTPREQ *stpreq_p;
	T_CMD_TCPCONN_NUM       tcpconn;

	stpreq_p        = (CMT_TSKIF_CPRISIMSTPREQ*)inf_p;
	linkno          = mnt_com_swap_USHORT(stpreq_p->linkno);
#ifndef OPT_RRH_ZYNQ_REC
#ifndef OPT_RRH_SIM
	if (0 == linkno) {
	    mnt_com_conv_linkhandle_to_internalrange(linkhandle, &linkno_min, &linkno_max);
	} else {
	    linkno_min = linkno_max = mnt_com_conv_linkno_simu_to_internal(linkno, linkhandle);
	}

	for (linkno_internal = linkno_min; linkno_internal <= linkno_max; linkno_internal++) {
        /* If not in simulator connection		*/
        tcpconn = mnt_com_get_tcpconn_simulator(linkno_internal);
        if(cmw_tblClientInfo[tcpconn].conn != CMD_CONN_Y)
        {
            cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_CpriSimu_StpReq] Connect mode NG");
            return;
        }

        wtime = 1000;
        /************************************************************/
        /* call api to stop cpri simulator mode					*/
        /************************************************************/
        ret = rrhApi_Dbg_Mqr_StopCpriSimu(D_RRH_PROCQUE_API_STOPSIMU,
                                     wtime,
                                     &rsp_data,
                                     linkno_internal );
        if(ret != E_API_RCD_OK)
        {
            cm_Assert(CMD_ASL_DBGLOW, rsp_data.result, "[m_mk_CpriSimu_StpReq] Stop cpri simulator mode NG");
        }

        /************************************************************/
        /* call api to start cpri link			*/
        /************************************************************/
        ret = rrhApi_Cpri_Mnt_EnableLink(linkno_internal, D_RRH_PROCQUE_MT, wtime,D_RRH_NULL);
        if(ret != E_API_RCD_OK)
        {
            cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_CpriSimu_StpReq] Start cpri link NG");
        }
	}
#endif
#endif
    /*Hunter buffer for CPRI simu start response*/
    if  (cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_CPRISIMSTPRSP), (void**)&a_cprisimustprsp_p)  != CMD_RES_OK)
    {
        cm_MAbort(CMD_ALMCD_BUFGET, (CHAR *)"m_mk_CpriSimu_StpReq",
            (CHAR *)"buffer hunt fail", 0, sizeof(CMT_TSKIF_CPRISIMSTPRSP), CMD_NUM0);
    }

	/*For MDIF side, linknumber is ignored*/
	a_cprisimustprsp_p->head.uiLength = sizeof(CMT_TSKIF_CPRISIMSRTRSP);
	a_cprisimustprsp_p->linknum = mnt_com_swap_USHORT(CMD_NUM1);
	a_cprisimustprsp_p->stopresult[CMD_NUM0].linkno  = linkno;
	a_cprisimustprsp_p->stopresult[CMD_NUM0].result  = ret;

	/* Set TCP data transmission message 	*/
	m_mk_cm_SetSndMsg(MKD_CPRISIMSTRRSP, a_cprisimustprsp_p, &sndmsg_p);
	sndmsg_p->linkhandle = linkhandle;

	/* Send TCP data tx request(CPRI simulator stop response) to LAN tx task*/
	ret = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, sndmsg_p);
	if(ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret, "cm_Enter NG" );
	}

	return;
}
/* @} */
