/*!
 * @skip  $ld:$
 * @file  f_dpda_anlz.c
 * @brief DPDAタスク 受信メッセージ解析処理
 * @date  2018/08/13 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:受信メッセージ内イベント番号とタスク状態から処理関数を実行する
 * @note  関数処理内容.
 *       -# 受信メッセージ内イベント番号異常又はタスク状態異常の場合、アボート処理を実施する(f_dpda_abort())
 *       -# イベント番号とタスク状態をIndexとしたジャンプテーブルより該当処理をCallする
 * @param  *msgP			[in]VOID	受信メッセージ
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/13 KCN)takagi Create
 */
INT f_dpda_anlz(VOID* msgP)
{
	INT					rtn = D_SYS_OK;			/* 復帰値							*/
	T_SYS_COMMON_THDIF*	rcvMsgP;				/* 受信メッセージ					*/
	UINT				evtNo = 0;				/* イベント番号						*/
	
	rcvMsgP = (T_SYS_COMMON_THDIF*)msgP;
	
	/* 受信イベント番号からタスク内イベント番号へ変換 */
	switch( rcvMsgP->uiEventNo )
	{
		case D_SYS_MSGID_SYS_PARAM_SET_REQ:			/* System Parameter Set Request		*/
			evtNo = E_DPDA_EVE_SYS_PARAM_SET_REQUEST;
			break;

		case D_SYS_MSGID_CARRIER_ON_SETTING_REQ:	/* Carrier ON Setting Request		*/
			evtNo = E_DPDA_EVE_CARRIER_ON_SETTING_REQUEST;
			break;

		case D_SYS_MSGID_CARRIER_OFF_SETTING_REQ:	/* Carrier OFF Setting Request		*/
			evtNo = E_DPDA_EVE_CARRIER_OFF_SETTING_REQUEST;
			break;

		case D_SYS_MSGID_TXRXCAL_INUSE_REQ:			/* The request of TxCal RxCal in use		*/
			evtNo = E_DPDA_EVE_TXRXCAL_INUSE_REQUEST;
			break;

		case D_SYS_MSGID_TXRXCAL_TRAINING_REQ:		/* The request of TxCal RxCal Training		*/
			evtNo = E_DPDA_EVE_TXRXCAL_TRAINING_REQUEST;
			break;

		case D_SYS_MSGID_TXON_REQ:					/* TX-ON(M-Plane) Request					*/
			evtNo = E_DPDA_EVE_TXON_REQUEST;
			break;

		case D_SYS_MSGID_RXON_REQ:					/* RX-ON(M-Plane) Request					*/
			evtNo = E_DPDA_EVE_RXON_REQUEST;
			break;

		case D_SYS_MSGID_TXOFF_REQ:					/* TX-OFF(M-Plane) Request					*/
			evtNo = E_DPDA_EVE_TXOFF_REQUEST;
			break;

		case D_SYS_MSGID_RXOFF_REQ:					/* RX-OFF(M-Plane) Request					*/
			evtNo = E_DPDA_EVE_RXOFF_REQUEST;
			break;

		case D_SYS_MSGID_TXSLEEP_REQ:					/* TX-SLEEP(M-Plane) Request					*/
			evtNo = E_DPDA_EVE_TXSLEEP_REQUEST;
			break;

		case D_SYS_MSGID_RXSLEEP_REQ:					/* RX-SLEEP(M-Plane) Request					*/
			evtNo = E_DPDA_EVE_RXSLEEP_REQUEST;
			break;

		case D_SYS_MSGID_TIMOUTNTC:					/* 電力報告周期通知					*/
			evtNo = E_DPDA_EVE_POW_PERIODIC_NOTI;
			break;
		
		case D_SYS_MSGID_TDDEN_CHG_NTC:
			evtNo =	E_DPDA_EVE_TDDEN_CHG_NTC;
			break;

		case D_API_RFP_MNT_POWER_ADJUST_REQ:
			evtNo =	E_DPDA_EVE_MNT_POWER_ADJUST_REQ;
			break;

		case D_SYS_MSGID_BEAMID_SET_REQ:
			evtNo =	E_DPDA_EVE_BEAMID_SET_REQ;
			break;

		default:
			/*	受信 起動要因異常によるAbort	*/
			(VOID)f_dpda_abort(rcvMsgP);
			
			/*	異常のため処理中断	*/
			return rtn;
	}
	
	/*	タスク状態異常の場合、アボート処理	*/
	if(f_dpdaw_info_all->taskInfo.taskState >= D_DPDA_STATENUM)
	{
		/*	タスク状態異常によるAbort	*/
		(VOID)f_dpda_abort(rcvMsgP);
	}
	else
	{
		/* DU種別により、テーブルを読み出す */
		switch(f_dpdaw_reKind)
		{
			/* sub6の場合 */
			case E_DPDA_REKIND_5GDU_SUB6:
				/* マトリクステーブルに状態、起動要因を設定 */
				(VOID)(*(f_dpdar_callSetRell_mtxtbl_sub6[f_dpdaw_info_all->taskInfo.taskState][evtNo].eventProcess_addr))(rcvMsgP);
			break;
		
			/* mmWの場合 */
			case E_DPDA_REKIND_5GDU_MMW:
				/* マトリクステーブルに状態、起動要因を設定 */
				(VOID)(*(f_dpdar_callSetRell_mtxtbl_mmW[f_dpdaw_info_all->taskInfo.taskState][evtNo].eventProcess_addr))(rcvMsgP);
			break;
			
			default:
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Impossible transition route. reKind = %d.", f_dpdaw_reKind );
			break;
		}
	}
	
	return rtn;
}

/* @} */
