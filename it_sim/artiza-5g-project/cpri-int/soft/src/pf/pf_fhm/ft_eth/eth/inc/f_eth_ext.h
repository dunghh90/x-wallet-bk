/*!
 * @skip  $ld:$
 * @file  f_eth_ext.h
 * @brief ETHスレッド 外部参照宣言
 * @date  2013/11/11 ALPHA) 中村 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_ETH
 * @{
 */
#ifndef F_ETH_EXT_H
#define F_ETH_EXT_H

#include "f_rrh_reg_eep.h"

/*!
 * @name 外部参照ヘッダ
 * @note 
 * @{
 */
extern	VOID				f_eth_main();										/*	スレッド起動処理				*/
extern	VOID				f_eth_init_IPset();									/*	スレッド起動時のIP設定処理		*/
extern	INT					f_eth_api_IPset(VOID* msgP);						/*	APIコール時のIP設定処理			*/
extern	INT 				f_eth_IPset(UINT ipAdr, UINT ipMask);				/*	IP設定共通処理					*/
extern	USHORT				f_ethw_thdState;									/*  thread state management table	*/
/* @} */


#endif
/* @} */
