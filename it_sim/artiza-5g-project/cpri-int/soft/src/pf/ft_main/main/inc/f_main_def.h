/*!
 * @skip		$ld:$
 * @file		f_main_def.h
 * @brief		pf_main define定義
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 * @date		2014/12/05 alpha)岡部 Modify
 * @date  		2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  		2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 * @date        2015/08/25 ALPHA)Miyazaki   TDD_RRE対応
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
#define	D_MAIN_CREATE_PTN_OTHER						0x0003						/*!< 生成・起動パターン(別パターン)	*/

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




#define D_MAIN_CTB_CFG_MASK						0x01000100		/**< CTB CFGレジスタ マスク値				*/
#define D_MAIN_CTB_CFG_EXPECT					0x00000100		/**< CTB CFGレジスタ 期待値					*/
#define D_MAIN_CTB_CFG_TRX1_MASK				0x01000100		/**< CTB CFGレジスタ TRX-FPGA#1 Config mask	*/
#define D_MAIN_CTB_CFG_TRX1_DONE				0x00000100		/**< CTB CFGレジスタ TRX-FPGA#1 Config NODE	*/
#define D_MAIN_CTB_CFG_WAIT_CNT					100				/**< CTB CFGレジスタ 監視時間 (100ms×100回)*/

#define D_MAIN_SET_PHY_STATE_VAL				0x00000800		/**< PHY Statusレジスタ 設定値				*/

#define D_MAIN_1NS_WAIT							1				/**< 待ち時間ベース (1ns)					*/
#define D_MAIN_1US_WAIT							1000			/**< 待ち時間ベース (1us)					*/
#define D_MAIN_1MS_WAIT							1000000			/**< 待ち時間ベース (1ms)					*/
#define D_MAIN_1S_WAIT							1				/**< 待ち時間ベース (1s)					*/


/* PCIe起動	*/
#define D_MAIN_SET_CHKREG_PTN1					0x5555AAAA		/**< 設定チェックレジスタ 設定パターン1		*/
#define D_MAIN_SET_CHKREG_PTN2					0xAAAA5555		/**< 設定チェックレジスタ 設定パターン2		*/

#define D_MAIN_SET_UPPER_END_IRQ_CLR			0x04000001		/**< Upper End IRQクリアレジスタ 設定値		*/
#define D_MAIN_SET_UPPER_END_IRQ_MSK			0x04000001		/**< Upper End IRQマスクレジスタ 設定値		*/

#define D_MAIN_VAL_PHY_STATE_CNT				3				/**< PHY Statusレジスタ Polling回数			*/

/* PCIe起動(TRX-FPGA1)	*/
#define D_MAIN_SET_IRQ_SET_DECODE_TRX1			0x0FF30FE9		/**< IRQ Decode設定レジスタ 設定値			*/
#define D_MAIN_SET_IRQ_MASK_TRX1				0x0FF00FE9		/**< IRQ Decode設定レジスタ 設定値			*/
#define D_MAIN_SET_REL_RESET_TRX1				0x08800000		/**< ROOT/ENDリセット解除 (TRX-FPGA1)		*/
#define D_MAIN_SET_REL_RESET2_TRX1				0x00080000		/**< ROOT/ENDリセット解除 (TRX-FPGA1)		*/
#define D_MAIN_SET_STA1_RESET_TRX1				0x00001000		/**< CTB STA1マスク解除bit/PCIe起動bit		*/
#define D_MAIN_SET_STA1_RESET2_TRX1				0x00030000		/**< CTB STA1マスク解除bit/PCIe起動bit		*/
#define D_MAIN_SET_COMMAND_TRX1					0x00000147		/**< Commandレジスタ 設定値					*/
#define D_MAIN_SET_SEC_LET_TIMER_TRX1			0xFFFF0100		/**< Second Latency Timerレジスタ 設定値	*/
#define D_MAIN_SET_ROOT_PORT_STA_CNT_TRX1		0x00000001		/**< Root Port Status/ Controlレジスタ 設定値*/

#define D_MAIN_SET_BHLC_TRX1					0x00EF0000		/**< BIST/Header/Last timer/CachLn ERROR Reg*/
#define D_MAIN_SET_STATE_COMMAND_TRX1			0x00000006		/**< Status/Commandレジスタ 設定値			*/
#define D_MAIN_SET_BASE_ADDR_TRX1				0x00000000		/**< Base Addressレジスタ 設定値			*/

#define D_MAIN_SET_BHLC_TRX1					0x00EF0000		/**< BIST/Header/Last timer/CachLn ERROR Reg*/

/* TRX-PL SV-IF確認	*/
#define	D_MAIN_SET_CTB_STA1M					0x00000000		/**< CTB TA1Mレジスタ 設定値				*/
#define	D_MAIN_SET_CTB_STA2M					0x00000000		/**< CTB TA2Mレジスタ 設定値				*/
#define	D_MAIN_SET_CTB_STA1						0x10000000		/**< CTB TA1レジスタ 設定値					*/
#define	D_MAIN_SET_CTB_STA1_TRX1				0x01000000		/**< CTB TA1レジスタ 設定値(TRX1)			*/
#define	D_MAIN_SET_CTB_STA2						0x01000000		/**< CTB TA2レジスタ 設定値					*/
#define	D_MAIN_SET_CTB_STA2_SRAM1				0x01000000		/**< CTB TA2レジスタ 設定値(SRAM1)			*/
#define	D_MAIN_SET_CTB_SFPSTA2M					0x000001FF		/**< CTB FPSTA2Mレジスタ 設定値				*/

