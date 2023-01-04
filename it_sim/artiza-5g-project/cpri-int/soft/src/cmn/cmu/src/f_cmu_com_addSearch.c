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
 * @brief		f_cmu_com_addSearch
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
 * @param		*tRouteTbl_p	[in]	メッセージ送信先管理情報テーブル.
 * @retval		D_CMU_OK				0:正常終了
 * @retval		D_CMU_NG				1:異常終了
 * @retval		D_CMU_NG_MUTEX_LOCK		2:Mutexロック失敗
 * @retval		D_CMU_NG_MUTEX_UNLOCK	3:Mutexアンロック失敗
 * @retval		D_CMU_NG_TABLE_FULL		4:テーブルが満
 * @retval		D_CMU_NG_EXIST			6:同一IDが存在
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
UINT f_cmu_com_addSearch(UINT uiEventID , UINT uiSignalKind, UINT uiQueID, T_CMU_ROUTE_MNG* tRouteTbl_p)
{
	UINT					loop		= 0;
	UINT					uiRet		= D_CMU_NG;
	UINT					uiRetMutex	= D_CMU_NG;
	UINT					index		= 0;
	
	/****************************************************************************/
	/* Mutex Lock																*/
	/****************************************************************************/
	uiRetMutex = pthread_mutex_lock(&(tRouteTbl_p->mutex));
	if( uiRetMutex != D_CMU_OK ){
		return D_CMU_NG_MUTEX_LOCK;
	}
	/************************************************************/
	/* テーブルがMAXの場合、追加不可のため処理終了				*/
	/************************************************************/
	if(tRouteTbl_p->infoNum >= D_CMU_TABLE_MAX)
	{
		/****************************************************************************/
		/* Mutex Unlock																*/
		/****************************************************************************/
		pthread_mutex_unlock(&(tRouteTbl_p->mutex));
		return D_CMU_NG_TABLE_FULL;
	}
	
	/************************************************************/
	/* テーブルに空きがあれば追加								*/
	/************************************************************/
	for(loop=0; loop < D_CMU_TABLE_MAX; loop++)
	{
		/********************************************************************************/
		/* 同一のEventID、送信先スレッドQIDが見つかった場合は追加しない					*/
		/********************************************************************************/
		if((tRouteTbl_p->info[loop].event_id == uiEventID) && 
		   (tRouteTbl_p->info[loop].event_id_sub == uiSignalKind) &&
		   (tRouteTbl_p->info[loop].dst_que_id == uiQueID))
		{
			uiRet = D_CMU_NG_EXIST;
			break;
		}
		
		if(tRouteTbl_p->info[loop].event_id == 0)
		{
			index = loop;
			uiRet = D_CMU_OK;
			break;
		}
	}

	if(uiRet == D_CMU_OK)
	{
		tRouteTbl_p->info[index].event_id = uiEventID;
		tRouteTbl_p->info[index].event_id_sub = uiSignalKind;
		tRouteTbl_p->info[index].dst_que_id = uiQueID;
		tRouteTbl_p->infoNum++;
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
