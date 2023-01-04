/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_lr_anlz.c
 *  @brief   Analyze external event, change it into internal event no	
 *  @date   2008/07/28  FFCS)zhengmh Create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/


/** @addtogroup RRH_MNT_LNR
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "tm_lr_header.h"			/* LNR task header					*/

/********************************************************************************************************************/
/**
 *  @brief     Analyze external event, change it into internal event no
 *  @note     Analyze external event, change it into internal event no
 *  @param  src_id  	[in] source task ID
 *  @param  event_no  	[in] received event number
 *  @return   Result code is returned
 *  @retval   CMD_OK			  0:	  OK
 *  @retval   CMD_NG			-1:	  NG
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/


UINT tm_lr_Anlz(UINT src_id, UINT event_no)
{
	UINT a_ret;

	a_ret = CMD_OK;
	switch(event_no)
	{
		case CMD_TSKIF_INIREQ:/*初期化*/
			lrw_eventno = LRD_EVT_INIREQ;
			break;
		case CMD_TSKIF_TCPSRVSTRREQ:/* TCP Server起動要求*/
			lrw_eventno = LRD_EVT_TCPSTRUP ;
			break;
		default:
			a_ret = CMD_NG;
			break;
	}

	return a_ret;
}

/* @} */

