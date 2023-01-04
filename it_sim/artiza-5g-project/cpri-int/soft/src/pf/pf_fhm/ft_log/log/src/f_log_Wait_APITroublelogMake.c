/*!
 * @skip		$ld:$
 * @file		f_log_Wait_APITroublelogMake.c
 * @brief		ログスレッド 運用ログ出力完了待ち状態_障害ログファイル作成要求
 * @author		ALPHA)yokoyama
 * @date		2013/10/29 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

#include "f_log_inc.h"

/*!
 * @brief		関数機能概要:障害ログファイル作成要求受信時処理を行う
 * @note		関数処理内容.
 *				-# API発行元プロセスへ障害ログファイル作成完了通知(NG)を送信する。
 * @param		*msgP			[in]	受信メッセージ
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-003-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/10/29 ALPHA)横山 Create
 */
INT f_log_Wait_APITroublelogMake(VOID* msgP)
{
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_log_Wait_APITroublelogMake] ENTER" );
	
	/* API発行元プロセスへ障害ログファイル作成完了通知を送信する */
	f_log_send_APITroubleLogFinNtc( (( T_API_LOG_TROUBLELOG_GET* )msgP)->head.uiSrcPQueueID,
									(( T_API_LOG_TROUBLELOG_GET* )msgP)->head.uiSrcTQueueID,
									D_RRH_NG,
									0,
									0,
									((T_API_LOG_TROUBLELOG_GET *) msgP)->head.uiSignalkind );
	
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_log_Wait_APITroublelogMake] RETURN" );
	return D_RRH_OK;
}

/* @} */
