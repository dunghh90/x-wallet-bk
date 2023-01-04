/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_Tra_o_ran_ResetGetReq.c
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
* @brief        o-ran-operations get request recieved function
* @note         get reset factor and reset time
* @param        w_req_id [in] Request ID 
*               w_payloadPtr[in] the buffer address pointer of received message
* @return       None
* @date         2019/02/13 KCN)Kamimoto 5GDU-LLS対応
* @warning      N/A
* @FeatureID    5GDU-002-004
* @Bug_No       N/A
* @CR_No        N/A
* @TBD_No       N/A
*
*/
VOID f_eqs_o_ran_operations_getReq( UINT w_req_id , VOID* w_payloadPtr )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM			sndMsg;
	UINT										reset_cause ;

	/* パラメータエリア初期化 */
	memset( &sndMsg , D_RRH_ZERO , sizeof( T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM )) ;


	/* Reset Cause取得処理 */
	(VOID)BPF_COM_LOG_GET_RST_REASON( &reset_cause ) ;

	/* MSI set => reset_cause  */
	switch( reset_cause )
	{
		case	D_RRH_RST_RSN_POWER :
			sndMsg.restart_cause = E_RESTART_CAUSE_POWER_ON ;
			break ;
		case	D_RRH_RST_RSN_REQ :
			sndMsg.restart_cause = E_RESTART_CAUSE_MPLANE_TRIGGERED_RESTART ;
			break ;
		case	D_RRH_RST_RSN_MMI :
			sndMsg.restart_cause = E_RESTART_CAUSE_MPLANE_TRIGGERED_RESTART ;
			break ;
		case	D_RRH_RST_RSN_PROCESS_DEAD :
			sndMsg.restart_cause = E_RESTART_CAUSE_SOFTWARE_FAILURE ;
			break ;
		case	D_RRH_RST_RSN_SFT_FAIL :
			sndMsg.restart_cause = E_RESTART_CAUSE_SOFTWARE_FAILURE ;
			break ;
		case	D_RRH_RST_RSN_SWRST1 :
			sndMsg.restart_cause = E_RESTART_CAUSE_SOFTWARE_FAILURE ;
			break ;
		case	D_RRH_RST_RSN_SWRST2 :
			sndMsg.restart_cause = E_RESTART_CAUSE_SOFTWARE_FAILURE ;
			break ;
		case	D_RRH_RST_RSN_ALMRST :
			sndMsg.restart_cause = E_RESTART_CAUSE_SOFTWARE_FAILURE ;
			break ;
		case	D_RRH_RST_RSN_IMG_FAIL :
			sndMsg.restart_cause = E_RESTART_CAUSE_SOFTWARE_FAILURE ;
			break ;
		case	D_RRH_RST_RSN_HRD_FAIL :
			sndMsg.restart_cause = E_RESTART_CAUSE_SOFTWARE_FAILURE ;
			break ;
		case	D_RRH_RST_RSN_LINK_TIMEOUT :
			sndMsg.restart_cause = E_RESTART_CAUSE_SUPERVISION_WATCHDOG ;
			break ;
		case	D_RRH_RST_RSN_CTLLINK :
			sndMsg.restart_cause = E_RESTART_CAUSE_SUPERVISION_WATCHDOG ;
			break ;
		case	D_RRH_RST_RSN_WDT :
			sndMsg.restart_cause = E_RESTART_CAUSE_OTHER_WATCHDOG_TIMER ;
			break ;
		default :
			sndMsg.restart_cause = E_RESTART_CAUSE_UNKNOWN ;
			break ;
	}
	/* MSI set => reset_time  */
	sndMsg.restart_time.year  = f_comw_operations_restart_time.year;
	sndMsg.restart_time.month = f_comw_operations_restart_time.month;
	sndMsg.restart_time.day   = f_comw_operations_restart_time.day;
	sndMsg.restart_time.hour  = f_comw_operations_restart_time.hour;
	sndMsg.restart_time.min   = f_comw_operations_restart_time.min ;
	sndMsg.restart_time.sec   = f_comw_operations_restart_time.sec;

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_OPERATIONS_GET_CFM ,				/* 送信するメッセージID		*/
						 w_req_id ,											/* リクエストID				*/
						 sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM) ,		/* 送信するデータサイズ		*/
						 &sndMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EQS ) ;							/* pf_eqsのスレッドID		*/

	return ;
}
/* @} */


