/*!
 * @skip		$ld:$
 * @file		f_cmu_send_msgSend.c
 * @brief		cmu スレッド間メッセージ受信,プロセス間メッセージ送信処理
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
 * @brief		f_cmu_com_searchID
 * @note		指定IDをテーブルの指定要素から検索.
 *					-# Mutex Lock.
 *					- Mutex Lock失敗の場合.
 *						-# アサートログを出力し、異常終了.
 *					-# 指定IDをテーブルの指定要素のイベントIDを比較.
 *					- 比較結果が一致した場合.
 *						-# OUTパラにテーブルの指定要素のQIDを設定.
 *					-# Mutex Unlock.
 *					- Mutex Unlock失敗の場合.
 *						-# アサートログを出力し、異常終了.
 * @param		uiLoop		[in]	メッセージ送信管理情報テーブルの要素数
 * @param		uiChkNum	[in]	EventID(signalKind)
 * @param		*uiDstQID	[out]	送信先プロセスQID
 * @retval		count
 * @return		見つかったQueID数
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
UINT f_cmu_com_searchID( UINT EventNo , UINT SignalKind , T_CMU_ROUTE_MNG* tRouteTbl_p , UINT* uiQueBox_p)
{
	UINT					count = 0;
	UINT					loop;
	
	/****************************************************************************/
	/* Mutex Lock																*/
	/****************************************************************************/
	(VOID)pthread_mutex_lock(&tRouteTbl_p->mutex);
	
	/************************************************************/
	/* 登録数分ループし、QueIDを設定							*/
	/************************************************************/
	for(loop=0; loop < (tRouteTbl_p->infoNum); loop++)
	{
		if((EventNo == tRouteTbl_p->info[loop].event_id) && 
		   (SignalKind == tRouteTbl_p->info[loop].event_id_sub))
		{
			uiQueBox_p[count] = tRouteTbl_p->info[loop].dst_que_id;
			count++;
		}
	}

	/****************************************************************************/
	/* Mutex Unlock																*/
	/****************************************************************************/
	(VOID)pthread_mutex_unlock(&tRouteTbl_p->mutex);
	
	return count;
}


/*!
 * @brief		f_cmu_com_msgSend
 * @note		cmuプロセス間メッセージ送信処理.
 *					-# 送信メッセージ設定.
 *					- 以下の情報を設定する.
 *						-# イベント番号.
 *						-# SignalKind.
 *						-# 最終送信先プロセスQID.
 *						-# 最終送信先スレッドQID.
 *						-# 送信元プロセスQID.
 *						-# 送信元スレッドQID.
 *						-# データ部のバイト長.
 *						-# データ.
 *					-# プロセス間メッセージ送信.
 * @param		*rcvMsg_p		[in]	受信メッセージ
 * @param		*tProcTbl		[in]	自スレッド情報
 * @param		uiDstQueID		[in]	送信先QIO
 * @return		N/A
 * @warning		CPRI信号受信イベントの場合、最下位ビットはみないこと(同メッセージで装置種別が違うだけのため)
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
VOID f_cmu_com_msgSend(T_RRH_MESSAGE_TYPE* rcvMsg_p, T_CMU_THREAD_MNG* tProcTbl, UINT uiDstQueID)
{
	T_RRH_MESSAGE_TYPE				*sndMsg;				/* 送信メッセージ */

	/* 渡された情報を設定し、そのまま送信 */
	sndMsg = rcvMsg_p;
	
	/* メッセージ送信処理 */
	BPF_RU_IPCM_MSGQ_SEND(
		uiDstQueID,														/* 送信MSGキューID		*/
		(VOID *)sndMsg													/* 送信MSGポインタ		*/
	);
}

