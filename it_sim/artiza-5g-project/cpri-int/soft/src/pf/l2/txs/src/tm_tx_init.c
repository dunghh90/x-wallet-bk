/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_tx_Init.c
 *  @brief  TXS task initialization.
 *  @date   2008/07/15 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_TXS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "l2_com_inc.h"
#include "f_rrh_reg_cpri.h"

#include "m_cm_header.h"			/* TRIF common head file	       		*/
#include "tm_tx_header.h"			/* TXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  TXS task initialization.
 *  @note  Set registers' addr to global variable.
 *             Clear LAPB transmission complete notification reg	
 *             Send init rsp
 *  @param  *inf_p [in] Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned.
 *  @retval -
 *  @date   2008/07/15 FFCS)Zouw Create 
 *  @date   2015/07/30 TDI)ikeda FHMでの17link対応
 */
/********************************************************************************************************************/
VOID tm_tx_Init(UINT *inf_p)
{
	UINT	ret_buf;
	CMT_TSKIF_HEAD *head_p;

	UINT loopnum = CMD_NUM0;

	txw_needfrmsg = TXD_TRUE;

	/* Send init response to init task			*/
	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_HEAD), (VOID **)&head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, CMD_NUM0, "tm_tx_Init:cm_BReq NG");
		return;
	}

	head_p->uiLength = sizeof(CMT_TSKIF_HEAD);
	head_p->uiEventNo = CMD_TSKIF_INIRSP;
	head_p->uiSrcTQueueID = CMD_TSKID_TXS;
	head_p->uiDstTQueueID = CMD_TSKID_INI;

	ret_buf = cm_Enter(CMD_TSKID_INI, CMD_QRB_NORMAL, CMD_TSKIF_INIRSP, head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
	}

	/* Update task state from init to use		*/
	txw_stateno = TXD_ST_USE ;

	/* init txw_curbankidx table*/
	for (loopnum = D_RRH_CPRINO_REC; loopnum < D_RRH_CPRINO_NUM; loopnum++)
	{
		txw_curbankidx[loopnum] = CMD_NUM0;
	}	
}

/* @} */

