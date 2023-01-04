/******************************************************************************************************************************/
/**
 * @skip		$Id:$
 * @file		f_lgc_matrix.c
 * @brief		Table definitions of state change matrix 
 * @note		NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 				See also StateTransitionTable(lgc).xls
 * @author		ALPHA)yokoyama
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 * @date		2013/10/29 ALPHA)横山 docomo ZYNQ化 & 共通化対応
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010 - 2013
 */
/******************************************************************************************************************************/

#include	"f_lgc_inc.h"

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

/* LOG State Mng */
const T_LGC_CALLSETRELL_MTXTBL f_lgcr_callSetRell_mtxtbl[D_LGC_STATENUM][E_LGC_EVENTNUM] __attribute__ ((aligned (16))) =
{
	{
		/*	init	*/
		{f_lgc_init						},					/**< スレッド起動						*/
		{f_lgc_nop						},					/**< 障害ログ出力要求					*/
		{f_lgc_nop						},					/**< ログ情報退避用タイマTO				*/
		{f_lgc_nop						},					/**< MSI受信通知						*/
	},
	{
		/*	通常運用状態	*/
		{f_lgc_nop						},					/**< スレッド起動						*/
		{f_lgc_MSI_send_ind				},					/**< MSIs送信通知						*/
		{f_lgc_Idle_ToTakeShelter		},					/**< ログ情報退避用タイマTO				*/
		{f_lgc_MSI_rcv_ind				},					/**< MSI受信通知						*/
	},


};

/** @} */

