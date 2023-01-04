/*!
 * @skip		$ld:$
 * @file		f_main_def.h
 * @brief		pf_main define定義
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date		2014/12/05 alpha)岡部 Modify
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 * @date  		2015/08/13 TDIPS) maruyama pf_mainで起動するスレッドの最大数(D_MAIN_THDID_MAX)変更
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#ifndef F_MAIN_DEF_H
#define F_MAIN_DEF_H

/* MAINスレッドの状態定義						*/
#define	D_MAIN_THD_STS_INIT							0x0000						/*!< 初期状態						*/
#define	D_MAIN_THD_STS_INI_WAITING					0x0001						/*!< 初期化完了待ち					*/
#define	D_MAIN_STATENUM								0x0002						/*!< 状態数							*/

/* スレッド生成・起動実施判定用データ			*/
#define	D_MAIN_SKIP									0x0000						/*!< 生成・起動スキップ				*/
#define	D_MAIN_CREATE_PTN_NOT_WDT					0x0001						/*!< 生成・起動パターン(WDT以外)	*/
#define	D_MAIN_CREATE_PTN_WDT						0x0002						/*!< 生成・起動パターン(WDT)		*/

/* Jumpテーブルの状態・起動要因の定数定義		*/
#define	D_MAIN_EVE_CREATE_PROC						0x0000
#define	D_MAIN_EVE_INIT_COMP_NTC					0x0001
#define	D_MAIN_EVENTNUM								0x0002

/* 各スレッドの初期処理実施状態					*/
#define	D_MAIN_THD_STS_COMP							1							/*!< 初期処理完了					*/

/*!
 * @name TDDカード起動設定値
 * @note 
 * @{
 */
#define D_MAIN_STA1_STA2_MASK_REL				0x00000003				/* STA1/STA2マスク解除				*/
#define D_MAIN_CPP_CPS_SERDES_DONE_CHECK_CNT	10						/* CPP/CPS Serdes Done確認最大数	*/
#define D_MAIN_SYS_PLL_LOCK_CHECK_CNT			10						/* SYS PLL Lock確認最大数			*/
#define D_MAIN_CNT_STA1_STA2_MASK_REL	        0x70001100				/* STA1/STA2マスク解除				*/
/* @} */

/*!
 * @name PFスレッド数情報定義
 * @note 
 * @{
 */
#define D_MAIN_THDID_MAX							16							/*!< pf_mainで起動するスレッドの最大数	*/
/* @}
 */

/*!
 * @name 装置種別定義
 * @note 
 * @{
 */
#define D_MAIN_DEVICE_KIND1_RRE						0x00						/* RE種別:RRE								*/
#define D_MAIN_DEVICE_KIND1_SRE						0x01						/* RE種別:SRE								*/
#define D_MAIN_DEVICE_KIND1_15G						0x01						/* 対応周波数帯:15G							*/
#define D_MAIN_DEVICE_KIND1_17G						0x02						/* 対応周波数帯:17G							*/
#define D_MAIN_DEVICE_KIND1_700M					0x03						/* 対応周波数帯:700M						*/
#define D_MAIN_DEVICE_KIND1_2G						0x04						/* 対応周波数帯:G							*/
#define D_MAIN_DEVICE_KIND1_800M					0x05						/* 対応周波数帯:800M						*/
#define D_MAIN_DEVICE_KIND1_35G						0x06						/* 対応周波数帯:3.5M						*/
/* @} */

/*!
 * @name リセット要因設定用メッセージサイズ定義
 * @note 
 * @{
 */
#define D_MAIN_MESSAGE_SIZE							64							/*!< メッセージサイズ				*/
/* @} */

/*!
 * @name Flashログ展開定義
 * @note 
 * @{
 */
#define D_MAIN_FLASH_LOG_COMP_ON					1							/* Flashログ圧縮有り				*/
#define D_MAIN_FLASH_LOG_COMP_OFF					0							/* Flashログ圧縮無し				*/
/* @} */



/*!
 * @name EEPROMデータアクセス定義
 * @note
 * @{
 */
#define D_MAIN_INV_UBOOT_FLASH_OFFSET1				0x00000000					/*!< u-boot.binヘッダーFlash読み込みオフセット1		*/
#define D_MAIN_INV_UBOOT_FLASH_OFFSET2				0x00100000					/*!< u-boot.binヘッダーFlash読み込みオフセット2		*/
#define D_MAIN_INV_UBOOT_FLASH_SIZE1 				0x00000050					/*!< u-boot.binヘッダーFlash読み込みサイズ1			*/
#define D_MAIN_INV_UBOOT_FLASH_SIZE2 				0x00000004					/*!< u-boot.binヘッダーFlash読み込みサイズ2			*/
#define D_MAIN_INV_UBOOT_FLASH_SIZE3 				0x00000080					/*!< u-boot.binヘッダーFlash読み込みサイズ3			*/
#define D_MAIN_INV_FSBL_STARTBIT_BUF1				0x33						/*!< FSBL開始位置バッファー1						*/
#define D_MAIN_INV_FSBL_STARTBIT_BUF2				0x32						/*!< FSBL開始位置バッファー2						*/
#define D_MAIN_INV_FSBL_STARTBIT_BUF3				0x31						/*!< FSBL開始位置バッファー2						*/
#define D_MAIN_INV_FSBL_STARTBIT_BUF4				0x30						/*!< FSBL開始位置バッファー3						*/
#define D_MAIN_INV_FSBL_SIZE1						0x43						/*!< FSBLサイズ1									*/
#define D_MAIN_INV_FSBL_SIZE2						0x42						/*!< FSBLサイズ2									*/
#define D_MAIN_INV_FSBL_SIZE3						0x41						/*!< FSBLサイズ2									*/
#define D_MAIN_INV_FSBL_SIZE4						0x40						/*!< FSBLサイズ3									*/
#define D_MAIN_INV_FSBL_MASK1 						0xFF000000					/*!< FSBLデータ取得MASK1							*/
#define D_MAIN_INV_FSBL_MASK2 						0x00FF0000					/*!< FSBLデータ取得MASK2							*/
#define D_MAIN_INV_FSBL_MASK3 						0x0000FF00					/*!< FSBLデータ取得MASK3							*/
#define D_MAIN_INV_BOOTVER_SETPOSITION 				0x0000002C					/*!< Boot Version格納位置							*/

#define D_MAIN_INV_BOOTVER_SIZE						0x00000004					/*!< Boot Version Flash読み込みサイズ                           */
/* @} */

/* FLASH EOL対応 add start */
/*!
 * @name ユーザーIP QSPI初期化設定関連定義
 * @note 
 * @{
 */
#define D_MAIN_QSPI_PORING_CNT						100									/* QSPI Poring Count			*/
#define D_MAIN_QSPI_WAIT_TIME						10									/* QSPI Poring wait time		*/
#define D_MAIN_QSPI_FLAGSTATUS_CNT					25									/* Flag Status Register Count	*/ /* FLASH EOL対応 */
/* @} */
/* FLASH EOL対応 add end */

#endif
/* @} */
