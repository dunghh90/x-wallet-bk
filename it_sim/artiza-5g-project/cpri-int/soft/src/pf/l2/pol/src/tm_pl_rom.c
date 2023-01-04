/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_rom.c
 *  @brief  Polling rom tables declaration
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


/** Main matrix table */
VOID (* const plr_mtrxtbl[PLD_MX_STATE][PLD_MX_EVENT])(UINT *inf_p) = 
{
	{  
		tm_pl_Init,  			       /* initialization request				*/
		tm_pl_Abort				/* Polling Period Interrupt Notice   	*/
	},
	
	{  
		tm_pl_Nop,				/* initialization request				*/
		tm_pl_Loop   			/* Polling Period Interrupt Notice   	*/
	}
};
/* @} */

