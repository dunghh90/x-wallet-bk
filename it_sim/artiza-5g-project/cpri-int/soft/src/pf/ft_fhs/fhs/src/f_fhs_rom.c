/*!
 * @skip 		$ld:$
 * @file 		f_fhs_rom.c
 * @brief		pf_fhs�X���b�h ROM�e�[�u��
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

#include	"f_fhs_inc.h"

/** @addtogroup RRH_PF_FHS
 *  @{
 */

/****************************************************************************************************************/
/*  Matrix function of FHS Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of FHS Thread
 * @note	Matrix function of FHS Thread
 * @{
 */
																		/* Matrix function of FHS Thread		*/
const	T_FHS_CALLSETRELL_MTXTBL	f_fhsr_callSetRell_mtxtbl[D_FHS_STATENUM][D_FHS_EVENTNUM] = 
{
	{
		{ f_fhs_InitReq			},										/* �������v��							*/
		{ f_fhs_abort			},										/* MSI���b�Z�[�W��M�ʒm				*/
		{ f_fhs_abort			}										/* NETCONF���[�U�[��񏑍��݉���		*/
	},
	{
		{ f_fhs_abort			},										/* �������v��							*/
		{ f_fhs_msi_recv_ind	},										/* MSI���b�Z�[�W��M�ʒm				*/
		{ f_fhs_user_write_rsp	}										/* NETCONF���[�U�[��񏑍��݉���		*/
	}
};
/* @} */

/****************************************************************************************************************/
/*  Judgement of Operational Status Table																		*/
/****************************************************************************************************************/
/*!
 * @name	Judgement of Operational Status Table
 * @note	Judgement of Operational Status Table
 * @{
 */
const	INT		f_fhsr_reg_state_mask[D_RRH_SFP_MAXNUM][3] =							/* Low Layer ��Q���W�X�^�}�X�N�l		*/
{
	/* L1�����N�f			*/		/* SFP Unmount			*/		/* SFP Power Daown		*/
	{D_DU_SVCTL_L1_DISCONNECT_1,	D_DU_SVCTL_SFP_UNMOUNT_1,		D_DU_SVCTL_SFP_POWERDOWN_1}	/* PORT#0	*/
};
/* @} */

/** @} */
