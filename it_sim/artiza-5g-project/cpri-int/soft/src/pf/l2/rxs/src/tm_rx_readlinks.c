/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_ReadLinks.c
 *  @brief  read out link data
 *  @date   2008/07/22 FFCS)Zouw Create
 *  @date   2013.11.22 FFCS)hongj modify for zynq. 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */
/********************************************************************************************************************/


/*! @addtogroup RRH_L2_RXS
 * @{ */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file     			*/
#include "tm_rx_header.h"			/* RXS task header					*/
#include "l2_com_inc.h"

/********************************************************************************************************************/
/*!
 *  @brief  read out link data.
 *  @note   Read data of all frames for the link
 *  @param  linkno    [in]  CPRIリンク番号
 *  @param  frame_num [in]  frame number
 *  @return Result code is returned
 *  @retval RXD_RET_ABNORMAL : 1 abnormal return
 *              RXD_RET_NORMAL     : 0 normal return
 *  @date   2008/07/22 FFCS)Zouw Create 
 *  @date   2013/11/25 FFCS)hongj modify for zynq
 *  @date   2015/06/19 ALP) murakami TDD-ZYNQ対応
 *  @date   2015/07/30 TDI) ikeda FHMでの17link対応
 */
/********************************************************************************************************************/
UINT tm_rx_ReadLinks(USHORT *linkno, USHORT frame_num )
{	
	CMT_TSKIF_L1DATRCVNTC *recvbuf_p;	/* Buffer pointer, used for receiving data	*/
	UINT 		bank_idx;	/* Bank index				*/
	UINT 		datalen;	/* Length of data(include address,control and L3 buffer size*/
	UINT		l3datalen;	/* L3 data lenght			*/
	UINT 		bnbuf_p;			/* Bank buffer		*/

	UINT		buf_ret;			/* Return value of buffer request	*/
	UINT		bknoreg_val;		/* Bank no notification reg value	*/

	UINT		contreg_val;		/* RX control reg value				*/
	UINT* 		tmpdata_p;
	UINT		contreg_addr;		/* Address of RX control reg value	*/
	UINT		rcvntcsiz;			/* Size of CMT_TSKIF_L1DATRCVNTC	*/
	UINT		ucharsiz;			/* Size of unsigned char	*/
	UINT		disp;				/* Offset					*/
	UINT		reg_val;
	UINT		loop_count;
	UINT		rx_flg_bit;
	UINT 		lpCnt=0;
	USHORT		num;
	
	if(frame_num == CMD_NUM0)
	{
		return RXD_RET_ABNORMAL;
	}
	bank_idx = CMD_NUM0;
	datalen = CMD_NUM0;
	recvbuf_p = NULL;
	bnbuf_p = CMD_NUM0;
	buf_ret = CMD_NUM0;

	bknoreg_val = CMD_NUM0;
	contreg_val = CMD_NUM0;
	contreg_addr = CMD_NUM0;
	rcvntcsiz = sizeof(CMT_TSKIF_L1DATRCVNTC);
	ucharsiz = sizeof(UCHAR);
	disp = rcvntcsiz - ucharsiz * CMD_NUM4;
	rx_flg_bit = CMD_NUM0;
	
	num = frame_num;
	
	while(num > CMD_NUM0)				/* While there are frames to be read	*/
	{
		/* Read "bank no notification register"	*/
		/*bknoreg_val = *(UINT*)D_RRH_REG_CPRI_LRXST;*/
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,
			M_RRH_REG_CM_LRXBNK(linkno[lpCnt]),&bknoreg_val);
		bank_idx = (bknoreg_val & RXD_MSK_BKNTC);

		/* If EN bit is 0	*/
		if((bknoreg_val & RXD_MSK_RXEN) == RXD_MSK_ZERO)
		{	
			cm_Assert(CMD_ASL_DBGHIGH, ((frame_num<<24)+(num<<16)+(linkno[lpCnt]<<8)+bknoreg_val), "RXD_MSK_RXEN read NG" );
			/*	次に処理するリンク番号を設定する	*/
			lpCnt++;
			/* Decrease the amount of remaining frames of this link by 1	*/
			num --;
			continue;								/* Go to next loop	*/
		}

		contreg_addr = (UINT)(M_RRH_REG_CM_LRXB10(linkno[lpCnt]) + ((bank_idx / 2) * RXD_ITV_RXCONT_BK));

		/* Read "RX control register"		*/
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,contreg_addr,&contreg_val);
		/* blank number is even */
		if((bank_idx % CMD_NUM2) == CMD_NUM1)
		{
			contreg_val >>= CMD_NUM16;
		}
		
		/* contreg_valにはFCSデータは含まないデータ長が設定されている */
		datalen = contreg_val & RXD_MSK_DTL;
		/*	データ長が0の場合	*/
		if(datalen == 0)
		{
			cm_Assert(CMD_ASL_DBGHIGH, ((num<<24)+(linkno[lpCnt]<<16)+datalen), "datalen=0 NG" );
			/*	次に処理するリンク番号を設定する	*/
			lpCnt++;
			/* Decrease the amount of remaining frames of this link by 1	*/
			num --;
			continue;								/* Go to next loop	*/
		}
		/*	データ長が2未満(address/controlがない)の場合	*/
		else if(datalen < (ucharsiz + ucharsiz))
		{
			cm_Assert(CMD_ASL_DBGHIGH, ((num<<24)+(linkno[lpCnt]<<16)+datalen), "datalen<2 NG" );
			/*	次に処理するリンク番号を設定する	*/
			/* アドレス部 制御部をカット */
			l3datalen = 0;
		}
		/*	データ長が2以上の場合	*/
		else
		{
			/* アドレス部 制御部をカット */
			l3datalen = (UINT)(datalen - ucharsiz - ucharsiz);
		}
		/* 100us以内にDDR受信フラグが1になるか */
		rx_flg_bit = 0x00010000;
		rx_flg_bit <<= bank_idx;
		
		for(loop_count = CMD_NUM0; loop_count < (RXD_FLG_TIME_100US / RXD_FLG_TIME_POLLING); loop_count++)
		{
			usleep(RXD_FLG_TIME_POLLING);
			
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,
				M_RRH_REG_DDR_RXFLG(bank_idx, linkno[lpCnt]), &reg_val);
			
			if((reg_val & rx_flg_bit) == rx_flg_bit)
			{
				break;
			}
		}
		
		/* DDR受信フラグが100us以内に1になった場合 */
		if(loop_count < (RXD_FLG_TIME_100US / RXD_FLG_TIME_POLLING) )
		{
			
			/* L2 buffer request			*/
			buf_ret = cm_L2BfGet(CMD_BUFCA_LAPRCV, rcvntcsiz + datalen, 
										linkno[lpCnt], (VOID **)&recvbuf_p);		

			if(buf_ret != CMD_RES_OK)				/* If request buffer NG		*/
			{
				cm_MAbort(CMD_ALMCD_BUFGET, 
				"tm_rx_ReadLinks", 
				"Buffer request error", 
				buf_ret, 
				rcvntcsiz + datalen, 
				CMD_NUM0);
			}
				
			tm_rx_LocBnBuf(linkno[lpCnt], bank_idx, &bnbuf_p);	/* Locate bank buffer */

			/* Copy bank buffer to soft buffer			*/
			tmpdata_p = (UINT*)&(recvbuf_p->Address);
			BPF_HM_DEVC_REG_READ_BUFFER_END(D_RRH_LOG_REG_LV_HDL_READ_BUF, bnbuf_p, datalen, tmpdata_p);
			
			/* DDR受信FLGクリア*/
			reg_val = 0x00000000;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,
				M_RRH_REG_DDR_RXFLG(bank_idx, linkno[lpCnt]),&reg_val);

			recvbuf_p->link_num = linkno[lpCnt];		/* Set link no		*/
			recvbuf_p->length = datalen;						/* Set data length	*/

			/* Set the offset from task head to the address field	*/
			recvbuf_p->disp = disp;	

			recvbuf_p->head.uiLength = rcvntcsiz + l3datalen;
			recvbuf_p->head.uiEventNo = CMD_TSKIF_L1DATRCVNTC;
			recvbuf_p->head.uiSrcTQueueID = CMD_TSKID_RXS;
			recvbuf_p->head.uiDstTQueueID = CMD_TSKID_LPB;
			recvbuf_p->head.uiSrcPQueueID = D_RRH_PROCQUE_L2;
			recvbuf_p->head.uiDstPQueueID = D_RRH_PROCQUE_L2;

			if(l2_dbgw_mkmtmode[linkno[lpCnt]].monitor == CMD_ON)
			/* If this link is in monitor mode	or CPRI TR ON */
			{
				tm_rx_cm_SndMonMsg((CMT_TSKIF_HEAD*)recvbuf_p,recvbuf_p->head.uiLength - sizeof(CMT_TSKIF_HEAD));
			}
				
			/* Send L1 data RX notification to LAPB task			*/
			buf_ret = cm_Enter(CMD_TSKID_LPB, CMD_QRB_NORMAL, CMD_TSKIF_L1DATRCVNTC, recvbuf_p);	
			if(buf_ret != CMD_RES_OK)
			{
				cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
			}
		}
		else
		{
			cm_Assert(CMD_ASL_DBGHIGH, ((num<<24)+(linkno[lpCnt]<<16)+loop_count), "DDR recv flag TO" );
		}
		
		/* Clear "RDY" bit of RX control reg	*/
		contreg_val = 0x00010000;
		contreg_val <<= bank_idx;
		
		/**(UINT*)D_RRH_REG_CPRI_LRXST = contreg_val;*/
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,
			M_RRH_REG_CM_LRXST(linkno[lpCnt]),&contreg_val);
		
		/*	次に処理するリンク番号を設定する	*/
		lpCnt++;
		/* Decrease the amount of remaining frames of this link by 1	*/
		num --;
	}

	return RXD_RET_NORMAL;
}

