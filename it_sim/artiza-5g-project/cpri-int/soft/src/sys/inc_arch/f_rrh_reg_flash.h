/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_flash.h
 * @brief flash関連定義
 * @date  2013/11/11 ALPHA) yokoyama Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_FLASH_H
#define F_RRH_REG_FLASH_H

#define D_RRH_QSPI_FLASH_FACE0							0		/**< Flash face#0				*/
#define D_RRH_QSPI_FLASH_FACE1							1		/**< Flash face#1				*/
#define D_RRH_QSPI_FLASH_FACE2							2		/**< Flash face#2				*/

/**
 * @name QSPI Flash Offset
 * @name QSPI Flash Offset define
 */
/* @{ */
#define D_RRH_QSPI_FLASH_OFFSET_BOOT					0x00000000	/*!< Boot領域(primary)								*/
#define D_RRH_QSPI_FLASH_OFFSET_BOOT_2					0x00080000	/*!< Boot領域(backup)								*/
#define D_RRH_QSPI_FLASH_OFFSET_BOOT_END				0x00100000	/*!< Boot領域最終位置								*/
#define D_RRH_QSPI_FLASH_OFFSET_OS						0x00100000	/*!< OS領域	(7M)									*/
#define D_RRH_QSPI_FLASH_OFFSET_TROUBLE0				0x06000000	/*!< 障害ログ領域0面#0(2048kbyte)					*/
#define D_RRH_QSPI_FLASH_OFFSET_TROUBLE1				0x06200000	/*!< 障害ログ領域0面#1(2048kbyte)					*/
#define D_RRH_QSPI_FLASH_OFFSET_TROUBLE2				0x0E000000	/*!< 障害ログ領域1面#0(2048kbyte)					*/
#define D_RRH_QSPI_FLASH_OFFSET_HEALTH					0x0E200000	/*!< 障害ログ領域1面#1(予備)(2048kbyte)				*/
#define D_RRH_QSPI_FLASH_OFFSET_INFO					0x06400000	/*!< サイクリックログ退避領域						*/
#define D_RRH_QSPI_FLASH_OFFSET_TRA_DATA_N_PGC_V		0x06A00000	/*!< mmW PGCトレパラ(通常V偏波)						*/
#define D_RRH_QSPI_FLASH_OFFSET_TRA_DATA_N_PGC_H		0x06A40000	/*!< mmW PGCトレパラ(通常H偏波)						*/
#define D_RRH_QSPI_FLASH_OFFSET_TRA_DATA_L_PGC_V		0x06A80000	/*!< mmW PGCトレパラ(低電力V偏波)					*/
#define D_RRH_QSPI_FLASH_OFFSET_TRA_DATA_L_PGC_H		0x06AC0000	/*!< mmW PGCトレパラ(低電力H偏波)					*/
#define D_RRH_QSPI_FLASH_OFFSET_WDTLOG					0x00A00000	/*!< WDTTOログ退避領域								*/
#define D_RRH_QSPI_FLASH_OFFSET_WDTLOG_WARN				0x00A10000	/*!< WDTTOログ(警告用)退避領域						*/
#define D_RRH_QSPI_FLASH_OFFSET_ECCLOG					0x00A20000	/*!< ECCログ退避領域								*/
#define D_RRH_QSPI_FLASH_OFFSET_PL						0x00B00000	/*!< PL-FPGAデバイス領域							*/
#define D_RRH_QSPI_FLASH_OFFSET_FPGA					0x80000000	/*!< FPGAデバイス領域								*/
#define D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE0		0x00000000	/*!< CPRI-FPGAデバイス領域0面(slave)				*/
#define D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE1		0x01000000	/*!< CPRI-FPGAデバイス領域1面(slave)				*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC					0x00F00000	/*!< RFICデバイス領域								*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX				0x00F90000	/*!< RFICデバイス領域								*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_RESERVE			0x00FA0000	/*!< RFIC予約										*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_TX_IMG1			0x00000000	/*!< RFICIMG領域									*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_TX_IMG2			0x00020000	/*!< RFICIMG領域									*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_RX_IMG1			0x00040000	/*!< RFICIMG領域									*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_RX_IMG2			0x00060000	/*!< RFICIMG領域									*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_CFG_RX				0x00080000	/*!< RFICCFG領域									*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX_H1			0x00090000	/*!< RFIC RXパラメータ								*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX_H2			0x00090040	/*!< RFIC RXパラメータ								*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX_ALL			0x0009006C	/*!< RFIC RXパラメータ								*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX_A			0x00090070	/*!< RFIC RXパラメータ								*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX_B			0x00090074	/*!< RFIC RXパラメータ								*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX_C			0x00090078	/*!< RFIC RXパラメータ								*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_RX_D			0x0009007C	/*!< RFIC RXパラメータ								*/
#define D_RRH_QSPI_FLASH_OFFSET_RFIC_PAR_DATA			0x00090080	/*!< RFIC RXパラメータ								*/
#define M_RRH_QSPI_FLASH_OFFSET_RFIC_ADDRGET(ofs)		(D_RRH_QSPI_FLASH_OFFSET_RFIC + ofs)

