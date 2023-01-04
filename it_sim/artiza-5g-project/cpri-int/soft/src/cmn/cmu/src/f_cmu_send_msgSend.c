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
 * @brief		f_send_msgRecvSend
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
 * @return		N/A
 * @warning		CPRI信号受信イベントの場合、最下位ビットはみないこと。(同メッセージで装置種別が違うだけのため)
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
VOID f_send_msgRecvSend(VOID* rcvMsg_p)
{
	T_RRH_MESSAGE_TYPE*		rcvMsgP;
	
	/* 受信メッセージ保持 */
	rcvMsgP = (T_RRH_MESSAGE_TYPE*)rcvMsg_p;
	
	/************************************************************/
	/* 送信先のプロセスQIDが指定されている場合、				*/
	/* それを判断してメッセージ送信								*/
	/************************************************************/
	if(rcvMsgP->header.uiDstPQueueID != 0)
	{
		/* メッセージ送信 */
		f_cmu_com_msgSend(rcvMsgP, &f_sendw_ProcTbl, rcvMsgP->header.uiDstPQueueID);
	}
	/************************************************************/
	/* 送信先のプロセスQIDが指定されていない場合、				*/
	/* テーブルから検索してメッセージ送信						*/
	/************************************************************/
	else{
		(VOID)f_cmu_com_msgRecvSend(rcvMsgP, &f_sendw_route, &f_sendw_ProcTbl);
	}
}

/* @} */
