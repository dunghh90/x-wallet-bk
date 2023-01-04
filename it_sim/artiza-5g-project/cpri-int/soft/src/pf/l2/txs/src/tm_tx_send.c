/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_tx_Send.c
 *  @brief  L1 data sending process
 *  @date   2008/07/15 FFCS)Zouw Create 
 *  @date   2013.11.22 FFCS)hongj modify for zynq.
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/


/*! @addtogroup RRH_L2_TXS
 * @{ */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "l2_com_inc.h"			/* TXS task header					*/
#include "f_rrh_reg_cpri.h"

#include "m_cm_header.h"			/* TRIF common head file     			*/
#include "tm_tx_header.h"			/* TXS task header					*/
#include "l2_main_inc.h"			/* TXS task header					*/
#include "rrhApi_Dbg.h"				/* TXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  L1 data sending process.
 *  @note   1. If simulator mode, send data to MK MT mng task, return.
 *              2. Check SET bit
 *              3. If SET bit normal, send data.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/17 FFCS)Zouw Create 
 *  @date   2013/11/25 FFCS)hongj modify for zynq 
 *  @date   2015/07/30 TDI)ikeda FHMでの17link対応
 *  @date	2016/03/10 TDI)satou シミュレータ環境でのバッファ解放漏れを修正
 */
/********************************************************************************************************************/
VOID	tm_tx_Send(UINT *inf_p)
{
	CMT_TSKIF_L1DATRQ *sndreq_p;		/* Data send request				*/
	UINT		bnbuf_p;				/* Pointer of bank buffer			*/
	UINT		*srcbuf_p;				/* Pointer of source buffer			*/
	UINT 		retry;					/* Retry times						*/
	UINT 		bank_idx;				/* Bank index						*/
	USHORT 		linkno;					/* Link no							*/
	UINT 		datalen;				/* Data length(Address + control + L3)*/
	UINT 		txcont;			/* Value of LAPB transmission buffer control reg*/
	UINT		contreg_addr;	/* Address of LAPB transmission buffer control reg*/
	UINT		dlenreg_addr;	/* Address of LAPB data length register		*/
	UINT		cprihdsiz;		/* CPRI header size							*/
	UINT		l3_len;			/* L3 data length							*/
	CMT_TSKIF_HEAD *monbuf_p=NULL;
	UINT						buf_ret;	/* Return value of buffer request	*/
#ifdef OPT_RRH_SIM
	CMT_TSKIF_L1DATCNF *datacnf;
#endif
	
	bnbuf_p = CMD_NUM0;
	srcbuf_p = NULL;
	retry = CMD_NUM0;
	bank_idx = CMD_NUM0;
	txcont = CMD_NUM0;
	sndreq_p = (CMT_TSKIF_L1DATRQ *)inf_p;
	linkno = sndreq_p->link_num;
	/* Check whether link no is valid		*/
	if((linkno >= D_RRH_CPRINO_NUM))
	{
		cm_MAbort(CMD_ALMCD_DATA, "tm_tx_Send", "Link no invalid", 
				linkno, CMD_NUM0, CMD_NUM0);
		txw_needfrmsg = TXD_FALSE;	
		
		return;
	}

	datalen = sndreq_p->length;				/* Data length (Address + control + L3)	*/
	
	if(datalen > TXD_BNBUF_SIZE)
	{
		cm_Assert( CMD_ASL_DBGLOW, datalen, "[tm_tx_Send] Data size > 10K");
		
		datalen = TXD_BNBUF_SIZE;
	}

	cprihdsiz = sizeof(CMT_TSKIF_L1DATRQ) - sizeof(CMT_TSKIF_HEAD);
	l3_len = datalen - sizeof(UCHAR) * CMD_NUM2;
	srcbuf_p = (UINT*)&(sndreq_p->Address);


	contreg_addr = (UINT)(M_RRH_REG_CM_LTXCNT(linkno));
	dlenreg_addr = CMD_NUM0;
	
	/* copy buffer for cpri monitor */
	if(l2_dbgw_mkmtmode[linkno].monitor == CMD_ON)
	{
		buf_ret = cm_BReq(CMD_BUFCA_MON, sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) +  cprihdsiz + l3_len, 
						(VOID **)&monbuf_p);
		if(buf_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, CMD_NUM0, "tm_tx_Send:monitor buffer error");
		}else{
			cm_MemCpy(((CHAR*)monbuf_p + sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD)),   /* pgr0689 */
						((CHAR*)sndreq_p + sizeof(CMT_TSKIF_HEAD)),cprihdsiz + l3_len);
		}
	}
	
	if(l2_dbgw_mkmtmode[linkno].simulator )	/* If simulator mode		*/
	{
		/* Send TX request to Maker MT management task			*/
		 tm_tx_cm_SndSimuMsg( sndreq_p);
		
		/* Should not free message buffer (It is freed by LAPB task)		*/
		txw_needfrmsg = TXD_FALSE;		
	
		/*****************************************************************/
		/* Monitor mode process											*/
		/*****************************************************************/	
		if(monbuf_p != NULL) /* pgr0689 */
		{
			tm_tx_cm_SndMonMsg(linkno,monbuf_p,cprihdsiz + l3_len);
		}
		return;							/* Return at once					*/
	}

