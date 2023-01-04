/*!
 * @skip  $ld:$
* @file  f_rfcp_prccelmt_EDIT.c(base: f_rfcp_act_carrierOn.c)
 * @brief pf_rfcpスレッド Processing Element EDIT要求受信処理
 * @date  2018/11/22 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_rfcp_inc.h"
#include "f_sys_type.h"
#include "BPF_COM_CMD.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_rfcp_prccelmt_EDIT
 * @note		Processing Element EDIT要求受信時の処理を行う
 *				-# パラメータチェック実施
 *				-# FPGAにVlan ID/C-Plane MACアドレスを設定
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/11/22 KCN)上本 Create
 */
/****************************************************************************************************************************/
int f_rfcp_prccelmt_EDIT(void* msgP)
{
	UINT						w_messageId;
	UINT						w_requestId;
	INT							w_payloadSizePtr;
	T_RFCP_PRCCEDT_REG *		w_payloadPtr;
	unsigned int				w_macaddr ;
	
	UINT remote_mac_addr_h;
	UINT remote_mac_addr_l;

    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_rfcp_prccelmt_EDIT called");

	/* MSIデータ取得 */
	/************************************************************/
	/* Get MSI Data Function									*/
	/************************************************************/
	f_com_MSIgetPayload ( msgP ,						/* [in]  Receive Message pointer			*/
						  &w_messageId ,				/* [out] Receive Message ID					*/
						  &w_requestId ,				/* [out] Receive Message Request ID			*/
						  &w_payloadSizePtr ,			/* [out] Receive Message payload size		*/
						  (VOID *)&w_payloadPtr ) ;		/* [out] Receive Message payload Pointer	*/

	/* 設定先の抽出 */
	if( w_payloadPtr->port == D_RFCP_PRCCEDT_PORT0 )
	{
		/* ポート番号 0のU-MACアドレス設定(MSB) */
		w_macaddr = D_RFCP_PREMT_MAC_ADDR_INIT ;
		w_macaddr = (unsigned int)w_payloadPtr->o_du_mac_address[0] << 8 ;
		w_macaddr = w_macaddr | (unsigned int)w_payloadPtr->o_du_mac_address[1] ;
		(VOID)BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE ,							/* [in] ログ種別(レジスタライト)	*/
									 M_DU_REG_ETH_PORT0(D_DU_REG_ETH_CAPDA11) ,			/* [in] アドレス(0xA1000480)		*/
									 &w_macaddr ) ;										/* [in] MACアドレスの設定 			*/


		remote_mac_addr_h = w_macaddr;

		/* ポート番号 0のU-MACアドレス設定(LSB) */
		w_macaddr = D_RFCP_PREMT_MAC_ADDR_INIT ;
		w_macaddr = (unsigned int)w_payloadPtr->o_du_mac_address[2] << 24 ;
		w_macaddr = w_macaddr | (unsigned int)w_payloadPtr->o_du_mac_address[3] << 16 ;
		w_macaddr = w_macaddr | (unsigned int)w_payloadPtr->o_du_mac_address[4] << 8 ;
		w_macaddr = w_macaddr | (unsigned int)w_payloadPtr->o_du_mac_address[5] ;
		(VOID)BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE ,							/* [in] ログ種別(レジスタライト)	*/
									 M_DU_REG_ETH_PORT0(D_DU_REG_ETH_CAPDA12) ,			/* [in] アドレス(0xA1000484)		*/
									 &w_macaddr ) ;										/* [in] MACアドレスの設定 			*/

		remote_mac_addr_l = w_macaddr;

		BPF_COM_CMD_SET_REMOTEMAC(w_payloadPtr->port ,remote_mac_addr_h ,remote_mac_addr_l );
	}
	else
	{
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "f_rfcp_prccelmt_EDIT not supported port(0x%x) ",w_payloadPtr->port);
	}

	/* MSI送信関数(OK応答)の実行 */
	(VOID)f_com_MSIsend( D_MSG_MSI_O_RAN_PROCESSING_ELEMENT_EDIT_CFM ,	/* 送信するメッセージID		*/
						 w_requestId ,									/* リクエストID				*/
						 D_RFCP_IPCNF_NON_SEND_MSG_LNGH ,				/* 送信するデータサイズ		*/
						 NULL ,											/* 送信データのポインタ		*/
						 D_RRH_PROCQUE_PF ,								/* PFのプロセスID			*/
						 D_SYS_THDQID_PF_RFA ) ;						/* pf_rfaのスレッドID		*/
    return D_SYS_OK;
}

/* @} */
