/*!
 * @skip 		$ld:$
 * @file 		f_pfm_rom.c
 * @brief		pf_pfm?X???b?h ROM?e?[?u??
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	ROM Table
 *  @note	
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

#include	"f_pfm_inc.h"

/** @addtogroup RRH_PF_PFM
 *  @{
 */

/****************************************************************************************************************/
/*  Matrix function of PFM Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of PFM Thread
 * @note	Matrix function of PFM Thread
 * @{
 */
																		/* Matrix function of PFM Thread		*/
const	T_PFM_CALLSETRELL_MTXTBL	f_pfmr_callSetRell_mtxtbl[E_PFM_STATE_MAX][E_PFM_EVT_MAX] = 
{
	{
		{ f_pfm_InitReq			},
		{ f_pfm_abort			},
		{ f_pfm_abort			},
		{ f_pfm_abort			}
	},
	{
		{ f_pfm_abort			},		
		{ f_pfm_msi_recv_ind	},		
		{ f_pfm_timeout_ind		},		
		{ f_pfm_log_timeout		}
	}
};
/* @} */

/****************************************************************************************************************/
/*  Performance Register Address																				*/
/****************************************************************************************************************/
/*!
 * @name	Performance Register Address
 * @note	Performance Register Address
 * @{
 */
T_PFM_REG_INFO				f_pfmw_rxwin_reg_addr_sub6[D_RRH_PFM_NUM_OF_REG] = 
{
	{	D_DUS_REG_DLP_PMILGAPHA0DN ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUS_REG_DLP_PMILGTIMA0DN ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUS_REG_DLP_PMTIMTEA0DN  ,E_PFM_OBJIDX_RX_EARLY	},
	{	D_DUS_REG_DLP_PMTIMTLA0DN  ,E_PFM_OBJIDX_RX_LATE	},
	{	D_DUS_REG_DLP_PMONTIMA0DN  ,E_PFM_OBJIDX_RX_ON_TIME	},
	{	D_DUS_REG_DLP_PMSEQDUPA0DN ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUS_REG_DLP_PMSEQMISA0DN ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUS_REG_DLP_PMRCVFCA0DN  ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUS_REG_DLP_PMRCVUPA0DN  ,E_PFM_OBJIDX_NOTHING	},

	{	D_DUS_REG_DLP_PMILGAPHA0UN ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUS_REG_DLP_PMILGTIMA0UN ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUS_REG_DLP_PMTIMTEA0UN  ,E_PFM_OBJIDX_RX_EARLY	},
	{	D_DUS_REG_DLP_PMTIMTLA0UN  ,E_PFM_OBJIDX_RX_LATE	},
	{	D_DUS_REG_DLP_PMONTIMA0UN  ,E_PFM_OBJIDX_RX_ON_TIME	},
	{	D_DUS_REG_DLP_PMSEQDUPA0UN ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUS_REG_DLP_PMSEQMISA0UN ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUS_REG_DLP_PMRCVFCA0UN  ,E_PFM_OBJIDX_NOTHING	},
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */

	{	D_DUS_REG_DLP_PMILGAPHA0UR ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUS_REG_DLP_PMILGTIMA0UR ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUS_REG_DLP_PMTIMTEA0UR  ,E_PFM_OBJIDX_RX_EARLY	},
	{	D_DUS_REG_DLP_PMTIMTLA0UR  ,E_PFM_OBJIDX_RX_LATE	},
	{	D_DUS_REG_DLP_PMONTIMA0UR  ,E_PFM_OBJIDX_RX_ON_TIME	},
	{	D_DUS_REG_DLP_PMSEQDUPA0UR ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUS_REG_DLP_PMSEQMISA0UR ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUS_REG_DLP_PMRCVFCA0UR  ,E_PFM_OBJIDX_NOTHING	},
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */

	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */
	{	0                          ,E_PFM_OBJIDX_NOTHING	}	/* dummy */

};


