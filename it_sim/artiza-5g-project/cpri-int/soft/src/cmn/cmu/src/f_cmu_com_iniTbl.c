/*!
 * @skip		$ld:$
 * @file		f_cmu_recv_iniTbl.c
 * @brief		cmu テーブル初期化処理
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
 * @brief		f_cmu_com_iniTbl
 * @note		cmuテーブル初期化処理.
 *					-# メッセージで渡されたメッセージ送信先情報管理テーブルを内部テーブルに保持
 *					-# プロセス情報テーブルを内部テーブルに保持
 *					-# メッセージ情報管理テーブルMAX情報フラグを0に設定
 * @param		*rcvMsg_p		[in]	受信メッセージ
 * @return		N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
VOID f_cmu_com_iniTbl(T_CMU_CREATE_THREAD* rcvMsg_p, T_CMU_ROUTE_MNG* tRoute_p, T_CMU_THREAD_MNG* tProcTbl_p)
{
	UINT loop;
	
	/****************************************************************************/
	/* Mutex Lock																*/
	/****************************************************************************/
	pthread_mutex_lock(&(tRoute_p->mutex));
	
	/************************************************************/
	/* メッセージ送信先情報管理テーブル初期化					*/
	/************************************************************/
	/* 内部テーブルに保持 */
	memcpy(tRoute_p->info, rcvMsg_p->tRouteInfo, (sizeof(T_CMU_ROUTE_INFO) * D_CMU_TABLE_MAX));
	
	/************************************************************/
	/* メッセージ送信先情報管理テーブル登録数設定				*/
	/************************************************************/
	tRoute_p->infoNum = D_CMU_TABLE_MAX;
	for(loop=0;loop<D_CMU_TABLE_MAX;loop++)
	{
		if(tRoute_p->info[loop].event_id == 0)
		{
			tRoute_p->infoNum = loop;
			break;
		}
	}
	
	/****************************************************************************/
	/* Mutex Unlock																*/
	/****************************************************************************/
	pthread_mutex_unlock(&(tRoute_p->mutex));
	
	/************************************************************/
	/* 自プロセス情報保持										*/
	/************************************************************/
	tProcTbl_p->uiProcID = rcvMsg_p->uiProcID;
	tProcTbl_p->uiProcQID = rcvMsg_p->uiProcQID;
	tProcTbl_p->uiThreadQID = rcvMsg_p->uiThreadQID;
}

/* @} */
