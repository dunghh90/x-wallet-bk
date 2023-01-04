/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_TraietfStsGETReq.c
 *  @brief  TRA ietf-hardware GET Request Recieved function
 *  @date   2018/11/20 KCN)Kamimoto create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_EQS
* @{ */

#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/


/* @{ */
/**
* @brief TRA ietf-hardware GET Request Recieved function
* @note TRA ietf-hardware GET Request factor judge,table data read.\n
* @param w_req_id [in] Request ID 
*        w_payloadPtr[in] the buffer address pointer of received message
* @return None
* @date 2018/11/20 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_TraietfStsGETReq( UINT w_req_id , VOID* w_payloadPtr )
{
	/* 変数宣言 */
	T_MPSW_MSI_IETF_HARDWARE_STATE_GET_CFM		sndMsg;							/* 送信メッセージ(ietf- hardware State GET要求(OK応答))	*/
	unsigned int								sv_sts0;						/* SV状態#0 */

	/* last-changed設定 */
	memcpy(&(sndMsg.msi_ietf_state_t.last_changed) , &f_eqsw_last_chg , sizeof( sndMsg.msi_ietf_state_t.last_changed ) );

	/* SV-Bus状態#0の読出し */
	sv_sts0 = f_eqsw_tra_sv_newsta.sv_inf[D_SYS_NUM0] ;

	/* admin状態の確認 */
	/* Admin状態の設定 */
	switch( sv_sts0 & D_DU_SVCTL_BIT_ADM_ALL )
	{
		case D_DU_SVCTL_BIT_ADM_LOCKED :										/* admin状態がLock状態なら */
		case D_DU_SVCTL_BIT_ADM_LOCKED_LO :										/* admin状態がLock状態(local)なら */
			sndMsg.msi_ietf_state_t.admin_state = E_MPSW_ADMIN_STATE_LOCKED ;
			break;
		case D_DU_SVCTL_BIT_ADM_DOWN:											/* admin状態がshutting-downなら */
			sndMsg.msi_ietf_state_t.admin_state = E_MPSW_ADMIN_STATE_SHUTTINGDOWN ;
			break;
		case D_DU_SVCTL_BIT_ADM_UNLOCKED:										/* admin状態がunlockedなら */
		case D_DU_SVCTL_BIT_ADM_UNLOCKED_LO:									/* admin状態がunlocked(local)なら */
			sndMsg.msi_ietf_state_t.admin_state = E_MPSW_ADMIN_STATE_UNLOCKED ;
			break;
		default:																/* admin状態がその他なら */
			sndMsg.msi_ietf_state_t.admin_state = E_MPSW_ADMIN_STATE_UNKNOWN ;
			break;
	}

	/* Operation State状態の設定 */
	switch( sv_sts0 & D_DU_SVCTL_BIT_OPE_ALL )
	{
		case D_DU_SVCTL_BIT_OPE_ENABLE:											/* Operational状態がイネーブルなら */
			sndMsg.msi_ietf_state_t.oper_state  = E_MPSW_OPER_STATE_ENABLED ;
			break;
		case D_DU_SVCTL_BIT_OPE_DISABLE:										/* Operational状態がディセーブルなら */
			sndMsg.msi_ietf_state_t.oper_state  = E_MPSW_OPER_STATE_DISABLED ;
			break;
		default:																/* Operational状態がその他なら */
			sndMsg.msi_ietf_state_t.oper_state  = E_MPSW_OPER_STATE_UNKNOWN ;
			break;
	}

	/* usage状態の設定 */
	switch ( sv_sts0 & D_DU_SVCTL_BIT_USA_ALL )
	{
		case D_DU_SVCTL_BIT_USA_BUSY:											/* usage状態がビジーなら */
			sndMsg.msi_ietf_state_t.usage_state = E_MPSW_USAGE_STATE_BUSY ;
			break;
		case D_DU_SVCTL_BIT_USA_ACTIVE:											/* usage状態がアクティブなら */
			sndMsg.msi_ietf_state_t.usage_state = E_MPSW_USAGE_STATE_ACTIVE ;
			break;
		case D_DU_SVCTL_BIT_USA_IDLE:											/* usage状態がアイドルなら */
			sndMsg.msi_ietf_state_t.usage_state = E_MPSW_USAGE_STATE_IDLE ;
			break;
		default:																/* usage状態がその他なら */
			sndMsg.msi_ietf_state_t.usage_state = E_MPSW_USAGE_STATE_UNKNOWN ;
			break;
	}

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_IETF_HARDWARE_STATE_GET_CFM ,			/* 送信するメッセージID		*/
						 w_req_id ,											/* リクエストID				*/
						 sizeof(T_MPSW_MSI_IETF_HARDWARE_STATE_GET_CFM) ,	/* 送信するデータサイズ		*/
						 &sndMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EQS ) ;							/* pf_eqsのスレッドID		*/

	return ;
}
/* @} */


