/*!
 * @skip		$ld:$
 * @file		f_dbg_cmd_inc.h
 * @brief		デバッグコマンド　インクルードファイル代表
 * @date		2013/11/26 ALPHA)宮崎 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013,2014,2015
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */


#ifndef F_DBG_CMD_INC_H_
#define F_DBG_CMD_INC_H_

#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<unistd.h>
#include 	<fcntl.h>
#include 	<string.h>
#include 	<errno.h>
#include	<limits.h>
#include	"f_sys_def.h"
#include	"m_cm_def.h"
#include	"f_rrh_type.h"
#include	"f_rrh_def.h"
#include	"f_rrh_reg_cpld.h"
#include	"f_rrh_reg_cnt.h"
#include	"f_rrh_reg_eep.h"
#include	"f_rrh_reg_flash.h"
#include	"BPF_HM_DEVC.h"									/*!< BPFデバイスアクセス関連ヘッダ		*/
#include	"BPF_RU_IPCM.h"									/*!< BPFヘッダ							*/
#include	"BPF_RM_SVRM.h"									/*!< BPF_RM_SVRMヘッダ									*/
#include	"BPF_COM_LOG.h"									/*!< BPF_COM_LOGヘッダ									*/
#include    "bpf_i_ru_hrtm.h"
#include    "bpf_i_ru_ipcm_shm.h"
#include    "bpf_i_ru_ipcm_sem.h"

#undef		CMD_TSKID_INI
#undef 		CMD_TSKID_CPR
#undef 		CMD_TSKID_SND
#undef 		CMD_TSKID_RCV
#undef 		CMD_TSKID_DBG
#undef		CMD_TSKID_LPB

#include	"f_dbg_com_def.h"									/*!< DEBGU共通定義ヘッダ				*/
#include	"f_dbg_com_type.h"								/*!< DEBGU共通型定義ヘッダ				*/
#include	"f_dbg_cmd_type.h"								/*!< DEBGUコマンド型定義ヘッダ			*/

#endif /* F_DBG_CMD_INC_H_ */

/* @} */

