/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_CpriSimu_DataRcv.c
 *  @brief   copy simulator data from L3 buffer to L2 buffer,and send to l2 process
 *		     
 *  @date     2013/11/29 FFCS)zhaodx modify for zynq
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

/********************************************************************************************************************/
/**
 *  @brief     1. Hunt L2 buffer, and copy simulator data from L3 buffer to L2 buffer
 *		     2. Send data reception request to RXIOCS task
 *  @note     1. Hunt L2 buffer, and copy simulator data from L3 buffer to L2 buffer
 *		    2. Send data reception request to RXIOCS task
 *  @param  inf_p			[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2013/11/29 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/
#ifdef OPT_RRH_ZYNQ_REC

VOID m_mk_CpriMsg_DataRcv(UINT *inf_p)
{
	CMT_TSKIF_TCPSNDREQ 	*sndmsg_p;		/* Pointer of TCP data send req IF	*/
	UINT					ret_buf;		/* Return value of buffer request	*/
	CMT_TSKIF_L1DATRQ 		*cpridata_p;		/* Pointer of cpri data		*/
	CMT_TSKIF_L1DATRQ 		*desbuf_p;		/* Pointer of destination buffer	*/
	UINT 					memcpylen;		/* Data length of mem copy			*/
	MNT_COM_CMT_CPRIF_MAPPING*	cpriMsg_p;
	/*UINT					cprihdsiz;	*/	/* Size of CPRI head				*/

	sndmsg_p = NULL;
	ret_buf = NULL;
	cpridata_p = (CMT_TSKIF_L1DATRQ *)inf_p;
	desbuf_p = NULL;
	memcpylen = NULL;
	/*cprihdsiz = sizeof(CMT_TSKIF_L1DATRQ) - sizeof(CMT_TSKIF_HEAD);*/	
	
	/* If in simulator connection			*/
	if(cmw_tblClientInfo[CMD_TCPNO_SIM].conn == CMD_CONN_Y)
	{
		/* Buffer request	*/
		ret_buf = cm_BReq(CMD_BUFCA_MON, cpridata_p->head.uiLength, 
					(VOID **)&desbuf_p);

		if(ret_buf != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_CpriSimu_DataSnd", 
			"Buffer request error", 
			ret_buf, 
			cpridata_p->head.uiLength, 
			NULL);	
		}

		/* CPRI header size + L3 buffer size	*/
		memcpylen = cpridata_p->head.uiLength - sizeof(CMT_TSKIF_HEAD);	
			
		cm_MemCpy(&(desbuf_p->link_num), &(cpridata_p->link_num), memcpylen);

		desbuf_p->head.uiLength = cpridata_p->head.uiLength;
		/* to big endian*/
		desbuf_p->link_num = mnt_com_swap_USHORT(desbuf_p->link_num);
		desbuf_p->length = mnt_com_swap_UINT(desbuf_p->length);
		desbuf_p->disp = mnt_com_swap_UINT(desbuf_p->disp);
		cpriMsg_p = (MNT_COM_CMT_CPRIF_MAPPING*)desbuf_p;
		m_mk_com_swap_convert(cpriMsg_p->m2.signal_kind,	cpriMsg_p->m2.body);
		
		/* Set TCP data transmission message 	*/
		m_mk_cm_SetSndMsg(MKD_CPRISIMDATSNDREQ, desbuf_p, &cpridata_p);

		/* Send TCP data tx request (CPRI data send request) to LAN tx task	*/
		ret_buf = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, cpridata_p);
		if(ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
		}
	}
	
	return;
}
#endif
/* @} */
