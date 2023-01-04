/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_eep.h
 * @brief System共通定義
 * @date  2018/04/19	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_EEP_H
#define F_RRH_REG_EEP_H

/*!
 * @name EEPROMアドレスオフセット(共通部)
 * @note EEPROMアクセス用のアドレスオフセット
 * @{
 */
/* 2020/12/29 M&C) Merge 4G FHM src (start add) */
#define	D_RRH_EEP_CARD_NAME					0x0000				/* カード品名(ASCII)			*/
#define	D_RRH_CARD_NAME_LEN					12
#define	D_RRH_EEP_MAKE_DATE1				0x000C				/* 製造番号(年/月)				*/
#define	D_RRH_EEP_MAKE_DATE2				0x000D

#define	D_RRH_EEP_MAKE_SERIAL_H1			0x000E				/* 製造番号(シリアル上位)		*/
#define	D_RRH_EEP_MAKE_SERIAL_H2			0x000F
#define	D_RRH_EEP_MAKE_SERIAL_L1			0x0010				/* 製造番号(シリアル下位)		*/
#define	D_RRH_EEP_MAKE_SERIAL_L2			0x0011

#define	D_RRH_EEP_TEST_YEAR1				0x0012				/* 試験日(年)					*/
#define	D_RRH_EEP_TEST_YEAR2				0x0013
#define	D_RRH_EEP_TEST_MONDAY1				0x0014				/* 試験日(月日)					*/
#define	D_RRH_EEP_TEST_MONDAY2				0x0015

#define	D_RRH_EEP_HARD_VER1					0x0016				/* ハードバージョン(総合版数)	*/
#define	D_RRH_EEP_HARD_VER2					0x0017
#define	D_RRH_EEP_HARD_VER3					0x0018
#define	D_RRH_EEP_HARD_VER4					0x0019
#define	D_RRH_EEP_HARD_VER5					0x001A
#define	D_RRH_EEP_HARD_VER6					0x001B

#define	D_RRH_EEP_CARD_NO					0x001C				/* カード図番					*/
#define	D_RRH_CARD_NO_LEN					12
/* 2020/12/29 M&C) Merge 4G FHM src (end add) */

#define	D_RRH_I2C_EEP_DATANUM				0x0800


/* 装置種別<RE種別>定義	*/
#define	D_RRH_EEP_DEVICE_KIND0				0x0040				/* 装置種別						*/
#define	D_RRH_EEP_DEV_KIND_5GDU				1					/* 5GDU			*//***** plan to delete	*****/
#define	D_RRH_EEP_DEV_KIND_5GLLS_PROTO		0x10				/* 5G-LLS(Proto	*/
#define	D_RRH_EEP_DEV_KIND_5GDU_SUB6		0x13				/* 5G-DU商用(sub6) */
#define	D_RRH_EEP_DEV_KIND_5GDU_MMW		    0x14				/* 5G-DU商用(mmW)  */

#define	D_RRH_EEP_ERASE_ON					1
#define	D_RRH_EEP_ERASE_OFF					0
#define	D_RRH_EEP_WRITTING					0
#define	D_RRH_EEP_WRITE_OK					1

/* <対応周波数帯> 01:15G/02:17G/03:700M/04:2G/05:800M/06:3.5G */
#define	D_RRH_EEP_DEVICE_KIND1				0x0041				/* 装置種別<対応周波数帯>		*/
#define	D_RRH_EEP_DEV_FREQ_37G				1					/* 3.7G		*//***** plan to delete	*****/
#define	D_RRH_EEP_DEV_FREQ_46G				2					/* 4.6G		*//***** plan to delete	*****/
#define	D_RRH_EEP_DEV_FREQ_240G				3					/* 24G		*//***** plan to delete	*****/
#define	D_RRH_EEP_DEV_FREQ_47G				0x10				/* 3.7G		*/
#define	D_RRH_EEP_DEV_FREQ_28G				0x11				/* 28G		*/

#define	D_RRH_EEP_BOARD_VER					0x0042				/* ボードバージョン				*/
#define	D_RRH_EEP_BOARD_VER_PT01			0x01				/* PT01版	*/
#define	D_RRH_EEP_BOARD_VER_PT02			0x02				/* PT02版	*/
#define	D_RRH_EEP_BOARD_VER_PT03			0x03				/* PT03版	*/

#define	D_RRH_EEP_START_FLG					0x0050				/* FPGA/Firm共通 起動面情報1	*/

/* ファイルセット1 Flash 0面	*/
#define	D_RRH_EEP_FSET1_FLASH0_VER			0x0070				/* 代表バージョン				*/
#define	D_RRH_EEP_FSET1_FLASH0_F01_VER		0x0076				/* 個別ファイル01 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F02_VER		0x0078				/* 個別ファイル02 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F03_VER		0x007A				/* 個別ファイル03 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F04_VER		0x007C				/* 個別ファイル04 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F05_VER		0x007E				/* 個別ファイル05 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F06_VER		0x0080				/* 個別ファイル06 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F07_VER		0x0082				/* 個別ファイル07 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F08_VER		0x0084				/* 個別ファイル08 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F09_VER		0x0086				/* 個別ファイル09 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F10_VER		0x0088				/* 個別ファイル10 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F11_VER		0x008A				/* 個別ファイル11 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F12_VER		0x008C				/* 個別ファイル12 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F13_VER		0x008E				/* 個別ファイル13 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F14_VER		0x0090				/* 個別ファイル14 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F15_VER		0x0092				/* 個別ファイル15 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH0_F16_VER		0x0094				/* 個別ファイル16 バージョン	*/

#define	D_RRH_EEP_FSET1_FLASH0_F01_EFLG		0x0096				/* 個別ファイル01 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F02_EFLG		0x0097				/* 個別ファイル02 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F03_EFLG		0x0098				/* 個別ファイル03 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F04_EFLG		0x0099				/* 個別ファイル04 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F05_EFLG		0x009A				/* 個別ファイル05 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F06_EFLG		0x009B				/* 個別ファイル06 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F07_EFLG		0x009C				/* 個別ファイル07 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F08_EFLG		0x009D				/* 個別ファイル08 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F09_EFLG		0x009E				/* 個別ファイル09 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F10_EFLG		0x009F				/* 個別ファイル10 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F11_EFLG		0x00A0				/* 個別ファイル11 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F12_EFLG		0x00A1				/* 個別ファイル12 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F13_EFLG		0x00A2				/* 個別ファイル13 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F14_EFLG		0x00A3				/* 個別ファイル14 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F15_EFLG		0x00A4				/* 個別ファイル15 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH0_F16_EFLG		0x00A5				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット1 Flash 1面	*/
#define	D_RRH_EEP_FSET1_FLASH1_VER			0x00B0				/* 代表バージョン				*/
#define	D_RRH_EEP_FSET1_FLASH1_F01_VER		0x00B6				/* 個別ファイル01 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F02_VER		0x00B8				/* 個別ファイル02 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F03_VER		0x00BA				/* 個別ファイル03 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F04_VER		0x00BC				/* 個別ファイル04 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F05_VER		0x00BE				/* 個別ファイル05 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F06_VER		0x00C0				/* 個別ファイル06 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F07_VER		0x00C2				/* 個別ファイル07 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F08_VER		0x00C4				/* 個別ファイル08 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F09_VER		0x00C6				/* 個別ファイル09 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F10_VER		0x00C8				/* 個別ファイル10 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F11_VER		0x00CA				/* 個別ファイル11 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F12_VER		0x00CC				/* 個別ファイル12 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F13_VER		0x00CE				/* 個別ファイル13 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F14_VER		0x00D0				/* 個別ファイル14 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F15_VER		0x00D2				/* 個別ファイル15 バージョン	*/
#define	D_RRH_EEP_FSET1_FLASH1_F16_VER		0x00D4				/* 個別ファイル16 バージョン	*/

