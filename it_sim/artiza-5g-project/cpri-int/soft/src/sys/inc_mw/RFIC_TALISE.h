/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   RFIC_TALISE.h
 *  @brief  Definitions for RFIC Common library
 *  @date   2015/04/15
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2015-
 */
/********************************************************************************************************************/

#ifndef _RFIC_TALISE_H_
#define _RFIC_TALISE_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "f_rrh_inc.h"
//#include "f_rrh_reg_rfic.h"

/********************************************************************************************************************/
/* External Definition                                                                                              */
/********************************************************************************************************************/
/*------------------------------------------------------------------------------------------------------------------*/
/* 戻り値                                                                                                           */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_TALISE_COMPLETE           0x00000000      /**< 正常                                                    */
#define RFIC_TALISE_NG                 0x00000001      /**< 異常                                                    */
#define RFIC_TALISE_PARAM_ERROR        0x00000002      /**< パラメータエラー                                        */
#define RFIC_TALISE_INIT_ERROR         0x00000100      /**< 初期化処理失敗                                          */
#define RFIC_TALISE_POWON_ERROR        0x00000101      /**< パワーオン失敗(Wrt)                                     */
#if 0
#define RFIC_TALISE_JS1PHYCNT          0x00000102      /**< JS1PHYCNT(Wrt)失敗                                      */
#define RFIC_TALISE_JS2PHYCNT          0x00000103      /**< JS2PHYCNT(Wrt)失敗                                      */
#define RFIC_TALISE_JS3PHYCNT          0x00000104      /**< JS3PHYCNT(Wrt)失敗                                      */
#define RFIC_TALISE_JS4PHYCNT          0x00000105      /**< JS4PHYCNT(Wrt)失敗                                      */
#endif
#define RFIC_TALISE_JSFW_R             0x00000106      /**< JSFW(Read)失敗                                          */
#define RFIC_TALISE_JSFB_R             0x00000107      /**< JSFW(Read)失敗                                          */
#define RFIC_TALISE_JSRXLMFC_R         0x00000108      /**< JSRXLMFC(Read)失敗                                      */
#define RFIC_TALISE_QPLL_R             0x00000109      /**< QPLL(Read)失敗                                          */
#define RFIC_TALISE_FPGAMASK_R         0x0000010A      /**< FPGAMASK(Read)失敗                                      */
#define RFIC_TALISE_QPLL_RST_R         0x0000010B      /**< QPLLリセット(Read)失敗                                  */
#define RFIC_TALISE_QPLL_LOCK          0x0000010C      /**< QPLLロック確認(Read)失敗                                */
#define RFIC_TALISE_PHY_RST_R          0x0000010D      /**< PHYリセット(Read)失敗                                   */
#define RFIC_TALISE_SERDES_CTX         0x0000010E      /**< FPGA SERDES DONE(CTX)確認失敗                           */
#define RFIC_TALISE_SERDES_JSFW        0x0000010F      /**< FPGA SERDES DONE(JSFW)確認失敗                          */
#define RFIC_TALISE_RFIC_RST_R         0x00000110      /**< RFICリセット(Read)確認失敗                              */
#define RFIC_TALISE_JSFW_W             0x00000186      /**< JSFW(Wrt)失敗                                           */
#define RFIC_TALISE_JSFB_W             0x00000187      /**< JSFW(Wrt)失敗                                           */
#define RFIC_TALISE_JSRXLMFC_W         0x00000188      /**< JSRXLMFC(Wrt)失敗                                       */
#define RFIC_TALISE_QPLL_W             0x00000189      /**< QPLL(Wrt)失敗                                           */
#define RFIC_TALISE_FPGAMASK_W         0x0000018A      /**< FPGAMASK(Wrt)失敗                                       */
#define RFIC_TALISE_QPLL_RST_W         0x0000018B      /**< QPLLリセット(Wrt)失敗                                   */
#define RFIC_TALISE_PHY_RST_W          0x0000018D      /**< PHYリセット(Wrt)失敗                                    */
#define RFIC_TALISE_QPLL_LOCK_E        0x0000018C      /**< QPLLロック確認エラー                                    */
#define RFIC_TALISE_SERDES_CTX_E       0x0000018E      /**< FPGA SERDES DONE(CTX)確認エラー                         */
#define RFIC_TALISE_SERDES_JSFW_E      0x0000018F      /**< FPGA SERDES DONE(JSFW)確認エラー                        */
#define RFIC_TALISE_RFIC_RST_W         0x00000190      /**< RFICリセット(Wrt)確認失敗                               */
// 追加分 2018/8/8 KCN)hfuku start
#define RFIC_TALISE_JS1PHYCNT0         0x000001A0      /**< JS1PHYCNT0(Wrt)失敗                                      */
#define RFIC_TALISE_JS1PHYCNT1         0x000001A1      /**< JS1PHYCNT1(Wrt)失敗                                      */
#define RFIC_TALISE_JS1PHYCNT2         0x000001A2      /**< JS1PHYCNT2(Wrt)失敗                                      */
#define RFIC_TALISE_JS1PHYCNT3         0x000001A3      /**< JS1PHYCNT3(Wrt)失敗                                      */
#define RFIC_TALISE_JS2PHYCNT0         0x000001A4      /**< JS2PHYCNT0(Wrt)失敗                                      */
#define RFIC_TALISE_JS2PHYCNT1         0x000001A5      /**< JS2PHYCNT1(Wrt)失敗                                      */
#define RFIC_TALISE_JS2PHYCNT2         0x000001A6      /**< JS2PHYCNT2(Wrt)失敗                                      */
#define RFIC_TALISE_JS2PHYCNT3         0x000001A7      /**< JS2PHYCNT3(Wrt)失敗                                      */
#define RFIC_TALISE_GTY_W              0x000001B0      /**< GTY(Wrt)失敗                                            */
// 追加分 2018/8/8 KCN)hfuku end
#define RFIC_TALISE_UNIT_ERR           0x00000200      /**< RFIC番号エラー                                          */
#define RFIC_TALISE_4WIRE_ERR          0x00000201      /**< Talise 4wireモード設定エラー                            */
#define RFIC_TALISE_OPENHW_ERR         0x00000202      /**< Talise Open HW設定エラー                                */
#define RFIC_TALISE_RESET_ERR          0x00000203      /**< Talise リセット解除エラー                               */
#define RFIC_TALISE_XBAR_ERR           0x00000204      /**< ADC Sample Xbar設定エラー                               */
#define RFIC_TALISE_CLKPLL_E           0x00000205      /**< Talise CLKPLLステータス確認エラー                       */
#define RFIC_TALISE_CLKPLL_STSE        0x00000206      /**< Talise CLKPLLステータスエラー                           */
#define RFIC_TALISE_SETMCS_E           0x00000207      /**< Talise MCS設定エラー                                    */
#define RFIC_TALISE_SETMCS1_STSE       0x00000210      /**< Talise MCS設定エラー1                                   */
#define RFIC_TALISE_SETMCS2_STSE       0x00000211      /**< Talise MCS設定エラー2                                   */
#define RFIC_TALISE_ARMSUM_E           0x00000220      /**< Talise verifyArmChecksumエラー                          */
#define RFIC_TALISE_RFPLL_STSE         0x00000221      /**< Talise RFPLLステータスエラー                            */
#define RFIC_TALISE_ARM_STSE           0x00000222      /**< Talise verifyArmChecksumエラー                          */
#define RFIC_TALISE_TXCAL_STSE         0x00000240      /**< Talise TXCALステータスエラー                            */
#define RFIC_TALISE_RXCAL_STSE         0x00000241      /**< Talise RXCALステータスエラー                            */

