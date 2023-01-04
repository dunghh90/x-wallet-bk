/*!
 * @skip  $ld:$
 * @file  f_rfcp_abort.c
 * @brief RFCPタスク Abort処理
 * @date  2011/07/26 ALPHA) 横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_rfcp_inc.h"

/*!
 * @brief 関数機能概要:Abort処理を行う。
 * @note  関数処理内容.
 *		-# Abort処理を実施。				f_com_abort() 関数Call
 *		-# 固定で正常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2011/07/26 ALPHA)横山 Create
 * @date 2013/03/07 ALPHA)松延 B25 対処
 */
INT f_rfcp_abort(VOID* rcvMsg_p)
{
	INT						rtn;			/*	処理結果格納変数		*/
	UINT					eventNo;		/*	イベント番号格納変数	*/
	T_SYS_COMMON_THDIF*		msgPtr;			/*	メッセージ変数			*/

	rtn = D_SYS_OK;

	/*	メッセージを取得	*/
	msgPtr = (T_SYS_COMMON_THDIF*)rcvMsg_p;
	
	/*	イベント番号を取得	*/
	eventNo = msgPtr->uiEventNo;
	
	/* assert処理 */
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "rfcp thread error %08x", eventNo);

	/*	abort処理 */
	f_com_abort(D_RRH_PROCID_PF, D_DU_ALMCD_TIF);
	
	return rtn;
}

/* @} */
