/*!
 * @skip		$ld:$
 * @file		f_log_inc.h
 * @brief		ログタスク 一括取り込みヘッダ
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/10/29 ALPHA)横山 docomo ZYNQ化 & 共通化対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010 - 2013
 */

/*!
 * @addtogroup RRH_PF_LOG
 * @{
 */
#ifndef F_LOG_INC_H
#define F_LOG_INC_H

/*!
 * @name ログタスク状態
 * @note ログタスクの状態定義
 * @{
 */

#include	"f_rrh_inc.h"									/*!< システムヘッダ		*/
#include	"f_sys_inc.h"									/*!< システムヘッダ		*/
#include	"signal.h"
#include	"f_com_inc.h"									/*!< 共通ヘッダ		*/

#include	"rrhApi_Com.h"									/*!< API				*/
#include	"rrhApi_Log.h"									/*!< API				*/

#include	"f_log_def.h"									/*!< 定数定義			*/
#include	"f_log_ext.h"									/*!< 外部参照宣言		*/
#include	"f_log_typ.h"									/*!< 構造体タグ宣言		*/
#include	"f_log_ftyp.h"									/*!< プロトタイプ宣言	*/
#include	"f_log_matrix.h"								/*!< マトリクス			*/
#include	"f_log_matrix_local.h"							/*!< 外部参照			*/
#include	"f_log_ram.h"									/*!< ramヘッダ			*/
#include	"f_log_ram_local.h"								/*!< ramヘッダローカル	*/

#include	"BPF_RU_IPCM.h"									/*!< BPF_RU_IPCMヘッダ	*/
#include	"BPF_COM_LOG.h"									/*!< BPF_COM_LOGヘッダ	*/
#include	"BPF_HM_DEVC.h"									/*!< BPF_HM_DEVCヘッダ	*/
#include	"BPF_RM_SVRM.h"									/*!< BPF_RM_SVRMヘッダ	*/
#include	"BPF_RU_HRTM.h"									/*!< BPF_RU_HRTMヘッダ	*/

#include	"f_cmn_com_inc.h"


/* @} */

#endif
/* @} */
