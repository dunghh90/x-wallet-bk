/*!
 * @skip  $ld:$
 * @file  f_eqs_anlz.c
 * @brief Analyze and dispatch message.
 * @date 2018/08/21 KCN)Kamimoto create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018-2019
 */
#include "f_eqs_header.h"			/* 5GDU-LLS監視タスクヘッダファイル			*/
    
/*!
 * @addtogroup RRH_PF_EQS
 * @{
 */

/*!
 *  @brief  Analyze and dispatch message(MSI)
 *  @note   This function is processed as follows.\n
 *          - Get event number from task IF header of message
 *          - Transform the event number to internal event number
 *          - Call the corresponding function in matrix
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2018/11/21 KCN)Kamimoto create
 *  @warning	N/A
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eqs_MSIanlz( VOID *pRcvMsg )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT						w_messageId;
	UINT						w_requestId;
	INT							w_payloadSizePtr;
	VOID *						w_payloadPtr;

	/************************************************************/
	/* Get MSI Data Function									*/
	/************************************************************/
	f_com_MSIgetPayload ( pRcvMsg ,				/* [in]  Receive Message pointer			*/
						  &w_messageId ,		/* [out] Receive Message ID					*/
						  &w_requestId ,		/* [out] Receive Message Request ID			*/
						  &w_payloadSizePtr ,	/* [out] Receive Message payload size		*/
						  &w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/
	if( f_eqsw_thrdstate <= D_RSV_TSK_STATE_USE )
	{
		switch( w_messageId )
		{
			case D_MSG_MSI_IETF_HARDWARE_GET_REQ :					/* ietf-hardware GET要求 */
				/* ietf-hardware GET要求処理を実施 */
				f_eqs_TraietfGETReq( w_requestId , w_payloadPtr );
				break;
			case D_MSG_MSI_IETF_HARDWARE_STATE_GET_REQ :			/* ietf-hardware state GET要求 */
				/* ietf-hardware state GET要求処理を実施 */
				f_eqs_TraietfStsGETReq( w_requestId , w_payloadPtr );
				break;
			case D_MSG_MSI_O_RAN_HARDWARE_GET_REQ :					/* O-RAN hardware GET要求 */
				/* O-RAN-hardware GET要求処理を実施 */
				f_eqs_Tra_o_ran_GETReq( w_requestId , w_payloadPtr );
				break;
			case D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_REQ :				/* ietf-hardware State EDIT要求 */
				/* ietf-Hardware state EDIT要求処理を実施 */
				f_eqs_TraietfStsEDITReq( w_requestId , w_payloadPtr ) ;
				break;
			case D_MSG_MSI_O_RAN_OPERATIONS_RESET_IND :					/* Reset要求 */
				/* O-RAN-Operations_Reset要求処理を実施 */
				f_eqs_Tra_o_ran_ResetReq( w_requestId , w_payloadPtr ) ;
				break;
			case D_MSG_MSI_O_RAN_FM_ALARM_START_IND :				/* アラーム通知開始通知 */
				/* MSI O_RAN_FM_ALARM_START_IND受信処理を実施 */
				f_eqs_Tra_o_ran_FMalrmStaIND( w_requestId , w_payloadPtr ) ;
				break ;
			case D_MSG_MSI_IETF_HARDWARE_STATE_CHANGE_START_IND :	/* アラーム通知開始通知 */
				/* MSI IETF_HARDWARE_STATE_CHANGE_START受信処理を実施 */
				f_eqs_TraietfStartIND( w_requestId , w_payloadPtr ) ;
				break ;
			case D_MSG_MSI_O_RAN_OPERATIONS_GET_REQ :					/* o-ran-operations get要求	*/
				/* O-RAN-Operations_Reset Get要求処理を実施 */
				f_eqs_o_ran_operations_getReq( w_requestId , w_payloadPtr ) ;
				break ;
			case D_MSG_MSI_O_RAN_OPERATIONS_EDIT_REQ :					/* o-ran-operations edit要求	*/
				/* o-ran-operations_edit要求処理を実施	*/
				f_eqs_o_ran_operations_editReq( w_requestId , w_payloadPtr ) ;
				break ;
			case D_MSG_MSI_O_RAN_HARDWARE_EDIT_REQ :
				/* O-RAN-Hardware Edit要求処理を実施 */
				f_eqs_Tra_o_ran_StsEditReq( w_requestId , w_payloadPtr ) ;
				break ;
			default :								/* その他 */
				f_com_assert(w_messageId, 
							"Exception MSI Event Rcv" );
				break;
		}
	}
	return ;
}

/* @} */

/*!
 *  @brief  Analyze and dispatch message
 *  @note   This function is processed as follows.\n
 *          - Get event number from task IF header of message
 *          - Transform the event number to internal event number
 *          - Call the corresponding function in matrix
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2013/11/14 FFCS)niemsh create
 *  @warning	N/A
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eqs_anlz( VOID *pRcvMsg )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT	a_tsk_state	= D_RSV_TSK_STATE_OK;	/* タスク状態格納変数			*/
	UINT 	a_event_no;
	T_SYS_COMMON_THDIF *ptTskif;
	T_API_SVP_DBG_SVCTL_NTC	*apip;
	
	/************************************************************/
	/* Get event number from task IF header of message			*/
	/************************************************************/
	ptTskif = (T_SYS_COMMON_THDIF*)pRcvMsg;
	a_event_no = ptTskif->uiEventNo;

	/************************************************************/
	/* Transform the event number to internal event number		*/
	/************************************************************/
	/* 起動要因番号から処理の振分け */
	switch( a_event_no )
	{
		/***************************************/
		/* TRAカード状態変化割込通知受信時 */
		/***************************************/
		case D_SYS_MSGID_TRA_IRQNTC :
			/* 自タスクのタスク状態が"通常運用状態"の場合 */
			if( f_eqsw_thrdstate <= D_RSV_TSK_STATE_USE )
			{
				/* TRAカード状態変化処理を実施 */
				f_eqs_TraStaChgNtc(( T_SYS_EQS_ALM_INFO*)pRcvMsg );
			}
			else
			{
				a_tsk_state = D_RSV_TSK_STATE_NG;
			}
			break;
		
		/****************************************/
		/* SV Bus 制御通知(for degug)受信時		*/
		/****************************************/
		case D_API_MSGID_SVP_DBG_SVCTL_NTC:
			apip = pRcvMsg;
			f_com_SVCtl(apip->data.svOpt, apip->data.svCnt, apip->data.svVal);
			return;
			break;
		
		/****************************************/
		/* MSI受信通知受信時		*/
		/****************************************/
		case D_SYS_MSGID_MSI_RCV_IND:
			/* MSIメッセージ受信解析処理を実行 */
			f_eqs_MSIanlz( pRcvMsg ) ;
			break;
			
		/****************************************/
		/* 停波抑止通知(by TP command)受信時	*/
		/****************************************/
		case D_API_MSGID_SVP_CMD_STPWVMSK_NTC:
			f_eqs_inhibitStpWv( ) ;
			break;
		default :
			/* その他の要求を受信した場合はアサート出力 */
			f_com_assert(a_event_no, 
					    "Exception Event Rcv" );
			
			break;
	}

	/* タスク状態と受信した起動要因が不適だった場合アサート出力 */
	if( a_tsk_state != D_RSV_TSK_STATE_OK )
	{
		f_com_assert(a_event_no,  "Task State NG" );
	}
	
	return;    
}

/* @} */

