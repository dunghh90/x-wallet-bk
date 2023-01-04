/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_FLog_switch.c
 *  @brief  強化ログ待避
 *  @date   20140411
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2014
 */
 
#include "f_trp_com.h"				/* TRIFプロセス共通ヘッダ			*/

/*! @addtogroup TRIF_TCM
 * @{ */

/*!
 *  @brief 強化ログ待避
 *  @note 
 *  
 *  @return none
 *  @retval none
 *  @date 20140411
 */
  
VOID f_trp_com_FLog_switch( VOID )
{
	UINT	count;
	int		errcd;		/* error code   */
	UINT	red_val		= CMD_NUM0;
	UINT	uit_ret		= CMD_NG;
	UINT	adradd		= D_TCM_REGADR_CPRI1_ERR_STATE;
	UINT	adradd_r	= D_TCM_REGADR_CPRI1_RFLM_ERR;
	T_TCM_IRQ_COUNT*		IrqCount;
	
	BPF_RU_IPCM_PROCSHM_ADDRGET(D_SHMID_CPU_IRQCNTTBL,(VOID*)(&IrqCount),&errcd);
	
	for(count=0;count<CMD_LINK_NUM_T;count++)
	{
#if defined(OPT_CPRI_L2HARD)
		if(count >= 2)
		{
			break;
		}
#endif
		/*-------------------------------*/
		/* TXS/RXS割り込み送受信表示ログ */
		/*-------------------------------*/
		f_trp_com_WlogTxsRxslog[f_trp_com_WlogWriteIndex].irqcount.send_IRQ_Count[count] = IrqCount->send_IRQ_Count[count];	/* DMA(HDLC sending)割込み受信カウント情報 */
		f_trp_com_WlogTxsRxslog[f_trp_com_WlogWriteIndex].irqcount.recv_IRQ_Count[count] = IrqCount->recv_IRQ_Count[count];	/* DMA(HDLC receiving)割込み受信カウント情報 */
		f_trp_com_WlogTxsRxslog[f_trp_com_WlogWriteIndex].irqcount.send_Clr_Count[count] = IrqCount->send_Clr_Count[count];	/* DMA表示レジスタクリアカウント情報(HDLC sending時) */
		f_trp_com_WlogTxsRxslog[f_trp_com_WlogWriteIndex].irqcount.recv_Clr_Count[count] = IrqCount->recv_Clr_Count[count];	/* DMA表示レジスタクリアカウント情報(HDLC receiving時) */
		f_trp_com_WlogTxsRxslog[f_trp_com_WlogWriteIndex].cmpcount[count] = f_trp_com_Wsndcmpcount[count];		/* DMA(HDLC Sending complete)送信完了カウンタ情報 */
		f_trp_com_WlogTxsRxslog[f_trp_com_WlogWriteIndex].sndcount[count] = f_trp_com_Wsndcount[count];			/* DMA(HDLC sending)送信カウント情報 */
	
//TBD		/*--------------------------------------*/
//		/* CPRIリンク障害要因ステータスレジスタ */
//		/*--------------------------------------*/
//		uit_ret = f_trp_com_Fread_reg( 		/* R: Result			*/
//		adradd,								/* I: Register adress	*/
//		&red_val							/* O: regval			*/
//		);
		
		if (uit_ret != CMD_OK)
		{
			F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_WARN2, "read register NG",(UCHAR *)&adradd, sizeof(adradd));
		}
		
		f_trp_com_WlogReglog[f_trp_com_WlogWriteIndex].cpr_err_state[count] = red_val;
		
#if defined(OPT_CPRI_L2HARD)
		/* Writeクリア */
		uit_ret = f_trp_com_Fwrite_reg(		/* R: Result			*/
		adradd,								/* I: Register adress	*/
		red_val								/* I: regval			*/
		);
		
		if (uit_ret != CMD_OK)
		{
			F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_WARN2, "write register NG",(UCHAR *)&adradd_r, sizeof(adradd_r));
		}
#endif
		adradd += D_TCM_REG_CPRI_ADRADD;
		
		/*-----------------------------------------*/
		/* CPRIリンク 対向フレーマ障害表示レジスタ */
		/*-----------------------------------------*/
//TBD		uit_ret = f_trp_com_Fread_reg( 		/* R: Result			*/
//		adradd_r,							/* I: Register adress	*/
//		&red_val							/* O: regval			*/
//		);
		
		if (uit_ret != CMD_OK)
		{
			F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_WARN2, "read register NG",(UCHAR *)&adradd_r, sizeof(adradd_r));
		}
		
		f_trp_com_WlogReglog[f_trp_com_WlogWriteIndex].cpr_rflm_err[count] = red_val;
		
//TBD		/* Writeクリア */
//		uit_ret = f_trp_com_Fwrite_reg(		/* R: Result			*/
//		adradd_r,							/* I: Register adress	*/
//		red_val								/* I: regval			*/
//		);
		
		if (uit_ret != CMD_OK)
		{
			F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_WARN2, "write register NG",(UCHAR *)&adradd_r, sizeof(adradd_r));
		}
		
		adradd_r += D_TCM_REG_CPRI_ADRADD_R;
	}
	
	/* Write Index */
	if( f_trp_com_WlogWriteIndex >= CMD_LOG_WRITE_INDEX_MAX-1 )
	{
		f_trp_com_WlogWriteIndex = 0;
	}
	else
	{
		f_trp_com_WlogWriteIndex++;
	}
	
	return ;
}
/* @} */  /* group TRIF_TCM */

