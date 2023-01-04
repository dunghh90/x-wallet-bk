/*!
 * @skip		$ld:$
 * @file		f_cmu_recv_init.c
 * @brief		cmu 初期状態処理
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
 * @brief		f_cmu_com_init
 * @note		cmu共通 スレッド初期状態処理.
 *					-# RAMテーブルの初期化.
 *					-# mutexオブジェクトの初期化.
 *					-# 初期化完了通知送信.
 * @param		*rcvMsg_p		[in]	受信メッセージ
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
UINT f_cmu_com_init(T_CMU_CREATE_THREAD* rcvMsg_p, T_CMU_ROUTE_MNG* tRoute_p, T_CMU_THREAD_MNG* tProcTbl_p, USHORT* usThdState)
{
	T_RRH_HEAD					*sndMsg;								/* 送信メッセージ(初期化完了通知)	*/
	INT							endcd		= 0;
	INT							errcd;
	
	/************************************************************/
	/* mutexオブジェクトの初期化								*/
	/************************************************************/
	pthread_mutex_init( &(tRoute_p->mutex), NULL);
	
	/************************************************************/
	/* テーブル初期化											*/
	/************************************************************/
	f_cmu_com_iniTbl(rcvMsg_p, tRoute_p, tProcTbl_p);
	
	/************************************************************/
	/* バッファ取得												*/
	/************************************************************/
	endcd = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_RRH_HEAD),						/* MessageSize						*/
										(VOID **)&sndMsg,						/* MessageBuffer					*/
										&errcd );								/* ErrorCode						*/
	if( endcd != D_RRH_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " ADDRGET NG %d ", endcd );
		return D_RRH_NG;
	}
	/************************************************************/
	/* 初期化完了通知送信設定									*/
	/************************************************************/
	/* 送信MSG作成 */
	sndMsg->uiEventNo			= D_CMU_INIT_COMP_IND;				/* イベント番号						*/
	sndMsg->uiSignalkind		= 0;								/* SignalKind(初期化完了通知は不要)	*/
	sndMsg->uiDstPQueueID		= rcvMsg_p->uiProcQID;				/* 最終送信先Process Queue ID		*/
	sndMsg->uiDstTQueueID		= rcvMsg_p->uidstThreadQID;			/* 最終送信先Thread  Queue ID		*/
	sndMsg->uiSrcPQueueID		= tProcTbl_p->uiProcQID;			/* 送信元    Process Queue ID		*/
	sndMsg->uiSrcTQueueID		= tProcTbl_p->uiThreadQID;			/* 送信元    Thread  Queue ID		*/
	sndMsg->uiLength			= sizeof(T_RRH_HEAD);				/* Length							*/
	
	/************************************************************/
	/* 初期化完了通知送信										*/
	/************************************************************/
	endcd = BPF_RU_IPCM_MSGQ_SEND(
				rcvMsg_p->uidstThreadQID,									/* 送信MSGキューID					*/
				(CHAR *)sndMsg												/* 送信MSGポインタ					*/
			);
	
	if( endcd != D_RRH_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, " MSGQ_SEND NG %d ", endcd );
		return D_RRH_NG;
	}
	
	/* 運用中状態を設定	*/
	*usThdState = D_CMU_THD_STS_USE;
	
	return D_RRH_OK;
}

/* @} */
