/*!
 * @skip		$ld:$
 * @file		f_lgc_anlz.c
 * @brief		ログスレッド 受信メッセージ解析処理
 * @author		alpha)yokoyama
 * @date		2010/05/24 ALPHA)yokoyama Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010 - 2014
 */

/*!
 * @addtogroup RRH_PF_LGC_LGC
 * @{
 */

#include "f_lgc_inc.h"

/*!
 * @brief		関数機能概要:受信メッセージ内イベント番号とスレッド状態から処理関数を実行する
 * @note		関数処理内容.
 *					-# 受信メッセージ内イベント番号異常又はスレッド状態異常の場合、アボート処理を実施する(f_lgc_abort())
 *					-# イベント番号とスレッド状態をIndexとしたジャンプテーブルより該当処理をCallする
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2010/05/24 ALPHA)横山 Create
 * @date 2013/03/28 ALPHA)松延 B25 対処
 * @date 2013/10/29 ALPHA)横山 docomo ZYNQ化 & 共通化対応
 * @date 2014/12/24 ALPHA)藤井 ppcEOL 対応
 * @date 2018/08/24 KCN)若水 カスケードを削除 sysKindの3G/S3GをSUB6/MMWに変更
 */
INT f_lgc_anlz(VOID* msgP)
{
	INT						rtn = D_RRH_OK;						/* 復帰値												*/
	T_RRH_HEAD*				rcvMsgP;							/* 受信メッセージ										*/
	UINT					evtNo = 0;							/* イベント番号											*/
	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_lgc_anlz] ENTER" );
	
	rcvMsgP = (T_RRH_HEAD*)msgP;
	
	/* 受信イベント番号からスレッド内イベント番号へ変換（Phase毎に修正必要） */
	switch( rcvMsgP->uiEventNo )
	{
	case D_SYS_MSGID_TROUBLELOG_OUT_REQ:				/* 障害ログ出力要求			 */
			evtNo = E_SYS_MSGID_TROUBLELOG_OUT_REQ;
			break;
	case D_SYS_MSGID_MSI_RCV_IND:
			evtNo = E_LGC_EVENT_MSI_REQ;
			break;
	case D_SYS_MSGID_CYCLIC_LOG_SHELTER_TO:				/* ログ情報退避用タイマTO	 */
			evtNo = E_SYS_MSGID_CYCLIC_LOG_SHELTER_TO;
			break;

	default:
		/*	受信 起動要因異常によるnop		*/
		(VOID)f_lgc_nop(rcvMsgP);
		
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "[f_lgc_anlz] error state=%08x", f_lgcw_thdState );
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ERROR, "[f_lgc_anlz] error event=%08x", evtNo );
		
		/*	異常のため処理中断				*/
		return D_RRH_NG;
	}
	
	/*	スレッド状態異常の場合はnop			*/
	if(f_lgcw_thdState >= D_LGC_STATENUM)
	{
		/*	スレッド状態異常によるnop		*/
		(VOID)f_lgc_nop(rcvMsgP);
	}
	else
	{
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_anlz] before state=%08x", f_lgcw_thdState );
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_anlz] event=%08x", evtNo );
		
		/* マトリクステーブルに状態、起動要因を設定 */
		(VOID)(*(f_lgcr_callSetRell_mtxtbl[ f_lgcw_thdState ][evtNo].eventProcess_addr))(rcvMsgP);
		
		cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_lgc_anlz] after state=%08x", f_lgcw_thdState );

	}
	return rtn;
}

/* @} */
