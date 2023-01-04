/*!
 * @skip  $ld:$
 * @file  f_du_reg_eep.h
 * @brief System共通定義
 * @date  2018/04/19	FJT)H.Yoshida	New Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_DU_REG_EEP_H
#define F_DU_REG_EEP_H

/*!
 * @name EEPROMアドレスオフセット(共通部)
 * @note EEPROMアクセス用のアドレスオフセット
 * @{
 */
#define	D_DU_I2C_EEP_TR_OFS					0x0400

#define	D_DU_EEP_CARD_NAME					0x0000				/* カード品名(ASCII)			*/
#define	D_DU_CARD_NAME_LEN					12

#define	D_DU_EEP_CARD_NO					0x0010				/* カード図番					*/
#define	D_DU_CARD_NO_LEN					12


#define	D_DU_EEP_MAKE_DATE1					0x0020				/* 製造番号(年/月/日)				*/
#define	D_DU_EEP_MAKE_DATE2					0x0021
#define	D_DU_EEP_MAKE_DATE3					0x0022
#define	D_DU_EEP_MAKE_DATE4					0x0023

#define	D_DU_EEP_MAKE_SERIAL_H1				0x0024				/* 製造番号(シリアル上位)		*/
#define	D_DU_EEP_MAKE_SERIAL_H2				0x0025
#define	D_DU_EEP_MAKE_SERIAL_L1				0x0026				/* 製造番号(シリアル下位)		*/
#define	D_DU_EEP_MAKE_SERIAL_L2				0x0027

#define	D_DU_EEP_TEST_YEAR1					0x0028				/* 試験日(年)					*/
#define	D_DU_EEP_TEST_YEAR2					0x0029
#define	D_DU_EEP_TEST_MONDAY1				0x002A				/* 試験日(月日)					*/
#define	D_DU_EEP_TEST_MONDAY2				0x002B

#define	D_DU_EEP_HARD_VER1					0x002C				/* ハードバージョン(総合版数)	*/
#define	D_DU_EEP_HARD_VER2					0x002D
#define	D_DU_EEP_HARD_VER3					0x002E
#define	D_DU_EEP_HARD_VER4					0x002F
#define	D_DU_EEP_HARD_VER5					0x0030
#define	D_DU_EEP_HARD_VER6					0x0031

													/* FAN/新Beam対応後のM-plane報告/RU-MT表示	*/
#define	D_DU_EEP_HARD_VER_MASK				0x0FFF				/* 上位4bitマスク値				*/
#define	D_DU_EEP_FANNML_BFOLD				0x0000				/* 0：通常FAN,旧Beam（sub6/mmw）*/
#define	D_DU_EEP_FANLOW_BFOLD				0x1000				/* 1：低速FAN,旧Beam（sub6/mmw）*/
#define	D_DU_EEP_FANNML_BFNEW				0x8000				/* 8：通常FAN,新Beam（mmw）		*/
#define	D_DU_EEP_FANLOW_BFNEW				0x9000				/* 9：低速FAN,旧Beam（mmw）		*/

/* 装置種別<RE種別>定義	*/
#define	D_DU_EEP_DEVICE_KIND0				0x0040				/* 装置種別						*/
#define	D_DU_EEP_DEV_KIND_5GLLS_PROTO		0x10				/* 5G-LLS(Proto					*/
#define	D_DU_EEP_DEV_KIND_5GDU_SUB6			0x13				/* 5G-DU治具(sub6)				*/
#define	D_DU_EEP_DEV_KIND_5GDU_MMW			0x14				/* 5G-DU治具(mmW)				*/

/* <対応周波数帯> 01:15G/02:17G/03:700M/04:2G/05:800M/06:3.5G */
#define	D_DU_EEP_DEVICE_KIND1				0x0041				/* 装置種別<対応周波数帯>		*/
#define	D_DU_EEP_DEV_FREQ_45G				0x10				/* 4.5G							*/
#define	D_DU_EEP_DEV_FREQ_37G				0x11				/* 3.7G							*/
#define	D_DU_EEP_DEV_FREQ_28G				0x12				/* 28G							*/

#define	D_DU_EEP_BOARD_VER					0x0042				/* ボードバージョン				*/
#define	D_DU_EEP_BOARD_VER_PT01				0x01				/* PT01版	*/
#define	D_DU_EEP_BOARD_VER_PT02				0x02				/* PT02版	*/
#define	D_DU_EEP_BOARD_VER_PT03				0x03				/* PT03版	*/

#define	D_DU_EEP_START_FLG					0x0050				/* FPGA/Firm共通 起動面情報1	*/


#define	D_DU_EEP_STUP_BUSY_COUNTER			0x0060				/* StartupBusyCounter	*/


/* ファイルセット1 Flash 0面	*/
#define	D_DU_EEP_FSET1_FLASH0_VER			0x0070				/* 代表バージョン				*/
#define	D_DU_EEP_FSET1_FLASH0_F01_VER		0x0076				/* 個別ファイル01 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F02_VER		0x0078				/* 個別ファイル02 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F03_VER		0x007A				/* 個別ファイル03 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F04_VER		0x007C				/* 個別ファイル04 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F05_VER		0x007E				/* 個別ファイル05 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F06_VER		0x0080				/* 個別ファイル06 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F07_VER		0x0082				/* 個別ファイル07 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F08_VER		0x0084				/* 個別ファイル08 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F09_VER		0x0086				/* 個別ファイル09 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F10_VER		0x0088				/* 個別ファイル10 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F11_VER		0x008A				/* 個別ファイル11 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F12_VER		0x008C				/* 個別ファイル12 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F13_VER		0x008E				/* 個別ファイル13 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F14_VER		0x0090				/* 個別ファイル14 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F15_VER		0x0092				/* 個別ファイル15 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH0_F16_VER		0x0094				/* 個別ファイル16 バージョン	*/

