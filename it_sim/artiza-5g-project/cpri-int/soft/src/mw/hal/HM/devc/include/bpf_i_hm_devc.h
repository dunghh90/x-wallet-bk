/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_hm_devc.h
 *  @brief  Definitions for BPF DEVC Local header
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#ifndef _BPF_I_HM_DEVC_H_
#define _BPF_I_HM_DEVC_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_HM_DEVC.h"
#include "bpf_l_com.h"
#include "BPF_COM_PLOG.h"
#include <linux/i2c-dev.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/mman.h>
#include <mtd/mtd-user.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include "f_rrh_inc.h"
#include "f_rrh_reg_eep.h"
#include "f_rrh_reg_cpld.h"

/********************************************************************************************************************/
/* Internal Definition                                                                                              */
/********************************************************************************************************************/
#define DEVC_FLG_OFF             0
#define DEVC_FLG_ON              1

#define BPF_HM_DEVC_SYS_NG   -1 /* システムコールNG */
#define BPF_HM_DEVC_SIZE_ZERO 0 /* 書き込み・読み込みサイズ0 */

#define BPF_HM_DEVC_TIME_STR_SIZE    11 /* 時刻文字列サイズ */

/* レジスタアドレス位置 */
#define BPF_HM_DEVC_CONTROL_OFFSET_RTC1			0xA0000010   /* RTC1                  */
#define BPF_HM_DEVC_CONTROL_OFFSET_RTC2			0xA0000014   /* RTC2                  */

/* EEPROM WRITEプロテクト	*/
#define BPF_HM_DEVC_EEP_WRITE_PROTECT_ADDR		0xFF0A0044		/* EEPROM WRITE PROTECT Addr	*/
#define BPF_HM_DEVC_EEP_WRITE_PROTECT_MASK		0x00001000		/* EEPROM WRITE PROTECT Mask	*/

/* CPU RTCパラメータ操作定義 */
#define BPF_HMDEVC_MONTH_OFFSET         1
#define BPF_HMDEVC_YEARE_OFFSET         1900
#define BPF_HMDEVC_2037_OFFSET          2037
#define BPF_HMDEVC_2037_OFFSET_BCD    0x2037
#define BPF_HMDEVC_2066_OFFSET          2066
#define BPF_HMDEVC_2100_OFFSET          2100
#define BPF_HMDEVC_YEAR_1970            (1970-BPF_HMDEVC_YEARE_OFFSET)
#define BPF_HMDEVC_YEAR_2030            (2030-BPF_HMDEVC_YEARE_OFFSET)
#define BPF_HMDEVC_YEAR_2037            (2037-BPF_HMDEVC_YEARE_OFFSET)
#define BPF_HMDEVC_YEAR_2066            (2066-BPF_HMDEVC_YEARE_OFFSET)
#define BPF_HMDEVC_YEAR_2090            (2090-BPF_HMDEVC_YEARE_OFFSET)
#define BPF_HMDEVC_YEAR_2100            (2100-BPF_HMDEVC_YEARE_OFFSET)

#define BPF_HMDEVC_TIM_OFFSET_2030      0x70DBD880
#define BPF_HMDEVC_TIM_OFFSET_2090      0xE1B7B100
#define BPF_HMDEVC_TIM_OFFSET_2100      0xF4865700

/* CPU I2Cパラメータ操作定義 */
#define BPF_HM_DEVC_WAIT_TO_100MS 100000000    /* 100ms秒タイマ           */
#define BPF_HM_DEVC_WAIT_TO_10MS   10000000    /* 10ms秒タイマ            */
#define BPF_HM_DEVC_WAIT_TO_1MS     1000000    /* 1ms秒タイマ             */
#define BPF_HM_DEVC_WAIT_TO_1US        1000    /* 1us秒タイマ             */
#define BPF_HM_DEVC_WAIT_TO_2US        2000    /* 2us秒タイマ             */
#define BPF_HM_DEVC_WAIT_TO_100US    100000    /* 100us秒タイマ           */
#define BPF_HM_DEVC_WAIT_TO_100NS       100    /* 100ns秒タイマ           */
#define BPF_HM_DEVC_WAIT_TO_MUTEX     20000    /* mutexガードタイマ       */

/* WDT関連定義 */
/* AXI TIMER 	*/
#define BPF_HM_DEVC_REG_AXITIMERCLRPLS0				0xA0010000			/* Timer0 クリア				*/
#define BPF_HM_DEVC_REG_AXITIMERCLRPLS1				0xA0010010			/* Timer1 クリア				*/
#define BPF_HM_DEVC_DATA_AXITIMER_CLR					0x00000000		/* タイマクリア					*/
#define BPF_HM_DEVC_DATA_AXITIMER_CNTLOAD				0x00000020		/* カウンタ設定Load				*/
#define BPF_HM_DEVC_DATA_AXITIMER_EN					0x000000C2		/* Timer0 EN、他				*/
#define BPF_HM_DEVC_DATA_AXITIMER_DIS					0x00000100		/* タイマDisable				*/
#define BPF_HM_DEVC_DATA_AXITIMER_VAL					0x0BEBC1FE		/* タイマ値(8s)					*/

#define BPF_HM_DEVC_REG_AXITIMERTLR0 				0xA0010004			/* タイマ設定					*/

/* SWDT(REG)	*/
#define BPF_HM_DEVC_REG_SWDT_MODE					0xFD4D0000			/* MODE							*/
#define BPF_HM_DEVC_REG_SWDT_CONTROL				0xFD4D0004			/* CONTROL						*/
#define BPF_HM_DEVC_REG_SWDT_CLEAR					0xFD4D0008			/* Clear register				*/
/* SWDT(MODE) */
#define	BPF_HM_DEVC_DATA_SWDT_MODE_DIS					0x00ABC1C0		/* Disable						*/
#define	BPF_HM_DEVC_DATA_SWDT_MODE_EN					0x00ABC1C3		/* Enable						*/
/* SWDT(CONTROL) */
#define	BPF_HM_DEVC_DATA_SWDT_CONTROL_TIMER				0x009201C3		/* Timer(19S)					*/
#define	BPF_HM_DEVC_DATA_SWDT_CONTROL_TIMER_EXT			0x009204A7		/* Timer(50S)					*/
#define BPF_HM_DEVC_DATA_SWDT_CLEAR						0x00001999		/* Clear Timer Value			*/
/* @} */
/* WDT関連定義 end */


/* I2Cベースアドレス     */
#define BPF_HM_DEVC_REG_I2C0_BASE			0xFF020000	/* I2C0ベースアドレス			*/
#define BPF_HM_DEVC_REG_I2C1_BASE			0xFF030000	/* I2C1ベースアドレス			*/
#define BPF_HM_DEVC_REG_RST_LPD_IOU2		0xFF5E0238	/* (CRL_APB)b10:I2C1reset b9:I2C0reset	*/

#define BPF_HM_DEVC_I2C_BASE_ADD			0x50		/* I2C(EEPROM)ベースアドレス	*/
#define BPF_HM_DEVC_I2C_BASE_ADD_CPLD		0x70		/* I2C(CPLD)ベースアドレス		*/
#define BPF_HM_DEVC_I2C_BASE_ADD_TEMP		0x48		/* I2C(温度)ベースアドレス		*/