#define	D_RRH_EEP_FSET1_FLASH1_F01_EFLG		0x00D6				/* 個別ファイル01 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F02_EFLG		0x00D7				/* 個別ファイル02 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F03_EFLG		0x00D8				/* 個別ファイル03 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F04_EFLG		0x00D9				/* 個別ファイル04 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F05_EFLG		0x00DA				/* 個別ファイル05 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F06_EFLG		0x00DB				/* 個別ファイル06 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F07_EFLG		0x00DC				/* 個別ファイル07 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F08_EFLG		0x00DD				/* 個別ファイル08 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F09_EFLG		0x00DE				/* 個別ファイル09 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F10_EFLG		0x00DF				/* 個別ファイル10 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F11_EFLG		0x00E0				/* 個別ファイル11 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F12_EFLG		0x00E1				/* 個別ファイル12 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F13_EFLG		0x00E2				/* 個別ファイル13 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F14_EFLG		0x00E3				/* 個別ファイル14 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F15_EFLG		0x00E4				/* 個別ファイル15 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FLASH1_F16_EFLG		0x00E5				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット1 Flash Factory面	*/
#define	D_RRH_EEP_FSET1_FACTRY_VER			0x00F0				/* 代表バージョン				*/
#define	D_RRH_EEP_FSET1_FACTRY_F01_VER		0x00F6				/* 個別ファイル01 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F02_VER		0x00F8				/* 個別ファイル02 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F03_VER		0x00FA				/* 個別ファイル03 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F04_VER		0x00FC				/* 個別ファイル04 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F05_VER		0x00FE				/* 個別ファイル05 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F06_VER		0x0100				/* 個別ファイル06 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F07_VER		0x0102				/* 個別ファイル07 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F08_VER		0x0104				/* 個別ファイル08 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F09_VER		0x0106				/* 個別ファイル09 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F10_VER		0x0108				/* 個別ファイル10 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F11_VER		0x010A				/* 個別ファイル11 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F12_VER		0x010C				/* 個別ファイル12 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F13_VER		0x010E				/* 個別ファイル13 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F14_VER		0x0110				/* 個別ファイル14 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F15_VER		0x0112				/* 個別ファイル15 バージョン	*/
#define	D_RRH_EEP_FSET1_FACTRY_F16_VER		0x0114				/* 個別ファイル16 バージョン	*/

#define	D_RRH_EEP_FSET1_FACTRY_F01_EFLG		0x0116				/* 個別ファイル01 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F02_EFLG		0x0117				/* 個別ファイル02 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F03_EFLG		0x0118				/* 個別ファイル03 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F04_EFLG		0x0119				/* 個別ファイル04 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F05_EFLG		0x011A				/* 個別ファイル05 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F06_EFLG		0x011B				/* 個別ファイル06 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F07_EFLG		0x011C				/* 個別ファイル07 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F08_EFLG		0x011D				/* 個別ファイル08 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F09_EFLG		0x011E				/* 個別ファイル09 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F10_EFLG		0x011F				/* 個別ファイル10 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F11_EFLG		0x0120				/* 個別ファイル11 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F12_EFLG		0x0121				/* 個別ファイル12 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F13_EFLG		0x0122				/* 個別ファイル13 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F14_EFLG		0x0123				/* 個別ファイル14 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F15_EFLG		0x0124				/* 個別ファイル15 イレースフラグ*/
#define	D_RRH_EEP_FSET1_FACTRY_F16_EFLG		0x0125				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット2 Flash 0面	*/
#define	D_RRH_EEP_FSET2_FLASH0_VER			0x0130				/* 代表バージョン				*/
#define	D_RRH_EEP_FSET2_FLASH0_F01_VER		0x0136				/* 個別ファイル01 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F02_VER		0x0138				/* 個別ファイル02 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F03_VER		0x013A				/* 個別ファイル03 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F04_VER		0x013C				/* 個別ファイル04 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F05_VER		0x013E				/* 個別ファイル05 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F06_VER		0x0140				/* 個別ファイル06 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F07_VER		0x0142				/* 個別ファイル07 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F08_VER		0x0144				/* 個別ファイル08 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F09_VER		0x0146				/* 個別ファイル09 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F10_VER		0x0148				/* 個別ファイル10 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F11_VER		0x014A				/* 個別ファイル11 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F12_VER		0x014C				/* 個別ファイル12 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F13_VER		0x014E				/* 個別ファイル13 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F14_VER		0x0150				/* 個別ファイル14 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F15_VER		0x0152				/* 個別ファイル15 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH0_F16_VER		0x0154				/* 個別ファイル16 バージョン	*/

#define	D_RRH_EEP_FSET2_FLASH0_F01_EFLG		0x0156				/* 個別ファイル01 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F02_EFLG		0x0157				/* 個別ファイル02 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F03_EFLG		0x0158				/* 個別ファイル03 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F04_EFLG		0x0159				/* 個別ファイル04 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F05_EFLG		0x015A				/* 個別ファイル05 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F06_EFLG		0x015B				/* 個別ファイル06 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F07_EFLG		0x015C				/* 個別ファイル07 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F08_EFLG		0x015D				/* 個別ファイル08 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F09_EFLG		0x015E				/* 個別ファイル09 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F10_EFLG		0x015F				/* 個別ファイル10 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F11_EFLG		0x0160				/* 個別ファイル11 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F12_EFLG		0x0161				/* 個別ファイル12 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F13_EFLG		0x0162				/* 個別ファイル13 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F14_EFLG		0x0163				/* 個別ファイル14 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F15_EFLG		0x0164				/* 個別ファイル15 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH0_F16_EFLG		0x0165				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット2 Flash 1面	*/
#define	D_RRH_EEP_FSET2_FLASH1_VER			0x0170				/* 代表バージョン				*/
#define	D_RRH_EEP_FSET2_FLASH1_F01_VER		0x0176				/* 個別ファイル01 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F02_VER		0x0178				/* 個別ファイル02 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F03_VER		0x017A				/* 個別ファイル03 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F04_VER		0x017C				/* 個別ファイル04 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F05_VER		0x017E				/* 個別ファイル05 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F06_VER		0x0180				/* 個別ファイル06 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F07_VER		0x0182				/* 個別ファイル07 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F08_VER		0x0184				/* 個別ファイル08 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F09_VER		0x0186				/* 個別ファイル09 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F10_VER		0x0188				/* 個別ファイル10 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F11_VER		0x018A				/* 個別ファイル11 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F12_VER		0x018C				/* 個別ファイル12 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F13_VER		0x018E				/* 個別ファイル13 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F14_VER		0x0190				/* 個別ファイル14 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F15_VER		0x0192				/* 個別ファイル15 バージョン	*/
#define	D_RRH_EEP_FSET2_FLASH1_F16_VER		0x0194				/* 個別ファイル16 バージョン	*/

