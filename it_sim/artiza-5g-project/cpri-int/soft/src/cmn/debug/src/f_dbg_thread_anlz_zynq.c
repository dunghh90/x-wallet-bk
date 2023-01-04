/*!
 * @skip		$ld:$
 * @file		f_dbg_thread_anlz_zynq.c
 * @brief		デバッグスレッド受信メッセージ解析処理
 * @author		alpha)宮崎
 * @date		2013/11/26 alpha)宮崎 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include "f_dbg_thread_inc.h"
extern UINT f_dbg_thread_exec_cmd( T_DBGTHREAD_EXEC_INFO *execParam, VOID *msgP );

/*!
 * @brief		f_dbg_thread_anlz
 * @note		受信メッセージを解析し、状態に対応する処理を実行.
 *					-# 受信パラメータポインタチェック.
 *						- NULL時はタスク間IFエラーにてアラーム制御関数Call(f_com_almReport()).
 *					-# 起動要因範囲チェック.
 *						- NG時：Assertログ取得関数Call(f_com_assert()).
 *						- NG時はタスク間IFエラーにてアラーム制御関数Call(f_com_almReport()).
 *					-# スレッド状態チェック.
 *						- NG時：Assertログ取得関数Call(f_com_assert()).
 *						- NG時はタスク状態異常にてアラーム制御関数Call(f_com_almReport()).
 *					-# 起動要因とタスク状態により該当関数をCall.
 *					-# 正常終了を戻り値として上位に返却.
 * @param		*msgP	[in]	受信メッセージ
 * @retval		D_RRH_OK	0:正常終了
 * @retval		D_RRH_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)宮崎
 */
UINT f_dbg_thread_anlz(T_DBGTHREAD_EXEC_INFO *execParam, VOID *msgP)
{
	/* 変数宣言	*/
	T_SYS_COMMON_THDIF*		rcvMsgP;				/* 受信メッセージ		*/

	/*	受信MSG NULLチェック	*/
	if(msgP == D_RRH_NULL)
	{
		/* NULL					*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Recive Message Empty. myProcQId:%08x myThreadQID:%08x", execParam->uiProcQID,execParam->uiDbgThreadQID);
		return D_RRH_NG;
	}
	else
	{
		rcvMsgP	= (T_SYS_COMMON_THDIF*)msgP;

		/* 受信イベント番号からスレッド内イベント番号へ変換 */
		switch( rcvMsgP->uiEventNo )
		{
			case CMD_TSKIF_DBG_EXEC_CMD_REQ:		/* デバッグコマンド実行要求	*/
				/* コマンド実行	*/
				f_dbg_thread_exec_cmd( execParam, msgP )	;
				break;

			default:
				if( execParam->usrFunc != NULL ) {	/* ユーザー定義関数(スレッド起動パラメータ）がある？ */
					(*execParam->usrFunc)( msgP );
					return D_RRH_NG;
				}
				else {
					/*	Assert Log		*/
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Thread IF NG(EventNo Error:%08x->%08x)", CMD_TSKIF_DBG_EXEC_CMD_REQ, rcvMsgP->uiEventNo );
					return D_RRH_NG;
				}
		}

	}
	return D_RRH_OK;
}

/* @} */
