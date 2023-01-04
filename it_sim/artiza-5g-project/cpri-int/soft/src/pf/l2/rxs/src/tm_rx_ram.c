/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_rx_ram.c
 *  @brief  RXS RAM tables declaration.
 *  @date   2008/07/22 FFCS)Zouw Create
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
#include "m_cm_header.h"			/* TRIF common head file	       		*/
#include "tm_rx_header.h"			/* RXS task header					*/


UINT rxw_stateno ;		/* Task state no					*/

UINT rxw_eventno  ;			/* Event no							*/

USHORT rxw_needfrmsg ;	/* Need/needn't free message buffer	*/



/* @} */