#define	D_RRH_EEP_FSET2_FLASH1_F01_EFLG		0x0196				/* 個別ファイル01 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F02_EFLG		0x0197				/* 個別ファイル02 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F03_EFLG		0x0198				/* 個別ファイル03 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F04_EFLG		0x0199				/* 個別ファイル04 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F05_EFLG		0x019A				/* 個別ファイル05 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F06_EFLG		0x019B				/* 個別ファイル06 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F07_EFLG		0x019C				/* 個別ファイル07 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F08_EFLG		0x019D				/* 個別ファイル08 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F09_EFLG		0x019E				/* 個別ファイル09 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F10_EFLG		0x019F				/* 個別ファイル10 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F11_EFLG		0x01A0				/* 個別ファイル11 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F12_EFLG		0x01A1				/* 個別ファイル12 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F13_EFLG		0x01A2				/* 個別ファイル13 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F14_EFLG		0x01A3				/* 個別ファイル14 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F15_EFLG		0x01A4				/* 個別ファイル15 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FLASH1_F16_EFLG		0x01A5				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット2 Flash Factory面	*/
#define	D_RRH_EEP_FSET2_FACTRY_VER			0x01B0				/* 代表バージョン				*/
#define	D_RRH_EEP_FSET2_FACTRY_F01_VER		0x01B6				/* 個別ファイル01 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F02_VER		0x01B8				/* 個別ファイル02 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F03_VER		0x01BA				/* 個別ファイル03 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F04_VER		0x01BC				/* 個別ファイル04 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F05_VER		0x01BE				/* 個別ファイル05 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F06_VER		0x01C0				/* 個別ファイル06 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F07_VER		0x01C2				/* 個別ファイル07 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F08_VER		0x01C4				/* 個別ファイル08 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F09_VER		0x01C6				/* 個別ファイル09 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F10_VER		0x01C8				/* 個別ファイル10 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F11_VER		0x01CA				/* 個別ファイル11 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F12_VER		0x01CC				/* 個別ファイル12 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F13_VER		0x01CE				/* 個別ファイル13 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F14_VER		0x01D0				/* 個別ファイル14 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F15_VER		0x01D2				/* 個別ファイル15 バージョン	*/
#define	D_RRH_EEP_FSET2_FACTRY_F16_VER		0x01D4				/* 個別ファイル16 バージョン	*/

#define	D_RRH_EEP_FSET2_FACTRY_F01_EFLG		0x01D6				/* 個別ファイル01 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F02_EFLG		0x01D7				/* 個別ファイル02 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F03_EFLG		0x01D8				/* 個別ファイル03 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F04_EFLG		0x01D9				/* 個別ファイル04 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F05_EFLG		0x01DA				/* 個別ファイル05 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F06_EFLG		0x01DB				/* 個別ファイル06 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F07_EFLG		0x01DC				/* 個別ファイル07 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F08_EFLG		0x01DD				/* 個別ファイル08 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F09_EFLG		0x01DE				/* 個別ファイル09 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F10_EFLG		0x01DF				/* 個別ファイル10 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F11_EFLG		0x01E0				/* 個別ファイル11 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F12_EFLG		0x01E1				/* 個別ファイル12 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F13_EFLG		0x01E2				/* 個別ファイル13 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F14_EFLG		0x01E3				/* 個別ファイル14 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F15_EFLG		0x01E4				/* 個別ファイル15 イレースフラグ*/
#define	D_RRH_EEP_FSET2_FACTRY_F16_EFLG		0x01E5				/* 個別ファイル16 イレースフラグ*/

/***** Ether(MT-PORT)情報	*****/
#define	D_RRH_EEP_MAC_ADDR_DBG_H1				0x0200			/* MACアドレス(ベンダ番号上位)	*/
#define	D_RRH_EEP_MAC_ADDR_DBG_H2				0x0201
#define	D_RRH_EEP_MAC_ADDR_DBG_M1				0x0202			/* MACアドレス(ベンダ番号下位)	*/
#define	D_RRH_EEP_MAC_ADDR_DBG_M2				0x0203
#define	D_RRH_EEP_MAC_ADDR_DBG_L1				0x0204			/* MACアドレス(ガード通番)		*/
#define	D_RRH_EEP_MAC_ADDR_DBG_L2				0x0205

#define	D_RRH_EEP_IP_ADDR_DBG_H1				0x0206			/* IPアドレス(上位)				*/
#define	D_RRH_EEP_IP_ADDR_DBG_H2				0x0207
#define	D_RRH_EEP_IP_ADDR_DBG_L1				0x0208			/* IPアドレス(下位)				*/
#define	D_RRH_EEP_IP_ADDR_DBG_L2				0x0209

/***** 25G eCPRI_Port#1情報	*****/
#define	D_RRH_EEP_MAC_ADDR_PORT1_H1				0x0220			/* MACアドレス(ベンダ番号上位)	*/
#define	D_RRH_EEP_MAC_ADDR_PORT1_H2				0x0221
#define	D_RRH_EEP_MAC_ADDR_PORT1_M1				0x0222			/* MACアドレス(ベンダ番号下位)	*/
#define	D_RRH_EEP_MAC_ADDR_PORT1_M2				0x0223
#define	D_RRH_EEP_MAC_ADDR_PORT1_L1				0x0224			/* MACアドレス(ガード通番)		*/
#define	D_RRH_EEP_MAC_ADDR_PORT1_L2				0x0225

#define	D_RRH_EEP_IP_ADDR_PORT1_H1				0x0226			/* IPアドレス(上位)				*/
#define	D_RRH_EEP_IP_ADDR_PORT1_H2				0x0227
#define	D_RRH_EEP_IP_ADDR_PORT1_L1				0x0228			/* IPアドレス(下位)				*/
#define	D_RRH_EEP_IP_ADDR_PORT1_L2				0x0229

/***** 25G eCPRI_Port#2情報	*****/
#define	D_RRH_EEP_MAC_ADDR_PORT2_H1				0x0260			/* MACアドレス(ベンダ番号上位)	*/
#define	D_RRH_EEP_MAC_ADDR_PORT2_H2				0x0261
#define	D_RRH_EEP_MAC_ADDR_PORT2_M1				0x0262			/* MACアドレス(ベンダ番号下位)	*/
#define	D_RRH_EEP_MAC_ADDR_PORT2_M2				0x0263
#define	D_RRH_EEP_MAC_ADDR_PORT2_L1				0x0264			/* MACアドレス(ガード通番)		*/
#define	D_RRH_EEP_MAC_ADDR_PORT2_L2				0x0265

#define	D_RRH_EEP_IP_ADDR_PORT2_H1				0x0266			/* IPアドレス(上位)				*/
#define	D_RRH_EEP_IP_ADDR_PORT2_H2				0x0267
#define	D_RRH_EEP_IP_ADDR_PORT2_L1				0x0268			/* IPアドレス(下位)				*/
#define	D_RRH_EEP_IP_ADDR_PORT2_L2				0x0269

/***** システムパラメータ	*****/
#define	D_RRH_EEP_VID_LOW_PORT1_H				0x0320			/* Low VID						*/
#define	D_RRH_EEP_VID_LOW_PORT1_L				0x0321
#define	D_RRH_EEP_VID_HIGH_PORT1_H				0x0322			/* High VID						*/
#define	D_RRH_EEP_VID_HIGH_PORT1_L				0x0323
#define	D_RRH_EEP_VID_LAST_PORT1_H				0x0324			/* Last Used VID				*/
#define	D_RRH_EEP_VID_LAST_PORT1_L				0x0325

/***** デバッグ情報			*****/
#define	D_RRH_EEP_LLSB_DBG_FLAG					0x03c0			/* LLSボードフラグ				*/
#define	D_RRH_EEP_DHCP_ENT_FLAG					0x03c1			/* DHCP有無フラグ				*/
#define D_RRH_EEP_SYNCE_SND_FLAG				0x03c8			/* SyncE ESMC送信フラグ			*/

#define D_RRH_EEP_SYNCE_NOACT					0x01			/* ESMC(S-Plane)送信を無効化	*/

#define	D_RRH_EEP_PC_CONNECT_FLAG				0x03e0			/* PC接続フラグ					*/

#define	D_RRH_EEP_IP_ADDR_ACCESS_H1				0x03e2			/* 接続先IPアドレス(上位)		*/
#define	D_RRH_EEP_IP_ADDR_ACCESS_H2				0x03e3
#define	D_RRH_EEP_IP_ADDR_ACCESS_L1				0x03e4			/* 接続先IPアドレス(下位)		*/
#define	D_RRH_EEP_IP_ADDR_ACCESS_L2				0x03e5

