/*!
 * @skip		$ld:$
 * @file		f_log_Wait_APITroublelogDel.c
 * @brief		ログスレッド 運用ログ出力完了待ち状態_障害ログファイル削除要求受信処理
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
 * @brief		関数機能概要:障害ログファイル削除要求受信時処理を行う
 * @note		関数処理内容.
 *				-# 指定されたファイルを削除する
 * @param		[in]  msgP
 * @retval		D_RRH_OK		0:正常終了
 * @return		INT
 * @warning		N/A
 * @FeatureID	PF-LOG-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/04 ALPHA)横山 Create
 */
INT f_log_Wait_APITroublelogDel(VOID* msgP)
{
	CHAR				a_shcmd[512];
	INT					ret;

	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_log_Wait_APITroublelogDel] ENTER" );
	
	memset( a_shcmd, 0 , sizeof(a_shcmd) );
	/* 指定されるファイルはFullPath指定なのでFileNameをそのままrmする	*/
	snprintf( a_shcmd, sizeof(a_shcmd), "rm -rf %s", (const char *)((T_API_LOG_TROUBLELOG_DEL *)msgP)->info.filename );
	
	ret = system(a_shcmd);
	if( D_RRH_MNG == ret )
	{
		cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_ERROR, "rm failed. %s", a_shcmd );
	}

	cmn_com_assert_th_get(f_logw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_log_Wait_APITroublelogDel] RETURN" );
	
	return D_RRH_OK;
}

/* @} */
