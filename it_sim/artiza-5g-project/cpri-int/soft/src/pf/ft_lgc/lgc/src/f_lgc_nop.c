/*!
 * @skip		$ld:$
 * @file		f_lgc_nop.c
 * @brief		ログスレッド NOP処理
 * @author		ALPHA)yokoyama
 * @date		2013/10/29 ALPHA) 横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LGC_LGC
 * @{
 */

#include "f_lgc_inc.h"

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
 * @date 2018/08/24 KCN)若水 カスケードを削除 sysKindの3G/S3GをSUB6/MMWに変更
 */
INT f_lgc_nop(VOID* rcvMsg_p)
{
	INT						rtn			= D_RRH_OK;		/*	処理結果格納変数		*/
	UINT					eventNo		= 0;			/*	イベント番号格納変数	*/
	T_RRH_HEAD*				msgPtr		= NULL;			/*	メッセージ変数			*/

	/* メッセージを取得	*/
	msgPtr = (T_RRH_HEAD*)rcvMsg_p;
	
	/* イベント番号を取得	*/
	eventNo = msgPtr->uiEventNo;
	
	/* assert処理 */
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_nop] event=%08x", eventNo );
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_nop] state=%08x", f_lgcw_thdState );

	return rtn;
}

/* @} */
