/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_Anlz.c
 *  @brief  Analyze external event
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

#include "m_cm_header.h"			/* TRIF common head file	       		*/
#include "tm_rx_header.h"			/* RXS task header					*/
#include "l2_com_inc.h"			

/********************************************************************************************************************/
/*!
 *  @brief  Analyze external event.
 *  @note   Analyze external event, and change into internal event.
 *  @param  srctsk [in] Source task
 *  @param  event [in] External event no
 *  @param  -       [out] -
 *  @return Analazed result
 *  @retval RXD_RET_NORMAL        0 :Normal
 *  @retval RXD_RET_ABNORMAL    1 :NG
 *  @date   2008/07/18 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
UINT tm_rx_Anlz(UINT srctsk, UINT event)
{
	if((srctsk == CMD_TSKID_POL) && (event == CMD_TSKIF_L1DATRCVNTC))
	{
		rxw_eventno = RXD_EV_RCVNTC_POL;/* L1 Data Receiving Notice (from POLLING task)	*/
		return RXD_RET_NORMAL;
	}

	switch(event)
	{
		case CMD_TSKIF_INIREQ:			/* Initialization Request		*/
			rxw_eventno = RXD_EV_INIREQ;
			break;
		/* L1 Data Receiving Notice (from make MT management)		*/
		case CMD_TSKIF_L1DATRCVNTC:
			rxw_eventno = RXD_EV_RCVNTC_MKM;	
			break;
		default:
			return RXD_RET_ABNORMAL;
	}

	return RXD_RET_NORMAL;
}

/* @} */

