/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_TxPo.c
 *  @brief  data sending complete.
 *  @date   2008/07/24 FFCS)Zouw Create 
 *  @date   2012/07/11 FFCS)niemsh modify for 1.5GVA  
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_POLLING
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file           			*/
#include "tm_pl_header.h"			/* POLLING task header					*/
#include "l2_com_inc.h"			
#include "f_rrh_reg_cpri.h"


/********************************************************************************************************************/
/*!
 *  @brief  data sending complete.
 *  @note   Check transmission completion.
 *  @param  linkno [in] CPRI link number
 *  
 *  @return Result code is returned.
 *  @retval -
 *  @date   2008/07/24 FFCS)Zouw Create 
 *  @date   2012/07/11 FFCS)niemsh modify for 1.5GVA
 *  @date   2015/07/30 TDI)ikeda FHMの17link対応
 */
/********************************************************************************************************************/
VOID tm_pl_TxPo(USHORT linkno)
{
	CMT_TSKIF_L1DATCNF *txend_p;	/* Pointer of L1 data send confirm IF	*/
	UINT			bit_result;	/* Bit masked result				*/
	UINT			bit_mask;		/* Bit mask value					*/
	UINT			buf_ret;		/* Return value of buffer request	*/
	UINT			txendreg;		/* Transmission completion register value	*/
	UINT			msk_txend;		/* Valid bits of LAPB transmission end register	*/
	UINT			setbit;			/* setbit */

	bit_result = CMD_NUM0;
	buf_ret = CMD_NUM0;
	txend_p = NULL;
	txendreg = CMD_NUM0;
	setbit = CMD_NUM0;

	bit_mask = PLD_MSK_ONE;	
	

	/* bit15,bit16を取り出すためのマスク情報　*/
	msk_txend = 0x00018000;

	/* Read LAPB transmission completion register	*/
	/*txendreg = *(UINT*)D_RRH_REG_CPRI_LTXCNT;*/
	BPF_HM_DEVC_REG_READ(0,
		M_RRH_REG_CM_LTXCNT(linkno),&txendreg);
	setbit = txendreg;	/* setbit */
	txendreg >>= CMD_NUM16;
	/* If no bit of LAPB transmission completion register equals 1*/
	/* bit15 OFF, bit16 ONの場合　*/
	if((setbit & msk_txend) != 0x00010000)	
	{
//		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "tm_pl_TxPo:reg:0x%08x=0x%08x", M_RRH_REG_CM_LTXCNT(linkno), setbit);
		return;			/* Return at once		*/
	}
	/* Clear corresponding bits of transmission completion register */
	/**(UINT*)D_RRH_REG_CPRI_LTXCNT = CMD_NUM_0x10000;*/
	BPF_HM_DEVC_REG_WRITE(0,
		M_RRH_REG_CM_LTXCNT(linkno),&setbit);


	/* Mask Transmission completion register value	*/
	bit_result = (txendreg & bit_mask);	
	if(bit_result)			/* If this link transmission completion	*/
	{
		/* Buffer request		*/
		buf_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_L1DATCNF), (VOID **)&txend_p);	

		if(buf_ret != CMD_RES_OK)	/* If buffer request NG			*/
		{
			/* Abort				*/
			cm_MAbort(CMD_ALMCD_BUFGET, 
			"tm_pl_TxPo", 
			"Buffer request error", 
			buf_ret, 
			sizeof(CMT_TSKIF_L1DATCNF), 
			CMD_NUM0);
		}
			
		txend_p->link_num = linkno;		/* Set link no		*/
		txend_p->head.uiLength =
				sizeof(CMT_TSKIF_L1DATCNF);

		txend_p->head.uiLength = sizeof(CMT_TSKIF_L1DATCNF);
		txend_p->head.uiEventNo = CMD_TSKIF_L1DATCNF;
		txend_p->head.uiSrcTQueueID = CMD_TSKID_POL;
		txend_p->head.uiDstTQueueID = CMD_TSKID_LPB;
		/* Send transmission confirm to LAPB task			*/
		buf_ret = cm_Enter(CMD_TSKID_LPB, CMD_QRB_NORMAL, CMD_TSKIF_L1DATCNF, txend_p);
		if(buf_ret != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
		}
	}

	return;
}

/* @} */

