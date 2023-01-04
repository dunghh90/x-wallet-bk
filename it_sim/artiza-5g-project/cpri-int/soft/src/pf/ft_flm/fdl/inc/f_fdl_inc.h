/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_fdl_inc.h
 *  @brief  FDLタスク インクルードヘッダ定義
 *  @date   2019/01/28 KCN)上本 create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019
 */
/*********************************************************************************/

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
#ifndef F_FDL_INC_H
#define F_FDL_INC_H

/*!
 * @name FDLタスク
 * @note 取込みヘッダ
 * @{
 */
#include "f_du_reg_ctb.h"
#include "f_sys_inc.h"									/*!< システムヘッダ					*/
#include "f_com_inc.h"									/*!< PF共通ヘッダ					*/

#include "f_mpsw_msg_o-ran_software_management_def.h"	/*!< MSIインタフェースdefine定義	*/
#include "f_mpsw_msg_o-ran_software_management_typ.h"	/*!< MSIインタフェースtypdef定義	*/

#include "f_fdl_def.h"									/*!< FDLタスクデファイン定義		*/
#include "f_fdl_typ.h"									/*!< FDLタスク共通構造体			*/
#include "f_fdl_ftyp.h"									/*!< FDLタスクプロトタイプ定義		*/
#include "f_fdl_ext.h"									/*!< FDLタスク外部参照ヘッダ定義	*/
/* @} */

#endif
/* @} */
