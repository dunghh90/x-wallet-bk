/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_aih_msiAldPwrGet.c
 *  @brief	ALD power on/off status get function 
 *  @date   2019/09/16 FJ)usui create
 * 
 *  ALL Rights Reserved, Copyright FUJITSU Limited 2019
 */
/*****************************************************************************/

/** @addtogroup RRH_PF_AIH
* @{ */

#include "f_aih_inc.h"
#include "BPF_COM_CMD.h"


/* @{ */
/**
* @brief      ALD電源状態取得処理
* @note       MSI通知に対してALD電源状態を応答する.\n
* @param      req         [in] the buffer address pointer of received message
* @return     none
* @date      2019/09/16 FJT)usui create
* @warning    N/A
* @FeatureID  5GDU-M-002
* @Bug_No     N/A
* @CR_No      N/A
* @TBD_No     N/A
*
*
*/
void f_aih_msiAldPwrGet( VOID *pRcvMsg )
{
	T_MPSW_MSI_O_RAN_ALD_PORT_GET_CFM	sndMsg;							/* 送信メッセージ	*/

	/************************************************************/
	/* Local parameter declaration								*/
	/************************************************************/
	UINT						w_messageId;
	UINT						w_requestId;
	INT							w_payloadSizePtr;
	VOID *						w_payloadPtr;

	/************************************************************/
	/* Get MSI Data Function									*/
	/************************************************************/
	f_com_MSIgetPayload ( pRcvMsg ,				/* [in]  Receive Message pointer			*/
						  &w_messageId ,		/* [out] Receive Message ID					*/
						  &w_requestId ,		/* [out] Receive Message Request ID			*/
						  &w_payloadSizePtr ,	/* [out] Receive Message payload size		*/
						  &w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/
	
	/* 送信データエリアを初期化 */
	memset(&sndMsg, D_RRH_ZERO ,sizeof(T_MPSW_MSI_O_RAN_ALD_PORT_GET_CFM));

	/* ALD電源状態を取得 */
	/* テーブルからALD電源状態を取得 */
	sndMsg.ald_port_id = E_AIH_PORT_ID;   /* port0 */
	if(( f_aih_aldCtrlLogp->powerState & E_AIH_CTRL_ALDON ) != 0 ){
		sndMsg.powerState = E_AIH_IF_ALDON;
	}else{
		sndMsg.powerState = E_AIH_IF_ALDOFF;
	}
	/* confirm MSG送信 */
	(VOID)f_com_MSIsend(
					D_MSG_MSI_O_RAN_ALD_PORT_GET_CFM,	/* 送信するメッセージID */
					w_requestId,						/* リクエストID         */
					sizeof(sndMsg),						/* 送信するデータサイズ */
					&sndMsg,							/* 送信データのポインタ */
					D_RRH_PROCQUE_PF,					/* PFのプロセスID       */
					D_SYS_THDQID_PF_AIH);				/* pf_aihのスレッドID   */
	
	return ;
}
/* @} */

