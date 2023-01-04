/*!
 *  @skip   $Id$
 *  @file   l2_dbg_main.c
 *  @brief  Dbg thread main function
 *  @date   2013/12/03 FFCS)hongj modify for zynq\n
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
*/ 

 /*!
 * @addtogroup RRH_L2_DBG
 * @{
 */
#include "m_cm_header.h"
#include "l2_com_inc.h"
#include "l2_dbg_inc.h"

/********************************************************************************************************************/
/*!
 *  @brief  L2 dbg process
 *  @note   Call analyse function to analyse event code, then do proper processing.
 *  @param  a_inf_p     [in] pointer of received message
 *  
 *  
 *  @return None
 *  @date   2013/12/03 FFCS)hongj modify for zynq\n
 */
/********************************************************************************************************************/
VOID tm_dbg_Main(CHAR * a_inf_p)
{
	INT 				ret;
	UINT				a_event;

	a_event = ((CMT_TSKIF_HEAD*)a_inf_p )->uiEventNo;


    /* タスク起動ログ書込み処理 	 */
	/* analayse command*/
	ret = l2_dbg_Anlz(a_event);
	if(ret == CMD_OK)
	{
		/* Matrix table process    */
		(*(l2_dbgr_mtrxtbl[l2_dbgw_eventno]))((UINT *)a_inf_p);
	}
	else
	{
		cm_Assert( CMD_ASL_DBGLOW, a_event, "[tm_dbg_Main] Event not found" );
	}

	return;
}

/* @} */
