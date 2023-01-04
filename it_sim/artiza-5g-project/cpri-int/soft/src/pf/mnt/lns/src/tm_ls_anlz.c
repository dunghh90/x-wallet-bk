/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_ls_Anlz.c
 *  @brief   Analyze external event, change it into internal event no	
 *  @date   2008/07/28  FFCS)zhengmh Create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_LNS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "tm_ls_header.h"			/* LNS task header					*/

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

UINT tm_ls_Anlz(UINT src_id, UINT event_no)
{
	UINT 	a_ret;

	a_ret = CMD_OK;
	switch(event_no)
	{
		case CMD_TSKIF_INIREQ:				/*初期化*/
			lsw_eventno = LSD_EVT_INIREQ;
			break;
		case CMD_TSKIF_TCPSNDREQ:		/* TCP データ送信通知*/
			lsw_eventno = LSD_EVT_SNDREQ;
			break;
		case CMD_TSKIF_TCPRELNTC:		
			lsw_eventno  = LSD_EVT_RELREQ;/* TCP解放通知	*/
			break;
		default:
			a_ret = CMD_NG;
			break;
	}

	return a_ret;
}
/* @} */
