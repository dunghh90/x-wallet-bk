/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_TcpLink_Rel.c
 *  @brief   Stop simulator/monitor for TXIOCS, RXIOCS and CPRI mng task
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
#include "f_cmn_cmu_inc.h"
#include "rrhApi_Dbg.h"
#include "rrhApi_Cpri.h"
/********************************************************************************************************************/
/**
 *  @brief     Stop simulator/monitor for TXIOCS, RXIOCS and CPRI mng task
 *  @note     Stop simulator/monitor for TXIOCS, RXIOCS and CPRI mng task
 *  @param  inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2008/07/25  FFCS)zhengmh Create
 *  @date     2013/11/22 FFCS)zhaodx modify for zynq
 *  @date     2015/09/03 TDIPS)ikeda 17リンク対応.
 *  @date     2015/09/16 TDI)satou rrhApi_Cpri_Mnt_EnableLink()をリンク数だけ実行するように
 *  @date     2015/10/09 TDI)satou cmw_tblClientInfoの配列番号変更
 */
/********************************************************************************************************************/
VOID m_mk_TcpLink_Rel(UINT *inf_p)
{
	T_API_COM_RSP_DATA	    rsp_data;
	CMT_TSKIF_TCPRELNTC 	*tcprel_p;	/* Pointer of TCP release notification IF	*/
	INT						linkhdl;	/* TCP link handle				*/
	UINT		ret_tstop;	/* Return value of stopping timer	*/
	UINT		ret_bfree;	/* Return value of freeing buffer	*/
	UINT					ret;		/* Return value of stop timer	*/
	INT                     wtime;
	UINT                    linkno;
	INT                     errcd;

	tcprel_p = (CMT_TSKIF_TCPRELNTC *)inf_p;
	linkhdl = tcprel_p->linkhandle;
	ret_tstop = CMD_NUM0;
	ret_bfree = CMD_NUM0;
	USHORT link_num_min;
	USHORT link_num_max;

	/* If link handle is simulator link handle	*/
	if ((linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC    ].tcplinkhdl)
	||  (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].tcplinkhdl)
	||  (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].tcplinkhdl)
	||  (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].tcplinkhdl)) /* pgr0012 pgr0860 */
	{
#ifndef OPT_RRH_ZYNQ_REC
		wtime = 0;

		if (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC].tcplinkhdl) {
		    link_num_min = link_num_max = D_RRH_CPRINO_REC;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC].conn = CMD_CONN_N;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_REC].tcplinkhdl = CMD_TCPHDL_NULL;
		} else if (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].tcplinkhdl) {
		    link_num_min = D_RRH_CPRINO_RE1;
		    link_num_max = D_RRH_CPRINO_RE6;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].conn = CMD_CONN_N;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE01_06].tcplinkhdl = CMD_TCPHDL_NULL;
		} else if (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].tcplinkhdl) {
		    link_num_min = D_RRH_CPRINO_RE7;
		    link_num_max = D_RRH_CPRINO_RE12;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].conn = CMD_CONN_N;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE07_12].tcplinkhdl = CMD_TCPHDL_NULL;
		} else {
		    link_num_min = D_RRH_CPRINO_RE13;
		    link_num_max = D_RRH_CPRINO_RE16;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].conn = CMD_CONN_N;             /* pgr0012 pgr0860 */
            cmw_tblClientInfo[CMD_TCPCONN_NUM_SIMNO_RE13_16].tcplinkhdl = CMD_TCPHDL_NULL;  /* pgr0012 pgr0860 */
		}
		
		for (linkno = link_num_min; linkno <= link_num_max; linkno++) {
            /************************************************************/
            /* call api to stop cpri simulator mode					*/
            /************************************************************/
            ret = rrhApi_Dbg_Mqr_StopCpriSimu(D_RRH_PROCQUE_API_STOPSIMU,
                                         wtime,
                                         &rsp_data,
                                         linkno);
            if(ret != E_API_RCD_OK)
            {
                cm_Assert(CMD_ASL_DBGLOW, rsp_data.result, "[m_mk_TcpLink_Rel] Stop cpri simulator mode NG");
            }

            /************************************************************/
            /* call api to start cpri link			*/
            /************************************************************/
            ret = rrhApi_Cpri_Mnt_EnableLink(linkno, D_RRH_PROCQUE_MT, wtime,D_RRH_NULL);
            if(ret != E_API_RCD_OK)
            {
                cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_TcpLink_Rel] Start cpri link NG");
            }
		}
#endif
	}
	else if ((linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC    ].tcplinkhdl)   /* pgr0012 pgr0860 */
	||       (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE01_06].tcplinkhdl)   /* pgr0012 pgr0860 */
	||       (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE07_12].tcplinkhdl)   /* pgr0012 pgr0860 */
	||       (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE13_16].tcplinkhdl))  /* pgr0012 pgr0860 */
	{
		wtime = 0;

		if (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC].tcplinkhdl) {            /* pgr0012 pgr0860 */
		    link_num_min = link_num_max = D_RRH_CPRINO_REC;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC].conn = CMD_CONN_N;                  /* pgr0012 pgr0860 */
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_REC].tcplinkhdl = CMD_TCPHDL_NULL;       /* pgr0012 pgr0860 */
		} else if (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE01_06].tcplinkhdl) { /* pgr0012 pgr0860 */
            link_num_min = D_RRH_CPRINO_RE1;
            link_num_max = D_RRH_CPRINO_RE6;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE01_06].conn = CMD_CONN_N;              /* pgr0012 pgr0860 */
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE01_06].tcplinkhdl = CMD_TCPHDL_NULL;   /* pgr0012 pgr0860 */
		} else if (linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE07_12].tcplinkhdl) { /* pgr0012 pgr0860 */
            link_num_min = D_RRH_CPRINO_RE7;
            link_num_max = D_RRH_CPRINO_RE12;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE07_12].conn = CMD_CONN_N;              /* pgr0012 pgr0860 */
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE07_12].tcplinkhdl = CMD_TCPHDL_NULL;   /* pgr0012 pgr0860 */
		} else {
            link_num_min = D_RRH_CPRINO_RE13;
            link_num_max = D_RRH_CPRINO_RE16;
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE13_16].conn = CMD_CONN_N;              /* pgr0012 pgr0860 */
            cmw_tblClientInfo[CMD_TCPCONN_NUM_MONNO_RE13_16].tcplinkhdl = CMD_TCPHDL_NULL;   /* pgr0012 pgr0860 */
		}

		for (linkno = link_num_min; linkno <= link_num_max; linkno++) {
            /* call api to stop monitor mode  */
            rrhApi_Dbg_Mnt_StopCpriMonitor(D_RRH_PROCQUE_MT, wtime, D_RRH_NULL, linkno);
            
			/* Unsubscribe l2 status notice */
			m_mk_unsubl2state(linkno);
		}
	}
	else  if(linkhdl == cmw_tblClientInfo[CMD_TCPCONN_NUM_FHMMT].tcplinkhdl)  /* pgr0012 pgr0860 */
	{
		m_mk_unSubCardstatus();
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
		cmw_tblClientInfo[CMD_TCPCONN_NUM_FHMMT].conn = CMD_CONN_N;             /* pgr0012 pgr0860 */
		cmw_tblClientInfo[CMD_TCPCONN_NUM_FHMMT].tcplinkhdl = CMD_TCPHDL_NULL;  /* pgr0012 pgr0860 */
		mkw_REMTConnect = CMD_CONN_N;
	}

	return;
}
/* @}*/
