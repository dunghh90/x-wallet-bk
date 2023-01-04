/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_TraietfStsEDITReq.c
 *  @brief  TRA ietf-hardware GET Request Recieved function
 *  @date   2019/02/12 KCN)Kamimoto create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup 5GDU_PF_EQS
* @{ */
#include <ctype.h>
#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/


/**
* @brief TRA ietf-hardware EDIT Request Recieved function
* @note TRA ietf-hardware EDIT Request factor judge,table data read.\n
* @param w_req_id [in] Request ID 
*        w_payloadPtr[in] the buffer address pointer of received message
* @return None
* @date 2019/02/12 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_TraietfStsEDITReq( UINT w_req_id , VOID* w_payloadPtr )
{
	/* 変数宣言 */
	T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REQ *		msgPtr;

	/* M-Planeからの要求は? */
	msgPtr = (T_MPSW_MSI_IETF_HARDWARE_STATE_EDIT_REQ *)w_payloadPtr;

	if( msgPtr->admin_state == E_MPSW_ADMIN_STATE_LOCKED )
	{
		/* 状態Lockに変更 */
		f_com_SVCtl( E_DU_SVCTL_OPE_ADM , D_DU_SVCTL_CHG , D_DU_SVCTL_ON_ADM_LOCKED ) ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Admin status set LOCKED = %d", msgPtr->admin_state) ;
	}
	else if( msgPtr->admin_state == E_MPSW_ADMIN_STATE_UNLOCKED )
	{
		/* 状態Unlockに変更 */
		f_com_SVCtl( E_DU_SVCTL_OPE_ADM , D_DU_SVCTL_CHG , D_DU_SVCTL_ON_ADM_UNLOCKED ) ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Admin status set UNLOCKED = %d", msgPtr->admin_state) ;
	}
	else
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Admin status Unknown = %d", msgPtr->admin_state) ;
	}

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_IETF_HARDWARE_STATE_EDIT_CFM ,			/* 送信するメッセージID		*/
						 w_req_id ,											/* リクエストID				*/
						 D_RRH_ZERO ,										/* 送信するデータサイズ		*/
						 NULL ,												/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EQS ) ;							/* pf_eqsのスレッドID		*/
	return ;
}
/* @} */


