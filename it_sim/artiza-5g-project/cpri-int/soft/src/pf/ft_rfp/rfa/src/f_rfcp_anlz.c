/*!
 * @skip  $ld:$
 * @file  f_rfcp_anlz.c
 * @brief pf_rfcpスレッド 受信メッセージ解析処理
 * @date  2018/08/14 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_rfcp_inc.h"


/****************************************************************************************************************************/
/*!
 * @brief		f_rfcp_MSIanlz
 * @note		受信メッセージ内イベント番号とスレッド状態から処理関数を実行する
 *				-# 受信メッセージ内イベント番号異常又はスレッド状態異常の場合、アボート処理を実施する(f_rfcp_abort())
 *				-# イベント番号とスレッド状態をIndexとしたジャンプテーブルより該当処理をCallする
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/14 KCN)takagi Create
 */
/****************************************************************************************************************************/
UINT f_rfcp_MSIanlz(VOID* msgP)
{
	UINT						w_messageId;
	UINT						w_requestId;
	INT							w_payloadSizePtr;
	VOID *						w_payloadPtr;
	UINT						w_evtNo = D_SYS_NG; ;

	/* MSIデータ取得 */
	/************************************************************/
	/* Get MSI Data Function									*/
	/************************************************************/
	f_com_MSIgetPayload ( msgP ,				/* [in]  Receive Message pointer			*/
						  &w_messageId ,		/* [out] Receive Message ID					*/
						  &w_requestId ,		/* [out] Receive Message Request ID			*/
						  &w_payloadSizePtr ,	/* [out] Receive Message payload size		*/
						  &w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/
	/* 受信イベントからスレッド内イベント番号へ変換 */
	switch( w_messageId )
	{
		case D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_REQ:				/* メッセージIDがProcessing Element EDIT要求なら */
			w_evtNo = D_RFCP_EVE_PRCCEMT_REQ ;
			break;
		case D_MSG_MSI_O_RAN_UPLANE_CONF_GET_REQ:				/* メッセージIDがu-plane conf. 状態GET要求なら	*/
			w_evtNo = D_RFCP_EVE_STATUS_REQ ;
			break;
		case D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_TX_REQ:				/* メッセージIDがu-plane conf. TX-EDIT要求なら		*/
			w_evtNo = D_RFCP_EVE_TXEDIT_REQ ;
			break;
		case D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REQ:				/* メッセージIDがu-plane conf. RX-EDIT要求なら		*/
			w_evtNo = D_RFCP_EVE_RXEDIT_REQ ;
			break;
		default:									/* その他なら									*/
			/*	受信 起動要因異常によるAbort	*/
			(VOID)f_rfcp_abort(msgP);

			/*	異常のため処理中断	*/
			return w_evtNo ;
	}

	return w_evtNo ;
}

/****************************************************************************************************************************/
/*!
 * @brief		f_rfcp_anlz
 * @note		受信メッセージ内イベント番号とスレッド状態から処理関数を実行する
 *				-# 受信メッセージ内イベント番号異常又はスレッド状態異常の場合、アボート処理を実施する(f_rfcp_abort())
 *				-# イベント番号とスレッド状態をIndexとしたジャンプテーブルより該当処理をCallする
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/14 KCN)takagi Create
 */
/****************************************************************************************************************************/
INT f_rfcp_anlz(VOID* msgP)
{
	INT						rtn = D_SYS_NG;							/* 復帰値								*/
	T_SYS_COMMON_THDIF*		rcvMsgP;								/* 受信メッセージ						*/
	UINT					evtNo;									/* イベント番号							*/
	
	rcvMsgP = (T_SYS_COMMON_THDIF*)msgP;
	
	/* 受信イベント番号からスレッド内イベント番号へ変換 */
	switch( rcvMsgP->uiEventNo )
	{
		case D_API_RFP_MNT_UPDATE_SYSTEM_PARAM_REQ:						/* System Parameter Update Request		*/
			evtNo = D_RFCP_EVE_SYSTEM_PARAM_UPDATE;
			break;
		
		case D_API_RFP_MQR_CARRIER_SET_REQ:								/* Carrier ON Request					*/
			evtNo = D_RFCP_EVE_CARRIER_ON;
			break;
		
		case D_API_RFP_MNT_CARRIER_RELEASE_REQ:							/* Carrier OFF Request					*/
			evtNo = D_RFCP_EVE_CARRIER_OFF;
			break;
		
		case D_API_RFP_MQR_TXRXCAL_INUSE_REQ:							/* The request of TxCal RxCal in use	*/
			evtNo = D_RFCP_EVE_TXRXCAL_INUSE_REQUEST;
			break;
		
		case D_API_RFP_MQR_TXRXCAL_TRAINING_REQ:						/* The request of TxCal RxCal Training	*/
			evtNo = D_RFCP_EVE_TXRXCAL_TRAINING_REQUEST;
			break;
		/****************************************/
		/* MSI受信通知受信時		*/
		/****************************************/
		case D_SYS_MSGID_MSI_RCV_IND:
			/* MSIメッセージ受信解析処理を実行 */
			evtNo = f_rfcp_MSIanlz( msgP ) ;
			break;
		
		case D_SYS_MSGID_TXON_CFM:										/* TX-ON(M-Plane) Comfirm				*/
			evtNo = D_RFCP_EVE_TXON_COMFIRM;
			break;
		case D_SYS_MSGID_RXON_CFM:										/* RX-ON(M-Plane) Comfirm				*/
			evtNo = D_RFCP_EVE_RXON_COMFIRM;
			break;
		case D_SYS_MSGID_TXOFF_CFM:										/* TX-OFF(M-Plane) Comfirm				*/
			evtNo = D_RFCP_EVE_TXOFF_COMFIRM;
			break;
		case D_SYS_MSGID_RXOFF_CFM:										/* RX-OFF(M-Plane) Comfirm				*/
			evtNo = D_RFCP_EVE_RXOFF_COMFIRM;
			break;
		default:
			/*	受信 起動要因異常によるAbort	*/
			(VOID)f_rfcp_abort(rcvMsgP);
			
			/*	異常のため処理中断	*/
			return rtn;
	}
	
	/*	スレッド状態異常の場合、アボート処理	*/
	if(f_rfcpw_thdInfo.thdState >= D_RFCP_STATENUM)
	{
		/*	スレッド状態異常によるAbort	*/
		(VOID)f_rfcp_abort(rcvMsgP);
	}
	else
	{
		/* マトリクステーブルに状態、起動要因を設定 */
		(VOID)(*(f_rfcpr_callSetRell_mtxtbl[f_rfcpw_thdInfo.thdState][evtNo].eventProcess_addr))(rcvMsgP);
	}
	
	rtn = D_SYS_OK;
	return rtn;
}

/* @} */
