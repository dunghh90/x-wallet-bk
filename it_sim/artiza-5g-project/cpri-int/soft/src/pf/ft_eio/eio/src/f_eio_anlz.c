/*!
 * @skip  $ld:$
 * @file  f_eio_anlz.c
 * @brief Analyze and dispatch message.
 * @date 2019/03/18 FJT)Koshida create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019-
 */
    
/*!
 * @addtogroup RRH_PF_EIO
 * @{
 */
#include "f_eio_header.h"			/* 5GDU-LLS外部IOタスクヘッダファイル			*/

/*!
 *  @brief  Analyze and dispatch message(MSI)
 *  @note   This function is processed as follows.\n
 *          - Get event number from task IF header of message
 *          - Transform the event number to internal event number
 *          - Call the corresponding function in matrix
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/03/18 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eio_MSIanlz( VOID *pRcvMsg )
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
	
	switch( w_messageId )
	{
		/* O-RAN-externalio GET要求 */
		case D_MSG_MSI_O_RAN_EXTERNALIO_GET_REQ :
			f_eio_msiExtIoGetReq( w_requestId );
			break;
			
		/* O-RAN-externalio EDIT要求 */
		case D_MSG_MSI_O_RAN_EXTERNALIO_EDIT_REQ :
			f_eio_msiExtOutEditReq( w_requestId, w_payloadPtr );
			break;
			
		/* O-RAN-externalio notification Start通知 */
		case D_MSG_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_START_IND :
			f_eio_msiExtInNtfyStartInd( w_requestId );
			break;
			
		/* O-RAN-fan GET要求 */
		case D_MSG_MSI_O_RAN_FAN_GET_REQ :
			f_eio_msiFanGetReq( w_requestId );
			break;
			
		/* その他 */
		default :
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Exception MSI Event Rcv(0x%08x)", w_messageId);
			break;
	}
	return ;
}


/*!
 *  @brief  Analyze and dispatch message
 *  @note   This function is processed as follows.\n
 *          - Get event number from task IF header of message
 *          - Transform the event number to internal event number
 *          - Call the corresponding function in matrix
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/03/18 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eio_anlz( VOID *pRcvMsg )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT 	a_msgID;
	UINT 	a_eventIdx;
	T_SYS_COMMON_THDIF *ptTskif;
	
	/************************************************************/
	/* Get event number from task IF header of message			*/
	/************************************************************/
	ptTskif = (T_SYS_COMMON_THDIF*)pRcvMsg;
	a_msgID = ptTskif->uiEventNo;

	/************************************************************/
	/* Transform the event number to internal event number		*/
	/************************************************************/
	/* 起動要因番号から処理の振分け */
	switch( a_msgID )
	{
		/* External IO SV timer timeout indication		*/
		case D_SYS_MSGID_EIO_EXTOUTSV_TO :
			a_eventIdx = E_EIO_EVT_EXTIOSV_TIMEOUT;
			break;
		
		/* FAN SV & Cntrol timer timeout indication		*/
		case D_SYS_MSGID_EIO_FANSVCTRL_TO:
			a_eventIdx = E_EIO_EVT_FANSVCTRL_TIMEOUT;
			break;
		
		/* FAN debug mode indication					*/
		case D_API_MSGID_EXT_MNT_FANDBGMODE_CHG_NTC:
			a_eventIdx = E_EIO_EVT_API_FANDBG_IND;
			break;
		
		/* FAN DAC chage indication						*/
		case D_API_MSGID_EXT_MNT_FANDAC_CHG_NTC:
			a_eventIdx = E_EIO_EVT_API_FANDAC_IND;
			break;
		
		/* MSI message receive notify					*/
		case D_SYS_MSGID_MSI_RCV_IND:
			a_eventIdx = E_EIO_EVT_MSI_MSGRCV_NTF;
			break;
		
		default :
			/* その他の要求を受信した場合はアサート出力 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RCV MSG UNKOWN 0x%p(0x%08x)", pRcvMsg, a_msgID );
			a_eventIdx = E_EIO_EVT_MAX;
			break;
	}
	/*	有効イベントの場合	*/
	if( a_eventIdx < E_EIO_EVT_MAX )
	{
		/* 処理関数をCALL */
		f_eior_matrix[f_eiow_thrdstate][a_eventIdx]( pRcvMsg );
	}
	
	return;
}


/*!
 *  @brief  nop function
 *  @note   This function is nop & assert log set.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/03/18 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_eio_nop( VOID *pRcvMsg )
{
	UINT 	a_msgID;
	
	/*	メッセージID取得	*/
	if(pRcvMsg != NULL)
	{
		a_msgID = ((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo;
	}
	else
	{
		a_msgID = 0;
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "STATUS MATRIX ERROR!! sts=0x%08x, evt=0x%08x", f_eiow_thrdstate, a_msgID );
	
	return;
}

/* @} */