#define	D_RRH_QSPI_FLASH_OFFSET_FPGA_PL					0x00400000	/*!< PL-FPGA(Master)ファイル領域					*/
#define	D_RRH_QSPI_FLASH_OFFSET_KRNL_OS					0x03800000	/*!< OSカーネルファイル領域							*/
#define	D_RRH_QSPI_FLASH_OFFSET_RFIC_BOOT				0x06800000	/*!< RFIC Bootファイル領域							*/
#define	D_RRH_QSPI_FLASH_OFFSET_RFIC_ARM				0x06900000	/*!< RFIC ファームファイル領域						*/
#define	D_RRH_QSPI_FLASH_OFFSET_PTP_PARA				0x06D00000	/*!< PTP-PLL初期設定ファイル領域					*/
#define	D_RRH_QSPI_FLASH_OFFSET_TRX_FPGA_FACE0			0x00000000	/*!< FPGA(Slave)ファイル領域(0面)					*/
#define	D_RRH_QSPI_FLASH_OFFSET_TRX_FPGA_FACE1			0x02000000	/*!< FPGA(Slave)ファイル領域(1面)					*/

/* Softパラメータ [FACE:0]	*/
#define D_RRH_QSPI_FLASH_OFFSET_NETCONF_USER_INFO		0x06C00000	/*!< 障害ログ領域0面#0(2048kbyte)					*/
#define D_RRH_QSPI_FLASH_OFFSET_SEARCHABLE_VLAN_INFO	0x06C10000	/*!< 障害ログ領域0面#1(2048kbyte)					*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_02		0x06C20000	/*!< [FACE:0]->Softパラメータ #02 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_03		0x06C30000	/*!< [FACE:0]->Softパラメータ #03 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_04		0x06C40000	/*!< [FACE:0]->Softパラメータ #04 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_05		0x06C50000	/*!< [FACE:0]->Softパラメータ #05 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_06		0x06C60000	/*!< [FACE:0]->Softパラメータ #06 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_07		0x06C70000	/*!< [FACE:0]->Softパラメータ #07 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_08		0x06C80000	/*!< [FACE:0]->Softパラメータ #08 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_09		0x06C90000	/*!< [FACE:0]->Softパラメータ #09 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_10		0x06CA0000	/*!< [FACE:0]->Softパラメータ #10 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_11		0x06CB0000	/*!< [FACE:0]->Softパラメータ #11 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_12		0x06CC0000	/*!< [FACE:0]->Softパラメータ #12 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_13		0x06CD0000	/*!< [FACE:0]->Softパラメータ #13 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_14		0x06CE0000	/*!< [FACE:0]->Softパラメータ #14 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE0_SOFT_PARAM_15		0x06CF0000	/*!< [FACE:0]->Softパラメータ #15 (64kbyte)			*/

