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
 * @brief		f_cmu_com_delSearch
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
 * @param		*tRouteTbl_p	[in]	メッセージ送信先管理情報テーブル.
 * @retval		D_CMU_OK				0:正常終了
 * @retval		D_CMU_NG				1:異常終了
 * @retval		D_CMU_NG_MUTEX_LOCK		2:Mutexロック失敗
 * @retval		D_CMU_NG_MUTEX_UNLOCK	3:Mutexアンロック失敗
 * @retval		D_CMU_NG_TABLE_EMPTY	5:テーブルが空
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
UINT f_cmu_com_delSearch(UINT uiEventID , UINT uiSignalKind, UINT uiQueID, T_CMU_ROUTE_MNG* tRouteTbl_p)
{
	UINT					loop		= 0;
	UINT					uiRet		= D_CMU_NG;
	UINT					uiRetMutex	= D_CMU_NG;
	
	/****************************************************************************/
	/* Mutex Lock																*/
	/****************************************************************************/
	uiRetMutex = pthread_mutex_lock(&(tRouteTbl_p->mutex));
	if( uiRetMutex != D_CMU_OK ){
		return D_CMU_NG_MUTEX_LOCK;
	}
	
	/************************************************************/
	/* テーブルが0の場合、削除出来ないため処理終了				*/
	/************************************************************/
	if(tRouteTbl_p->infoNum == 0)
	{
		/****************************************************************************/
		/* Mutex Unlock																*/
		/****************************************************************************/
		pthread_mutex_unlock(&(tRouteTbl_p->mutex));
		return D_CMU_NG_TABLE_EMPTY;
	}
	
	/************************************************************/
	/* テーブルに登録情報があれば削除							*/
	/************************************************************/
	for(loop=0; loop < D_CMU_TABLE_MAX ; loop++)
	{
		if((tRouteTbl_p->info[loop].event_id == uiEventID) && 
		   (tRouteTbl_p->info[loop].event_id_sub == uiSignalKind) &&
		   (tRouteTbl_p->info[loop].dst_que_id == uiQueID))
		{
			memset(&tRouteTbl_p->info[loop], 0, sizeof(T_CMU_ROUTE_INFO));
			tRouteTbl_p->infoNum--;
			uiRet = D_CMU_OK;
			break;
		}
	}
	
	/****************************************************************************/
	/* Mutex Unlock																*/
	/****************************************************************************/
	uiRetMutex = pthread_mutex_unlock(&(tRouteTbl_p->mutex));
	if( uiRetMutex != D_CMU_OK ){
		uiRet = D_CMU_NG_MUTEX_UNLOCK;
	}
	
	return uiRet;
}

/* @} */
