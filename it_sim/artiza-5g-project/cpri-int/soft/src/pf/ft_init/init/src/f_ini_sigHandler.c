/*!
 * @skip  $ld:$
 * @file  f_ini_sigHandler.c
 * @brief シグナルハンドラー
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include "BPF_COM_LOG.h"

/* この定義は暫定です。必要に応じて各スレッド内のテーブルを追加してください */
static bpf_rm_svrm_sigget_tbl_t log_siggettble[] = 
{
	{"f_iniw_tskstn"			 ,sizeof(f_iniw_tskstn)			,&f_iniw_tskstn			,2},
	{"f_iniw_tsk_rcv_tbl"		 ,sizeof(f_iniw_tsk_rcv_tbl)	,&f_iniw_tsk_rcv_tbl[0]	,2},
	{"f_iniw_pid"				 ,sizeof(f_iniw_pid)			,&f_iniw_pid[0]			,4},
	{"f_iniw_sigchld"			 ,sizeof(f_iniw_sigchld)		,&f_iniw_sigchld		,4},
	{"f_comw_rrhVersion"		 ,sizeof(f_comw_rrhVersion)		,&f_comw_rrhVersion		,4}
};

/*!
 * @brief 関数機能概要:子プロセス終了シグナル検出時の処理を行う
 * @note  関数処理内容.
 *		-# ログ設定
 * @param  signal   [in] signal number
 * @param  siginfo  [in] signal information
 * @param  u_contxt [in] user context
 * @return VOID
 * @retval none
 * @date 2013/03/16 ALPHA)kondou Create RRH-007-000 初期化
 */
INT f_ini_sigHandler(INT signal, siginfo_t *siginfo, VOID *u_contxt , int fd )
{
	if(fd > 0)
	{
		/* 注意:											 */
		/* 本関数はシグナルハンドラ上で動作する				 */
		/* 本関数内では非同期シグナル安全な関数のみ使用可能	 */
		BPF_RM_SVRM_SIGNAL_TBLSAVE(fd,
							   sizeof(log_siggettble)/sizeof(log_siggettble[0]),
							   (char*)log_siggettble
							   );
	}
	return(sizeof(log_siggettble)/sizeof(log_siggettble[0]));
}

/* @} */