/* Softパラメータ [FACE:1]	*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_00		0x0EC00000	/*!< [FACE:1]->Softパラメータ #00 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_01		0x0EC10000	/*!< [FACE:1]->Softパラメータ #01 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_02		0x0EC20000	/*!< [FACE:1]->Softパラメータ #02 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_03		0x0EC30000	/*!< [FACE:1]->Softパラメータ #03 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_04		0x0EC40000	/*!< [FACE:1]->Softパラメータ #04 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_05		0x0EC50000	/*!< [FACE:1]->Softパラメータ #05 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_06		0x0EC60000	/*!< [FACE:1]->Softパラメータ #06 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_07		0x0EC70000	/*!< [FACE:1]->Softパラメータ #07 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_08		0x0EC80000	/*!< [FACE:1]->Softパラメータ #08 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_09		0x0EC90000	/*!< [FACE:1]->Softパラメータ #09 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_10		0x0ECA0000	/*!< [FACE:1]->Softパラメータ #10 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_11		0x0ECB0000	/*!< [FACE:1]->Softパラメータ #11 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_12		0x0ECC0000	/*!< [FACE:1]->Softパラメータ #12 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_13		0x0ECD0000	/*!< [FACE:1]->Softパラメータ #13 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_14		0x0ECE0000	/*!< [FACE:1]->Softパラメータ #14 (64kbyte)			*/
#define D_RRH_QSPI_FLASH_OFFSET_FACE1_SOFT_PARAM_15		0x0ECF0000	/*!< [FACE:1]->Softパラメータ #15 (64kbyte)			*/
/* @} */

/**
 * @name Flash Log Information
 * @name Flash Log Information define
 */
/* @{ */
#define D_RRH_FLASH_NOT_ERASED						0			/*!< Flash Log 領域 Erase未				*/
#define D_RRH_FLASH_ERASED							1			/*!< Flash Log 領域 Erase済み			*/
#define D_RRH_FLASH_WRITE_FIN						2			/*!< Flash Log 領域 書き込み完了		*/
/* @} */

/**
 * @name 障害ログ面情報
 * @name 障害ログ面情報 define
 */
/* @{ */
enum{
	E_RRH_QSPI_FLASH_TROUBLELOG_FACE0 = 0,						/*!< 障害ログ情報退避面#0							*/
	E_RRH_QSPI_FLASH_TROUBLELOG_FACE1,							/*!< 障害ログ情報退避面#1							*/
	E_RRH_QSPI_FLASH_TROUBLELOG_FACE2,							/*!< 障害ログ情報退避面#2							*/
	E_RRH_QSPI_FLASH_TROUBLELOG_FACE_MAX
};
/* @} */

/**
 * @name 障害ログSIZE情報
 * @name 障害ログSIZE情報 define
 */
/* @{ */
#define D_RRH_QSPI_FLASH_TROUBLELOG_SIZE				0x00200000	/*!< 障害ログサイズ									*/
/* @} */

/**
 * @name 障害ログ種別情報
 * @name 障害ログ種別情報 define
 */
/* @{ */
#define D_RRH_LOGTYPE_TROUBLE							0			/*!< 障害ログ(通常)									*/
#define D_RRH_LOGTYPE_HEALTH_CHCEK						1			/*!< 障害ログ(Health Check)							*/
/* @} */

/**
 * @name CyclicログSIZE情報
 * @name CyclicログSIZE情報 define
 */
/* @{ */
#define D_RRH_QSPI_FLASH_CYCLICLOG_SIZE					0x00300000	/*!< Cyclicログサイズ(圧縮後MAX)			*/

#define D_RRH_QSPI_FLASH_WDTLOG_SIZE					0x00010000	/*!< WDTログサイズ							*/
#define D_RRH_QSPI_FLASH_WDTLOG_WARN_SIZE				0x00010000	/*!< WDT(警告)ログサイズ					*/

#define D_RRH_QSPI_FLASH_OFFSET_ECCLOG_SIZE				0x00010000	/*!< ECCログ退避領域サイズ					*/
/* @} */

/**
 * @name MAXIMUM constant
 * @note MAXIMUM constant definition
 */
/* @{ */
#define	D_RRH_FLASH_WRT_MAX					2						/*!< Maximum of FLASH access info		*/
/* @} */

/**
 * @name File Size
 * @note File Size definition
 */
