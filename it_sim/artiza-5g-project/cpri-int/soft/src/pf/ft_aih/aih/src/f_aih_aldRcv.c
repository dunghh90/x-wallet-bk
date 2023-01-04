/**
 *  @skip	$Id:$
 *  @file	f_aih_aldRcv.c
 *  @brief  5GDU-LLS ALD receive function
 *  @date   2019/08/05 FJT)Takahashi create
 *  @date   2019/09/13 FJT)Takahashi modify IT2 issue
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */

/** @addtogroup RRH_PF_AIH
* @{ */

#include "f_aih_inc.h"			/* ALD(AISG)タスクヘッダファイル		*/

/**
* @brief 5GDU-LLS ALD receive function
* @note  ALD(AISG) interface function.\n
* @param  *pRcvMsg   [in]    received message buffer
* @return VOID
* @retval none
* @date 2019/08/01 FJT)Takahashi create
* @date 2020/08/24 FJT)Takahashi modify CR-20A
* @warning	N/A
* @FeatureID	5GDU-M-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_aih_aldRcv( VOID *pRcvMsg )
{
	T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM	sndMsg;
	INT											endcd = 0;
	INT											errcd;
	T_SYS_AISG_RCV_DAT_NTC *					pAisgRcvNtc;
	UINT										logSuppress = D_RRH_OFF;
	UINT										msg_array;

	pAisgRcvNtc = (T_SYS_AISG_RCV_DAT_NTC *)pRcvMsg;

	/* 受信数もチェックする(リードクリア) */
	UINT   uiRcvCnt;
	UINT   uiRcvErrCnt;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFNM,&uiRcvCnt);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFDL,&uiRcvErrCnt);

	/* デバイススキャン中かつ結果OK？ */
	if( (f_aih_aldCtrlLogp->scan.deviceScan == D_RRH_ON) && (pAisgRcvNtc->uiResult == D_SYS_OK) ){

		f_aih_aldCtrlLogp->scan.l_received_octets += pAisgRcvNtc->uiAllLength;
		f_aih_aldMsgLog(D_DU_AIH_ALD_CTRL_DIR_TYPE_RCV,pAisgRcvNtc->uiLength,(VOID*)pAisgRcvNtc->ucData);

		/* 最初? */
		if(f_aih_aldCtrlLogp->scan.f_received_octets == 0){
			f_aih_aldCtrlLogp->scan.f_received_octets = pAisgRcvNtc->uiLength;
			memcpy( f_aih_aldCtrlLogp->scan.f_received_data,pAisgRcvNtc->ucData,pAisgRcvNtc->uiLength );
			if(pAisgRcvNtc->uiLength == pAisgRcvNtc->uiAllLength){
				/* １フレームのみ受信した場合のみ次フレームもしくはタイムアウトを待つ */
				return;
			}
		}
	}

	endcd = BPF_RU_HRTM_CANCEL( E_RRH_TIMID_AIH_ALDRCV, &errcd ); /* ALD受信待ちタイマ停止 */
	if( endcd != 0 ) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Timer Cancel NG, endcd = %08x, errcd = %08x", endcd, errcd ) ;
	}

	/* 送信データエリアを初期化 */
	memset(&sndMsg, D_RRH_ZERO ,sizeof(sndMsg));
	sndMsg.ald_port_id =  E_AIH_PORT_ID;

	/* 受信ERR有無chk */
	if (D_SYS_OK != pAisgRcvNtc->uiResult)
	{
		UCHAR	error_type = 0;
	
		/* 受信ERR情報read */
		if ( pAisgRcvNtc->usErr[0] != 0 ){
			/* FCS error */
			sndMsg.ald_error_wrong_crc = E_AIH_IF_CRCNG;  /* CRC =NGにset */
			error_type |= 1;
		}
		if ( ( pAisgRcvNtc->usErr[1] | pAisgRcvNtc->usErr[2] | pAisgRcvNtc->usErr[3] ) != 0 ){
			/* SHF or LHF or ABF error */
			sndMsg.ald_error_stop_flag = E_AIH_IF_ALDSIZENG;  /* stop-flag =NGにset */
			error_type |= 2;
		}
		if(sndMsg.ald_error_stop_flag == 0){
			sndMsg.ald_error_wrong_crc = E_AIH_IF_CRCNG;  /* CRC =NGにset */
			error_type |= 1;
		}

		/* CR-rru20A  Add start  case of rcv-err ******************************************************/
		if ( sndMsg.ald_error_stop_flag == E_AIH_IF_ALDSIZENG ){
			sndMsg.ald_number_of_received_octets +=3; /* add rcv_octets of START-flag & FCS */
		}else{
			sndMsg.ald_number_of_received_octets +=4; /* add rcv_octets of START/STOP-flag & FCS */
		}
		/* CR-rru20A  End  ****************************************************************************/
	
		CHAR*				  OnOff_str[2] = {"OFF","ON"};
		f_com_dumpMsgAisgSend("RX ERROR(1): frames-with-wrong-crc:(%s) frames-without-stop-flag:(%s)",
						OnOff_str[sndMsg.ald_error_wrong_crc],
						OnOff_str[sndMsg.ald_error_stop_flag]);
	

		if((f_aih_aldCtrlLogp->logSuppressFlg & D_DU_AIH_ALD_CTRL_LOG_SUPP_ERR) == D_RRH_OFF){
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"RX ERROR: CRC NG:(%s) Too Short Frame(%s) Too Large Frame(%s) ABNormal(%s) RXASFNM:0x%x RXASFDL:0x%x",
						OnOff_str[pAisgRcvNtc->usErr[0]],
						OnOff_str[pAisgRcvNtc->usErr[1]],
						OnOff_str[pAisgRcvNtc->usErr[2]],
						OnOff_str[pAisgRcvNtc->usErr[3]],
						uiRcvCnt,
						uiRcvErrCnt );
		}
	
		f_aih_aldMsgLog(D_DU_AIH_ALD_CTRL_DIR_TYPE_ERR,sizeof(error_type),&error_type);
	
		/* device scan時は無効 */
		if(f_aih_aldCtrlLogp->scan.deviceScan == D_RRH_ON){
			logSuppress = D_DU_AIH_ALD_CTRL_LOG_SUPP_ERR;	/* device scan中はこれでログが埋まるので取らないようにする*/
		}

	}else if(f_aih_aldCtrlLogp->scan.deviceScan == D_RRH_ON){
		sndMsg.ald_resp_size = f_aih_aldCtrlLogp->scan.f_received_octets;
		memcpy( (VOID*)&(sndMsg.ald_resp_data[0]), f_aih_aldCtrlLogp->scan.f_received_data, sndMsg.ald_resp_size );
		sndMsg.ald_number_of_received_octets = f_aih_aldCtrlLogp->scan.l_received_octets;
		/* CR-rru20A  Add start  ******************************************************************************/
		/* data値0x7E,0x7Dは、recived_octetsを2octets分countする (FPGAが0x7D5E,0x7D5Dから7E/7Dに変換するため) */
		for ( msg_array=0; msg_array < (sndMsg.ald_resp_size) ; msg_array++){
			if( (sndMsg.ald_resp_data[msg_array] == 0x7E) || (sndMsg.ald_resp_data[msg_array] == 0x7D) ){
				sndMsg.ald_number_of_received_octets ++;
			}
		}
		sndMsg.ald_number_of_received_octets +=4; /* add octets of START/STOP-flag & FCS  */
		/* CR-rru20A  End  **************************************************************************************/

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"Device Scan Multi-Hit:first:%d all:%d RXASFNM:0x%x RXASFDL:0x%x",
							f_aih_aldCtrlLogp->scan.f_received_octets,
							f_aih_aldCtrlLogp->scan.l_received_octets,
							uiRcvCnt,
							uiRcvErrCnt);

	}else{
		sndMsg.ald_resp_size = pAisgRcvNtc->uiLength;
		memcpy( (VOID*)&(sndMsg.ald_resp_data[0]), pAisgRcvNtc->ucData, sndMsg.ald_resp_size );
		sndMsg.ald_number_of_received_octets = pAisgRcvNtc->uiAllLength;
		/* CR-rru20A  Add start  ******************************************************************************/
		/* data値0x7E,0x7Dは、recived_octetsを2octets分countする (FPGAが0x7D5E, 0x7D5Dを7E, 7Dに変換するため) */
		for ( msg_array=0; msg_array < (sndMsg.ald_resp_size) ; msg_array++){
			if( (sndMsg.ald_resp_data[msg_array] == 0x7E) || (sndMsg.ald_resp_data[msg_array] == 0x7D) ){
				sndMsg.ald_number_of_received_octets ++;
			}
		}
		sndMsg.ald_number_of_received_octets +=4; /* add octets of START/STOP-flag & FCS  */
		/* CR-rru20A  End  **************************************************************************************/
		f_aih_aldMsgLog(D_DU_AIH_ALD_CTRL_DIR_TYPE_RCV,sndMsg.ald_resp_size,(VOID*)&(sndMsg.ald_resp_data[0]));	
	}
	
	(VOID)f_com_MSIsend(
						D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM,	/* 送信するメッセージID */
						f_aih_aldCtrlLogp->last_requestId,			/* リクエストID         */
						sizeof(sndMsg),								/* 送信するデータサイズ */
						&sndMsg,									/* 送信データのポインタ */
						D_RRH_PROCQUE_PF,							/* PFのプロセスID       */
						D_SYS_THDQID_PF_AIH);						/* pf_aihのスレッドID   */


	f_aih_initCtl(logSuppress);
	f_aih_thrdstate = E_AIH_STAT_IDLE;                /* Update task state to IDLE state */

	return ;
}

/* @} */

