/*!
 * @skip  $ld:$
 * @file  f_ful_abort.c
 * @brief FULタスク Abort処理
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_ful_inc.h"

/*!
 * @brief 関数機能概要:Abort処理を行う。
 * @note  関数処理内容.
 *		-# Abort処理を実施。				f_com_abort() 関数Call
 *		-# 固定で正常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/01/28 KCN)上本 Create
 */
INT	f_ful_abort(VOID* rcvMsg_p)
{
	INT						rtn;			/*	処理結果格納変数		*/

	UINT					w_messageId;
	UINT					w_requestId;
	INT						w_payloadSizePtr;
	VOID *					w_payloadPtr;

	rtn = D_SYS_OK;

	/*	メッセージを取得	*/
	/* MSIデータ取得 */
	/************************************************************/
	/* Get MSI Data Function									*/
	/************************************************************/
	f_com_MSIgetPayload(	rcvMsg_p ,				/* [in]  Receive Message pointer			*/
							&w_messageId ,			/* [out] Receive Message ID					*/
							&w_requestId ,			/* [out] Receive Message Request ID			*/
							&w_payloadSizePtr ,		/* [out] Receive Message payload size		*/
							&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	/* assert処理 */
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "ful thread error %08x", w_messageId);

	/*	abort処理 */
	f_com_abort(D_RRH_PROCID_PF, D_DU_ALMCD_TIF);
	
	return rtn;
}

/* @} */
