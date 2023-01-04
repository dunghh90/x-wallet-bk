/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_lr_Main.c
 *  @brief   Call event-processing function, according to received event 	
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
 *  @brief     Call event-processing function, according to received event
 *  @note     1.Analyze the received the event
 		     2.Call the corresponding routine
 *  @param  src_id  	[in] source task ID
 *  @param  event_no  	[in] received event number
 *  @param  a_inf_p  	[in] point of buffer 
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/


VOID tm_lr_Main(UINT src_id, UINT event_no, UINT * a_inf_p)
{
	UINT a_ret;

	/*Analyze the received the event */	
	a_ret = tm_lr_Anlz( src_id,  event_no);
	if(a_ret == CMD_OK)
	{
		/*Call the corresponding entry*/
		lrr_mtrxtbl[lrw_tststat][lrw_eventno](a_inf_p);
	}
	else
	{
		cm_Assert(CMD_ASL_USELOW, event_no, "LNR receive abnormal event");
	}	

	return;
}

/* @} */
