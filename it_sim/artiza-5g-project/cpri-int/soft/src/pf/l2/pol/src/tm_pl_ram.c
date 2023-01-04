/********************************************************************************************************************/
/*!
 *  @skip   $Id:$
 *  @file   tm_pl_ram.c
 *  @brief  Polling RAM tables declaration.
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


UINT plw_stateno  ;	/* State number			*/

UINT plw_eventno ;			/* Event number			*/

USHORT plw_linknum ;	/* CPRI link amount		*/

VOID*	plw_assert_p;

/* @} */

