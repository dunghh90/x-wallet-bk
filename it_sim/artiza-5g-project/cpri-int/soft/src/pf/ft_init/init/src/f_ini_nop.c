/*!
 * @skip  $ld:$
 * @file  f_ini_nop.c
 * @brief nop処理
 * @date  2014/02/01 ALPHA) yokoyama Create PF-Init-001-002-001 初期化
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"

/*!
 * @brief f_ini_nop
 * @note  関数処理内容.
 *		-# 無処理
 * @param  signal info				[in] Signal情報
 * @warning		N/A
 * @FeatureID	PF-Init-001-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/02/01 ALPHA)yokoyama Create
 */
INT f_ini_nop(VOID* msg_p , VOID* buf_p )
{
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_nop] ENTER" );

	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_nop] RETURN" );
	
	return D_RRH_OK;
}

/* @} */
