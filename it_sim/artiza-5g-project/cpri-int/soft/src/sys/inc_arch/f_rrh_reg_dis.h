/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_dis.h
 * @brief レジスタアドレス定義ヘッダ(DIS)
 * @date  2015/07/25 TDI) satou     新規追加
 * @date  2015/08/05 TDI) satou     ハードソフト仕様書変更対応(FH1DS1X_DIS_reg.xlsx 版数1.00)
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_DIS_H
#define F_RRH_REG_DIS_H

/*!
 * @name DISレジスタアドレス
 * @note DISレジスタアドレス
 * @{
 */
#define D_RRH_REG_DIS_SYS               0x81020000  /**< AxC LTE帯域幅設定                                */
#define D_RRH_REG_DIS_LDL1              0x81020100  /**< Downlink遅延調整値LTE用 Port 1                   */
#define D_RRH_REG_DIS_LDL2              0x81020104  /**< Downlink遅延調整値LTE用 Port 2                   */
#define D_RRH_REG_DIS_LDL3              0x81020108  /**< Downlink遅延調整値LTE用 Port 3                   */
#define D_RRH_REG_DIS_LDL4              0x8102010C  /**< Downlink遅延調整値LTE用 Port 4                   */
#define D_RRH_REG_DIS_LDL5              0x81020110  /**< Downlink遅延調整値LTE用 Port 5                   */
#define D_RRH_REG_DIS_LDL6              0x81020114  /**< Downlink遅延調整値LTE用 Port 6                   */
#define D_RRH_REG_DIS_LDL7              0x81020118  /**< Downlink遅延調整値LTE用 Port 7                   */
#define D_RRH_REG_DIS_LDL8              0x8102011C  /**< Downlink遅延調整値LTE用 Port 8                   */
#define D_RRH_REG_DIS_LDL9              0x81020120  /**< Downlink遅延調整値LTE用 Port 9                   */
#define D_RRH_REG_DIS_LDL10             0x81020124  /**< Downlink遅延調整値LTE用 Port 10                  */
#define D_RRH_REG_DIS_LDL11             0x81020128  /**< Downlink遅延調整値LTE用 Port 11                  */
#define D_RRH_REG_DIS_LDL12             0x8102012C  /**< Downlink遅延調整値LTE用 Port 12                  */
#define D_RRH_REG_DIS_LDL13             0x81020130  /**< Downlink遅延調整値LTE用 Port 13                  */
#define D_RRH_REG_DIS_LDL14             0x81020134  /**< Downlink遅延調整値LTE用 Port 14                  */
#define D_RRH_REG_DIS_LDL15             0x81020138  /**< Downlink遅延調整値LTE用 Port 15                  */
#define D_RRH_REG_DIS_LDL16             0x8102013C  /**< Downlink遅延調整値LTE用 Port 16                  */
#define D_RRH_REG_DIS_LDISEN            0x81020140  /**< 分配有効Port設定LTE用                            */
#define D_RRH_REG_DIS_LSWAP0            0x81020144  /**< ブランチSWAP有効Port設定LTE Car0用               */
#define D_RRH_REG_DIS_LSWAP1            0x81020148  /**< ブランチSWAP有効Port設定LTE Car1用               */
#define D_RRH_REG_DIS_WDL1              0x81020180  /**< Downlink遅延調整値3G用(DFIL調整値) Port 1        */
#define D_RRH_REG_DIS_WDL2              0x81020184  /**< Downlink遅延調整値3G用(DFIL調整値) Port 2        */
#define D_RRH_REG_DIS_WDL3              0x81020188  /**< Downlink遅延調整値3G用(DFIL調整値) Port 3        */
#define D_RRH_REG_DIS_WDL4              0x8102018C  /**< Downlink遅延調整値3G用(DFIL調整値) Port 4        */
#define D_RRH_REG_DIS_WDL5              0x81020190  /**< Downlink遅延調整値3G用(DFIL調整値) Port 5        */
#define D_RRH_REG_DIS_WDL6              0x81020194  /**< Downlink遅延調整値3G用(DFIL調整値) Port 6        */
#define D_RRH_REG_DIS_WDL7              0x81020198  /**< Downlink遅延調整値3G用(DFIL調整値) Port 7        */
#define D_RRH_REG_DIS_WDL8              0x8102019C  /**< Downlink遅延調整値3G用(DFIL調整値) Port 8        */
#define D_RRH_REG_DIS_WDL9              0x810201A0  /**< Downlink遅延調整値3G用(DFIL調整値) Port 9        */
#define D_RRH_REG_DIS_WDL10             0x810201A4  /**< Downlink遅延調整値3G用(DFIL調整値) Port 10       */
#define D_RRH_REG_DIS_WDL11             0x810201A8  /**< Downlink遅延調整値3G用(DFIL調整値) Port 11       */
#define D_RRH_REG_DIS_WDL12             0x810201AC  /**< Downlink遅延調整値3G用(DFIL調整値) Port 12       */
#define D_RRH_REG_DIS_WDL13             0x810201B0  /**< Downlink遅延調整値3G用(DFIL調整値) Port 13       */
#define D_RRH_REG_DIS_WDL14             0x810201B4  /**< Downlink遅延調整値3G用(DFIL調整値) Port 14       */
#define D_RRH_REG_DIS_WDL15             0x810201B8  /**< Downlink遅延調整値3G用(DFIL調整値) Port 15       */
#define D_RRH_REG_DIS_WDL16             0x810201BC  /**< Downlink遅延調整値3G用(DFIL調整値) Port 16       */
#define D_RRH_REG_DIS_WDISEN            0x810201C0  /**< 分配有効Port設定3G用                             */
#define D_RRH_REG_DIS_WSWAP             0x810201C4  /**< ブランチSWAP有効Port設定3G用                     */
#define D_RRH_REG_DIS_RE1TOFST          0x81020600  /**< 擬似RE Timing Offset Port 1                      */
#define D_RRH_REG_DIS_RE2TOFST          0x81020604  /**< 擬似RE Timing Offset Port 2                      */
#define D_RRH_REG_DIS_RE3TOFST          0x81020608  /**< 擬似RE Timing Offset Port 3                      */
#define D_RRH_REG_DIS_RE4TOFST          0x8102060C  /**< 擬似RE Timing Offset Port 4                      */
#define D_RRH_REG_DIS_RE5TOFST          0x81020610  /**< 擬似RE Timing Offset Port 5                      */
#define D_RRH_REG_DIS_RE6TOFST          0x81020614  /**< 擬似RE Timing Offset Port 6                      */
#define D_RRH_REG_DIS_RE7TOFST          0x81020618  /**< 擬似RE Timing Offset Port 7                      */
#define D_RRH_REG_DIS_RE8TOFST          0x8102061C  /**< 擬似RE Timing Offset Port 8                      */
#define D_RRH_REG_DIS_RE9TOFST          0x81020620  /**< 擬似RE Timing Offset Port 9                      */
#define D_RRH_REG_DIS_RE10TOFST         0x81020624  /**< 擬似RE Timing Offset Port 10                     */
#define D_RRH_REG_DIS_RE11TOFST         0x81020628  /**< 擬似RE Timing Offset Port 11                     */
#define D_RRH_REG_DIS_RE12TOFST         0x8102062C  /**< 擬似RE Timing Offset Port 12                     */
#define D_RRH_REG_DIS_RE13TOFST         0x81020630  /**< 擬似RE Timing Offset Port 13                     */
#define D_RRH_REG_DIS_RE14TOFST         0x81020634  /**< 擬似RE Timing Offset Port 14                     */
#define D_RRH_REG_DIS_RE15TOFST         0x81020638  /**< 擬似RE Timing Offset Port 15                     */
#define D_RRH_REG_DIS_RE16TOFST         0x8102063C  /**< 擬似RE Timing Offset Port 16                     */
#define D_RRH_REG_DIS_RECDTSEL          0x81020640  /**< 擬似RECデータ選択                                */
#define D_RRH_REG_DIS_DISTEST           0x81020700  /**< テストモード(reserve)                            */
#define D_RRH_REG_DIS_DISMON0           0x81020710  /**< テストモニタ(reserve)                            */
#define D_RRH_REG_DIS_DISMONSEL         0x810207FC  /**< モニタ選択                                       */
/** @} */

#define M_RRH_REG_DIS_LDL(cprino) (UINT)(D_RRH_REG_DIS_LDL1 + 0x4*((cprino) - 1))
#define M_RRH_REG_DIS_WDL(cprino) (UINT)(D_RRH_REG_DIS_WDL1 + 0x4*((cprino) - 1))
#endif
/* @} */