#define	D_DU_EEP_FSET1_FLASH0_F01_EFLG		0x0096				/* 個別ファイル01 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F02_EFLG		0x0097				/* 個別ファイル02 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F03_EFLG		0x0098				/* 個別ファイル03 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F04_EFLG		0x0099				/* 個別ファイル04 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F05_EFLG		0x009A				/* 個別ファイル05 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F06_EFLG		0x009B				/* 個別ファイル06 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F07_EFLG		0x009C				/* 個別ファイル07 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F08_EFLG		0x009D				/* 個別ファイル08 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F09_EFLG		0x009E				/* 個別ファイル09 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F10_EFLG		0x009F				/* 個別ファイル10 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F11_EFLG		0x00A0				/* 個別ファイル11 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F12_EFLG		0x00A1				/* 個別ファイル12 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F13_EFLG		0x00A2				/* 個別ファイル13 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F14_EFLG		0x00A3				/* 個別ファイル14 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F15_EFLG		0x00A4				/* 個別ファイル15 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH0_F16_EFLG		0x00A5				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット1 Flash 1面	*/
#define	D_DU_EEP_FSET1_FLASH1_VER			0x00B0				/* 代表バージョン				*/
#define	D_DU_EEP_FSET1_FLASH1_F01_VER		0x00B6				/* 個別ファイル01 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F02_VER		0x00B8				/* 個別ファイル02 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F03_VER		0x00BA				/* 個別ファイル03 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F04_VER		0x00BC				/* 個別ファイル04 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F05_VER		0x00BE				/* 個別ファイル05 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F06_VER		0x00C0				/* 個別ファイル06 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F07_VER		0x00C2				/* 個別ファイル07 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F08_VER		0x00C4				/* 個別ファイル08 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F09_VER		0x00C6				/* 個別ファイル09 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F10_VER		0x00C8				/* 個別ファイル10 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F11_VER		0x00CA				/* 個別ファイル11 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F12_VER		0x00CC				/* 個別ファイル12 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F13_VER		0x00CE				/* 個別ファイル13 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F14_VER		0x00D0				/* 個別ファイル14 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F15_VER		0x00D2				/* 個別ファイル15 バージョン	*/
#define	D_DU_EEP_FSET1_FLASH1_F16_VER		0x00D4				/* 個別ファイル16 バージョン	*/

#define	D_DU_EEP_FSET1_FLASH1_F01_EFLG		0x00D6				/* 個別ファイル01 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F02_EFLG		0x00D7				/* 個別ファイル02 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F03_EFLG		0x00D8				/* 個別ファイル03 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F04_EFLG		0x00D9				/* 個別ファイル04 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F05_EFLG		0x00DA				/* 個別ファイル05 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F06_EFLG		0x00DB				/* 個別ファイル06 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F07_EFLG		0x00DC				/* 個別ファイル07 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F08_EFLG		0x00DD				/* 個別ファイル08 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F09_EFLG		0x00DE				/* 個別ファイル09 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F10_EFLG		0x00DF				/* 個別ファイル10 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F11_EFLG		0x00E0				/* 個別ファイル11 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F12_EFLG		0x00E1				/* 個別ファイル12 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F13_EFLG		0x00E2				/* 個別ファイル13 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F14_EFLG		0x00E3				/* 個別ファイル14 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F15_EFLG		0x00E4				/* 個別ファイル15 イレースフラグ*/
#define	D_DU_EEP_FSET1_FLASH1_F16_EFLG		0x00E5				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット1 Flash Factory面	*/
#define	D_DU_EEP_FSET1_FACTRY_VER			0x00F0				/* 代表バージョン				*/
#define	D_DU_EEP_FSET1_FACTRY_F01_VER		0x00F6				/* 個別ファイル01 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F02_VER		0x00F8				/* 個別ファイル02 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F03_VER		0x00FA				/* 個別ファイル03 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F04_VER		0x00FC				/* 個別ファイル04 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F05_VER		0x00FE				/* 個別ファイル05 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F06_VER		0x0100				/* 個別ファイル06 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F07_VER		0x0102				/* 個別ファイル07 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F08_VER		0x0104				/* 個別ファイル08 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F09_VER		0x0106				/* 個別ファイル09 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F10_VER		0x0108				/* 個別ファイル10 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F11_VER		0x010A				/* 個別ファイル11 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F12_VER		0x010C				/* 個別ファイル12 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F13_VER		0x010E				/* 個別ファイル13 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F14_VER		0x0110				/* 個別ファイル14 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F15_VER		0x0112				/* 個別ファイル15 バージョン	*/
#define	D_DU_EEP_FSET1_FACTRY_F16_VER		0x0114				/* 個別ファイル16 バージョン	*/

