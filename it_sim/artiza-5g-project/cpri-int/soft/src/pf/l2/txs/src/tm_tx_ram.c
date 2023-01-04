/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_tx_ram.c
 *  @brief  TXS RAM tables declaration.
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

#include "m_cm_header.h"			/* TRIF common head file	       		*/
#include "tm_tx_header.h"			/* TXS task header					*/


UINT txw_stateno ;					/* State number		*/

UINT txw_eventno ;						/* Event number		*/

USHORT txw_curbankidx[D_RRH_CPRINO_NUM] ;	/* Current to-be-selected bank index*/

USHORT txw_needfrmsg ;				/* Need/needn't free message buffer	*/

/* @} */

