/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_nego_rom.c
 *  @brief  PF_nego thread ROM table define
 *  @date 2015/04/22 FFCS)fuxg create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_NEGO
* @{ */

#include "f_sys_inc.h"
#include "f_nego_inc.h"

/** main matrix table */
void    (* const f_nego_matrix[D_NEGO_STATMAX][D_NEGO_EVTMAX]) (UINT *buff_adr) = 
{
	/* 0: init state */
	{  
	  f_nego_nop,						/* auto nego start indication */
	  f_nego_nop,						/* auto nego stop indication */
  	  f_nego_nop,   					/* auto nego timeout notification */
	  f_nego_nop						/* auto nego restart indication */
	},

	/* 1: idle state */
	{	
	  f_nego_AutoNegoStartInd,			/* auto nego start indication */
	  f_nego_nop,						/* auto nego stop indication */
	  f_nego_nop,						/* auto nego timeout notification */
	  f_nego_AutoNegoReStartInd			/* auto nego restart indication */
	},

	/* 2: run state */
	{	
	  f_nego_nop,						/* auto nego start indication */
	  f_nego_AutoNegoStopInd,			/* auto nego stop indication */
	  f_nego_AutoNegoTimeOutNtc,		/* auto nego timeout notification */
	  f_nego_nop						/* auto nego restart indication */
	}
};
/* @} */

