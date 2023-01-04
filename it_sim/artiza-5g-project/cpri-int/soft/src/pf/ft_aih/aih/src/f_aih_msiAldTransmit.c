/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_aih_msiAldTransmit.c
 *  @brief	ALD data transmition function 
 *  @date   2019/08/05 FJ)Takahashi create
 *  @date   2019/09/13 FJ)Takahashi modify for IT2
 * 
 *  ALL Rights Reserved, Copyright FUJITSU Limited 2019
 */
/*****************************************************************************/

/** @addtogroup RRH_PF_AIH
* @{ */

#include "f_aih_inc.h"
#include "BPF_COM_CMD.h"

/* @{ */
/**
* @brief      デバイススキャンチェック
* @note       デバイススキャンチェック
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     none
* @date       2019/08/05  FJT)Takahashi create
* @date       2019/09/13  FJ)Takahashi modify for IT2
* @warning    N/A
* @FeatureID  5GDU-M-002
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*
*/
void f_aih_chkDeviceScan(UINT ald_data_len, UCHAR *ald_data )
{
	T_AIH_HDLC_XID_MSG*		hdlc_xid_p;
	T_AIH_PI_PL*			pipl_p;
	UCHAR*					tmp_p;
	UINT					paramlen=0;
	UINT					count;
	UINT					piValid;

	f_aih_aldCtrlLogp->scan.deviceScan = D_RRH_OFF;

	/* XID+PI:1,PI:3の最小レングス分 */
	if( ald_data_len < ( sizeof(T_AIH_HDLC_XID_MSG) + (sizeof(T_AIH_PI_PL) * 2) )){
		return;
	}

	hdlc_xid_p = (T_AIH_HDLC_XID_MSG*)(ald_data);
	/* unicast? */
	if(hdlc_xid_p->addr != D_DU_AIH_ALD_CTRL_ADDR_ALL){
		return;
	}
	/* not XID? */
	if(( hdlc_xid_p->ctl & 0xEF) != 0xAF){
		return;
	}
	/* FI */
	if( hdlc_xid_p->fI != 0x81){
		return;
	}
	/* FI */
	if( hdlc_xid_p->gI != 0xF0 ){
		return;
	}
	/* PI:1,PI:3の最小レングス分 */
	if(hdlc_xid_p->gL < (sizeof(T_AIH_PI_PL) * 2)){
		return;
	}

	paramlen = ald_data_len - sizeof(T_AIH_HDLC_XID_MSG);
	if(paramlen > hdlc_xid_p->gL){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "ALD XID Message length mismatch GL:%u LEN:%u",hdlc_xid_p->gL,paramlen);
		paramlen = hdlc_xid_p->gL;
	}
	/* PIをチェックしdevicescanかどうか調べる */
	tmp_p = (UCHAR*)(ald_data + sizeof(T_AIH_HDLC_XID_MSG));
	piValid = 0;
	for(count=0; count < paramlen ; ){
		pipl_p = (T_AIH_PI_PL*)(tmp_p + count);
		switch(pipl_p->pI){
			case 1:
				piValid |= 1;
				break;
			case 3:
				piValid |= 2;
				break;
			default:
				return;
				break;
		}
		count += sizeof(pipl_p->pI) + sizeof(pipl_p->pL) + pipl_p->pL;	/* PI + PL + PV */
	}

	if(piValid == 0x3){
		memset(&(f_aih_aldCtrlLogp->scan),0,sizeof(f_aih_aldCtrlLogp->scan));
		f_aih_aldCtrlLogp->scan.deviceScan = D_RRH_ON;
		f_aih_aldCtrlLogp->devScanCount++;
	}

	return;
}

/* @} */

