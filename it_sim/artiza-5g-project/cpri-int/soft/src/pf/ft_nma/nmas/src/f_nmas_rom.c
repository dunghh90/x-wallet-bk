/*!
 * @skip  $ld:$
 * @file  f_nmas_rom.c
 * @brief ROM definition.
 * @date  2019/02/21 KCN)kamimoto  Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

#include "f_nmas_inc.h"
    
/*!
 * @addtogroup RRH_PF_NMAS
 * @{
 */
/****************************************************************************/
/* nma task function matrix                                                */
/****************************************************************************/
VOID (*const f_nmasr_matrix[D_NMAS_STATMAX][D_NMAS_EVTMAX])(VOID* pRcvMsg) = 
{
	/*Init state*/
	{
		f_nmas_init,						/**< スレッド起動						*/
		f_nmas_nop,							/* Install File Write Request(form pf_flm) */
		f_nmas_nop							/* LUT LOG Write Request(form pf_cca) */
	},	
	
	/* Running state*/
  	{
  		f_nmas_nop,						/**< スレッド起動						*/
		f_nmas_InstallFilWriteReq,		/* Install File Write Request(form pf_flm) */
		f_nmas_LutLogWriteNtf			/* LUT LOG Write Request(form pf_cca) */
	}
};

/* @} */

