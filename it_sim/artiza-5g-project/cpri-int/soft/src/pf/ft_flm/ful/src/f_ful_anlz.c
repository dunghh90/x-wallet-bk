/*!
 * @skip  $ld:$
 * @file  f_ful_anlz.c
 * @brief FULタスク 受信メッセージ解析処理
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
 * @brief 関数機能概要:受信メッセージ内イベント番号とタスク状態から処理関数を実行する
 * @note  関数処理内容.
 *       -# 受信メッセージ内イベント番号異常又はタスク状態異常の場合、アボート処理を実施する(f_ful_abort())
 *       -# イベント番号とタスク状態をIndexとしたジャンプテーブルより該当処理をCallする
 * @param  *msgP			[in]VOID	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2019/01/28 KCN)上本 Create
 */
INT	f_ful_anlz ( VOID* msgP )
{
	INT						rtn = D_SYS_NG ;				/* 復帰値							*/
	T_SYS_COMMON_THDIF*		rcvMsgp ;						/* 受信メッセージ					*/
	UINT					evtNo = 0 ;						/* イベント番号						*/
	UINT					w_messageId;
	UINT					w_requestId;
	INT						w_payloadSizePtr;
	VOID *					w_payloadPtr;

	rcvMsgp = (T_SYS_COMMON_THDIF*)msgP;

	/* MSI受信通知受信チェック */
	if( rcvMsgp->uiEventNo == D_SYS_MSGID_MSI_RCV_IND )
	{
		/* MSIデータ取得 */
		/************************************************************/
		/* Get MSI Data Function									*/
		/************************************************************/
		f_com_MSIgetPayload(	msgP ,					/* [in]  Receive Message pointer			*/
								&w_messageId ,			/* [out] Receive Message ID					*/
								&w_requestId ,			/* [out] Receive Message Request ID			*/
								&w_payloadSizePtr ,		/* [out] Receive Message payload size		*/
								&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/
		/* 受信イベント番号からタスク内イベント番号へ変換 */
		switch( w_messageId )
		{
			case	D_MSG_MSI_O_RAN_FILE_MANAGEMENT_RETRIEVE_REQ :											/* メッセージIDがFile-List転送要求	*/
				evtNo = E_FUL_EVE_FILELIST ;
				break ;
			case	D_MSG_MSI_O_RAN_FILE_MANAGEMENT_UPLOAD_REQ :											/* メッセージIDがアップロード要求	*/
				evtNo = E_FUL_EVE_UPLOAD ;
				break ;
			default	:											/* メッセージIDがその他				*/
				/*	受信 起動要因異常によるAbort	*/
				(VOID)f_ful_abort(msgP);

				/*	異常のため処理中断	*/
				return rtn;
		}
	}
	else
	{
		/*	受信 起動要因異常によるAbort	*/
		(VOID)f_ful_abort(msgP);

		/*	異常のため処理中断	*/
		return rtn;
	}

	/*	スレッド状態異常の場合、アボート処理	*/
	if(f_ful_thdInfo.thdState >= D_FUL_STATENUM)
	{
		/* スレッド状態異常によるAbort */
		(VOID)f_ful_abort(rcvMsgp);
		return rtn ;
	}
	else
	{
		/* マトリクステーブルに状態、起動要因を設定 */
		(VOID)(*(f_ful_callSetRell_mtxtbl[f_ful_thdInfo.thdState][evtNo].eventProcess_addr))(rcvMsgp);
	}

	rtn = D_SYS_OK ;
	return rtn;
}

/* @} */
