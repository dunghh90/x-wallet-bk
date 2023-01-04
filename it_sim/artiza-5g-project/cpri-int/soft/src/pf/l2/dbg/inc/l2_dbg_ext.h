/*!
 * @skip  $ld:$
 * @file  l2_dbg_ext.h
 * @brief 外部参照宣言
 * @date  2013/12/03 FFCS)hongj create for zynq
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_L2_DBG
 * @{
 */
#ifndef L2_DBG_EXT_H
#define L2_DBG_EXT_H

/*!
 * @name 関数外部宣言
 * @note 関数外部宣言定義
 * @{
 */


extern UINT l2_dbgw_eventno ;					/* Event number		*/

extern VOID (* const l2_dbgr_mtrxtbl[L2_DBG_MX_EVENT])(UINT *inf_p);

/* @} */

#endif
/* @} */