#define RFIC_PLL_SYSREF_R              0x00000300      /**< PLL-IC SYSREF発行(Read)確認エラー                       */
#define RFIC_PLL_SYSREF_W              0x00000380      /**< PLL-IC SYSREF発行(Wrt)確認エラー                        */

#define RFIC_TALISE_JSRX_D             0x00000600      /**< Talise JSRXディセーブルエラー                           */
#define RFIC_TALISE_JSFB_D             0x00000602      /**< Talise JSFBディセーブルエラー                           */
#define RFIC_TALISE_JSFW_D             0x00000604      /**< Talise JSFWディセーブルエラー                           */
#define RFIC_TALISE_JSFB_PYYR          0x00000606      /**< JSFB PYYリセット(Read)確認エラー                        */
#define RFIC_TALISE_JSRX_PYYR          0x00000607      /**< JSRX PYYリセット(Read)確認エラー                        */
#define RFIC_TALISE_PHY1_STS           0x00000608      /**< FPGA PHY1 Status確認エラー                              */
#define RFIC_TALISE_PHY2_STS           0x00000609      /**< FPGA PHY2 Status確認エラー                              */
#define RFIC_TALISE_PHY3_STS           0x0000060A      /**< FPGA PHY3 Status確認エラー                              */
#define RFIC_TALISE_PHY4_STS           0x0000060B      /**< FPGA PHY4 Status確認エラー                              */
#define RFIC_TALISE_SYSREF_MSK         0x0000060C      /**< FPGA 外部SYSREFマスク解除エラー                         */
#define RFIC_TALISE_JSRX_E             0x00000680      /**< Talise JSRXイネーブルエラー                             */
#define RFIC_TALISE_JSRX_SYSREFE       0x00000681      /**< Talise JSRX SYSREFイネーブルエラー                      */
#define RFIC_TALISE_JSFB_E             0x00000682      /**< Talise JSFBイネーブルエラー                             */
#define RFIC_TALISE_JSFB_SYSREFE       0x00000683      /**< Talise JSFB SYSREFイネーブルエラー                      */
#define RFIC_TALISE_JSFW_E             0x00000684      /**< Talise JSFWイネーブルエラー                             */
#define RFIC_TALISE_JSFW_SYSREFE       0x00000685      /**< Talise JSFW SYSREFイネーブルエラー                      */
#define RFIC_TALISE_JSFB_PYYW          0x00000686      /**< JSFB PYYリセット(Wrt)確認エラー                         */
#define RFIC_TALISE_JSRX_PYYW          0x00000687      /**< JSRX PYYリセット(Wrt)確認エラー                         */
#define RFIC_TALISE_PHY1_STS_E         0x00000688      /**< FPGA PHY1 Statusエラー                                  */
#define RFIC_TALISE_PHY2_STS_E         0x00000689      /**< FPGA PHY2 Statusエラー                                  */
#define RFIC_TALISE_PHY3_STS_E         0x0000068A      /**< FPGA PHY3 Statusエラー                                  */
#define RFIC_TALISE_PHY4_STS_E         0x0000068B      /**< FPGA PHY4 Statusエラー                                  */
#define RFIC_TALISE_MCS                0x00000700      /**< Talise MCS確認エラー                                    */
#define RFIC_TALISE_SYSREF             0x00000701      /**< FPGA 外部SYSREF検出確認エラー                           */
#define RFIC_TALISE_SYSREF_MSK_R       0x00000702      /**< FPGA 外部SYSREFマスク(Read)確認エラー                   */
#define RFIC_TALISE_SYSREF_JSFB_R      0x00000703      /**< FPGA JSFB内部SYSREFマスク(Read)確認エラー               */
#define RFIC_TALISE_SYSREF_JSRX_R      0x00000704      /**< FPGA JSRX内部SYSREFマスク(Read)確認エラー               */
#define RFIC_TALISE_JSFB_SYNC          0x00000705      /**< FPGA JSFB SYNC確認エラー                                */
#define RFIC_TALISE_JSRX_SYNC          0x00000706      /**< FPGA JSRX SYNC確認エラー                                */
#define RFIC_TALISE_JSFB_ESTS          0x00000707      /**< JSFB Error Status確認エラー                             */
#define RFIC_TALISE_JSRX_ESTS          0x00000708      /**< JSFB Error Status確認エラー                             */
#define RFIC_TALISE_JSFB_ERR           0x00000709      /**< JSFB Error 確認エラー                                   */
#define RFIC_TALISE_JSRX_ERR           0x0000070A      /**< JSFB Error 確認エラー                                   */
#define RFIC_TALISE_MCS_E              0x00000780      /**< Talise MCSエラー                                        */
#define RFIC_TALISE_SYSREF_E           0x00000781      /**< FPGA 外部SYSREF検出エラー                               */
#define RFIC_TALISE_SYSREF_MSK_W       0x00000782      /**< FPGA 外部SYSREFマスク(Wrt)確認エラー                    */
#define RFIC_TALISE_SYSREF_JSFB_W      0x00000783      /**< FPGA JSFB内部SYSREFマスク(Wrt)確認エラー                */
#define RFIC_TALISE_SYSREF_JSRX_W      0x00000784      /**< FPGA JSRX内部SYSREFマスク(Wrt)確認エラー                */
#define RFIC_TALISE_JSFB_SYNC_E        0x00000785      /**< FPGA JSFB SYNCエラー                                    */
#define RFIC_TALISE_JSRX_SYNC_E        0x00000786      /**< FPGA JSRX SYNCエラー                                    */
#define RFIC_TALISE_JSFB_ESTS_E        0x00000787      /**< JSFB Error Statusエラー                                 */
#define RFIC_TALISE_JSRX_ESTS_E        0x00000788      /**< JSFB Error Statusエラー                                 */
#define RFIC_TALISE_JSFB_ERR_E         0x00000789      /**< JSFB Error エラー                                       */
#define RFIC_TALISE_JSRX_ERR_E         0x0000078A      /**< JSFB Error エラー                                       */
#define RFIC_TALISE_FPGA_LNK           0x0000070B      /**< FPGA Link確認エラー                                     */
#define RFIC_TALISE_FPGA_LNK_E         0x0000078B      /**< FPGA Link確認エラー                                     */
#define RFIC_TALISE_FPGA_ERR           0x0000070C      /**< FPGA エラー                                             */
#define RFIC_TALISE_FPGA_ERR_E         0x0000078C      /**< FPGA エラー                                             */
#define RFIC_TALISE_JSFW_STSA          0x0000070D      /**< Talise JSFW ステータスチェック-Aエラー                  */
#define RFIC_TALISE_JSRX_STSA          0x0000070E      /**< Talise JSFW ステータスチェック-Bエラー                  */
#define RFIC_TALISE_JSFW_STSB          0x0000070F      /**< Talise JSRX ステータスチェック-Aエラー                  */
#define RFIC_TALISE_GPIO_INIT_E        0x00000710      /**< Talise GPIO設定エラー                                   */
#define RFIC_TALISE_GPIO_E             0x00000711      /**< Talise GPIO設定(設定値は要確認★)エラー                 */
#define RFIC_TALISE_RADIOON_E          0x00000712      /**< Talise 送信設定エラー                                   */

