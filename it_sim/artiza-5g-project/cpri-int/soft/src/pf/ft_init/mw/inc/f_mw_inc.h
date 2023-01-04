/*!
 * @skip  $ld:$
 * @file  f_mw_inc.h
 * @brief 初期化プロセス 一括取り込みヘッダ
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */
#ifndef F_INI_INC_H
#define F_INI_INC_H

#include	<stdio.h>
#include	<signal.h>
#include	<sys/prctl.h>
#include	"f_rrh_inc.h"							/*!< 装置共通定義		*/
#include	"f_cmn_com_inc.h"						/*!< Common定義			*/
#include	"f_com_inc.h"							/*!< PF Common定義		*/
#include	"f_rrh_reg_cpld.h"						/*!< CPLD定義			*/
#include	"f_rrh_reg_eep.h"						/*!< EEPROM定義			*/
#include	"f_sys_inc.h"							/*!< PF共通照宣言		*/
#include	"BPF_RM_SVRM.h"							/*!< サーバ運転管理		*/
#include	"BPF_RU_IPCM.h"							/*!< プロセス間通信管理	*/
#include	"BPF_HM_DEVC.h"							/*!< デバイス制御		*/
#include	"BPF_COM_LOG.h"							/*!< LOG機能			*/
#include	"BPF_RU_ITCM.h"							/*!< スレッド間通信管理	*/
#endif
/* @} */
