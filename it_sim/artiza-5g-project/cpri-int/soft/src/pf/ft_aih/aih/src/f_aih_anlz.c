/*!
 * @skip  $ld:$
 * @file  f_aih_anlz.c
 * @brief Analyze and dispatch message.
 * @date 2019/07/31 FJT)Takahashi create
 * @date 2019/09/13 FJT)Takahashi modify by IT2 issue.
 * All Rights Reserved, Copyright FUJITSU LIMITED 2019-
 */
    
/*!
 * @addtogroup RRH_PF_AIH
 * @{
 */
#include "f_aih_inc.h"			/* ALD(AISG)タスクヘッダファイル		*/

/*!
 *  @brief  Analyze and dispatch message(MSI)
 *  @note   This function is processed as follows.\n
 *          - Get event number from task IF header of message
 *          - Transform the event number to internal event number
 *          - Call the corresponding function in matrix
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/07/31 FJT)Takahashi create
 *  @date 2019/09/13 FJT)Takahashi modify by IT2 issue.
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

UINT f_aih_MSIanlz( VOID *pRcvMsg )
{ 
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT						w_messageId;
	UINT						w_requestId;
	INT							w_payloadSizePtr;
	VOID *						w_payloadPtr;
	UINT						w_evtNo = D_SYS_NG; ;

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
		/* ALD制御データ転送要求 */
		case D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ :
			w_evtNo = E_AIH_EVT_MSI_TRANSMIT_NTF ;  /* 1 */
			break;
			
		/* ALD電源ONOFF制御要求 */
		case D_MSG_MSI_O_RAN_ALD_PORT_EDIT_REQ :
			w_evtNo = E_AIH_EVT_MSI_PWRCTRL_NTF ;   /* 7 */
			break;
		
		case D_MSG_MSI_O_RAN_ALD_PORT_GET_REQ :
			w_evtNo = E_AIH_EVT_MSI_PWRGET_NTF ;    /* 8 */
			break;
		
		/* その他 */
		default :
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Exception MSI Event Rcv(0x%08x)", w_messageId);
			/*	異常のため処理中断	*/
			break;
	}
	return w_evtNo ;
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
 *  @date 2019/07/31 FJT)Takahashi create
 *  @date 2019/09/04 FJT)Takahashi modify by IT2 issue.
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_aih_anlz( VOID *pRcvMsg )
{
	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT 	a_msgID;
	UINT 	a_eventIdx;
	T_SYS_COMMON_THDIF *ptTskif;
	T_SYS_AISG_RCV_INT_NTC * ptAsigRcvInt;
	
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
		/* MSI受信通知 */
		case D_SYS_MSGID_MSI_RCV_IND:
			/* MSIメッセージ受信解析処理 */
			a_eventIdx = f_aih_MSIanlz( pRcvMsg ) ;
			break;
		
		/* ALD transmission completion interrupt */
		case D_SYS_MSGID_AIH_ALDSND_END:
			a_eventIdx = E_AIH_EVT_ALD_SEND_IRQ; /* 2 */
			break;
		
		/* ALD reception completion interrupt */
		case D_SYS_MSGID_AIH_ALDRCV_END:
			a_eventIdx = E_AIH_EVT_ALD_RCV_IRQ; /* 3 */
			break;
		
		/* ALD receive error interrupt */
/*		case D_SYS_MSGID_AIH_ALDRCV_ERR:
			a_eventIdx = E_AIH_EVT_ALD_ERR_IRQ;*/ /* 4 */
/*			break;*/
		
		/* ALD receive notification 受信要求 */
		case D_SYS_MSGID_AIH_ALDRCV_NTF:
			ptAsigRcvInt = (T_SYS_AISG_RCV_INT_NTC*)pRcvMsg;
			if(ptAsigRcvInt)
			{
				ptAsigRcvInt->f_func(ptAsigRcvInt->iParam);
			}
			return ;

		/* ALD receive start interrupt */
		case D_SYS_MSGID_AIH_ALDRCV_START:
			a_eventIdx = E_AIH_EVT_ALD_RCVING_IRQ; /* 5 */
			break;
		
		/* ALD wait timer timeout interrupt */
		case D_SYS_MSGID_AIH_ALD_TIMEOUT :
			a_eventIdx = E_AIH_EVT_ALD_RCV_TIMEOUT; /* 6 */
			break;
		
		default :
			/* その他の要求を受信した場合はアサート出力 */
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "RCV MSG UNKOWN (0x%08x)", a_msgID );
			a_eventIdx = E_AIH_EVT_NUM;
			break;
	}
	/*	有効イベントの場合	*/
	if( a_eventIdx < E_AIH_EVT_NUM )
	{
		/* 処理関数をCALL */
		f_aihr_matrix[f_aih_thrdstate][a_eventIdx]( pRcvMsg );
	}
	
	return;
}


