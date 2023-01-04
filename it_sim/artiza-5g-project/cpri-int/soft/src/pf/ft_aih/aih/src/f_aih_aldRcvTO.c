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

#include "f_aih_inc.h"			/* ALD(AISG)�^�X�N�w�b�_�t�@�C��		*/

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
	UINT	rxaserrDat;    /* AISG��MERR���read�l   */
	CHAR*				  OnOff_str[2] = {"OFF","ON"};
	UCHAR	error_type = 0;
	UINT	logSuppress = D_RRH_OFF;
	UINT	msg_array;
	
	/* ���M�f�[�^�G���A�������� */
	memset(&sndMsg, D_RRH_ZERO ,sizeof(sndMsg));

	/* ��MERR��� ������ */
	sndMsg.ald_port_id = E_AIH_PORT_ID;

	/* ��MERR���read */
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_AISG_RXASERR, &rxaserrDat);
	if ( ( rxaserrDat & E_AIH_CTRL_CRCERR ) != 0 ){
		/* CRC ERR */
		sndMsg.ald_error_wrong_crc = E_AIH_IF_CRCNG;  /* CRC =NG��set */
		error_type |= 1;
		sndMsg.ald_number_of_received_octets +=4; /* CR-rru20A: add octets of START/STOP-flag & FCS */
	}
	if ( (rxaserrDat & (E_AIH_CTRL_LENERR)) != 0 ){
		/* LENGTH ERR */
		sndMsg.ald_error_stop_flag = E_AIH_IF_ALDSIZENG;  /* stop-flag =NG��set */
		error_type |= 2;
		sndMsg.ald_number_of_received_octets +=3; /* CR-rru20A: add octets of START-flag & FCS */
	}

	/* ��M�����`�F�b�N����(���[�h�N���A) */
	UINT   uiRcvCnt;
	UINT   uiRcvErrCnt;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFNM,&uiRcvCnt);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFDL,&uiRcvErrCnt);

	/* ��M��flag  ON�̏ꍇ:  err���Ȃ����CRC-NG set���AMSI����(CFM) */
	/*             OFF�̏ꍇ: ald��M�Ȃ�(resp-data empty)�Ƃ���MSI����(CFM)  */
	if( f_aih_aldCtrlLogp->rcvstartFlg == E_AIH_FLG_RCVON ){

		/* �f�o�C�X�X�L������ */
		if( (error_type == 0) && (f_aih_aldCtrlLogp->scan.deviceScan == D_RRH_ON) && (f_aih_aldCtrlLogp->scan.f_received_octets > 0) ){

			sndMsg.ald_resp_size = f_aih_aldCtrlLogp->scan.f_received_octets;
			memcpy( (VOID*)&(sndMsg.ald_resp_data[0]), f_aih_aldCtrlLogp->scan.f_received_data, sndMsg.ald_resp_size );
			sndMsg.ald_number_of_received_octets = f_aih_aldCtrlLogp->scan.l_received_octets;
			/* CR-rru20A  Add start  ******************************************************************************/
			/* data�l0x7E,0x7D�́Arecived_octets��2octets��count���� (FPGA��0x7D5E,0x7D5D����7E/7D�ɕϊ����邽��) */
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
			logSuppress = D_DU_AIH_ALD_CTRL_LOG_SUPP_DEVS;	/* �A������ꍇ�́A����Ń��O�����܂�̂ŁA���Ȃ��悤�ɂ���*/

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
			logSuppress = D_DU_AIH_ALD_CTRL_LOG_SUPP_TO;	/* �A������ꍇ�́A����Ń��O�����܂�̂ŁA���Ȃ��悤�ɂ���*/

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
	/* confirm MSG���M */
	(VOID)f_com_MSIsend(
						D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM,	/* ���M���郁�b�Z�[�WID */
						f_aih_aldCtrlLogp->last_requestId,			/* ���N�G�X�gID         */
						sizeof(sndMsg),								/* ���M����f�[�^�T�C�Y */
						&sndMsg,									/* ���M�f�[�^�̃|�C���^ */
						D_RRH_PROCQUE_PF,							/* PF�̃v���Z�XID       */
						D_SYS_THDQID_PF_AIH);						/* pf_aih�̃X���b�hID   */


	f_aih_initCtl(logSuppress);
	f_aih_thrdstate = E_AIH_STAT_IDLE;						/* Change task state to IDLE     */
	return ;
}

/* @} */