/* SFPアクセス用定義	*/
#define	BPF_HM_DEVC_REG_SFP_CTB_I2CSEL		0xA0000580	/* SFP アクセス選択レジスタ	*/
#define	BPF_HM_DEVC_SFP_BASE_ADD_A0			0x5000		/* SFP A0アクセス	*/
#define	BPF_HM_DEVC_SFP_BASE_ADD_A2			0x5100		/* SFP A2アクセス	*/
#define	BPF_HM_DEVC_SFP_BASE_ADD_SA0   		0x6000  	/* Slave SFP A0アクセス */
#define	BPF_HM_DEVC_SFP_BASE_ADD_SA2   		0x6100  	/* Slave SFP A2アクセス */
#define	BPF_HM_DEVC_SFPP_ENABLE				0x0001		/* SFPPアクセスEnableBit	*/
#define	BPF_HM_DEVC_SFPS_ENABLE				0x0002  	/* Slave SFPアクセスEnableBit */

#define	BPF_HM_DEVC_REG_SFP_SEL				0xFF0A0048	/* SFP アクセス選択レジスタ	*/
#define	BPF_HM_DEVC_SFP1_ENABLE				0x0040		/* SFP1アクセスEnableBit	*/
#define	BPF_HM_DEVC_SFP2_ENABLE				0x0080  	/* SFP2アクセスEnableBit */

#define	BPF_SFP_ENABLE						1			/* SFP Enable 	*/
#define	BPF_SFP_DISABLE						0			/* SFP Disable 	*/

/* QSPI-FLASHアクセス定義	*/
/* QSPI-FLASH U/L設定アドレス */
#define BPF_HM_DEVC_FLASH_UL_BOUND    0x08000000  /* QSPI-FLASH U/L境界(16byte) */
/* ZYNC QPSI CHIP選択ビット */
#define BPF_HM_DEVC_QSPI_CHIP_SEL_BIT   0x10000000
#define BPF_HM_DEVC_QSPI_TWO_MEM_BIT    0x40000000

/* ZYNQ QPSI OFFSET */
#define BPF_HM_DEVC_QSPI_BOOT_OFFSET1   0x00000000
#define BPF_HM_DEVC_QSPI_BOOT_OFFSET2   0x00100000
#define BPF_HM_DEVC_QSPI_BOOT_FSBL_START      0x30
#define BPF_HM_DEVC_QSPI_BOOT_FSBL_SIZE       0x40
#define BPF_HM_DEVC_QSPI_BOOT_VER             0x2C
#define BPF_HM_DEVC_QSPI_BOOT_ID_OFFSET       0x24
#define BPF_HM_DEVC_QSPI_BOOT_ID_NAME         "XNLX"  /* Flash上にはリトルエンディアンで格納されているためそれに合わせる(実際は XLNX) */

/* PS QSPIアクセスコマンドアドレス */
#define ZYNQ_QSPI_CONFIG_REG       0xFF0F0000
#define ZYNQ_QSPI_INTR_STATUS_REG  0xFF0F0004
#define ZYNQ_QSPI_TXD0             0xFF0F001C
#define ZYNQ_QSPI_TXD1             0xFF0F0080
#define ZYNQ_QSPI_TXD2             0xFF0F0084
#define ZYNQ_QSPI_LQSPI_CFG        0xFF0F00A0
#define ZYNQ_QSPI_RX_DATA_REG      0xFF0F0020

#define BPF_HM_DEVC_FLASH_SECTOR			0x01000000	/* FLASHセクタサイズ			*/

#define BPF_HM_DEVC_FLASH_OFFSET_BOOT_END	0x00200000	/* Flash offset(boot最終)		*/
/* QSPI-FLASHアクセス定義 end	*/

/* PL-QSPI(SLAVE-FLASH)アクセス用定義	*/
/* PL QSPIアクセスコマンドダミーデータサイズ */
#define BPF_HM_DEVC_PLQSPI_DUMMY_BYTE			3

/* PL QSPIアクセスコマンド設定値 */
#define BPF_HM_DEVC_PLQSPI_CMD_CS_SEL_ON		0xFFFFFFFE	/* CS選択					*/
#define BPF_HM_DEVC_PLQSPI_CMD_CS_SEL_OFF		0xFFFFFFFF	/* CS選択解除				*/
#define BPF_HM_DEVC_PLQSPI_CMD_TRANS_ENABLE		0x00000086	/* データ送信イネーブル		*/
#define BPF_HM_DEVC_PLQSPI_CMD_TRANS_DISABLE	0x00000186	/* データ送信ディセーブル	*/
#define BPF_HM_DEVC_PLQSPI_CMD_SYS_DISABLE		0x00000184	/* SPIシステムディセーブル	*/

#define BPF_HM_DEVC_PLQSPI_WRITE_ENABLE				0x06	/* WRITE ENABLE コマンド	*/
#define BPF_HM_DEVC_PLQSPI_WRITE_EXT_ADR			0xC5	/* WRITE EXTENDED ADDRESS コマンド	*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* Flash EOL対応 add start	*/
#define BPF_HM_DEVC_PLQSPI_QUAD_FAST_PROGRAM		0x32	/* QUAD INPUT FAST PROGRAMコマンド	*/
#define BPF_HM_DEVC_PLQSPI_PAGE_PROGRAM				0x02	/* PAGE PROGRAMコマンド(Macronix)	*/
#define BPF_HM_DEVC_PLQSPI_READ_FLG_STS_REG			0x70	/* Flag Status register read コマンド	*/
#define BPF_HM_DEVC_PLQSPI_READ_STS_REG				0x05	/* Status register read コマンド	*/
// #define BPF_HM_DEVC_PLQSPI_WRITE_VLT_REG			0x81	/* WRITE VOLATILE CONFIGURATION REGISTER コマンド	*/
// #define BPF_HM_DEVC_PLQSPI_QUAD_FAST_READ			0x6B	/* QUAD OUTPUT FAST READコマンド	*/
// #define BPF_HM_DEVC_PLQSPI_WRITE_STS_REG			0x01	/* write status/configuration register コマンド	*/

#define BPF_HM_DEVC_QSPI_READ_ID					0x9F	/* Flash Kind(Manufacture ID)読出しコマンド */
/* Flash EOL対応 add end	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* PL QSPIオフセット位置 */
#define BPF_HM_DEVC_PLQSPI_OFFSET_BASE			0xA0020000	/* base offset				*/
#define BPF_HM_DEVC_PLQSPI_OFFSET_RESET			0xA0020040	/* Software reset			*/
#define BPF_HM_DEVC_PLQSPI_OFFSET_CNT			0xA0020060	/* control					*/
#define BPF_HM_DEVC_PLQSPI_OFFSET_STATE			0xA0020064	/* status					*/
#define BPF_HM_DEVC_PLQSPI_OFFSET_TX			0xA0020068	/* data transmit			*/
#define BPF_HM_DEVC_PLQSPI_OFFSET_RX			0xA002006C	/* data receive				*/
#define BPF_HM_DEVC_PLQSPI_OFFSET_CS			0xA0020070	/* slave select				*/

/* PL QSPI面選択 */
#define BPF_HM_DEVC_PLQSPI_BOUNDARY_ADD			0x08000000	/* PLQSPI境界面アドレス		*/
#define BPF_HM_DEVC_PLQSPI_FACE_0				0x0			/* PLQSPI用FLASH 0面		*/
#define BPF_HM_DEVC_PLQSPI_FACE_1				0x1			/* PLQSPI用FLASH 0面		*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#define BPF_HM_DEVC_PLQSPI_CENTER_ADD			0x02000000	/* PLQSPI_Centerアドレス	*/
#define BPF_HM_DEVC_PLQSPI_FACE_LOWER			0x00		/* PLQSPI用FLASH LOWER面	*/
#define BPF_HM_DEVC_PLQSPI_FACE_SECOND			0x01		/* PLQSPI用FLASH SECOND面	*/
#define BPF_HM_DEVC_PLQSPI_FACE_THIRD			0x02		/* PLQSPI用FLASH THIRD面	*/
#define BPF_HM_DEVC_PLQSPI_FACE_UPPER			0x03		/* PLQSPI用FLASH UPPER面	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