/***** SFN/FRM補正計算関連	*****/
#define	D_RRH_EEP_SFN_RFM_FLAG					0x03f0			/* SFN/RFM補正フラグ			*/
#define	D_RRH_EEP_OFFSET_A_H					0x03f1			/* offset_a						*/
#define	D_RRH_EEP_OFFSET_A_M					0x03f2
#define	D_RRH_EEP_OFFSET_A_L					0x03f3
#define	D_RRH_EEP_OFFSET_B_H					0x03f4			/* offset_b						*/
#define	D_RRH_EEP_OFFSET_B_L					0x03f5

/***** ★ 既存define ★ *****/
#define	D_RRH_EEP_FLASH0_SOFT_VER			0x0028				/* Flash 0面ソフトバージョン(ver)	*/
#define	D_RRH_EEP_FLASH0_SOFT_YEAR			0x002A				/* Flash 0面ソフトバージョン(年)	*/
#define	D_RRH_EEP_FLASH0_SOFT_MONDAY		0x002C				/* Flash 0面ソフトバージョン(月)	*/
#define	D_RRH_EEP_FLASH1_SOFT_VER			0x002E				/* Flash 1面ソフトバージョン(ver)	*/
#define	D_RRH_EEP_FLASH1_SOFT_YEAR			0x0030				/* Flash 1面ソフトバージョン(年)	*/
#define	D_RRH_EEP_FLASH1_SOFT_MONDAY		0x0032				/* Flash 1面ソフトバージョン(月)	*/
#define	D_RRH_FLASH_SOFT_VER_LEN			6

#define	D_RRH_EEP_FACTORY_MNG00				0x0034				/* 工場管理用0					*/
#define	D_RRH_EEP_FACTORY_MNG01				0x0035
#define	D_RRH_EEP_FACTORY_MNG10				0x0036				/* 工場管理用1					*/
#define	D_RRH_EEP_FACTORY_MNG11				0x0037
#define	D_RRH_EEP_FACTORY_MNG20				0x0038				/* 工場管理用2					*/
#define	D_RRH_EEP_FACTORY_MNG21				0x0039
#define	D_RRH_EEP_FACTORY_MNG30				0x003A				/* 工場管理用3					*/
#define	D_RRH_EEP_FACTORY_MNG31				0x003B

#define	D_RRH_EEP_MAC_VEND_H1				0x003C				/* MACアドレス(ベンダ番号上位)	*/
#define	D_RRH_EEP_MAC_VEND_H2				0x003D
#define	D_RRH_EEP_MAC_VEND_L1				0x003E				/* MACアドレス(ベンダ番号下位)	*/
#define	D_RRH_EEP_MAC_VEND_L2				0x003F

/* 2020/12/29 M&C) Merge 4G FHM src (start add) */
#define D_RRH_EEP_IP_ADDER_3G_H1            0x0042              /**< IPアドレス(上位)(3G) */
#define D_RRH_EEP_IP_ADDER_3G_H2            0x0043              /**< IPアドレス(上位)(3G) */
#define D_RRH_EEP_IP_ADDER_3G_L1            0x0044              /**< IPアドレス(下位)(3G) */
#define D_RRH_EEP_IP_ADDER_3G_L2            0x0045              /**< IPアドレス(下位)(3G) */

#define D_RRH_EEP_NETMASK_3G_H1             0x0046              /**< ネットマスク(上位)(3G) */
#define D_RRH_EEP_NETMASK_3G_H2             0x0047              /**< ネットマスク(上位)(3G) */
#define D_RRH_EEP_NETMASK_3G_L1             0x0048              /**< ネットマスク(下位)(3G) */
#define D_RRH_EEP_NETMASK_3G_L2             0x0049              /**< ネットマスク(下位)(3G) */
/* 2020/12/29 M&C) Merge 4G FHM src (end add) */

#define	D_RRH_EEP_DL_ERASE_FLG_FIRM0		0x0054				/* DL用Flashイレースフラグ(FIRM)	*/
#define	D_RRH_EEP_DL_ERASE_FLG_FIRM1		0x0055
#define	D_RRH_EEP_DL_ERASE_FLG_FPGA0		0x0056				/* DL用Flashイレースフラグ(FPGA)	*/
#define	D_RRH_EEP_DL_ERASE_FLG_FPGA1		0x0057

#define D_RRH_EEP_DL_ERASE_FLG_RFTX0		0x0058				/* DL用Flashイレースフラグ(TX側RFデバイス) */
#define D_RRH_EEP_DL_ERASE_FLG_RFTX1		0x0059

#define D_RRH_EEP_DL_ERASE_FLG_RFRX0		0x005a				/* DL用Flashイレースフラグ(RX側RFデバイス) */
#define D_RRH_EEP_DL_ERASE_FLG_RFRX1		0x005b

#define	D_RRH_EEP_FLASH0_FPGA_VER00			0x005C				/* Flash0面FPGA代表バージョン0						*/
#define	D_RRH_EEP_FLASH0_FPGA_VER01			0x005D
#define	D_RRH_EEP_FLASH0_FPGA_FPGA			0x005C				/* Flash0面FPGA代表バージョン0						*/

#define	D_RRH_EEP_FLASH0_FPGA_VER10			0x005E				/* Flash0面FPGA代表バージョン1(TX側RFデバイス)		*/
#define	D_RRH_EEP_FLASH0_FPGA_VER11			0x005F
#define	D_RRH_EEP_FLASH0_FPGA_TX			0x005E				/* Flash0面FPGA代表バージョン1(TX側RFデバイス)		*/

#define	D_RRH_EEP_FLASH0_FPGA_VER20			0x0060				/* Flash0面FPGA代表バージョン2(RX側RFデバイス)		*/
#define	D_RRH_EEP_FLASH0_FPGA_VER21			0x0061
#define	D_RRH_EEP_FLASH0_FPGA_RX			0x0060

#define	D_RRH_EEP_FLASH1_FPGA_VER00			0x0062				/* Flash1面FPGA代表バージョン0						*/
#define	D_RRH_EEP_FLASH1_FPGA_VER01			0x0063
#define	D_RRH_EEP_FLASH1_FPGA_FPGA			0x0062				/* Flash1面FPGA代表バージョン0						*/

#define	D_RRH_EEP_FLASH1_FPGA_VER10			0x0064				/* Flash1面FPGA代表バージョン1(TX側RFデバイス)		*/
#define	D_RRH_EEP_FLASH1_FPGA_VER11			0x0065
#define	D_RRH_EEP_FLASH1_FPGA_TX			0x0064				/* Flash1面FPGA代表バージョン1(TX側RFデバイス)		*/

#define	D_RRH_EEP_FLASH1_FPGA_VER20			0x0066				/* Flash1面FPGA代表バージョン2(RX側RFデバイス)		*/
#define	D_RRH_EEP_FLASH1_FPGA_VER21			0x0067
#define	D_RRH_EEP_FLASH1_FPGA_RX			0x0066

// #define	D_RRH_EEP_START_FLG					0x0068				/* FPGA/Firm共通 起動面情報							*/
#define	D_RRH_EEP_START_FLG_ENA				0x0069				/* FPGA/Firm共通 起動面情報 有効BIT					*/
#define	D_RRH_EEP_CPRI_START_FLG0			0x006a				/* CPRI-FPGA起動面情報								*/
#define	D_RRH_EEP_CPRI_START_FLG1			0x006b				
#define	D_RRH_EEP_VUPU_START_FLG0			0x006c				/* VUPU起動面情報									*/
#define	D_RRH_EEP_VUPU_START_FLG1			0x006d				

