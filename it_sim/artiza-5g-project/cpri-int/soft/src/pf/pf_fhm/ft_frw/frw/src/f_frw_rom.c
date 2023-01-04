/*!
 * @skip  $ld:$
 * @file  f_frw_rom.c
 * @brief ROM definition.
 * @date  2013/11/13 FFCS)linb Create
 * @date  2015/08/07 TDIPS)sasaki Update(f_frw_flashReadReq function -> f_frw_nop)
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_frw_inc.h"
    
/*!
 * @addtogroup RRH_PF_FRW
 * @{
 */
/** frw task function matrix table */
VOID (*const f_frwr_matrix[D_FRW_STATMAX][D_FRW_EVTMAX])(VOID* pRcvMsg) = 
{
	/*Init state*/
	{
		f_frw_nop,						/* Flash Erase Request(from pf_fmng) */
		f_frw_nop,						/* Flash Write Request(from pf_fmng) */
		f_frw_nop,						/* Flash Read Request(from pf_dia) */
		f_frw_nop,						/* EventLog Write Request(Clear/Write)(from pf_log) */
		f_frw_nop,						/* Trouble Log Write Request(from pf_main) */
		f_frw_nop						/* FW Info Save Request(from pf_fmmng) */
	},
	/* Running state*/
  	{
  		f_frw_flashEraseReq,			/* Flash Erase Request(from pf_fmng) */
		f_frw_flashWriteReq,			/* Flash Write Request(from pf_fmng) */
		f_frw_nop,						/* Flash Read Request(from pf_dia) */
		f_frw_flashWriteEvtLog,			/* EventLog Write Request(Clear/Write)(from pf_log) */
		f_frw_troubleLogWriteReq,		/* Trouble Log Write Request(from pf_main) */
		f_frw_flashFwInfoSaveReq		/* FW Info Save Request(from pf_fmmng) */
	}
};

/* @} */

