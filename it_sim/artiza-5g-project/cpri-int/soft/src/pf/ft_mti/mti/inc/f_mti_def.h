/*!
 * @skip		$ld:$
 * @file		f_mti_def.h
 * @brief		MTIスレッド デファイン定義.
 * @date		2019/05/13 Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_MTI
 * @{
 */
#ifndef F_MTI_DEF_H
#define F_MTI_DEF_H


/* @} */

/*!
 * @name MTIスレッド定義
 * @note MTIスレッド定義
 * @{
 */
/* @} */

/*!
 * @name 動作種別定義
 * @note 動作種別定義
 * @{
 */
														/* bit31:BeamID配置設定				*/
#define	D_MTI_OPTION_BEAMID_POSITION_NEW	0x80000000	/* 	off :旧配置 / on:新配置			*/

														/* bit 7:mmW BeamID 新旧モード設定	*/
#define	D_MTI_OPTION_BF_ENVELOPE_MODE_ON	0x00000080	/* 	off :旧beam / on:新Beam			*/

														/* bit 6:FAN回転数モード設定		*/
#define	D_MTI_OPTION_FAN_SPIN_MODE_ON		0x00000040	/* 	off :通常モード / on:静音モード	*/

/*!
 * @name 動作種別定義
 * @note 動作種別定義
 * @{
 */
#define	D_MTI_WOFFSET_BF_ENVELOPE_MODE	1				/* 	mmW BeamID 新旧モード設定		*/
#define	D_MTI_WOFFSET_FAN_SPIN_MODE		2				/* 	FAN回転数モード設定				*/


#endif
/* @} */
