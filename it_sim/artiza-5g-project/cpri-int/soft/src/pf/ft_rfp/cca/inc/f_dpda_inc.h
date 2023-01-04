/*!
 * @skip  $ld:$
 * @file  f_dpda_inc.h
 * @brief DPDAタスク 一括取り込みヘッダ
 * @date  2011/07/26 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2012
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#ifndef F_DPDA_INC_H
#define F_DPDA_INC_H

/*!
 * @name DPDAタスク
 * @note 取り込みヘッダ
 * @{
 */
#include	<math.h>
#include	<stdlib.h>
#include	<errno.h>
#include	"f_rrh_inc.h"
#include	"f_sys_inc.h"									/*!< システムヘッダ		*/
#include	"f_com_inc.h"									/*!< PF共通ヘッダ		*/
#include	"BPF_COM_END.h"									/*!< BPF COM定義		*/
#include	"BPF_RU_IPCM.h"									/*!< BPF_RU_IPCMヘッダ	*/
#include	"BPF_COM_LOG.h"									/*!< BPF define定義		*/
#include	"BPF_RM_SVRM.h"									/*!< BPF define定義		*/
#include	"rrhApi_Rfp.h"									/*!< API RF INC定義		*/
#include	"f_mpsw_msg_o-ran_uplane_conf_def.h"
#include	"f_mpsw_msg_o-ran_uplane_conf_typ.h"
#include	"f_dpda_def.h"									/*!< 定数定義			*/
#include	"f_dpda_ext.h"									/*!< 外部参照宣言		*/
#include	"f_rfp_typ.h"									/*!< インタフェース宣言	*/
#include	"f_dpda_typ.h"									/*!< 構造体タグ宣言		*/
#include	"f_dpda_ftyp.h"									/*!< プロトタイプ宣言	*/
#include	"f_dpda_matrix.h"								/*!< マトリクス			*/
#include	"f_dpda_matrix_local.h"							/*!< 外部参照			*/
#include	"f_dpda_ram.h"									/*!< ramヘッダ			*/
#include	"f_dpda_ram_local.h"							/*!< ramヘッダローカル	*/
#include	"f_dpda_rom.h"									/*!< romヘッダ			*/
#include	"f_dpda_rom_local.h"							/*!< romヘッダローカル	*/
#include	"RFIC_mmW_API.h"								/*!< RF(mmW)用ヘッダル	*/
#include	"RFIC_TALISE.h"									/*!< RF(sub6)用ヘッダル	*/
#include	"f_du_reg_dlp.h"
/* @} */

#endif
/* @} */
