/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_Tra_o_ran_ResetReq.c
 *  @brief  TRA O-RAN-RESET Request Recieved function
 *  @date   2019/02/13 KCN)Kamimoto create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup 5GDU_PF_EQS
* @{ */
#include <ctype.h>
#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/


/**
* @brief        o-ran-operations edit request recieved function
* @note         set re-call-home-no-ssh-timer to eeprom
* @param        w_req_id [in] Request ID 
*               w_payloadPtr[in] the buffer address pointer of received message
* @return       None
* @date         2019/06/20 FJT)Yoshida 5GDU-LLS対応
* @warning      N/A
* @FeatureID    5GDU-002-004
* @Bug_No       N/A
* @CR_No        N/A
* @TBD_No       N/A
*
*/
VOID f_eqs_o_ran_operations_editReq( UINT w_req_id , VOID* w_payloadPtr )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REQ	*reqMsg;
	UCHAR									eepval;
	int										ret;

	reqMsg = (T_MPSW_MSI_O_RAN_OPERATIONS_EDIT_REQ *)w_payloadPtr;

	/* EEPROM書替え要否チェック	*/
	if(f_cmw_tra_inv_tbl->cmx_callhome_retry_timer != reqMsg->callhome_retry_timer) {

		eepval = (UCHAR)((reqMsg->callhome_retry_timer & 0xFF00) >> 8);
		ret  = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_CALLHOME_SSH_TIMER_H, &eepval);

		eepval = (UCHAR)(reqMsg->callhome_retry_timer & 0x00FF);
		ret |= BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_CALLHOME_SSH_TIMER_L, &eepval);

		if(ret != BPF_HM_DEVC_COMPLETE){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "EEPROM write error");
		}
		f_cmw_tra_inv_tbl->cmx_callhome_retry_timer = reqMsg->callhome_retry_timer;
	}
	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_OPERATIONS_EDIT_CFM,				/* 送信するメッセージID		*/
						 w_req_id ,											/* リクエストID				*/
						 0,													/* 送信するデータサイズ		*/
						 0,													/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EQS ) ;							/* pf_eqsのスレッドID		*/
	return ;
}
/* @} */


