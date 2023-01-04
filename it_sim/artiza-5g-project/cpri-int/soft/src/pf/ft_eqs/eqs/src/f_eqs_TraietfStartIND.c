/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_TraietfStartIND.c
 *  @brief  TRA ietf-hardware state change Start Ind Recieved function
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
* @brief TRA ietf-hardware state change Start Ind Recieved function
* @note TRA ietf-hardware state change Start Ind factor judge,table data read.\n
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
VOID f_eqs_TraietfStartIND( UINT w_req_id , VOID* w_payloadPtr )
{

	UINT				wSvInf;
	
	/* プロセス */
	if(f_eqs_status_start_data == D_EQS_NTF_START_INIT)
	{
		/* Notification通知開始状態の設定(開始) */
		f_eqs_status_start_data = D_EQS_NTF_START_RUN ;
		
		/*	admin stateはremoteとLocalのOR値を渡す	*/
		wSvInf = ((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01] & ~D_DU_SVCTL_BIT_ADM_LO_ALL) | 
					((f_eqsw_tra_sv_newsta.sv_inf[E_EQS_SVINF_01] & D_DU_SVCTL_BIT_ADM_LO_ALL)>>24));
		/*	状態変更Notification	*/
		f_eqs_stsChgProc( wSvInf );

		/* DUとの接続済 */
		f_com_mtCtl_con(D_RRH_ON);

	}
	else
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "ietf Start IND Done(%d)", f_eqs_status_start_data );
	}
	return ;
}
/* @} */


