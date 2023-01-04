/*!
 * @skip  $ld:$
 * @file  f_ini_ext.h
 * @brief 初期化プロセス 外部参照宣言
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 * @date  2013/11/07 ALPHA) nakamura modify for ZYNQ
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */
#ifndef F_INI_EXT_H
#define F_INI_EXT_H

#include "f_rrh_type.h"
#include "fldc4.h"

/*!
 * @name RAMテーブル
 * @note RAMテーブル外部参照定義
 * @{
 */
extern	USHORT				f_iniw_tskstn;							/**< タスク管理					*/
extern	USHORT				f_iniw_tsk_rcv_tbl[D_INI_PROCNO_MAX];	/**< 初期化応答受信テーブル		*/
extern	INT					f_iniw_pid[D_INI_PROCNO_MAX];			/**< プロセスID管理テーブル		*/
extern	UINT				f_iniw_sigchld;							/**< SIGCHLDフラグ				*/
extern	UINT				f_comw_rrhVersion;						/**< バージョン情報				*/
extern	sigset_t			f_iniw_sigSetVal;
extern	UCHAR				f_iniw_compressData[D_INI_COMPDATA];	/**< 圧縮情報					*/

extern	T_RRH_PID_MNG			*procIdTbl;							/**< プロセスID管理テーブル		*/
extern	T_INIT_SIG_INFO_TBL	f_iniw_sig_info_alm;					/**< Signal情報保存テーブル		*/
extern	T_INIT_SIG_INFO_TBL	f_iniw_sig_info_rst;					/**< Signal情報保存テーブル		*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern	T_INIT_SIG_INFO_TBL	f_iniw_sig_info_hlt;					/**< Signal情報保存テーブル		*/
extern	T_INIT_SIG_INFO_TBL	f_iniw_sig_info_run[2];					/**< Signal情報保存テーブル		*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
extern	UINT				f_iniw_alreadyRESET;					/**< RESET済みフラグ			*/
extern	UINT				f_iniw_force_log_save;					/**< LOG保存フラグ				*/

extern	VOID*				f_iniw_assert_main_p;					/**< メインスレッド用assertlogポインタ	*/
extern	VOID*				f_iniw_assert_alm_p;					/**< almメインスレッド用assertlogポインタ	*/
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern	VOID*				f_iniw_assert_run_p;					/**< runスレッド用assertlogポインタ	*/
extern	VOID*				f_iniw_assert_hlt_p;					/**< hltスレッド用assertlogポインタ	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
extern	VOID*				f_iniw_assert_con_p;					/**< conメインスレッド用assertlogポインタ	*/
extern	T_RRH_REDEVCOMPTBL*	f_ini_re_dev_tbl;						/**< RE device management table(ini用) 		*/

/* @} */


/*!
 * @name ROMテーブル
 * @note ROMテーブル外部参照定義
 * @{
 */
extern const	T_INI_CRT_EVENT_CTBL				f_inir_procCreMng[D_INI_PROCNO_MAX];
extern const	UINT								f_inir_AccidentlogType[D_INI_PROCNO_MAX];
extern const	UINT								f_inir_reqSigType[E_INI_LOGTYPE_MAX];
extern const	UINT								f_inir_resSigType[E_INI_LOGTYPE_MAX];
extern const	T_INIT_SIG_RESET_TBL				f_inir_SigResetMng_tdd[E_INI_SIGNAL_MAX];
extern const	T_RRH_SYS_CREATE_SHMMEM				f_comr_shmmemCreMng[E_RRH_SHMID_APL_MAX];
extern const	T_INIT_CRT_EVENT_CTBL				f_inir_thdCreMng[D_INI_INITHDNO_MAX];
extern	const	T_INIT_TROUBLE_CALLSETRELL_MTXTBL	f_inir_callSetRell_mtxtbl_alm[D_INIT_STATENUM_ALM][D_LOG_EVENTNUM_ALM];
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern	const	T_INIT_TROUBLE_CALLSETRELL_MTXTBL	f_inir_callSetRell_mtxtbl_hlt[D_INIT_STATENUM_HLT][D_LOG_EVENTNUM_HLT];
extern	const	T_INIT_TROUBLE_CALLSETRELL_MTXTBL	f_inir_callSetRell_mtxtbl_run[D_INIT_STATENUM_RUN][D_LOG_EVENTNUM_RUN];
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
extern	const	T_INIT_SIG_STATUS_INFO_TBL			f_inir_sigusr1[ D_INI_SIG1_STATUS_MAX ];
extern	const	T_INIT_SIG_STATUS_INFO_TBL			f_inir_sigusr2[ D_INI_SIG2_STATUS_MAX ];
/* @} */


#endif
/* @} */