#define BPF_HM_DEVC_TX_MAX_CNT_998        998         /* カウント最大値（998）  */
#define BPF_HM_DEVC_TX_MAX_CNT_1000       1000        /* カウント最大値（1000） */
#define BPF_HM_DEVC_TX_MAX_CNT_5000       5000        /* カウント最大値（5000） */
#define BPF_HM_DEVC_TX_MAX_CNT_9998       9998        /* カウント最大値（9998） */
#define BPF_HM_DEVC_TX_MAX_CNT_10000      10000       /* カウント最大値（10000）*/
#define BPF_HM_DEVC_TX_MAX_CNT_999998     999998      /* カウント最大値（999998） */
#define BPF_HM_DEVC_TX_MAX_CNT_1000000    1000000     /* カウント最大値（1000000）*/

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* Flash EOL対応 add start	*/
#define BPF_HM_DEVC_PLQSPI_FLGSTSCHKBIT	 0x00000080		/* Flag Status Register確認値(Micron)	*/
#define BPF_HM_DEVC_PLQSPI_STSREGCHKBIT	 0x00000001		/* Status Register確認値(Macronix)	*/
/* Flash EOL対応 add end	*/

/* PL QSPI(SLAVE-RE_FILE)オフセット位置 */
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_BASE			0x40030000	/* base offset				*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_RESET			0x40030040	/* Software reset			*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT			0x40030060	/* control					*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_STATE			0x40030064	/* status					*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_TX			0x40030068	/* data transmit			*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_RX			0x4003006C	/* data receive				*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CS			0x40030070	/* slave select				*/

/* PL QSPIアクセスコマンド設定値 */
#define BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS0		0xFFFFFFFE	/* CSss0選択				*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS1		0xFFFFFFFD	/* CSss1選択				*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS2		0xFFFFFFFB	/* CSss2選択				*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_SS3		0xFFFFFFF7	/* CSss3選択				*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_OFF		0xFFFFFFFF	/* CS選択解除				*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_TRANS_ENABLE		0x00000086	/* データ送信イネーブル		*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_TRANS_DISABLE	0x00000186	/* データ送信ディセーブル	*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_SYS_DISABLE		0x00000184	/* SPIシステムディセーブル	*/

/* PL QSPI(SLAVE-RE_FILE)オフセット位置 */
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_BASE			0x40030000	/* base offset				*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_RESET			0x40030040	/* Software reset			*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT			0x40030060	/* control					*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_STATE			0x40030064	/* status					*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_TX			0x40030068	/* data transmit			*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_RX			0x4003006C	/* data receive				*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CS			0x40030070	/* slave select				*/

/* PL QSPI面選択 */
#define BPF_HM_DEVC_PLQSPI_RE_FILE_MAXSIZE		0x01000000	/* RE FILE領域最大サイズ	*/
#define BPF_HM_DEVC_PLQSPI_RE_FILE_DEV_NUM		0x04		/* RE FILE用FLASH device数	*/
/* PL-QSPI(RE_FILE)アクセス用定義 end	*/

/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* SPI関連 */
#define BPF_HM_DEVC_SPI_SPI0			0					/* SPI選択:SPI0							*/
#define BPF_HM_DEVC_SPI_SPI1			1					/* SPI選択:SPI1							*/

#define BPF_HM_DEVC_SPI_RFIC				0x00			/* RFICアクセス用		*/
#define BPF_HM_DEVC_SPI_EXTPLL				0x01			/* EXTPLLアクセス用		*/
#define BPF_HM_DEVC_SPI_PTPPLL				0x02			/* PTPPLLアクセス用		*/
#define BPF_HM_DEVC_SPI_RFPLL				0x03			/* RF-LLアクセス用		*/

#define BPF_HM_DEVC_SPI_CONFIG_INIT_RFIC	(0x00027C0D)	/* RFICアクセス用設定値				*/
#define BPF_HM_DEVC_SPI_CONFIG_INIT_EXTPLL	(0x00027C0D)	/* EXTPLLアクセス用設定値				*/
#define BPF_HM_DEVC_SPI_CONFIG_INIT_PTPPLL	(0x00027C0D)	/* PTPPLLアクセス用設定値				*/

#define BPF_HM_ZYNQ_SPI_OFFSET_BASE			(0xFF000000)
#define BPF_HM_ZYNQ_SPI0_OFFSET				(0xFF040000)
#define BPF_HM_ZYNQ_SPI1_OFFSET				(0xFF050000)

#define BPF_HM_SPI_INTRPT_EN_INIT			(0x0000007F)

#define BPF_HM_ZYNQ_SPI_CONFIG				(0x00000000)
#define BPF_HM_ZYNQ_SPI_INTR_STATUS			(0x00000004)
#define BPF_HM_ZYNQ_SPI_INTRPT_EN			(0x00000008)
#define BPF_HM_ZYNQ_SPI_EN					(0x00000014)
#define BPF_HM_ZYNQ_SPI_DELAY				(0x00000018)
#define BPF_HM_ZYNQ_SPI_TX_DATA				(0x0000001C)
#define BPF_HM_ZYNQ_SPI_RX_DATA				(0x00000020)
#define BPF_HM_ZYNQ_SPI_CONFIG_CS_BIT		(0x00000400)

#define BPF_HM_ZYNQ_SPI_INTR_STATUS_IXR_RXNEMPTY		(0x00000010)

#define BPF_HM_DEVC_SPI_REG_RFIC			0xA0000500
#define BPF_HM_DEVC_SPI_REG_RFIC_RESET		0xA0000040
#define BPF_HM_DEVC_SPI_VAL_RFIC_FWFB1			0x00000005
#define BPF_HM_DEVC_SPI_VAL_RFIC_FWFB2			0x00000006
#define BPF_HM_DEVC_SPI_VAL_RFIC_RX1			0x00000009
#define BPF_HM_DEVC_SPI_VAL_RFIC_RX2			0x0000000A

#define BPF_HM_DEVC_SPI_FIFO			128			/* SPI用 FIFO			*/

#define BPF_HM_DEVC_SPI0_CS0			0					/* SPI0 - ss0 : PTP-PLL						*/
#define BPF_HM_DEVC_SPI0_CS1			1					/* SPI0 - ss1 : RF-IC						*/
#define BPF_HM_DEVC_SPI0_CS2			2					/* SPI0 - ss2 : なし						*/
#define BPF_HM_DEVC_SPI1_CS0			3					/* SPI1 - ss0 : EXT-PLL						*/
#define BPF_HM_DEVC_SPI1_CS1			4					/* SPI1 - ss1 : なし						*/
#define BPF_HM_DEVC_SPI1_CS2			5					/* SPI1 - ss2 : なし						*/

#define BPF_HM_DEVC_SPI_CS0				0					/* CS選択:CS0							*/
#define BPF_HM_DEVC_SPI_CS1				1					/* CS選択:CS1							*/
#define BPF_HM_DEVC_SPI_CS2				2					/* CS選択:CS2							*/

#define BPF_HM_DEVC_QSPICMD_READID		0x9F				/* Flash Kind(Manufacture ID)読出しコマンド */

