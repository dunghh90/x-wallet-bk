/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_tx_Main.c
 *  @brief  TXS task main function.
 *  @date   2008/07/15 FFCS)Zouw Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_TXS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRIF common head file		       	*/
#include "tm_tx_header.h"			/* TXS task header					*/

/********************************************************************************************************************/
/*!
 *  @brief  TXS task main function.
 *  @note  Call event-process function, according to received event .
 *  @param  source     [in] Source task
 *  @param  event      [in] Event no
 *  @param  *inf_p       [in] Pointer of received buffer
 *  @param  -            [out] -
 *  @return Result code is returned.
 *  @retval -
 *  @date   2008/07/15 FFCS)Zouw Create 
 */
/********************************************************************************************************************/
VOID tm_tx_Main(UINT source, UINT event, UINT *inf_p)
{
	UINT 		ret;		/* Return value of event analyze function	*/
	UINT		ret_bfree;	/* Return value of  buffer free				*/

	ret = TXD_RET_ABNORMAL;
	ret_bfree = CMD_NUM0;
	
	/* Get internal event no			*/
	ret = tm_tx_Anlz(source, event);

	if(ret == TXD_RET_NORMAL)
	{
         	(*(txr_mtrxtbl[txw_stateno][txw_eventno]))(inf_p);	/* Matrix table process    */
	}
	else
	{
		cm_Assert( CMD_ASL_DBGHIGH, event, "[tm_tx_Main] Event not found" );
	}

	if(txw_needfrmsg && inf_p)		/* If need free msg buffer and pointer not null */
	{
		ret_bfree = cm_BFree(inf_p);/* Free message buffer			*/

		if(ret_bfree != CMD_RES_OK)
		{
			cm_MAbort( CMD_ALMCD_BUFREL, 
						"tm_tx_Main", 
					   	"cm_BFree NG", 
					 	  ret_bfree, 
					 	  (UINT)inf_p, 
					 	  CMD_NUM0);			
		}
	}
	
	txw_needfrmsg = TXD_TRUE;		/* Need to free message buffer		*/

	return;
}

/* @} */

