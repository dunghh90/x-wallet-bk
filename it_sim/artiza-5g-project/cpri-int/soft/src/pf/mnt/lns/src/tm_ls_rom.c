/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     tm_ls_rom.c
 *  @brief   declare ROM table in  LAN Message Sending Task
 *  @date   2008/07/28  FFCS)zhengmh Create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_LNS
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"			/* TRA common header file			       */
#include "tm_ls_header.h"			/* LNS task header					*/

/********************************************************************************************************************/
/**
 *  @brief     declare ROM table in  LAN Message Sending Task
 *  @note     declare ROM table in  LAN Message Sending Task
 *  @param  None
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28  FFCS)zhengmh Create
 */
/********************************************************************************************************************/


void	( *lsr_mtrxtbl[ LSD_MAX_STATE ][ LSD_MAX_EVENT ] )
											( UINT *a_inf_p ) =
{
	{tm_ls_Init,tm_ls_Abort,tm_ls_Abort},		/*Init state*/
  	{tm_ls_Nop,tm_ls_TcpSnd,tm_ls_TcpRel}		/* Running state*/
};

/* @} */
