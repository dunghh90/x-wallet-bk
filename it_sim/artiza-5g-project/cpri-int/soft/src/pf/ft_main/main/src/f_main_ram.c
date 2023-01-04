/*!
 * @skip		$ld:$
 * @file		f_main_ram.c
 * @brief		pf_main RAMテーブル
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date  		2014/12/15 ALPHA) okabe Create RRH-013-000 共通ppc対応
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/09 ALPHA) tomioka modify RRH-007-000 TDD-Zynq対応
 * @date  		2015/07/16 ALPHA) ueda modify for M-RRU-ZSYS-01646対応
 * @date		2015/08/27 ALPHA) miyazaki modify to TDD-RRE-Zynq 
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include	"f_main_inc.h"

USHORT					f_mainw_thdmng_state;						/**< thread state management table		*/
T_MAIN_THDID			f_mainw_thdId;								/**< thread ID							*/

T_RRH_TRAINVTBL			f_mainw_inv_mng;							/**< Inventory Manage Table				*/
 
sigset_t				f_mainw_sigSetVal;							/**< Set Signal Date					*/

T_CMN_ALMLOG_FUNCTION_LIST f_cmnw_almlogfunc;
T_CMN_SV_FUNCTION_LIST     f_cmnw_svfunc;


UCHAR					f_mainw_orgData[D_RRH_FLASH_LOG_SIZE];		/**< 圧縮データ用元データ				*/

UCHAR					f_mainw_iniData[D_RRH_FLASH_INIT_DATA_SIZE];	/**< FLASH退避データ	*/

VOID*					f_mainw_assert_p;							/**< main thread assert pointer */

UCHAR					f_mainw_rficFlashData[D_RRH_DATA_SIZE_1M];	/**< rfic Flash Data(1Mbyte) */

UINT					f_mainw_reboot_status;						/**< Reset reason */

UINT					f_mainw_threadinitcmpbit;					/**< init comp */

UINT					f_mainw_process_start_mode;					/**< process */

/** @} */