#define	D_DU_EEP_FSET1_FACTRY_F01_EFLG		0x0116				/* 個別ファイル01 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F02_EFLG		0x0117				/* 個別ファイル02 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F03_EFLG		0x0118				/* 個別ファイル03 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F04_EFLG		0x0119				/* 個別ファイル04 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F05_EFLG		0x011A				/* 個別ファイル05 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F06_EFLG		0x011B				/* 個別ファイル06 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F07_EFLG		0x011C				/* 個別ファイル07 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F08_EFLG		0x011D				/* 個別ファイル08 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F09_EFLG		0x011E				/* 個別ファイル09 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F10_EFLG		0x011F				/* 個別ファイル10 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F11_EFLG		0x0120				/* 個別ファイル11 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F12_EFLG		0x0121				/* 個別ファイル12 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F13_EFLG		0x0122				/* 個別ファイル13 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F14_EFLG		0x0123				/* 個別ファイル14 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F15_EFLG		0x0124				/* 個別ファイル15 イレースフラグ*/
#define	D_DU_EEP_FSET1_FACTRY_F16_EFLG		0x0125				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット2 Flash 0面	*/
#define	D_DU_EEP_FSET2_FLASH0_VER			0x0130				/* 代表バージョン				*/
#define	D_DU_EEP_FSET2_FLASH0_F01_VER		0x0136				/* 個別ファイル01 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F02_VER		0x0138				/* 個別ファイル02 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F03_VER		0x013A				/* 個別ファイル03 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F04_VER		0x013C				/* 個別ファイル04 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F05_VER		0x013E				/* 個別ファイル05 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F06_VER		0x0140				/* 個別ファイル06 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F07_VER		0x0142				/* 個別ファイル07 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F08_VER		0x0144				/* 個別ファイル08 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F09_VER		0x0146				/* 個別ファイル09 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F10_VER		0x0148				/* 個別ファイル10 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F11_VER		0x014A				/* 個別ファイル11 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F12_VER		0x014C				/* 個別ファイル12 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F13_VER		0x014E				/* 個別ファイル13 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F14_VER		0x0150				/* 個別ファイル14 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F15_VER		0x0152				/* 個別ファイル15 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH0_F16_VER		0x0154				/* 個別ファイル16 バージョン	*/

#define	D_DU_EEP_FSET2_FLASH0_F01_EFLG		0x0156				/* 個別ファイル01 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F02_EFLG		0x0157				/* 個別ファイル02 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F03_EFLG		0x0158				/* 個別ファイル03 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F04_EFLG		0x0159				/* 個別ファイル04 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F05_EFLG		0x015A				/* 個別ファイル05 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F06_EFLG		0x015B				/* 個別ファイル06 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F07_EFLG		0x015C				/* 個別ファイル07 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F08_EFLG		0x015D				/* 個別ファイル08 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F09_EFLG		0x015E				/* 個別ファイル09 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F10_EFLG		0x015F				/* 個別ファイル10 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F11_EFLG		0x0160				/* 個別ファイル11 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F12_EFLG		0x0161				/* 個別ファイル12 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F13_EFLG		0x0162				/* 個別ファイル13 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F14_EFLG		0x0163				/* 個別ファイル14 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F15_EFLG		0x0164				/* 個別ファイル15 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH0_F16_EFLG		0x0165				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット2 Flash 1面	*/
#define	D_DU_EEP_FSET2_FLASH1_VER			0x0170				/* 代表バージョン				*/
#define	D_DU_EEP_FSET2_FLASH1_F01_VER		0x0176				/* 個別ファイル01 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F02_VER		0x0178				/* 個別ファイル02 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F03_VER		0x017A				/* 個別ファイル03 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F04_VER		0x017C				/* 個別ファイル04 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F05_VER		0x017E				/* 個別ファイル05 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F06_VER		0x0180				/* 個別ファイル06 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F07_VER		0x0182				/* 個別ファイル07 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F08_VER		0x0184				/* 個別ファイル08 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F09_VER		0x0186				/* 個別ファイル09 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F10_VER		0x0188				/* 個別ファイル10 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F11_VER		0x018A				/* 個別ファイル11 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F12_VER		0x018C				/* 個別ファイル12 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F13_VER		0x018E				/* 個別ファイル13 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F14_VER		0x0190				/* 個別ファイル14 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F15_VER		0x0192				/* 個別ファイル15 バージョン	*/
#define	D_DU_EEP_FSET2_FLASH1_F16_VER		0x0194				/* 個別ファイル16 バージョン	*/

#define	D_DU_EEP_FSET2_FLASH1_F01_EFLG		0x0196				/* 個別ファイル01 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F02_EFLG		0x0197				/* 個別ファイル02 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F03_EFLG		0x0198				/* 個別ファイル03 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F04_EFLG		0x0199				/* 個別ファイル04 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F05_EFLG		0x019A				/* 個別ファイル05 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F06_EFLG		0x019B				/* 個別ファイル06 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F07_EFLG		0x019C				/* 個別ファイル07 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F08_EFLG		0x019D				/* 個別ファイル08 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F09_EFLG		0x019E				/* 個別ファイル09 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F10_EFLG		0x019F				/* 個別ファイル10 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F11_EFLG		0x01A0				/* 個別ファイル11 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F12_EFLG		0x01A1				/* 個別ファイル12 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F13_EFLG		0x01A2				/* 個別ファイル13 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F14_EFLG		0x01A3				/* 個別ファイル14 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F15_EFLG		0x01A4				/* 個別ファイル15 イレースフラグ*/
#define	D_DU_EEP_FSET2_FLASH1_F16_EFLG		0x01A5				/* 個別ファイル16 イレースフラグ*/

