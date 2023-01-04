/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_lr_nop.c
 *  @brief   no operation
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
 *  @brief     no operation
 *  @note     Free the received message if it is not NULL
 *  @param  a_inf_p  	[in] point of buffer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/

VOID tm_lr_Nop(UINT * a_inf_p)
{
	/*Free the received message if it is not NULL*/
	UINT a_ret;

	cm_Assert( CMD_ASL_DBGLOW, (UINT)a_inf_p, "tm_lr_Nop is called" );

	if(a_inf_p)
	{
		a_ret = cm_BFree((void*)a_inf_p);		
		if(a_ret != CMD_RES_OK)
		{
			cm_MAbort(CMD_ALMCD_BUFREL,"tm_lr_Nop", 
						"cm_BFree:NG",
						a_ret,(UINT)a_inf_p, CMD_NUM0);
		}
	}
	
}
/* @} */