/********************************************************************************************************************/
/*!
 *  @brief  Monitor Data Sending Management.
 *  @note   send data to mkmt task in montor mode
 *  @param  *a_msg_p [in]  Pointer of message
 *  @param  a_msglen [in] Length of message	
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/22 FFCS)Zouw Create 
 *  @date   2013/11/25 FFCS)hongj modify for zynq 
 *  @date   2015/7/29 TDIPS)ikeda rev.27586 コード修正
 */
/********************************************************************************************************************/
VOID tm_rx_cm_SndMonMsg(CMT_TSKIF_HEAD *a_msg_p,INT a_msglen)
{
	UINT						buf_ret;				/* Return value of buffer request	*/
	CMT_TSKIF_HEAD 				*monbuf_p;
	CMT_TSKIF_MKMTHEAD			*MKMTHEAD_p;
	T_RRH_SYSTEM_TIME					dt;
	CHAR			*l1Data;
	USHORT linkno;

	linkno = ((CMT_TSKIF_L1DATRCVNTC *)a_msg_p)->link_num;

	buf_ret = cm_BReq(CMD_BUFCA_MON, sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) +  a_msglen, 
					(VOID **)&monbuf_p);

	if(buf_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, CMD_NUM0, "tm_rx_readlinks:monitor buffer error");
