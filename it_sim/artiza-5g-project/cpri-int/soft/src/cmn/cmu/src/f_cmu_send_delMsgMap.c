/*!
 * @skip		$ld:$
 * @file		f_cmu_send_delMsgMap.c
 * @brief		cmu メッセージ送信先情報管理テーブル削除処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#include "f_cmn_cmu_inc.h"

/*!
 * @brief		f_send_delMsgMap
 * @note		cmu メッセージ送信先情報管理テーブルから要素削除.
 *					-# テーブルの登録数が0か判定.
 *					- 0の場合.
 *						-# 削除出来ないため処理終了.
 *					- 上記以外の場合.
 *						-# 処理継続.
 *					-# Mutex Lock.
 *					- Mutex Lock失敗の場合.
 *						-# アサートログを出力し、異常終了.
 *					- 上記以外場合.
 *						-# 処理継続.
 *					-# 削除するEventID＆送信先プロセスQIDがテーブルに登録済みか判定.
 *					- 登録ありの場合.
 *						-# EventIDに0を設定.
 *						-# 送信先プロセスQIDに0を設定.
 *						-# アサートログを出力.
 *						-# 削除済みフラグをONに設定.
 *						-# メッセージ送信先情報管理テーブルのMAX情報を0に設定.
 *					-# 削除済みフラグ判定.
 *					-削除済みの場合.
 *						-# 正常終了.
 *					-削除未の場合.
 *						-# アサートログを出力.
 *						-# 異常終了.
 * @param		uiEventID		[in]	EventID.
 * @param		uiSignalKind	[in]	SignalKind.
 * @param		uiQueID			[in]	送信先スレッドQID.
 * @retval		D_RRH_OK	0:正常終了
 * @retval		D_RRH_NG	1:異常終了
 * @return		処理結果
 * @warning		CPRI信号受信通知(0x0301)の場合、INパラのuiEventIDにはSignalKindの値が入る.
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
UINT f_send_delMsgMap(UINT uiEventID, UINT uiSignalKind, UINT uiQueID)
{
	UINT	uiRet;
	
	uiRet = f_cmu_com_delSearch(uiEventID, uiSignalKind, uiQueID, &f_sendw_route);

	if(uiRet != D_RRH_OK)
	{
		if(uiRet == D_CMU_NG_MUTEX_LOCK)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,(const CHAR *)"pthread_mutex_lock NG.  EventID=0x%08lx, SigKind=0x%08lx, QueID=0x%08lx.",uiEventID, uiSignalKind, uiQueID);
		}
		else if(uiRet == D_CMU_NG_MUTEX_UNLOCK)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,(const CHAR *)"pthread_mutex_unlock NG.  EventID=0x%08lx, SigKind=0x%08lx, QueID=0x%08lx.",uiEventID, uiSignalKind, uiQueID);
		}
		else if(uiRet == D_CMU_NG_TABLE_EMPTY)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,(const CHAR *)"Can't delete. Table is empty.  EventID=0x%08lx, SigKind=0x%08lx, QueID=0x%08lx.",uiEventID, uiSignalKind, uiQueID);
		}
		else
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,(const CHAR *)"Not exist ID on Table.  EventID=0x%08lx, SigKind=0x%08lx, QueID=0x%08lx.",uiEventID, uiSignalKind, uiQueID);
		}
		
		uiRet = D_RRH_NG;
	}
	
	return uiRet;
}

/* @} */