/* 2020/12/29 M&C) Merge 4G FHM src (start add) */
/* 装置種別<RE種別>定義	*/
#define	D_RRH_EEP_DEV_KIND_RRE				0					/* RRE								*/
#define	D_RRH_EEP_DEV_KIND_SRE				1					/* SRE								*/
#define	D_RRH_EEP_DEV_KIND_SLC				2					/* SLC								*/
#define	D_RRH_EEP_DEV_KIND_1CHIP			3					/* 1CHIP							*/
#define	D_RRH_EEP_DEV_KIND_SRE_TDD			4					/* 3.5G_TDD_SRE(arria10)			*/
#define	D_RRH_EEP_DEV_KIND_RRE_TDD			5					/* 3.5G_TDD_RRE(まだ未定)			*/
#define	D_RRH_EEP_DEV_KIND_FHM				0x10				/* FHM								*/
/* 2020/12/29 M&C) Merge 4G FHM src (end add) */

#define	D_RRH_EEP_SYSPARA_FLG0				0x0072				/* システムパラメータ受信フラグ		*/
#define	D_RRH_EEP_SYSPARA_FLG1				0x0073

#define	D_RRH_EEP_TILT_CONNECT_INFO0		0x0074				/* TILT接続情報						*/
#define	D_RRH_EEP_TILT_CONNECT_INFO1		0x0075

#define	D_RRH_EEP_OPTICAL_TYPE0				0x0076				/* 光タイプ							*/
#define	D_RRH_EEP_OPTICAL_TYPE1				0x0077

#define	D_RRH_EEP_IP_ADDER_S3G_H1			0x0078				/* IPアドレス(上位) (S3G)			*/
#define	D_RRH_EEP_IP_ADDER_S3G_H2			0x0079
#define	D_RRH_EEP_IP_ADDER_S3G_L1			0x007A				/* IPアドレス(下位) (S3G)			*/
#define	D_RRH_EEP_IP_ADDER_S3G_L2			0x007B

#define	D_RRH_EEP_NETMASK_S3G_H1			0x007C				/* ネットマスク(上位) (S3G)			*/
#define	D_RRH_EEP_NETMASK_S3G_H2			0x007D
#define	D_RRH_EEP_NETMASK_S3G_L1			0x007E				/* ネットマスク(下位) (S3G)			*/
#define	D_RRH_EEP_NETMASK_S3G_L2			0x007F

#define	D_RRH_EEP_DEF_ROUTER_S3G_H1			0x0080				/* デフォルトルータ(上位) (S3G)		*/
#define	D_RRH_EEP_DEF_ROUTER_S3G_H2			0x0081
#define	D_RRH_EEP_DEF_ROUTER_S3G_L1			0x0082				/* デフォルトルータ(下位) (S3G)		*/
#define	D_RRH_EEP_DEF_ROUTER_S3G_L2			0x0083

#define	D_RRH_EEP_CMD_PORT_S3G1				0x0084				/* コマンド転送ポート番号 (S3G)		*/
#define	D_RRH_EEP_CMD_PORT_S3G2				0x0085

#define	D_RRH_EEP_CMD_PORT_3G1				0x0084				/* コマンド転送ポート番号 (3G)		*/
#define	D_RRH_EEP_CMD_PORT_3G2				0x0085

#define	D_RRH_EEP_FILE_PORT_S3G1			0x0086				/* ファイル転送ポート番号 (S3G)		*/
#define	D_RRH_EEP_FILE_PORT_S3G2			0x0087

#define	D_RRH_EEP_FILE_PORT_3G1				0x0086				/* ファイル転送ポート番号 (3G)		*/
#define	D_RRH_EEP_FILE_PORT_3G2				0x0087

#define	D_RRH_EEP_SM_PORT_3G1				0x0088				/* ポート番号 (S3G)					*/
#define	D_RRH_EEP_SM_PORT_3G2				0x0089


#define	D_RRH_EEP_SM_PORT_S3G1				0x0088				/* ポート番号 (S3G)					*/
#define	D_RRH_EEP_SM_PORT_S3G2				0x0089

/* SRE対応 - Start */
#define	D_RRH_EEP_ROLL_BACK_INFO			0x009E				/* RollBackInfo						 */
/* SRE対応 - End */

#define	D_RRH_EEP_TEMP_ALM_THRESHOLD		0x00A0				/* 温度ALM閾値						*/

#define D_RRH_EEP_DL_ERASE_FLG_APDIC0		0x00B0				/* DL用Flashイレースフラグ(APD-IC Boot) */
#define D_RRH_EEP_DL_ERASE_FLG_APDIC1		0x00B1

#define	D_RRH_EEP_CHECK_SUM0				0x00FC				/* EEPROMチェックサム				*/
#define	D_RRH_EEP_CHECK_SUM1				0x00FD

#define	D_RRH_EEP_VERSION0					0x00FE				/* EEPROMバージョン					*/
#define	D_RRH_EEP_VERSION1					0x00FF

#define	D_RRH_EEP_TEMPALM					0x00A0				/* 温度ALM閾値 */ /* 2020/12/29 M&C) Merge 4G FHM src (add) */

#define	D_RRH_EEP_RE_PRIORITY_SYS_INFO0		0x0100				/* RE優先システム情報				*/
#define	D_RRH_EEP_RE_PRIORITY_SYS_INFO1		0x0101

#define	D_RRH_EEP_S3G_BANDWIDTH0			0x0102				/* S3G帯域幅						*/
#define	D_RRH_EEP_S3G_BANDWIDTH1			0x0103

#define	D_RRH_EEP_3G_CAR_FLG0				0x0104				/* 3Gキャリア運用有無				*/
#define	D_RRH_EEP_3G_CAR_FLG1				0x0105

#define	D_RRH_EEP_TX_SYSTEM_NUM0			0x0106				/* 送信系統数						*/
#define	D_RRH_EEP_TX_SYSTEM_NUM1			0x0107

#define	D_RRH_EEP_SEC_NCHIP_234_H			0x0108				/* セクタnチップ補正(2、3、4系) D15-D8 */
#define	D_RRH_EEP_SEC_NCHIP_234_L			0x0109				/* セクタnチップ補正(2、3、4系) D7-D0  */

#define	D_RRH_EEP_SEC_NCHIP_567_H			0x010A				/* セクタnチップ補正(5、6、7系) D15-D8 */
#define	D_RRH_EEP_SEC_NCHIP_567_L			0x010B				/* セクタnチップ補正(5、6、7系) D7-D0  */

#define	D_RRH_EEP_CAR_FRQ10					0x0110				/* 3Gキャリア#1運用周波数			*/
#define	D_RRH_EEP_CAR_FRQ11					0x0111
#define	D_RRH_EEP_CAR_FRQ20					0x0112				/* 3Gキャリア#2運用周波数			*/
#define	D_RRH_EEP_CAR_FRQ21					0x0113
#define	D_RRH_EEP_CAR_FRQ30					0x0114				/* 3Gキャリア#3運用周波数			*/
#define	D_RRH_EEP_CAR_FRQ31					0x0115
#define	D_RRH_EEP_CAR_FRQ40					0x0116				/* 3Gキャリア#4運用周波数			*/
#define	D_RRH_EEP_CAR_FRQ41					0x0117

#define	D_RRH_EEP_MT_EXT_FLG0				0x0118				/* 対3G用MT信号/外部装置データ対応有無	*/
#define	D_RRH_EEP_MT_EXT_FLG1				0x0119

#define	D_RRH_EEP_SYSPARA_TIMER0			0x011A				/* システムパラメータ転送待ちタイマ		*/
#define	D_RRH_EEP_SYSPARA_TIMER1			0x011B

#define	D_RRH_EEP_FIRM_SEND_TIMER0			0x011C				/* ファームウェア送信確認タイマ(更新用)	*/
#define	D_RRH_EEP_FIRM_SEND_TIMER1			0x011D

