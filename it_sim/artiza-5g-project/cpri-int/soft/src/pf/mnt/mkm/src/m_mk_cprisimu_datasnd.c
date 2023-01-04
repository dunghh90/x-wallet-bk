/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_CpriSimu_DataSnd.c
 *  @brief   Copy CPRI simulator data from L2 buffer to L3 buffer, send it to PC
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/22 FFCS)zhaodx modify for zynq
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
 *  @brief     Copy CPRI simulator data from L2 buffer to L3 buffer, send it to PC
 *  @note     Copy CPRI simulator data from L2 buffer to L3 buffer, send it to PC
 *  @param  inf_p			[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/22 FFCS)zhaodx modify for zynq
 *  @date     2015/09/17 TDI)satou CMD_TSKIF_L1DATCNFのCPRIリンク番号設定を追加(修正前は0固定)
 *  @date     2015/09/24 TDI)satou CPRIリンク番号から対応するTCPコネクション情報を参照するように
 */
/********************************************************************************************************************/
VOID m_mk_CpriSimu_DataSnd(UINT *inf_p)
{
	CMT_TSKIF_TCPSNDREQ 	*sndmsg_p;		/* Pointer of TCP data send req IF	*/
	CMT_TSKIF_L1DATCNF		*datacfm_p;		/* Pointer of L1 data confirm IF	*/
	UINT					ret_buf;		/* Return value of buffer request	*/
	CMT_TSKIF_L1DATRQ		*simdata_p;		/* Pointer of simulator data		*/
	USHORT                  link_num;       /* CPRIリンク番号                   */
	T_CMD_TCPCONN_NUM       tcpconn;

	sndmsg_p  = NULL;
	datacfm_p = NULL;
	ret_buf   = CMD_NUM0;
	simdata_p = (CMT_TSKIF_L1DATRQ *)inf_p;
	link_num  = mnt_com_swap_USHORT(simdata_p->link_num);
	tcpconn   = mnt_com_get_tcpconn_simulator(link_num);
	simdata_p->link_num = mnt_com_conv_linkno_internal_to_simu(link_num);
	simdata_p->link_num = mnt_com_swap_USHORT(simdata_p->link_num);
	
	/* If in simulator connection			*/
	if(cmw_tblClientInfo[tcpconn].conn == CMD_CONN_Y)
	{
		/* Set TCP data transmission message	*/
		m_mk_cm_SetSndMsg(MKD_CPRISIMDATSNDREQ, simdata_p, &sndmsg_p);
		sndmsg_p->linkhandle = cmw_tblClientInfo[tcpconn].tcplinkhdl;

		/* Send TCP data tx request (CPRI data send request) to LAN tx task	*/
		ret_buf = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, sndmsg_p);
		if(ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
		}
	}

	/* Buffer request	*/
	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_L1DATCNF), (VOID **)&datacfm_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
		"m_mk_CpriSimu_DataSnd", 
		"Buffer request error", 
		ret_buf, 
		sizeof(CMT_TSKIF_L1DATCNF), 
		CMD_NUM0);	
	}
	
	datacfm_p->link_num = link_num;
	datacfm_p->head.uiEventNo = CMD_TSKIF_L1DATCNF;
	datacfm_p->head.uiSignalkind = 0;
	datacfm_p->head.uiLength = sizeof(CMT_TSKIF_L1DATCNF);
	datacfm_p->head.uiDstPQueueID = D_RRH_PROCQUE_L2;
	datacfm_p->head.uiDstTQueueID = 0;
	datacfm_p->head.uiSrcPQueueID = D_RRH_PROCQUE_MT;
	datacfm_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
	
	/* Send L1 data tx confirm to LAPB task		*/
	ret_buf = cm_Enter(D_RRH_PROCQUE_L2, CMD_QRB_NORMAL, CMD_TSKIF_L1DATCNF, datacfm_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
	}
	
	mkw_needfrmsg = MKD_FALSE;
}
/* @} */