/*------------------------------------------------------------------------------------------------------------------*/
/* RFIC種別                                                                                                         */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_TALISE_UNIT0      (0)                  /**< ユニット番号 : #0                                          */
#define RFIC_TALISE_UNIT1      (1)                  /**< ユニット番号 : #1                                          */
#define RFIC_TALISE_UNIT2      (2)                  /**< ユニット番号 : #2                                          */
#define RFIC_TALISE_UNIT3      (3)                  /**< ユニット番号 : #3                                          */
#define RFIC_TALISE_UNIT_MIN   (RFIC_TALISE_UNIT0)  /**< ユニット番号 : 最小値                                      */
#define RFIC_TALISE_UNIT_MAX   (RFIC_TALISE_UNIT1)  /**< ユニット番号 : 最大値                                      */
#define RFIC_TALISE_UNIT_ALL   (0xFF)               /**< ユニット番号 : 全選択                                      */
/*------------------------------------------------------------------------------------------------------------------*/
/* デバイスID                                                                                                       */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_TALISE_CLK_PLL    (0)                  /*!< Selects CLK PLL for Rx and Tx                              */
#define RFIC_TALISE_RF_PLL     (1)                  /*!< Selects RF PLL for Rx and Tx                               */
#define RFIC_TALISE_AUX_PLL    (2)                  /*!< Selects AUX PLL for Rx and tx                              */
/*------------------------------------------------------------------------------------------------------------------*/
/* フレーマー                                                                                                       */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_TALISE_FRAMER_A           (0)          /*!< Framer A selection                                         */
#define RFIC_TALISE_FRAMER_B           (1)          /*!< Framer B selection                                         */
#define RFIC_TALISE_FRAMER_A_AND_B     (2)          /*!< Used for cases where Rx1 uses one framer, Rx2 uses the second framer */
/*------------------------------------------------------------------------------------------------------------------*/
/* デフレーマー                                                                                                     */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_TALISE_DEFRAMER_A         (0)          /*!< Deframer A selection                                       */
#define RFIC_TALISE_DEFRAMER_B         (1)          /*!< Deframer B selection                                       */
#define RFIC_TALISE_DEFRAMER_A_AND_B   (2)          /*!< Used for cases where Tx1 uses one deframer, Tx2 uses the second deframer */
/*------------------------------------------------------------------------------------------------------------------*/
/* TX設定経路情報                                                                                                   */
/*------------------------------------------------------------------------------------------------------------------*/
#define RFIC_TALISE_TX1                (1)          /* TX設定経路情報　TX1                                          */
#define RFIC_TALISE_TX2                (2)          /* TX設定経路情報　TX2                                          */
/*------------------------------------------------------------------------------------------------------------------*/
/* Function Prototype                                                                                               */
/*------------------------------------------------------------------------------------------------------------------*/
/* Talise API */
int RFIC_TALISE_OPENHW(uint8_t unit);
int RFIC_TALISE_RESETDEVICE(uint8_t unit);
int RFIC_TALISE_INITIALIZE(uint8_t unit);
int RFIC_TALISE_GETPLLSLOCKSTATUS(uint8_t unit, uint8_t *status);
int RFIC_TALISE_ENABLEMULTICHIPSYNC(uint8_t unit, uint8_t enable);
int RFIC_TALISE_INITARM(uint8_t unit);
int RFIC_TALISE_LOADSTREAM(uint8_t unit);
int RFIC_TALISE_LOADARM(uint8_t unit);
int RFIC_TALISE_VERIFYARMCHECKSUM(uint8_t unit);
int RFIC_TALISE_SETRFPLLFREQUENCY(uint8_t unit, unsigned long dev_id,unsigned long freq_khz);
int RFIC_TALISE_RUNINITCALS(uint8_t unit, unsigned long cal_mask);
int RFIC_TALISE_WAITINITCALS(uint8_t unit, unsigned long timeoutMs, uint8_t *status);
int RFIC_TALISE_ENABLEFRAMERLINK(uint8_t unit, unsigned long framerSel ,unsigned long enable);
int RFIC_TALISE_ENABLESYSREFTOFRAMER(uint8_t unit, unsigned long framerSel, unsigned long enable);
int RFIC_TALISE_ENABLEDEFRAMERLINK(uint8_t unit, unsigned long deframerSel, unsigned long enable);
int RFIC_TALISE_ENABLESYSREFTODEFRAMER(uint8_t unit, unsigned long deframerSel, unsigned long enable);
int RFIC_TALISE_READDEFRAMERSTATUS(uint8_t unit, unsigned long deframerSel);
int RFIC_TALISE_READFRAMERSTATUS(uint8_t unit, unsigned long framerSel);
int RFIC_TALISE_ENABLETRACKINGCALS(uint8_t unit, unsigned long cal_mask);
int RFIC_TALISE_RADIOON(uint8_t unit);
int RFIC_TALISE_RADIOOFF(uint8_t unit);
int RFIC_TALISE_SHUTDOWN(uint8_t unit);
int RFIC_TALISE_CLOSEHW(uint8_t unit);
int RFIC_TALISE_SETUPAUXDACS(uint8_t unit);
int RFIC_TALISE_WRITEAUXDAC(uint8_t unit, unsigned long auxDacIndex, unsigned long auxDacCode);
int RFIC_TALISE_GETTEMPERATURE(uint8_t unit);
int RFIC_TALISE_SETTXATTENUATION(uint8_t unit, unsigned long txChannel, unsigned long txAttenuation_mdB);
int RFIC_TALISE_GETTXATTENUATION(uint8_t unit, unsigned long txChannel);
int RFIC_TALISE_SETTXTOORXMAPPING(uint8_t unit, unsigned long txCalEnable, unsigned long oRx1Map, unsigned long oRx2Map);
int RFIC_TALISE_SETARMGPIOPINS(uint8_t unit);
int RFIC_TALISE_SETRADIOCTLPINMODE(uint8_t unit, unsigned long pinOptionsMask, unsigned long orxEnGpioPinSel);
int RFIC_TALISE_SETRXMANUALGAIN(uint8_t unit, unsigned long rxChannel, unsigned long gainIndex);
int RFIC_TALISE_SETOBSRXMANUALGAIN(uint8_t unit, unsigned long obsRxCh, unsigned long gainIndex);
int RFIC_TALISE_SETRXTXENABLE(uint8_t unit, unsigned long rxOrxChannel, unsigned long txChannel);
int RFIC_TALISE_GETRXTXENABLE(uint8_t unit);
int RFIC_TALISE_GETRADIOSTATE(uint8_t unit);
int RFIC_TALISE_GETINITCALSTATUS(uint8_t unit);
int RFIC_TALISE_CHECKINITCALCOMPLETE(uint8_t unit);
int RFIC_TALISE_ENABLEFRAMERTESTDATA(uint8_t unit, unsigned long framerSelect, unsigned long testDataSource, unsigned long injectPoint);
int RFIC_TALISE_SETUPADCSAMPLEXBAR(uint8_t unit, unsigned int framerSel, unsigned int *conv);
int RFIC_TALISE_GETERRCODE(uint8_t unit);
int RFIC_TALISE_RESCHEDULETRACKINGCAL(uint8_t unit, unsigned long trackingCal);
int RFIC_TALISE_ENABLEDEFRAMERPRBSCHECKER(uint8_t unit, unsigned long polyOrder, unsigned long checkerLocation);
int RFIC_TALISE_READDEFRAMERPRBSCOUNTERS(uint8_t unit, unsigned long lane);
int RFIC_TALISE_SETRFPLLLOOPFILTER(uint8_t unit, unsigned long loopBandwidth_kHz, unsigned long stability);
int RFIC_TALISE_GETRFPLLLOOPFILTER(uint8_t unit);

/* Talise START */
int RFIC_TAL_FPGA_INITIALIZE(uint8_t unit);
int RFIC_TAL_RFIC_INITIALIZE(uint8_t unit);
int RFIC_TAL_PLLIC_MCS(uint8_t unit);
int RFIC_TAL_ARM_START(uint8_t unit);
int RFIC_TAL_RFIC_CAL(uint8_t unit);
int RFIC_TAL_JESD_SET(uint8_t unit);
int RFIC_TAL_PLLIC_JESD(uint8_t unit);
int RFIC_TAL_SENDRCV_SET(uint8_t unit);
int RFIC_TAL_START(void);	// 追加 2018/8/9 KCN)hfuku
int RFIC_TAL_GETTXQECSTATUS(uint8_t unit , int channelSel ,int status_len , void* status );


/********************************************* EOF ******************************************************************/
#endif /* _RFIC_TALISE_H_ */