#ifdef OPT_RRH_SIM
	/* 仮想環境上ではL1dataの送信は完了しないため、ここで送信完了を返す.
	 * でないと、バッファが詰まってしまい、CPRI-MTが接続できない */
	(VOID)cm_BReq(CMD_BUFCA_TSKIF, sizeof *datacnf, (VOID**)&datacnf);
	datacnf->link_num           = linkno;
	datacnf->head.uiEventNo     = CMD_TSKIF_L1DATCNF;
	datacnf->head.uiSignalkind  = 0;
	datacnf->head.uiLength      = sizeof *datacnf;
	datacnf->head.uiDstPQueueID = 0;
	datacnf->head.uiDstTQueueID = CMD_TSKID_LPB;
	datacnf->head.uiSrcPQueueID = 0;
	datacnf->head.uiSrcTQueueID = CMD_TSKID_TXS;
	(VOID)cm_Enter(CMD_TSKID_LPB, CMD_QRB_NORMAL, CMD_TSKIF_L1DATCNF, datacnf);

	/*****************************************************************/
	/* Monitor mode process											*/
	/*****************************************************************/
	if(monbuf_p != NULL)
	/* If this link is in monitor mode	or CPRI TR ON */
	{
		tm_tx_cm_SndMonMsg(linkno,monbuf_p,cprihdsiz + l3_len);
	}
#else
	while(retry < TXD_MAX_RETRY)		/* While retry times not exceed		*/
	{
		/* Read transmission control register 		*/
		/*txcont = cm_ReadReg(contreg_addr);*/
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_HDL_READ,
						contreg_addr,&txcont);
		
		txcont &= TXD_MSK_TXSET;
		/* If "SET" bit of transmission control register becomes zero		*/
		if(!txcont)	
		{
			break;			/* Jump out while		*/
		}
		
		retry ++;			/* Increment retry times	*/
	}

	if(retry >= TXD_MAX_RETRY)			/* If retry times exceeds	*/
	{
		tm_lb_l2state_sub_ntc(linkno, D_L2_LINKSTATE_ABNORMAL, __func__);

		txw_needfrmsg = TXD_FALSE;	

		return;							/* Return at once		*/
	}
	
	/*****************************************************************/
	/* Send L1 data													*/
	/*****************************************************************/	
	tm_tx_SelectBank(linkno, &bank_idx);	/* Select a TX bank for this link*/

	tm_tx_LocBnBuf(linkno, bank_idx, &bnbuf_p);	/* Locate address of bank buffer*/

	BPF_HM_DEVC_REG_WRITE_BUFFER_END(D_RRH_LOG_REG_LV_HDL_WRITE_BUF,
					bnbuf_p,datalen,srcbuf_p);

	dlenreg_addr = (UINT)(M_RRH_REG_CM_LTXB10(linkno)  + TXD_ITV_TXDTL_BK * bank_idx);

	/* Specify the data length in LAPB transmission buffer data length register	*/
	/* blank number is even,shift it to high-16byte*/
	if((dlenreg_addr % CMD_NUM4) == CMD_NUM2)
	/**(UINT*)(dlenreg_addr - CMD_NUM2) = (datalen << CMD_NUM16) & 0x3fff0000;*/
	{
		datalen <<= CMD_NUM16;
		datalen &= 0x3fff0000;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,
					dlenreg_addr - CMD_NUM2,&datalen);
	}
	else
	{
		datalen &= 0x3fff;
		/**(UINT*)dlenreg_addr = datalen & 0x3fff;*/
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,
					dlenreg_addr,&datalen);
	}

	/* Specify bank number in LAPB transmission control register(write 0 to SET bit)	*/
	/**(UINT*)contreg_addr = (TXD_MSK_ZERO | bank_idx);*/
	bank_idx |= TXD_MSK_ZERO;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,
					contreg_addr,&bank_idx);
	/* Write 1 to SET bit of transmission control register, to begin transmission*/
	/**(UINT*)contreg_addr = (TXD_MSK_TXSET | bank_idx);*/
	bank_idx |= TXD_MSK_TXSET;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,
					contreg_addr,&bank_idx);
	
	/*****************************************************************/
	/* Monitor mode process											*/
	/*****************************************************************/	
	if(monbuf_p != NULL)
	/* If this link is in monitor mode	or CPRI TR ON */
	{	
		tm_tx_cm_SndMonMsg(linkno,monbuf_p,cprihdsiz + l3_len);
	}