/* 汎用SPI関連 */
#define BPF_HM_DEVC_SPI_TYPE_FANDAC             0
#define BPF_HM_DEVC_SPI_TYPE_TXDAC              1
#define BPF_HM_DEVC_SPI_TYPE_RXADC              2
#define BPF_HM_DEVC_SPI_TYPE_TXDAC_OFFSET       0x00020002
#define BPF_HM_DEVC_SPI_TYPE_RXADC_OFFSET       0x00020004
#define BPF_HM_DEVC_SPI_TYPE_FANADC_OFFSET      0x00030001

#define BPF_HM_DEVC_SPI_PORT_OFFSET             0xa0000700    /* 汎用ポートオフセット             */
#define BPF_HM_DEVC_SPI_CLOCK_OFFSET            0xA0000704    /* 汎用SPI クロック設定オフセット   */
#define BPF_HM_DEVC_SPI_EN_OFFSET               0xA0000708    /* 汎用SPI イネーブル設定オフセット */
#define BPF_HM_DEVC_SPI_TX_DATA_OFFSET          0xA000070c    /* 汎用SPI 出力制御設定オフセット   */
#define BPF_HM_DEVC_SPI_SEND_DATA_OFFSET        0xA0000710    /* 汎用SPI 送信データ設定オフセット */
#define BPF_HM_DEVC_SPI_RX_DATA_OFFSET          0xA0000714    /* 汎用SPI 受信データ設定オフセット */


#define BPF_HM_DEVC_SPI_PGC_IF_OFFSET				0xA2000A00
#define BPF_HM_DEVC_SPI_PGC_CH1_GAIN_OFFSET			0xA2000A10
#define BPF_HM_DEVC_SPI_PGC_CH1_PAHSE_OFFSET		0xA2000A14
#define BPF_HM_DEVC_SPI_PGC_CH1_CTL_OFFSET			0xA2000A18
#define BPF_HM_DEVC_SPI_PGC_CH2_GAIN_OFFSET			0xA2000A20
#define BPF_HM_DEVC_SPI_PGC_CH2_PAHSE_OFFSET		0xA2000A24
#define BPF_HM_DEVC_SPI_PGC_CH2_CTL_OFFSET			0xA2000A28
#define BPF_HM_DEVC_SPI_PGC_CH3_GAIN_OFFSET			0xA2000A30
#define BPF_HM_DEVC_SPI_PGC_CH3_PAHSE_OFFSET		0xA2000A34
#define BPF_HM_DEVC_SPI_PGC_CH3_CTL_OFFSET			0xA2000A38
#define BPF_HM_DEVC_SPI_PGC_CH4_GAIN_OFFSET			0xA2000A40
#define BPF_HM_DEVC_SPI_PGC_CH4_PAHSE_OFFSET		0xA2000A44
#define BPF_HM_DEVC_SPI_PGC_CH4_CTL_OFFSET			0xA2000A48

#define BPF_HM_DEVC_SPI_PGC_CH1_RX_GAIN_OFFSET		0xA2000A50
#define BPF_HM_DEVC_SPI_PGC_CH1_RX_PAHSE_OFFSET		0xA2000A54
#define BPF_HM_DEVC_SPI_PGC_CH1_RX_CTL_OFFSET		0xA2000A58
#define BPF_HM_DEVC_SPI_PGC_CH2_RX_GAIN_OFFSET		0xA2000A60
#define BPF_HM_DEVC_SPI_PGC_CH2_RX_PAHSE_OFFSET		0xA2000A64
#define BPF_HM_DEVC_SPI_PGC_CH2_RX_CTL_OFFSET		0xA2000A68
#define BPF_HM_DEVC_SPI_PGC_CH3_RX_GAIN_OFFSET		0xA2000A70
#define BPF_HM_DEVC_SPI_PGC_CH3_RX_PAHSE_OFFSET		0xA2000A74
#define BPF_HM_DEVC_SPI_PGC_CH3_RX_CTL_OFFSET		0xA2000A78
#define BPF_HM_DEVC_SPI_PGC_CH4_RX_GAIN_OFFSET		0xA2000A80
#define BPF_HM_DEVC_SPI_PGC_CH4_RX_PAHSE_OFFSET		0xA2000A84
#define BPF_HM_DEVC_SPI_PGC_CH4_RX_CTL_OFFSET		0xA2000A88

#define BPF_HM_DEVC_SPI_PGC_MODE_ENABLE				0x80000000
#define BPF_HM_DEVC_SPI_PGC_BUSY					0x00020000
#define BPF_HM_DEVC_SPI_PGC_ACCESS					0x00030000

#define BPF_HM_DEVC_SPI_PGCLUT_OFFSET_START			0xA2008000
#define BPF_HM_DEVC_SPI_PGCLUT_OFFSET_END			0xA200DA00

/* PLL Select */
#define BPF_HM_DEVC_PLL_SELECT_OFFSET				0xFF0A0048
#define BPF_HM_DEVC_PLL_SELECT_ENB_EXTPLL			0x00000080	/*[7:6]="10"	*/
#define BPF_HM_DEVC_PLL_SELECT_ENB_RFPLL			0x00000040	/*[7:6]="01"	*/
#define BPF_HM_DEVC_PLL_SELECT_DIS					0x000000C0

/* PCI関連 */
#define BPF_HM_DEVC_PCI_AREA_START					0xB0000000
#define BPF_HM_DEVC_PCI_AREA_END					0xB1FFFFFF

/* I2C(SoftMacro)関連 */
#define BPF_HM_DEVC_I2C_READ          0             /* I2C(SoftMacro) read処理          */
#define BPF_HM_DEVC_I2C_WRITE         1             /* I2C(SoftMacro) write処理         */
#define BPF_HM_DEVC_I2C_PGC_NORMAL    0             /* I2C(SoftMacro) Normal処理        */
#define BPF_HM_DEVC_I2C_PGC_LUT       1             /* I2C(SoftMacro) LUT処理           */
#define BPF_HM_DEVC_I2C_READ_SIZE     0x1           /* I2C(SoftMacro) readサイズ(1byte) */

#define BPF_HM_DEVC_I2C_PGC_GRP1      1             /* I2C(SoftMacro)GRP  グループ1(V#1～V#4)   */
#define BPF_HM_DEVC_I2C_PGC_GRP2      2             /* I2C(SoftMacro)GRP  グループ2(V#5～V#8)   */
#define BPF_HM_DEVC_I2C_PGC_GRP3      3             /* I2C(SoftMacro)GRP  グループ3(V#9～V#12)  */
#define BPF_HM_DEVC_I2C_PGC_GRP4      4             /* I2C(SoftMacro)GRP  グループ4(V#13～V#16) */
#define BPF_HM_DEVC_I2C_PGC_GRP5      5             /* I2C(SoftMacro)GRP  グループ5(H#1～H#4)   */
#define BPF_HM_DEVC_I2C_PGC_GRP6      6             /* I2C(SoftMacro)GRP  グループ6(H#5～H#8)   */
#define BPF_HM_DEVC_I2C_PGC_GRP7      7             /* I2C(SoftMacro)GRP  グループ7(H#9～H#12)  */
#define BPF_HM_DEVC_I2C_PGC_GRP8      8             /* I2C(SoftMacro)GRP  グループ8(H#13～H#16) */

