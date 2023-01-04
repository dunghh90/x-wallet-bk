/*!
 * @skip  $ld:$
 * @file  f_ini_recieve_SIGUSR2.c
 * @brief 障害ログ処理
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
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
 * @brief	f_ini_recieve_SIGUSR2
 * @note 	関数処理内容.
 *			-# ハード自律タイマ値を最大に設定する
 *			-# 下記のシグナル受信時に本関数がコールされる
 *				-# ・SIGCHLD
 *				-# ・L3信号によるリセット
 *				-# ・RE強制リセット(L1 Inband Reset)
 *				-# ・LM(WDT)による自律リセット
 *				-# ・Local Reset SW ON
 *				-# ・LM(WDT)による自律リセット
 *				-# ・ソフト障害検知
 *			-# アラームヒストリログ保存
 *			-# Disable処理を実施する。
 *			-# ログの保存処理を実施する。
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
 * @date  2013/11/07 ALPHA) nakamura Create
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @data  2015/11/17 ALPHA) murakami Create RRH-006-000 TDD-RRE-ZYNQ対応
 */
INT f_ini_recieve_SIGUSR2(siginfo_t *sigInfo , VOID* buf_p)
{
	T_INIT_SIG_INFO_TBL		infoTbl	;
	UINT					hwAlmFactAry[E_RRH_ALMHIS_ALM_FACTOR_NUM];

	memset(&infoTbl,0,sizeof(infoTbl));

	/* WDT T.O										*/
	switch( sigInfo->si_status){
		case D_RRH_SIGUSR2_OPT_WDTTO:
			BPF_COM_LOG_DMESG("Rcv WDT Time out ...\n");	
	
			BPF_HM_DEVC_REG_READ_BUFFER(0, D_DU_REG_CTB_SV1INF,sizeof(hwAlmFactAry), hwAlmFactAry) ;
			f_com_alarmHitoryLogSet( D_RRH_ALARM_FACTOR_TYPE_SW, D_DU_ALMCD_WDT, hwAlmFactAry);
			
			infoTbl.uiResetFlg_define	=	D_INI_SIG_RESET_ON		;
			infoTbl.resetReason			=	D_RRH_RST_RSN_WDT		;
			infoTbl.logType 			=	E_INI_LOGTYPE_TROUBLE	;
			infoTbl.trouble_logget		=	D_INI_SIG_RESET_ON		;

			f_ini_logget_alm_reset(&infoTbl,buf_p);

			break;
		case D_RRH_SIGUSR2_OPT_CPRIL1RST:
			BPF_COM_LOG_DMESG("Rcv M-plane Link down reset ...\n");

			infoTbl.uiResetFlg_define	=	D_INI_SIG_RESET_ON		;
			infoTbl.resetReason			=	D_RRH_RST_RSN_CTLLINK	;
			infoTbl.logType 			=	E_INI_LOGTYPE_TROUBLE	;
			infoTbl.trouble_logget		=	f_iniw_force_log_save	;

			f_ini_logget_finish( &infoTbl ,buf_p );

			break;
		case D_RRH_SIGUSR2_OPT_RSTCMD:
			BPF_COM_LOG_DMESG("Rcv MMI reset ...\n");

			infoTbl.uiResetFlg_define	=	D_INI_SIG_RESET_ON		;
			infoTbl.resetReason			=	D_RRH_RST_RSN_MMI		;
			infoTbl.logType 			=	E_INI_LOGTYPE_TROUBLE	;
			infoTbl.trouble_logget		=	f_iniw_force_log_save	;

			f_ini_logget_finish( &infoTbl ,buf_p );

			break;
		default:
			BPF_COM_LOG_DMESG("Rcv UNKNOWN reset(0x%x) ...\n", sigInfo->si_status);
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "rcv signal SIGUSR2 other option=0x%08x.", sigInfo->si_status);
			f_ini_reset( D_RRH_RST_RSN_UNKNWN ,0 ,buf_p);
			break;
	}
	return D_RRH_OK;
}

/* @} */