/* @{ */
/**
* @brief      ALD転送データ処理
* @note       MSI dataをALDに転送する.\n
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     none
* @date       2019/08/05  FJT)Takahashi create
* @date       2019/09/13  FJ)Takahashi modify for IT2
* @warning    N/A
* @FeatureID  5GDU-M-002
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*
*/
void f_aih_msiAldTransmit( VOID *pRcvMsg )
{
	T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ	errMsg;
	T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ	*reqMsg;
	UINT	portid;
	UINT	aldDatSize;
	INT		endcd = 0;
	INT		errcd;
	UINT	sendResult;
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

	f_aih_aldCtrlLogp->last_requestId = w_requestId;              /* keep request ID, set it to CFM msg */
	
	reqMsg = (T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ *)w_payloadPtr;
	portid = reqMsg->ald_port_id;
	aldDatSize = reqMsg->ald_req_size;

	memset(&errMsg, D_RRH_ZERO ,sizeof(errMsg));
	errMsg.ald_port_id =  portid;
	errMsg.status = E_AIH_IF_NOTSENDING;
	errMsg.msgErr.errorCode = 0;

	/* AISG電源Off */
	if( (f_aih_aldCtrlLogp->powerState & E_AIH_CTRL_ALDON) == 0 ){

		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "dc-enabled-status is false(0x%x).", f_aih_aldCtrlLogp->powerState);

		strcpy( &errMsg.msgErr.errorDescription[0] , "dc-enabled-status is false." ) ;
		(VOID)f_com_MSIsend(
					D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ ,			/* 送信するメッセージID		*/
					w_requestId ,										/* リクエストID				*/
					sizeof(errMsg) ,									/* 送信するデータサイズ		*/
					&errMsg ,											/* 送信データのポインタ		*/
					D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
					D_SYS_THDQID_PF_AIH ) ;								/* pf_aihのスレッドID		*/

		return;
	}

	if(aldDatSize > D_AIH_ALDBLOG_MSG_SIZE)
	{
		/*バッファサイズ超過*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ald msg is too large. size(%d)", aldDatSize);
		
		snprintf( &errMsg.msgErr.errorDescription[0] ,D_AIH_REJ_MSG_SIZE, "msg too large size(%d).",aldDatSize) ;
		(VOID)f_com_MSIsend(
					D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ ,			/* 送信するメッセージID		*/
					w_requestId ,										/* リクエストID				*/
					sizeof(errMsg) ,									/* 送信するデータサイズ		*/
					&errMsg ,											/* 送信データのポインタ		*/
					D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
					D_SYS_THDQID_PF_AIH ) ;								/* pf_aihのスレッドID		*/

		return;
	}
	
	if(aldDatSize < D_AIH_MSG_MIN_SIZE){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ald msg is too short. size(%d)", aldDatSize);

		snprintf( &errMsg.msgErr.errorDescription[0] ,D_AIH_REJ_MSG_SIZE, "msg too short size(%d).",aldDatSize);
		(VOID)f_com_MSIsend(
					D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ ,			/* 送信するメッセージID		*/
					w_requestId ,										/* リクエストID				*/
					sizeof(errMsg),										/* 送信するデータサイズ		*/
					&errMsg ,											/* 送信データのポインタ		*/
					D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
					D_SYS_THDQID_PF_AIH ) ;								/* pf_aihのスレッドID		*/
		
		return;
	}
	
	/* parameter check. PORT-IDが正常値(=0)なら処理実行、異常値ならREJECT */
	if( portid == E_AIH_PORT_ID ){
		sendResult = f_com_bspAISGSnd(	(char*)&(reqMsg->ald_req_data[0]),	/* transmit data addr	*/
							aldDatSize,						/* transmit data size	*/
							D_SYS_AISG_BAUDRATE_9600,		/* BAUDRATE				*/
							D_SYS_AISG_LITTLE_ENDIAN,		/* Little endian		*/
							0);								/* hard timer: not use	*/

		if(sendResult != D_RRH_OK){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ALD data transmission error(size:%d data:0x%02X%02X).", aldDatSize,reqMsg->ald_req_data[0],reqMsg->ald_req_data[1]);
			strcpy( &errMsg.msgErr.errorDescription[0] , "ALD data transmission error." ) ;
			(VOID)f_com_MSIsend(
					D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ ,			/* 送信するメッセージID		*/
					w_requestId ,										/* リクエストID				*/
					sizeof(errMsg),										/* 送信するデータサイズ		*/
					&errMsg ,											/* 送信データのポインタ		*/
					D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
					D_SYS_THDQID_PF_AIH ) ;								/* pf_aihのスレッドID		*/
			return;
		}

		f_aih_chkDeviceScan(aldDatSize , (UCHAR*)&(reqMsg->ald_req_data[0]));
		f_aih_aldCtrlLogp->last_devAddr = (UCHAR)(reqMsg->ald_req_data[0]);
		f_aih_aldMsgLog(D_DU_AIH_ALD_CTRL_DIR_TYPE_SND,aldDatSize,&(reqMsg->ald_req_data[0]));

		/* TS : TS25.462																					*/
		/* The transmission of the response shall be finalised within the time t= n*10*4/datarate			*/
		/* where n is the number of octets in the response frame including all HDLC framing overhead.		*/
		/* The maximum gap time between two consecutive octets shall not exceed the time t= 3*10/datarate.	*/
		/* This corresponds to a 25% utilisation of the Data Link Layer.									*/
		UINT aisg_timeout_val;
		aisg_timeout_val = ((( (( aldDatSize * 4 ) - 3 ) * 10 ) * 1000 ) / 9600 );	/* 受信までの時間 */
		aisg_timeout_val += 20;		/* RET処理時間:10msec + ソフト処理時間(送信時の4msec wait+α) => 20msec */

		/* 最低限の待ち時間(device scanを考慮し、最低400msecは待ち受ける) */
		if( aisg_timeout_val < D_AIH_ALDTOUT_TIMVAL ){
			aisg_timeout_val = D_AIH_ALDTOUT_TIMVAL;
		}

		/* ==================================================================== */
		/* ALD受信待ちtimer起動													*/
		/* ==================================================================== */
		endcd = BPF_RU_HRTM_REGISTER(
								BPF_RU_HRTM_REG_ONCE,			/* Timer種別					*/
								E_RRH_TIMID_AIH_ALDRCV,			/* Timer ID						*/
								aisg_timeout_val,				/* Timer値						*/
								D_SYS_MSGID_AIH_ALD_TIMEOUT,	/* TimeOut時通知Event番号		*/
								D_SYS_THDQID_PF_AIH,			/* TimeOut時送信先Queue番号		*/
								&errcd );
		if(endcd != D_SYS_OK ) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Timer Start NG, endcd = %08x, errcd = %08x", endcd, errcd ) ;
		}

		f_aih_thrdstate  = E_AIH_STAT_ALDSEND;  /* Thread状態遷移(送信中状態) */

	}
	else{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ald portid failed. %d", portid);

		snprintf( &errMsg.msgErr.errorDescription[0] ,D_AIH_REJ_MSG_SIZE, "invalid port id(%d).",portid);
		(VOID)f_com_MSIsend(
					D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ ,			/* 送信するメッセージID		*/
					w_requestId ,										/* リクエストID				*/
					sizeof(errMsg),										/* 送信するデータサイズ		*/
					&errMsg ,											/* 送信データのポインタ		*/
					D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
					D_SYS_THDQID_PF_AIH ) ;								/* pf_aihのスレッドID		*/
	}
	
	return ;
}
/* @} */
