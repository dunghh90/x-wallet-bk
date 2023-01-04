/*!
 * @skip  $ld:$
 * @file  l2_main_ext.h
 * @brief 外部参照宣言
 * @date  2013/11/28
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_L2
 * @{
 */
#ifndef L2_MAIN_EXT_H
#define L2_MAIN_EXT_H

/*!
 * @name 関数外部宣言
 * @note 関数外部宣言定義
 * @{
 */
#include "f_com_aplCom.h"

extern UINT l2_mainw_stateno ;					/* State number		*/

extern UINT l2_mainw_eventno ;					/* Event number		*/

extern VOID (* const l2_mainr_mtrxtbl[L2_MAIN_MX_STATE][L2_MAIN_MX_EVENT])(UINT *inf_p);

extern const T_SYS_THRDINFO l2_mainr_ThrMngTbl[8];
/* @} */

#endif
/* @} */
