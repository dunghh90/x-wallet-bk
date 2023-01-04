/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_TcpInit_Rsp.c
 *  @brief   If response result is OK, send TCP server start request to LAN rx task
 *  @date   2008/07/25  FFCS)zhengmh Create
 *  @date   2013/11/23 FFCS)zhaodx modify for zynq
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
 *  @brief     If response result is OK, send TCP server start request to LAN rx task
 *  @note     If response result is OK, send TCP server start request to LAN rx task
 *  @param  buf_p  [in] Pointer of buffer
 *  @return   None
 *  @retval    -
 *  @date     2008/07/25  FFCS)zhengmh Create
 *  @date     2013/11/23 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_mk_TcpInit_Rsp(UINT *buf_p)
{
	CMT_TSKIF_TCPSRVSTRRSP		 *rsp_p;	/* Pointer of TCP server init rsp IF*/
	
	rsp_p = (CMT_TSKIF_TCPSRVSTRRSP *)buf_p;

	mkw_linkhandle = rsp_p->result;
	
	if(rsp_p->result >= CMD_NUM0) 		/* If response result is OK		*/
	{

#if 0       
		ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_HEAD), (VOID **)&head_p);
		if(ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, CMD_NUM0, "m_mk_TcpInit_Rsp:cm_BReq NG");
			return;
		}

		head_p->uiLength = sizeof(CMT_TSKIF_HEAD);
		head_p->uiEventNo = CMD_TSKIF_INIRSP;
		head_p->uiSrcTQueueID = CMD_TSKID_MKM;
		head_p->uiDstTQueueID = CMD_TSKID_INI;
		/* Send TCP server init response to RSV task */
		ret_buf = cm_Enter(CMD_TSKID_INI, CMD_QRB_NORMAL, CMD_TSKIF_TCPSRVINITRSP, head_p);
		if(ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
		}
#endif
		/* Change task state into USE state		*/
		mkw_stateno = MKD_ST_USE;

	}
	else
	{
		cm_MAbort(CMD_ALMCD_ETHERNET, 
			             "m_mk_TcpInit_Rsp", 
			             "TCP server start result error",
			             rsp_p->result,
			             CMD_NUM0,
			             CMD_NUM0) ;
	}	
}

/* @}*/

