/**
 *  @skip	$Id:$
 *  @file	f_aih_aldPowNotify.c
 *  @brief  5GDU-LLS ALD power on complete notification function
 *  @date   2019/10/07 FJT)Usui create
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */

/** @addtogroup RRH_PF_AIH
* @{ */

#include "f_aih_inc.h"			/* ALD(AISG)タスクヘッダファイル		*/

/**
* @brief 5GDU-LLS ALD power on complete notification function
* @note  ALD(AISG) interface function.\n
* @param  *pRcvMsg   [in]    received message buffer
* @return VOID
* @retval none
* @date 2019/10/07 FJT)Usui create
* @warning	N/A
* @FeatureID	5GDU-M-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_aih_aldPow_notification( UINT dc_enable_status_val )
{
	T_MPSW_MSI_O_RAN_ALD_PORT_DC_ENABLED_STATUS_CHANGE_NTF_IND sndMsg_ind;
	
	/* 送信データエリアを初期化 */
	memset(&sndMsg_ind, D_RRH_ZERO ,sizeof(sndMsg_ind));
	
	/* notification送信 */
	sndMsg_ind.ald_port_id = E_AIH_PORT_ID;
	sndMsg_ind.dc_enable_status_val =  dc_enable_status_val;

	/* MSI送信 */
	(VOID)f_com_MSIsend(D_MSG_MSI_O_RAN_ALD_PORT_DC_ENABLED_STATUS_CHANGE_NTF_IND ,			/* 送信するメッセージID		*/
						0x00000000 ,														/* リクエストID				*/
						sizeof(sndMsg_ind) ,												/* 送信するデータサイズ		*/
						&sndMsg_ind ,														/* 送信データのポインタ		*/
						D_RRH_PROCQUE_PF ,													/* PFのプロセスID			*/
						D_SYS_THDQID_PF_AIH ) ;												/* pf_aihのスレッドID		*/

	f_com_mtCtl_aisgCtl(dc_enable_status_val);
	
	return ;
}


/* @} */

