/*!
 * @skip  $ld:$
 * @file  f_ini_AccidentlogGet.c
 * @brief 障害ログ取得
 * @date  2013/12/24 ALPHA) yokoyama Create RRH-007-000 初期化
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include "f_dbg_com_ext.h"
#include "m_cm_def.h"
#include "m_cm_typ.h"


/*!
 * @brief	f_ini_AccidentlogGet
 * @note 	関数処理内容.
 *			-# 障害ログ種別によって出力先を変更する
 *			-# 装置共通ログを出力する
 * @param		trouble kind	[in]	障害種別
 * @param		count			[in]	出力カウンタ
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2013/12/24 ALPHA) yokoyama Create
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/11/20 ALPHA) murakami TDD-RRE(ZYNQ)対応
 */
VOID f_ini_AccidentlogGet( T_INIT_SIG_INFO_TBL* infoTbl, VOID* buf_p )
{
	CHAR				syscmd[256];
	int					len;
	INT					ret;
	
	memset(syscmd,0,sizeof(syscmd));
	len = snprintf( syscmd,sizeof(syscmd),"cmd_alllog > /var/log");
	if( E_INI_LOGTYPE_TROUBLE == infoTbl->logType ){
		if( infoTbl->uiResetFlg_define == D_INI_SIG_RESET_ON ){
			/* RESET有障害の場合	*/
			len += snprintf( &syscmd[len] , sizeof(syscmd) - len ,"/troublelog_rst/cmd_alllog.log");
			ret = system("cp -rf /var/log/core.* /var/log/troublelog_rst/ 1>/dev/null 2> /dev/null");
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_WARNING, "core file copy %d", ret );	/* pgr0539 */
			ret = system("cmd_logfilescpy /var/log/troublelog_rst/ 1>/dev/null 2> /dev/null");
			cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_WARNING, "log file copy %d", ret );	/* pgr0539 */
		}
		else{
			/* 通常障害の場合	*/
			len += snprintf( &syscmd[len] , sizeof(syscmd) - len ,"/troublelog_rst/cmd_alllog_alm.log");
		}
	}
	else if( E_INI_LOGTYPE_HLTCHK == infoTbl->logType ){
		/* ヘルスチェック断の場合	*/
		len += snprintf( &syscmd[len] , sizeof(syscmd) - len ,"/healthchklog/cmd_alllog.log");
		ret = system("cmd_logfilescpy /var/log/healthchklog/ 1>/dev/null 2> /dev/null");
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_WARNING, "log file copy %d", ret );	/* pgr0539 */
	}
	else{
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_WARNING, "Log type error %d", infoTbl->logType );	/* pgr0539 */
		infoTbl->trouble_logget = D_INI_SIG_RESET_OFF;
		return;
	}
	(void)snprintf( &syscmd[len] , sizeof(syscmd) - len ," 2> /dev/null");
	ret = system( syscmd );
	if( D_RRH_MNG == ret ){
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "command failed. %s", syscmd );
		infoTbl->trouble_logget = D_INI_SIG_RESET_OFF;
	}
	return;
}

/* @} */
