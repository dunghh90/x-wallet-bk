/****************************************************************************/
/* NAME		: 初期化処理										 			*/
/*																			*/
/* FUNCTION	: 																*/
/*			1. Set rx related registers' addr to global variable			*/
/*			2. Clear RDY bit of LAPB RX data control registers				*/
/*			3. Send init rsp												*/
/*																			*/
/* CALLING	: VOID tm_rx_Init(inf_p)										*/
/*																			*/
/* PARAMETER: [I]	UINT *inf_p		Pointer of buffer						*/
/*																			*/
/*																			*/
/* RETURN	: None															*/
/*																			*/
/* ITEM No	: MB01-002-004-801				  								*/
/*																			*/
/* DATE		: 2005/11/10			FFCS)Zhangtx		新規作成			*/
/*																			*/
/* UPDATE 	: 																*/
/*			: 2006/04/03	FFCS)wangchx		WCDMA119-1258&1259			*/
/*								CPM TIMER1割込みマスクレジスタ				*/
/*								の解除処理を追加修正						*/
/* 		 	: 2006/04/21	FFCS)wang	WCDMA119-1634対応					*/
/*										Init processing						*/
/*			  2007/02/14	FFCS)zhangtx	CR07A-036	CRPI#24->12			*/
/*							change 12 links to 6 links(TRIF)				*/
/*																			*/
/*				All Rights Reserved, Copyright (C) FUJITSU LIMITED 2005-2007*/
/****************************************************************************/
/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_Init.c
 *  @brief  initialization process
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
#include "m_cm_header.h"			/* TRIF common head file	        		*/
#include "tm_rx_header.h"			/* RXS task header					*/
#include "l2_com_inc.h"			

/********************************************************************************************************************/
/*!
 *  @brief  initialization process.
 *  @note   1. Set rx related registers' addr to global variable.
 *              2. Clear RDY bit of LAPB RX data control registers
 *              3. Send init rsp
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/18 FFCS)Zouw Create 
 *  @date    2015.08.11 TDIPS)ikeda 変数削除
 */
/********************************************************************************************************************/
VOID tm_rx_Init(UINT *inf_p)
{
	CMT_TSKIF_HEAD *head_p;
	INT			ret_buf;

	rxw_needfrmsg = RXD_TRUE;
	
#if 0
	/***********************************************************/	
	/* Clear RDY bit of LAPB RX data control registers			*/
	/***********************************************************/
	/* modify for M-S3G-eNBSYS-00012*/
	m_cm_RXRegClear();
#endif	

	ret_buf = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_HEAD), (VOID **)&head_p);
    if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, CMD_NUM0, "tm_rx_Init:cm_BReq NG");
		return;
	}

	head_p->uiLength = sizeof(CMT_TSKIF_HEAD);
	head_p->uiEventNo = CMD_TSKIF_INIRSP;
	head_p->uiSrcTQueueID = CMD_TSKID_RXS;
	head_p->uiDstTQueueID = CMD_TSKID_INI;

	/* Send init response to init task			*/
	ret_buf = cm_Enter(CMD_TSKID_INI, CMD_QRB_NORMAL, CMD_TSKIF_INIRSP, head_p);
	if(ret_buf != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret_buf, "cm_Enter NG" );
	}

	/* Update task state from init to use		*/
	rxw_stateno = RXD_ST_USE ;
}

/* @} */

