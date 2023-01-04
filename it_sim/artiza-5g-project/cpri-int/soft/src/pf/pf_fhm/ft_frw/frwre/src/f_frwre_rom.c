/*!
 * @skip  $ld:$
 * @file  f_frwre_rom.c
 * @brief ROM definition.
 * @date  2015/08/04 TDIPS)sasaki Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

#include "f_frwre_inc.h"
    
/*!
 * @addtogroup RRH_PF_FRWRE
 * @{
 */
/** frwre task function matrix table */
VOID (*const f_frwrer_matrix[D_FRWRE_STATMAX][D_FRWRE_EVTMAX])(VOID* pRcvMsg) = 
{
	/* Init state		*/
	{
		f_frwre_nop,				/* Flash Erase Request(from pf_fmng)	*/
		f_frwre_nop,				/* Flash Write Request(from pf_fmng)	*/
		f_frwre_nop					/* Flash Read Request(from pf_fmng)		*/
	},	
	
	/* Running state	*/
  	{
  		f_frwre_flashEraseReq,		/* Flash Erase Request(from pf_fmng)	*/
		f_frwre_flashWriteReq,		/* Flash Write Request(from pf_fmng)	*/
		f_frwre_flashReadReq		/* Flash Read Request(from pf_dia)		*/
	}						
};

/* @} */

