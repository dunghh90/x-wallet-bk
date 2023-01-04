/*!
 * @skip 		$ld:$
 * @file 		f_dhcp_anlz.c
 * @brief		pf_dhcpスレッド 受信メッセージ解析処理
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dhcp_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_dhcp_anlz
 * @note		受信メッセージ内イベント番号とスレッド状態から処理関数を実行する
 *				-# 受信メッセージ内イベント番号異常又はスレッド状態異常の場合、アボート処理を実施する(f_dhcp_abort())
 *				-# 受信メッセージ処理異常の場合、Assertログ処理を実施する(f_dhcp_assert))
 *				-# イベント番号とスレッド状態をIndexとしたジャンプテーブルより該当処理をCallする
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT	f_dhcp_anlz(VOID*	msgP)
{
	INT								rtn = D_SYS_NG;						/* 復帰値				*/
	UINT							evtNo;								/* イベント番号			*/

	/* 受信msgチェック	*/
	switch( ((T_SYS_COMMON_THDIF *)msgP)->uiEventNo ) {
		case	D_API_MSGID_MPN_MPLANE_READY_REQ:						/* M-Plane接続準備要求	*/
			evtNo = D_DHCP_EVT_MPLANE_CNCT_READY;
			break;

		case	D_SYS_MSGID_MSI_RCV_IND:								/* MSI受信通知			*/
			evtNo = D_DHCP_MSGID_MSI_RCV_IND;
			break;

		case	D_SYS_MSGID_NMA_VLAN_WRITE_RSP:							/* serchable-vlans書込み応答	*/
			evtNo = D_DHCP_MSGID_NMA_VLAN_WRITE_RSP;
			break;

		default:														/* 起動要因異常			*/
			/* Assertログ出力	*/
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 				/* Assertレベル			*/
								"Input Event Error, evt=%08x", 			/* message				*/
								((T_SYS_COMMON_THDIF *)msgP)->uiEventNo );	/* Information		*/
			/* Abort処理 : タスク間メッセージ異常	*/
			f_com_abort( D_RRH_PROCID_PF, D_DU_ALMCD_TIF );
			return	rtn;												/* 異常のため処理終了	*/
	}

	/* Thread状態チェック	*/
	if( f_dhcpw_thdInfo.thdState >= D_DHCP_EVENTNUM ) {

		/* Assertログ出力	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 					/* Assertレベル			*/
							"Thread Status Error, sta=%d",				/* message				*/
							f_dhcpw_thdInfo.thdState );					/* Information			*/

		/* Abort処理 : Thread状態異常	*/
		f_com_abort( D_RRH_PROCID_PF, D_DU_ALMCD_TKST );
		return	rtn;													/* 異常のため処理終了	*/
	}

	/* マトリクス関数コール	*/
	rtn = (*(f_dhcpr_callSetRell_mtxtbl[f_dhcpw_thdInfo.thdState][evtNo].eventProcess_addr))(msgP);

	/* マトリクス関数処理結果チェック	*/
	if( rtn != D_SYS_OK ) {

		/* Assertログ出力	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 					/* Assertレベル			*/
							"Mtrx Func Result Error, rtn=%d, sta=%d, evt=%d", 	/* message		*/
							rtn, f_dhcpw_thdInfo.thdState, evtNo );		/* Information			*/
	}
	return	rtn;
}

/* @} */