#define	D_RRH_EEP_VUPU_WRITE_FLAG0			0x0120				/* VUPU用FLASHエリア書込み有無フラグ */
#define	D_RRH_EEP_VUPU_WRITE_FLAG1			0x0121

#define	D_RRH_EEP_VUPU_FLASH0_VER0			0x0122				/* Flash0面VUPU代表バージョン */
#define	D_RRH_EEP_VUPU_FLASH0_VER1			0x0123

#define	D_RRH_EEP_VUPU_FLASH1_VER0			0x0124				/* Flash1面VUPU代表バージョン */
#define	D_RRH_EEP_VUPU_FLASH1_VER1			0x0125

#define	D_RRH_EEP_VUPU_SIZE_H0				0x0126				/* VUPUサイズ格納(上位) */
#define	D_RRH_EEP_VUPU_SIZE_H1				0x0127

#define	D_RRH_EEP_VUPU_SIZE_L0				0x0128				/* VUPUサイズ格納(下位) */
#define	D_RRH_EEP_VUPU_SIZE_L1				0x0129

#define	D_RRH_EEP_VUPU_ERASE_TMP0			0x012C				/* DL用Flashイレースフラグ(VUPU-TMP-FLASH(QSPI側)用) */
#define	D_RRH_EEP_VUPU_ERASE_TMP1			0x012D

#define	D_RRH_EEP_VUPU_ERASE_0				0x012E				/* DL用Flashイレースフラグ(VUPU用FLASH) */
#define	D_RRH_EEP_VUPU_ERASE_1				0x012F

#define	D_RRH_EEP_FIRM0_CHECKSUM_CALC_H0	0x0130				/* チェックサム計算用サイズ(上位)MPUファームウェア FLASH 0面 */
#define	D_RRH_EEP_FIRM0_CHECKSUM_CALC_H1	0x0131
#define	D_RRH_EEP_FIRM0_CHECKSUM_CALC_L0	0x0132				/* チェックサム計算用サイズ(下位)MPUファームウェア FLASH 0面 */
#define	D_RRH_EEP_FIRM0_CHECKSUM_CALC_L1	0x0133

#define	D_RRH_EEP_FPGA0_CHECKSUM_CALC_H0	0x0134				/* チェックサム計算用サイズ(上位)CPRI-FPGA(RRE) FLASH 0面	*/
#define	D_RRH_EEP_FPGA0_CHECKSUM_CALC_H1	0x0135
#define	D_RRH_EEP_FPGA0_CHECKSUM_CALC_L0	0x0136				/* チェックサム計算用サイズ(下位)CPRI-FPGA(RRE) FLASH 0面	*/
#define	D_RRH_EEP_FPGA0_CHECKSUM_CALC_L1	0x0137

#define	D_RRH_EEP_RFTX0_CHECKSUM_CALC_H0	0x0138				/* チェックサム計算用サイズ(上位)TX側RFデバイス FLASH 0面	*/
#define	D_RRH_EEP_RFTX0_CHECKSUM_CALC_H1	0x0139
#define	D_RRH_EEP_RFTX0_CHECKSUM_CALC_L0	0x013a				/* チェックサム計算用サイズ(下位)TX側RFデバイス FLASH 0面	*/
#define	D_RRH_EEP_RFTX0_CHECKSUM_CALC_L1	0x013b

#define	D_RRH_EEP_RFRX0_CHECKSUM_CALC_H0	0x013c				/* チェックサム計算用サイズ(上位)RX側RFデバイス FLASH 0面	*/
#define	D_RRH_EEP_RFRX0_CHECKSUM_CALC_H1	0x013d
#define	D_RRH_EEP_RFRX0_CHECKSUM_CALC_L0	0x013e				/* チェックサム計算用サイズ(下位)RX側RFデバイス FLASH 0面	*/
#define	D_RRH_EEP_RFRX0_CHECKSUM_CALC_L1	0x013f

#define	D_RRH_EEP_APDIC0_CHECKSUM_CALC_H0	0x0140				/* チェックサム計算用サイズ(上位)APD-IC Boot FLASH 0面		*/
#define	D_RRH_EEP_APDIC0_CHECKSUM_CALC_H1	0x0141
#define	D_RRH_EEP_APDIC0_CHECKSUM_CALC_L0	0x0142				/* チェックサム計算用サイズ(下位)APD-IC Boot FLASH 0面		*/
#define	D_RRH_EEP_APDIC0_CHECKSUM_CALC_L1	0x0143


#define	D_RRH_EEP_FIRM1_CHECKSUM_CALC_H0	0x0144				/* チェックサム計算用サイズ(上位)MPUファームウェアFLASH1面	*/
#define	D_RRH_EEP_FIRM1_CHECKSUM_CALC_H1	0x0145
#define	D_RRH_EEP_FIRM1_CHECKSUM_CALC_L0	0x0146				/* チェックサム計算用サイズ(下位)MPUファームウェアFLASH1面	*/
#define	D_RRH_EEP_FIRM1_CHECKSUM_CALC_L1	0x0147

#define	D_RRH_EEP_FPGA1_CHECKSUM_CALC_H0	0x0148				/* チェックサム計算用サイズ(上位)CPRI-FPGA(RRE) FLASH 1面	*/
#define	D_RRH_EEP_FPGA1_CHECKSUM_CALC_H1	0x0149
#define	D_RRH_EEP_FPGA1_CHECKSUM_CALC_L0	0x014A				/* チェックサム計算用サイズ(下位)CPRI-FPGA(RRE) FLASH 1面	*/
#define	D_RRH_EEP_FPGA1_CHECKSUM_CALC_L1	0x014B

#define	D_RRH_EEP_RFTX1_CHECKSUM_CALC_H0	0x014C				/* チェックサム計算用サイズ(上位)TX側RFデバイス FLASH 1面	*/
#define	D_RRH_EEP_RFTX1_CHECKSUM_CALC_H1	0x014D
#define	D_RRH_EEP_RFTX1_CHECKSUM_CALC_L0	0x014E				/* チェックサム計算用サイズ(下位)TX側RFデバイス FLASH 1面	*/
#define	D_RRH_EEP_RFTX1_CHECKSUM_CALC_L1	0x014F

#define	D_RRH_EEP_RFRX1_CHECKSUM_CALC_H0	0x0150				/* チェックサム計算用サイズ(上位)RX側RFデバイス FLASH 1面	*/
#define	D_RRH_EEP_RFRX1_CHECKSUM_CALC_H1	0x0151
#define	D_RRH_EEP_RFRX1_CHECKSUM_CALC_L0	0x0152				/* チェックサム計算用サイズ(下位)RX側RFデバイス FLASH 1面	*/
#define	D_RRH_EEP_RFRX1_CHECKSUM_CALC_L1	0x0153

#define	D_RRH_EEP_APDIC1_CHECKSUM_CALC_H0	0x0154				/* チェックサム計算用サイズ(上位)APD-IC Boot FLASH 1面		*/
#define	D_RRH_EEP_APDIC1_CHECKSUM_CALC_H1	0x0155
#define	D_RRH_EEP_APDIC1_CHECKSUM_CALC_L0	0x0156				/* チェックサム計算用サイズ(下位)APD-IC Boot FLASH 1面		*/
#define	D_RRH_EEP_APDIC1_CHECKSUM_CALC_L1	0x0157

#define	D_RRH_EEP_BOOTVER_H0_PR				0x0160				/* BOOT版数(0面 Primary)	*/
#define	D_RRH_EEP_BOOTVER_L0_PR				0x0161				/* BOOT版数(0面 Primary)	*/
#define	D_RRH_EEP_BOOTVER_H0_BK				0x0162				/* BOOT版数(0面 Backup)		*/
#define	D_RRH_EEP_BOOTVER_L0_BK				0x0163				/* BOOT版数(0面 Backup)		*/
#define	D_RRH_EEP_BOOTVER_H1_PR				0x0164				/* BOOT版数(1面 Primary)	*/
#define	D_RRH_EEP_BOOTVER_L1_PR				0x0165				/* BOOT版数(1面 Primary)	*/
#define	D_RRH_EEP_BOOTVER_H1_BK				0x0166				/* BOOT版数(1面 Backup)		*/
#define	D_RRH_EEP_BOOTVER_L1_BK				0x0167				/* BOOT版数(1面 Backup)		*/