#define BPF_HM_DEVC_I2C_OFFSET0       0xA0030000    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET1       0xA0030100    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET2       0xA0030104    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET3       0xA0030108    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET4       0xA0030124    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET5       0xA0030040    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET6       0xA0030120    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET7       0xA003013C    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET8       0xA0030140    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET9       0xA003010C    /* I2C(SoftMacro)オフセット */
#define BPF_HM_DEVC_I2C_OFFSET10      0xA2000E10    /* I2C(SoftMacro)オフセット */

#define BPF_HM_DEVC_I2C_PAGE_OFFSET0  0x000         /* I2C(SoftMacro)ページ番号オフセット */
#define BPF_HM_DEVC_I2C_PAGE_OFFSET1  0x100         /* I2C(SoftMacro)ページ番号オフセット */
#define BPF_HM_DEVC_I2C_PAGE_OFFSET2  0x200         /* I2C(SoftMacro)ページ番号オフセット */
#define BPF_HM_DEVC_I2C_PAGE_OFFSET3  0x300         /* I2C(SoftMacro)ページ番号オフセット */
#define BPF_HM_DEVC_I2C_PAGE_OFFSET4  0x400         /* I2C(SoftMacro)ページ番号オフセット */
#define BPF_HM_DEVC_I2C_PAGE_OFFSET5  0x500         /* I2C(SoftMacro)ページ番号オフセット */
#define BPF_HM_DEVC_I2C_PAGE_OFFSET6  0x600         /* I2C(SoftMacro)ページ番号オフセット */
#define BPF_HM_DEVC_I2C_PAGE_OFFSET7  0x700         /* I2C(SoftMacro)ページ番号オフセット */

#define BPF_HM_DEVC_I2C_PAGEDATA0  0x0              /* I2C(SoftMacro)ページ番号 */
#define BPF_HM_DEVC_I2C_PAGEDATA1  0x1              /* I2C(SoftMacro)ページ番号 */
#define BPF_HM_DEVC_I2C_PAGEDATA2  0x2              /* I2C(SoftMacro)ページ番号 */
#define BPF_HM_DEVC_I2C_PAGEDATA3  0x3              /* I2C(SoftMacro)ページ番号 */
#define BPF_HM_DEVC_I2C_PAGEDATA4  0x4              /* I2C(SoftMacro)ページ番号 */
#define BPF_HM_DEVC_I2C_PAGEDATA5  0x5              /* I2C(SoftMacro)ページ番号 */
#define BPF_HM_DEVC_I2C_PAGEDATA6  0x6              /* I2C(SoftMacro)ページ番号 */
#define BPF_HM_DEVC_I2C_PAGEDATA7  0x7              /* I2C(SoftMacro)ページ番号 */

/* TRX配下SPI関連 */
#define BPF_HM_DEVC_TRXSPI_PORT_OFFSET          0xB0000700     /* TRX配下SPI 汎用ポートオフセット     */
#define BPF_HM_DEVC_TRXSPI_CLOCK_OFFSET         0xB0000704     /* TRX配下SPI クロック設定オフセット   */
#define BPF_HM_DEVC_TRXSPI_EN_OFFSET            0xB0000708     /* TRX配下SPI イネーブル設定オフセット */
#define BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET       0xB000070C     /* TRX配下SPI 出力制御設定オフセット   */
#define BPF_HM_DEVC_TRXSPI_SEND_DATA_OFFSET     0xB0000710     /* TRX配下SPI 送信データ設定オフセット */
#define BPF_HM_DEVC_TRXSPI_RX_DATA_OFFSET       0xB0000714     /* TRX配下SPI 受信データ設定オフセット */

#define BPF_HM_DEVC_TRXSPI_CLOCK_TxVATT_PA_VGDAC   0x00FFFF00     /* TRX配下SPI クロック設定(TxVATT/PA_VGDAC アクセス(sub6))       */

#define BPF_HM_DEVC_TRXSPI_CLOCK_QMOD           0x0FFFFFF0     /* TRX配下SPI クロック設定(QMOD/QDEM)       */
#define BPF_HM_DEVC_TRXSPI_CLOCK_DAC            0x00FFFF00     /* TRX配下SPI クロック設定(DAC)             */
#define BPF_HM_DEVC_TRXSPI_CLOCK_VATTDAC        0x000AAA00     /* TRX配下SPI クロック設定(VATTDAC)         */
#define BPF_HM_DEVC_TRXSPI_EN_QMOD              0xF0000007     /* TRX配下SPI イネーブル設定(QMOD/QDEM)     */
#define BPF_HM_DEVC_TRXSPI_EN_DAC               0xFF0000FF     /* TRX配下SPI イネーブル設定(DAC)           */
#define BPF_HM_DEVC_TRXSPI_EN_VATTDAC           0x000000C0     /* TRX配下SPI イネーブル設定(VATTDAC)       */
#define BPF_HM_DEVC_TRXSPI_TX_DATA_ON           0xFFFFFFFF     /* TRX配下SPI 出力制御設定(ON)              */
#define BPF_HM_DEVC_TRXSPI_TX_READ_DATA_ON      0xFFE00000     /* TRX配下SPI 出力制御設定(ON)              */
#define BPF_HM_DEVC_TRXSPI_TX_DATA_OFF          0x00000000     /* TRX配下SPI 出力制御設定(OFF)             */
#define BPF_HM_DEVC_TRXSPI_TARGET_SUB6PAVGDAC   0x00030200     /* TRX配下SPI ターゲット設定(SUB6PAVG_DAC)  */
#define BPF_HM_DEVC_TRXSPI_TARGET_SUB6TXVATTDAC 0x00030100     /* TRX配下SPI ターゲット設定(SUB6TxVATT_DAC)*/


#define BPF_HM_DEVC_TRXSPI_TARGET_QMOD_V          0x00020003     /* TRX配下SPI ターゲット設定(QMOD/MIX)      */
#define BPF_HM_DEVC_TRXSPI_TARGET_QDEM_V          0x00020004     /* TRX配下SPI ターゲット設定(QMOD/VGA)      */
#define BPF_HM_DEVC_TRXSPI_TARGET_QMOD_H          0x00020018     /* TRX配下SPI ターゲット設定(QMOD)          */
#define BPF_HM_DEVC_TRXSPI_TARGET_QDEM_H          0x00020020     /* TRX配下SPI ターゲット設定(QDEM)          */
#define BPF_HM_DEVC_TRXSPI_TARGET_MMWQMODDAC      0x00030040     /* TRX配下SPI ターゲット設定(QDEM/QMOD-DAC) */
#define BPF_HM_DEVC_TRXSPI_TARGET_MMWPAVGDAC      0x00030080     /* TRX配下SPI ターゲット設定(MMWPAVG_DAC)   */
#define BPF_HM_DEVC_TRXSPI_TARGET_MMWTXVATTDAC_V  0x00020400     /* TRX配下SPI ターゲット設定(MMWTxVATT_DAC) */
#define BPF_HM_DEVC_TRXSPI_TARGET_MMWRXVATTDAC_V  0x00020800     /* TRX配下SPI ターゲット設定(MMWTxVATT_DAC) */
#define BPF_HM_DEVC_TRXSPI_TARGET_MMWTXVATTDAC_H  0x00021000     /* TRX配下SPI ターゲット設定(MMWTxVATT_DAC) */
#define BPF_HM_DEVC_TRXSPI_TARGET_MMWRXVATTDAC_H  0x00022000     /* TRX配下SPI ターゲット設定(RxVATT_DAC)    */

