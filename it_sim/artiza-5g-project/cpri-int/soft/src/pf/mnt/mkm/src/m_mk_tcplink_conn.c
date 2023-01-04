/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_TcpLink_Conn.c
 *  @brief   Judge TCP link type and connection status, then accept/deny connection
 *  @date   2008/07/25  FFCS)zhengmh Create
 *  @date     2013/11/23 FFCS)zhaodx modify for zynq
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

/********************************************************************************************************************/
/**
 *  @brief     Judge TCP link type and connection status, then accept/deny connection
 *  @note     Judge TCP link type and connection status, then accept/deny connection
 *  @param  inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2008/07/25  FFCS)zhengmh Create
 *  @date     2013/11/23 FFCS)zhaodx modify for zynq
 *  @date     2015/09/24 TDI)satou •¡”CPRI-MTÚ‘±‘Î‰ž
 */
/********************************************************************************************************************/



VOID m_mk_TcpLink_Conn(UINT *inf_p)
{
	CMT_TSKIF_TCPRELNTC 	*tcprel_p;	/* Pointer of TCP release notification IF*/
	UINT					ret_buf;	/* Return value of buffer request	*/
	USHORT					tcplinktyp;	/* TCP link type			*/
	INT 					linkhdl;	/* TCP link handle			*/
	CMT_TSKIF_TCPCONNNTC	*tconn_p;	/* Pointer of TCP connection notification IF*/
	T_CMD_TCPCONN_NUM       sim_mon_no;

	tcprel_p = NULL;
	ret_buf = CMD_NUM0;
	tconn_p = (CMT_TSKIF_TCPCONNNTC *)inf_p;
	tcplinktyp = tconn_p->linktype;
	linkhdl = tconn_p->linkhandle;
	sim_mon_no = tconn_p->sim_mon_no;

	switch(tcplinktyp)
	{
		case CMD_LINKTYPE_MKSIMU:
			/* If not in simulator connection		*/
			if(cmw_tblClientInfo[sim_mon_no].conn == CMD_CONN_N)
			{
				cmw_tblClientInfo[sim_mon_no].conn = CMD_CONN_Y;
				cmw_tblClientInfo[sim_mon_no].tcplinkhdl = linkhdl;
			}
			else
			{
				/* Buffer request	*/
				ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC), (VOID **)&tcprel_p);

				if(ret_buf != CMD_RES_OK)
				{
					cm_MAbort(CMD_ALMCD_BUFGET, 
					"m_mk_TcpLink_Conn", 
					"Buffer request error", 
					ret_buf, 
					sizeof(CMT_TSKIF_TCPRELNTC), 
					CMD_NUM0);
				}

				tcprel_p->linkhandle = linkhdl;
				tcprel_p->head.uiLength =
							sizeof(CMT_TSKIF_TCPRELNTC);

				tcprel_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
				tcprel_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
				tcprel_p->head.uiDstTQueueID = CMD_TSKID_LNS;
				/* Send TCP release notification to LAN tx task		*/
				ret_buf = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPRELNTC, tcprel_p);
				if(ret_buf != CMD_RES_OK)
				{
					cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
				}
			}
			break;
		case CMD_LINKTYPE_MKMON:
			/* If not in monitor connection		*/
			if(cmw_tblClientInfo[sim_mon_no].conn == CMD_CONN_N)
			{
				cmw_tblClientInfo[sim_mon_no].conn = CMD_CONN_Y;
				cmw_tblClientInfo[sim_mon_no].tcplinkhdl = linkhdl;

				/*f_recv_addMsgMap(CMD_TSKIF_L3STANTC,0,CMD_TSKID_MKM);*/
			}
			else
			{
				/* Buffer request	*/
				ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC), (VOID **)&tcprel_p);

				if(ret_buf != CMD_RES_OK)
				{
					cm_MAbort(CMD_ALMCD_BUFGET, 
					"m_mk_TcpLink_Conn", 
					"Buffer request error", 
					ret_buf, 
					sizeof(CMT_TSKIF_TCPRELNTC), 
					CMD_NUM0);
				}

				tcprel_p->linkhandle = linkhdl;
				tcprel_p->head.uiLength =
							sizeof(CMT_TSKIF_TCPRELNTC);
				tcprel_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
				tcprel_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
				tcprel_p->head.uiDstTQueueID = CMD_TSKID_LNS;

				/* Send TCP release notification to LAN tx task		*/
				ret_buf = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPRELNTC, tcprel_p);
				if(ret_buf != CMD_RES_OK)
				{
					cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
				}
			}
			break;
		case CMD_LINKTYPE_RE:
			if(cmw_tblClientInfo[sim_mon_no].conn == CMD_CONN_N)
			{
				cmw_tblClientInfo[sim_mon_no].conn = CMD_CONN_Y;
				cmw_tblClientInfo[sim_mon_no].tcplinkhdl = linkhdl;
				/* Subscribe card status notice */
				m_mk_subCardstatus();
			}
			else
			{
				/* Buffer request	*/
				ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC), (VOID **)&tcprel_p);

				if(ret_buf != CMD_RES_OK)
				{
					cm_MAbort(CMD_ALMCD_BUFGET, 
					"m_mk_TcpLink_Conn", 
					"Buffer request error", 
					ret_buf, 
					sizeof(CMT_TSKIF_TCPRELNTC), 
					CMD_NUM0);
				}

				tcprel_p->linkhandle = linkhdl;
				tcprel_p->head.uiLength =
							sizeof(CMT_TSKIF_TCPRELNTC);
				tcprel_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
				tcprel_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
				tcprel_p->head.uiDstTQueueID = CMD_TSKID_LNS;


				/* Send TCP release notification to LAN tx task		*/
				ret_buf = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPRELNTC, tcprel_p);
				if(ret_buf != CMD_RES_OK)
				{
					cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
				}
			}
			break;
		default:
			cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_TcpLink_Conn] Link type NG");
			break;
	}
	return;
}

/* @}*/