/* 2020/12/29 M&C) Merge 4G FHM src (start add) */
#define	D_RRH_EEP_STA_BOOTVER_H0_PR			0x0168				/* 起動時BOOT版数(0面 Primary)	*/
#define	D_RRH_EEP_STA_BOOTVER_L0_PR			0x0169				/* 起動時BOOT版数(0面 Primary)	*/
#define	D_RRH_EEP_STA_BOOTVER_H0_BK			0x016A				/* 起動時BOOT版数(0面 Backup)	*/
#define	D_RRH_EEP_STA_BOOTVER_L0_BK			0x016B				/* 起動時BOOT版数(0面 Backup)	*/
#define	D_RRH_EEP_STA_BOOTVER_H1_PR			0x016C				/* 起動時BOOT版数(1面 Primary)	*/
#define	D_RRH_EEP_STA_BOOTVER_L1_PR			0x016D				/* 起動時BOOT版数(1面 Primary)	*/
#define	D_RRH_EEP_STA_BOOTVER_H1_BK			0x016E				/* 起動時BOOT版数(1面 Backup)	*/
#define	D_RRH_EEP_STA_BOOTVER_L1_BK			0x016F				/* 起動時BOOT版数(1面 Backup)	*/

/** 局ON/OFF情報(TILT-INF信号制御) */
#define D_RRH_EEP_BASE_ONOFF                0x017F

/**
 * 配下RE#n ON/OFF情報(TILT-INF信号制御)取得マクロ
 * @note D_RRH_CPRINO_RE1 ... のマクロを引数に渡してください
 */
#define M_RRH_EEP_RE_ONOFF(cprino) (UINT)(0x180 + (cprino-1))

/**
 * Master CPRIリンク#n有効/無効(DIP-SW)
 * @note D_RRH_CPRINO_RE1 ... のマクロを引数に渡してください
 */
#define M_RRH_EEP_CPRILINK_ENABLE(cprino) (UINT)(0x1A0 + (cprino-1))

#define M_RRH_EEP_REPORTIN_CPRI_NO(bitno) (UINT)(0x1B0 + (bitno))
#define M_RRH_EEP_REPORTIN_RE_BIT(bitno)  (UINT)(0x1C0 + (bitno))
/* 2020/12/29 M&C) Merge 4G FHM src (end add) */

#define	D_RRH_EEP_TROUBLE_LOG				0x3D3				/* 障害ログ退避面						*/
#define	D_RRH_EEP_CYCLIC_LOG				0x3D4				/* サイクリックログ退避面				*/
#define	D_RRH_EEP_CYCLIC_CLEAR_FLG0			0x3D5				/* サイクリックログイレースフラグ0面用	*/
#define	D_RRH_EEP_CYCLIC_CLEAR_FLG1			0x3D6				/* サイクリックログイレースフラグ1面用	*/

#define	D_RRH_EEP_SOFT_RESET_FLG			0x0205				/* ソフトリセットフラグ			*/
#define	D_RRH_EEP_ALM_SELF_RESET_FLG		0x0206				/* ALM自律リセット有無フラグ	*/

#define	D_RRH_EEP_GAIN_ADJUST_SET_A			0x0207				/* ゲイン微調設定値(Ant#A)		*/
#define	D_RRH_EEP_GAIN_ADJUST_SET_B			0x0208				/* ゲイン微調設定値(Ant#B)		*/
#define	D_RRH_EEP_GAIN_ADJUST_SET_C			0x0209				/* ゲイン微調設定値(Ant#C)		*/
#define	D_RRH_EEP_GAIN_ADJUST_SET_D			0x020A				/* ゲイン微調設定値(Ant#D)		*/

#define	D_RRH_EEP_REAL_LOG_IP_FIL0			0x0210				/* リアル通信ログIPフィルタ		*/
#define	D_RRH_EEP_REAL_LOG_IP_FIL1			0x0211
#define	D_RRH_EEP_REAL_LOG_IP_FIL2			0x0212
#define	D_RRH_EEP_REAL_LOG_IP_FIL3			0x0213

#define	D_RRH_EEP_REAL_LOG_OUTPUT			0x0214				/* リアル通信ログ出力先			*/

#define	D_RRH_EEP_SOFT_RESET_MASK			0x0215				/* ソフトリセットマスク			*/
#define	D_RRH_EEP_SOFT_BOOTMMODE			0x0216				/* ソフト起動モード				*/
#define	D_RRH_SIMMODE_L2					0x10				/* L2のみSIM					*/
#define	D_RRH_SIMMODE_PL					0x20				/* PLのみSIM					*/
#define	D_RRH_SIMMODE_BOTH					0x30				/* L2PLがSIM					*/

#define	D_RRH_EEP_CPRI_CTLE_1				0x021C				/* CPRI CTLEパラメータ1			*/
#define	D_RRH_EEP_CPRI_CTLE_2				0x021D				/* CPRI CTLEパラメータ2			*/
#define	D_RRH_EEP_CPRI_CTLE_3				0x021E				/* CPRI CTLEパラメータ3			*/
#define	D_RRH_EEP_CPRI_DCD_REVISION			0x021F				/* CPRI DCD補正パラメータ		*/
/*　DCM-35G_ハードソフト仕様書(Zynq版).docx 1.13ハソ変　対応　ここから */
#define	D_RRH_EEP_FAN_IMPLEMENTS			0x021B				/* FAN 有効/無効(RE-MT制御)  下位1bit(接続有無) 1：有効，0：無効 */	
/*　DCM-35G_ハードソフト仕様書(Zynq版).docx 1.13ハソ変　対応　ここまで */ 
#define	D_RRH_EEP_PA_ONOFF_SWITCH			0x0220				/* PA-ON/OFFスイッチ情報		*/

#define	D_RRH_EEP_STATIC_CPRI_RATE_MODE		0x0221				/* 9.8G強制切り替えモード		*/

#define	D_RRH_EEP_TIMESAVE_FLG				0x0222				/* 時刻退避フラグ				*/
#define	D_RRH_EEP_TIMESAVE_YEAR1			0x0223				/* 時刻退避情報					*/
#define	D_RRH_EEP_TIMESAVE_YEAR2			0x0224
#define	D_RRH_EEP_TIMESAVE_MONTH			0x0225
#define	D_RRH_EEP_TIMESAVE_DAY				0x0226
#define	D_RRH_EEP_TIMESAVE_TIME				0x0227
#define	D_RRH_EEP_TIMESAVE_MIN				0x0228
#define	D_RRH_EEP_TIMESAVE_SEC				0x0229
#define	D_RRH_EEP_TIMESAVE_10MSEC			0x022A
#define	D_RRH_EEP_FLASH_FALESAFE			0x0236				/* FLASHフェールセーフ抑止フラグ */ /* 2020/12/29 M&C) Merge 4G FHM src (add) */

#define	D_RRH_EEP_WDTCOUNTER				0x0230				/* 過去のWDT発生回数									*/
#define	D_RRH_EEP_WDTLOG_LASTFACE			0x0231				/* WDTログ最終書き込み面								*/
#define	D_RRH_EEP_WDTLOG_STATUS0			0x0232				/* WDTログ状態(0面)	1:書き込み済,1以外書き込み未		*/
#define	D_RRH_EEP_WDTLOG_STATUS1			0x0233				/* WDTログ状態(1面)	1:書き込み済,1以外書き込み未		*/

