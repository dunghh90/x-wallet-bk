/*!
 * @skip  $ld:$
 * @file  f_ini_logget_sigCheck.c
 * @brief SIGUSR1受信時処理
 * @date  2014/02/01 ALPHA) yokoyama Create RRH-007-000 初期化
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include    "BPF_COM_END.h"
#include <sys/wait.h> 

/*!
 * @brief	f_ini_logget_sigCheck
 * @note 	関数処理内容.
 *			-# Signalを解析して処理種別とEvent番号を返す
 * @param  signal info				[in] Signal情報
 * @param  kind						[in] Signal種別
 * @param  event					[in] event番号
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2014/02/01 ALPHA) yokoyama Create
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 */
VOID f_ini_logget_sigCheck( siginfo_t 	*sigInfo,
							UINT		*kind,
							UINT		*eventNo,
							VOID*		buf_p
							)
{
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_logget_sigCheck] ENTER" );
	
	/* M-planeヘルスチェック断	*/
	if( ( SIGUSR1 == sigInfo->si_signo ) && ( D_RRH_SUGUSR_OPT_RESET_WDT_RST_NTC == sigInfo->si_status) ){
		*kind = E_INI_LOGTYPE_HLTCHK;
		*eventNo = 0;
	}
	/* その他=障害発生	*/
	else{
		*kind = E_INI_LOGTYPE_TROUBLE;
		*eventNo = 0;
	}
	
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_logget_sigCheck] RETURN kind=%08x eventNo=%08x", *kind, *eventNo );
	return;
}

/* @} */