/* ファイルセット2 Flash Factory面	*/
#define	D_DU_EEP_FSET2_FACTRY_VER			0x01B0				/* 代表バージョン				*/
#define	D_DU_EEP_FSET2_FACTRY_F01_VER		0x01B6				/* 個別ファイル01 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F02_VER		0x01B8				/* 個別ファイル02 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F03_VER		0x01BA				/* 個別ファイル03 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F04_VER		0x01BC				/* 個別ファイル04 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F05_VER		0x01BE				/* 個別ファイル05 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F06_VER		0x01C0				/* 個別ファイル06 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F07_VER		0x01C2				/* 個別ファイル07 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F08_VER		0x01C4				/* 個別ファイル08 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F09_VER		0x01C6				/* 個別ファイル09 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F10_VER		0x01C8				/* 個別ファイル10 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F11_VER		0x01CA				/* 個別ファイル11 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F12_VER		0x01CC				/* 個別ファイル12 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F13_VER		0x01CE				/* 個別ファイル13 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F14_VER		0x01D0				/* 個別ファイル14 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F15_VER		0x01D2				/* 個別ファイル15 バージョン	*/
#define	D_DU_EEP_FSET2_FACTRY_F16_VER		0x01D4				/* 個別ファイル16 バージョン	*/

#define	D_DU_EEP_FSET2_FACTRY_F01_EFLG		0x01D6				/* 個別ファイル01 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F02_EFLG		0x01D7				/* 個別ファイル02 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F03_EFLG		0x01D8				/* 個別ファイル03 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F04_EFLG		0x01D9				/* 個別ファイル04 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F05_EFLG		0x01DA				/* 個別ファイル05 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F06_EFLG		0x01DB				/* 個別ファイル06 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F07_EFLG		0x01DC				/* 個別ファイル07 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F08_EFLG		0x01DD				/* 個別ファイル08 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F09_EFLG		0x01DE				/* 個別ファイル09 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F10_EFLG		0x01DF				/* 個別ファイル10 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F11_EFLG		0x01E0				/* 個別ファイル11 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F12_EFLG		0x01E1				/* 個別ファイル12 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F13_EFLG		0x01E2				/* 個別ファイル13 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F14_EFLG		0x01E3				/* 個別ファイル14 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F15_EFLG		0x01E4				/* 個別ファイル15 イレースフラグ*/
#define	D_DU_EEP_FSET2_FACTRY_F16_EFLG		0x01E5				/* 個別ファイル16 イレースフラグ*/

#define	D_DU_EEP_SWSLOT_STATUS				0x017C				/* ステータス状態				*/
#define	D_DU_EEP_STATUS_EMPTY				0					/* Slot状態 = EMPTY				*/
#define	D_DU_EEP_STATUS_VALID				1					/* Slot状態 = VALID				*/
#define	D_DU_EEP_STATUS_INVALID				2					/* Slot状態 = INVALID			*/

#define	D_DU_EEP_BUILD0_NAME				0x0180				/* ビルド名称(0面)				*/
#define	D_DU_EEP_BUILD1_NAME				0x01C0				/* ビルド名称(1面)				*/

/***** Ether(MT-PORT)情報	*****/
#define	D_DU_EEP_MAC_ADDR_DBG_H1			0x0200				/* MACアドレス(ベンダ番号上位)	*/
#define	D_DU_EEP_MAC_ADDR_DBG_H2			0x0201
#define	D_DU_EEP_MAC_ADDR_DBG_M1			0x0202				/* MACアドレス(ベンダ番号下位)	*/
#define	D_DU_EEP_MAC_ADDR_DBG_M2			0x0203
#define	D_DU_EEP_MAC_ADDR_DBG_L1			0x0204				/* MACアドレス(ガード通番)		*/
#define	D_DU_EEP_MAC_ADDR_DBG_L2			0x0205

#define	D_DU_EEP_IP_ADDR_DBG_H1				0x0206				/* IPアドレス(上位)				*/
#define	D_DU_EEP_IP_ADDR_DBG_H2				0x0207
#define	D_DU_EEP_IP_ADDR_DBG_L1				0x0208				/* IPアドレス(下位)				*/
#define	D_DU_EEP_IP_ADDR_DBG_L2				0x0209

/***** 25G eCPRI_Port#1情報	*****/
#define	D_DU_EEP_MAC_ADDR_PORT1_H1			0x0220				/* MACアドレス(ベンダ番号上位)	*/
#define	D_DU_EEP_MAC_ADDR_PORT1_H2			0x0221
#define	D_DU_EEP_MAC_ADDR_PORT1_M1			0x0222				/* MACアドレス(ベンダ番号下位)	*/
#define	D_DU_EEP_MAC_ADDR_PORT1_M2			0x0223
#define	D_DU_EEP_MAC_ADDR_PORT1_L1			0x0224				/* MACアドレス(ガード通番)		*/
#define	D_DU_EEP_MAC_ADDR_PORT1_L2			0x0225

#define	D_DU_EEP_IP_ADDR_PORT1_H1			0x0226				/* IPアドレス(上位)				*/
#define	D_DU_EEP_IP_ADDR_PORT1_H2			0x0227
#define	D_DU_EEP_IP_ADDR_PORT1_L1			0x0228				/* IPアドレス(下位)				*/
#define	D_DU_EEP_IP_ADDR_PORT1_L2			0x0229