#define	D_RRH_EEP_TP_DEBUG0					0x0300				/* TPデバック					*/
#define	D_RRH_EEP_TP_DEBUG1					0x0301
/* @} */

/*!
 * @name EEPROMアドレスオフセット(トレーニング部)
 * @note EEPROMアクセス用のアドレスオフセット
 * @{
 */
#define	D_RRH_EEP_TRA_TXCNT						0x200			/* トレーニングデータ数(Tx)		*/
#define	D_RRH_EEP_TRA_RXCNT						0x200			/* トレーニングデータ数(Rx)		*/

#define	D_RRH_EEP_TRA_TX_ANT_OFFSET				0x40			/* アンテナ間オフセット			*/
#define	D_RRH_EEP_TRA_TX_ANT_OFFSET_RRE			0x80			/* アンテナ間オフセット(RRE用)	*/

/********************/
/* Tx関連設定情報	*/
/********************/
#define	D_RRH_EEP_TRA_TX_LEN					0x0400			/* TXのLength情報				*/
#define	D_RRH_EEP_PA_DRAIN_AB					0x0402			/*TDD_RRE_ZYNQ	*/
#define	D_RRH_EEP_TRA_TX_RF_GAIN_A				0x0404
#define	D_RRH_EEP_TRA_TX_MCAR_GAIN_A			0x0406
#define	D_RRH_EEP_TRA_TX_DEM_GAIN_Q_A			0x0408
#define	D_RRH_EEP_TRA_TX_DEM_GAIN_I_A			0x040A
#define	D_RRH_EEP_TRA_TX_FB_DLYCLK_A			0x040C
#define	D_RRH_EEP_TRA_TX_FB_DLYFIL_A			0x040E
#define	D_RRH_EEP_TRA_TX_TXVGA_A				0x0414
#define	D_RRH_EEP_TRA_TX_PAPA_A					0x041E
#define	D_RRH_EEP_TRA_TX_PACA_A					0x0420
#define	D_RRH_EEP_TRA_TX_EPS_2_A				0x0428
#define	D_RRH_EEP_TRA_TX_PHI_2_A				0x042A
#define	D_RRH_EEP_TRA_TX_OFFSETI_A				0x042C
#define	D_RRH_EEP_TRA_TX_OFFSETQ_A				0x042E

#define	D_RRH_EEP_TRA_TX_RF_GAIN_B				0x0444
#define	D_RRH_EEP_TRA_TX_MCAR_GAIN_B			0x0446
#define	D_RRH_EEP_TRA_TX_DEM_GAIN_Q_B			0x0448
#define	D_RRH_EEP_TRA_TX_DEM_GAIN_I_B			0x044A
#define	D_RRH_EEP_TRA_TX_FB_DLYCLK_B			0x044C
#define	D_RRH_EEP_TRA_TX_FB_DLYFIL_B			0x044E
#define	D_RRH_EEP_TRA_TX_TXVGA_B				0x0454
#define	D_RRH_EEP_TRA_TX_PAPA_B					0x045E
#define	D_RRH_EEP_TRA_TX_PACA_B					0x0460
#define	D_RRH_EEP_TRA_TX_EPS_2_B				0x0468
#define	D_RRH_EEP_TRA_TX_PHI_2_B				0x046A
#define	D_RRH_EEP_TRA_TX_OFFSETI_B				0x046C
#define	D_RRH_EEP_TRA_TX_OFFSETQ_B				0x046E

#define	D_RRH_EEP_TRA_TX_RF_GAIN_C				0x0484
#define	D_RRH_EEP_TRA_TX_MCAR_GAIN_C			0x0486
#define	D_RRH_EEP_TRA_TX_DEM_GAIN_Q_C			0x0488
#define	D_RRH_EEP_TRA_TX_DEM_GAIN_I_C			0x048A
#define	D_RRH_EEP_TRA_TX_FB_DLYCLK_C			0x048C
#define	D_RRH_EEP_TRA_TX_FB_DLYFIL_C			0x048E
#define	D_RRH_EEP_TRA_TX_TXVGA_C				0x0494
#define	D_RRH_EEP_TRA_TX_PAPA_C					0x049E
#define	D_RRH_EEP_TRA_TX_PACA_C					0x04A0
#define	D_RRH_EEP_TRA_TX_EPS_2_C				0x04A8
#define	D_RRH_EEP_TRA_TX_PHI_2_C				0x04AA
#define	D_RRH_EEP_TRA_TX_OFFSETI_C				0x04AC
#define	D_RRH_EEP_TRA_TX_OFFSETQ_C				0x04AE

#define	D_RRH_EEP_TRA_TX_RF_GAIN_D				0x04C4
#define	D_RRH_EEP_TRA_TX_MCAR_GAIN_D			0x04C6
#define	D_RRH_EEP_TRA_TX_DEM_GAIN_Q_D			0x04C8
#define	D_RRH_EEP_TRA_TX_DEM_GAIN_I_D			0x04CA
#define	D_RRH_EEP_TRA_TX_FB_DLYCLK_D			0x04CC
#define	D_RRH_EEP_TRA_TX_FB_DLYFIL_D			0x04CE
#define	D_RRH_EEP_TRA_TX_TXVGA_D				0x04D4
#define	D_RRH_EEP_TRA_TX_PAPA_D					0x04DE
#define	D_RRH_EEP_TRA_TX_PACA_D					0x04E0
#define	D_RRH_EEP_TRA_TX_EPS_2_D				0x04E8
#define	D_RRH_EEP_TRA_TX_PHI_2_D				0x04EA
#define	D_RRH_EEP_TRA_TX_OFFSETI_D				0x04EC
#define	D_RRH_EEP_TRA_TX_OFFSETQ_D				0x04EE

#define	D_RRH_EEP_TRA_TX_CRC16					0x05FE			/* CRC16値(Tx)					*/

/********************/
/* Rx関連設定情報	*/
/********************/

#define	D_RRH_EEP_TRA_RX_LEN					0x0600			/* RXのLength情報				*/

#define	D_RRH_EEP_TRA_RX_MIXER_ATTENUATION_A	0x0616
#define	D_RRH_EEP_TRA_RX_VGA1_SETTING_A			0x0618
#define	D_RRH_EEP_TRA_RX_VGA2_SETTING_A			0x061A
#define	D_RRH_EEP_TRA_RX_GAIN_INDEX_A			0x061C

#define	D_RRH_EEP_TRA_RX_MIXER_ATTENUATION_B	0x0656
#define	D_RRH_EEP_TRA_RX_VGA1_SETTING_B			0x0658
#define	D_RRH_EEP_TRA_RX_VGA2_SETTING_B			0x065A
#define	D_RRH_EEP_TRA_RX_GAIN_INDEX_B			0x065C

#define	D_RRH_EEP_TRA_RX_MIXER_ATTENUATION_C	0x0696
#define	D_RRH_EEP_TRA_RX_VGA1_SETTING_C			0x0698
#define	D_RRH_EEP_TRA_RX_VGA2_SETTING_C			0x069A
#define	D_RRH_EEP_TRA_RX_GAIN_INDEX_C			0x069C

#define	D_RRH_EEP_TRA_RX_MIXER_ATTENUATION_D	0x06D6
#define	D_RRH_EEP_TRA_RX_VGA1_SETTING_D			0x06D8
#define	D_RRH_EEP_TRA_RX_VGA2_SETTING_D			0x06DA
#define	D_RRH_EEP_TRA_RX_GAIN_INDEX_D			0x06DC

#define	D_RRH_EEP_TRA_RX_CRC16					0x07FE			/* CRC16値(Rx)					*/

#include "f_du_reg_eep.h"


/* @} */
#endif
/* @} */

