/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_Tra_o_ran_StsEditReq.c
 *  @brief  TRA O-RAN-hardware GET Request Recieved function
 *  @date   2019/04/02 FJT)koshida create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup 5GDU_PF_EQS
* @{ */
#include <ctype.h>
#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/


/**
* @brief TRA O-RAN-hardware EDIT Request Recieved function
* @note TRA O-RAN-hardware EDIT Request factor judge,table data read.\n
* @param w_req_id [in] Request ID 
*        w_payloadPtr[in] the buffer address pointer of received message
* @return None
* @date 2019/04/02 FJT)koshida 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_Tra_o_ran_StsEditReq( UINT w_req_id , VOID* w_payloadPtr )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_HARDWARE_EDIT_REQ *		msgPtr;

	/* M-Planeからの要求は? */
	msgPtr = (T_MPSW_MSI_O_RAN_HARDWARE_EDIT_REQ *)w_payloadPtr;

	if( msgPtr->energy_saving_enabled == E_HARD_POW_STAT_SLEEPING )
	{
		/* 状態Sleepに変更 */
		f_com_SVCtl( E_DU_SVCTL_OPE_ENG , D_DU_SVCTL_CHG , D_DU_SVCTL_ON_ENG_SLEEP ) ;
		f_com_SVCtl( E_DU_SVCTL_OPE_FIDF0_ALM , D_DU_SVCTL_ON , D_DU_FLTID_TX_SLEEP ) ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Power status set SLEEP = %d", msgPtr->energy_saving_enabled) ;
	}
	else if( msgPtr->energy_saving_enabled == E_HARD_POW_STAT_AWAKE )
	{
		/* 状態awakeに変更 */
		f_com_SVCtl( E_DU_SVCTL_OPE_ENG , D_DU_SVCTL_CHG , D_DU_SVCTL_ON_ENG_AWAKE ) ;
		f_com_SVCtl( E_DU_SVCTL_OPE_FIDF0_ALM , D_DU_SVCTL_OFF , D_DU_FLTID_TX_SLEEP ) ;
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "Power status set AWAKE = %d", msgPtr->energy_saving_enabled) ;
	}
	else
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Power status Unknown = %d", msgPtr->energy_saving_enabled) ;
	}

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_HARDWARE_EDIT_CFM ,				/* 送信するメッセージID		*/
						 w_req_id ,										/* リクエストID				*/
						 D_RRH_ZERO ,									/* 送信するデータサイズ		*/
						 NULL ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,								/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EQS ) ;						/* pf_eqsのスレッドID		*/
	return ;
}
/* @} */


