/*!
 * @skip		$ld:$
 * @file		f_log_nop.c
 * @brief		ログスレッド NOP処理
 * @author		ALPHA)yokoyama
 * @date		2013/10/29 ALPHA) 横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

#include "f_log_inc.h"

/*!
 * @brief		関数機能概要:NOP処理を行う。
 * @note		関数処理内容.
 *				-# NOP処理を行う
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @retval		D_RRH_OK		0:正常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/10/29 ALPHA)横山 Create
 */
INT f_log_nop(VOID* rcvMsg_p)
{
	INT						rtn			= D_RRH_OK;		/*	処理結果格納変数		*/
	UINT					eventNo		= 0;			/*	イベント番号格納変数	*/
	T_RRH_HEAD*				msgPtr		= NULL;			/*	メッセージ変数			*/

	/* メッセージを取得	*/
	msgPtr = (T_RRH_HEAD*)rcvMsg_p;
	
	/* イベント番号を取得	*/
	eventNo = msgPtr->uiEventNo;
	
	/* assert処理 */
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "[f_log_nop] %08x", eventNo );

	return rtn;
}

/* @} */
