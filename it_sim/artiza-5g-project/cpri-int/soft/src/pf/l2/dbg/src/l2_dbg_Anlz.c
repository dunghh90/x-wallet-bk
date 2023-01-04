/*!
 * @skip    $Id$
 * @file    l2_dbg_Anlz.c
 * @brief   the entry of L2 dbg if
 * @date    2013.12.03 FFCS)hongj create for zynq.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013
*/


/********************************************************************************************************/
#include "m_cm_header.h"
#include "l2_com_inc.h"
#include "l2_dbg_inc.h"
#include "rrhApi_L2.h"
#include "rrhApi_Dbg.h"


 /*!
 * @addtogroup RRH_L2_DBG
 * @{
 */


/*!
 *  @brief   L2 Dbg IF変換処理関数(l2_dbg_Anlz)
 *  @note    Transform task IF to internal event number
 *  @param   a_event		[in]  a_event
 *  @return  inter_event
 *  @date    2013.12.03 FFCS)hongj create for zynq.
 */
UINT l2_dbg_Anlz(UINT a_event)
{
	UINT	ret;                                            /*!< */
	
	ret = CMD_OK;

    /* API NO. to l2 main event */
	switch(a_event)
	{
		/* CPRI信号受信予約	*/
		case D_API_MSGID_L2_CPRIMSG_SUB_REQ:
		    l2_dbgw_eventno = L2_DBG_EVTNO_CPRIMSG_SUB_REQ;
			break;
		/* CPRI信号受信予約	*/
		case D_API_MSGID_L2_CPRIMSG_UNSUB_IND:
		    l2_dbgw_eventno = L2_DBG_EVTNO_CPRIMSG_UNSUB_IND;
			break;
		/* LAPBリンク状態予約	*/
		case D_API_MSGID_L2_LINKSTATE_SUB_REQ:
		    l2_dbgw_eventno = L2_DBG_EVTNO_LAPBSTATE_SUB_REQ;
			break;
		/* LAPBリンク状態予約中止	*/
		case D_API_MSGID_L2_LINKSTATE_UNSUB_IND:
		    l2_dbgw_eventno = L2_DBG_EVTNO_LAPBSTATE_UNSUB_IND;
			break;
		/* CPRISIMU開始要求	*/
		case D_API_MSGID_DBG_CPRISIMU_START_REQ:
			l2_dbgw_eventno = L2_DBG_EVTNO_CPRISIMU_START_REQ;
			break;
		/* CPRISIMU停止通知	*/
		case D_API_MSGID_DBG_CPRISIMU_STOP_REQ:
			l2_dbgw_eventno = L2_DBG_EVTNO_CPRISIMU_STOP_REQ;
			break;
		/* CPRIMonitor開始通知	*/
		case D_API_MSGID_DBG_CPRIMON_START_IND:
			l2_dbgw_eventno = L2_DBG_EVTNO_CPRIMON_START_IND;
			break;
		/* CPRIMonitor停止通知	*/
		case D_API_MSGID_DBG_CPRIMON_STOP_IND:
			l2_dbgw_eventno = L2_DBG_EVTNO_CPRIMON_STOP_IND;
			break;
	    /* CPRI Message統計通知	*/
		case D_API_MSGID_L2_CPRIMSG_STASTIC_IND:
			l2_dbgw_eventno = L2_DBG_EVTNO_CPRIMSG_STATISTICS_IND;
			break;
		/*Others						*/
		default:
			ret = CMD_NG;
	}
	return(ret);
}

/* @} */

