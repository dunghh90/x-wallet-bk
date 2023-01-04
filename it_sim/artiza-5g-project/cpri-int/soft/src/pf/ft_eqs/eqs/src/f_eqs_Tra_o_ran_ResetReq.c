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
* @brief TRA O-RAN-Reset Request Recieved function
* @note TRA O-RAN-Reset Request factor judge,table data read.\n
* @param w_req_id [in] Request ID 
*        w_payloadPtr[in] the buffer address pointer of received message
* @return None
* @date 2019/02/13 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_Tra_o_ran_ResetReq( UINT w_req_id , VOID* w_payloadPtr )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_OPERATIONS_RESET_IND *		msgPtr;
	INT		rtn_bpf = 0;
	UCHAR	eep_data;

	/* 受信バッファのアドレス型変換 */
	msgPtr = ( T_MPSW_MSI_O_RAN_OPERATIONS_RESET_IND * )w_payloadPtr ;

	/* リセット情報の分析 */
	if( msgPtr->restart_cause == E_RESTART_CAUSE_MPLANE_TRIGGERED_RESTART )
	{
		/* リセット契機がCUからのrpc(Reset)コマンドなら */
		/* フェールセーフABカウンタクリア */
		eep_data = 0;
		rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SFP_FAILSAFE_AB_CNT, &eep_data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
		{
			/* エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "EEP-ROM Write Error(%04x) rtn_bpf=%08x", D_DU_EEP_SFP_FAILSAFE_AB_CNT, rtn_bpf);
		}
		f_cmw_tra_inv_tbl->cmx_sfp_failsafe_ab_cnt = 0xFF;/* 誤検出を防止するため33以上の値(0xFF)とする */
		/* フェールセーフABCDカウンタクリア */
		eep_data = 0;
		rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SFP_FAILSAFE_ABCD_CNT, &eep_data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
		{
			/* エラー */
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "EEP-ROM Write Error(%04x) rtn_bpf=%08x", D_DU_EEP_SFP_FAILSAFE_ABCD_CNT, rtn_bpf);
		}
		f_cmw_tra_inv_tbl->cmx_sfp_failsafe_abcd_cnt = 0xFF;/* 誤検出を防止するため9以上の値(0xFF)とする */

		/* INITプロセスへSignal送信 */
		BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI , SIGUSR1 , D_RRH_SUGUSR_OPT_RESET_CU_RST_NTC) ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "send SIGNAL of rpc Reset(0x%x)",msgPtr->restart_cause);
	}
	else if(msgPtr->restart_cause == E_RESTART_CAUSE_SUPERVISION_WATCHDOG )
	{
		/* リセット契機がsupervision wathcdog timerタイムアウトなら */
		/* INITプロセスへSignal送信 */
		BPF_RM_SVRM_SIGNAL_SEND(D_RRH_PROCID_INI , SIGUSR1 , D_RRH_SUGUSR_OPT_RESET_WDT_RST_NTC) ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "send SIGNAL of wdt Reset(0x%x)",msgPtr->restart_cause);
	}
	return ;
}
/* @} */


