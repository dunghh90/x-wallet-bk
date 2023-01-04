/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   RFIC_mmW_API.h
 *  @brief  Definitions for RFIC mmW library
 *  @date   2018/08/14
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2018-
 */
/********************************************************************************************************************/

#ifndef _RFIC_MMW_API_H_
#define _RFIC_MMW_API_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include "BPF_HM_DEVC.h"
#include "BPF_COM_PLOG.h"
#include "BPF_COM_LOG.h"
#include "f_du_reg_eep.h"
#include "f_du_reg_rfeep.h"
#include "f_rrh_reg_flash.h"
#include "BPF_RU_FACC.h"

/********************************************************************************************************************/
/* External Definition                                                                                              */
/********************************************************************************************************************/
/*------------------------------------------------------------------------------------------------------------------*/
/* 戻り値                                                                                                           */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_MMW_COMPLETE				0 //正常終了
#define RFIC_MMW_NG						1 //異常終了
#define RFIC_MMW_RETRY_10				2 //リトライ(10回まで)
#define RFIC_MMW_RETRY_3				3 //リトライ(3回まで)

/*------------------------------------------------------------------------------------------------------------------*/
/* モード種別                                                                                                       */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_MMW_SET_TRANING_DATA		0 //通常(トレーニングデータ設定)
#define RFIC_MMW_SET_DEFAULT_DATA		1 //Default初期値設定
#define RFIC_MMW_NORMAL_POWER_MODE		0 //通常電力モード
#define RFIC_MMW_LOW_POWER_MODE			1 //低電力モード
#define RFIC_MMW_DEVICE_IDLE			0 //idle

/*------------------------------------------------------------------------------------------------------------------*/
/* PGC BIAS                                                                                                         */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_MMW_PGC_BIAS_DATA_NUM_GRP		8
#define RFIC_MMW_PGC_BIAS_DATA_NUM_PTN1		11
#define RFIC_MMW_PGC_BIAS_DATA_NUM_PTN2		3

/*------------------------------------------------------------------------------------------------------------------*/
/* RF_PLL                                                                                                           */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_MMW_RF_PLL_DATA_NUM		116

/*------------------------------------------------------------------------------------------------------------------*/
/* PGC LUT                                                                                                          */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_MMW_LUT_TRANING_DATA_NUM	0x2D00	/* 0x5A00を2で割った値 */
#define RFIC_MMW_LUT_TRANING_DATA_NUM_23BEAM	0x200	/* BEAM ID=23のTX/RXトレーニングデータのバイト数 */
#define RFIC_MMW_LUT_PHASE_OFFSET_TABLE_PATH    "/opt/pf/rfp/cca/"                /* 位相オフセットテーブルのパス名     */
#define RFIC_MMW_LUT_PHASE_OFFSET_TABLE_NAME    "pf_cca_PhaseOffsetTable.bin"     /* 位相オフセットテーブルのファイル名 */

/*------------------------------------------------------------------------------------------------------------------*/
/* HAL                                                                                                              */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA		0x00000000
#define RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB		0x00000001
#define RFIC_MMW_DEVC_TRXSPI_SEL0				0x00000000
#define RFIC_MMW_DEVC_TRXSPI_SEL1				0x00000001
#define RFIC_MMW_DEVC_TRXSPI_SEL2				0x00000002
#define RFIC_MMW_DEVC_TRXSPI_SEL3				0x00000003


/*------------------------------------------------------------------------------------------------------------------*/
/* EEPROM / FLASH                                                                                                   */
/*------------------------------------------------------------------------------------------------------------------*/
//RFEEPトレーニングデータ部 RFEEPと共有メモリのオフセット差分
#define RFIC_MMW_OFFSET_RFEEP_SHRMEM			0x400

//FLASH バージョン情報のサイズ
#define RFIC_MMW_OFFSET_FLASH_VER_INFO			0x00000008


/*------------------------------------------------------------------------------------------------------------------*/
/* FPGA                                                                                                             */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_MMW_TRX_FPGA1					0
#define RFIC_MMW_TRX_FPGA2					1
#define RFIC_MMW_TRX_FPGA_BUSY_CHK_BIT		0x00020000
#define RFIC_MMW_TRX_FPGA_GEN_PORT_ADDR		0x00010700
#define RFIC_MMW_TRX_FPGA_GEN_CLK_ADDR		0x00010704
#define RFIC_MMW_TRX_FPGA_GEN_ENA_ADDR		0x00010708
#define RFIC_MMW_TRX_FPGA_GEN_WRITE_ADDR	0x00010710
#define RFIC_MMW_JC_PLL_SYSREF_OFF			0
#define RFIC_MMW_JC_PLL_SYSREF_ON			1
#define RFIC_MMW_JC_PLL_REG_ADDR			0x0139
#define RFIC_MMW_BASE_ADDR_CTX_FPGA1		0xB0000000
#define RFIC_MMW_BASE_ADDR_JSFW_FPGA1		0xB1200000	//先頭アドレスは0xB1204000
#define RFIC_MMW_BASE_ADDR_JSRX_FPGA1		0xB1300000	//先頭アドレスは0xB1301000
#define RFIC_MMW_BASE_ADDR_CTX_FPGA2		0xB2000000
#define RFIC_MMW_BASE_ADDR_JSFW_FPGA2		0xB3200000	//先頭アドレスは0xB3204000
#define RFIC_MMW_BASE_ADDR_JSRX_FPGA2		0xB3300000	//先頭アドレスは0xB3301000
#define RFIC_MMW_FLG_RETRY					1
#define RFIC_MMW_FLG_CLEAR					0

/*------------------------------------------------------------------------------------------------------------------*/
/* Function Prototype                                                                                               */
/*------------------------------------------------------------------------------------------------------------------*/
int RFIC_mmW_RF_PLL(void);
int RFIC_mmW_RF_AMP_DAC(int def_flg, int pw_mode);
int RFIC_mmW_PGC_BIAS(void);
int RFIC_mmW_TX_VATT_DAC(int def_flg, int pw_mode);
int RFIC_mmW_RX_VATT_DAC(int def_flg, int pw_mode);
int RFIC_mmW_QMOD_QDEM_DAC(int def_flg, int pw_mode);
int RFIC_mmW_QMOD(int def_flg, int pw_mode);
int RFIC_mmW_QDEM(int def_flg, int pw_mode);
int RFIC_mmW_TX_DAC(void);
int RFIC_mmW_RX_ADC(void);
int RFIC_mmW_JC_PLL(int on_off);
int f_dpda_mmW_setTxDacSerdesParameter(unsigned int fpga_no);
int f_dpda_mmW_setTxDacJesdQpllJsfwPhy(unsigned int fpga_no);
int f_dpda_mmW_retryTxDacJesdQpllJsfwPhy(unsigned int fpga_no);
int f_dpda_mmW_setTxDac(void);
int f_dpda_mmW_setTxDacJesd(unsigned int fpga_no);
int f_dpda_mmW_setRxAdc(void);
int f_dpda_mmW_setRxAdcJesd(unsigned int fpga_no);
int f_dpda_mmW_checkRxAdcIlas(unsigned int fpga_no);
int RFIC_mmW_START(void);
unsigned int f_dpda_mmW_setTxDacLinkPage(unsigned int pageno);
int RFIC_mmW_PGC_LUT(void);


/********************************************* EOF ******************************************************************/
#endif /* _RFIC_MMW_API_H_ */