/* AISG */
#define D_MAIN_SET_CTB_IRQAISGM					0x0000001F		/**< CTB IRQAISGMレジスタ 設定値			*/


#define D_MAIN_SET_PL_SVIF_CHK					0x10000000		/**< TRXx PL SV-IF Checkレジスタ 設定値		*/
#define D_MAIN_SET_PL_SVIF_CNF					0x00000000		/**< TRXx PL SV-IF Configレジスタ 設定値	*/

#define D_MAIN_CTB_STA1_WAIT_CNT				100				/**< CTB TA1レジスタ 監視時間 (1ms×100回)	*/
#define D_MAIN_CTB_STA2_WAIT_CNT				1000			/**< CTB TA2レジスタ 監視時間 (100us×1000回)*/
#define D_MAIN_PL_SVIF_CHK_CNT					10				/**< PL SV-IF Check  監視時間 (1ms×10回)	*/
/* @} */

/*!
 * @name PFスレッド数情報定義
 * @note 
 * @{
 */
#define D_MAIN_THDID_MAX						D_SYS_THDID_PF_MAX			/*!< pf_mainで起動するスレッドの最大数		*/
/* @}
 */


/*!
 * @name リセット要因設定用メッセージサイズ定義
 * @note 
 * @{
 */
#define D_MAIN_MESSAGE_SIZE							64			/*!< メッセージサイズ						*/
/* @} */

/*!
 * @name Flashログ展開定義
 * @note 
 * @{
 */
#define D_MAIN_FLASH_LOG_COMP_ON					1			/* Flashログ圧縮有り						*/
#define D_MAIN_FLASH_LOG_COMP_OFF					0			/* Flashログ圧縮無し						*/
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

#define D_MAIN_INV_BOOTVER_SIZE						0x00000004					/*!< Boot Version Flash読み込みサイズ				*/
/* @} */

/* スレッド内イベント番号(ログ用)				*/
#define	D_MAIN_EVT_NO1								0x1001						/*!< スレッド内イベント番号No1		*/
#define	D_MAIN_EVT_NO2								0x1002						/*!< スレッド内イベント番号No2		*/
#define	D_MAIN_EVT_NO3								0x1003						/*!< スレッド内イベント番号No3		*/
#define	D_MAIN_EVT_NO4								0x1004						/*!< スレッド内イベント番号No4		*/
#define	D_MAIN_EVT_NO5								0x1005						/*!< スレッド内イベント番号No5		*/
#define	D_MAIN_EVT_NO6								0x1006						/*!< スレッド内イベント番号No6		*/
#define	D_MAIN_EVT_NO7								0x1007						/*!< スレッド内イベント番号No7		*/
#define	D_MAIN_EVT_NO8								0x1008						/*!< スレッド内イベント番号No8		*/
#define	D_MAIN_EVT_NO9								0x1009						/*!< スレッド内イベント番号No9		*/
#define	D_MAIN_EVT_NO10								0x100a						/*!< スレッド内イベント番号No10		*/
#define	D_MAIN_EVT_NO11								0x100b						/*!< スレッド内イベント番号No11		*/
#define	D_MAIN_EVT_NO12								0x100c						/*!< スレッド内イベント番号No12		*/
#define	D_MAIN_EVT_NO13								0x100d						/*!< スレッド内イベント番号No13		*/
#define	D_MAIN_EVT_NO14								0x100e						/*!< スレッド内イベント番号No14		*/
#define	D_MAIN_EVT_NO15								0x100f						/*!< スレッド内イベント番号No15		*/

/* @} */

/*!
 * @name キャリア番号定義
 * @note 
 * @{
 */
#define D_MAIN_CAR_NUM0				0x0000		/**< carrier number is 0 */
#define D_MAIN_CAR_NUM1				0x0001		/**< carrier number is 1 */
#define D_MAIN_CAR_NUM2				0x0002		/**< carrier number is 2 */
#define D_MAIN_CAR_NUM3				0x0003		/**< carrier number is 3 */
#define D_MAIN_CAR_NUM4				0x0004		/**< carrier number is 4 */
/* @} */

/* FLASH EOL対応 add start */
/*!
 * @name ユーザーIP QSPI初期化設定関連定義
 * @note 
 * @{
 */
#define D_MAIN_QSPI_PORING_CNT						100									/* QSPI Poring Count			*/
#define D_MAIN_QSPI_WAIT_TIME						10									/* QSPI Poring wait time		*/
#define D_MAIN_QSPI_FLAGSTATUS_CNT					2									/* Flag Status Register Count	*/ /* FLASH EOL対応 */
/* @} */
/* FLASH EOL対応 add end */

/*!
 * @name NETCONFユーザー情報
 * @note NETCONFユーザー情報の仮登録。(pf_nmaでFLASHから収集するように変更する予定)
 * @{
 */
#define D_MAIN_NETCONF_USER_0						"o-ran-user\0"						/**< NETCONFユーザー情報#0 アカウント  */
#define D_MAIN_NETCONF_PASSWORD_0					"o-ran-password\0"					/**< NETCONFユーザー情報#0 パスワード  */
#define D_MAIN_NETCONF_GROUP_0						"sudo\0"							/**< NETCONFユーザー情報#0 ユーザー権限  */

#define D_MAIN_NETCONF_USER_1						"\0"								/**< NETCONFユーザー情報#1 アカウント  */
#define D_MAIN_NETCONF_PASSWORD_1					"\0"								/**< NETCONFユーザー情報#1 パスワード  */
#define D_MAIN_NETCONF_GROUP_1						"\0"								/**< NETCONFユーザー情報#1 ユーザー権限  */


#endif
/* @} */
