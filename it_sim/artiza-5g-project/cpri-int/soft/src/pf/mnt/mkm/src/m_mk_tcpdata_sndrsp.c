/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_TcpData_SndRsp.c
 *  @brief   If response result is NG, then send TCP release req to LAN tx task
 *  @date   2008/07/25  FFCS)zhengmh Create
 *  @date     2013/12/03 FFCS)zhaodx modify for zynq
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
 *  @brief     If response result is NG, then send TCP release req to LAN tx task
 *  @note     If response result is NG, then send TCP release req to LAN tx task
 *  @param  inf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2008/07/25  FFCS)zhengmh Create
 *  @date     2013/12/03 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_mk_TcpData_SndRsp(UINT *inf_p)
{
	CMT_TSKIF_TCPRELNTC 	*tcprel_p;		/* Pointer of TCP link release req IF*/
	UINT					ret_buf;		/* Return value of buffer request	*/	
	CMT_TSKIF_TCPSNDRSP 	*sndrsp_p;		/* Pointer of TCP data send rsp IF	*/
	UINT 					tcplinkno;		/* TCP link no					*/
	INT					linkhdl;		/* TCP link handle				*/
	INT                     i;

	tcprel_p = NULL;
	ret_buf = CMD_NUM0;
	sndrsp_p = (CMT_TSKIF_TCPSNDRSP *)inf_p;
	tcplinkno = CMD_NUM0;
	linkhdl = sndrsp_p->linkhandle;

	/* If TCP data transmission is not abnormal					*/
	if(sndrsp_p->result != CMD_NG)		
	{
		return;			/* Return without any processing		*/
	}

	for (i = 0; i < CMD_TCPCONN_NUM_MAX; i++) {
	    if (cmw_tblClientInfo[i].tcplinkhdl == linkhdl) { /* pgr0013 pgr0862 */
            /* Buffer request	*/
            ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_TCPRELNTC), (VOID **)&tcprel_p);

            if(ret_buf != CMD_RES_OK)
            {
                cm_MAbort(CMD_ALMCD_BUFGET,
                "m_mk_TcpData_SndRsp",
                "Buffer request error",
                ret_buf,
                sizeof(CMT_TSKIF_TCPRELNTC),
                CMD_NUM0);
            }

            tcprel_p->linkhandle = cmw_tblClientInfo[i].tcplinkhdl; /* pgr0013 pgr0862 */
            tcprel_p->head.uiLength = sizeof(CMT_TSKIF_TCPRELNTC);
            tcprel_p->head.uiEventNo = CMD_TSKIF_TCPRELNTC;
            tcprel_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
            tcprel_p->head.uiDstTQueueID = CMD_TSKID_LNS;

            /* Send tcp release notification to LAN tx task		*/
            ret_buf = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPRELNTC, tcprel_p);
            if(ret_buf != CMD_RES_OK)
            {
                cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
            }

            return;
	    }
	}
}

/* @}*/
