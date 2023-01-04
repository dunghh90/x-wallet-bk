/*!
 * @skip		$ld:$
 * @file		f_lgc_Idle_MakeALMTroublelog.c
 * @brief		ログスレッド 運用状態_障害ログ出力要求処理
 * @author		ALPHA)yokoyama
 * @date		2013/10/29 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LGC_LGC
 * @{
 */

#include "f_lgc_inc.h"

/*!
 * @brief		関数機能概要:障害ログ出力要求受信時処理を行う
 * @note		関数処理内容.
 *				-# 障害ログ出力関数をCALLする
 * @param		*msgP			[in]	受信メッセージ
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/10/29 ALPHA)横山 Create
 */
INT f_lgc_Idle_MakeALMTroublelog(VOID* msgP)
{
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_lgc_Idle_MakeALMTroublelog] ENTER" );
	
	/* 種別は中で判定する	*/
	f_lgc_makeTroubleLog( ((T_SYS_TROUBLE_LOG_OUTPUT_REQ *)msgP)->trouble_kind );
	
	cmn_com_assert_th_get(f_lgcw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_log_Idle_MakeALMTroublelgc] RETURN" );
	
	return D_RRH_OK;
}

/* @} */