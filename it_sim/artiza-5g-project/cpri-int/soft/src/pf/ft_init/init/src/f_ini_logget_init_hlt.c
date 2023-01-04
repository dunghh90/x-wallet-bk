/*!
 * @skip  $ld:$
 * @file  f_ini_logget_init_hlt.c
 * @brief SIGUSR1受信時処理
 * @date  2014/02/01 ALPHA) yokoyama Create RRH-007-000 初期化
 * @date  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/07/28 TDIPS) maruyama Create(PFからの移動）
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_INI
 * @{
 */

#include "f_ini_inc.h"
#include    "BPF_COM_END.h"
#include <sys/wait.h> 
/*!
 * @brief	f_ini_logget_init_hlt
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
 * @param   sigInfo     [in]    signal information
 * @param   buf_p       [out]   log buffer
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2014/02/01 ALPHA) yokoyama Create
 * @date  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 */
INT f_ini_logget_init_hlt(siginfo_t *sigInfo , VOID* buf_p)
{
	UINT				sigState;		/*	シグナル種別ステータス	*/
	INT					count;			/*	ループカウンタ			*/
	UINT	uiResetFlg			= D_INI_SIG_RESET_OFF;
	UINT	uiResetFlg_define	= D_INI_SIG_RESET_OFF;
	
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_ENTER, "[f_ini_logget_init_hlt] ENTER" );
	
	sigState = E_INI_SIGNAL_SWERR;
	
	/************************************************************/
	/* Disable処理												*/
	/************************************************************/
// 擬似環境 20210316
//#ifndef SIM_DEBUG
//	f_ini_disable(sigState , buf_p);
//#endif	
// 擬似環境 20210316
	/************************************************************/
	/* 障害ログの保存処理										*/
	/************************************************************/
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
		BPF_RM_SVRM_SIGNAL_SEND( procIdTbl->info[count].aplProcessID, SIGUSR1, D_RRH_SIGUSR_OPT_HLTCHK_REQ);
		
		/* 受信は受けても受けなくとも問題ない	*/
	}
	
	/* 障害ログを取得する場合は状態遷移し、自分自身にSignalを送信して処理を続ける	*/
	/* signal受信をしたらAssertなどの共通情報を取得する								*/
	f_iniw_sig_info_hlt.state = 1;
	f_iniw_sig_info_hlt.logcount = 0;
	
	/* RE種別に応じてRESET ON/OFFを抽出する。*/

	uiResetFlg			=	f_inir_SigResetMng_tdd[sigState].reset_onoff;
	uiResetFlg_define 	=	f_inir_SigResetMng_tdd[sigState].reset_onoff;

	/* 内部テーブルに情報を設定する	*/
	f_iniw_sig_info_hlt.addInfo				= D_RRH_SIGUSR_OPT_RSET_SWERR_NTC;
	f_iniw_sig_info_hlt.sigstate			= sigState;
	f_iniw_sig_info_hlt.logType				= E_INI_LOGTYPE_HLTCHK;
	f_iniw_sig_info_hlt.trouble_logget		= D_RRH_ON;
	f_iniw_sig_info_hlt.uiCompelResetFlg	= D_INI_COMPEL_RESET_OFF;
	f_iniw_sig_info_hlt.resetReason			= D_RRH_RST_RSN_SWRST1;
	f_iniw_sig_info_hlt.uiResetFlg			= uiResetFlg;
	f_iniw_sig_info_hlt.uiResetFlg_define	= uiResetFlg_define;
	
	/* INITプロセスへSignal送信(次処理へ進め)	*/
	BPF_RM_SVRM_SIGNAL_SEND( D_RRH_PROCID_INI, SIGUSR1, D_RRH_SIGUSR_OPT_LOG_OUT_NEXT_HLT);
	
	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_RETURN, "[f_ini_logget_init_hlt] RETURN" );

	return D_RRH_OK;
}

/* @} */
