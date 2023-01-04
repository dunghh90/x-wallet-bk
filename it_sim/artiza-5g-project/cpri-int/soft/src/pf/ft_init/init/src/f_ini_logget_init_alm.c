/*!
 * @skip  $ld:$
 * @file  f_ini_logget_init_alm.c
 * @brief SIGUSR1受信時処理
 * @date  2014/02/01 ALPHA) yokoyama Create RRH-007-000 初期化
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include    "BPF_COM_END.h"
#include <sys/wait.h> 


/*!
 * @brief	f_ini_logget_init_alm
 * @note 	関数処理内容.
 *			-# 下記のシグナル受信時に本関数がコールされる
 *				-# ・SIGUSR1/SIGCHLD
 *			-# Disable処理を実施する。
 *			-# ログの保存処理を実施する。
 *			-# アラームヒストリログ保存
 *			-# ソフト障害検知時には、さらにヘルスチェック断かを判定する。
 *			-# 障害通知を受信した以外のプロセスに障害発生シグナルを送信する
 *				-# 受信待ち受け時間は1sec
 *			-# 各プロセスから障害ログファイル出力完了シグナルを受信する
 *				-# 受信したプロセスの受信管理テーブルを更新する
 *				-# 全プロセスから障害ログファイル出力完了シグナルを受信した場合
 *				-# ログ出力要求の各プロセスのへの送信順はL2、MT、L3、PFとする。
 *			-# INITプロセスのプライオリティを変更
 *			-# 障害ログを保存する
 *			-# INITプロセスのプライオリティを元の値に変更する
 *			-# リセットが必要シグナルの場合、リセット処理を実施する。
 * @param  signal info				[in] Signal情報
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2014/02/01 ALPHA) yokoyama Create
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 */
INT f_ini_logget_init_alm(VOID* msg_p , VOID* buf_p)
{
	
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_logget_init_alm] ENTER" );

	if( f_iniw_alreadyRESET == D_RRH_ON )
	{
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_INFO, "[f_ini_logget_init_alm] New ALM is occur. But already RESET is running." );
		return D_RRH_OK;
	}

	T_INIT_SIG_NTC*	sig_ntc			  = msg_p;
	T_INIT_SIG_INFO_TBL*	infoTbl_p = &(sig_ntc->infoTbl);

	f_ini_logget_alm_reset(infoTbl_p  , buf_p );

	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_logget_init_alm] RETURN" );

	return D_RRH_OK;
}

/* @} */