/***** 25G eCPRI_Port#2情報	*****/
#define	D_DU_EEP_MAC_ADDR_PORT2_H1			0x0260				/* MACアドレス(ベンダ番号上位)	*/
#define	D_DU_EEP_MAC_ADDR_PORT2_H2			0x0261
#define	D_DU_EEP_MAC_ADDR_PORT2_M1			0x0262				/* MACアドレス(ベンダ番号下位)	*/
#define	D_DU_EEP_MAC_ADDR_PORT2_M2			0x0263
#define	D_DU_EEP_MAC_ADDR_PORT2_L1			0x0264				/* MACアドレス(ガード通番)		*/
#define	D_DU_EEP_MAC_ADDR_PORT2_L2			0x0265

#define	D_DU_EEP_IP_ADDR_PORT2_H1			0x0266				/* IPアドレス(上位)				*/
#define	D_DU_EEP_IP_ADDR_PORT2_H2			0x0267
#define	D_DU_EEP_IP_ADDR_PORT2_L1			0x0268				/* IPアドレス(下位)				*/
#define	D_DU_EEP_IP_ADDR_PORT2_L2			0x0269

/***** システムパラメータ	*****/
#define	D_DU_EEP_VID_LOW_PORT1_H			0x0320				/* Low VID						*/
#define	D_DU_EEP_VID_LOW_PORT1_L			0x0321
#define	D_DU_EEP_VID_HIGH_PORT1_H			0x0322				/* High VID						*/
#define	D_DU_EEP_VID_HIGH_PORT1_L			0x0323
#define	D_DU_EEP_VID_LAST_PORT1_H			0x0324				/* Last Used VID				*/
#define	D_DU_EEP_VID_LAST_PORT1_L			0x0325
#define	D_DU_EEP_VID_PTP_CONF_PTRN			0x0326				/* PTP Config Pattern			*/

#define	D_DU_EEP_VID_MPLANE_PCP				0x0327				/* M-Plane pcp					*/
#define	D_DU_EEP_VID_SCAN_MODE				0x0328				/*  VLAN scan実行モード			*/

#define	D_DU_EEP_SSH_USEABLE_FLAG			0x0329				/* ssh-port有効フラグ			*/
#define	D_DU_EEP_CALLHOME_SSH_PORT_H		0x032A				/* call-home-ssh-port			*/
#define	D_DU_EEP_CALLHOME_SSH_PORT_L		0x032B
#define	D_DU_EEP_SERVER_SSH_PORT_H			0x032C				/* server-ssh-port				*/
#define	D_DU_EEP_SERVER_SSH_PORT_L			0x032D
#define	D_DU_EEP_CALLHOME_SSH_TIMER_H		0x032E				/* callhome retry timer			*/
#define	D_DU_EEP_CALLHOME_SSH_TIMER_L		0x032F

#define	D_DU_EEP_VID_DOMAIN_NO				0x0370				/* domain-number				*/
#define	D_DU_EEP_VID_MULTI_MACADDR			0x0371				/* multicast-mac-address		*/
#define	D_DU_EEP_VID_ACPT_CLK_CLASS			0x0372				/* accepted-clock-classes		*/

#define	D_DU_EEP_TEMP_ALM_THRESHOLD			0x0380				/* 温度ALM閾値					*/
#define	D_DU_EEP_PAONOFF					0x0381				/* PAON/OFF						*/
#define	D_DU_EEP_TXGAIN						0x0382				/* TXGAIN						*/
#define	D_DU_EEP_RXCALCMP					0x0383				/* RxCalトレーニング完了フラグ	*/

#define	D_DU_EEP_BF_ENVELOPE_MODE			0x0385				/* mmW BeamID 新旧モード設定	*/
#define	D_DU_EEP_FAN_SPIN_MODE				0x0387				/* FAN回転数モード設定			*/


#define	D_DU_EEP_BORDINFO_MMW				0x0391				/* mmWトレーニング完了 RFボードシリアルNo */

#define	D_DU_EEP_T3ADJ_H					0x03a6				/* T3補正値X */
#define	D_DU_EEP_T3ADJ_L					0x03a7				/* T3補正値X */
#define	D_DU_EEP_T2ADJ_H					0x03a8				/* T2補正値Y */
#define	D_DU_EEP_T2ADJ_L					0x03a9				/* T2補正値Y */

#define D_DU_EEP_SFP_FAILSAFE_AB_CNT		0x03aa				/* フェールセーフABカウンタ */
#define D_DU_EEP_SFP_FAILSAFE_ABCD_CNT		0x03ab				/* フェールセーフABCDカウンタ */

#define	D_DU_EEP_DHCP_SERIALNO				0x03b0				/* DHCP シリアル番号 */

/***** デバッグ情報			*****/
#define	D_DU_EEP_DHCP_ENT_FLAG				0x03c1				/* DHCP有無フラグ				*/
#define	D_DU_EEP_PTP_STOP_FLAG				0x03c2				/* PTPdeviceアクセス抑止フラグ	*/
#define	D_DU_EEP_PTP_START_MODE				0x03c3				/* PTP masterモード				*/
#define	D_DU_EEP_ESMC_SND_FLAG				0x03cb				/* SyncE ESMC送信フラグ			*/

#define	D_DU_EEP_FLASH_WRITE_FLAG_SSH		0x03ce				/* SSHユーザーアカウント情報FLASH書込みフラグ	*/
#define	D_DU_EEP_FLASH_WRITE_FLAG_VLAN		0x03cf				/* VLAN情報FLASH書込みフラグ	*/

