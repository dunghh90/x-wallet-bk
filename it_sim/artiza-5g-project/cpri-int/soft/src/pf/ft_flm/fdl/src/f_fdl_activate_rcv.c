/*!
 * @skip  $ld:$
 * @file  f_fdl_activate_rcv.c
 * @brief FDLタスク activate処理
 * @date  2019/02/21 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_fdl_inc.h"

/*!
 * @brief 関数機能概要:activate処理を行う。
 * @note  関数処理内容.
 *		-# activate処理を実施。				f_fdl_activate_rcv() 関数Call
 *		-# 固定で正常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @date 2019/02/21 KCN)上本 Create
 */
INT	f_fdl_activate_rcv(VOID* rcvMsg_p)
{
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM			sndMsg_cfm ;
	T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND		sndMsg_ind ;
	
	INT						rtn = D_SYS_OK ;

	UINT					r_messageId;
	UINT					w_requestId;
	INT						w_payloadSizePtr;
	VOID *					w_payloadPtr;
	INT						rtn_bpf = 0;
	UCHAR					eep_data = 0;

	char*					w_slot_name; /* ワークポインタ */

	char					err_eep[32]  = "activate failed" ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_activate_rcv start.") ;

	memset(&sndMsg_cfm , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM)) ;
	memset(&sndMsg_ind , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND)) ;

	/* MSIデータの取得 */
	f_com_MSIgetPayload(rcvMsg_p ,				/* [in]  Receive Message pointer			*/
						&r_messageId ,			/* [out] Receive Message ID					*/
						&w_requestId ,			/* [out] Receive Message Request ID			*/
						&w_payloadSizePtr ,		/* [out] Receive Message payload size		*/
						&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	w_slot_name = ((T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_REQ*)w_payloadPtr)->slot_name;
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Starting Software Activate(%s)...", w_slot_name ) ;

	/* 正常応答 */
	sndMsg_cfm.notification_timeout = D_FDL_NOTIF_TO_ACT;
	/* MSI送信 */
	(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM ,					/* 送信するメッセージID		*/
						w_requestId ,														/* リクエストID				*/
						sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_CFM) ,			/* 送信するデータサイズ		*/
						&sndMsg_cfm ,														/* 送信データのポインタ		*/
						D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
						D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/

	if(strcmp(w_slot_name,f_fdl_Slot_nametbl[D_SYS_NUM0]) == 0){
		eep_data = D_SYS_NUM0;
	}
	else if(strcmp(w_slot_name,f_fdl_Slot_nametbl[D_SYS_NUM1]) == 0){
		eep_data = D_SYS_NUM1;
	}
	else{
		/* エラー処理 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "Input Parameter Error. :slot name %s" , w_slot_name ) ;
		rtn = D_SYS_NG;
	}

	if(rtn == D_SYS_OK){
		/* ステータスのチェック */
		if(f_cmw_tra_inv_tbl->cmx_slot_status[eep_data] == D_DU_EEP_STATUS_VALID)
		{
			/* VALIDなら起動面を切り替える */
			rtn_bpf = BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_START_FLG, &eep_data);
			if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
			{
				/* エラー */
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "EEP-ROM Write Error(%s)", w_slot_name) ;
				rtn = D_SYS_NG ;
			}
		}else {
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR , "SW-Maneger Status Error(%s)", w_slot_name) ;
			rtn = D_SYS_NG ;
		}
	}


	if(rtn == D_SYS_OK){
		/* 正常応答 */
		sndMsg_ind.status = E_SWM_ACTIVATE_STATUS_COMPLETED;
		strcpy(sndMsg_ind.slot_name, w_slot_name);
		/* MSI送信 */
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND ,				/* 送信するメッセージID		*/
							w_requestId ,														/* リクエストID				*/
							sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND) ,		/* 送信するデータサイズ		*/
							&sndMsg_ind ,														/* 送信データのポインタ		*/
							D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
							D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/


		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Software Activate OK(%s)...", w_slot_name ) ;

	}else{
		/* エラー応答 */
		/* 異常応答メッセージの生成 */
		sndMsg_ind.status = E_SWM_ACTIVATE_STATUS_APPLICATION_ERROR;
		strcpy(sndMsg_ind.slot_name, w_slot_name);
		memcpy(&sndMsg_ind.msgErr.errorDescription[0] , &err_eep , sizeof(err_eep));
		/* MSI送信 */
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND ,				/* 送信するメッセージID		*/
							w_requestId ,														/* リクエストID				*/
							sizeof(T_MPSW_MSI_O_RAN_SOFTWARE_MANAGEMENT_ACTIVATE_NTF_IND) ,		/* 送信するデータサイズ		*/
							&sndMsg_ind ,														/* 送信データのポインタ		*/
							D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
							D_SYS_THDQID_PF_FDL ) ;												/* pf_eqsのスレッドID		*/

		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "Software Activate NG(%s)...", w_slot_name ) ;

	}

	/* SV-Busのダウンロード(OFF)の設定を行う */
	f_com_SVCtl( E_DU_SVCTL_OPE_FLDL , D_DU_SVCTL_OFF , D_DU_SVCTL_ON_FL_DWNLD ) ;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_RETURN, "f_fdl_activate_rcv end.") ;
	return rtn ;
}
/* @} */


