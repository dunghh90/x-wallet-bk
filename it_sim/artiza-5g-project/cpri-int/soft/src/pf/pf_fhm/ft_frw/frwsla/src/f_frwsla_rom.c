/*!
 * @skip  $ld:$
 * @file  f_frwsla_rom.c
 * @brief ROM definition.
 * @date  2014/12/12 FFCS)linb Create\n
 * @date  2015/08/07 TDIPS)sasaki Update(f_frwsla_flashReadReq -> f_frwsla_nop)
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014-2015
 */

#include "f_frwsla_inc.h"
    
/*!
 * @addtogroup RRH_PF_FRWSLA
 * @{
 */
/** frwsla task function matrix table */
VOID (*const f_frwslar_matrix[D_FRWSLA_STATMAX][D_FRWSLA_EVTMAX])(VOID* pRcvMsg) = 
{
	/*Init state*/
	{
		f_frwsla_nop,						/* Flash Erase Request(from pf_fmng) */
		f_frwsla_nop,						/* Flash Write Request(from pf_fmng) */
		f_frwsla_nop						/* Flash Read Request(from pf_dia) */
	},	
	
	/* Running state*/
  	{
  		f_frwsla_flashEraseReq,				/* Flash Erase Request(from pf_fmng) */
		f_frwsla_flashWriteReq,				/* Flash Write Request(from pf_fmng) */
		f_frwsla_nop						/* Flash Read Request(from pf_dia) */
	}						
};

/* @} */