#define	D_DU_EEP_SOFT_RESET_MSK				0x03d0				/* ソフトリセットマスクフラグ	*/
#define	D_DU_EEP_SOFT_RESET_FLG				0x03d1				/* ソフトリセットフラグ			*/
#define	D_DU_EEP_ALM_SELF_RESET_FLG			0x03d2				/* ALM自律リセット有無フラグ	*/
#define	D_DU_EEP_TROUBLE_LOG				0x03d3				/* 障害ログ退避面				*/
#define	D_DU_EEP_CYCLIC_LOG					0x03d4				/* サイクリックログ退避面		*/
#define	D_DU_EEP_CYCLIC_CLEAR_FLG0			0x03d5				/* サイクリックログイレースフラグ0面用	*/
#define	D_DU_EEP_CYCLIC_CLEAR_FLG1			0x03d6				/* サイクリックログイレースフラグ1面用	*/
#define	D_DU_EEP_TIMESAVE_FLG				0x03d7				/* 時刻退避フラグ				*/
#define	D_DU_EEP_TIMESAVE_YEAR1				0x03d8				/* 時刻退避領域					*/
#define	D_DU_EEP_TIMESAVE_YEAR2				0x03d9				/* 時刻退避領域					*/
#define	D_DU_EEP_TIMESAVE_MONTH				0x03da				/* 時刻退避領域					*/
#define	D_DU_EEP_TIMESAVE_DAY				0x03db				/* 時刻退避領域					*/
#define	D_DU_EEP_TIMESAVE_TIME				0x03dc				/* 時刻退避領域					*/
#define	D_DU_EEP_TIMESAVE_MIN				0x03dd				/* 時刻退避領域					*/
#define	D_DU_EEP_TIMESAVE_SEC				0x03de				/* 時刻退避領域					*/
#define	D_DU_EEP_TIMESAVE_10MSEC			0x03df				/* 時刻退避領域					*/

#define	D_DU_EEP_PC_CONNECT_FLAG			0x03e0				/* PC接続フラグ					*/
#define	D_DU_EEP_MP_CONNECT_FLAG			0x03e1				/* MPlane接続フラグ				*/

#define	D_DU_EEP_IP_ADDR_ACCESS_H1			0x03e2				/* 接続先IPアドレス(上位)		*/
#define	D_DU_EEP_IP_ADDR_ACCESS_H2			0x03e3
#define	D_DU_EEP_IP_ADDR_ACCESS_L1			0x03e4				/* 接続先IPアドレス(下位)		*/
#define	D_DU_EEP_IP_ADDR_ACCESS_L2			0x03e5

#define	D_DU_EEP_PGCLOG						0x03e6				/* PGC LOG フラグ				*/
#define	D_DU_EEP_BEAMFORMING_MODE			0x03e7				/* ビームフォーミングモード		*/

#define	D_DU_EEP_DPMODE_MP					0x03e8				/* ダンプモード(M-plane用)		*/
#define	D_DU_EEP_DPMODE_SP					0x03e9				/* ダンプモード(S-plane用)		*/
#define	D_DU_EEP_DPMODE_DBG					0x03ea				/* ダンプモード(デバッグ用)		*/
#define	D_DU_EEP_DPMODE_AISG				0x03eb				/* ダンプモード(AISG用)			*/


/***** SFN/FRM補正計算関連	*****/
#define	D_DU_EEP_OFFSET_A_H					0x03f1				/* offset_a						*/
#define	D_DU_EEP_OFFSET_A_M					0x03f2
#define	D_DU_EEP_OFFSET_A_L					0x03f3
#define	D_DU_EEP_OFFSET_B_H					0x03f4				/* offset_b						*/
#define	D_DU_EEP_OFFSET_B_L					0x03f5


#define	D_DU_EEP_CFREQ_FLG					0x03f6
#define	D_DU_EEP_CFREQ_OFFSET_1				0x03f7
#define	D_DU_EEP_CFREQ_OFFSET_2				0x03f8
#define	D_DU_EEP_CFREQ_OFFSET_3				0x03f9
#define	D_DU_EEP_CFREQ_OFFSET_4				0x03fA


 /* 5GDU_chg */
/*!
 * @name EEPROMアドレスオフセット(トレーニング部) SUB6用
 * @note EEPROMアクセス用のアドレスオフセット
 * @{
 */
#define	D_DU_EEP_TRA_ALLDATAT				0x800				/* EEPROM全データ数(共通部、トレーニング部)	*/
#define	D_DU_EEP_TRA_TXCNT					0x300				/* トレーニングデータ数(Tx)		*/
#define	D_DU_EEP_TRA_RXCNT					0x100				/* トレーニングデータ数(Rx)		*/

#define	D_DU_EEP_TRA_TX_ANT_OFFSET			0x80				/* アンテナ間オフセット			*/
#define	D_DU_EEP_TRA_RX_ANT_OFFSET			0x40				/* アンテナ間オフセット			*/
#define	D_DU_EEP_TRA_TX_ANT_OFFSET_RRE		0x80				/* アンテナ間オフセット(RRE用)	*/

/********************/
/* Tx関連設定情報	*/
/********************/
#define	D_DU_EEP_TRA_TX_LEN					0x0400				/* TXのLength情報				*/
#define	D_DU_EEP_TRA_TX_RF_GAIN_A			0x0400
#define	D_DU_EEP_TRA_TX_MCAR_GAIN_A			0x0402
#define	D_DU_EEP_TRA_TX_DEM_GAIN_Q_A		0x0404
#define	D_DU_EEP_TRA_TX_DEM_GAIN_I_A		0x0406
#define	D_DU_EEP_TRA_TX_FB_DLYCLK_A			0x0408
#define	D_DU_EEP_TRA_TX_FB_DLYFIL_A			0x040A
#define	D_DU_EEP_TRA_TX_FWVATT_DAC_A		0x040E
#define	D_DU_EEP_TRA_TX_PAFA_A				0x0412
#define	D_DU_EEP_TRA_TX_VSWR_RTNGAIN_A_Q	0x0414
#define	D_DU_EEP_TRA_TX_VSWR_RTNGAIN_A_I	0x0416
#define	D_DU_EEP_TRA_TX_FBEQ_A				0x0440