#define BPF_HM_DEVC_TRXSPI_VATTDAC_SHIFT        0x0000000A     /* TRX配下SPI データ設定ベースシフト(SUB6)  */
#define BPF_HM_DEVC_TRXSPI_VATTDAC_DATA         0x000FFF00     /* TRX配下SPI 設定データ                    */
#define BPF_HM_DEVC_TRXSPI_ANTNO_SHIFT          0x00000016     /* TRX配下SPI アンテナ番号ベースシフト      */
#define BPF_HM_DEVC_TRXSPI_PORT_ENABLE          0x00020000     /* TRX配下SPI Port Busy                     */
#define BPF_HM_DEVC_TRXSPI_VATTDAC_SHIFTMMW     0x00000008     /* TRX配下SPI データ設定ベースシフト(MMW)   */
#define BPF_HM_DEVC_TRXSPI_PAVGDAC_DATA         0x00100000     /* TRX配下SPI 固定値データ                  */

#define BPF_HM_DEVC_TRXSPI_VATTDAC_ANTA         0x00000000     /* TRX配下SPI ANT-A情報                     */
#define BPF_HM_DEVC_TRXSPI_VATTDAC_ANTB         0x00000001     /* TRX配下SPI ANT-B情報                     */
#define BPF_HM_DEVC_TRXSPI_VATTDAC_ANTC         0x00000002     /* TRX配下SPI ANT-C情報                     */
#define BPF_HM_DEVC_TRXSPI_VATTDAC_ANTD         0x00000003     /* TRX配下SPI ANT-D情報                     */
#define BPF_HM_DEVC_TRXSPI_VATTDAC_ANTE         0x00000004     /* TRX配下SPI ANT-E情報                     */
#define BPF_HM_DEVC_TRXSPI_VATTDAC_ANTF         0x00000005     /* TRX配下SPI ANT-F情報                     */
#define BPF_HM_DEVC_TRXSPI_VATTDAC_ANTG         0x00000006     /* TRX配下SPI ANT-G情報                     */
#define BPF_HM_DEVC_TRXSPI_VATTDAC_ANTH         0x00000007     /* TRX配下SPI ANT-H情報                     */

#define BPF_HM_DEVC_TRXSPI_SEL0                 0x00000000     /* TRX配下SPI セレクタ情報                  */
#define BPF_HM_DEVC_TRXSPI_SEL1                 0x00000001     /* TRX配下SPI セレクタ情報                  */
#define BPF_HM_DEVC_TRXSPI_SEL2                 0x00000002     /* TRX配下SPI セレクタ情報                  */
#define BPF_HM_DEVC_TRXSPI_SEL3                 0x00000003     /* TRX配下SPI セレクタ情報                  */
#define BPF_HM_DEVC_TRXSPI_SEL4                 0x00000004     /* TRX配下SPI セレクタ情報                  */
#define BPF_HM_DEVC_TRXSPI_SEL5                 0x00000005     /* TRX配下SPI セレクタ情報                  */
#define BPF_HM_DEVC_TRXSPI_SEL6                 0x00000006     /* TRX配下SPI セレクタ情報                  */
#define BPF_HM_DEVC_TRXSPI_SEL7                 0x00000007     /* TRX配下SPI セレクタ情報                  */
#define BPF_HM_DEVC_TRXSPI_SEL8                 0x00000008     /* TRX配下SPI セレクタ情報                  */

#define BPF_HM_DEVC_TRXSPI_QMOD_ADDRESS_SHIFT   0x00000015     /* TRX配下SPI アドレス設定ベースシフト      */
#define BPF_HM_DEVC_TRXSPI_QMOD_ADDRESS_EN_BIT  0x0000003F     /* TRX配下SPI アドレス有効範囲              */
#define BPF_HM_DEVC_TRXSPI_QMOD_DATA_SHIFT      0x00000005     /* TRX配下SPI Writeデータ設定ベースシフト   */
#define BPF_HM_DEVC_TRXSPI_QMOD_DATA_EN_BIT     0x0000FFFF     /* TRX配下SPI Writeデータ有効範囲           */
#define BPF_HM_DEVC_TRXSPI_QMOD_DATA            0x000FFF00     /* TRX配下SPI 設定データ                    */

#define BPF_HM_DEVC_PAVGDAC_SUB6ANT             8
#define BPF_HM_DEVC_PAVGDAC_MMWANT              4

#define BPF_HM_DEVC_SOFTMAC_TX_FIFO             0x00000080      /* TX-FIFO Empty確認                       */
#define BPF_HM_DEVC_SOFTMAC_RX_FIFO             0x00000040      /* RX-FIFO Empty確認                       */
#define BPF_HM_DEVC_SOFTMAC_BUSY                0x00000004      /* I2C Bus busy確認                        */

#define BPF_HM_DEVC_RFEEP_ADDR_MASK             0x000000FF      /* I2C(EEP)アドレスマスク                  */


/* デバイスアクセス */
#define BPF_HM_DEVC_MTD0		"/dev/mtd0"
#define BPF_HM_DEVC_I2C0		"/dev/i2c-0"
#define BPF_HM_DEVC_I2C1		"/dev/i2c-1"
#define BPF_HM_DEVC_SPI00		"/dev/spidev2.0"
#define BPF_HM_DEVC_SPI01		"/dev/spidev2.1"
#define BPF_HM_DEVC_SPI02		"/dev/spidev2.2"
#define BPF_HM_DEVC_SPI10		"/dev/spidev3.0"
#define BPF_HM_DEVC_SPI11		"/dev/spidev3.1"
#define BPF_HM_DEVC_SPI12		"/dev/spidev3.2"
#define BPF_HM_DEVC_UIO0		"/dev/uio0"
#define BPF_HM_DEVC_UIO1		"/dev/uio1"
#define BPF_HM_DEVC_UIO2		"/dev/uio2"
#define BPF_HM_DEVC_UIO3		"/dev/uio3"
#define BPF_HM_DEVC_UIO4		"/dev/uio4"
#define BPF_HM_DEVC_UIO5		"/dev/uio5"
#define BPF_HM_DEVC_UIO6		"/dev/uio6"
#define BPF_HM_DEVC_UIO7		"/dev/uio7"
#define BPF_HM_DEVC_UIO8		"/dev/uio8"
#define BPF_HM_DEVC_UIO9		"/dev/uio9"
#define BPF_HM_DEVC_UIO10		"/dev/uio10"
#define BPF_HM_DEVC_UIO11		"/dev/uio11"
#define BPF_HM_DEVC_UIO12		"/dev/uio12"
#define BPF_HM_DEVC_UIO13		"/dev/uio13"
#define BPF_HM_DEVC_UIO14		"/dev/uio14"
#define BPF_HM_DEVC_UIO15		"/dev/uio15"
#define BPF_HM_DEVC_UIO16		"/dev/uio16"
#define BPF_HM_DEVC_UIO17		"/dev/uio17"
#define BPF_HM_DEVC_UIO18		"/dev/uio18"
#define BPF_HM_DEVC_UIO19		"/dev/uio19"
#define BPF_HM_DEVC_UIO20		"/dev/uio20"
#define BPF_HM_DEVC_UIO21		"/dev/uio21"
#define BPF_HM_DEVC_UIO22		"/dev/uio22"
#define BPF_HM_DEVC_UIO23		"/dev/uio23"
#define BPF_HM_DEVC_UIO24		"/dev/uio24"
#define BPF_HM_DEVC_UIO25		"/dev/uio25"
#define BPF_HM_DEVC_UIO26		"/dev/uio26"
#define BPF_HM_DEVC_UIO27		"/dev/uio27"
#define BPF_HM_DEVC_UIO28		"/dev/uio28"
#define BPF_HM_DEVC_UIO29		"/dev/uio29"
#define BPF_HM_DEVC_UIO30		"/dev/uio30"
#define BPF_HM_DEVC_UIO31		"/dev/uio31"
#define BPF_HM_DEVC_NULL		"NULL"

