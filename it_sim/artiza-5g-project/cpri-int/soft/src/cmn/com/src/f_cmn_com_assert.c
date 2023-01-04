/*!
 * @skip  $ld:$
 * @file  f_cmn_com_abort.c
 * @brief abort処理
 * @date  2013/11/26 ALPHA) 近藤 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_cmn_com_inc.h"

/*!
 * @brief 関数機能概要:abort処理
 * @note  関数処理内容.
 *       -# Assert取得
 *       -# ソフトALM要因の設定
 *       -# プロセス固有情報出力
 *       -# シグナル送信
 * @param  processID     [in]UINT         プロセス識別子
 * @param  almCode       [in]UINT         ALM要因
 * @param  fileName      [in]VOID const*  ファイル名
 * @param  lineNo        [in]UINT         行番号
 * @return VOID
 * @retval none
 * @warning   N/A
 * @FeatureID N/A
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2013/12/12 ffcs)niemah
 */
VOID f_cmn_com_assert(UINT	logLevel , UINT code , const VOID *access , const VOID * fileName, UINT lineNo )
{
	bpf_com_log_assert_real(logLevel, (const CHAR *)fileName, lineNo, "0x%08x,%s", code, (const CHAR *)access);
	return;
}
/* @} */
