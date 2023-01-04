/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_aih_msiAldPwrCtrl.c
 *  @brief	ALD power on/off control function 
 *  @date   2019/08/02 FJ)Takahashi create
 *  @date   2019/09/04 FJT)Takahashi modify by IT2 issue.
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
* @brief      ALD電源制御処理
* @note       MSI通知で指定された電源ONOFF設定値をAISG電源制御レジスタに設定する.\n
* @param      req         [in] the buffer address pointer of received message
* @param      w_requestId [in] the ID of MSI requested number
* @return     none
* @date       2019/08/02  FJT)Takahashi create
*  @date      2019/09/04 FJT)Takahashi modify by IT2 issue.
* @warning    N/A
* @FeatureID  5GDU-M-002
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*
*/
void f_aih_msiAldPwrCtrl( VOID *pRcvMsg )
{
	T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_REJ	errMsg;
	T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_REQ	*reqMsg;
	T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_CFM	cfmMsg;
	AIH_PWRCTRL_IF	pwrCtrl;
	UINT	portid;

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
	
	/* parameter check. PORT-IDが正常値(=0)なら、ALD電源制御を実行 */
	reqMsg = (T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_REQ *)w_payloadPtr;
	portid = reqMsg->ald_port_id;

	if( portid == E_AIH_PORT_ID )
	{
		if((reqMsg->power_control == E_AIH_IF_ALDON) || (reqMsg->power_control == E_AIH_IF_ALDOFF)){
			cfmMsg.ald_port_id = portid;
			/* confirm MSG送信 */
			(VOID)f_com_MSIsend(
							D_MSG_MSI_O_RAN_ALD_PORT_EDIT_CFM,	/* 送信するメッセージID */
							w_requestId,						/* リクエストID         */
							sizeof(cfmMsg),						/* 送信するデータサイズ */
							&cfmMsg,							/* 送信データのポインタ */
							D_RRH_PROCQUE_PF,					/* PFのプロセスID       */
							D_SYS_THDQID_PF_AIH);				/* pf_aihのスレッドID   */
		}

		switch ( reqMsg->power_control ){
			case E_AIH_IF_ALDON:
				/* AISG電源OFF場合 */
				if((f_aih_aldCtrlLogp->powerState & E_AIH_CTRL_ALDON) == 0){
					(VOID)BPF_HM_DEVC_GETTIME(&(f_aih_aldCtrlLogp->powerDate));  /* Set System time */

					f_aih_aldCtrlLogp->powerState = E_AIH_CTRL_ALDON;

					(void)BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_AISG_ALDPWR, E_AIH_CTRL_ALDON);

					f_com_taskDelay(100,0,0,0);	/* 保護時間 */

					(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_AISG_ALDPWR, &pwrCtrl.regDat);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "ALD-port:dc-enabled [ FALSE to TRUE ]:Val:0x%08x",pwrCtrl.regDat);
					f_com_dumpMsgAisgSend("ALD-port:dc-enabled [ FALSE to TRUE ]");

					f_aih_aldPow_notification(E_AIH_IF_ALDON);

				}else{
					(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_AISG_ALDPWR, &pwrCtrl.regDat);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ALD-port:dc-enabled [ TRUE to TRUE ]:Val:0x%08x",pwrCtrl.regDat);
				}
				break;
			case E_AIH_IF_ALDOFF:
				/* AISG電源ON場合 */
				if((f_aih_aldCtrlLogp->powerState & E_AIH_CTRL_ALDON) != 0){
					(VOID)BPF_HM_DEVC_GETTIME(&(f_aih_aldCtrlLogp->powerDate));  /* Set System time */

					f_aih_aldCtrlLogp->powerState = E_AIH_CTRL_ALDOFF;

					(void)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_AISG_ALDPWR, E_AIH_CTRL_ALDON);

					f_com_taskDelay(100,0,0,0);	/* 保護時間 */

					/* がっつり消す */
					f_aih_initCtl(D_RRH_OFF);

					f_aih_aldCtrlLogp->devScanCount = 0;
					memset(f_aih_aldCtrlLogp->devValid,0,sizeof(f_aih_aldCtrlLogp->devValid));
					memset(f_aih_aldCtrlLogp->devName,0,sizeof(f_aih_aldCtrlLogp->devName));
					memset(f_aih_aldCtrlLogp->devAddrSnd,0,sizeof(f_aih_aldCtrlLogp->devAddrSnd));
					memset(f_aih_aldCtrlLogp->devAddrRcv,0,sizeof(f_aih_aldCtrlLogp->devAddrRcv));
					memset(f_aih_aldCtrlLogp->devTdiffRR,0,sizeof(f_aih_aldCtrlLogp->devTdiffRR));
					memset(f_aih_aldCtrlLogp->devTdiffCR,0,sizeof(f_aih_aldCtrlLogp->devTdiffCR));

					(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_AISG_ALDPWR, &pwrCtrl.regDat);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "ALD-port:dc-enabled [ TRUE to FALSE ]:Val:0x%08x",pwrCtrl.regDat);
					f_com_dumpMsgAisgSend("ALD-port:dc-enabled [ TRUE to FALSE ]");

					f_aih_aldPow_notification(E_AIH_IF_ALDOFF);

				}else{
					(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_AISG_ALDPWR, &pwrCtrl.regDat);
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "ALD-port:dc-enabled [ FALSE to FALSE ]:Val:0x%08x",pwrCtrl.regDat);
				}

				break;
			default:
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_aih_oran_PWR-CTRL failed. %d", reqMsg->power_control);

				memset(&errMsg, D_RRH_ZERO ,sizeof(T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_REJ));
				errMsg.ald_port_id = portid;
				errMsg.msgErr.errorCode = 0;
				strcpy( &errMsg.msgErr.errorDescription[0] , "invalid pwr_ctrl value." ) ;
				(VOID)f_com_MSIsend(
								D_MSG_MSI_O_RAN_ALD_PORT_EDIT_REJ ,				/* 送信するメッセージID	*/
								w_requestId ,									/* リクエストID			*/
								sizeof(errMsg) ,								/* 送信するデータサイズ	*/
								&errMsg ,										/* 送信データのポインタ	*/
								D_RRH_PROCQUE_PF ,								/* PFのプロセスID		*/
								D_SYS_THDQID_PF_AIH ) ;							/* pf_aihのスレッドID	*/
				break;
		}
	}
	else{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_aih_oran_PORT-ID failed. %d", portid);
		
		memset(&errMsg, D_RRH_ZERO ,sizeof(T_MPSW_MSI_O_RAN_ALD_PORT_EDIT_REJ));
		errMsg.ald_port_id = portid;
		errMsg.msgErr.errorCode = 0;
		strcpy( &errMsg.msgErr.errorDescription[0] , "invalid port id." ) ;
		(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_ALD_PORT_EDIT_REJ ,		/* 送信するメッセージID	*/
						w_requestId ,									/* リクエストID			*/
						sizeof(errMsg),									/* 送信するデータサイズ	*/
					 	&errMsg ,										/* 送信データのポインタ	*/
					 	D_RRH_PROCQUE_PF ,								/* PFのプロセスID		*/
					 	D_SYS_THDQID_PF_AIH ) ;							/* pf_aihのスレッドID	*/
	}

	return ;
}
/* @} */