#define	D_DU_EEP_TRA_TX_RF_GAIN_B			0x0480
#define	D_DU_EEP_TRA_TX_MCAR_GAIN_B			0x0482
#define	D_DU_EEP_TRA_TX_DEM_GAIN_Q_B		0x0484
#define	D_DU_EEP_TRA_TX_DEM_GAIN_I_B		0x0486
#define	D_DU_EEP_TRA_TX_FB_DLYCLK_B			0x0488
#define	D_DU_EEP_TRA_TX_FB_DLYFIL_B			0x048A
#define	D_DU_EEP_TRA_TX_FWVATT_DAC_B		0x048E
#define	D_DU_EEP_TRA_TX_PAFA_B				0x0492
#define	D_DU_EEP_TRA_TX_VSWR_RTNGAIN_B_Q	0x0494
#define	D_DU_EEP_TRA_TX_VSWR_RTNGAIN_B_I	0x0496
#define	D_DU_EEP_TRA_TX_FBEQ_B				0x04C0

#define	D_DU_EEP_TRA_TX_RF_GAIN_C			0x0500
#define	D_DU_EEP_TRA_TX_MCAR_GAIN_C			0x0502
#define	D_DU_EEP_TRA_TX_DEM_GAIN_Q_C		0x0504
#define	D_DU_EEP_TRA_TX_DEM_GAIN_I_C		0x0506
#define	D_DU_EEP_TRA_TX_FB_DLYCLK_C			0x0508
#define	D_DU_EEP_TRA_TX_FB_DLYFIL_C			0x050A
#define	D_DU_EEP_TRA_TX_FWVATT_DAC_C		0x050E
#define	D_DU_EEP_TRA_TX_PAFA_C				0x0512
#define	D_DU_EEP_TRA_TX_VSWR_RTNGAIN_C_Q	0x0514
#define	D_DU_EEP_TRA_TX_VSWR_RTNGAIN_C_I	0x0516
#define D_DU_EEP_TRA_TX_CAL_PHASE_OFFSET_ANT_A 0x0418
#define D_DU_EEP_TRA_RX_CAL_PHASE_OFFSET_ANT_A 0x0722


#define	D_DU_EEP_TRA_TX_FBEQ_C				0x0540

#define	D_DU_EEP_TRA_TX_RF_GAIN_D			0x0580
#define	D_DU_EEP_TRA_TX_MCAR_GAIN_D			0x0582
#define	D_DU_EEP_TRA_TX_DEM_GAIN_Q_D		0x0584
#define	D_DU_EEP_TRA_TX_DEM_GAIN_I_D		0x0586
#define	D_DU_EEP_TRA_TX_FB_DLYCLK_D			0x0588
#define	D_DU_EEP_TRA_TX_FB_DLYFIL_D			0x058A
#define	D_DU_EEP_TRA_TX_FWVATT_DAC_D		0x058E
#define	D_DU_EEP_TRA_TX_PAFA_D				0x0592
#define	D_DU_EEP_TRA_TX_VSWR_RTNGAIN_D_Q	0x0594
#define	D_DU_EEP_TRA_TX_VSWR_RTNGAIN_D_I	0x0596
#define	D_DU_EEP_TRA_TX_FBEQ_D				0x05C0

#define	D_DU_EEP_TRA_TX_CRC16				0x06FE				/* CRC16値(Tx)					*/
 /* 5GDU_chg */
/********************/
/* Rx関連設定情報	*/
/********************/

#define	D_DU_EEP_TRA_RX_LEN					0x0700				/* RXのLength情報				*/
#define	D_DU_EEP_TRA_RX_CRC16				0x07FE				/* CRC16値(Rx)					*/


 /* 5GDU_chg */
/* MMW用EEPROM　定義 */


#define	D_DU_EEP_MMW_TRA_ALLDATAT			0x800				/* EEPROM全データ数(共通部、トレーニング部)	*/
#define	D_DU_EEP_MMW_TRA_TXCNT_NML			0x100				/* トレーニングデータ数(Tx) NMLモード		*/
#define	D_DU_EEP_MMW_TRA_RXCNT_NML			0x100				/* トレーニングデータ数(Rx) NMLモード		*/


/****************************/
/* Tx関連設定情報 NMLモード	*/
/****************************/
#define	D_DU_EEP_MMW_TRA_TX_LEN_NML			0x0400				/* TXのLength情報 NMLモード		*/
#define	D_DU_EEP_MMW_TRA_TX_OFS_NML			0x0000				/* TXのLength情報 NMLモード		*/
#define	D_DU_EEP_MMW_TRA_TX_CRC16_NML		0x00FE				/* CRC16値(Tx) NMLモード		*/


/****************************/
/* Rx関連設定情報 NMLモード	*/
/****************************/

#define	D_DU_EEP_MMW_TRA_RX_LEN_NML			0x0600				/* RXのLength情報 NMLモード		*/
#define	D_DU_EEP_MMW_TRA_RX_OFS_NML			0x0200				/* RXのLength情報 NMLモード		*/
#define	D_DU_EEP_MMW_TRA_RX_CRC16_NML		0x02FE				/* CRC16値(Rx) NMLモード		*/

