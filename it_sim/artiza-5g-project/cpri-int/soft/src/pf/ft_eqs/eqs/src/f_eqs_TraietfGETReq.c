/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_TraietfGETReq.c
 *  @brief  TRA ietf-hardware GET Request Recieved function
 *  @date   2018/11/19 KCN)Kamimoto create for 5GDU-LLS
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_EQS
* @{ */
#include <ctype.h>
#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/

/* @{ */
/**
* @brief TRA ietf-hardware GET Request Recieved function
* @note TRA ietf-hardware GET Request factor judge,table data read.\n
* @param w_req_id [in] Request ID 
*        w_payloadPtr[in] the buffer address pointer of received message
* @return None
* @date 2018/08/23 KCN)Kamimoto 5GDU-LLS対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_TraietfGETReq( UINT w_req_id , VOID* w_payloadPtr )
{
	/* 変数宣言 */
	
	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_IETF_HARDWARE_GET_CFM ,					/* 送信するメッセージID		*/
						 w_req_id ,											/* リクエストID				*/
						 sizeof(T_MPSW_MSI_IETF_HARDWARE_GET_CFM) ,			/* 送信するデータサイズ		*/
						 &f_eqs_ietfHwMsg ,									/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EQS ) ;							/* pf_eqsのスレッドID		*/
	return ;
}
/* @} */