/********************************************************************************************************************/
/* Internal enum                                                                                                    */
/********************************************************************************************************************/
/* 全装置共通情報	*/
enum ipcm_msgq_used {
	BPF_HM_DEVC_UID_QSPI_FLASH  = 0,
	BPF_HM_DEVC_UID_I2C_0,
	BPF_HM_DEVC_UID_I2C_1,
	BPF_HM_DEVC_UID_I2C_2,
	BPF_HM_DEVC_UID_SPI_0,
	BPF_HM_DEVC_UID_SPI_1,
	BPF_HM_DEVC_UID_SPI_2,
	BPF_HM_DEVC_UID_SPI_3,
	BPF_HM_DEVC_UID_SPI_4,
	BPF_HM_DEVC_UID_WDT,
	BPF_HM_DEVC_UID_QSPI_SLAVE_FLASH,
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
	BPF_HM_DEVC_RE_FILE_FLASH,
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
	BPF_HM_DEVC_COM_LAST
};

/* 装置個別情報(UIO定義) ソース上では使用することは無いがDevice数管理のために定義	*/
/* ZYNQ		*/
/* device tree 参照 */
enum {
	BPF_HM_DEVC_UID_cpu = BPF_HM_DEVC_COM_LAST,
	BPF_HM_DEVC_UID_pcie_root,
	BPF_HM_DEVC_UID_pcie_endp,
	BPF_HM_DEVC_UID_hpm0_cnt,
	BPF_HM_DEVC_UID_hpm0_eth,
	BPF_HM_DEVC_UID_hpm0_phyl,
	BPF_HM_DEVC_UID_hpm1_trx1,
	BPF_HM_DEVC_UID_hpm1_trx2,
	BPF_HM_DEVC_UID_hpm1_trx3,
	BPF_HM_DEVC_UID_hpm1_trx4,
	BPF_HM_DEVC_UID_hpm1_pcie,
	BPF_HM_DEVC_UID_dummy11,
	BPF_HM_DEVC_UID_dummy12,
	BPF_HM_DEVC_UID_dummy13,
	BPF_HM_DEVC_UID_dummy14,
	BPF_HM_DEVC_UID_dummy15,
	BPF_HM_DEVC_UID_dummy16,
	BPF_HM_DEVC_UID_dummy17,
	BPF_HM_DEVC_UID_dummy18,
	BPF_HM_DEVC_UID_dummy19,
	BPF_HM_DEVC_UID_dummy20,
	BPF_HM_DEVC_UID_dummy21,
	BPF_HM_DEVC_UID_dummy22,
	BPF_HM_DEVC_UID_dummy23,
	BPF_HM_DEVC_UID_dummy24,
	BPF_HM_DEVC_UID_dummy25,
	BPF_HM_DEVC_UID_dummy26,
	BPF_HM_DEVC_UID_dummy27,
	BPF_HM_DEVC_UID_dummy28,
	BPF_HM_DEVC_UID_dummy29,
	BPF_HM_DEVC_UID_dummy30,
	BPF_HM_DEVC_UID_dummy31,
	BPF_HM_DEVC_ZYNQ_LAST
};


#define BPF_HM_DEVC_QSPI_FLASH	BPF_HM_DEVC_UID_QSPI_FLASH
#define BPF_HM_DEVC_I2C_0		BPF_HM_DEVC_UID_I2C_0
#define BPF_HM_DEVC_I2C_1		BPF_HM_DEVC_UID_I2C_1
#define BPF_HM_DEVC_I2C_2		BPF_HM_DEVC_UID_I2C_2
#define BPF_HM_DEVC_IOPERI		BPF_HM_DEVC_UID_cpu
#define BPF_HM_DEVC_WDT			BPF_HM_DEVC_UID_WDT
#define BPF_HM_DEVC_SPI_0		BPF_HM_DEVC_UID_SPI_0
#define BPF_HM_DEVC_SPI_1		BPF_HM_DEVC_UID_SPI_1
#define BPF_HM_DEVC_SPI_2		BPF_HM_DEVC_UID_SPI_2
#define BPF_HM_DEVC_SPI_3		BPF_HM_DEVC_UID_SPI_3
#define BPF_HM_DEVC_SPI_4		BPF_HM_DEVC_UID_SPI_4
#define BPF_HM_DEVC_SLAVE_FLASH	BPF_HM_DEVC_UID_QSPI_SLAVE_FLASH

/* !!!!!!CAUTION!!!!!!															*/
/* デバイス情報最大数(共通+個別で一番大きいサイズのものを指定すること)			*/
/* 2014/12/17現在はZYNQ装置が一番Device数が多いためZYNQ装置のenumを設定しておく	*/
#define BPF_HM_DEVC_DEVCNT_MAX			BPF_HM_DEVC_ZYNQ_LAST + 1

/********************************************************************************************************************/
/* Internal Structure                                                                                               */
/********************************************************************************************************************/
/* 共有メモリに存在 */
typedef struct{
    pthread_mutex_t         mutex;               /* mutexオブジェクト        */
}t_bpf_hm_devc_info;

typedef struct {
	t_bpf_hm_devc_info      info[ BPF_HM_DEVC_DEVCNT_MAX ];  /* 各種情報                 */
	int                     force_access;                    /* 強制アクセスモード       */
	int                     force_pid;                       /* 強制モード中プロセスID   */
	int                     force_tid;                       /* 強制モード中スレッドID   */
	unsigned int            device_kind;                     /* 装置種別                 */
	int                     fpga_access;                     /* レジスタアクセス可否     */
	int                     pl_config;                       /* PL-CONFIG状態            */
	unsigned int            qspi_side;                       /* QSPI 0面/1面状態         */
	unsigned char           qspi_uplw;                       /* QSPI Upper/Lower状態     */
/* FLASH EOL対応 add start */
	unsigned char           flash_kind;                      /* PL側Flash種別(0:Micron/1:Macronix) */
	unsigned char           ps_flash_kind;                   /* PS側Flash種別(0:Micron/1:Macronix) */
	unsigned char           reserve[1];                      /* reserve                  */
/* FLASH EOL対応 add end */
	unsigned int            wdt_set_axi;                     /* WDT初期化フラグ(AXI)     */
	unsigned int            wdt_set_swdt;                    /* WDT初期化フラグ(SWDT)    */
	unsigned int            spi_rfic_ctrl;                   /* SPI(RFIC)制御状態        */
	unsigned int            mmw_pgclog;                      /* PGCLOG                   */

	unsigned int            pa_vgadac[4];                    /* pa_vgadac                */
	unsigned int            qmod_dac[4];                     /* qmod_dac                 */
	unsigned int            qdem_dac[4];                     /* qdem_dac                 */
	unsigned int            tx_vatt[4];                      /* tx_vatt                  */
	unsigned int            rx_vatt[4];                      /* tx_vatt                  */

	unsigned int            time_correction_suppress_cnt;     /* 時刻補正抑止        */
	unsigned int            time_force_update_flg;            /* 時刻補正必要有無    */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* FLASH EOL対応 add start */
	unsigned char           flash_kind_pl;                   /* PL FPGA Flash種別(0:Micron/1:Macronix) */
/* FLASH EOL対応 add end */
/* FLASH EOL対応 add start */
	unsigned char           flash_kind_re[4];                /* RE file Flash種別(0:Micron/1:Macronix) */
/* FLASH EOL対応 add end */
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

} t_bpf_hm_devc_data;

