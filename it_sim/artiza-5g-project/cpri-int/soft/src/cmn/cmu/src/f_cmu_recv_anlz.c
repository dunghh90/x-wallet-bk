/*!
 * @skip		$ld:$
 * @file		f_cmu_recv_anlz.c
 * @brief		cmu 受信メッセージ解析処理
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
 * @brief		f_recv_anlz
 * @note		受信メッセージを解析し、状態に対応する処理を実行.
 *					-# 受信パラメータポインタチェック.
 *						- NG時：Assertログ取得関数Call(BPF_COM_LOG_ASSERT()).
 *					-# 起動要因毎にイベント番号変換.
 *					-# スレッド状態チェック.
 *						- NG時：Assertログ取得関数Call(BPF_COM_LOG_ASSERT()).
 *					-# 起動要因とタスク状態により該当関数をCall.
 *					-# 正常終了を戻り値として上位に返却.
 * @param		*rcvMsg_p	[in]	受信メッセージ
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
UINT f_recv_anlz(VOID *msgP)
{
	/* 変数宣言	*/
	T_RRH_MESSAGE_TYPE*		rcvMsgP;				/* 受信メッセージ		*/
	UINT					evtNo;

	/*	受信MSG NULLチェック	*/
	if(msgP == D_RRH_NULL)
	{
		/*	Assert Log		*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, (const char*)"Message NULL NG");

		return D_RRH_NG;
	}

	rcvMsgP	= (T_RRH_MESSAGE_TYPE*)msgP;

	/* 受信イベント番号からスレッド内イベント番号へ変換 */
	switch( rcvMsgP->header.uiEventNo )
	{
		case D_CMU_INIT_REQ:		/* 初期化要求	*/
			evtNo	= D_CMU_THD_EVE_INIT;
			break;

		default:
			evtNo	= D_CMU_THD_EVE_MSG;
			break;
	}

	/* スレッド状態異常 */
	if(f_recvw_thdState >= D_CMU_STATENUM)
	{
		/*	Assert Log		*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_CRITICAL, (const char*)"Thread State NG");

		return D_RRH_NG;
	}

	/* 受信メッセージより起動要因取得							*/
	/* 起動要因とタスク状態よりマトリクステーブルから関数起動	*/
	/* マトリクステーブルから起動された関数の戻り値を上位に返却	*/
	(VOID)(*(f_cmur_callSetRell_recvTbl[f_recvw_thdState][evtNo].eventProcess_addr))(msgP);

	return D_RRH_OK;
}

/* @} */