#if 0
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"tm_tx_Send", 
			"Buffer request error", 
			buf_ret, 
			sizeof(CMT_TSKIF_HEAD) + a_msglen, 
			NULL);	
#endif
		return;
	}

	cm_MemCpy(((CHAR*)monbuf_p + sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD)), 
				((CHAR*)a_msg_p + sizeof(CMT_TSKIF_HEAD)),a_msglen);

	monbuf_p->uiEventNo     = CMD_TSKIF_L1MONDATRCVNTC;
	monbuf_p->uiDstPQueueID = l2_dbgw_mkmtmode[linkno].rspQID_moni;
	monbuf_p->uiDstTQueueID = 0;
	monbuf_p->uiSrcPQueueID = D_RRH_PROCQUE_L2;
	monbuf_p->uiSrcTQueueID = CMD_TSKID_RXS;
	monbuf_p->uiLength = sizeof(CMT_TSKIF_HEAD) + a_msglen + sizeof( CMT_TSKIF_MKMTHEAD) ;

	MKMTHEAD_p = (CMT_TSKIF_MKMTHEAD*)((CHAR*)monbuf_p + sizeof(CMT_TSKIF_HEAD));
	cm_RtcGet(&dt);
	dt.year = l2_com_swap_USHORT(dt.year);
	cm_MemCpy(&(MKMTHEAD_p->dummy1),&dt,sizeof(DATETIME_T)); /* pgr0548 */

	/* to big endian*/
	l1Data  = (CHAR*)monbuf_p;
	/*link num*/
	*(USHORT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD)] = l2_com_swap_USHORT(*(USHORT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD)]);
	/*length*/
	*(UINT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) + 8] = l2_com_swap_UINT(*(UINT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) + 8]);
	/*disp*/
	*(UINT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) + 12] = l2_com_swap_UINT(*(UINT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) + 12]);
	
	/* Send L1 data send request(monitor) to Maker MT management task	*/
	l2_com_monitorMsgToApl(l2_dbgw_mkmtmode[linkno].rspQID_moni,monbuf_p,monbuf_p->uiLength);

	return;
}


/* @} */