/*!
 * @brief		f_cmu_com_msgRecvSend
 * @note		cmuスレッド間メッセージ受信、プロセス間メッセージ送信関数をコール.
 *					-# KeyIDより共有メモリアドレスを取得.(BPF_RU_IPCM_PROCMSGQ_RECEIVE()).
 *					-# CPRI信号受信イベントか判定.
 *					- CPRI信号受信イベント以外の場合.
 *						-# 受信メッセージのEventIDを設定.
 *					- CPRI信号受信イベントの場合.
 *						-# 受信メッセージのSignalKindの最下位ビットを0にした値を設定.
 *					-# メッセージ送信先管理情報テーブル数分ループ.
 *						-# 送信先プロセスID検索関数をコール(f_send_searchID()).
 *						-# 戻り値を判定し、プロセスIDが見つかったか判定.
 *						- プロセスIDが見つかった場合.
 *							-# メッセージ送信関数をコール(f_send_msgSend()).
 *							-# 送信フラグをONに設定.
 *						- プロセスIDが見つからなかった場合.
 *							-# ループ継続.
 *					-# 送信フラグを判定.
 *					- 送信フラグがOFFの場合.
 *						-# メッセージ送信関数をコール(f_send_msgSend()).
 *					- 送信フラグがONの場合.
 *						-# 処理終了.
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @retval		D_RRH_OK	0:正常終了
 * @retval		D_RRH_NG	1:異常終了
 * @return		処理結果
 * @warning		SignalKindの最下位ビットはみないこと。(同メッセージで装置種別が違うだけのため)
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
UINT f_cmu_com_msgRecvSend(T_RRH_MESSAGE_TYPE* rcvMsg_p, T_CMU_ROUTE_MNG* tRoute_p, T_CMU_THREAD_MNG* tProcTbl)
{
	T_RRH_MSGFORM*			sendMsg;					/* 送信用メッセージバッファ		*/
	UINT					uiRet		= D_RRH_OK;		/* 処理結果						*/
	INT						errcd;						/* 詳細NGコード					*/
	UINT					loop		= 0;
	UINT					uiCount		= 0;
	UINT					uiChkNum	= 0;
	UINT					uiSendQueInfo[D_CMU_TABLE_MAX];
	
	/* 初期化 */
	memset(uiSendQueInfo, 0, sizeof(uiSendQueInfo));
	
	/* SignalKindは最下位ビットを落として送信先QIDを検索 */
	uiChkNum = rcvMsg_p->header.uiSignalkind & ~D_CMD_SIGKIND_BIT;
	
	/* 送信先プロセスID検索*/
	uiCount = f_cmu_com_searchID(rcvMsg_p->header.uiEventNo, uiChkNum, tRoute_p, uiSendQueInfo);
	
	/* 送信先テーブルから該当ID無し */
	if(uiCount == 0)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Message Send NG EventNo:%08X",rcvMsg_p->header.uiEventNo);
	}

	for(loop=0;loop<uiCount;loop++)
	{
		/* 重複送信の場合、再度メッセージ領域を取得する必要あり */
		/* 元データを最後に送信するための判定処理 */
		if((uiCount - 1) != loop)
		{
			/* 領域確保 */
			uiRet = BPF_RU_IPCM_PROCMSG_ADDRGET(E_BPF_RU_IPCM_BUFF_KIND_TASK,
											  rcvMsg_p->header.uiLength,
											  (void **)&sendMsg,
											  &errcd);
			
			if(uiRet != 0){
				BPF_COM_LOG_ASSERT ( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET uiRet=%d, error code=%d", uiRet,  errcd);
				continue;
			}
			
			/* 送信データ・ヘッダ部コピー */
			sendMsg->header.uiEventNo		= rcvMsg_p->header.uiEventNo;			/* イベント番号										*/
			sendMsg->header.uiSignalkind	= rcvMsg_p->header.uiSignalkind;		/* SignalKind(EventIDが重複しない場合はDon't care)	*/
			sendMsg->header.uiDstPQueueID	= rcvMsg_p->header.uiDstPQueueID;		/* 最終送信先Process Queue ID						*/
			sendMsg->header.uiDstTQueueID	= rcvMsg_p->header.uiDstTQueueID;		/* 最終送信先Thread  Queue ID						*/
			sendMsg->header.uiSrcPQueueID	= rcvMsg_p->header.uiSrcPQueueID;		/* 送信元    Process Queue ID						*/
			sendMsg->header.uiSrcTQueueID	= rcvMsg_p->header.uiSrcTQueueID;		/* 送信元    Thread  Queue ID						*/
			sendMsg->header.uiLength		= rcvMsg_p->header.uiLength;			/* データ部バイト長(KeyIDのため固定長)				*/
			
			memcpy( sendMsg->msgData, ((T_RRH_MSGFORM*)rcvMsg_p)->msgData, (rcvMsg_p->header.uiLength - sizeof( T_RRH_HEAD )) );
			
			/* メッセージ送信 */
			f_cmu_com_msgSend((T_RRH_MESSAGE_TYPE *)sendMsg, tProcTbl, uiSendQueInfo[loop]);
		}
		else{
			/* メッセージ送信 */
			f_cmu_com_msgSend(rcvMsg_p, tProcTbl, uiSendQueInfo[loop]);
		}
	}

	return uiRet;
}


/* @} */
