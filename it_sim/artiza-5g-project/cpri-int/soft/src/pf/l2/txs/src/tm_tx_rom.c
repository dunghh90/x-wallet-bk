/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_tx_rom.c
 *  @brief  TXS rom tables declaration.
 *  @date   2008/07/15 FFCS)Zouw Create 
 *  @date   2013.11.22 FFCS)hongj modify for zynq.
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_TXS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRIF common head file	        		*/
#include "tm_tx_header.h"			/* TXS task header					*/


/** Main matrix table */
VOID (* const txr_mtrxtbl[TXD_MX_STATE][TXD_MX_EVENT])(UINT *inf_p) = 
{
	{  
		tm_tx_Init, 		       /* Initialization Request		*/
		tm_tx_Abort 		/* L1 Data Receiving Notice 	*/
	},
	
	{  
		tm_tx_Nop,  		       /* Initialization Request		*/
		tm_tx_Send  		/* L1 Data Receiving Notice 	*/
	}
};

/* @} */

