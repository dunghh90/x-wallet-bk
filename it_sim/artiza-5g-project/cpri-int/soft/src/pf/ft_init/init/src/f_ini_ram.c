/*!
 * @skip  $ld:$
 * @file  f_ini_ram.c
 * @brief 初期化RAMテーブル定義
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 * @date  2013/11/07 ALPHA) nakamura modify for ZYNQ
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */
#include	"f_ini_inc.h"

USHORT					f_iniw_tskstn;							/**< タスク管理					*/
USHORT					f_iniw_tsk_rcv_tbl[D_INI_PROCNO_MAX];	/**< 初期化応答受信テーブル		*/
INT						f_iniw_pid[D_INI_PROCNO_MAX];			/**< プロセスID管理テーブル		*/
UINT					f_iniw_sigchld;							/**< SIGCHLDフラグ				*/
UINT					f_comw_rrhVersion;						/**< バージョン情報				*/
sigset_t				f_iniw_sigSetVal;
UCHAR					f_iniw_compressData[D_INI_COMPDATA];	/**< 圧縮情報					*/

T_RRH_PID_MNG			*procIdTbl;								/** プロセスID管理テーブル		*/
T_INIT_SIG_INFO_TBL		f_iniw_sig_info_alm;					/**< Signal情報保存テーブル(ALM)			*/
T_INIT_SIG_INFO_TBL		f_iniw_sig_info_rst;					/**< Signal情報保存テーブル(ALM_RESET有)	*/
T_INIT_SIG_INFO_TBL		f_iniw_sig_info_hlt;					/**< Signal情報保存テーブル(HealthCheck)	*/
T_INIT_SIG_INFO_TBL		f_iniw_sig_info_run[2];					/**< Signal情報保存テーブル(運用情報取得)	*/
T_INIT_SIG_INFO_TBL		f_iniw_sig_info_usr2;					/**< Signal情報保存テーブル(SIGUSR2)		*/
UINT					f_iniw_alreadyRESET;					/**< RESET済みフラグ			*/
UINT					f_iniw_force_log_save;					/**< LOG保存フラグ				*/

VOID*					f_iniw_assert_main_p;					/**< メインスレッド用assertlogポインタ	*/
VOID*					f_iniw_assert_alm_p;					/**< almメインスレッド用assertlogポインタ	*/
/* 2020/12/28 M&C) Implement device startup (start add) */
VOID*					f_iniw_assert_run_p;					/**< runスレッド用assertlogポインタ	*/
VOID*					f_iniw_assert_hlt_p;					/**< hltスレッド用assertlogポインタ	*/
/* 2020/12/28 M&C) Implement device startup (end add) */
VOID*					f_iniw_assert_con_p;					/**< conメインスレッド用assertlogポインタ	*/

/** @} */