extern t_bpf_hm_devc_data*    di_devc_data_p;

/* グローバルテーブルに存在 */
typedef struct{
    unsigned int  validflg;             /* 有効無効フラグ          */
    char*         filename;             /* ファイル名              */
    unsigned int  startoffset;          /* 開始アドレス            */
    unsigned int  filesize;             /* ファイルサイズ          */
    unsigned int  access_type;          /* アクセスタイプ(2byte/4byte)  */
    unsigned int  irq_no;               /* IRQ番号                 */
    unsigned int  fpga_state;           /* FPGA状態に影響を受けるか否か(OFF=受けない)		*/
    unsigned int  pl_config_state;      /* PL-CONFIG状態に影響を受けるか否か(OFF=受けない)	*/
    int           fd;                   /* ファイルディスクリプタ  */
    char          *map;                 /* マップディスクリプタ    */
}t_bpf_hm_devc_fd_info;

typedef struct {
	t_bpf_hm_devc_fd_info   info[ BPF_HM_DEVC_DEVCNT_MAX ];   /* 各種情報      */
} t_bpf_hm_devc_fd_data;

/* 実際に使用するテーブル */
extern t_bpf_hm_devc_fd_data  di_devc_data_fd;

/* Device情報テーブル	*/
extern const t_bpf_hm_devc_fd_data  di_devc_data_fd_data_zynqU;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* Flash EOL対応 add start	*/
/* PL QSIP-FLASH Writeコマンドテーブル	*/
extern const unsigned char plQspiFlash_write_cmd[2];
extern const unsigned char plQspiFlash_stsReg[2];
extern const unsigned char plQspiFlash_stsReg_chkbit[2];
extern const unsigned char plQspiFlash_stsReg_rsltbit[2];
/* Flash EOL対応 add end	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/********************************************************************************************************************/
/* Internal Function Routine Prototype                                                                              */
/********************************************************************************************************************/
extern unsigned int  di_devc_assign_flag;

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
/* Flash EOL対応 chg start	*/
extern unsigned int reQspiFlashIndx;
extern const unsigned int flash_re_file_face2data[BPF_HM_DEVC_RE_FILE_FLASH_FACE_MAX][3];
/* Flash EOL対応 chg end	*/
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

int bpf_hm_dev_reg_info(volatile unsigned int offset, volatile int* read_offset,volatile int* size,volatile char** map_descriptor,volatile pthread_mutex_t  **mutex, volatile int* access_type);
int bpf_hm_dev_io_info(volatile unsigned int offset, volatile int* read_offset,volatile int* size,volatile char** map_descriptor,volatile pthread_mutex_t  **mutex, volatile int* access_type);
int bpf_hm_dev_reg_read_simple ( volatile int access_type, volatile char *map_descriptor, volatile int offset, volatile int size, unsigned int *data_p );
int bpf_hm_dev_reg_write_simple( volatile int access_type, volatile char *map_descriptor, volatile int offset, volatile int size, unsigned int *data_p );
void bpf_hm_dev_reg_read_2byte( volatile char *map_descriptor, volatile int offset, volatile int size, unsigned int *data_p );
void bpf_hm_dev_reg_read_4byte( volatile char *map_descriptor, volatile int offset, volatile int size, unsigned int *data_p );
int bpf_hm_dev_reg_read ( volatile int access_type, volatile char *map_descriptor, volatile int offset, volatile int size, volatile pthread_mutex_t *mutex, unsigned int *data_p );
void bpf_hm_dev_reg_write_2byte( volatile char *map_descriptor, volatile int offset, volatile int size, unsigned int *data_p );
void bpf_hm_dev_reg_write_4byte( volatile char *map_descriptor, volatile int offset, volatile int size, unsigned int *data_p );
int bpf_hm_dev_reg_write( volatile int access_type, volatile char *map_descriptor, volatile int offset, volatile int size, volatile pthread_mutex_t *mutex, unsigned int *data_p );
int bpf_hm_dev_irq_info(unsigned int dev,int* file_descriptor,pthread_mutex_t  **mutex);
int bpf_hm_devc_open();
int bpf_hm_devc_data_ini();

int bpf_hm_devc_reg_read_fpga(unsigned int offset, unsigned int *data_p);
int bpf_hm_devc_reg_write_fpga(unsigned int offset, unsigned int *data_p);
int bpf_hm_i2c_read( unsigned int kind, int setpage, unsigned short offset, unsigned int count, unsigned int access_type, unsigned char *data_p );
int bpf_hm_i2c_read_no_mutex( unsigned int kind, int setpage, unsigned short offset, unsigned int count, unsigned int access_type, unsigned char *data_p );
int bpf_hm_i2c_write( unsigned int kind, int setpage, unsigned short offset, unsigned char *data_p, unsigned short size);
int bpf_hm_i2c_write_no_mutex( unsigned int kind, int setpage, unsigned short offset, unsigned char *data_p, unsigned short size);

int bpf_hm_devc_mutex_lock( pthread_mutex_t* mutex );
int bpf_hm_devc_delay( int sec, int nsec);
int bpf_hm_devc_qspi_flash_slave_pl_qspi(unsigned int dat_len, unsigned char *tx_dat, unsigned char *rx_dat);

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
int bpf_hm_devc_qspi_flash_re_file_qspi(unsigned int dat_len, unsigned int *tx_dat, unsigned int *rx_dat, unsigned short cs_cnt, unsigned int ss);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

int bpf_hm_devc_select_qspi_area2(int file_descriptor, unsigned int face, unsigned int offset);

int bpf_hm_devc_spi_reg_read(unsigned int offset);
int bpf_hm_devc_spi_reg_write(unsigned int offset,unsigned int data);
void bpf_hm_devc_spi_spi_csend(unsigned int bus, unsigned int configSpeed);
int bpf_hm_devc_spi_ps_spi(unsigned int bus, unsigned int cs, unsigned int dat_len, unsigned int *tx_dat, unsigned int *rx_dat, unsigned short cs_cnt, unsigned int configSpeed);
int bpf_hm_devc_spi_pl_spi(unsigned int dat_len, unsigned int *tx_dat, unsigned int target_type, unsigned int *rx_dat);
int bpf_hm_devc_spi_pl_spi_reg_read(unsigned int offset, unsigned int *data_p);
int bpf_hm_devc_spi_pl_spi_reg_write(unsigned int offset, unsigned int data);
int bpf_hm_i2c_soft_rfeep(unsigned int eep_page, unsigned int address, unsigned int *data_p, unsigned int kind);
int bpf_hm_i2c_soft_reset();

int bpf_hm_devc_spi_trx_spi_write_sub6(
                                  unsigned int target_type,
                                  unsigned char ant,
                                  unsigned int dat);

int bpf_hm_devc_spi_trx_spi_write_mmw(
                                  unsigned int target_type,
                                  unsigned int dat);

int bpf_hm_devc_spi_trx_spi_read_mmw(
                                  unsigned int target_type,
                                  unsigned int dat,
                                  unsigned int* r_dat);

int bpf_hm_devc_spi_pl_spi_fandac( unsigned int data);

void bpf_hm_devc_pll_select(unsigned int select);


/********************************************* EOF ******************************************************************/
#endif /* _BPF_I_HM_DEVC_H_ */

