/*!
 * @skip		$ld:$
 * @file		f_hdl_inc.h
 * @brief		HDLタスク 一括取り込みヘッダ
 * @date		2010/05/24 ALPHA)鮫島 Create
 * @date		2013/10/29 ALPHA)鮫島 docomo ZYNQ化 & 共通化対応
 * @date		2015/04/13 ALPHA)鎌田 DCN向け35G-SRE対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */
#ifndef F_HDL_INC_H
#define F_HDL_INC_H

/*!
 * @name HDLタスク状態
 * @note HDLタスクの状態定義
 * @{
 */
#include	<signal.h>
#include	"f_sys_inc.h"									/*!< システムヘッダ		*/
#include	"f_hdl_def.h"									/*!< 定数定義			*/
#include	"f_hdl_ext.h"									/*!< 外部参照宣言		*/
#include	"f_hdl_ftyp.h"									/*!< プロトタイプ宣言	*/
#include	"f_hdl_ram_local.h"								/*!< プロトタイプ宣言	*/
#include	"f_com_inc.h"									/*!< 共通定義			*/
#include	"f_rrh_inc.h"									/*!< RRHヘッダ			*/
#include	"BPF_RU_IPCM.h"									/*!< BPF_RU_IPCMヘッダ	*/
#include	"BPF_RM_SVRM.h"									/*!< BPF_RM_SVRMヘッダ	*/
#include	"BPF_HM_DEVC.h"									/*!< BPF_RM_DEVCヘッダ	*/
#include	"BPF_RU_ITCM.h"									/*!< BPF_RU_ITCMヘッダ	*/
#include	"BPF_COM_LOG.h"									/*!< BPF_COM_LOGヘッダ	*/

/* @} */

#endif
/* @} */
