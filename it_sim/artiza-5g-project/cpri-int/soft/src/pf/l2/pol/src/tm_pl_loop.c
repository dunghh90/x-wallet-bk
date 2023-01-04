/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_Loop.c
 *  @brief  Check hardware looply.
 *  @date   2008/07/24 FFCS)Zouw Create 
 *  @date   2013/11/25 FFCS)hongj Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2015
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_POLLING
 * @{ */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRIF common head file           		*/
#include "tm_pl_header.h"			/* POLLING task header					*/
#include "l2_com_inc.h"
#include "f_rrh_reg_cnt.h"

/********************************************************************************************************************/
/*!
 *  @brief  check hardware looply.
 *  @note  Check hardware periodically.
 *  @param  *inf_p [in] pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned.
 *  @retval -
 *  @date   2008/07/24 FFCS)Zouw Create 
 *  @date   2013/11/25 FFCS)hongj modify for zynq 
 *  @date   2015/06/17 ALP)murakami TDD-ZYNQ対応
 *  @date   2015/07/30 TDI)ikeda FHMでの17cpri対応
 *  @date   2015/07/31 TDI)satou コメント修正
 *  @date   2015/07/31 TDI)satou 「LAPB transmission complete notice register ライト1クリア」削除(仕様書の誤記)
 */
/********************************************************************************************************************/
VOID tm_pl_Loop(UINT *inf_p)
{	
	UINT		rx_reg;					/* LAPB RX notification register value */
	UINT		bit_mask ;
	UINT		bknoreg_val;				/* Bank no notification reg value	*/
	UINT		reg_val;
	UINT		rx_suffix;
	UINT		linkno;
	UINT		txntc;			/* Tx notification register value	*/
	UINT		bitMsk = CMD_ON;
	UINT		linkNum;		/*	for debug	*/
	
#ifndef OPT_RRH_SIM	

	/*	LAPB送受信マスク解除	*/
	reg_val = 0x0000FFFE;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_CNTS_IRQLRXSTM,&reg_val);
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, D_RRH_REG_CNTS_IRQLTXSTM,&reg_val);
	/* 17cpri対応 */
	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++)
	{
	    /*
	     * L1確立からソフトのLAPBリンクUPまでの間に、LAPB受信データ(SABM等)が複数受信済みに
	     * なっているため、以下の初期化を行う。
	     *
	     * ※受信コントロールレジスタ(M_RRH_REG_CM_LRXST)の複数の有効BANKを一度に纏めて
	     *   "1"ライトクリアを行うのは禁止事項。実施した場合は、それ以後の新規LAPB受信の際に
	     *   実際に送信データが格納されるBANKとLAPB受信BANK番号通知レジスタ(M_RRH_REG_CM_LRXBNK)
	     *   の表示とで不整合が発生するため。
	     *
	     * ※通常の受信フローと同じく、LAPB受信BANK番号通知(M_RRH_REG_CM_LRXBNK)の読出しと、
	     *   該当BANKの受信STクリアをワンセットとして有効BANK分のクリア処理を繰り返し実施する。
	     */
		BPF_HM_DEVC_REG_READ(0,
			M_RRH_REG_CM_LRXST(linkno),&rx_reg);	/* Read LAPB RX notification reg */
		while((rx_reg & CMD_NUM1) != CMD_NUM0)		/* If any link receives data		*/
		{
			/*check there is bit*/
			if((rx_reg & 0xff0000) == CMD_NUM0)
			{
				break;
			}
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,
				M_RRH_REG_CM_LRXBNK(linkno),&bknoreg_val);
			bit_mask = (CMD_NUM1 << CMD_NUM16);
			bit_mask <<= (bknoreg_val & CMD_NUM7);
			rx_suffix = bknoreg_val & CMD_NUM7;
			
			/* DDR受信FLGクリア*/
			reg_val = 0x00000000;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,
				M_RRH_REG_DDR_RXFLG(rx_suffix, linkno),&reg_val);
			
			/* clear bit*/
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,			/* clear LAPB RX notification reg */
				M_RRH_REG_CM_LRXST(linkno),&bit_mask);

			/* Read LAPB RX notification reg again*/
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,
				M_RRH_REG_CM_LRXST(linkno),&rx_reg);	/* Read LAPB RX notification reg */
		}
	}
	
#endif	
	for(;;)
	{
#ifndef OPT_RRH_SIM	
		
		BPF_HM_DEVC_REG_READ(0, D_RRH_REG_CNTS_IRQLTXST, &txntc);
		linkNum = 0;
		/* 17cpri対応 */
		for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++)
		{
			if(linkno != D_RRH_CPRINO_REC)
			{
				bitMsk = CMD_ON<<(16+linkno-1);
			}
			else
			{
				bitMsk = CMD_ON;
			}
			if((txntc & bitMsk) != CMD_NUM0)	/* If any link receives data		*/
			{
//				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CALL tm_pl_TxPo:link=%d, linkno);
				tm_pl_TxPo(linkno);		/* Check transmission completion	*/
			}
		}
		tm_pl_RxPo();		/* Check data arrival				*/
		
		/* wait for 4ms*/
		m_cm_Delay(4);
#else	
		/* wait for 4ms*/
		m_cm_Delay(400);
#endif
	}

	return;

}


/* @} */

