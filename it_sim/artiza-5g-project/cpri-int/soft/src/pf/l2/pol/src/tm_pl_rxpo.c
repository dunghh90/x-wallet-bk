/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_RxPo.c
 *  @brief  data receiving complete.
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
#include "m_cm_header.h"			/* TRIF common head file            			*/
#include "tm_pl_header.h"			/* POLLING task header					*/
#include "l2_com_inc.h"			
#include "f_rrh_reg_cnt.h"

/********************************************************************************************************************/
/*!
 *  @brief  data recieving complete.
 *  @note  Check data arrival.
 *  @param  - [in] -
 *  @param  -      [out] -
 *  @return Result code is returned.
 *  @retval -
 *  @date   2008/07/24 FFCS)Zouw Create 
 *  @date   2012/07/11 FFCS)niemsh modify for 1.5GVA
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応  
 */
/********************************************************************************************************************/
VOID tm_pl_RxPo(VOID)
{
	CMT_TSKIF_L1DATRCVNTC_L2 *recvbuf_p;
	INT			ret_buf;
	USHORT linkno;
	UINT rxntc;			/* RX notification register value	*/
	UINT linkNum = 0;
	UINT recvInfo[D_RRH_CPRINO_NUM][2];
	UINT bitMsk = CMD_ON;
	
	BPF_HM_DEVC_REG_READ(0, D_RRH_REG_CNTS_IRQLRXST, &rxntc);
	
	/* 17cpri対応 */
	for (linkno = D_RRH_CPRINO_REC; linkno < D_RRH_CPRINO_NUM; linkno++)
	{
		/*	一旦、all"f"で初期化する	*/
		recvInfo[linkNum][0] = 0xFFFFFFFF;
		
		if(linkno != D_RRH_CPRINO_REC)
		{
			bitMsk = CMD_ON<<(16+linkno-1);
		}
		else
		{
			bitMsk = CMD_ON;
		}
		if((rxntc & bitMsk) != CMD_NUM0)	/* If any link receives data		*/
		{
			/*	受信データがあるlink番号を設定する	*/
			recvInfo[linkNum][0]=linkno;
			/*	有効データがあるlink数をカウントアップする	*/
			linkNum++;
		}
	}
	if(linkNum != 0)
	{
		ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_L1DATRCVNTC_L2), (VOID **)&recvbuf_p);
		if(ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, CMD_NUM0, "tm_pl_RxPo:cm_BReq NG");
			return;
		}

		recvbuf_p->head.uiLength = sizeof(CMT_TSKIF_L1DATRCVNTC_L2);
		recvbuf_p->head.uiEventNo = CMD_TSKIF_L1DATRCVNTC;
		recvbuf_p->head.uiSrcTQueueID = CMD_TSKID_POL;
		recvbuf_p->head.uiDstTQueueID = CMD_TSKID_RXS;
		recvbuf_p->linkNum = linkNum;
		for (linkno = 0; linkno < linkNum; linkno++) /* pgr0692 */
		{
			recvbuf_p->recvInfo[linkno][0] = recvInfo[linkno][0];
		}
//		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "tm_pl_RxPo:Num=%d/reg=0x%08x", linkNum, rxntc);
		
		/* Send data RX notification to RXIOCS task (Without any buffer)	*/
		ret_buf = cm_Enter(CMD_TSKID_RXS, CMD_QRB_NORMAL, CMD_TSKIF_L1DATRCVNTC, recvbuf_p);
		if(ret_buf != CMD_RES_OK)
		{
			cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
		}
	}

	return;
}

/* @} */

