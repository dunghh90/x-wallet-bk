/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_CpriMon_StrReq.c
 *  @brief  Start monitor for L2 process
 *  @date   2008/07/25 FFCS)zhengmh create
 *  @date   2013/11/28 FFCS)zhaodx  modify for zynq
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
 *  @brief    Start monitor for L2 process
 *  @note     Start monitor for L2 process
 *  @param    inf_p      [in] Pointer of buffer
 *  @param    linkhandle [in] TCP link handle
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/28 FFCS)zhaodx  modify for zynq
 *  @date     2015/09/16 TDI)satou CPRIリンク番号を要求から取り出すように
 *  @date     2015/09/24 TDI)satou 引数にlinkhandleを追加
 */
/********************************************************************************************************************/
VOID m_mk_CpriMon_StrReq(UINT *inf_p, INT linkhandle)
{
	CMT_TSKIF_TCPSNDREQ 	*sndmsg_p;		/* Pointer of TCP data send req IF	*/
	INT 					wtime;
	UINT					linkno;
	INT						buf_ret;
	UINT					linkno_min;
	UINT					linkno_max;
	USHORT					*rcvMsg_p;
	sndmsg_p = NULL;
	wtime = 0;

	if(inf_p == NULL)
	{
		cm_Assert(CMD_ASL_DBGHIGH, (INT)inf_p, "NULL POINTER" );
		return;
	}
	rcvMsg_p = (USHORT*)inf_p;
	/*	リンク情報に移動する	*/
	rcvMsg_p += 8;
	/*	モニタ開始対象のCPRIリンク番号を取得する	*/
	mnt_com_conv_linkhandle_to_internalrange(linkhandle, &linkno_min, &linkno_max);
	/* call api to start up monitor mode  */
	for (linkno = linkno_min; linkno < linkno_max+1; linkno++, rcvMsg_p++)
	{
		/*	リンク情報がStartの場合	*/
		if(*rcvMsg_p != 0)
		{
			/*	L2にモニタ開始のAPIを発行する	*/
			rrhApi_Dbg_Mnt_StartupCpriMonitor(D_RRH_PROCQUE_MT, wtime, D_RRH_NULL, linkno);

			/* call api to subscribe cpri state from pf proc*/
			rrhApi_Cpri_Mpr_SubscribeLinkState(linkno, D_RRH_PROCQUE_MT,wtime, D_RRH_NULL);

			/* call api to subscribe lapb link state  */
			m_mk_subl2state(linkno);
		}
	}
	/* Set TCP data tx message	*/
	m_mk_cm_SetSndMsg(MKD_CPRIMONSTRRSP, NULL, &sndmsg_p);
	sndmsg_p->linkhandle = linkhandle;

	/* Send TCP data tx request(CPRI monitor start response) to LAN tx task	*/
	buf_ret = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, sndmsg_p);
	if(buf_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
	}
}
/* @} */
