/*!
 * @skip		$ld:$
 * @file		f_log_Idle_NtcTroublelog.c
 * @brief		ログスレッド 通常状態_運用ログ出力完了通知受信処理
 * @author		ALPHA)yokoyama
 * @date		2013/10/29 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2014
 */

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

#include "f_log_inc.h"

/*!
 * @brief		関数機能概要:運用ログ出力完了通知受信時処理を行う
 * @note		関数処理内容.
 *				-# /var/log/sendlog配下をクリアする
 * @param		*msgP			[in]	受信メッセージ
 * @retval		D_RRH_OK		0:正常終了
 * @retval		D_RRH_NG		1:異常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-003-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/04 ALPHA)横山 Create
 * @date		2014/12/23 ALPHA)藤井 modify for ppcEOL
 */
INT f_log_Idle_NtcTroublelog(VOID* msgP)
{
	INT										ret;
	UINT									sysKind;
	CHAR									a_shcmd[64];

	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_log_Idle_NtcTroublelog] ENTER" );
	
	sysKind = ((T_SYS_RUN_LOG_OUTPUT_NTC *)msgP)->head.uiSignalkind;
	
	/* 中止された状態になったので/var/log/sendlog配下をクリアする */
	/****************************************/
	/* /var/log/sendlog配下をクリアする		*/
	/****************************************/
	memset( a_shcmd, 0x00, sizeof(a_shcmd) );
	snprintf( a_shcmd, sizeof(a_shcmd), "rm -rf %s/* > /dev/null 2>&1", f_comr_runLogPath[sysKind] );
	ret = system(a_shcmd);

	if( D_RRH_MNG == ret )
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "rm failed." );
	}

	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_log_Idle_NtcTroublelog] RETURN(%d)", sysKind );
	return D_RRH_OK;
}

/* @} */
