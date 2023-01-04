/*!
 * @skip  $ld:$
 * @file  f_ini_logget_init_run.c
 * @brief SIGUSR1受信時処理
 * @date  2014/02/01 ALPHA) yokoyama Create RRH-007-000 初期化
 * @date  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/07/28 TDIPS) maruyama Create(PFからの移動）
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_PF_INI
 * @{
 */

#include "f_ini_inc.h"
#include    "BPF_COM_END.h"
#include <sys/wait.h> 

/*!
 * @brief	f_ini_logget_init_run
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
 * @param   sigInfo [in ] Signal情報
 * @param   buf_p   [out] ログ用バッファ
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2014/02/01 ALPHA) yokoyama Create
 * @date  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 */
INT f_ini_logget_init_run(siginfo_t *sigInfo , VOID* buf_p )
{
	INT					count;			/*	ループカウンタ			*/
	INT					send_si_status = D_RRH_SIGUSR_OPT_RUNLOG_GET;
	INT					next_si_status = D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_RUN;
	INT					run_kind = 0;
	
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_logget_init_run] ENTER" );
	
	if(sigInfo->si_status == D_RRH_SIGUSR_OPT_RUNLOG_REQ)
	{
		run_kind = 0;
		send_si_status = D_RRH_SIGUSR_OPT_RUNLOG_GET;
		next_si_status = D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_RUN;
	}
	else if( sigInfo->si_status == D_RRH_SIGUSR_OPT_RUNLOG_REQ_3G )
	{
		run_kind = 1;
		send_si_status = D_RRH_SIGUSR_OPT_RUNLOG_GET_3G;
		next_si_status = D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_RUN_3G;
	}
	else
	{
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ERROR, "signal kind error si_status = %x", sigInfo->si_status );
	}

	/************************************************************/
	/* SIGNAL送信(各プロセスへのログ取得要求)					*/
	/************************************************************/
	for( count = 0; count < procIdTbl->count ; count++ )
	{
		if( D_RRH_PROCID_INI == procIdTbl->info[count].aplProcessID )
		{
			/* INITへは送信しない	*/
			continue;
		}
		
		/* 生存プロセスに障害ログ出力要求シグナルを送信 */
		BPF_RM_SVRM_SIGNAL_SEND( procIdTbl->info[count].aplProcessID, SIGUSR1, send_si_status);
		
		/* 受信は受けても受けなくとも問題ない	*/
	}
	
	/* 障害ログを取得する場合は状態遷移し、自分自身にSignalを送信して処理を続ける	*/
	/* signal受信をしたらAssertなどの共通情報を取得する								*/
	f_iniw_sig_info_run[run_kind].state = 1;
	f_iniw_sig_info_run[run_kind].logcount = 0;
	
	/* 内部テーブルに情報を設定する	*/
	f_iniw_sig_info_run[run_kind].logType = E_INI_LOGTYPE_RUNNING;
	
	/* INITプロセスへSignal送信(次処理へ進め)	*/
	BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_INI, SIGUSR1, next_si_status);
	
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_RETURN, "[f_ini_logget_init_run] RETURN" );

	return D_RRH_OK;
}

/* @} */
