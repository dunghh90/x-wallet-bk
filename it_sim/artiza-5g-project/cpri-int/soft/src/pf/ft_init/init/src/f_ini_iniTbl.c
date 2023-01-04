/*!
 * @skip  $ld:$
 * @file  f_ini_iniTbl.c
 * @brief RAMテーブル初期化処理
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"

/*!
 * @brief		f_ini_iniTbl
 * @note		関数処理内容.
 *					-# 各RAMテーブルに初期値を設定
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/03/16 ALPHA)kondou Create RRH-007-000 初期化
 * @date 2013/11/07 ALPHA)nakamura modify for ZYNQ
 */
VOID f_ini_iniTbl()
{

	/************************************************************/
	/* 各RAMテーブルに初期値を設定								*/
	/************************************************************/
	memset((VOID*)&f_iniw_pid, 0, sizeof(f_iniw_pid));
	memset((VOID*)&f_iniw_tsk_rcv_tbl, 0, sizeof(f_iniw_tsk_rcv_tbl));
	f_iniw_sigchld = D_RRH_OFF;

	BPF_COM_PLOG_TRACE_START(D_RRH_PROCID_INI);	/* proc trace start */
	f_iniw_assert_main_p = f_cmn_com_assert_th_create(D_RRH_LOG_AST_DEFAULT,"Init main thread");

	/* タスク管理状態を"INIT処理開始で更新 "*/
	f_iniw_tskstn = D_INI_TSK_STN_STA;

	return;
}

/* @} */
