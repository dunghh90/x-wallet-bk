/*!
 * @skip  $ld:$
* @file  f_rfcp_upcnf_GET.c(base: f_rfcp_act_carrierOn.c)
 * @brief pf_rfcpスレッド u-plane conf. 状態GET要求受信処理
 * @date  2018/11/22 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_rfcp_inc.h"
#include "f_sys_type.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_rfcp_upcnf_GET
 * @note		u-plane conf. 状態GET要求受信時の処理を行う
 *				-# パラメータチェック実施
 *				-# 処理実行中状態を返却する
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/11/22 KCN)上本 Create
 */
/****************************************************************************************************************************/
int f_rfcp_upcnf_GET(void* msgP)
{
	UINT						w_messageId;
	UINT						w_requestId;
	INT							w_payloadSizePtr;
	T_RFCP_UPCNF_GET_REQ *		w_payloadPtr;
	T_RFCP_UPCNF_GET_CMF		sndMsg;

    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_rfcp_upcnf_GET called");

	memset(&sndMsg , D_RRH_ZERO , sizeof(T_RFCP_UPCNF_GET_CMF));

	/* MSIデータ取得 */
	/************************************************************/
	/* Get MSI Data Function									*/
	/************************************************************/
	f_com_MSIgetPayload ( msgP ,						/* [in]  Receive Message pointer			*/
						  &w_messageId ,				/* [out] Receive Message ID					*/
						  &w_requestId ,				/* [out] Receive Message Request ID			*/
						  &w_payloadSizePtr ,			/* [out] Receive Message payload size		*/
						  (VOID *)&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	/* 状態の設定 */
	sndMsg.state = f_upcnf_state[w_payloadPtr->ant][w_payloadPtr->car][w_payloadPtr->tx_rx] ;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_com_MSIsend send(D_MSG_MSI_O_RAN_UPLANE_CONF_GET_CFM) state = %d" , sndMsg.state);
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_upcnf_state(ant:%d):(car:%d):(TXRX:%d) = %d"
													, w_payloadPtr->ant 
													, w_payloadPtr->car 
													, w_payloadPtr->tx_rx
													, f_upcnf_state[w_payloadPtr->ant][w_payloadPtr->car][w_payloadPtr->tx_rx]);

	/* MSI送信関数(OK応答)の実行 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_UPLANE_CONF_GET_CFM ,	/* 送信するメッセージID		*/
						 w_requestId ,							/* リクエストID				*/
						 sizeof( T_RFCP_UPCNF_GET_CMF ),		/* 送信するデータサイズ		*/
						 &sndMsg ,								/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,						/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_RFA ) ;				/* pf_rfaのスレッドID		*/
	
    return D_SYS_OK;
}

/* @} */
