/*!
 * @skip		$ld:$
 * @file		f_log_Wait_MakeALMTroublelog.c
 * @brief		ログスレッド 運用ログ出力完了待ち状態_障害ログ出力要求処理
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
 * @brief		関数機能概要:障害ログ出力要求受信時処理を行う
 * @note		関数処理内容.
 *				-# 障害ログ出力関数をCALLする
 * @param		*msgP			[in]	受信メッセージ
 * @return		INT
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	PF-LOG-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/10/29 ALPHA)横山 Create
 */
INT f_log_Wait_MakeALMTroublelog(VOID* msgP)
{
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_log_Wait_MakeALMTroublelog] ENTER" );
	
	/* 種別は中で判定する	*/
	f_log_makeTroubleLog( ((T_SYS_TROUBLE_LOG_OUTPUT_REQ *)msgP)->trouble_kind );
	
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_log_Wait_MakeALMTroublelog] RETURN" );
	return D_RRH_OK;
}

/* @} */
