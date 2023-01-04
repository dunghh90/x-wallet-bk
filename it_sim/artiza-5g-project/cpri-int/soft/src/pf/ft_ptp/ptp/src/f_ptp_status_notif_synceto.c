/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_ptp_status_notif_synceto.c
 *  @brief  pf_ptp SyncE-Status Reoprting-Period Timer T.O
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
* @brief SyncE-Status Reporting Period Timer Time Out function
* @note SyncE-Status Change Notification factor MSI Send..\n
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
VOID f_ptp_status_notif_synceto( VOID* msgP )
{
	UINT					sts_data ;
	T_MPSW_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND	sndMsg ;

	/* 報告周期分はガードする */
	if(f_syncE_reporting_guarded > D_RRH_ZERO){
		f_syncE_reporting_guarded--;
		if(f_syncE_reporting_guarded > D_RRH_ZERO){
			return;
		}
	}

	/* プロセス */
	/* パラメータエリアの初期化 */
	memset(&sndMsg , D_PTP_ZERO , sizeof(T_MPSW_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND)) ;

	/* SV-Busの読出し */
	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ , D_DU_REG_CTB_SV16INF , &sts_data );

	/* SyncE-Lock状態の確認 */
	sts_data = sts_data & D_DU_SVCTL_SYNCE_STATE_LOCKED ;
	if(sts_data != f_synce_status_data)
	{
		/* SyncE Locked 状態が前回データと不一致なら */
		if(sts_data == D_DU_SVCTL_SYNCE_STATE_LOCKED)
		{
			/* SyncE-State Change Notification(Locked)を通知 */
			sndMsg.lockstate = E_SYNCE_LOCK_STATE_LOCKED ;
		}
		else
		{
			/* SyncE-State Change Notification(Unlocked)を通知 */
			sndMsg.lockstate = E_SYNCE_LOCK_STATE_UNLOCKED ;
		}
		/* MSI送信処理 */
		(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND ,				/* メッセージID					*/
							D_PTP_REQ_NTF ,												/* リクエストID					*/
							sizeof(T_MPSW_MSI_O_RAN_SYNC_SYNCESTATE_CHANGE_NTF_IND),		/* 送信するデータサイズ			*/
							&sndMsg,													/* 送信データのポインタ			*/
							D_RRH_PROCQUE_PTP,											/* PTPのプロセスID				*/
							D_SYS_THDQID_PTP_MAIN) ;									/* ptp_mainのスレッドID			*/
		/* 通知送信ログ出力	*/
		ZL303XX_TRACE_ALWAYS( "SyncE state change notification : pre = %08x => aft = %08x",
							  f_synce_status_data, sts_data , 0 , 0 , 0 , 0 );
		f_synce_status_data = sts_data ;
		f_syncE_reporting_guarded = (UINT)f_syncE_reporting_period;
	}
	return;
}
/* @} */


