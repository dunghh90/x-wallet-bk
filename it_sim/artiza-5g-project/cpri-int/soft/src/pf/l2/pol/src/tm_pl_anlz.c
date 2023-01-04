/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_Anlz.c
 *  @brief  Analyze external event
 *  @date   2008/07/23 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_POLLING
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file           			*/
#include "tm_pl_header.h"			/* POLLING task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  Analyze external event.
 *  @note   Analyze external event, and change into internal event.
 *  @param  source [in] Source task
 *  @param  event [in] External event no
 *  @param  -       [out] -
 *  @return Analazed result
 *  @retval PLD_RET_NORMAL        0 :Normal
 *  @retval PLD_RET_ABNORMAL    1 :NG
 *  @date   2008/07/15 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
UINT tm_pl_Anlz(UINT source, UINT event)
{
	switch(event)
	{
		case CMD_TSKIF_INIREQ:		/* initialization request		*/
			plw_eventno = PLD_EV_INITREQ;
			break;
		case CMD_TSKIF_POLINTRNTC:	/* Polling Period Interrupt Notice	*/
			plw_eventno = PLD_EV_POLINTRNTC;
			break;
		default:
			return PLD_RET_ABNORMAL;
	}
	
	return PLD_RET_NORMAL;
}

/* @} */

