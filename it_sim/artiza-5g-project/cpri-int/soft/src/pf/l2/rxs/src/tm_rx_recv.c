/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_Recv.c
 *  @brief  L1 data receiving process
 *  @date   2008/07/18 FFCS)Zouw Create 
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

/********************************************************************************************************************/
/*!
 *  @brief  L1 data receiving process.
 *  @note   While haven't read maxmum frames, read data of all links
 *              and save into soft	
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/18 FFCS)Zouw Create 
 *  @date   2015/07/30 TDI)ikeda FHMでの17link対応
 */
/********************************************************************************************************************/
VOID tm_rx_Recv(UINT *inf_p)
{
	UINT ret_rl;		/* Return value of reading links	*/
	CMT_TSKIF_L1DATRCVNTC_L2 *rcv_p;
	UINT	bit_mask;
	UINT	bit_chk;
	UINT	bank_idx;
	USHORT	linkNo[8*17];
	USHORT	msgCnt = 0;
	USHORT	i;
	
	rcv_p = (CMT_TSKIF_L1DATRCVNTC_L2 *)inf_p;
	/*	bankの先頭からチェックする	*/
	bit_mask  = 0x00010000;
	for (bank_idx = CMD_NUM0 ;bank_idx < RXD_BD_MDEIF_BANK; bank_idx ++)
	{
		bit_chk = (bit_mask << bank_idx);
		
		for (i=CMD_NUM0; i<(rcv_p->linkNum); i++) /* pgr0692 */
		{
			/*	CPRIリンクNoが範囲外の場合	*/
			if(rcv_p->recvInfo[i][0]>D_RRH_CPRINO_RE16)
			{
				continue;
			}
			if(bank_idx == 0)
			{
				rcv_p->recvInfo[i][1] = 0;
				BPF_HM_DEVC_REG_READ(0,
					M_RRH_REG_CM_LRXST(rcv_p->recvInfo[i][0]),&rcv_p->recvInfo[i][1]);
			}
			if(((rcv_p->recvInfo[i][1] & CMD_ON) != 0)&&((rcv_p->recvInfo[i][1] & bit_chk) != 0))
			{
				linkNo[msgCnt] = rcv_p->recvInfo[i][0];
				msgCnt++;
			}
		}
	}
	if(msgCnt != 0)
	{
		/* Read all RX links*/
		ret_rl = tm_rx_ReadLinks(linkNo, msgCnt);
		if(ret_rl == RXD_RET_ABNORMAL)			/* return abnormal 	*/
		{
			/*	nop	*/
		}
	}
	return;
}

/* @} */

