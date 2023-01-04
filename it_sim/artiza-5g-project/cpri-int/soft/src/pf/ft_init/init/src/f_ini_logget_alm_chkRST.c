/*!
 * @skip  $ld:$
 * @file  f_ini_logget_alm_chkRST.c
 * @brief SIGUSR1受信時処理
 * @date  2014/02/01 ALPHA) yokoyama Create RRH-007-000 初期化
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-2016
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include    "BPF_COM_END.h"
#include <sys/wait.h> 

/*!
 * @brief	f_ini_logget_alm_chkRST
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
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/11/17 ALPHA) murakami Create RRH-006-000 TDD-RRE-ZYNQ対応
 * @data  2016/03/14 ALPHA) fujiy modify for M-RRU-ZSYS-02088
 */
INT f_ini_logget_alm_chkRST(siginfo_t *sigInfo , VOID* buf_p , T_INIT_SIG_INFO_TBL* tmptbl )
{
	UINT					almCode = 0;
	UINT					hwAlmFactAry[E_RRH_ALMHIS_ALM_FACTOR_NUM];

	switch(sigInfo->si_signo){
		case SIGUSR1:
			/* ソフト障害検知(ALM発生時)					*/
			if(( (sigInfo->si_status & D_INI_SIGOPT_KIND) == D_RRH_SIGUSR_OPT_RSET_SWALM_NTC ) ||
				( (sigInfo->si_status & D_INI_SIGOPT_KIND) == D_RRH_SIGUSR_OPT_RSET_INIALM_NTC ))
			{

				tmptbl->addInfo				= D_RRH_SIGUSR_OPT_RSET_SWALM_NTC;
				tmptbl->sigstate			= E_INI_SIGNAL_SWALM;
				tmptbl->logType				= E_INI_LOGTYPE_TROUBLE;
				tmptbl->resetReason			= D_RRH_RST_RSN_SWRST1;
				tmptbl->uiResetFlg_define	= D_INI_SIG_RESET_ON;
				tmptbl->trouble_logget		= D_INI_SIG_RESET_ON;

				/* ALM要因を抽出 */
				almCode = sigInfo->si_status & D_INI_SIGOPT_ALMREN;

				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "rcv signal soft alarm. almCode = 0x%08x.", almCode);

				/*	Soft ALM要因の設定,Initのほうは、PF側ですでに設定されている	*/
				if((sigInfo->si_status & D_INI_SIGOPT_KIND) == D_RRH_SIGUSR_OPT_RSET_SWALM_NTC)
				{
					f_com_SVCtl(E_DU_SVCTL_OPE_SFT_ALMCD, D_DU_SVCTL_CHG, almCode);
				}

				/* アラームヒストリログ保持					*/
				BPF_HM_DEVC_REG_READ_BUFFER(0, D_DU_REG_CTB_SV1INF,sizeof(hwAlmFactAry), hwAlmFactAry) ;
				f_com_alarmHitoryLogSet( D_RRH_ALARM_FACTOR_TYPE_SW, almCode, hwAlmFactAry);
			}
			/* 障害ログ出力要求(Critical ALM or Major ALM)	*/
			else if( (sigInfo->si_status & D_INI_SIGOPT_KIND) == D_RRH_SIGUSR_OPT_RESORCE_ALM_NTC )
			{
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "rcv signal SIGUSR1 resource alm option=0x%08x.", sigInfo->si_status);
				tmptbl->addInfo				= D_RRH_SIGUSR_OPT_RSET_SWALM_NTC;
				tmptbl->sigstate			= E_INI_SIGNAL_RSCALM;
				tmptbl->logType				= E_INI_LOGTYPE_TROUBLE;
				tmptbl->resetReason			= D_RRH_RST_RSN_UNKNWN;
				tmptbl->uiResetFlg_define	= D_INI_SIG_RESET_OFF;
				tmptbl->trouble_logget		= D_INI_SIG_RESET_ON;

				BPF_HM_DEVC_REG_READ_BUFFER(0, D_DU_REG_CTB_SV1INF,sizeof(hwAlmFactAry), hwAlmFactAry) ;
				f_com_alarmHitoryLogSet( D_RRH_ALARM_FACTOR_TYPE_HW, 0, hwAlmFactAry);
				f_iniw_force_log_save = D_INI_SIG_RESET_ON;

			}
			else if( sigInfo->si_status == D_RRH_SUGUSR_OPT_RESET_CU_RST_NTC )
			{
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "rcv signal SIGUSR1 M-plane reset option=0x%08x logsave:0x%08x.", sigInfo->si_status,f_iniw_force_log_save);
				tmptbl->addInfo				= 0;
				tmptbl->sigstate			= E_INI_SIGNAL_MP_L3_RMT;
				tmptbl->logType				= E_INI_LOGTYPE_TROUBLE;
				tmptbl->resetReason			= D_RRH_RST_RSN_REQ;
				tmptbl->uiResetFlg_define	= D_INI_SIG_RESET_ON;
				tmptbl->trouble_logget		= f_iniw_force_log_save;
			}
			else if( sigInfo->si_status == D_RRH_SUGUSR_OPT_RESET_WDT_RST_NTC )
			{
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "rcv signal SIGUSR1 M-plane watch dog reset option=0x%08x logsave:0x%08x.", sigInfo->si_status,f_iniw_force_log_save);

				tmptbl->addInfo				= 0;
				tmptbl->sigstate			= E_INI_SIGNAL_MP_WDT;
				tmptbl->logType				= E_INI_LOGTYPE_HLTCHK;
				tmptbl->resetReason			= D_RRH_RST_RSN_LINK_TIMEOUT;
				tmptbl->uiResetFlg_define	= D_INI_SIG_RESET_ON;
				tmptbl->trouble_logget		= D_INI_SIG_RESET_ON;
			}
			else
			{
				cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "rcv signal SIGUSR1 other option=0x%08x.", sigInfo->si_status);
				tmptbl->uiResetFlg_define = D_INI_SIG_RESET_CHK;
			}
			break;
		case SIGCHLD:
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_ERROR, "PROCESS DEAD(pid:%d,sts:0x%x,flg:%d)",sigInfo->si_pid,sigInfo->si_status,f_iniw_sigchld);

			tmptbl->addInfo				= sigInfo->si_pid;
			tmptbl->sigstate			= E_INI_SIGNAL_SIGCHLD;
			tmptbl->logType				= E_INI_LOGTYPE_TROUBLE;
			tmptbl->resetReason			= D_RRH_RST_RSN_PROCESS_DEAD;
			tmptbl->uiResetFlg_define	= D_INI_SIG_RESET_ON;
			tmptbl->trouble_logget		= D_INI_SIG_RESET_ON;

			f_com_SVCtl(E_DU_SVCTL_OPE_SFT_ALMCD, D_DU_SVCTL_CHG, D_DU_ALMCD_TSK);

			/* alarm historyログにタスク異常として残す */
			BPF_HM_DEVC_REG_READ_BUFFER(0, D_DU_REG_CTB_SV1INF,sizeof(hwAlmFactAry), hwAlmFactAry) ;
			f_com_alarmHitoryLogSet( D_RRH_ALARM_FACTOR_TYPE_SW, D_DU_ALMCD_TSK, hwAlmFactAry );

			break;
			
		default:
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_WARNING, "Unknown signal rcv  sig:%d,pid:%d,sts:0x%x", sigInfo->si_signo,sigInfo->si_pid,sigInfo->si_status);
			tmptbl->uiResetFlg_define = D_INI_SIG_RESET_CHK;
			break;
	}

	return (INT)(tmptbl->uiResetFlg_define);
}

/* @} */
