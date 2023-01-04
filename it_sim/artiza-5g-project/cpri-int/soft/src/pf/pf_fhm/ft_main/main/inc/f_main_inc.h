/*!
 * @skip  $ld:$
 * @file  f_main_inc.h
 * @brief pf_main 一括取り込みヘッダ
 * @date  2013/03/19 ALPHA)松延 Create
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/07/30 TDIPS) maruyama PFプロセスにfrwreスレッド追加のためヘッダファイル追記
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */
#ifndef F_MAIN_INC_H
#define F_MAIN_INC_H

#include	<signal.h>
#include    <time.h>
#include	"f_sys_inc.h"									/*!< 装置共通定義				*/
#include	"f_sys_type.h"									/*!< 装置共通定義				*/
#include	"f_rrh_inc.h"									/*!< RRH共通定義				*/
#include	"f_rrh_reg_eep.h"								/*!< EEPROM共通定義				*/
#include	"f_com_inc.h"									/*!< タスク間共通定義			*/
#include	"f_dbg_thread_inc.h"							/*!< Debugスレッド定義			*/
#include	"f_dbg_com_ftype.h"								/*!< DEBUGプロトタイプ宣言		*/
#include	"f_main_def.h"									/*!< 型宣言定義					*/
#include	"f_main_ext.h"									/*!< 外部参照宣言				*/
#include	"f_main_typ.h"									/*!< 構造体タグ宣言				*/
#include	"f_main_ftyp.h"									/*!< プロトタイプ宣言			*/
#include	"f_hdl_ftyp.h"									/*!< hdl関数プロトタイプ宣言	*/
#include	"f_wdt_ext.h"									/*!< wdt関数プロトタイプ宣言	*/
#include	"f_eth_ext.h"									/*!< eth関数プロトタイプ宣言	*/
#include    "f_dpda_ftyp.h"                                 /*!< dpda関数プロトタイプ宣言   */
#include	"f_cpr_ftyp.h"									/*!< cpri関数プロトタイプ宣言	*/
#include	"f_frw_ftyp.h"									/*!< frw関数プロトタイプ宣言	*/
#include	"f_frwsla_ftyp.h"								/*!< frwsla関数プロトタイプ宣言	*/
#include	"f_nego_inc.h"									/*!< nego関数プロトタイプ宣言	*/
#include	"f_fmng_ftyp.h"									/*!< fmng関数プロトタイプ宣言	*/
#include	"f_cmn_cmu_ftyp.h"								/*!< cmu関数プロトタイプ宣言	*/
#include 	"f_log_inc.h"									/*!< logスレッド定義			*/
#include 	"BPF_RU_IPCM.h"									/*!< BPF_RU_IPCM定義			*/
#include	"BPF_RM_SVRM.h"									/*!< BPF_RM_SVRM定義			*/
#include	"m_cm_def.h"									/*!< CMN定義			*/
#include	"rrhApi_Svp.h"
#include	"rrhApi_File.h"
#include	"rrhApi_Cpri.h"
#include	"rrhApi_Nego.h"
#include	"rrhApi_Dbg.h"
#include	"f_frwre_ftyp.h"								/*!< frwre関数プロトタイプ宣言	*/

#endif
/* @} */
