/*!
 * @skip  $ld:$
 * @file  f_cmn_com_inc.h
 * @brief 装置共通ヘッダ一括Includeヘッダ
 * @date  2008/09/01 ALPHA) 戸塚 Create
 * @date  2013/11/26 ALPHA) 近藤 zynq化対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */
#ifndef F_CMN_COM_INC_H
#define F_CMN_COM_INC_H

#include <signal.h>
#include "f_rrh_inc.h"
#include "f_cmn_com_def.h"
#include "f_cmn_com_typ.h"
#include "f_cmn_com_ext.h"
#include "BPF_RM_SVRM.h"							/*!< サーバ運転管理		*/
#include "BPF_RU_IPCM.h"							/*!< プロセス間通信管理	*/
#include "BPF_HM_DEVC.h"							/*!< デバイス制御		*/
#include "BPF_COM_LOG.h"							/*!< LOG機能			*/
#include "BPF_RU_ITCM.h"							/*!< スレッド間通信管理	*/
#include "BPF_COM_PLOG.h"							/*!< PLOG機能			*/

#endif
/* @} */
