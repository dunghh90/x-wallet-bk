/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_Main.c
 *  @brief  Polling task main function.
 *  @date   2008/07/25 FFCS)Zouw Create 
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
 *  @brief  Polling task main function.
 *  @note  Call event-process function, according to received event .
 *  @param  source     [in] Source task
 *  @param  event      [in] Event no
 *  @param  *inf_p       [in] Pointer of received buffer
 *  @param  -            [out] -
 *  @return Result code is returned.
 *  @retval -
 *  @date   2008/07/25 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
VOID tm_pl_Main(UINT source, UINT event, UINT *inf_p)
{	
	UINT 		ret;			/* Return value of event analyze function	*/
	
	ret = PLD_RET_ABNORMAL;
	/* Get internal event no			*/
	ret = tm_pl_Anlz(source, event);

	if(ret == PLD_RET_NORMAL)
	{
         (*(plr_mtrxtbl[plw_stateno][plw_eventno]))(inf_p);	/*  Matrix table process  */
	}
	else
	{	
		cm_Assert( CMD_ASL_DBGHIGH, event, "[tm_pl_Main] Event not found" );
	}

	return;
}
/* @} */

