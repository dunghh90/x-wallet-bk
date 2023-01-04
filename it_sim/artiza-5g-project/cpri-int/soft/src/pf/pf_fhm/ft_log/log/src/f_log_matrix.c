/******************************************************************************************************************************/
/**
 * @skip		$Id:$
 * @file		f_log_matrix.c
 * @brief		Table definitions of state change matrix 
 * @note		NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 				See also StateTransitionTable(log).xls
 * @author		ALPHA)yokoyama
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 * @date		2013/10/29 ALPHA)横山 docomo ZYNQ化 & 共通化対応
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010 - 2013
 */
/******************************************************************************************************************************/

#include	"f_log_inc.h"

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

/* LOG State Mng */
const T_LOG_CALLSETRELL_MTXTBL f_logr_callSetRell_mtxtbl[D_LOG_STATENUM][E_LOG_EVENTNUM] __attribute__ ((aligned (16))) =
{
	{
		/*	init	*/
		{f_log_init						},					/**< スレッド起動						*/
		{f_log_nop						},					/**< 障害ログファイル作成要求			*/
		{f_log_nop						},					/**< 障害ログファイル作成中止			*/
		{f_log_nop						},					/**< 障害ログファイル削除				*/
		{f_log_nop						},					/**< 運用ログ出力完了通知				*/
		{f_log_nop						},					/**< 障害ログ出力要求					*/
		{f_log_nop						}					/**< ログ情報退避用タイマTO				*/
	},
	{
		/*	通常運用状態	*/
		{f_log_nop						},					/**< スレッド起動						*/
		{f_log_Idle_APITroublelogMake	},					/**< 障害ログファイル作成要求			*/
		{f_log_nop						},					/**< 障害ログファイル作成中止			*/
		{f_log_Idle_APITroublelogDel	},					/**< 障害ログファイル削除				*/
		{f_log_Idle_NtcTroublelog		},					/**< 運用ログ出力完了通知				*/
		{f_log_Idle_MakeALMTroublelog	},					/**< 障害ログ出力要求					*/
		{f_log_Idle_ToTakeShelter		}					/**< ログ情報退避用タイマTO				*/
	},
	{
		/*	運用ログ出力完了待ち	*/
		{f_log_nop						},					/**< スレッド起動						*/
		{f_log_Wait_APITroublelogMake	},					/**< 障害ログファイル作成要求			*/
		{f_log_Wait_APITroublelogStop	},					/**< 障害ログファイル作成中止			*/
		{f_log_Wait_APITroublelogDel	},					/**< 障害ログファイル削除				*/
		{f_log_Wait_NtcTroublelog		},					/**< 運用ログ出力完了通知				*/
		{f_log_Wait_MakeALMTroublelog	},					/**< 障害ログ出力要求					*/
		{f_log_Wait_ToTakeShelter		}					/**< ログ情報退避用タイマTO				*/
	}
};

/** @} */

