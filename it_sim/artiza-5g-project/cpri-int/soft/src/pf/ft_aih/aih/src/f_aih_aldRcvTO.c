/**
 *  @skip	$Id:$
 *  @file	f_aih_aldRcvTO.c
 *  @brief  5GDU-LLS ALD receive timeout function
 *  @date   2019/08/05 FJT)Takahashi create
 *  @date   2019/09/13 FJT)Takahashi modify IT2 issue
 *  @date   2019/09/19 FJT)Takahashi modify IT2 issue
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */

/** @addtogroup RRH_PF_AIH
* @{ */

#include "f_aih_inc.h"			/* ALD(AISG)タスクヘッダファイル		*/

/**
* @brief 5GDU-LLS ALD receive timeout function
* @note  ALD(AISG) interface function.\n
* @param  *pRcvMsg   [in]    received message buffer
* @return VOID
* @retval none
* @date 2019/08/01 FJT)Takahashi create
* @date 2019/09/13 FJT)Takahashi modify IT2 issue
* @date 2020/08/24 FJT)Takahashi modify CR-20A
* @warning	N/A
* @FeatureID	5GDU-M-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_aih_aldRcvTO( VOID *pRcvMsg )
{
	T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM	sndMsg;
	UINT	rxaserrDat;    /* AISG受信ERR情報read値   */
	CHAR*				  OnOff_str[2] = {"OFF","ON"};
	UCHAR	error_type = 0;
	UINT	logSuppress = D_RRH_OFF;
	UINT	msg_array;
	
	/* 送信データエリアを初期化 */
	memset(&sndMsg, D_RRH_ZERO ,sizeof(sndMsg));

	/* 受信ERR情報 初期化 */
	sndMsg.ald_port_id = E_AIH_PORT_ID;

	/* 受信ERR情報read */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_AISG_RXASERR, &rxaserrDat);
	if ( ( rxaserrDat & E_AIH_CTRL_CRCERR ) != 0 ){
		/* CRC ERR */
		sndMsg.ald_error_wrong_crc = E_AIH_IF_CRCNG;  /* CRC =NGにset */
		error_type |= 1;
		sndMsg.ald_number_of_received_octets +=4; /* CR-rru20A: add octets of START/STOP-flag & FCS */
	}
	if ( (rxaserrDat & (E_AIH_CTRL_LENERR)) != 0 ){
		/* LENGTH ERR */
		sndMsg.ald_error_stop_flag = E_AIH_IF_ALDSIZENG;  /* stop-flag =NGにset */
		error_type |= 2;
		sndMsg.ald_number_of_received_octets +=3; /* CR-rru20A: add octets of START-flag & FCS */
	}

	/* 受信数もチェックする(リードクリア) */
	UINT   uiRcvCnt;
	UINT   uiRcvErrCnt;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFNM,&uiRcvCnt);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFDL,&uiRcvErrCnt);

	/* 受信中flag  ONの場合:  err情報なければCRC-NG setし、MSI応答(CFM) */
	/*             OFFの場合: ald受信なし(resp-data empty)としてMSI応答(CFM)  */
	if( f_aih_aldCtrlLogp->rcvstartFlg == E_AIH_FLG_RCVON ){

		/* デバイススキャン時 */
		if( (error_type == 0) && (f_aih_aldCtrlLogp->scan.deviceScan == D_RRH_ON) && (f_aih_aldCtrlLogp->scan.f_received_octets > 0) ){

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
			sndMsg.ald_number_of_received_octets +=4;	/* add octets of START/STOP-flag & FCS */
			/* CR-rru20A  End  **************************************************************************************/
			if((f_aih_aldCtrlLogp->logSuppressFlg & D_DU_AIH_ALD_CTRL_LOG_SUPP_DEVS) == D_RRH_OFF){
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"Device Scan One-Hit:first:%d all:%d RXASFNM:0x%x RXASFDL:0x%x",
								f_aih_aldCtrlLogp->scan.f_received_octets,
								f_aih_aldCtrlLogp->scan.l_received_octets,
								uiRcvCnt,
								uiRcvErrCnt);
			}
			logSuppress = D_DU_AIH_ALD_CTRL_LOG_SUPP_DEVS;	/* 連続する場合は、これでログが埋まるので、取らないようにする*/

		}else{
			if( error_type == 0 ){
				sndMsg.ald_error_wrong_crc = E_AIH_IF_CRCNG;
				error_type |= 1;
			}
			sndMsg.ald_number_of_received_octets +=4; /* CR-rru20A: add octets of START/STOP-flag & FCS */

			f_com_dumpMsgAisgSend("RX ERROR(2): frames-with-wrong-crc:(%s) frames-without-stop-flag:(%s)",
							OnOff_str[sndMsg.ald_error_wrong_crc],
							OnOff_str[sndMsg.ald_error_stop_flag]);

			f_aih_aldMsgLog(D_DU_AIH_ALD_CTRL_DIR_TYPE_ERR,sizeof(error_type),&error_type);

			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"RX ERROR: CRC NG:(%s) Too Short Frame(%s) Too Large Frame(%s) ABNormal(%s) RXASFNM:0x%x RXASFDL:0x%x DevS:%u Oct:%u Toct:%u",
							OnOff_str[(rxaserrDat & D_AISG_RBIT_ERRMSK_FCS) >> 3],
							OnOff_str[(rxaserrDat & D_DU_AISG_RXASERR_SHF) >> 2],
							OnOff_str[(rxaserrDat & D_DU_AISG_RXASERR_LOF) >> 1],
							OnOff_str[rxaserrDat & D_DU_AISG_RXASERR_ABF],
							uiRcvCnt,uiRcvErrCnt,
							f_aih_aldCtrlLogp->scan.deviceScan,
							f_aih_aldCtrlLogp->scan.f_received_octets,
							f_aih_aldCtrlLogp->scan.l_received_octets);
		}

	}else{

		if(error_type == 0){
			f_com_dumpMsgAisgSend("RX TIMEOUT: NOT received the HDLC frame");

			f_aih_aldMsgLog(D_DU_AIH_ALD_CTRL_DIR_TYPE_TO,sizeof(error_type),&error_type);

			if((f_aih_aldCtrlLogp->logSuppressFlg & D_DU_AIH_ALD_CTRL_LOG_SUPP_TO) == D_RRH_OFF){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "ALD receive timeout(crc:0x%x,stop:0x%x,RXASFNM:0x%x,RXASFDL:0x%x)",
							sndMsg.ald_error_wrong_crc,
							sndMsg.ald_error_stop_flag,
							uiRcvCnt,uiRcvErrCnt);
			}
			logSuppress = D_DU_AIH_ALD_CTRL_LOG_SUPP_TO;	/* 連続する場合は、これでログが埋まるので、取らないようにする*/

		}else{
			f_com_dumpMsgAisgSend("RX ERROR(3): frames-with-wrong-crc:(%s) frames-without-stop-flag:(%s)",
						OnOff_str[sndMsg.ald_error_wrong_crc],
						OnOff_str[sndMsg.ald_error_stop_flag]);

			f_aih_aldMsgLog(D_DU_AIH_ALD_CTRL_DIR_TYPE_ERR,sizeof(error_type),&error_type);

			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "RX ERROR:(crc:0x%x,stop:0x%x,RXASFNM:0x%x,RXASFDL:0x%x)",
						sndMsg.ald_error_wrong_crc,
						sndMsg.ald_error_stop_flag,
						uiRcvCnt,uiRcvErrCnt);

		}
	}
	/* confirm MSG送信 */
	(VOID)f_com_MSIsend(
						D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM,	/* 送信するメッセージID */
						f_aih_aldCtrlLogp->last_requestId,			/* リクエストID         */
						sizeof(sndMsg),								/* 送信するデータサイズ */
						&sndMsg,									/* 送信データのポインタ */
						D_RRH_PROCQUE_PF,							/* PFのプロセスID       */
						D_SYS_THDQID_PF_AIH);						/* pf_aihのスレッドID   */


	f_aih_initCtl(logSuppress);
	f_aih_thrdstate = E_AIH_STAT_IDLE;						/* Change task state to IDLE     */
	return ;
}

/* @} */

