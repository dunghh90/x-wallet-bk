/*!
 * @skip   $Id$
 * @file   f_cmu_dbg_rom_zynq.c
 * @brief  デバッグスレッド ROM定義
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */

#include "f_dbg_thread_inc.h"


/*
 * @name	デバッグ実行関数定義テーブル
 * @note	各プロセスにおけるデバッグコマンド実行スレッド名と実行関数を定義する
 */
T_DBGCMD_EXEC_FUNC	DebugCmdExecFunc[ D_CMD_DBG_EXEC_FUNCS_CNT ]	=	{
	{	D_RRH_PROCQUE_PF,	"main",		"libDBG_pf_main.so",	"dbgcmd_exec_pf_main"	},
	{	D_RRH_PROCQUE_PF,	"cca",		"libDBG_pf_cca.so",		"dbgcmd_exec_pf_dpda"	}
};

/* @} */

