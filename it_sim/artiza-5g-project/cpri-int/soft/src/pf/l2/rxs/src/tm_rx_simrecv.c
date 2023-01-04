/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_Simrecv.c
 *  @brief  simulator data receiving
 *  @date   2008/07/22 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/


/*! @addtogroup RRH_L2_RXS
 * @{ */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file	       		*/
#include "tm_rx_header.h"			/* RXS task header					*/
#include "l2_com_inc.h"

/********************************************************************************************************************/
/*!
 *  @brief  simulator data receiving.
 *  @note   Send received simulator data to LPB directly
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/18 FFCS)Zouw Create 
 *  @date   2015/09/17 TDI)satou link_idxの減算処理を削除
 */
/********************************************************************************************************************/
VOID tm_rx_Simrecv(UINT *inf_p)
{
	CMT_TSKIF_L1DATRCVNTC	*rcvdata_p;		/* Monitor of received data		*/
	UINT					cprihdsiz;		/* CPRI header size				*/
	UINT					l3_len;			/* L3 data length				*/
	INT						buf_ret;

	rcvdata_p = (CMT_TSKIF_L1DATRCVNTC *)inf_p;
	/*to little endian*/
	rcvdata_p->link_num = l2_com_swap_USHORT(rcvdata_p->link_num);
	
	cprihdsiz = sizeof(CMT_TSKIF_L1DATRCVNTC) - sizeof(CMT_TSKIF_HEAD);
	l3_len = rcvdata_p->length - sizeof(UCHAR) * CMD_NUM2;

	/* Should not free message buffer (Simulator buffer is sent to LAPB task directly)	*/
	rxw_needfrmsg = RXD_FALSE;					

   	rcvdata_p->head.uiLength = sizeof(CMT_TSKIF_L1DATRCVNTC) + rcvdata_p->length - sizeof(UCHAR) * CMD_NUM2;
	rcvdata_p->head.uiEventNo = CMD_TSKIF_L1DATRCVNTC;
	rcvdata_p->head.uiSrcTQueueID = CMD_TSKID_RXS;
	rcvdata_p->head.uiDstTQueueID = CMD_TSKID_LPB;
	rcvdata_p->head.uiDstPQueueID = 0;

	/* If monitor is ON		*/
	if(l2_dbgw_mkmtmode[rcvdata_p->link_num].monitor == CMD_ON)
	{
		tm_rx_cm_SndMonMsg((CMT_TSKIF_HEAD*)rcvdata_p,cprihdsiz + l3_len);
	}

	/* Send RX notification to LAPB task	(simulator) 	*/
	buf_ret = cm_Enter(CMD_TSKID_LPB, CMD_QRB_NORMAL, CMD_TSKIF_L1DATRCVNTC, inf_p);
	if(buf_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
	}
	return;

}

/* @} */
	
