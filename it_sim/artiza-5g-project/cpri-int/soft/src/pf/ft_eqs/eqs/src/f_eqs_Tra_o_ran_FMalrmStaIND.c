/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_Tra_o_ran_FMalrmStaIND.c
 *  @brief  TRA O-RAN-FM Alarm Start Ind Recieved function
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
* @brief TRA O-RAN-FM Alarm Start Ind Recieved function
* @note TRA O-RAN-FM Alarm Start Ind factor judge,table data read.\n
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
VOID f_eqs_Tra_o_ran_FMalrmStaIND( UINT w_req_id , VOID* w_payloadPtr )
{
	/* 変数宣言 */

	/* プロセス */

	/* Notification通知開始状態のチェック */
	if( f_eqs_alm_start_data == D_EQS_NTF_START_INIT )
	{
		/* Notification通知開始状態を運用中に設定 */
		f_eqs_alm_start_data = D_EQS_NTF_START_RUN ;
		
		/* FM Notification */
		f_eqs_faultChg( f_eqsw_tra_sv_newsta.sv_inf );
	}
	else
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FM Start IND Done(%d)", f_eqs_alm_start_data );
	}
	return ;
}
/* @} */


