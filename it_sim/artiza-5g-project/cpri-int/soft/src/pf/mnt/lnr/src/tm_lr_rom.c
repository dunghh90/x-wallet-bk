/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_lr_rom.c
 *  @brief   declare ROM table in  LNR task
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
 *  @brief     declare ROM table in  LNR task
 *  @note     declare ROM table in  LNR task
 *  @param  None
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/


void	( *lrr_mtrxtbl[ LRD_MAX_STATE ][ LRD_MAX_EVENT ] )
											( UINT *a_inf_p ) =
{
	{tm_lr_Init,tm_lr_Abort},	/*Init state*/
  	{tm_lr_Nop,tm_lr_TcpInit}	/* Running state*/
};

/* @} */