T_PFM_REG_INFO				f_pfmw_rxwin_reg_addr_mmw[D_RRH_PFM_NUM_OF_REG] = 
{
	{	D_DUM_REG_DLP_PMILGAPHA0DN ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUM_REG_DLP_PMILGTIMA0DN ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUM_REG_DLP_PMTIMTEA0DN  ,E_PFM_OBJIDX_RX_EARLY	},
	{	D_DUM_REG_DLP_PMTIMTLA0DN  ,E_PFM_OBJIDX_RX_LATE	},
	{	D_DUM_REG_DLP_PMONTIMA0DN  ,E_PFM_OBJIDX_RX_ON_TIME	},
	{	D_DUM_REG_DLP_PMSEQDUPA0DN ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMSEQMISA0DN ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMRCVFCA0DN  ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMRCVUPA0DN  ,E_PFM_OBJIDX_NOTHING	},

	{	D_DUM_REG_DLP_PMILGAPHA0UN ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUM_REG_DLP_PMILGTIMA0UN ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUM_REG_DLP_PMTIMTEA0UN  ,E_PFM_OBJIDX_RX_EARLY	},
	{	D_DUM_REG_DLP_PMTIMTLA0UN  ,E_PFM_OBJIDX_RX_LATE	},
	{	D_DUM_REG_DLP_PMONTIMA0UN  ,E_PFM_OBJIDX_RX_ON_TIME	},
	{	D_DUM_REG_DLP_PMSEQDUPA0UN ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMSEQMISA0UN ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMRCVFCA0UN  ,E_PFM_OBJIDX_NOTHING	},
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */

	{	D_DUM_REG_DLP_PMILGAPHA0UR ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUM_REG_DLP_PMILGTIMA0UR ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUM_REG_DLP_PMTIMTEA0UR  ,E_PFM_OBJIDX_RX_EARLY	},
	{	D_DUM_REG_DLP_PMTIMTLA0UR  ,E_PFM_OBJIDX_RX_LATE	},
	{	D_DUM_REG_DLP_PMONTIMA0UR  ,E_PFM_OBJIDX_RX_ON_TIME	},
	{	D_DUM_REG_DLP_PMSEQDUPA0UR ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMSEQMISA0UR ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMRCVFCA0UR  ,E_PFM_OBJIDX_NOTHING	},
	{	0                          ,E_PFM_OBJIDX_NOTHING	},	/* dummy */

	{	D_DUM_REG_DLP_PMILGAPHA0DS ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUM_REG_DLP_PMILGTIMA0DS ,E_PFM_OBJIDX_RX_CORRUPT	},
	{	D_DUM_REG_DLP_PMTIMTEA0DS  ,E_PFM_OBJIDX_RX_EARLY	},
	{	D_DUM_REG_DLP_PMTIMTLA0DS  ,E_PFM_OBJIDX_RX_LATE	},
	{	D_DUM_REG_DLP_PMONTIMA0DS  ,E_PFM_OBJIDX_RX_ON_TIME	},
	{	D_DUM_REG_DLP_PMSEQDUPA0DS ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMSEQMISA0DS ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMRCVFCA0DS  ,E_PFM_OBJIDX_NOTHING	},
	{	D_DUM_REG_DLP_PMRCVUPA0DS  ,E_PFM_OBJIDX_NOTHING	}
	
};


UINT f_pfmw_rxwin_reg_addr_com[D_RRH_PFM_NUM_OF_REG_COM] = {
	D_DUS_REG_DLP_PMRCVPKT     ,
	D_DUS_REG_DLP_PMILGCMH     ,
	D_DUS_REG_DLP_PMIDINV      ,
	D_DUS_REG_DLP_PMSRPERR     ,
	D_DUS_REG_DLP_EXPRCVRB     ,
	D_DUS_REG_DLP_PMRBRCVLOSA0 ,
	D_DUS_REG_DLP_PMRBRCVLOSB0 ,
	D_DUS_REG_DLP_PMRBRCVLOSC0 ,
	D_DUS_REG_DLP_PMRBRCVLOSD0 ,
	D_DUS_REG_DLP_PMRBRCVLOSA1 ,
	D_DUS_REG_DLP_PMRBRCVLOSB1 ,
	D_DUS_REG_DLP_PMRBRCVLOSC1 ,
	D_DUS_REG_DLP_PMRBRCVLOSD1 ,
	D_DU_REG_ULP_PME1SNDUP
};



/* @} */

/* @} */
