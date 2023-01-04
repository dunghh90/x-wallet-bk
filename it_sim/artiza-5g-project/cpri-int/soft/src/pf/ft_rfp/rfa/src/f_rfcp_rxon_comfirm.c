/*!
 * @skip  $ld:$
* @file  f_rfcp_rxon_comfirm.c(base: f_rfcp_txon_comfirm.c)
 * @brief pf_rfcpスレッド RX-ON(M-Plane)応答受信処理
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
 * @brief		f_rfcp_rxon_comfirm
 * @note		RX-ON(M-Plane)応答受信時の処理を行う
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
 * @date		2019/02/22 KCN)吉木 notification追加
 */
/****************************************************************************************************************************/
int f_rfcp_rxon_comfirm(void* msgP)
{
	T_RFP_MPL_RX_ON_SETTING_CFM*		rcvMsg = (T_RFP_MPL_RX_ON_SETTING_CFM*)msgP ;

    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_rfcp_rxon_comfirm called");

	if ( rcvMsg->msgErr.msgErr.errorCode != D_RFCP_UPCNF_RSLT_OK ) {
		/* NG応答の場合ログを残す */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , "f_com_MSIsend send cancel(D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ)");
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR , "f_upcnf_state(ant:%d):(car:%d):RX = %d requestID=%d latest requestID=%d"
														, rcvMsg->ant 
														, rcvMsg->car 
														, f_upcnf_state[rcvMsg->ant][rcvMsg->car][D_RFCP_UPCNF_STSRX]
														, rcvMsg->requestId 
														, f_upcnf_latest_msi_reqid[rcvMsg->ant][rcvMsg->car][D_RFCP_UPCNF_STSRX]);
	}

	if(f_upcnf_latest_msi_reqid[rcvMsg->ant][rcvMsg->car][D_RFCP_UPCNF_STSRX] == rcvMsg->requestId) {
		/* 最新の要求に対する応答の場合 */
		/* 状態の変更 */
		f_upcnf_state[rcvMsg->ant][rcvMsg->car][D_RFCP_UPCNF_STSRX] = E_RFCP_UPCNF_STS_READY ;
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_com_MSIsend send(D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM)");
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_upcnf_state(ant:%d):(car:%d):RX = %d requestID=%d latest requestID=%d"
														, rcvMsg->ant 
														, rcvMsg->car 
														, f_upcnf_state[rcvMsg->ant][rcvMsg->car][D_RFCP_UPCNF_STSRX]
														, rcvMsg->requestId 
														, f_upcnf_latest_msi_reqid[rcvMsg->ant][rcvMsg->car][D_RFCP_UPCNF_STSRX]);
	   /* notification MSG作成 */
	   T_MPSW_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND StatesndMsg;
	   StatesndMsg.number = 1;
	   StatesndMsg.array_carrier_state_t[0].ant = rcvMsg->ant;
	   StatesndMsg.array_carrier_state_t[0].car = rcvMsg->car;
	   StatesndMsg.array_carrier_state_t[0].state = f_upcnf_state[rcvMsg->ant][rcvMsg->car][D_RFCP_UPCNF_STSRX];
	   /* notification送信 */
	   (VOID)f_com_MSIsend(
	       D_MSG_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND,                 /* 送信するメッセージID     */
	       0,                                                                  /* リクエストID             */
	       sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND),        /* 送信するデータサイズ     */
	       &StatesndMsg,                                                       /* 送信データのポインタ     */
	       D_RRH_PROCQUE_PF,                                                   /* PFのプロセスID           */
	       D_SYS_THDQID_PF_RFA);                                               /* pf_rfaのスレッドID       */
        /* 最新のrequest idを初期化 */
		f_upcnf_latest_msi_reqid[rcvMsg->ant][rcvMsg->car][D_RFCP_UPCNF_STSRX] = 0;
	}
	else {
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_com_MSIsend send cancel(D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM)");
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_upcnf_latest_msi_reqid=%x  requestId=%x"
								,f_upcnf_latest_msi_reqid[rcvMsg->ant][rcvMsg->car][D_RFCP_UPCNF_STSRX]
								,rcvMsg->requestId);
	}
    return D_SYS_OK;
}

/* @} */
