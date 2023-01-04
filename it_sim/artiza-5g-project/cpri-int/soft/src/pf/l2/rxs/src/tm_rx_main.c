/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_Main.c
 *  @brief  RXS task main function.
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
#include "m_cm_header.h"			/* TRIF common head file     			*/
#include "tm_rx_header.h"			/* RXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  RXS task main function.
 *  @note  Call event-process function, according to received event .
 *  @param  source     [in] Source task
 *  @param  event      [in] Event no
 *  @param  *inf_p       [in] Pointer of received buffer
 *  @param  -            [out] -
 *  @return Result code is returned.
 *  @retval -
 *  @date   2008/07/18 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
VOID tm_rx_Main(UINT source, UINT event, UINT *inf_p)
{	
	UINT 		ret;			/* Return value of event analyze function	*/
	UINT		ret_bfree;		/* Return value of  buffer free				*/

	ret = RXD_RET_ABNORMAL;
	ret_bfree = CMD_NUM0;
	
	/* Get internal event no			*/
	ret = tm_rx_Anlz(source, event);

	if(ret == RXD_RET_NORMAL)
	{
         (*(rxr_mtrxtbl[rxw_stateno][rxw_eventno]))(inf_p);	/*  Matrix table process  */
	}
	else
	{
		cm_Assert( CMD_ASL_DBGHIGH, event, "[tm_rx_Main] Event not found" );
	}

	if(rxw_needfrmsg && inf_p)		/* If need free msg buffer and pointer not null*/
	{
		ret_bfree = cm_BFree(inf_p);/* Free message buffer			*/
		
		if(ret_bfree != CMD_RES_OK)
		{
			cm_MAbort( CMD_ALMCD_BUFREL, 
						"tm_rx_Main", 
					   	"cm_BFree NG", 
					 	  ret_bfree, 
					 	  (UINT)inf_p, 
					 	  CMD_NUM0 );			
		}	
	}

	rxw_needfrmsg = RXD_TRUE;		/* Need to free message buffer		*/
}

/* @} */