/*!
 *  @brief  nop function
 *  @note   This function is nop & assert log set.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/07/31 FJT)Takahashi create
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_aih_nop( VOID *pRcvMsg )
{
	UINT 	a_msgID = 0;

	if(pRcvMsg != NULL){
		a_msgID = ((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo;
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "STATUS MATRIX ERROR!! sts=0x%08x, evt=0x%08x", f_aih_thrdstate, a_msgID );
	
	return;
}

/*!
 *  @brief  nop function
 *  @note   This function discard MSI notice & assert log set.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/08/01 FJT)Takahashi create
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_aih_nop1( VOID *pRcvMsg )
{
	UINT 	a_msgID = 0;
	
	/*	メッセージID取得	*/
	if(pRcvMsg != NULL){
		a_msgID = ((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo;
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "STATUS MATRIX ERROR!! sts=0x%08x, evt=0x%08x", f_aih_thrdstate, a_msgID );
	return;
}

/*!
 *  @brief  nop function
 *  @note   This function log ALD send completion & assert log set.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/08/01 FJT)Takahashi create
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_aih_nop2( VOID *pRcvMsg )
{
	UINT 	a_msgID = 0;
	
	/*	メッセージID取得	*/
	if(pRcvMsg != NULL){
		a_msgID = ((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "STATUS MATRIX ERROR!! sts=0x%08x, evt=0x%08x", f_aih_thrdstate, a_msgID );
	
	return;
}

/*!
 *  @brief  nop function
 *  @note   This function log ALD discard & assert log set.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/08/01 FJT)Takahashi create
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_aih_nop3( VOID *pRcvMsg )
{
	UINT 	a_msgID = 0;
	T_SYS_AISG_RCV_DAT_NTC*	pRcvntc;
	
	if(pRcvMsg != NULL){
		a_msgID = ((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo;
		pRcvntc = (T_SYS_AISG_RCV_DAT_NTC*)pRcvMsg;

		if(f_aih_thrdstate == E_AIH_STAT_IDLE){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "MSG Discard!! sts=0x%08x evt=0x%08x ret=0x%x len=0x%x alen=0x%x data=0x%02x%02x err=0x%x 0x%x 0x%x 0x%x", 
					f_aih_thrdstate,
					a_msgID,
					pRcvntc->uiResult,
					pRcvntc->uiLength,
					pRcvntc->uiAllLength,
					pRcvntc->ucData[0],
					pRcvntc->ucData[1],
					pRcvntc->usErr[0],
					pRcvntc->usErr[1],
					pRcvntc->usErr[2],
					pRcvntc->usErr[3]
					);
		}else{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "STATUS ERROR!! sts=0x%08x evt=0x%08x ret=0x%x len=0x%x alen=0x%x data=0x%02x%02x err=0x%x 0x%x 0x%x 0x%x", 
					f_aih_thrdstate,
					a_msgID,
					pRcvntc->uiResult,
					pRcvntc->uiLength,
					pRcvntc->uiAllLength,
					pRcvntc->ucData[0],
					pRcvntc->ucData[1],
					pRcvntc->usErr[0],
					pRcvntc->usErr[1],
					pRcvntc->usErr[2],
					pRcvntc->usErr[3]
					);
		}
	}else{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "STATUS ERROR!! sts=0x%08x, evt=0x%08x", f_aih_thrdstate, a_msgID );
	}
	return;
}

/*!
 *  @brief  nop function
 *  @note   This function log ALD error & assert log set.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/08/01 FJT)Takahashi create
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_aih_nop4( VOID *pRcvMsg )
{
	UINT 	a_msgID = 0;
	
	/*	メッセージID取得	*/
	if(pRcvMsg != NULL){
		a_msgID = ((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo;
	}
	
	if(f_aih_thrdstate == E_AIH_STAT_IDLE){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "MSG Discard!! sts=0x%08x, evt=0x%08x", f_aih_thrdstate, a_msgID );
	}else{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "STATUS MATRIX ERROR!! sts=0x%08x, evt=0x%08x", f_aih_thrdstate, a_msgID );
	}
	
	return;
}

/*!
 *  @brief  nop function
 *  @note   This function log ALD receive start & assert log set.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/08/01 FJT)Takahashi create
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_aih_nop5( VOID *pRcvMsg )
{
	UINT 	a_msgID = 0;
	
	/*	メッセージID取得	*/
	if(pRcvMsg != NULL){
		a_msgID = ((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "STATUS MATRIX ERROR!! sts=0x%08x, evt=0x%08x", f_aih_thrdstate, a_msgID );
	return;
}

/*!
 *  @brief  nop function
 *  @note   This function log ALD timeout & assert log set.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/08/01 FJT)Takahashi create
 *  @warning	N/A
 *  @FeatureID	5GDU-M-002
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_aih_nop6( VOID *pRcvMsg )
{
	UINT 	a_msgID = 0;
	
	if(pRcvMsg != NULL){
		a_msgID = ((T_SYS_COMMON_THDIF*)pRcvMsg)->uiEventNo;
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "STATUS MATRIX ERROR!! sts=0x%08x, evt=0x%08x", f_aih_thrdstate, a_msgID );
	return;
}

/* @} */

