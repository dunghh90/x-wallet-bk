/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_HB.c
 *  @brief   send heart beat response
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
 *  @brief     send heart beat response
 *  @note     send heart beat response
 *  @param  inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)zhengmh create
 *  @date     2013/11/26 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_mk_HB(UINT *inf_p)
{
	CMT_TSKIF_TCPSNDREQ 	*tcpsnd_p;
	CMT_TSKIF_TCPDATNTC	*tcprcv_p;
	INT 						buf_p[CMD_NUM3];
	INT							buf_ret;
	
	tcpsnd_p = (CMT_TSKIF_TCPSNDREQ*)inf_p;
	tcprcv_p	= (CMT_TSKIF_TCPDATNTC*)inf_p;
	
	cm_MemCpy(buf_p,&tcprcv_p->linkhandle,sizeof(buf_p)); /* pgr0548 */
	cm_MemCpy(&tcpsnd_p->linkhandle,buf_p,sizeof(buf_p)); /* pgr0548 */
	tcpsnd_p->msgbody = NULL;
	tcpsnd_p->msgevent = MKD_HBRSP;
	tcpsnd_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + CMD_NUM4 * sizeof(int);

	tcpsnd_p->head.uiEventNo = CMD_TSKIF_TCPSNDREQ;
	tcpsnd_p->head.uiSrcTQueueID = CMD_TSKID_MKM;
	tcpsnd_p->head.uiDstTQueueID = CMD_TSKID_LNS;
	/* TCP data tx request	(Health check response)	*/
	buf_ret = cm_Enter(CMD_TSKID_LNS, CMD_QRB_NORMAL, CMD_TSKIF_TCPSNDREQ, tcpsnd_p);
	if(buf_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
	}

	mkw_needfrmsg = MKD_FALSE;
}
/* @} */
