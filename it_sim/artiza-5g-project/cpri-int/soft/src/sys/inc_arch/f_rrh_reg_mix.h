/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_mix.h
 * @brief レジスタアドレス定義ヘッダ(MIX)
 * @date  2015/07/25 TDI) satou     新規追加
 * @date  2015/08/05 TDI) satou     ハードソフト仕様書変更対応(FH1DS1X_MIX_reg.xlsx 版数1.00)
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_MIX_H
#define F_RRH_REG_MIX_H

/*!
 * @name MIXレジスタアドレス
 * @note MIXレジスタアドレス
 * @{
 */
#define D_RRH_REG_MIX_SYSM              0x81020000  /**< AxC LTE帯域幅設定(ミラー)                        */
#define D_RRH_REG_MIX_FHMTOFST          0x81020880  /**< FHM_TOFFSET設定                                  */
#define D_RRH_REG_MIX_LUL1              0x81020900  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 1         */
#define D_RRH_REG_MIX_LUL2              0x81020904  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 2         */
#define D_RRH_REG_MIX_LUL3              0x81020908  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 3         */
#define D_RRH_REG_MIX_LUL4              0x8102090C  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 4         */
#define D_RRH_REG_MIX_LUL5              0x81020910  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 5         */
#define D_RRH_REG_MIX_LUL6              0x81020914  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 6         */
#define D_RRH_REG_MIX_LUL7              0x81020918  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 7         */
#define D_RRH_REG_MIX_LUL8              0x8102091C  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 8         */
#define D_RRH_REG_MIX_LUL9              0x81020920  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 9         */
#define D_RRH_REG_MIX_LUL10             0x81020924  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 10        */
#define D_RRH_REG_MIX_LUL11             0x81020928  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 11        */
#define D_RRH_REG_MIX_LUL12             0x8102092C  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 12        */
#define D_RRH_REG_MIX_LUL13             0x81020930  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 13        */
#define D_RRH_REG_MIX_LUL14             0x81020934  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 14        */
#define D_RRH_REG_MIX_LUL15             0x81020938  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 15        */
#define D_RRH_REG_MIX_LUL16             0x8102093C  /**< Uplink遅延調整値LTE用(DFIL調整値) Port 16        */
#define D_RRH_REG_MIX_LMIXEN            0x81020940  /**< 合成有効Port設定LTE用                            */
#define D_RRH_REG_MIX_WUL1              0x81020980  /**< Uplink遅延調整値3G用(DFIL調整値) Port 1          */
#define D_RRH_REG_MIX_WUL2              0x81020984  /**< Uplink遅延調整値3G用(DFIL調整値) Port 2          */
#define D_RRH_REG_MIX_WUL3              0x81020988  /**< Uplink遅延調整値3G用(DFIL調整値) Port 3          */
#define D_RRH_REG_MIX_WUL4              0x8102098C  /**< Uplink遅延調整値3G用(DFIL調整値) Port 4          */
#define D_RRH_REG_MIX_WUL5              0x81020990  /**< Uplink遅延調整値3G用(DFIL調整値) Port 5          */
#define D_RRH_REG_MIX_WUL6              0x81020994  /**< Uplink遅延調整値3G用(DFIL調整値) Port 6          */
#define D_RRH_REG_MIX_WUL7              0x81020998  /**< Uplink遅延調整値3G用(DFIL調整値) Port 7          */
#define D_RRH_REG_MIX_WUL8              0x8102099C  /**< Uplink遅延調整値3G用(DFIL調整値) Port 8          */
#define D_RRH_REG_MIX_WUL9              0x810209A0  /**< Uplink遅延調整値3G用(DFIL調整値) Port 9          */
#define D_RRH_REG_MIX_WUL10             0x810209A4  /**< Uplink遅延調整値3G用(DFIL調整値) Port 10         */
#define D_RRH_REG_MIX_WUL11             0x810209A8  /**< Uplink遅延調整値3G用(DFIL調整値) Port 11         */
#define D_RRH_REG_MIX_WUL12             0x810209AC  /**< Uplink遅延調整値3G用(DFIL調整値) Port 12         */
#define D_RRH_REG_MIX_WUL13             0x810209B0  /**< Uplink遅延調整値3G用(DFIL調整値) Port 13         */
#define D_RRH_REG_MIX_WUL14             0x810209B4  /**< Uplink遅延調整値3G用(DFIL調整値) Port 14         */
#define D_RRH_REG_MIX_WUL15             0x810209B8  /**< Uplink遅延調整値3G用(DFIL調整値) Port 15         */
#define D_RRH_REG_MIX_WUL16             0x810209BC  /**< Uplink遅延調整値3G用(DFIL調整値) Port 16         */
#define D_RRH_REG_MIX_WMIXEN            0x810209C0  /**< 合成有効Port設定3G用                             */
#define D_RRH_REG_MIX_AGCHITH           0x81020A00  /**< AGC電力閾値(上限)                                */
#define D_RRH_REG_MIX_AGCLOTH           0x81020A04  /**< AGC電力閾値(下限)                                */
#define D_RRH_REG_MIX_HAGCHI            0x81020A08  /**< 高速AGC実施閾値(上限)                            */
#define D_RRH_REG_MIX_HAGCLO            0x81020A0C  /**< 高速AGC実施閾値(下限)                            */
#define D_RRH_REG_MIX_AGCMOD            0x81020A10  /**< 固定GAINモード                                   */
#define D_RRH_REG_MIX_AGCFIX            0x81020A14  /**< 固定GAIN値(Gainアドレス)                         */
#define D_RRH_REG_MIX_AGC00             0x81020A18  /**< AGC Gainアドレス表示 Br0 Car0                    */
#define D_RRH_REG_MIX_AGC01             0x81020A1C  /**< AGC Gainアドレス表示 Br0 Car1                    */
#define D_RRH_REG_MIX_AGC02             0x81020A20  /**< AGC Gainアドレス表示 Br0 Car2                    */
#define D_RRH_REG_MIX_AGC03             0x81020A24  /**< AGC Gainアドレス表示 Br0 Car3                    */
#define D_RRH_REG_MIX_AGC10             0x81020A28  /**< AGC Gainアドレス表示 Br1 Car0                    */
#define D_RRH_REG_MIX_AGC11             0x81020A2C  /**< AGC Gainアドレス表示 Br1 Car1                    */
#define D_RRH_REG_MIX_AGC12             0x81020A30  /**< AGC Gainアドレス表示 Br1 Car2                    */
#define D_RRH_REG_MIX_AGC13             0x81020A34  /**< AGC Gainアドレス表示 Br1 Car3                    */
#define D_RRH_REG_MIX_RSLMTHI           0x81020B00  /**< 合成RSSI 報告閾値(上限）                         */
#define D_RRH_REG_MIX_RSLMTLO           0x81020B04  /**< 合成RSSI 報告閾値(下限）                         */
#define D_RRH_REG_MIX_RSOFFSET          0x81020B08  /**< 合成RSSI 対数用オフセット補正値                  */
#define D_RRH_REG_MIX_RSVS              0x81020B0C  /**< 合成RSSI報告ＶＳ割り当て設定                     */
#define D_RRH_REG_MIX_RS00              0x81020B80  /**< 合成RSSI値モニタ表示 Br0 Car0                    */
#define D_RRH_REG_MIX_RS01              0x81020B84  /**< 合成RSSI値モニタ表示 Br0 Car1                    */
#define D_RRH_REG_MIX_RS02              0x81020B88  /**< 合成RSSI値モニタ表示 Br0 Car2                    */
#define D_RRH_REG_MIX_RS03              0x81020B8C  /**< 合成RSSI値モニタ表示 Br0 Car3                    */
#define D_RRH_REG_MIX_RS10              0x81020B90  /**< 合成RSSI値モニタ表示 Br1 Car0                    */
#define D_RRH_REG_MIX_RS11              0x81020B94  /**< 合成RSSI値モニタ表示 Br1 Car1                    */
#define D_RRH_REG_MIX_RS12              0x81020B98  /**< 合成RSSI値モニタ表示 Br1 Car2                    */
#define D_RRH_REG_MIX_RS13              0x81020B9C  /**< 合成RSSI値モニタ表示 Br1 Car3                    */
#define D_RRH_REG_MIX_R01RS00           0x81020C00  /**< RSSI値モニタ表示 Port1 Br0 Car0                  */
#define D_RRH_REG_MIX_R01RS01           0x81020C04  /**< RSSI値モニタ表示 Port1 Br0 Car1                  */
#define D_RRH_REG_MIX_R01RS02           0x81020C08  /**< RSSI値モニタ表示 Port1 Br0 Car2                  */
#define D_RRH_REG_MIX_R01RS03           0x81020C0C  /**< RSSI値モニタ表示 Port1 Br0 Car3                  */
#define D_RRH_REG_MIX_R01RS10           0x81020C10  /**< RSSI値モニタ表示 Port1 Br1 Car0                  */
#define D_RRH_REG_MIX_R01RS11           0x81020C14  /**< RSSI値モニタ表示 Port1 Br1 Car1                  */
#define D_RRH_REG_MIX_R01RS12           0x81020C18  /**< RSSI値モニタ表示 Port1 Br1 Car2                  */
#define D_RRH_REG_MIX_R01RS13           0x81020C1C  /**< RSSI値モニタ表示 Port1 Br1 Car3                  */
#define D_RRH_REG_MIX_R02RS00           0x81020C20  /**< RSSI値モニタ表示 Port2 Br0 Car0                  */
#define D_RRH_REG_MIX_R02RS01           0x81020C24  /**< RSSI値モニタ表示 Port2 Br0 Car1                  */
#define D_RRH_REG_MIX_R02RS02           0x81020C28  /**< RSSI値モニタ表示 Port2 Br0 Car2                  */
#define D_RRH_REG_MIX_R02RS03           0x81020C2C  /**< RSSI値モニタ表示 Port2 Br0 Car3                  */
#define D_RRH_REG_MIX_R02RS10           0x81020C30  /**< RSSI値モニタ表示 Port2 Br1 Car0                  */
#define D_RRH_REG_MIX_R02RS11           0x81020C34  /**< RSSI値モニタ表示 Port2 Br1 Car1                  */
#define D_RRH_REG_MIX_R02RS12           0x81020C38  /**< RSSI値モニタ表示 Port2 Br1 Car2                  */
#define D_RRH_REG_MIX_R02RS13           0x81020C3C  /**< RSSI値モニタ表示 Port2 Br1 Car3                  */
#define D_RRH_REG_MIX_R03RS00           0x81020C40  /**< RSSI値モニタ表示 Port3 Br0 Car0                  */
#define D_RRH_REG_MIX_R03RS01           0x81020C44  /**< RSSI値モニタ表示 Port3 Br0 Car1                  */
#define D_RRH_REG_MIX_R03RS02           0x81020C48  /**< RSSI値モニタ表示 Port3 Br0 Car2                  */
#define D_RRH_REG_MIX_R03RS03           0x81020C4C  /**< RSSI値モニタ表示 Port3 Br0 Car3                  */
#define D_RRH_REG_MIX_R03RS10           0x81020C50  /**< RSSI値モニタ表示 Port3 Br1 Car0                  */
#define D_RRH_REG_MIX_R03RS11           0x81020C54  /**< RSSI値モニタ表示 Port3 Br1 Car1                  */
#define D_RRH_REG_MIX_R03RS12           0x81020C58  /**< RSSI値モニタ表示 Port3 Br1 Car2                  */
#define D_RRH_REG_MIX_R03RS13           0x81020C5C  /**< RSSI値モニタ表示 Port3 Br1 Car3                  */
#define D_RRH_REG_MIX_R04RS00           0x81020C60  /**< RSSI値モニタ表示 Port4 Br0 Car0                  */
#define D_RRH_REG_MIX_R04RS01           0x81020C64  /**< RSSI値モニタ表示 Port4 Br0 Car1                  */
#define D_RRH_REG_MIX_R04RS02           0x81020C68  /**< RSSI値モニタ表示 Port4 Br0 Car2                  */
#define D_RRH_REG_MIX_R04RS03           0x81020C6C  /**< RSSI値モニタ表示 Port4 Br0 Car3                  */
#define D_RRH_REG_MIX_R04RS10           0x81020C70  /**< RSSI値モニタ表示 Port4 Br1 Car0                  */
#define D_RRH_REG_MIX_R04RS11           0x81020C74  /**< RSSI値モニタ表示 Port4 Br1 Car1                  */
#define D_RRH_REG_MIX_R04RS12           0x81020C78  /**< RSSI値モニタ表示 Port4 Br1 Car2                  */
#define D_RRH_REG_MIX_R04RS13           0x81020C7C  /**< RSSI値モニタ表示 Port4 Br1 Car3                  */
#define D_RRH_REG_MIX_R05RS00           0x81020C80  /**< RSSI値モニタ表示 Port5 Br0 Car0                  */
#define D_RRH_REG_MIX_R05RS01           0x81020C84  /**< RSSI値モニタ表示 Port5 Br0 Car1                  */
#define D_RRH_REG_MIX_R05RS02           0x81020C88  /**< RSSI値モニタ表示 Port5 Br0 Car2                  */
#define D_RRH_REG_MIX_R05RS03           0x81020C8C  /**< RSSI値モニタ表示 Port5 Br0 Car3                  */
#define D_RRH_REG_MIX_R05RS10           0x81020C90  /**< RSSI値モニタ表示 Port5 Br1 Car0                  */
#define D_RRH_REG_MIX_R05RS11           0x81020C94  /**< RSSI値モニタ表示 Port5 Br1 Car1                  */
#define D_RRH_REG_MIX_R05RS12           0x81020C98  /**< RSSI値モニタ表示 Port5 Br1 Car2                  */
#define D_RRH_REG_MIX_R05RS13           0x81020C9C  /**< RSSI値モニタ表示 Port5 Br1 Car3                  */
#define D_RRH_REG_MIX_R06RS00           0x81020CA0  /**< RSSI値モニタ表示 Port6 Br0 Car0                  */
#define D_RRH_REG_MIX_R06RS01           0x81020CA4  /**< RSSI値モニタ表示 Port6 Br0 Car1                  */
#define D_RRH_REG_MIX_R06RS02           0x81020CA8  /**< RSSI値モニタ表示 Port6 Br0 Car2                  */
#define D_RRH_REG_MIX_R06RS03           0x81020CAC  /**< RSSI値モニタ表示 Port6 Br0 Car3                  */
#define D_RRH_REG_MIX_R06RS10           0x81020CB0  /**< RSSI値モニタ表示 Port6 Br1 Car0                  */
#define D_RRH_REG_MIX_R06RS11           0x81020CB4  /**< RSSI値モニタ表示 Port6 Br1 Car1                  */
#define D_RRH_REG_MIX_R06RS12           0x81020CB8  /**< RSSI値モニタ表示 Port6 Br1 Car2                  */
#define D_RRH_REG_MIX_R06RS13           0x81020CBC  /**< RSSI値モニタ表示 Port6 Br1 Car3                  */
#define D_RRH_REG_MIX_R07RS00           0x81020CC0  /**< RSSI値モニタ表示 Port7 Br0 Car0                  */
#define D_RRH_REG_MIX_R07RS01           0x81020CC4  /**< RSSI値モニタ表示 Port7 Br0 Car1                  */
#define D_RRH_REG_MIX_R07RS02           0x81020CC8  /**< RSSI値モニタ表示 Port7 Br0 Car2                  */
#define D_RRH_REG_MIX_R07RS03           0x81020CCC  /**< RSSI値モニタ表示 Port7 Br0 Car3                  */
#define D_RRH_REG_MIX_R07RS10           0x81020CD0  /**< RSSI値モニタ表示 Port7 Br1 Car0                  */
#define D_RRH_REG_MIX_R07RS11           0x81020CD4  /**< RSSI値モニタ表示 Port7 Br1 Car1                  */
#define D_RRH_REG_MIX_R07RS12           0x81020CD8  /**< RSSI値モニタ表示 Port7 Br1 Car2                  */
#define D_RRH_REG_MIX_R07RS13           0x81020CDC  /**< RSSI値モニタ表示 Port7 Br1 Car3                  */
#define D_RRH_REG_MIX_R08RS00           0x81020CE0  /**< RSSI値モニタ表示 Port8 Br0 Car0                  */
#define D_RRH_REG_MIX_R08RS01           0x81020CE4  /**< RSSI値モニタ表示 Port8 Br0 Car1                  */
#define D_RRH_REG_MIX_R08RS02           0x81020CE8  /**< RSSI値モニタ表示 Port8 Br0 Car2                  */
#define D_RRH_REG_MIX_R08RS03           0x81020CEC  /**< RSSI値モニタ表示 Port8 Br0 Car3                  */
#define D_RRH_REG_MIX_R08RS10           0x81020CF0  /**< RSSI値モニタ表示 Port8 Br1 Car0                  */
#define D_RRH_REG_MIX_R08RS11           0x81020CF4  /**< RSSI値モニタ表示 Port8 Br1 Car1                  */
#define D_RRH_REG_MIX_R08RS12           0x81020CF8  /**< RSSI値モニタ表示 Port8 Br1 Car2                  */
#define D_RRH_REG_MIX_R08RS13           0x81020CFC  /**< RSSI値モニタ表示 Port8 Br1 Car3                  */
#define D_RRH_REG_MIX_R09RS00           0x81020D00  /**< RSSI値モニタ表示 Port9 Br0 Car0                  */
#define D_RRH_REG_MIX_R09RS01           0x81020D04  /**< RSSI値モニタ表示 Port9 Br0 Car1                  */
#define D_RRH_REG_MIX_R09RS02           0x81020D08  /**< RSSI値モニタ表示 Port9 Br0 Car2                  */
#define D_RRH_REG_MIX_R09RS03           0x81020D0C  /**< RSSI値モニタ表示 Port9 Br0 Car3                  */
#define D_RRH_REG_MIX_R09RS10           0x81020D10  /**< RSSI値モニタ表示 Port9 Br1 Car0                  */
#define D_RRH_REG_MIX_R09RS11           0x81020D14  /**< RSSI値モニタ表示 Port9 Br1 Car1                  */
#define D_RRH_REG_MIX_R09RS12           0x81020D18  /**< RSSI値モニタ表示 Port9 Br1 Car2                  */
#define D_RRH_REG_MIX_R09RS13           0x81020D1C  /**< RSSI値モニタ表示 Port9 Br1 Car3                  */
#define D_RRH_REG_MIX_R10RS00           0x81020D20  /**< RSSI値モニタ表示 Port10 Br0 Car0                 */
#define D_RRH_REG_MIX_R10RS01           0x81020D24  /**< RSSI値モニタ表示 Port10 Br0 Car1                 */
#define D_RRH_REG_MIX_R10RS02           0x81020D28  /**< RSSI値モニタ表示 Port10 Br0 Car2                 */
#define D_RRH_REG_MIX_R10RS03           0x81020D2C  /**< RSSI値モニタ表示 Port10 Br0 Car3                 */
#define D_RRH_REG_MIX_R10RS10           0x81020D30  /**< RSSI値モニタ表示 Port10 Br1 Car0                 */
#define D_RRH_REG_MIX_R10RS11           0x81020D34  /**< RSSI値モニタ表示 Port10 Br1 Car1                 */
#define D_RRH_REG_MIX_R10RS12           0x81020D38  /**< RSSI値モニタ表示 Port10 Br1 Car2                 */
#define D_RRH_REG_MIX_R10RS13           0x81020D3C  /**< RSSI値モニタ表示 Port10 Br1 Car3                 */
#define D_RRH_REG_MIX_R11RS00           0x81020D40  /**< RSSI値モニタ表示 Port11 Br0 Car0                 */
#define D_RRH_REG_MIX_R11RS01           0x81020D44  /**< RSSI値モニタ表示 Port11 Br0 Car1                 */
#define D_RRH_REG_MIX_R11RS02           0x81020D48  /**< RSSI値モニタ表示 Port11 Br0 Car2                 */
#define D_RRH_REG_MIX_R11RS03           0x81020D4C  /**< RSSI値モニタ表示 Port11 Br0 Car3                 */
#define D_RRH_REG_MIX_R11RS10           0x81020D50  /**< RSSI値モニタ表示 Port11 Br1 Car0                 */
#define D_RRH_REG_MIX_R11RS11           0x81020D54  /**< RSSI値モニタ表示 Port11 Br1 Car1                 */
#define D_RRH_REG_MIX_R11RS12           0x81020D58  /**< RSSI値モニタ表示 Port11 Br1 Car2                 */
#define D_RRH_REG_MIX_R11RS13           0x81020D5C  /**< RSSI値モニタ表示 Port11 Br1 Car3                 */
#define D_RRH_REG_MIX_R12RS00           0x81020D60  /**< RSSI値モニタ表示 Port12 Br0 Car0                 */
#define D_RRH_REG_MIX_R12RS01           0x81020D64  /**< RSSI値モニタ表示 Port12 Br0 Car1                 */
#define D_RRH_REG_MIX_R12RS02           0x81020D68  /**< RSSI値モニタ表示 Port12 Br0 Car2                 */
#define D_RRH_REG_MIX_R12RS03           0x81020D6C  /**< RSSI値モニタ表示 Port12 Br0 Car3                 */
#define D_RRH_REG_MIX_R12RS10           0x81020D70  /**< RSSI値モニタ表示 Port12 Br1 Car0                 */
#define D_RRH_REG_MIX_R12RS11           0x81020D74  /**< RSSI値モニタ表示 Port12 Br1 Car1                 */
#define D_RRH_REG_MIX_R12RS12           0x81020D78  /**< RSSI値モニタ表示 Port12 Br1 Car2                 */
#define D_RRH_REG_MIX_R12RS13           0x81020D7C  /**< RSSI値モニタ表示 Port12 Br1 Car3                 */
#define D_RRH_REG_MIX_R13RS00           0x81020D80  /**< RSSI値モニタ表示 Port13 Br0 Car0                 */
#define D_RRH_REG_MIX_R13RS01           0x81020D84  /**< RSSI値モニタ表示 Port13 Br0 Car1                 */
#define D_RRH_REG_MIX_R13RS02           0x81020D88  /**< RSSI値モニタ表示 Port13 Br0 Car2                 */
#define D_RRH_REG_MIX_R13RS03           0x81020D8C  /**< RSSI値モニタ表示 Port13 Br0 Car3                 */
#define D_RRH_REG_MIX_R13RS10           0x81020D90  /**< RSSI値モニタ表示 Port13 Br1 Car0                 */
#define D_RRH_REG_MIX_R13RS11           0x81020D94  /**< RSSI値モニタ表示 Port13 Br1 Car1                 */
#define D_RRH_REG_MIX_R13RS12           0x81020D98  /**< RSSI値モニタ表示 Port13 Br1 Car2                 */
#define D_RRH_REG_MIX_R13RS13           0x81020D9C  /**< RSSI値モニタ表示 Port13 Br1 Car3                 */
#define D_RRH_REG_MIX_R14RS00           0x81020DA0  /**< RSSI値モニタ表示 Port14 Br0 Car0                 */
#define D_RRH_REG_MIX_R14RS01           0x81020DA4  /**< RSSI値モニタ表示 Port14 Br0 Car1                 */
#define D_RRH_REG_MIX_R14RS02           0x81020DA8  /**< RSSI値モニタ表示 Port14 Br0 Car2                 */
#define D_RRH_REG_MIX_R14RS03           0x81020DAC  /**< RSSI値モニタ表示 Port14 Br0 Car3                 */
#define D_RRH_REG_MIX_R14RS10           0x81020DB0  /**< RSSI値モニタ表示 Port14 Br1 Car0                 */
#define D_RRH_REG_MIX_R14RS11           0x81020DB4  /**< RSSI値モニタ表示 Port14 Br1 Car1                 */
#define D_RRH_REG_MIX_R14RS12           0x81020DB8  /**< RSSI値モニタ表示 Port14 Br1 Car2                 */
#define D_RRH_REG_MIX_R14RS13           0x81020DBC  /**< RSSI値モニタ表示 Port14 Br1 Car3                 */
#define D_RRH_REG_MIX_R15RS00           0x81020DC0  /**< RSSI値モニタ表示 Port15 Br0 Car0                 */
#define D_RRH_REG_MIX_R15RS01           0x81020DC4  /**< RSSI値モニタ表示 Port15 Br0 Car1                 */
#define D_RRH_REG_MIX_R15RS02           0x81020DC8  /**< RSSI値モニタ表示 Port15 Br0 Car2                 */
#define D_RRH_REG_MIX_R15RS03           0x81020DCC  /**< RSSI値モニタ表示 Port15 Br0 Car3                 */
#define D_RRH_REG_MIX_R15RS10           0x81020DD0  /**< RSSI値モニタ表示 Port15 Br1 Car0                 */
#define D_RRH_REG_MIX_R15RS11           0x81020DD4  /**< RSSI値モニタ表示 Port15 Br1 Car1                 */
#define D_RRH_REG_MIX_R15RS12           0x81020DD8  /**< RSSI値モニタ表示 Port15 Br1 Car2                 */
#define D_RRH_REG_MIX_R15RS13           0x81020DDC  /**< RSSI値モニタ表示 Port15 Br1 Car3                 */
#define D_RRH_REG_MIX_R16RS00           0x81020DE0  /**< RSSI値モニタ表示 Port16 Br0 Car0                 */
#define D_RRH_REG_MIX_R16RS01           0x81020DE4  /**< RSSI値モニタ表示 Port16 Br0 Car1                 */
#define D_RRH_REG_MIX_R16RS02           0x81020DE8  /**< RSSI値モニタ表示 Port16 Br0 Car2                 */
#define D_RRH_REG_MIX_R16RS03           0x81020DEC  /**< RSSI値モニタ表示 Port16 Br0 Car3                 */
#define D_RRH_REG_MIX_R16RS10           0x81020DF0  /**< RSSI値モニタ表示 Port16 Br1 Car0                 */
#define D_RRH_REG_MIX_R16RS11           0x81020DF4  /**< RSSI値モニタ表示 Port16 Br1 Car1                 */
#define D_RRH_REG_MIX_R16RS12           0x81020DF8  /**< RSSI値モニタ表示 Port16 Br1 Car2                 */
#define D_RRH_REG_MIX_R16RS13           0x81020DFC  /**< RSSI値モニタ表示 Port16 Br1 Car3                 */
#define D_RRH_REG_MIX_RTWPREP0          0x81020E00  /**< RTWPモニタ0                                      */
#define D_RRH_REG_MIX_RTWPREP1          0x81020E04  /**< RTWPモニタ1                                      */
#define D_RRH_REG_MIX_RTWPREP2          0x81020E08  /**< RTWPモニタ2                                      */
#define D_RRH_REG_MIX_RTWPREP3          0x81020E0C  /**< RTWPモニタ3                                      */
#define D_RRH_REG_MIX_RTWPREP4          0x81020E10  /**< RTWPモニタ4                                      */
#define D_RRH_REG_MIX_RTWPREP5          0x81020E14  /**< RTWPモニタ5                                      */
#define D_RRH_REG_MIX_RTWPREP6          0x81020E18  /**< RTWPモニタ6                                      */
#define D_RRH_REG_MIX_RTWPREP7          0x81020E1C  /**< RTWPモニタ7                                      */
#define D_RRH_REG_MIX_PMON01            0x81020E80  /**< 遅延調整前位相表示モニタ Port 1                  */
#define D_RRH_REG_MIX_PMON02            0x81020E84  /**< 遅延調整前位相表示モニタ Port 2                  */
#define D_RRH_REG_MIX_PMON03            0x81020E88  /**< 遅延調整前位相表示モニタ Port 3                  */
#define D_RRH_REG_MIX_PMON04            0x81020E8C  /**< 遅延調整前位相表示モニタ Port 4                  */
#define D_RRH_REG_MIX_PMON05            0x81020E90  /**< 遅延調整前位相表示モニタ Port 5                  */
#define D_RRH_REG_MIX_PMON06            0x81020E94  /**< 遅延調整前位相表示モニタ Port 6                  */
#define D_RRH_REG_MIX_PMON07            0x81020E98  /**< 遅延調整前位相表示モニタ Port 7                  */
#define D_RRH_REG_MIX_PMON08            0x81020E9C  /**< 遅延調整前位相表示モニタ Port 8                  */
#define D_RRH_REG_MIX_PMON09            0x81020EA0  /**< 遅延調整前位相表示モニタ Port 9                  */
#define D_RRH_REG_MIX_PMON10            0x81020EA4  /**< 遅延調整前位相表示モニタ Port 10                 */
#define D_RRH_REG_MIX_PMON11            0x81020EA8  /**< 遅延調整前位相表示モニタ Port 11                 */
#define D_RRH_REG_MIX_PMON12            0x81020EAC  /**< 遅延調整前位相表示モニタ Port 12                 */
#define D_RRH_REG_MIX_PMON13            0x81020EB0  /**< 遅延調整前位相表示モニタ Port 13                 */
#define D_RRH_REG_MIX_PMON14            0x81020EB4  /**< 遅延調整前位相表示モニタ Port 14                 */
#define D_RRH_REG_MIX_PMON15            0x81020EB8  /**< 遅延調整前位相表示モニタ Port 15                 */
#define D_RRH_REG_MIX_PMON16            0x81020EBC  /**< 遅延調整前位相表示モニタ Port 16                 */
#define D_RRH_REG_MIX_PMONMAP           0x81020EC0  /**< 遅延調整後位相表示モニタ                         */
#define D_RRH_REG_MIX_MIXTEST           0x81020F30  /**< テストモード(reserve)                            */
#define D_RRH_REG_MIX_MIXMON            0x81020F40  /**< テストモニタ(reserve)                            */
#define D_RRH_REG_MIX_MIXMONSEL         0x81020FFC  /**< モニタ選択                                       */
/** @} */

#define M_RRH_REG_MIX_LUL(cprino)  (UINT)(D_RRH_REG_MIX_LUL1   + 0x4*((cprino) - 1))
#define M_RRH_REG_MIX_WUL(cprino)  (UINT)(D_RRH_REG_MIX_WUL1   + 0x4*((cprino) - 1))
#define M_RRH_REG_MIX_PMON(cprino) (UINT)(D_RRH_REG_MIX_PMON01 + 0x4*((cprino) - 1))

#endif
/* @} */
