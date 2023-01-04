/*!
 * @skip 		$ld:$
 * @file 		f_fhs_rom.c
 * @brief		pf_fhsスレッド ROMテーブル
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
		{ f_fhs_InitReq			},										/* 初期化要求							*/
		{ f_fhs_abort			},										/* MSIメッセージ受信通知				*/
		{ f_fhs_abort			}										/* NETCONFユーザー情報書込み応答		*/
	},
	{
		{ f_fhs_abort			},										/* 初期化要求							*/
		{ f_fhs_msi_recv_ind	},										/* MSIメッセージ受信通知				*/
		{ f_fhs_user_write_rsp	}										/* NETCONFユーザー情報書込み応答		*/
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
const	INT		f_fhsr_reg_state_mask[D_RRH_SFP_MAXNUM][3] =							/* Low Layer 障害レジスタマスク値		*/
{
	/* L1リンク断			*/		/* SFP Unmount			*/		/* SFP Power Daown		*/
	{D_DU_SVCTL_L1_DISCONNECT_1,	D_DU_SVCTL_SFP_UNMOUNT_1,		D_DU_SVCTL_SFP_POWERDOWN_1}	/* PORT#0	*/
};
/* @} */

/** @} */
