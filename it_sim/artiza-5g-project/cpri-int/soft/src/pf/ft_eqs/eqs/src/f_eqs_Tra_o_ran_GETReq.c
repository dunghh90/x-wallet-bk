/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_eqs_Tra_o_ran_GETReq.c
 *  @brief  TRA O-RAN-hardware GET Request Recieved function
 *  @date   2018/11/19 KCN)Kamimoto create for 5GDU-LLS
 *  @date   2019/06/13 KCN)Fujita O-RAN対応
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018-2019
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_EQS
* @{ */
#include <ctype.h>
#include "f_eqs_header.h"			/* RE監視タスクヘッダファイル			*/


/* @{ */
/**
* @brief TRA O-RAN-hardware GET Request Recieved function
* @note TRA O-RAN-hardware GET Request factor judge,table data read.\n
* @param w_req_id [in] Request ID 
*        w_payloadPtr[in] the buffer address pointer of received message
* @return None
* @date 2018/11/26 KCN)Kamimoto 5GDU-LLS対応
* @date 2019/06/13 KCN)Fujita O-RAN対応
* @warning	N/A
* @FeatureID	RRH-011-011
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_eqs_Tra_o_ran_GETReq( UINT w_req_id , VOID* w_payloadPtr )
{
	/* 変数宣言 */
	T_MPSW_MSI_O_RAN_HARDWARE_GET_CFM				sndMsg;							/* 送信メッセージ(ietf-hardware GET要求(OK応答))	*/
	unsigned int									sv_sts0;						/* SV状態#0 */

	/* 読出し情報エリアの初期化 */
	memset(&sndMsg , D_RRH_ZERO , sizeof(T_MPSW_MSI_O_RAN_HARDWARE_GET_CFM));

	/* product_codeの読み出し */
	strncpy( sndMsg.product_code, f_com_product_code, D_SYS_NUM16);


	/* SV-Bus状態#0の読出し */
	sv_sts0 = f_eqsw_tra_sv_newsta.sv_inf[D_SYS_NUM0] ;

	/* availability状態の設定 */
	switch( sv_sts0 & D_DU_SVCTL_BIT_AVL_ALL )
	{
		case D_DU_SVCTL_BIT_AVL_FAULTY :									/* availability状態がfaulty状態なら */
			sndMsg.availability_state = E_AVAILABILITY_TYPE_FAULTY ;
			break;
		case D_DU_SVCTL_BIT_AVL_DEGRADED:									/* availability状態がdegradedなら */
			sndMsg.availability_state = E_AVAILABILITY_TYPE_DEGRADED ;
			break;
		case D_DU_SVCTL_BIT_AVL_NORMAL:										/* availability状態がnormalなら */
			sndMsg.availability_state = E_AVAILABILITY_TYPE_NORMAL ;
			break;
		default:															/* availability状態がその他なら */
			sndMsg.availability_state = E_AVAILABILITY_TYPE_UNKNOWN ;
			break;
	}

	/* Power状態の設定 */
	switch( sv_sts0 & D_DU_SVCTL_BIT_ENG_ALL )
	{
		case D_DU_SVCTL_BIT_ENG_SLEEP :										/* Power状態がsleep状態なら */
			sndMsg.power_state = E_HARD_POW_STAT_SLEEPING ;
			break;
		case D_DU_SVCTL_BIT_ENG_AWAKE:										/* Power状態がawakeなら */
			sndMsg.power_state = E_HARD_POW_STAT_AWAKE ;
			break;
		default:															/* Power状態がその他なら */
			sndMsg.power_state = E_HARD_POW_STAT_UNKNOWN ;
			break;
	}

	/* 情報のダンプ */
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "product code= %s",	sndMsg.product_code );
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "availability= %d", sndMsg.availability_state ) ;

	/* MSI送信処理 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_HARDWARE_GET_CFM ,					/* 送信するメッセージID		*/
						 w_req_id ,											/* リクエストID				*/
						 sizeof(T_MPSW_MSI_O_RAN_HARDWARE_GET_CFM) ,			/* 送信するデータサイズ		*/
						 &sndMsg ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,									/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_EQS ) ;							/* pf_eqsのスレッドID		*/
	return ;
}
/* @} */


