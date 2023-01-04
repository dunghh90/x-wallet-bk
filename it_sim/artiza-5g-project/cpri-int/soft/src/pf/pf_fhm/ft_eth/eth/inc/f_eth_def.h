/*!
 * @skip  $ld:$
 * @file  f_eth_def.h
 * @brief ETHスレッド 定数定義
 * @date  2013/11/11 ALPHA) 中村 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_ETH
 * @{
 */
#ifndef F_ETH_DEF_H
#define F_ETH_DEF_H

/*!
 * @name IPアドレスデフォルト定義
 * @note 
 * @{
 */
#define D_ETH_IPADR							0xc0a801BF				/*!< IPアドレスデフォルト値				*/
#define D_ETH_IPMSK							0xffffff00				/*!< IPアドレスデフォルト値				*/
/* @} */

/*!
 * @name ETHスレッド状態
 * @note ETHスレッドの状態定義
 * @{
 */
#define	D_ETH_THD_STSD_INIT						0x0000				/*!< 初期状態							*/
#define	D_ETH_THD_STSD_IDLE						0x0001				/*!< 通常運用状態						*/
/* @} */

#endif
/* @} */