#endif

	/* Should not free message buffer (It is freed by LAPB task)		*/
	txw_needfrmsg = TXD_FALSE;	

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  monitor data sending process.
 *  @note   send data to MKM task in monitor mode.
 *  @param  linkno [in] CPRI link number
 *  @param  monbuf_p [out] CPRI monitor buffer
 *  @param  a_msglen [in] Length of message
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/15 FFCS)Zouw Create
 *  @date   2013/11/25 FFCS)hongj modify for zynq 
 *  @date   2015/07/31 TDI)ikeda FHMでの17link対応
 */
/********************************************************************************************************************/

VOID tm_tx_cm_SndMonMsg( INT linkno, CMT_TSKIF_HEAD *monbuf_p,INT a_msglen)
{
	CMT_TSKIF_MKMTHEAD			*MKMTHEAD_p;
	T_RRH_SYSTEM_TIME					dt;
	CHAR			*l1Data;

	monbuf_p->uiEventNo     = CMD_TSKIF_L1MONDATSNDNTC;
	monbuf_p->uiDstPQueueID = l2_dbgw_mkmtmode[linkno].rspQID_moni;
	monbuf_p->uiDstTQueueID = 0;
	monbuf_p->uiSrcPQueueID = D_RRH_PROCQUE_L2;
	monbuf_p->uiSrcTQueueID = CMD_TSKID_SND;
	monbuf_p->uiLength = sizeof(CMT_TSKIF_HEAD) + a_msglen + sizeof( CMT_TSKIF_MKMTHEAD) ;
	MKMTHEAD_p = (CMT_TSKIF_MKMTHEAD*)((CHAR*)monbuf_p + sizeof(CMT_TSKIF_HEAD));
	cm_RtcGet(&dt); /* pgr0548 */
	dt.year = l2_com_swap_USHORT(dt.year);
	cm_MemCpy((VOID *)(&(MKMTHEAD_p->dummy1)),&dt,sizeof(DATETIME_T)); /* pgr0548 dummy2のエリアを見越してコピーしているため問題無し */

	/* to big endian*/
	l1Data  = (CHAR*)monbuf_p;
	/*link num*/
	*(USHORT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD)] = l2_com_swap_USHORT(*(USHORT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD)]);
	/*length*/
	*(UINT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) + 8] = l2_com_swap_UINT(*(UINT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) + 8]);
	/*disp*/
	*(UINT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) + 12] = l2_com_swap_UINT(*(UINT*)&l1Data[sizeof(CMT_TSKIF_HEAD) + sizeof( CMT_TSKIF_MKMTHEAD) + 12]);

	/* Send L1 data send request(monitor) to Maker MT management task	*/
	l2_com_monitorMsgToApl(l2_dbgw_mkmtmode[linkno].rspQID_moni, monbuf_p, monbuf_p->uiLength);	

	return;
}

/********************************************************************************************************************/
/*!
 *  @brief  simulator data sending process.
 *  @note   send data to MKM task in monitor mode.
 *  @param  *a_msg_p [in]  Pointer of message
 *  
 *  
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/25 FFCS)hongj modify for zynq 
 *  @date   2015/07/30 TDI)ikeda FHMでの17link対応
 */
/********************************************************************************************************************/
VOID tm_tx_cm_SndSimuMsg( CMT_TSKIF_L1DATRQ *a_msg_p)
{
	UINT					ret_buf;		/* Return value of buffer request	*/
	UINT					memcpylen;		/* Data length of mem copy			*/
	CMT_TSKIF_L1DATRQ		*desbuf_p;		/* Pointer of destination buffer	*/
	USHORT					linkno;

	linkno = a_msg_p->link_num;

	/* Buffer request	*/
	ret_buf = cm_BReq(CMD_BUFCA_MON, a_msg_p->head.uiLength, 
				(VOID **)&desbuf_p);

	if(ret_buf != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, 
			"m_mk_CpriSimu_DataSnd", 
			"Buffer request error", 
			ret_buf, 
			a_msg_p->head.uiLength, 
			CMD_NUM0);	
		return;
	}

	/* CPRI header size + L3 buffer size	*/
	memcpylen = a_msg_p->head.uiLength - sizeof(CMT_TSKIF_HEAD);	
			
	cm_MemCpy(&(desbuf_p->link_num), &(a_msg_p->link_num), memcpylen);

	desbuf_p->head.uiLength = a_msg_p->head.uiLength;
	/* to big endian*/
	desbuf_p->link_num = l2_com_swap_USHORT(desbuf_p->link_num);
	desbuf_p->length = l2_com_swap_UINT(desbuf_p->length);
	desbuf_p->disp = l2_com_swap_UINT(desbuf_p->disp);

	l2_com_cpriMsgToApl(D_RRH_PROCQUE_MT, desbuf_p, desbuf_p->head.uiLength);

	return;
}
/* @} */

