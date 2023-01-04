/*!
 * @skip		$ld:$
 * @file		f_main_anlz.c
 * @brief		pf_main 受信メッセージ解析処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/*!
 * @brief		f_main_anlz
 * @note		受信メッセージを解析し、状態に対応する処理を実行.
 *					-# 受信パラメータポインタチェック.
 *						- NULL時はタスク間IFエラーにてアラーム制御(f_com_almReport()).
 *					-# 起動要因範囲チェック.
 *						- NG時：Assertログ取得(BPF_COM_LOG_ASSERT()).
 *						- NG時はタスク間IFエラーにてアラーム制御(f_com_almReport()).
 *					-# スレッド状態チェック.
 *						- NG時：Assertログ取得(BPF_COM_LOG_ASSERT()).
 *						- NG時はタスク状態異常にてアラーム制御(f_com_almReport()).
 *					-# 起動要因とタスク状態により該当関数をCall.
 *					-# 正常終了を戻り値として上位に返却.
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
UINT f_main_anlz(VOID *msgP)
{
	/* 変数宣言	*/
	INT						rtn;
	T_SYS_COMMON_THDIF*		rcvMsgP;				/* 受信メッセージ		*/
	UINT					evtNo;
	
	rtn	= D_SYS_OK;
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_anlz] ENTER" );

	/*	受信MSG NULLチェック	*/
	if(msgP == NULL)
	{
		/* NULL					*/
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Thread IF Message NULL.");
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_anlz] RETURN" );
		f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_TIF);
		return rtn;
	}
	else
	{
		rcvMsgP	= (T_SYS_COMMON_THDIF*)msgP;
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ENTER,
			"EvNo=0x%08x Sigkind=0x%08x DstPQID=0x%08x DstTQID=0x%08x SrcPQID=0x%08x SrcTQID=0x%08x Len=0x%08x KeyID=0x%08x ", 
			rcvMsgP->uiEventNo, rcvMsgP->uiSignalkind, rcvMsgP->uiDstPQueueID, rcvMsgP->uiDstTQueueID, rcvMsgP->uiSrcPQueueID, rcvMsgP->uiSrcTQueueID, rcvMsgP->uiLength, rcvMsgP->uiKeyID);
		
		/* 受信イベント番号からスレッド内イベント番号へ変換 */
		switch( rcvMsgP->uiEventNo )
		{
			case D_SYS_MSGID_INIT_COMP_NTC:		/* 初期化完了通知	*/
				evtNo	= D_MAIN_EVE_INIT_COMP_NTC;
				break;
			
			default:
				/*	Assert Log		*/
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,
					"NG.EvNo=0x%08x Sigkind=0x%08x DstPQID=0x%08x DstTQID=0x%08x SrcPQID=0x%08x SrcTQID=0x%08x Len=0x%08x KeyID=0x%08x ", 
					rcvMsgP->uiEventNo, rcvMsgP->uiSignalkind, rcvMsgP->uiDstPQueueID, rcvMsgP->uiDstTQueueID, rcvMsgP->uiSrcPQueueID, rcvMsgP->uiSrcTQueueID, rcvMsgP->uiLength, rcvMsgP->uiKeyID);
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_anlz] RETURN" );
				return D_SYS_NG;
		}
		
		/* タスク状態異常の場合	*/
		if(f_mainw_thdmng_state >= D_MAIN_STATENUM)
		{
			/*	Assert Log		*/
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Thread State NG State.");
			
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_anlz] RETURN" );
			f_com_abort(D_SYS_THDID_PF_MAIN, D_DU_ALMCD_TKST);
			return D_SYS_NG;
		}
		
		/* 受信メッセージより起動要因取得							*/
		/* 起動要因とタスク状態よりマトリクステーブルから関数起動	*/
		/* マトリクステーブルから起動された関数の戻り値を上位に返却	*/
		(VOID)(*(f_mainr_callSetRell_mtxtbl[f_mainw_thdmng_state][evtNo].eventProcess_addr))(rcvMsgP);
	}

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_anlz] RETURN" );
	return rtn;
}

/* @} */
