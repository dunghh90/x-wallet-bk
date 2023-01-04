/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   RFIC_PMC.h
 *  @brief  Definitions for RFIC Common library
 *  @date   2015/04/15
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2015-
 */
/********************************************************************************************************************/

#ifndef _RFIC_PMC_H_
#define _RFIC_PMC_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "f_rrh_inc.h"
#include "f_rrh_reg_rfic.h"

/********************************************************************************************************************/
/* External Definition                                                                                              */
/********************************************************************************************************************/
/*------------------------------------------------------------------------------------------------------------------*/
/* 戻り値                                                                                                           */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_PMC_COMPLETE           0x00000000      /**< 正常                                                       */
#define RFIC_PMC_NG                 0x00000001      /**< 異常                                                       */
#define RFIC_PMC_NG_UNITX_AB_BOOT   0x00000101      /**< UniTx#AB Firm Boot失敗                                     */
#define RFIC_PMC_NG_UNITX_CD_BOOT   0x00000102      /**< UniTx#CD Firm Boot失敗                                     */
#define RFIC_PMC_NG_UNIRX_AB_BOOT   0x00000103      /**< UniTx#AB Firm Boot失敗                                     */
#define RFIC_PMC_NG_UNIRX_CD_BOOT   0x00000104      /**< UniTx#CD Firm Boot失敗                                     */
#define RFIC_PMC_NG_UNITX_CONFIG    0x00000105      /**< UniTx JESD Digital Config失敗                              */
#define RFIC_PMC_NG_UNIRX_CONFIG    0x00000106      /**< UniRx JESD Digital Config失敗                              */
#define RFIC_PMC_NG_JSFW_SYSREF     0x00000201      /**< JSFW 外部SYSREF検出確認NG                                  */
#define RFIC_PMC_NG_JSFW_PHYDONE    0x00000202      /**< JSFW PHYDONE確認タイムアウト                               */
#define RFIC_PMC_NG_JSFW_PHY        0x00000203      /**< JSFW PHY Status確認リトライアウト                          */
#define RFIC_PMC_NG_JSFW_ERR        0x00000204      /**< JSFWエラー確認NG                                           */
#define RFIC_PMC_NG_JSFW_QPLL_LOCK  0x00000205      /**< JESD QPLL Lock確認NG                                       */
#define RFIC_PMC_NG_JSFB_PHYDONE    0x00000301      /**< JSFB PHYDONE確認タイムアウト                               */
#define RFIC_PMC_NG_JSFB_PHY        0x00000302      /**< JSFB PHY Status確認リトライアウト                          */
#define RFIC_PMC_NG_JSFB_FPGA_ERR   0x00000303      /**< JSFB FPGA Error Status確認リトライアウト                   */
#define RFIC_PMC_NG_JSFB_ERR        0x00000304      /**< JSFB エラー確認NG                                          */
#define RFIC_PMC_NG_JSRX_PHYDONE    0x00000401      /**< JSRX PHYDONE確認タイムアウト                               */
#define RFIC_PMC_NG_JSRX_PHY        0x00000402      /**< JSRX PHY Status確認リトライアウト                          */
#define RFIC_PMC_NG_JSRX_FPGA_ERR   0x00000403      /**< JSRX FPGA Error Status確認リトライアウト                   */
#define RFIC_PMC_NG_JSRX_ERR        0x00000404      /**< JSRX エラー確認NG                                          */
#define RFIC_PMC_NG_CMD_RSP         0x00000501      /**< 「Mailbox Messaging Operation」でNG応答受信                */
#define RFIC_PMC_NG_LINKUP          0x00000600      /**< JESD LINKUP失敗 下位4bitでFW/FB/RXいずれかを表す           */
#define RFIC_PMC_NG_LINKUP_FW       0x00000601      /**< JESD LINKUP失敗(FW)                                        */
#define RFIC_PMC_NG_LINKUP_FB       0x00000602      /**< JESD LINKUP失敗(FB)                                        */
#define RFIC_PMC_NG_LINKUP_FW_FB    0x00000603      /**< JESD LINKUP失敗(FW, FB)                                    */
#define RFIC_PMC_NG_LINKUP_RX       0x00000604      /**< JESD LINKUP失敗(RX)                                        */
#define RFIC_PMC_NG_LINKUP_FW_RX    0x00000605      /**< JESD LINKUP失敗(FW, RX)                                    */
#define RFIC_PMC_NG_LINKUP_FB_RX    0x00000606      /**< JESD LINKUP失敗(FB, RX)                                    */
#define RFIC_PMC_NG_LINKUP_ALL      0x00000607      /**< JESD LINKUP失敗(FW, FB, RX)                                */

/*------------------------------------------------------------------------------------------------------------------*/
/* RFIC種別                                                                                                         */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_PMC_TX         (0)                     /**< Tx/Rx種別 : TX                                             */
#define RFIC_PMC_RX         (1)                     /**< Tx/Rx種別 : RX                                             */
#define RFIC_PMC_TXRX_MAX   (RFIC_PMC_RX)           /**< Tx/Rx種別 : 最大値                                         */

#define RFIC_PMC_UNIT0      (0)                     /**< ユニット番号 : #0                                          */
#define RFIC_PMC_UNIT1      (1)                     /**< ユニット番号 : #1                                          */
#define RFIC_PMC_UNIT_MIN   (RFIC_PMC_UNIT0)        /**< ユニット番号 : 最小値                                      */
#define RFIC_PMC_UNIT_MAX   (RFIC_PMC_UNIT1)        /**< ユニット番号 : 最大値                                      */

/*------------------------------------------------------------------------------------------------------------------*/
/* Mailbox Massaging Operation                                                                                      */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_PMC_MAILBOX_RSP_NUM (6)                /**< MAILBOX_RESPのレジスタ数                                   */

/*------------------------------------------------------------------------------------------------------------------*/
/* JESD種別                                                                                                         */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_PMC_JESD_FW    (0)                     /**< JESD FW                                                    */
#define RFIC_PMC_JESD_FB    (1)                     /**< JESD FB                                                    */
#define RFIC_PMC_JESD_RX    (2)                     /**< JESD RX                                                    */

/*------------------------------------------------------------------------------------------------------------------*/
/* Function Prototype                                                                                               */
/*------------------------------------------------------------------------------------------------------------------*/
int RFIC_PMC_DCM_PLLIC_SET(void);
int RFIC_PMC_DCM_BOOT(uint8_t *tx_img1, uint8_t *tx_img2, uint8_t *rx_img1, uint8_t *rx_img2);
int RFIC_PMC_DCM_UniTX_SET(uint8_t *trainingTx);
int RFIC_PMC_DCM_UniRX_SET(uint8_t *config, uint8_t *antParam, uint8_t *trainingRx);
int RFIC_PMC_DCM_JESD_BOOT(void);
int RFIC_PMC_DISABLE_MASK_JESD(void);
int RFIC_PMC_SEND_MAILBOX(UINT txRx, UINT unit, uint16_t cmdId, uint64_t paramFull);
int RFIC_PMC_SEND_MAILBOX_RSP(UINT txRx, UINT unit, uint16_t cmdId, uint64_t paramFull, UINT rsp[RFIC_PMC_MAILBOX_RSP_NUM]);
int RFIC_PMC_DMA_WRITE(UINT txRx, UINT unit, uint64_t paramFull);
int RFIC_PMC_MIO3_MIO5_START(void);
int RFIC_PMC_UNIRX_ADC_CLIP_SET(void);
int RFIC_PMC_INIT(UINT reKind);

/********************************************* EOF ******************************************************************/
#endif /* _RFIC_PMC_H_ */
