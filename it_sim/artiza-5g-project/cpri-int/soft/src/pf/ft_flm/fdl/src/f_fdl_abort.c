/*!
 * @skip  $ld:$
 * @file  f_fdl_abort.c
 * @brief FDLタスク Abort処理
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_fdl_inc.h"

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
INT	f_fdl_abort(VOID* msg_p)
{
	INT						rtn;			/*	処理結果格納変数		*/

	T_SYS_COMMON_THDIF*		rcvMsgp ;						/* 受信メッセージ					*/
	UINT					w_messageId;
	UINT					w_requestId;
	INT						w_payloadSizePtr;
	VOID *					w_payloadPtr;

	rtn = D_SYS_OK;
	rcvMsgp = (T_SYS_COMMON_THDIF*)msg_p;

	if(rcvMsgp->uiEventNo == D_SYS_MSGID_MSI_RCV_IND)
	{
		/*	メッセージを取得	*/
		/* MSIデータ取得 */
		/************************************************************/
		/* Get MSI Data Function									*/
		/************************************************************/
		f_com_MSIgetPayload(	msg_p ,					/* [in]  Receive Message pointer			*/
								&w_messageId ,			/* [out] Receive Message ID					*/
								&w_requestId ,			/* [out] Receive Message Request ID			*/
								&w_payloadSizePtr ,		/* [out] Receive Message payload size		*/
								&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/
	
		/* assert処理 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "fdl thread error MSI %08x", w_messageId);

	}else {
		/* assert処理 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "fdl thread error Event 08x", rcvMsgp->uiEventNo);
	}
	/*	abort処理 */
	f_com_abort(D_RRH_PROCID_PF, D_DU_ALMCD_TIF);
	
	return rtn;
}

/* @} */