#define	D_DU_EEP_SOFT_FILE_SIZE_0			0x130				/* ソフトファイルサイズ(0面)	*/
#define	D_DU_EEP_SOFT_FILE_SIZE_1			0x134				/* ソフトファイルサイズ(1面)	*/
#define	D_DU_EEP_SOFT_FILE_SIZE_F			0x138				/* ソフトファイルサイズ(Factory面)	*/

#define	D_DU_EEP_PLFPGA_FILE_SIZE_0			0x13C				/* PL-FPGAファイルサイズ(0面)	*/
#define	D_DU_EEP_PLFPGA_FILE_SIZE_1			0x140				/* PL-FPGAファイルサイズ(1面)	*/
#define	D_DU_EEP_PLFPGA_FILE_SIZE_F			0x144				/* PL-FPGAファイルサイズ(Factory面)	*/

#define	D_DU_EEP_TRXFPGA_FILE_SIZE_0		0x148				/* TRX-FPGAファイルサイズ(0面)	*/
#define	D_DU_EEP_TRXFPGA_FILE_SIZE_1		0x14C				/* TRX-FPGAファイルサイズ(1面)	*/
#define	D_DU_EEP_TRXFPGA_FILE_SIZE_F		0x150				/* TRX-FPGAファイルサイズ(Factory面)	*/

#define	D_DU_EEP_RFICBOT_FILE_SIZE_0		0x154				/* RFIC-Bootファイルサイズ(0面)	*/
#define	D_DU_EEP_RFICBOT_FILE_SIZE_1		0x158				/* RFIC-Bootファイルサイズ(1面)	*/
#define	D_DU_EEP_RFICBOT_FILE_SIZE_F		0x15C				/* RFIC-Bootファイルサイズ(Factory面)	*/

#define	D_DU_EEP_RFICARM_FILE_SIZE_0		0x160				/* RFIC-ARMファイルサイズ(0面)	*/
#define	D_DU_EEP_RFICARM_FILE_SIZE_1		0x164				/* RFIC-ARMファイルサイズ(1面)	*/
#define	D_DU_EEP_RFICARM_FILE_SIZE_F		0x168				/* RFIC-ARMファイルサイズ(Factory面)	*/

#define	D_DU_EEP_SOFTPAR_FILE_SIZE_0		0x16C				/* Softパラメータファイルサイズ(0面)	*/
#define	D_DU_EEP_SOFTPAR_FILE_SIZE_1		0x170				/* Softパラメータファイルサイズ(1面)	*/
#define	D_DU_EEP_SOFTPAR_FILE_SIZE_F		0x174				/* Softパラメータファイルサイズ(Factory面)	*/

#define	D_DU_EEP_PTPCOND_FILE_SIZE_0		0x1AE				/* PTP-PLLファイルサイズ(0面)	*/
#define	D_DU_EEP_PTPCOND_FILE_SIZE_1		0x1B2				/* PTP-PLLファイルサイズ(1面)	*/
#define	D_DU_EEP_PTPCOND_FILE_SIZE_F		0x1B6				/* PTP-PLLファイルサイズ(Factory面)	*/

#define	D_DU_EEP_BUILD_FILE_SIZE_0			0x1A2				/* ビルドファイルサイズ(0面)	*/
#define	D_DU_EEP_BUILD_FILE_SIZE_1			0x1A6				/* ビルドファイルサイズ(1面)	*/
#define	D_DU_EEP_BUILD_FILE_SIZE_F			0x1AA				/* ビルドファイルサイズ(Factory面)	*/

#define	D_DU_EEP_BUILD_DATE0				0x072				/* ビルド日付(0面)				*/
#define	D_DU_EEP_SOFT_VER0					0x076				/* OS+カーネルファイルバージョン(0面)	*/
#define	D_DU_EEP_PLFPGA_VER0				0x078				/* PL-FPGAファイルバージョン(0面)	*/
#define	D_DU_EEP_TRXFPGA_VER0				0x07A				/* PL-FPGAファイルバージョン(0面)	*/
#define	D_DU_EEP_RFICBOT_VER0				0x07C				/* PL-FPGAファイルバージョン(0面)	*/
#define	D_DU_EEP_RFICARM_VER0				0x07E				/* PL-FPGAファイルバージョン(0面)	*/
#define	D_DU_EEP_PTPCOND_VER0				0x082				/* PL-FPGAファイルバージョン(0面)	*/

#define	D_DU_EEP_BUILD_DATE1				0x0B2				/* ビルド日付(0面)				*/
#define	D_DU_EEP_SOFT_VER1					0x0B6				/* OS+カーネルファイルバージョン(0面)	*/
#define	D_DU_EEP_PLFPGA_VER1				0x0B8				/* PL-FPGAファイルバージョン(0面)	*/
#define	D_DU_EEP_TRXFPGA_VER1				0x0BA				/* PL-FPGAファイルバージョン(0面)	*/
#define	D_DU_EEP_RFICBOT_VER1				0x0BC				/* PL-FPGAファイルバージョン(0面)	*/
#define	D_DU_EEP_RFICARM_VER1				0x0BE				/* PL-FPGAファイルバージョン(0面)	*/
#define	D_DU_EEP_PTPCOND_VER1				0x0C2				/* PL-FPGAファイルバージョン(0面)	*/



 /* 5GDU_chg */
/* @} */
#endif
/* @} */

