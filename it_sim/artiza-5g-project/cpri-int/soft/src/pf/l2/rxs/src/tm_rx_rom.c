/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_rom.c
 *  @brief  RXS rom tables declaration.
 *  @date   2008/07/18 FFCS)Zouw Create 
 *  @date   2013.11.22 FFCS)hongj modify for zynq.
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/*! @addtogroup RRH_L2_RXS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"			/* TRIF common head file		       	*/
#include "tm_rx_header.h"			/* RXS task header					*/


/** Main matrix table */
VOID (* const rxr_mtrxtbl[RXD_MX_STATE][RXD_MX_EVENT])(UINT *inf_p) = 
{
	{  
		tm_rx_Init,				/* Initialization Request 	               			*/
		tm_rx_Abort, 			/* L1 Data Receiving Notice (from POLLING task)	*/
		tm_rx_Abort   			/* L1 Data Receiving Notice (from make MT management)	*/
	},
	
	{  
		tm_rx_Nop,				/* Initialization Request 	               			*/
		tm_rx_Recv,  			/* L1 Data Receiving Notice (from POLLING task)	*/
		tm_rx_Simrecv 			/* L1 Data Receiving Notice (from make MT management)	*/
	}
};

/* @} */

