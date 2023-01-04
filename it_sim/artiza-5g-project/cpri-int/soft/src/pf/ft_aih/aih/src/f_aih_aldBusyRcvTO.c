/**
 *  @skip	$Id:$
 *  @file	f_aih_aldBusyRcvTO.c
 *  @brief  5GDU-LLS ALD send busy and receive timeout function
 *  @date   2019/08/07 FJT)Takahashi create
 *  @date   2019/09/13 FJT)Takahashi modify IT2 issue
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */

/** @addtogroup RRH_PF_AIH
* @{ */

#include "f_aih_inc.h"			/* ALD(AISG)�^�X�N�w�b�_�t�@�C��		*/

/**
* @brief 5GDU-LLS ALD send busy and receive timeout function
* @note  ALD(AISG) interface function.\n
* @param  *pRcvMsg   [in]    received message buffer
* @return VOID
* @retval none
* @date 2019/08/07 FJT)Takahashi create
* @date 2019/09/13 FJT)Takahashi modify IT2 issue
* @warning	N/A
* @FeatureID	5GDU-M-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/

VOID f_aih_aldBusyRcvTO( VOID *pRcvMsg )
{
	T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM		cfmMsg;
	T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ		rejMsg;

	/* ��M�����`�F�b�N����(���[�h�N���A) */
	UINT   uiRcvCnt;
	UINT   uiRcvErrCnt;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFNM,&uiRcvCnt);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFDL,&uiRcvErrCnt);

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ALD receive timeout by send busy(RXASFNM:0x%x,RXASFDL:0x%x)",uiRcvCnt,uiRcvErrCnt );

	/* ��M��flag�� ON�̏ꍇ: err�v�� CRC-NG set���AMSI����(CFM) */
	/*             OFF�̏ꍇ: ���M��busy�Ƃ���MSI����(REJ) */
	if( f_aih_aldCtrlLogp->rcvstartFlg == E_AIH_FLG_RCVON ) {

		f_com_dumpMsgAisgSend("RX ERROR(4): frames-with-wrong-crc:(ON) frames-without-stop-flag:(OFF)");

		/* ���M�f�[�^�G���A�������� */
		memset(&cfmMsg, D_RRH_ZERO ,sizeof(T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM));
		/* confirm(CRC NG) MSG���M */
		cfmMsg.ald_port_id = E_AIH_PORT_ID;
		cfmMsg.ald_error_wrong_crc = E_AIH_IF_CRCNG;
		(VOID)f_com_MSIsend(
						D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM,		/* ���M���郁�b�Z�[�WID */
						f_aih_aldCtrlLogp->last_requestId,				/* ���N�G�X�gID         */
						sizeof(cfmMsg),									/* ���M����f�[�^�T�C�Y */
						&cfmMsg,										/* ���M�f�[�^�̃|�C���^ */
						D_RRH_PROCQUE_PF,								/* PF�̃v���Z�XID       */
						D_SYS_THDQID_PF_AIH);							/* pf_aih�̃X���b�hID   */

		UCHAR	error_type = 1;

		f_aih_aldMsgLog(D_DU_AIH_ALD_CTRL_DIR_TYPE_ERR,sizeof(error_type),&error_type);
		
	}
	else{
		/* ALD busy */
		memset(&rejMsg, D_RRH_ZERO ,sizeof(T_MPSW_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ));
		rejMsg.ald_port_id = E_AIH_PORT_ID;
		rejMsg.msgErr.errorCode = 0;
		strcpy( &rejMsg.msgErr.errorDescription[0] , "ALD rcv timeout by send busy." ) ;
		rejMsg.status = E_AIH_IF_SENDING;
		(VOID)f_com_MSIsend(
						D_MSG_MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ ,	/* ���M���郁�b�Z�[�WID	*/
						f_aih_aldCtrlLogp->last_requestId,			/* ���N�G�X�gID			*/
						sizeof(rejMsg),								/* ���M����f�[�^�T�C�Y	*/
						&rejMsg,									/* ���M�f�[�^�̃|�C���^	*/
						D_RRH_PROCQUE_PF ,							/* PF�̃v���Z�XID		*/
						D_SYS_THDQID_PF_AIH ) ;						/* pf_aih�̃X���b�hID	*/
	}

	f_aih_initCtl(D_RRH_OFF);

	f_aih_thrdstate = E_AIH_STAT_IDLE; /* Update task state to IDLE state */

	return ;
}

/* @} */

