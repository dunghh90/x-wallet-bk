/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_CpriSimu_DataRcv.c
 *  @brief   copy simulator data from L3 buffer to L2 buffer,and send to l2 process
 *		     
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/25 FFCS)zhaodx modify for zynq
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

/********************************************************************************************************************/
/**
 *  @brief  Hunt L2 buffer, and copy simulator data from L3 buffer to L2 buffer \n
 *		    Send data reception request to RXIOCS task
 *  
 *	
 *  @param  inf_p      [in] Pointer of buffer
 *  @param  linkhandle [in] TCP link handle
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/29 FFCS)zhaodx modify for zynq
 *  @date     2015/09/24 TDI)satou CPRIリンク番号から対応するTCPコネクション情報を参照するように
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_CpriSimu_DataRcv(UINT *inf_p, INT linkhandle)
{
	CMT_TSKIF_L1DATRCVNTC 	*cpricom_p;	/* Pointer of L1 data rx notification IF*/
	UINT					ret_buf;	/* Return value of buffer request	*/
	UINT 					datalen;	/* Dada length(CPRI header + L3 buffer)*/
	MKT_CPRISIMUDATRCV 		*datrecv_p;	/* Pointer of CPRI simulator data reception notification*/
	UINT					cprihdsiz;	/* CPRI head size					*/
	INT                     wtime;
	INT						rtn __attribute__((unused));
	UINT					ret_bfree;	/* Return value of freeing buffer	*/
	T_CMD_TCPCONN_NUM       tcpconn;
	USHORT                  link_num;

#ifdef OPT_RRH_ZYNQ_REC
	MNT_COM_CMT_CPRIF_MAPPING*	cpriMsg_p;
#endif

	cpricom_p = NULL;
	ret_buf = CMD_NUM0;
	datrecv_p = (MKT_CPRISIMUDATRCV *)inf_p;
	datalen = datrecv_p->msglen; 
	cprihdsiz = sizeof(CMT_TSKIF_L1DATRCVNTC) - sizeof(CMT_TSKIF_HEAD);
	link_num = mnt_com_swap_USHORT(*(USHORT*)&datrecv_p->buf[0]);
	link_num = mnt_com_conv_linkno_simu_to_internal(link_num, linkhandle);
	*(USHORT*)&datrecv_p->buf[0] = link_num;
	*(USHORT*)&datrecv_p->buf[0] = mnt_com_swap_USHORT(*(USHORT*)&datrecv_p->buf[0]);
	tcpconn = mnt_com_get_tcpconn_simulator(link_num);
	
	/* If not in simulator connection		*/
	if(cmw_tblClientInfo[tcpconn].conn != CMD_CONN_Y)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM0, "[m_mk_CpriSimu_DataRcv] Connect mode NG");
		return;
	}
	
	/* Get layer 2 buffer			*/
	ret_buf = cm_L2BfGet(CMD_BUFCA_LAPRCV, sizeof(CMT_TSKIF_HEAD) + datalen,
			CMD_NUM1, (VOID **)&cpricom_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
		"m_mk_CpriSimu_DataRcv", 
		"Buffer request error", 
		ret_buf, 
		sizeof(CMT_TSKIF_HEAD) + datalen, 
		CMD_NUM0);	
	}

	/* Memory copy (CPRI header + 	L3 buffer)	*/
	cm_MemCpy(&(cpricom_p->link_num), &(datrecv_p->buf[CMD_NUM0]), datalen);	

	/*  Data byte length = CPRI header length + L3 buffer length 		*/
	cpricom_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + datalen;		
	cpricom_p->length = cpricom_p->head.uiLength - sizeof(CMT_TSKIF_HEAD) + sizeof(UCHAR) * CMD_NUM2 - cprihdsiz;
	cpricom_p->disp = sizeof(CMT_TSKIF_L1DATRCVNTC) - sizeof(UCHAR) * CMD_NUM4;
#ifndef OPT_RRH_ZYNQ_REC
	/* Send L1 data reception notification to RXIOCS task		*/
	wtime = 0;
	rtn = rrhApi_Dbg_Mnt_SendSimuCpriMsg(D_RRH_PROCQUE_MT, wtime,D_RRH_NULL,cpricom_p);

	ret_bfree = cm_BFree((void *)cpricom_p);
	
	if(ret_bfree != CMD_RES_OK)
	{
		cm_MAbort( CMD_ALMCD_BUFREL, 
					"m_mk_CpriSimu_DataRcv",
					"cm_BFree NG",
					ret_bfree,
					(UINT)cpricom_p,
					CMD_NUM0);
	}

#else
	cpriMsg_p = (MNT_COM_CMT_CPRIF_MAPPING*)cpricom_p;
	m_mk_com_swap_convert(mnt_com_swap_USHORT(cpriMsg_p->m2.signal_kind),
		cpriMsg_p->m2.body);

	rtn = m_cm_L3MsgSend_REC( cpricom_p );
#endif
	
	return;
}
/* @} */
