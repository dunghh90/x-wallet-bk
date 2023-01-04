/*!
 * @skip  $ld:$
 * @file  mnt_main_ext.h
 * @brief 外部参照宣言
 * @date  2013/11/28
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_MNT
 * @{
 */
#ifndef MNT_MAIN_EXT_H
#define MNT_MAIN_EXT_H

/*!
 * @name 関数外部宣言
 * @note 関数外部宣言定義
 * @{
 */


extern UINT mnt_mainw_stateno;					/* State number		*/

extern UINT mnt_mainw_eventno;					/* Event number		*/

extern VOID (* const mnt_mainr_mtrxtbl[MNT_MAIN_MX_STATE][MNT_MAIN_MX_EVENT])(UINT *inf_p);

/* @} */

#endif
/* @} */
