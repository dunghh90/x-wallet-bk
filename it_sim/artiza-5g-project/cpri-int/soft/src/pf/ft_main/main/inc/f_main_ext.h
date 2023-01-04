/*!
 * @skip		$ld:$
 * @file		f_main_ext.h
 * @brief		pf_main extern定義
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/07/16 ALPHA) ueda modify for M-RRU-ZSYS-01646対応
 * @date		2015/08/27 ALPHA) miyazaki modify for TDD_RRE_ZYNQ
 * @date		2015/10/28 ALPHA) miyazaki ハソ1.13対応
 * @date 		2018/06/22 Fujitsu)吉田 5G-DU割込み処理追加
 * 
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-2018
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */
#ifndef F_MAIN_EXT_H
#define F_MAIN_EXT_H

#include "f_cmn_cmu_inc.h"
#include "f_main_ram.h"
#include "f_main_rom.h"
#include "f_main_matrix.h"
#include "f_main_def.h"
#include "f_main_typ.h"

extern	USHORT					f_mainw_thdmng_state;					/**< thread state management table		*/
extern	T_MAIN_THDID			f_mainw_thdId;							/**< thread ID							*/
extern	T_RRH_TRAINVTBL			f_mainw_inv_mng;						/**< Inventory Manage Table				*/
extern	T_MAIN_CRT_EVENT_CTBL					f_mainr_thdCreMng[D_MAIN_THDID_MAX];
extern	T_MAIN_CRT_EVENT_CTBL					f_mainr_thdCreMng_ptp[D_MAIN_THDID_MAX];
extern	const	T_MAIN_CALLSETRELL_MTXTBL		f_mainr_callSetRell_mtxtbl[D_MAIN_STATENUM][D_MAIN_EVENTNUM];		/**< INI State MngSTM  */
extern	const	T_HDL_CRT_EVENT_CTBL			f_hdlr_thdCreMng[D_RRH_HDLTHDNO_MAX];
extern	const	T_CMU_CREATE_THREAD				f_mainr_cmusRecvThdCreMng;
extern	const	T_CMU_CREATE_THREAD				f_mainr_cmusSendThdCreMng;
extern	UINT	f_mainr_diagBit_tdd;
extern	const	T_DBGTHREAD_EXEC_INFO			f_mainr_debugparam;
extern	sigset_t				f_mainw_sigSetVal;						/**< Set Signal Date					*/
extern	const UCHAR f_mainr_invShortInfo[D_RRH_I2C_EEP_DATANUM/4];		/**< Invent Info エンディアン変換情報	*/
extern	const T_MAIN_THEAD_CHG_TBL f_mainr_threadIDInfo[D_SYS_THDQNO_PF_MAX];	/**< ThreadID TreadQID変換情報	*/
extern	T_CMN_ALMLOG_FUNCTION_LIST f_cmnw_almlogfunc;
extern	UCHAR	f_mainw_orgData[D_RRH_FLASH_LOG_SIZE];					/**< 圧縮データ用元データ				*/
extern	UCHAR	f_mainw_iniData[D_RRH_FLASH_INIT_DATA_SIZE];			/**< FLASH退避データ	*/
extern	VOID*	f_mainw_assert_p;							/**< main thread assert pointer */
extern	UCHAR					f_mainw_rficFlashData[D_RRH_DATA_SIZE_1M];	/**< rfic Flash Data(1Mbyte) */
extern	UINT	f_mainw_reboot_status;									/**< Reset reason */
extern	const T_MAIN_SFP_TYPE_TBL f_mainr_sfpType[E_COM_SFP_TYPE_NUM];			/**< SFP Type情報						*/
extern	UINT	f_mainw_threadinitcmpbit;						/**< init comp */
extern	UINT	f_mainw_process_start_mode;						/**< process start */


extern	T_CMU_CREATE_THREAD_MP				f_mainr_cmusRecvThdCreMng_mp_pf;
extern	T_CMU_CREATE_THREAD_MP				f_mainr_cmusRecvThdCreMng_mp_ptp;


#endif
/* @} */
