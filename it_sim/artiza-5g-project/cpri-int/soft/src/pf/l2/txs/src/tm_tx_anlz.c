/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_tx_Anlz.c
 *  @brief  Analyze external event
 *  @date   2008/07/15 FFCS)Zouw Create 
 *  @date   2013.11.18 FFCS)hongj modify for zynq.
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_TXS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRIF common head file	       		*/
#include "tm_tx_header.h"			/* TXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  Analyze external event.
 *  @note   Analyze external event, and change into internal event.
 *  @param  srctsk [in] Source task
 *  @param  event [in] External event no
 *  @param  -       [out] -
 *  @return Analazed result
 *  @retval TXD_RET_NORMAL        0 :Normal
 *  @retval TXD_RET_ABNORMAL    1 :NG
 *  @date   2008/07/15 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
UINT tm_tx_Anlz(UINT srctsk, UINT event)
{
	switch(event)
	{
		case CMD_TSKIF_INIREQ:				/* initialization request         */
			txw_eventno = TXD_EV_INIREQ;
			break;
		case CMD_TSKIF_L1DATTXREQ:			/* L1 Data Sending Notice	*/
			txw_eventno = TXD_EV_L1DATTXREQ;
			break;
		default:
			return TXD_RET_ABNORMAL;
	}
	return TXD_RET_NORMAL;
}

/* @} */
