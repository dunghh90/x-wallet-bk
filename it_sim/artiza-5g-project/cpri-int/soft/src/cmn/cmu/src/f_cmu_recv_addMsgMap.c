/*!
 * @skip		$ld:$
 * @file		f_cmu_recv_addMsgMap.c
 * @brief		cmu_ プロセス間メッセージ受信処理
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
 * @brief		f_recv_addMsgMap
 * @note		cmu メッセージ送信先情報管理テーブルに要素追加.
 *					-# テーブルがMAXか判定.
 *					- MAXの場合.
 *						-# 追加不可のため異常終了.
 *					-# Mutex Lock.
 *					- Mutex Lock失敗の場合.
 *						-# アサートログを出力し、異常終了.
 *					-# 追加するEventID＆送信先スレッドQIDがテーブルに登録済みか判定.
 *					- 既に登録済みの場合.
 *						-# アサートログを出力.
 *						-# 異常終了.
 *					-# テーブルに空きがあるか判定.
 *					- 空きがある場合.
 *						-# テーブルのEventIDに引数のEventIDを設定.
 *						-# テーブルの送信先スレッドQIDに引数の送信先スレッドQIDを設定.
 *						-# アサートログを出力.
 *					-# Mutex Unlock.
 *					- Mutex Unlock失敗の場合.
 *						-# アサートログを出力し、異常終了.
 *					-# 正常終了.
 * @param		uiEventID		[in]	EventID.
 * @param		uiSignalKind	[in]	SignalKind.
 * @param		uiQueID			[in]	送信先スレッドQID.
 * @retval		D_RRH_OK	0:正常終了
 * @retval		D_RRH_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
UINT f_recv_addMsgMap(UINT uiEventID, UINT uiSignalKind, UINT uiQueID)
{
	UINT	uiRet;
	
	uiRet = f_cmu_com_addSearch(uiEventID, uiSignalKind, uiQueID, &f_recvw_route);

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
		else if(uiRet == D_CMU_NG_TABLE_FULL)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,(const CHAR *)"Can't add. Table is full.  EventID=0x%08lx, SigKind=0x%08lx, QueID=0x%08lx.",uiEventID, uiSignalKind, uiQueID);
		}
		else if(uiRet == D_CMU_NG_EXIST)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,(const CHAR *)"Can't add. Find same EventID and ThreadQID.  EventID=0x%08lx, SigKind=0x%08lx, QueID=0x%08lx.",uiEventID, uiSignalKind, uiQueID);
		}
		else
		{
			/* ありえないルート */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,(const CHAR *)"NG route.  EventID=0x%08lx, SigKind=0x%08lx, QueID=0x%08lx.",uiEventID, uiSignalKind, uiQueID);
		}
		
		uiRet = D_RRH_NG;
	}
	
	return uiRet;
}

/* @} */
