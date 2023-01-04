/*!
 * @skip		$ld:$
 * @file		f_hdl_def.h
 * @brief		HDLスレッド デファイン定義.
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 * @date		2013/10/29 ALPHA)鮫島 docomo ZYNQ化 & 共通化対応
* @date		2015/04/22 ALPHA)鎌田 DCN向け35G-SRE対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_HDL
 * @{
 */
#ifndef F_HDL_DEF_H
#define F_HDL_DEF_H


/* @} */

/*!
 * @name HDLスレッド状態
 * @note HDLスレッドの状態定義
 * @{
 */
#define	D_HDL_THD_STS_INIT						0x0000							/*!< 初期状態							*/
#define	D_HDL_THD_STS_ACT						0x0001							/*!< 通常運用状態						*/
#define	D_HDL_STATENUM							0x0002							/*!< 診断スレッド状態数					*/
/* @} */

/** @name PF_HDL thread name len
 *  @{
 */
#define	D_HDL_THDNAME_LEN	16													/**< thread name len 					*/
/** @} */

/*!
 * @name    割り込み待ち用define
 * @{
 */
#define D_HDL_BPF_IRQ_CPRI_REC		BPF_IRQ_CPRI
#define D_HDL_BPF_IRQ_CPRI_RE		BPF_IRQ_CPRI_MASTER
/* @} */

#endif
/* @} */
