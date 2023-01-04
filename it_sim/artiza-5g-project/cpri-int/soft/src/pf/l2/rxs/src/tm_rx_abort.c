/****************************************************************************/
/* NAME		: アボート処理													*/
/*																			*/
/* FUNCTION	: Abort															*/
/*																			*/
/* CALLING	: VOID tm_rx_Abort(inf_p)										*/
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
/*																			*/
/*				All Rights Reserved, Copyright (C) FUJITSU LIMITED 2005		*/
/****************************************************************************/
/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_Abort.c
 *  @brief  Abnormal process
 *  @date   2008/07/15 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/


/*! @addtogroup RRH_L2_RXS
 * @{ */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file    			*/
#include "tm_rx_header.h"			/* RXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  Abnormal process.
 *  @note   Output abnormal log.
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2008/07/15 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
VOID tm_rx_Abort(UINT *inf_p)
{
	cm_MAbort(CMD_ALMCD_EVENT, 
	"tm_rx_Abort", 
	"event-state abnormal", 
	rxw_eventno, 
	rxw_stateno, 
	CMD_NUM0);	
}

/* @} */
