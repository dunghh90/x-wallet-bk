/*!
 * @skip		$ld:$
 * @file		f_main_ext.h
 * @brief		pf_main extern定義
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date        2015/07/16 ALPHA) ueda modify for M-RRU-ZSYS-01646対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
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

extern USHORT                           f_mainw_thdmng_state;
extern USHORT                           f_mainw_thdIniMng[D_SYS_THDQNO_PF_MAX];
extern T_MAIN_THDID                     f_mainw_thdId;
extern T_RRH_TRAINVTBL                  f_mainw_inv_mng;
extern T_MAIN_CRT_EVENT_CTBL            f_mainr_thdCreMng_TDD[D_MAIN_THDID_MAX];
extern const T_MAIN_CALLSETRELL_MTXTBL  f_mainr_callSetRell_mtxtbl[D_MAIN_STATENUM][D_MAIN_EVENTNUM];
extern const T_HDL_CRT_EVENT_CTBL       f_hdlr_thdCreMng[D_RRH_HDLTHDNO_MAX];
extern const T_CMU_CREATE_THREAD        f_mainr_cmusRecvThdCreMng;
extern const T_CMU_CREATE_THREAD        f_mainr_cmusSendThdCreMng;
extern const T_DBGTHREAD_EXEC_INFO      f_mainr_debugparam;
extern sigset_t                         f_mainw_sigSetVal;
extern const UCHAR                      f_mainr_invShortInfo[D_RRH_I2C_EEP_DATANUM / 2];
extern const T_MAIN_THEAD_CHG_TBL       f_mainr_threadIDInfo[D_SYS_THDQNO_PF_MAX];
extern UINT                             f_mainw_uiQueChgTDD[D_SYS_THDQNO_PF_MAX];
extern T_CMN_ALMLOG_FUNCTION_LIST       f_cmnw_almlogfunc;
extern UCHAR                            f_mainw_orgData[D_RRH_FLASH_LOG_SIZE];
extern VOID                             *f_mainw_assert_p;
extern UINT                             f_mainw_reboot_status;
extern INT                              f_mainw_diagmem;

#endif
/* @} */
