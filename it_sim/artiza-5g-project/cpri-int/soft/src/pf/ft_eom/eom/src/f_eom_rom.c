/*!
 * @skip 		$ld:$
 * @file 		f_eom_rom.c
 * @brief		pf_eom�X���b�h ROM�e�[�u��
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

#include	"f_eom_inc.h"

/** @addtogroup RRH_PF_EOM
 *  @{
 */

/****************************************************************************************************************/
/*  Matrix function of EOM Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of EOM Thread
 * @note	Matrix function of EOM Thread
 * @{
 */
																		/* Matrix function of EOM Thread		*/
const	T_EOM_CALLSETRELL_MTXTBL	f_eomr_callSetRell_mtxtbl[E_EOM_STATE_MAX][E_EOM_EVT_MAX] = 
{
	{
		{ f_eom_InitReq			},										/* �������v��							*/
		{ f_eom_abort			}										/* MSI���b�Z�[�W��M�ʒm				*/
	},
	{
		{ f_eom_abort			},										/* �������v��							*/
		{ f_eom_msi_recv_ind	}										/* MSI���b�Z�[�W��M�ʒm				*/
	}
};
/* @} */

/** @} */
