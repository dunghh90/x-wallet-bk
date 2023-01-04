/*!
 * @skip 		$ld:$
 * @file 		f_eom_anlz.c
 * @brief		pf_eomスレッド 受信メッセージ解析処理
 * @date		2018/11/23 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_EOM
 * @{
 */

#include "f_eom_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_eom_anlz
 * @note		受信メッセージ内イベント番号とスレッド状態から処理関数を実行する
 *				-# 受信メッセージ内イベント番号異常又はスレッド状態異常の場合、アボート処理を実施する(f_eom_abort())
 *				-# 受信メッセージ処理異常の場合、Assertログ処理を実施する(f_eom_assert))
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
INT	f_eom_anlz(VOID*	msgP)
{
	INT								rtn = D_SYS_NG;						/* 復帰値				*/
	UINT							evtNo;								/* イベント番号			*/

	/* 受信msgチェック	*/
	switch( ((T_SYS_COMMON_THDIF *)msgP)->uiEventNo ) {
		case	D_SYS_MSGID_MSI_RCV_IND:								/* MSI受信通知			*/
			evtNo = E_EOM_EVT_MSI_RCV_IND;
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
	if( f_eomw_thdInfo.thdState >= E_EOM_EVT_MAX ) {

		/* Assertログ出力	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 					/* Assertレベル			*/
							"Thread Status Error, sta=%d",				/* message				*/
							f_eomw_thdInfo.thdState );					/* Information			*/

		/* Abort処理 : Thread状態異常	*/
		f_com_abort( D_RRH_PROCID_PF, D_DU_ALMCD_TKST );
		return	rtn;													/* 異常のため処理終了	*/
	}

	/* マトリクス関数コール	*/
	rtn = (*(f_eomr_callSetRell_mtxtbl[f_eomw_thdInfo.thdState][evtNo].eventProcess_addr))(msgP);

	/* マトリクス関数処理結果チェック	*/
	if( rtn != D_SYS_OK ) {

		/* Assertログ出力	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 					/* Assertレベル			*/
							"Mtrx Func Result Error, rtn=%d, sta=%d, evt=%d", 	/* message		*/
							rtn, f_eomw_thdInfo.thdState, evtNo );		/* Information			*/
	}
	return	rtn;
}

/* @} */
