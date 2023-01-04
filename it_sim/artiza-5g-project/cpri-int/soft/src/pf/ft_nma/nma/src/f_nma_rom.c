/*!
 * @skip  $ld:$
 * @file  f_nma_rom.c
 * @brief ROM definition.
 * @date  2013/11/13 FFCS)linb Create\n
 * @date  2019/02/21 KCN)kamimoto  Create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

#include "f_nma_inc.h"
    
/*!
 * @addtogroup RRH_PF_NMA
 * @{
 */
/****************************************************************************/
/* nma task function matrix                                                */
/****************************************************************************/
VOID (*const f_nmar_matrix[D_NMA_STATMAX][D_NMA_EVTMAX])(VOID* pRcvMsg) = 
{
	/*Init state*/
	{
		f_nma_init,						/**< スレッド起動						*/
		f_nma_nop,						/* EventLog Write Request(Clear/Write)(from pf_log) */
		f_nma_nop,						/* Trouble Log Write Request(from pf_main) */
		f_nma_nop,						/* Trouble Log Write Request(from pf_lgc) */
		f_nma_nop,						/* Install File Write Request(form pf_flm) */
		f_nma_nop,						/* NetConf Data Write Request(from pf_fhs) */
		f_nma_nop						/* VLAN Data Write Request(from ft_dhcp) */
	},	
	
	/* Running state*/
  	{
  		f_nma_nop,						/**< スレッド起動						*/
		f_nma_flashWriteEvtLog,			/* EventLog Write Request(Clear/Write)(from pf_log) */
		f_nma_troubleLogWriteReq,		/* Trouble Log Write Request(from pf_main) */
		f_nma_troubleLogWriteReq,		/* Trouble Log Write Request(from pf_lgc) */
		f_nma_InstallFilWriteReq,		/* Install File Write Request(form pf_flm) */
		f_nma_NetConfDataWriteReq,		/* NetConf Data Write Request(from pf_fhs) */
		f_nma_VLANDataWriteReq			/* VLAN Data Write Request(from ft_dhcp) */
	}						
};

/* @} */

