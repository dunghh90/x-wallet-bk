/*!
 * @skip   $Id$
 * @file   f_dbg_thread_inc.h
 * @brief  デバッグスレッド共通インクルード定義
 * @date   2013/11/22 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


#ifndef F_DBG_THRAD_INC_H_
#define F_DBG_THRAD_INC_H_

#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<unistd.h>
#include 	<fcntl.h>
#include 	<string.h>
#include 	<errno.h>
#include	<limits.h>
#include	<sys/prctl.h>
#include	"f_rrh_def.h"
#include	"m_cm_def.h"
#include	"f_rrh_type.h"
#include	"f_sys_def.h"
#include	"f_sys_type.h"
#include	"BPF_HM_DEVC.h"									/*!< BPF_RU_IPCMヘッダ									*/
#include	"BPF_RU_IPCM.h"									/*!< BPF_RU_IPCMヘッダ									*/
#include	"BPF_RM_SVRM.h"									/*!< BPF_RM_SVRMヘッダ									*/
#include	"BPF_COM_LOG.h"									/*!< BPF_COM_LOGヘッダ									*/
#include	"f_dbg_com_def.h"								/*!< */
#include	"f_dbg_com_type.h"
#include	"f_dbg_thread_def.h"							/*!< DEBUGスレッド定数定義ヘッダ						*/
#include	"f_dbg_thread_type.h"							/*!< DEBUGスレッド型定義ヘッダ							*/
#include	"f_dbg_thread_ext.h"							/*!< DEBUGスレッド外部定義関数プロトタイプ宣言ヘッダ	*/


#endif /* F_DBG_THRAD_INC_H_ */
