/*!
 * @skip  $ld:$
* @file  f_rfcp_upcnf_RXEDIT.c(base: f_rfcp_upcnf_TXEDIT.c)
 * @brief pf_rfcpスレッドu-plane conf. RX-EDIT要求受信処理
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

char	t_upcnf_rxedit_errmsg[32]  = "failed get process msg addr." ;
char	t_upcnf_rxedit_errmsg1[32] = "failed send message." ;

/****************************************************************************************************************************/
/*!
 * @brief		f_rfcp_upcnf_RXEDIT
 * @note		u-plane conf. RX-EDIT要求受信時の処理を行う
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
int f_rfcp_upcnf_RXEDIT(void* msgP)
{
	INT									ret;
	INT									errcd;
	UINT								w_messageId;
	UINT								w_requestId;
	INT									w_payloadSizePtr;
	T_RFCP_UPCNF_RXEDIT_REQ *			w_payloadPtr;
	T_RFP_MPL_RX_ON_SETTING_REQ*		OnsndMsg;
	T_RFP_MPL_RX_OFF_SETTING_REQ*		OffsndMsg;
	T_RFP_MPL_RX_SLEEP_SETTING_REQ*		SleepsndMsg;
	T_RFCP_UPCNF_RXEDIT_REJ				errMsg;

    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_rfcp_upcnf_RXEDIT called");

	memset(&errMsg , D_RRH_ZERO , sizeof(T_RFCP_UPCNF_RXEDIT_REJ));

	/* MSIデータ取得 */
	/************************************************************/
	/* Get MSI Data Function									*/
	/************************************************************/
	f_com_MSIgetPayload ( msgP ,						/* [in]  Receive Message pointer			*/
						  &w_messageId ,				/* [out] Receive Message ID					*/
						  &w_requestId ,				/* [out] Receive Message Request ID			*/
						  &w_payloadSizePtr ,			/* [out] Receive Message payload size		*/
						  (VOID *)&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	/* READY以外からのACTIVEの場合、PF_CCAにRX_EDITの設定を行う */
	if( w_payloadPtr->rx_array_car_t.active == E_UP_CAR_ACTIVE_ACTIVE && 
        f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX] != E_RFCP_UPCNF_STS_READY)
	{
		/* RX-ONなら */
		/* 通信用共有メモリを取得する	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
											sizeof(T_RFP_MPL_RX_ON_SETTING_REQ),	/* Size								*/
											(VOID **)&OnsndMsg,						/* msgP								*/
											&errcd );
		if( ret != D_SYS_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_rfcp_upcnf_RXEDIT failed get process message address. %d", errcd);
			/* MSI送信関数(NG応答) */
			errMsg.msgErr.errorCode = D_RFCP_UPCNF_RSCE_BUSY ;
			memcpy( &errMsg.msgErr.errorDescription , t_upcnf_rxedit_errmsg , D_RFCP_UPCNF_ERR_MSG_SIZE ) ;

			(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ ,	/* 送信するメッセージID		*/
						 w_requestId ,										/* リクエストID				*/
						 sizeof(T_RFCP_UPCNF_RXEDIT_REJ) ,					/* 送信するデータサイズ		*/
						 &errMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_RFA ) ;							/* pf_rfaのスレッドID		*/

			return D_SYS_NG;
		}
		/* 送信MSG作成	*/
		OnsndMsg->thdif.uiEventNo 		= D_SYS_MSGID_RXON_REQ ;			/* RX-ON(M-Plane)要求				*/
		OnsndMsg->thdif.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* 送信先Process Queue ID			*/
		OnsndMsg->thdif.uiDstTQueueID	= D_SYS_THDQID_PF_CCA;				/* 送信先Thread Queue ID			*/
		OnsndMsg->thdif.uiSrcPQueueID	= D_RRH_PROCQUE_PF;					/* 送信元Process Queue ID			*/
		OnsndMsg->thdif.uiSrcTQueueID	= D_SYS_THDQID_PF_RFA;				/* 送信元Thread Queue ID			*/
		OnsndMsg->requestId = w_requestId ;									/* リクエストID						*/

		memcpy( &(OnsndMsg->body) , w_payloadPtr , sizeof( T_RFCP_UPCNF_RXEDIT_REQ ) );

		/* RX-ON(M-Plane)を送信する */
		ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_CCA ,				/* 送信先Thread Queue ID			*/
										(void *)OnsndMsg );					/* 送信Message						*/

		if( ret != D_SYS_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_rfcp_upcnf_RXEDIT failed send message. %d", ret);
			/* MSI送信関数(NG応答) */
			errMsg.msgErr.errorCode = D_RFCP_UPCNF_RSCE_BUSY ;
			memcpy( &errMsg.msgErr.errorDescription , t_upcnf_rxedit_errmsg1 , D_RFCP_UPCNF_ERR_MSG_SIZE ) ;

			(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ ,	/* 送信するメッセージID		*/
						 w_requestId ,										/* リクエストID				*/
						 sizeof(T_RFCP_UPCNF_RXEDIT_REJ) ,					/* 送信するデータサイズ		*/
						 &errMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_RFA ) ;							/* pf_rfaのスレッドID		*/

			return D_SYS_NG;
		}
	}
	else if(w_payloadPtr->rx_array_car_t.active == E_UP_CAR_ACTIVE_INACTIVE)
	{
		/* RX-OFFなら */
        /* stateがREADY以外の場合はREJ送信 */
        if(f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX] == E_RFCP_UPCNF_STS_DISABLE) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_rfcp_upcnf_RXEDIT failed car state=%d", 
                f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX]);
			/* MSI送信関数(NG応答) */
			errMsg.msgErr.errorCode = D_RFCP_UPCNF_RSCE_BUSY ;
			memcpy( &errMsg.msgErr.errorDescription , t_upcnf_rxedit_errmsg1 , D_RFCP_UPCNF_ERR_MSG_SIZE ) ;

			(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ ,	/* 送信するメッセージID		*/
						 w_requestId ,										/* リクエストID				*/
						 sizeof(T_RFCP_UPCNF_RXEDIT_REJ) ,					/* 送信するデータサイズ		*/
						 &errMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_RFA ) ;							/* pf_rfaのスレッドID		*/
			return D_SYS_NG;
        }

		/* 通信用共有メモリを取得する	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
											sizeof(T_RFP_MPL_RX_OFF_SETTING_REQ),	/* Size								*/
											(VOID **)&OffsndMsg,					/* msgP								*/
											&errcd );
		if( ret != D_SYS_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_rfcp_upcnf_RXEDIT failed get process message address. %d", errcd);
			/* MSI送信関数(NG応答) */
			errMsg.msgErr.errorCode = D_RFCP_UPCNF_RSCE_BUSY ;
			memcpy( &errMsg.msgErr.errorDescription , t_upcnf_rxedit_errmsg , D_RFCP_UPCNF_ERR_MSG_SIZE ) ;

			(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ ,	/* 送信するメッセージID		*/
						 w_requestId ,										/* リクエストID				*/
						 sizeof(T_RFCP_UPCNF_RXEDIT_REJ) ,					/* 送信するデータサイズ		*/
						 &errMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_RFA ) ;							/* pf_rfaのスレッドID		*/

			return D_SYS_NG;
		}
		/* 送信MSG作成	*/
		OffsndMsg->thdif.uiEventNo 		= D_SYS_MSGID_RXOFF_REQ ;			/* RX-OFF(M-Plane)要求				*/
		OffsndMsg->thdif.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* 送信先Process Queue ID			*/
		OffsndMsg->thdif.uiDstTQueueID	= D_SYS_THDQID_PF_CCA;				/* 送信先Thread Queue ID			*/
		OffsndMsg->thdif.uiSrcPQueueID	= D_RRH_PROCQUE_PF;					/* 送信元Process Queue ID			*/
		OffsndMsg->thdif.uiSrcTQueueID	= D_SYS_THDQID_PF_RFA;				/* 送信元Thread Queue ID			*/
		OffsndMsg->requestId = w_requestId ;								/* リクエストID						*/

		memcpy( &(OffsndMsg->body) , w_payloadPtr , sizeof( T_RFCP_UPCNF_RXEDIT_REQ ) );

		/* RX-OFF(M-Plane)を送信する */
		ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_CCA ,				/* 送信先Thread Queue ID			*/
										(void *)OffsndMsg );				/* 送信Message						*/

		if( ret != D_SYS_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_rfcp_upcnf_RXEDIT failed send message. %d", ret);
			/* MSI送信関数(NG応答) */
			errMsg.msgErr.errorCode = D_RFCP_UPCNF_RSCE_BUSY ;
			memcpy( &errMsg.msgErr.errorDescription , t_upcnf_rxedit_errmsg1 , D_RFCP_UPCNF_ERR_MSG_SIZE ) ;

			(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ ,	/* 送信するメッセージID		*/
						 w_requestId ,										/* リクエストID				*/
						 sizeof(T_RFCP_UPCNF_RXEDIT_REJ) ,					/* 送信するデータサイズ		*/
						 &errMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_RFA ) ;							/* pf_rfaのスレッドID		*/

			return D_SYS_NG;
		}
	}
    else {
        /* RX-SLEEPの場合はSLEEP制御 or RX-SLEEPからのACTIVEの場合はSLEEP解除制御をする */
        /* stateがREADY以外の場合はREJ送信 */
        if(f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX] != E_RFCP_UPCNF_STS_READY) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_rfcp_upcnf_RXEDIT failed car state=%d", 
                f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX]);
			/* MSI送信関数(NG応答) */
			errMsg.msgErr.errorCode = D_RFCP_UPCNF_RSCE_BUSY ;
			memcpy( &errMsg.msgErr.errorDescription , t_upcnf_rxedit_errmsg1 , D_RFCP_UPCNF_ERR_MSG_SIZE ) ;

			(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ ,	/* 送信するメッセージID		*/
						 w_requestId ,										/* リクエストID				*/
						 sizeof(T_RFCP_UPCNF_RXEDIT_REJ) ,					/* 送信するデータサイズ		*/
						 &errMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_RFA ) ;							/* pf_rfaのスレッドID		*/
			return D_SYS_NG;
        }

		/* 通信用共有メモリを取得する	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
											sizeof(T_RFP_MPL_RX_SLEEP_SETTING_REQ),	/* Size								*/
											(VOID **)&SleepsndMsg,					/* msgP								*/
											&errcd );
		if( ret != D_SYS_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_rfcp_upcnf_RXEDIT failed get process message address. %d", errcd);
			/* MSI送信関数(NG応答) */
			errMsg.msgErr.errorCode = D_RFCP_UPCNF_RSCE_BUSY ;
			memcpy( &errMsg.msgErr.errorDescription , t_upcnf_rxedit_errmsg , D_RFCP_UPCNF_ERR_MSG_SIZE ) ;

			(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_REJ ,	/* 送信するメッセージID		*/
						 w_requestId ,										/* リクエストID				*/
						 sizeof(T_RFCP_UPCNF_RXEDIT_REJ) ,					/* 送信するデータサイズ		*/
						 &errMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_RFA ) ;							/* pf_rfaのスレッドID		*/

			return D_SYS_NG;
		}
		/* 送信MSG作成	*/
		SleepsndMsg->thdif.uiEventNo 		= D_SYS_MSGID_RXSLEEP_REQ ;			/* RX-SLEEP(M-Plane)要求				*/
		SleepsndMsg->thdif.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* 送信先Process Queue ID			*/
		SleepsndMsg->thdif.uiDstTQueueID	= D_SYS_THDQID_PF_CCA;				/* 送信先Thread Queue ID			*/
		SleepsndMsg->thdif.uiSrcPQueueID	= D_RRH_PROCQUE_PF;					/* 送信元Process Queue ID			*/
		SleepsndMsg->thdif.uiSrcTQueueID	= D_SYS_THDQID_PF_RFA;				/* 送信元Thread Queue ID			*/
		SleepsndMsg->requestId = w_requestId ;								/* リクエストID						*/

		memcpy( &(SleepsndMsg->body) , w_payloadPtr , sizeof( T_RFCP_UPCNF_RXEDIT_REQ ) );

		/* RX-SLEEP(M-Plane)を送信する */
		ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_CCA ,				/* 送信先Thread Queue ID			*/
										(void *)SleepsndMsg );				/* 送信Message						*/

    }

	/* MSI送信関数(OK応答)の実行 */
	(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_UPLANE_CONF_EDIT_RX_CFM ,	/* 送信するメッセージID		*/
						w_requestId ,								/* リクエストID				*/
						D_RFCP_IPCNF_NON_SEND_MSG_LNGH ,			/* 送信するデータサイズ		*/
						NULL ,										/* 送信データのポインタ		*/
						D_RRH_PROCQUE_PF ,							/* PFのプロセスID			*/
						D_SYS_THDQID_PF_RFA ) ;						/* pf_rfaのスレッドID		*/
    if((w_payloadPtr->rx_array_car_t.active == E_UP_CAR_ACTIVE_INACTIVE ) || 
        (w_payloadPtr->rx_array_car_t.active == E_UP_CAR_ACTIVE_ACTIVE && 
        f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX] != E_RFCP_UPCNF_STS_READY)) {

		if(f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX] != E_RFCP_UPCNF_STS_BUSY) {
			/* 状態の設定 */
			f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX] = E_RFCP_UPCNF_STS_BUSY ;
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_rfcp_upcnf_RXEDIT BPF_RU_IPCM_MSGQ_SEND send");
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_upcnf_state(ant:%d):(car:%d):RX = %d"
															, w_payloadPtr->rx_array_car_t.ant 
															, w_payloadPtr->rx_array_car_t.car 
															, f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX]);
	
		    /* notification MSG作成 */
	   		T_MPSW_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND StatesndMsg;
	    	StatesndMsg.number = 1;
	    	StatesndMsg.array_carrier_state_t[0].ant = w_payloadPtr->rx_array_car_t.ant;
	    	StatesndMsg.array_carrier_state_t[0].car = w_payloadPtr->rx_array_car_t.car;
		    StatesndMsg.array_carrier_state_t[0].state = f_upcnf_state[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX];
	
	    	/* notification送信 */
	    	(VOID)f_com_MSIsend(
	        	D_MSG_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND,                 /* 送信するメッセージID     */
	        	0,                                                                  /* リクエストID             */
	        	sizeof(T_MPSW_MSI_O_RAN_UPLANE_CONF_RX_STATE_CHANGE_NTF_IND),        /* 送信するデータサイズ     */
	        	&StatesndMsg,                                                       /* 送信データのポインタ     */
	        	D_RRH_PROCQUE_PF,                                                   /* PFのプロセスID           */
	        	D_SYS_THDQID_PF_RFA);                                               /* pf_rfaのスレッドID       */
		}
		
		/* 最新のrequest IDを保持 */
		f_upcnf_latest_msi_reqid[w_payloadPtr->rx_array_car_t.ant][w_payloadPtr->rx_array_car_t.car][D_RFCP_UPCNF_STSRX] = w_requestId;
    }
    return D_SYS_OK;
}

/* @} */
