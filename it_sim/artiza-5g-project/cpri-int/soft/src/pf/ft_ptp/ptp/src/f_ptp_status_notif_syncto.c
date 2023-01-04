/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_ptp_status_notif_syncto.c
 *  @brief  pf_ptp Sync-Status Reoprting-Period Timer T.O
 *  @date   2019/02/14 KCN)Kamimoto create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/** @addtogroup 5GDU_PF_EQS
* @{ */
#include <ctype.h>
#include "f_ptp_inc.h"


/**
* @brief Sync-Status Reporting Period Timer Time Out function
* @note Sync-Status Change Notification factor MSI Send..\n
* @param msg_P [in] Message pointer
* @return None
* @date 2019/02/14 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_ptp_status_notif_syncto( VOID* msgP )
{
	UINT					sts_data ;
	T_MPSW_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND	sndMsg;

	/* プロセス */
	/* パラメータエリアの初期化 */
	memset(&sndMsg , D_PTP_ZERO , sizeof(T_MPSW_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND)) ;

	/* SV-Busの読出し */
	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ , D_DU_REG_CTB_SV16INF , &sts_data );

	/* Sync-状態の確認 */
	sts_data = sts_data & D_DU_SVCTL_SYNC_STATE_ALL ;
	if(sts_data != f_sync_status_data)
	{
		/* Sync 状態が前回データと不一致なら */
		if((sts_data & D_DU_SVCTL_SYNC_STATE_FREERUN) == D_DU_SVCTL_SYNC_STATE_FREERUN)
		{
			/* Sync Change Notification(FREERUN)を通知 */
			sndMsg.sync_state = E_SYNC_STATE_FREERUN ;
		}
		else if((sts_data & D_DU_SVCTL_SYNC_STATE_LOCKED) == D_DU_SVCTL_SYNC_STATE_LOCKED)
		{
			/* Sync Change Notification(LOCKED)を通知 */
			sndMsg.sync_state = E_SYNC_STATE_LOCKED ;
		}
		else if((sts_data & D_DU_SVCTL_SYNC_STATE_HOLDOVER) == D_DU_SVCTL_SYNC_STATE_HOLDOVER)
		{
			/* Sync Change Notification(HOLDOVER)を通知 */
			sndMsg.sync_state = E_SYNC_STATE_HOLDOVER ;
		}

		/* MSI送信処理 */
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND ,				/* メッセージID					*/
							D_PTP_REQ_NTF ,												/* リクエストID					*/
							sizeof(T_MPSW_MSI_O_RAN_SYNC_SYNCSTATE_CHANGE_NTF_IND),		/* 送信するデータサイズ			*/
							&sndMsg,													/* 送信データのポインタ			*/
							D_RRH_PROCQUE_PTP,											/* PTPのプロセスID				*/
							D_SYS_THDQID_PTP_MAIN) ;									/* ptp_mainのスレッドID			*/

		/* 通知送信ログ出力	*/
		ZL303XX_TRACE_ALWAYS( "Sync  state change notification : pre = %08x => aft = %08x",
							  f_sync_status_data, sts_data , 0 , 0 , 0 , 0 );
		f_sync_status_data = sts_data ;
	}
	return;
}
/* @} */


