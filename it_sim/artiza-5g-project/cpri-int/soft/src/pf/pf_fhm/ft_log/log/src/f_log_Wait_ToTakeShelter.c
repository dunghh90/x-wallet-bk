/*!
 * @skip		$ld:$
 * @file		f_log_Wait_ToTakeShelter.c
 * @brief		ログスレッド 運用ログ出力完了待ち状態_ログ情報退避用タイマTO処理
 * @author		ALPHA)yokoyama
 * @date		2013/11/04 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

#include "f_log_inc.h"

/*!
 * @brief		関数機能概要:ログ情報退避用タイマTO受信時処理を行う
 * @note		関数処理内容.
 *				-# ログ退避処理をCallする
 * @param		[in]  msgP
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-002-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/10/29 ALPHA)横山 Create
 */
INT f_log_Wait_ToTakeShelter(VOID* msgP)
{
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_log_Wait_ToTakeShelter] ENTER" );
	
	/* ログ退避処理関数をCallする	*/
	(VOID)f_log_takeShelter();
	
	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_log_Wait_ToTakeShelter] RETURN" );
	return D_RRH_OK;
}

/* @} */