/* @{ */
#define	D_DU_QSPI_FLASH_OFFSET_FPGA_PL					0x00400000	/*!< PL-FPGA(Master)ファイル領域					*/
#define	D_DU_QSPI_FLASH_OFFSET_KRNL_OS					0x03800000	/*!< OSカーネルファイル領域							*/
#define	D_DU_QSPI_FLASH_OFFSET_RFIC_BOOT				0x06800000	/*!< RFIC Bootファイル領域							*/
#define	D_DU_QSPI_FLASH_OFFSET_RFIC_ARM					0x06900000	/*!< RFIC ファームファイル領域						*/
#define	D_DU_QSPI_FLASH_OFFSET_PTP_PARA					0x06D00000	/*!< PTP-PLL初期設定ファイル領域					*/
#define	D_DU_QSPI_FLASH_OFFSET_TRX_FPGA_FACE0			0x00000000	/*!< FPGA(Slave)ファイル領域(0面)					*/
#define	D_DU_QSPI_FLASH_OFFSET_TRX_FPGA_FACE1			0x02000000	/*!< FPGA(Slave)ファイル領域(1面)					*/
#define	D_DU_QSPI_FLASH_OFFSET_LUT_LOG_0				0x07C00000	/*!< LUT LOG(PL側FLASH)								*/
#define	D_DU_QSPI_FLASH_OFFSET_LUT_LOG_1				0x07C80000	/*!< LUT LOG(PL側FLASH)								*/
#define	D_DU_QSPI_FLASH_OFFSET_LUT_LOG_2				0x07D00000	/*!< LUT LOG(PL側FLASH)								*/
#define	D_DU_QSPI_FLASH_OFFSET_LUT_LOG_3				0x07D80000	/*!< LUT LOG(PL側FLASH)								*/
#define	D_DU_QSPI_FLASH_OFFSET_LUT_LOG_HEAD				0x07DF0000	/*!< LUT LOG Header(PL側FLASH)						*/

#define D_DU_QSPI_FLASH_SIZE_BOOT_ONE			0x00040000				/* 256KB */
#define D_DU_QSPI_FLASH_SIZE_BOOT_PART			0x00080000				/* 512KB */
#define D_DU_QSPI_FLASH_SIZE_BOOT				0x00100000				/* 1M   */
#define D_DU_QSPI_FLASH_SIZE_FIRM				0x00700000				/* 7M   */
#define D_DU_QSPI_FLASH_SIZE_FPGA				0x01000000				/* 16M  */
#define D_DU_QSPI_FLASH_SIZE_RFIC				0x00090000				/* 128K*4 + 64K   */
#define D_DU_QSPI_FLASH_SIZE_PL					0x00400000				/* 4M   */

#define	D_DU_QSPI_FLASH_SIZE_FPGA_PL			0x02400000				/* PL Flashサイズ					*/
#define	D_DU_QSPI_FLASH_SIZE_KRNL_OS			0x02800000				/* OS Flashサイズ					*/
#define	D_DU_QSPI_FLASH_SIZE_RFIC_BOOT			0x00100000				/* RFIC BOOT Flashサイズ			*/
#define	D_DU_QSPI_FLASH_SIZE_RFIC_ARM			0x00100000				/* RFIC ファーム Flashサイズ		*/
#define	D_DU_QSPI_FLASH_SIZE_PTP_PARA			0x00020000				/* PTP-PLL Flashサイズ				*/
#define	D_DU_QSPI_FLASH_SIZE_TRX_FPGA			0x01700000				/* TRX Flashサイズ					*/
#define	D_DU_QSPI_FLASH_SIZE_LUT_LOG_ALL		0x00200000				/* LUT_LOG ALL						*/
#define	D_DU_QSPI_FLASH_SIZE_LUT_LOG_ONE		0x00070000				/* LUT_LOG １個分					*/
#define	D_DU_QSPI_FLASH_SIZE_LUT_LOG_HEAD		0x00010000				/* LUT_LOG ヘッダサイズ				*/

#define	D_DU_QSPI_FLASH_SIZE_WRITE				1024 * 1024				/* ファーム更新用バッファ			*/
#define	D_DU_QSPI_FLASH_SIZE_SOFT_PARA			64 * 1024				/* Netconf更新用バッファ			*/

#define D_DU_QSPI_FLASH_FACE0							0		/**< Flash face#0				*/
#define D_DU_QSPI_FLASH_FACE1							1		/**< Flash face#1				*/
#define D_DU_QSPI_FLASH_FACE2							2		/**< Flash face#2				*/

/* @} */
#endif
/* @} */
