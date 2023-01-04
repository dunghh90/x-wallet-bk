/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_def.h
 * @brief  define
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/05/25 FJT)Nagasima CR-00012-001
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 * @date   2009/01/17 FFCS)Shihzh modify for BI-TBD-557
 * @date   2009/01/19 FFCS)Shihzh modify for eNB-004-003 BD-TBD-11
 * @date   2009/02/04 FFCS)Shihzh M-S3G-eNBPF-01284
 * @date   2009/02/04 FFCS)Shihzh M-S3G-eNBPF-01284
 * @date   2009/03/04 FFCS)Shihzh modify for API 2nd release
 * @date   2009/03/25 FFCS)Shihzh M-S3G-eNBPF-01581
 * @date   2009/06/23 FFCS)Shihzh M-S3G-eNBPF-02129
 * @date   2010/10/25 FJT)Koshida create for eNB-004 of TS-656案5対応.
 * @date   2010/11/12 FJT)Tokunaga M-S3G-eNBSYS-xxxxx [TS-810項]
 * @date   2011/02/17 FJT)Koshida modify for M-S3G-eNBSYS-02253(TS-111)対応.
 * @date   2011/12/11   FJT)K.Koshida  DMICRO-008
 * @date   2016/02/25 TDI)satou キャリア状態不一致補正保護段数を8回に変更
 * @date   2021/02/04 M&C)Huan.dh add CARONOFF event
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2011
 */
/****************************************************************************/
#ifndef _F_TRP_REC_DEF_h
#define _F_TRP_REC_DEF_h

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  クラス
 */
typedef enum {
    EC_REC_COM,
    EC_REC_C01,
    EC_REC_C02,
    EC_REC_C02_3G,
    EC_REC_C03,
    EC_REC_C03_3G,
    EC_REC_C04,
    EC_REC_C05,
    EC_REC_C06,
    EC_REC_C07,
    EC_REC_C08,
    EC_REC_CFF,
    EC_REC_CLS_MAX
}   ET_REC_CLS;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(全)
 */
typedef enum {
    EC_REC_CXX_EVT_IT_THRINI_REQ,         /*!< スレッド初期化要求                                             */
    EC_REC_CXX_EVT_IT_ATHINICMP_NTC,      /*!< 全スレッド初期化完了通知                                       */
    EC_REC_CXX_EVT_IT_APRINICMP_NTC,      /*!< 全プロセス初期化完了通知                                       */
    EC_REC_CXX_EVT_IT_CPRSTSNTC_B,        /*!< CPRIリンク状態変化通知(StateB)                                 */
    EC_REC_CXX_EVT_IT_CPRSTSNTC_C,        /*!< CPRIリンク状態変化通知(StateC)                                 */
    EC_REC_CXX_EVT_IT_CPRSTSNTC_F,        /*!< CPRIリンク状態変化通知(StateF)                                 */
    EC_REC_CXX_EVT_IT_CPRSTSNTC_L,        /*!< CPRIリンク状態変化通知(L3確立)                                 */
    EC_REC_CXX_EVT_IT_CPRDSCNTC_E,        /*!< CPRIリンク断通知(StateF未満)                                   */
    EC_REC_CXX_EVT_IT_CPRDSCNTC_F,        /*!< CPRIリンク断通知(StateF)                                       */
    EC_REC_CXX_EVT_IT_ACTSET_RSP,         /*!< Active設定応答                                                 */
    EC_REC_CXX_EVT_IT_ACTCHG_NTC_OK,      /*!< Active切替結果通知(OK)                                         */
    EC_REC_CXX_EVT_IT_ACTCHG_NTC_NG,      /*!< Active切替結果通知(NG)                                         */
    EC_REC_CXX_EVT_IT_NEG_NTC,            /*!< Negative通知(Negative)                                         */
    EC_REC_CXX_EVT_IT_NEGTO_NTC,          /*!< Negative通知(Negative TO)                                      */
    EC_REC_CXX_EVT_IT_2NDRST_REQ,         /*!< 二次リセット要求                                               */
    EC_REC_CXX_EVT_IT_REPRM_RSP,          /*!< REシステムパラメータ更新応答                                   */
    EC_REC_CXX_EVT_IT_REFIL_RSP,          /*!< REファイル更新応答                                             */
    EC_REC_CXX_EVT_IT_2NDPRC_IND,         /*!< 二次リセットプロシージャ実行指示                               */
    EC_REC_CXX_EVT_IT_RETIMSETPRC_IND,    /*!< RE時刻設定プロシージャ実行指示                                 */
    EC_REC_CXX_EVT_IT_REPRMPRC_IND,       /*!< REシステムパラメータ更新プロシージャ実行指示                   */
    EC_REC_CXX_EVT_IT_REFILPRC_IND,       /*!< REファイル更新プロシージャ実行指示                             */
    EC_REC_CXX_EVT_IT_TOFVALPRC_IND,      /*!< Toffset値報告プロシージャ実行指示                              */
    EC_REC_CXX_EVT_IT_ACTSETPRC_IND,      /*!< Active設定プロシージャ実行指示                                 */
    EC_REC_CXX_EVT_IT_REEQPINFPRC_IND,    /*!< RE装置構成情報報告プロシージャ実行指示                         */
    EC_REC_CXX_EVT_IT_RESLTINFPRC_IND,    /*!< REスロット情報報告プロシージャ実行指示                         */
    EC_REC_CXX_EVT_IT_RESTSPRC_IND,       /*!< RE状態報告確プロシージャ実行指示                               */
    EC_REC_CXX_EVT_IT_REPRTPRC_IND,       /*!< RE PORT状態報告プロシージャ実行指示                            */
    EC_REC_CXX_EVT_IT_RECRDSSCPRC_IND,    /*!< REカード状態報告(セクタ部)プロシージャ実行指示                 */
    EC_REC_CXX_EVT_IT_RECRDSCMPRC_IND,    /*!< REカード状態報告(共通部)プロシージャ実行指示                   */
    EC_REC_CXX_EVT_IT_RECRRSTSPRC_IND,    /*!< REキャリア状態報告プロシージャ実行指示                         */
    EC_REC_CXX_EVT_IT_MTSETFPRC_IND,      /*!< MTアドレス設定(FLD-MT)プロシージャ実行指示                     */
    EC_REC_CXX_EVT_IT_MTSETSPRC_IND,      /*!< MTアドレス設定(SV-MT)プロシージャ実行指示                      */
    EC_REC_CXX_EVT_IT_RESCMPPRC_IND,      /*!< RE起動完了プロシージャ実行指示                                 */
    EC_REC_CXX_EVT_IT_RERSTPRC_IND,       /*!< REリセットプロシージャ実行指示                                 */
    EC_REC_CXX_EVT_IT_REFRCRSTPRC_IND,    /*!< RE強制リセットプロシージャ実行指示                             */
    EC_REC_CXX_EVT_IT_FRCSTPPRC_IND,      /*!< 強制停止プロシージャ実行指示                                   */
    EC_REC_CXX_EVT_IT_TXSAPRC_IND,        /*!< TRX設定(自律)プロシージャ実行指示                              */
    EC_REC_CXX_EVT_IT_TXRAPRC_IND,        /*!< TRX解放(自律)プロシージャ実行指示                              */
    EC_REC_CXX_EVT_IT_NONAPRC_IND,        /*!< 無変調キャリアON/OFF指定(自律)(ON)プロシージャ実行指示         */
    EC_REC_CXX_EVT_IT_NOFAPRC_IND,        /*!< 無変調キャリアON/OFF指定(自律)(OFF)プロシージャ実行指示        */
    EC_REC_CXX_EVT_L3_TXS_RSP,            /*!< CPRI信号受信通知(TRX設定応答)                                  */
    EC_REC_CXX_EVT_L3_TXR_RSP,            /*!< CPRI信号受信通知(TRX解放応答)                                  */
    EC_REC_CXX_EVT_L3_TOFVAL_RSP,         /*!< CPRI信号受信通知(Toffset値報告応答)                            */
    EC_REC_CXX_EVT_L3_NMC_RSP,            /*!< CPRI信号受信通知(無変調キャリアON/OFF指定応答)                 */
    EC_REC_CXX_EVT_L3_REEQPINF_RSP,       /*!< CPRI信号受信通知(RE装置構成情報報告応答)                       */
    EC_REC_CXX_EVT_L3_RESLTINF_RSP,       /*!< CPRI信号受信通知(REスロット情報報告応答)                       */
    EC_REC_CXX_EVT_L3_RESTS_RSP,          /*!< CPRI信号受信通知(RE状態報告応答)                               */
    EC_REC_CXX_EVT_L3_RECRDINF_RSP,       /*!< CPRI信号受信通知(REカード情報報告応答)                         */
    EC_REC_CXX_EVT_L3_REVER_RSP,          /*!< CPRI信号受信通知(REバージョン報告応答)                         */
    EC_REC_CXX_EVT_L3_RECRDCTL_RSP,       /*!< CPRI信号受信通知(REカード制御応答)                             */
    EC_REC_CXX_EVT_L3_RECRDSSC_RSP,       /*!< CPRI信号受信通知(REカード状態報告(セクタ部)応答)               */
    EC_REC_CXX_EVT_L3_RECRDSCM_RSP,       /*!< CPRI信号受信通知(REカード状態報告(共通部)応答)                 */
    EC_REC_CXX_EVT_L3_RECRRSTS_RSP,       /*!< CPRI信号受信通知(REキャリア状態報告応答)                       */
    EC_REC_CXX_EVT_L3_REPRTCTL_RSP,       /*!< CPRI信号受信通知(RE PORT制御応答)                              */
    EC_REC_CXX_EVT_L3_REPRTSTS_RSP,       /*!< CPRI信号受信通知(RE PORT状態報告応答)                          */
    EC_REC_CXX_EVT_TO_TXS_CNF,            /*!< タイムアウト発生通知(TRX設定確認タイマ)                        */
    EC_REC_CXX_EVT_TO_TXR_CNF,            /*!< タイムアウト発生通知(TRX解放確認タイマ)                        */
    EC_REC_CXX_EVT_TO_TOFVAL_CNF,         /*!< タイムアウト発生通知(Toffset値報告確認タイマ)                  */
    EC_REC_CXX_EVT_TO_NMC_CNF,            /*!< タイムアウト発生通知(無変調キャリアON/OFF指定確認タイマ)       */
    EC_REC_CXX_EVT_TO_REEQPINF_CNF,       /*!< タイムアウト発生通知(RE装置構成情報報告確認タイマ)             */
    EC_REC_CXX_EVT_TO_RESLTINF_CNF,       /*!< タイムアウト発生通知(REスロット情報報告確認タイマ)             */
    EC_REC_CXX_EVT_TO_RESTS_CNF,          /*!< タイムアウト発生通知(RE状態報告確認タイマ)                     */
    EC_REC_CXX_EVT_TO_REVER_CNF,          /*!< タイムアウト発生通知(REバージョン報告確認タイマ)               */
    EC_REC_CXX_EVT_TO_RECRDCTL_CNF,       /*!< タイムアウト発生通知(REカード制御確認タイマ)                   */
    EC_REC_CXX_EVT_TO_RECRDSSC_CNF,       /*!< タイムアウト発生通知(REカード状態報告(セクタ部)確認タイマ)     */
    EC_REC_CXX_EVT_TO_RECRDSCM_CNF,       /*!< タイムアウト発生通知(REカード状態報告(共通部)確認タイマ)       */
    EC_REC_CXX_EVT_TO_RECRRSTS_CNF,       /*!< タイムアウト発生通知(REキャリア状態報告確認タイマ)             */
    EC_REC_CXX_EVT_TO_REPRTCTL_CNF,       /*!< タイムアウト発生通知(RE PORT制御確認タイマ)                    */
    EC_REC_CXX_EVT_TO_REPRTSTS_CNF,       /*!< タイムアウト発生通知(RE PORT状態報告確認タイマ)                */
    EC_REC_CXX_EVT_TO_RERST_CNF,          /*!< タイムアウト発生通知(REリセット確認タイマ)                     */
    EC_REC_CXX_EVT_TO_REFRCRST_CNF,       /*!< タイムアウト発生通知(RE強制リセット確認タイマ)                 */
    EC_REC_CXX_EVT_TO_RETIMSET_INT,       /*!< タイムアウト発生通知(RE時刻設定送信間隔タイマ)                 */
    EC_REC_CXX_EVT_TO_TOFVAL_INT,         /*!< タイムアウト発生通知(Toffset値報告送信間隔タイマ)              */
    EC_REC_CXX_EVT_TO_REEQPINF_INT,       /*!< タイムアウト発生通知(RE装置構成情報報告送信間隔タイマ)         */
    EC_REC_CXX_EVT_TO_RESTS_INT,          /*!< タイムアウト発生通知(RE状態報告送信間隔タイマ)                 */
    EC_REC_CXX_EVT_TO_RECRDSSC_INT,       /*!< タイムアウト発生通知(REカード状態報告(セクタ部)送信間隔タイマ) */
    EC_REC_CXX_EVT_TO_RECRDSCM_INT,       /*!< タイムアウト発生通知(REカード状態報告(共通部)送信間隔タイマ)   */
    EC_REC_CXX_EVT_TO_RECRRSTS_INT,       /*!< タイムアウト発生通知(REキャリア状態報告送信間隔タイマ)         */
    EC_REC_CXX_EVT_TO_REPRTSTS_INT,       /*!< タイムアウト発生通知(RE PORT状態報告送信間隔タイマ)            */
    EC_REC_CXX_EVT_TO_ACTCHG_WAT,         /*!< タイムアウト発生通知(Active切替指示送信待ちタイマ)             */
    EC_REC_CXX_EVT_TO_2NDRST_WAT,         /*!< タイムアウト発生通知(二次リセット待ちタイマ)                   */
    EC_REC_CXX_EVT_IP_TXS_REQ,            /*!< TRX設定要求                                                    */
    EC_REC_CXX_EVT_IP_TXR_REQ,            /*!< TRX解放要求                                                    */
    EC_REC_CXX_EVT_IP_NON_REQ,            /*!< 無変調キャリアON/OFF指定(ON)要求                               */
    EC_REC_CXX_EVT_IP_NOF_REQ,            /*!< 無変調キャリアON/OFF指定(OFF)要求                              */
    EC_REC_CXX_EVT_IP_CPRRST_REQ_SW,      /*!< CPRIリンクリセット要求(ソフトウェア)                           */
    EC_REC_CXX_EVT_IP_CPRRST_REQ_HW,      /*!< CPRIリンクリセット要求(ハードウェア)                           */
    EC_REC_CXX_EVT_IP_RERST_REQ,          /*!< REリセット要求                                                 */
    EC_REC_CXX_EVT_IP_TIMCHG_NTC,         /*!< 時刻変更通知                                                   */
    EC_REC_CXX_EVT_IP_RECRDCTL_REQ,       /*!< REカード制御要求                                               */
    EC_REC_CXX_EVT_IP_VER_REQ,            /*!< バージョン報告要求                                             */
    EC_REC_CXX_EVT_MAX,                   /*!< イベントMAX                                                    */
    EC_REC_CXX_EVT_NON                    /*!< イベントなし                                                   */
}   ET_REC_CXX_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(C01)
 */
typedef enum {
    EC_REC_C01_EVT_IT_THRINI_REQ,         /*!< (00)スレッド初期化要求                                             */
    EC_REC_C01_EVT_IT_ATHINICMP_NTC,      /*!< (01)全スレッド初期化完了通知                                       */
    EC_REC_C01_EVT_IT_APRINICMP_NTC,      /*!< (02)全プロセス初期化完了通知                                       */
    EC_REC_C01_EVT_IT_APSCONCMP_NTC,      /*!< (03)AP起動条件完了通知                                             */
    EC_REC_C01_EVT_IT_NECHGSTA_REQ,       /*!< (04)N/E切替開始要求                                                */
    EC_REC_C01_EVT_IT_NECHGCMP_NTC,       /*!< (05)N/E切替完了通知                                                */
    EC_REC_C01_EVT_IT_NEDATSTA_REQ,       /*!< (06)データ引継ぎ開始通知                                           */
    EC_REC_C01_EVT_IT_NEDATCMP_NTC,       /*!< (07)データ引継ぎ完了通知                                           */
    EC_REC_C01_EVT_IT_SIGSTPCMP_NTC,      /*!< (08)Auto Signal Stop Complete Notice                               */
    EC_REC_C01_EVT_MAX,                   /*!< (09)イベントMAX                                                    */
    EC_REC_C01_EVT_NON                    /*!< (10)イベントなし                                                   */
}   ET_REC_C01_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(C02)
 */
typedef enum {
    EC_REC_C02_EVT_IT_THRINI_REQ,         /*!< (00)スレッド初期化要求                                             */
    EC_REC_C02_EVT_IT_ATHINICMP_NTC,      /*!< (01)全スレッド初期化完了通知                                       */
    EC_REC_C02_EVT_IT_APRINICMP_NTC,      /*!< (02)全プロセス初期化完了通知                                       */
    EC_REC_C02_EVT_IT_CPRSTSNTC_B,        /*!< (03)CPRIリンク状態変化通知(StateB)                                 */
    EC_REC_C02_EVT_IT_CPRSTSNTC_E,        /*!< (04)CPRIリンク状態変化通知(StateE)                                 */
    EC_REC_C02_EVT_IT_CPRSTSNTC_F,        /*!< (05)CPRIリンク状態変化通知(StateF)                                 */
    EC_REC_C02_EVT_IT_CPRSTSNTC_L,        /*!< (06)CPRIリンク状態変化通知(L3確立)                                 */
    EC_REC_C02_EVT_IT_CPRDSCNTC_E,        /*!< (07)CPRIリンク断通知(StateF未満)                                   */
    EC_REC_C02_EVT_IT_CPRDSCNTC_F,        /*!< (08)CPRIリンク断通知(StateF)                                       */
    EC_REC_C02_EVT_IT_INTT14CHG_NTC,      /*!< (09)割込(T14値変化)通知                                            */
    EC_REC_C02_EVT_IT_ACTSET_RSP,         /*!< (10)Active設定応答                                                 */
    EC_REC_C02_EVT_IT_ACTCHG_NTC_OK,      /*!< (11)Active切替結果通知(OK)                                         */
    EC_REC_C02_EVT_IT_ACTCHG_NTC_NG,      /*!< (12)Active切替結果通知(NG)                                         */
    EC_REC_C02_EVT_IT_NEG_NTC,            /*!< (13)Negative通知(Negative)                                         */
    EC_REC_C02_EVT_IT_NEGTO_NTC,          /*!< (14)Negative通知(Negative TO)                                      */
    EC_REC_C02_EVT_IT_2NDRST_REQ,         /*!< (15)二次リセット要求                                               */
    EC_REC_C02_EVT_IT_REPRM_RSP,          /*!< (16)REシステムパラメータ更新応答                                   */
    EC_REC_C02_EVT_IT_REFIL_RSP,          /*!< (17)REファイル更新応答                                             */
    EC_REC_C02_EVT_IT_MTSETF_RSP,         /*!< (18)MTアドレス設定(FLD-MT)応答                                     */
    EC_REC_C02_EVT_IT_MTSETS_RSP,         /*!< (19)MTアドレス設定(SV-MT)応答                                      */
    EC_REC_C02_EVT_IT_2NDPRC_IND,         /*!< (20)二次リセットプロシージャ実行指示                               */
    EC_REC_C02_EVT_IT_RETIMSETPRC_IND,    /*!< (21)RE時刻設定プロシージャ実行指示                                 */
    EC_REC_C02_EVT_IT_REPRMPRC_IND,       /*!< (22)REシステムパラメータ更新プロシージャ実行指示                   */
    EC_REC_C02_EVT_IT_REFILPRC_IND,       /*!< (23)REファイル更新プロシージャ実行指示                             */
    EC_REC_C02_EVT_IT_TOFVALPRC_IND,      /*!< (24)Toffset値報告プロシージャ実行指示                              */
    EC_REC_C02_EVT_IT_ACTSETPRC_IND,      /*!< (25)Active設定プロシージャ実行指示                                 */
    EC_REC_C02_EVT_IT_REEQPINFPRC_IND,    /*!< (26)RE装置構成情報報告プロシージャ実行指示                         */
    EC_REC_C02_EVT_IT_RESLTINFPRC_IND,    /*!< (27)REスロット情報報告プロシージャ実行指示                         */
    EC_REC_C02_EVT_IT_RESTSPRC_IND,       /*!< (28)RE状態報告確プロシージャ実行指示                               */
    EC_REC_C02_EVT_IT_REPRTPRC_IND,       /*!< (29)RE PORT状態報告プロシージャ実行指示                            */
    EC_REC_C02_EVT_IT_RECRDSSCPRC_IND,    /*!< (30)REカード状態報告(セクタ部)プロシージャ実行指示                 */
    EC_REC_C02_EVT_IT_RECRDSCMPRC_IND,    /*!< (31)REカード状態報告(共通部)プロシージャ実行指示                   */
    EC_REC_C02_EVT_IT_RECRRSTSPRC_IND,    /*!< (32)REキャリア状態報告プロシージャ実行指示                         */
    EC_REC_C02_EVT_IT_MTSETFPRC_IND,      /*!< (33)MTアドレス設定(FLD-MT)プロシージャ実行指示                     */
    EC_REC_C02_EVT_IT_MTSETSPRC_IND,      /*!< (34)MTアドレス設定(SV-MT)プロシージャ実行指示                      */
    EC_REC_C02_EVT_IT_RESCMPPRC_IND,      /*!< (35)RE起動完了プロシージャ実行指示                                 */
    EC_REC_C02_EVT_IT_RERSTPRC_IND,       /*!< (36)REリセットプロシージャ実行指示                                 */
    EC_REC_C02_EVT_IT_REFRCRSTSETPRC_IND, /*!< (37)RE強制リセット設置プロシージャ実行指示                         */
    EC_REC_C02_EVT_IT_REFRCRSTPRC_IND,    /*!< (38)RE強制リセットプロシージャ実行指示                             */
    EC_REC_C02_EVT_IT_FRCSTPPRC_IND,      /*!< (39)強制停止プロシージャ実行指示                                   */
    EC_REC_C02_EVT_L3_TOFVAL_RSP,         /*!< (40)CPRI信号受信通知(Toffset値報告応答)                            */
    EC_REC_C02_EVT_L3_REEQPINF_RSP,       /*!< (41)CPRI信号受信通知(RE装置構成情報報告応答)                       */
    EC_REC_C02_EVT_L3_RESLTINF_RSP,       /*!< (42)CPRI信号受信通知(REスロット情報報告応答)                       */
    EC_REC_C02_EVT_L3_RESTS_RSP,          /*!< (43)CPRI信号受信通知(RE状態報告応答)                               */
    EC_REC_C02_EVT_L3_REPRTSTS_RSP,       /*!< (44)CPRI信号受信通知(RE PORT状態報告応答)                          */
    EC_REC_C02_EVT_L3_RECRDSSC_RSP,       /*!< (45)CPRI信号受信通知(REカード状態報告(セクタ部)応答)               */
    EC_REC_C02_EVT_L3_RECRDSCM_RSP,       /*!< (46)CPRI信号受信通知(REカード状態報告(共通部)応答)                 */
    EC_REC_C02_EVT_L3_RECRRSTS_RSP,       /*!< (47)CPRI信号受信通知(REキャリア状態報告応答)                       */
    EC_REC_C02_EVT_TO_TOFVAL_CNF,         /*!< (48)タイムアウト発生通知(Toffset値報告確認タイマ)                  */
    EC_REC_C02_EVT_TO_REEQPINF_CNF,       /*!< (49)タイムアウト発生通知(RE装置構成情報報告確認タイマ)             */
    EC_REC_C02_EVT_TO_RESLTINF_CNF,       /*!< (50)タイムアウト発生通知(REスロット情報報告確認タイマ)             */
    EC_REC_C02_EVT_TO_RESTS_CNF,          /*!< (51)タイムアウト発生通知(RE状態報告確認タイマ)                     */
    EC_REC_C02_EVT_TO_REPRTSTS_CNF,       /*!< (52)タイムアウト発生通知(RE PORT状態報告確認タイマ)                */
    EC_REC_C02_EVT_TO_RECRDSSC_CNF,       /*!< (53)タイムアウト発生通知(REカード状態報告(セクタ部)確認タイマ)     */
    EC_REC_C02_EVT_TO_RECRDSCM_CNF,       /*!< (54)タイムアウト発生通知(REカード状態報告(共通部)確認タイマ)       */
    EC_REC_C02_EVT_TO_RECRRSTS_CNF,       /*!< (55)タイムアウト発生通知(REキャリア状態報告確認タイマ)             */
    EC_REC_C02_EVT_TO_RERST_CNF,          /*!< (56)タイムアウト発生通知(REリセット確認タイマ)                     */
    EC_REC_C02_EVT_TO_REFRCRST_CNF,       /*!< (57)タイムアウト発生通知(RE強制リセット確認タイマ)                 */
    EC_REC_C02_EVT_TO_RETIMSET_INT,       /*!< (58)タイムアウト発生通知(RE時刻設定送信間隔タイマ)                 */
    EC_REC_C02_EVT_TO_TOFVAL_INT,         /*!< (59)タイムアウト発生通知(Toffset値報告送信間隔タイマ)              */
    EC_REC_C02_EVT_TO_REEQPINF_INT,       /*!< (60)タイムアウト発生通知(RE装置構成情報報告送信間隔タイマ)         */
    EC_REC_C02_EVT_TO_RESTS_INT,          /*!< (61)タイムアウト発生通知(RE状態報告送信間隔タイマ)                 */
    EC_REC_C02_EVT_TO_REPRTSTS_INT,       /*!< (62)タイムアウト発生通知(RE PORT状態報告送信間隔タイマ)            */
    EC_REC_C02_EVT_TO_RECRDSSC_INT,       /*!< (63)タイムアウト発生通知(REカード状態報告(セクタ部)送信間隔タイマ) */
    EC_REC_C02_EVT_TO_RECRDSCM_INT,       /*!< (64)タイムアウト発生通知(REカード状態報告(共通部)送信間隔タイマ)   */
    EC_REC_C02_EVT_TO_RECRRSTS_INT,       /*!< (65)タイムアウト発生通知(REキャリア状態報告送信間隔タイマ)         */
    EC_REC_C02_EVT_TO_2NDRST_WAT,         /*!< (66)タイムアウト発生通知(二次リセット待ちタイマ)                   */
    EC_REC_C02_EVT_TO_ACTCHG_WAT,         /*!< (67)タイムアウト発生通知(Active切替指示送信待ちタイマ)             */
    EC_REC_C02_EVT_TO_T14RTY_WAT,         /*!< (68)タイムアウト発生通知(遅延値設定待ちタイマ)                     */
    EC_REC_C02_EVT_TO_REAUTRST_WAT,       /*!< (69)タイムアウト発生通知(REauto Reset 待ちタイマ)                 */
    EC_REC_C02_EVT_IT_NECHGSTA_REQ,       /*!< (70)N/E切替開始要求                                                */
    EC_REC_C02_EVT_IT_NECHGCMP_NTC,       /*!< (71)N/E切替完了通知                                                */
    EC_REC_C02_EVT_IT_NEDATSTA_REQ,       /*!< (72)データ引継ぎ開始通知                                           */
    EC_REC_C02_EVT_IT_NEDATCMP_NTC,       /*!< (73)データ引継ぎ完了通知                                           */
    EC_REC_C02_EVT_IT_LNKRCV_NTC,         /*!< (74)CPRI link Recovery完了通知                                     */
    EC_REC_C02_EVT_IT_DLPMIT_REQ,         /*!< (75)(0x86050000)RE file download permission request                */
    EC_REC_C02_EVT_IT_MTPMIT_REQ,         /*!< (76)(0x8A020000)MT data sending permission request                 */
    EC_REC_C02_EVT_IT_L2RELIND,           /*!< (77)データリンク解放表示                                           */
//	EC_REC_C02_EVT_IT_MTADDSET_REQ,		  /*!< (78)(0x4001)RE Forwarding Notice( MTアドレス設定要求 )			  */
	EC_REC_C02_EVT_TO_MTADDSET,       	  /*!< (78)タイムアウト発生通知(MTアドレス設定確認タイマ)                 */
//	EC_REC_C02_EVT_TO_MTADDSET_REC,		  /*!< (80)(0x4001)RE Forwarding Notice(「MTアドレス設定確認タイマ」タイムアウト(REC) )			  */
    EC_REC_C02_EVT_L3_RECRDCNT_RSP,       /*!< (45)CPRI信号受信通知(REカード todo)                                 */
    EC_REC_C02_EVT_MAX,                   /*!< (79)イベントMAX                                                    */
    EC_REC_C02_EVT_NON,                   /*!< (80)イベントなし                                                   */
}   ET_REC_C02_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(C03)
 */
typedef enum {
    EC_REC_C03_EVT_IT_RECRRSTS_RSP,       /*!< (00)REからキャリア状態報告応答受信                                 */
    EC_REC_C03_EVT_IT_TXR_RSP,            /*!< (01)REからTRX解放応答受信                                          */
    EC_REC_C03_EVT_IT_TXS_RSP,            /*!< (02)REからTRX設定応答受信                                          */
    EC_REC_C03_EVT_TO_TXR_CNF,            /*!< (03)REからのTRX解放応答待ちタイムアウト                            */
    EC_REC_C03_EVT_TO_TXS_CNF,            /*!< (04)REからのTRX設定応答待ちタイムアウト                            */
    EC_REC_C03_EVT_L3_TXS_REQ_RSP,        /*!< (05)RECからのTRX設定要求受信                                       */
    EC_REC_C03_EVT_L3_TXR_REQ_RSP,        /*!< (06)RECからのTRX解放要求受信                                       */
    EC_REC_C03_EVT_L3_CARONOFF_REQ_RSP,   /*!< (07)Receive CARONOFF request from REC                             */
    EC_REC_C03_EVT_IT_CARONOFF_RSP,       /*!< (08)Receive CARONOFF response from RE                             */
    EC_REC_C03_EVT_IT_CARLSTS_REQ,
    EC_REC_C03_EVT_IT_CARLSTS_RES,
    EC_REC_C03_EVT_MAX,                   /*!< (09)イベントMAX                                                    */
    EC_REC_C03_EVT_NON                    /*!< (10)イベントなし                                                   */
}   ET_REC_C03_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(C04)
 */
typedef enum {
    EC_REC_C04_EVT_IT_THRINI_REQ,         /*!< (00)スレッド初期化要求                                             */
    EC_REC_C04_EVT_IT_ATHINICMP_NTC,      /*!< (01)全スレッド初期化完了通知                                       */
    EC_REC_C04_EVT_IT_APRINICMP_NTC,      /*!< (02)全プロセス初期化完了通知                                       */
    EC_REC_C04_EVT_IT_NONAPRC_IND,        /*!< (03)無変調キャリアON/OFF指定(自律)(ON)プロシージャ実行指示         */
    EC_REC_C04_EVT_IT_NOFAPRC_IND,        /*!< (04)無変調キャリアON/OFF指定(自律)(ON)プロシージャ実行指示         */
    EC_REC_C04_EVT_L3_NMC_RSP,            /*!< (05)CPRI信号受信通知(無変調キャリアON/OFF指定応答)                 */
    EC_REC_C04_EVT_TO_NMC_CNF,            /*!< (06)タイムアウト発生通知(無変調キャリアON/OFF指定確認タイマ)       */
    EC_REC_C04_EVT_IP_NON_REQ,            /*!< (07)無変調キャリアON/OFF指定(ON)要求                               */
    EC_REC_C04_EVT_IP_NOF_REQ,            /*!< (08)無変調キャリアON/OFF指定(OFF)要求                              */
    EC_REC_C04_EVT_IT_NECHGSTA_REQ,       /*!< (09)N/E切替開始要求                                                */
    EC_REC_C04_EVT_IT_NECHGCMP_NTC,       /*!< (10)N/E切替完了通知                                                */
    EC_REC_C04_EVT_IT_NEDATSTA_REQ,       /*!< (11)データ引継ぎ開始通知                                           */
    EC_REC_C04_EVT_IT_NEDATCMP_NTC,       /*!< (12)データ引継ぎ完了通知                                           */
    EC_REC_C04_EVT_IT_LNKRCV_NTC,         /*!< (13)CPRI link Recovery完了通知                                     */
    EC_REC_C04_EVT_MAX,                   /*!< (14)イベントMAX                                                    */
    EC_REC_C04_EVT_NON                    /*!< (15)イベントなし                                                   */
}   ET_REC_C04_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(C05)
 */
typedef enum {
    EC_REC_C05_EVT_IT_THRINI_REQ,         /*!< (00)スレッド初期化要求                                             */
    EC_REC_C05_EVT_IT_ATHINICMP_NTC,      /*!< (01)全スレッド初期化完了通知                                       */
    EC_REC_C05_EVT_IT_APRINICMP_NTC,      /*!< (02)全プロセス初期化完了通知                                       */
    EC_REC_C05_EVT_IT_CPRSTSNTC_B,        /*!< (03)CPRIリンク状態変化通知(StateB)                                 */
    EC_REC_C05_EVT_IT_CPRSTSNTC_C,        /*!< (04)CPRIリンク状態変化通知(StateC)                                 */
    EC_REC_C05_EVT_IT_CPRSTSNTC_F,        /*!< (05)CPRIリンク状態変化通知(StateF)                                 */
    EC_REC_C05_EVT_TO_RERST_CNF,          /*!< (06)タイムアウト発生通知(REリセット確認タイマ)                     */
    EC_REC_C05_EVT_TO_REFRCRST_CNF,       /*!< (07)タイムアウト発生通知(RE強制リセット確認タイマ)                 */
    EC_REC_C05_EVT_IP_CPRRST_REQ_SW,      /*!< (08)CPRIリンクリセット要求(ソフトウェア)                           */
    EC_REC_C05_EVT_IP_CPRRST_REQ_HW,      /*!< (09)CPRIリンクリセット要求(ハードウェア)                           */
    EC_REC_C05_EVT_IT_NECHGSTA_REQ,       /*!< (10)N/E切替開始要求                                                */
    EC_REC_C05_EVT_IT_NECHGCMP_NTC,       /*!< (11)N/E切替完了通知                                                */
    EC_REC_C05_EVT_IT_NEDATSTA_REQ,       /*!< (12)データ引継ぎ開始通知                                           */
    EC_REC_C05_EVT_IT_NEDATCMP_NTC,       /*!< (13)データ引継ぎ完了通知                                           */
    EC_REC_C05_EVT_MAX,                   /*!< (14)イベントMAX                                                    */
    EC_REC_C05_EVT_NON                    /*!< (15)イベントなし                                                   */
}   ET_REC_C05_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(C06)
 */
typedef enum {
    EC_REC_C06_EVT_IT_THRINI_REQ,         /*!< (00)スレッド初期化要求                                             */
    EC_REC_C06_EVT_IT_ATHINICMP_NTC,      /*!< (01)全スレッド初期化完了通知                                       */
    EC_REC_C06_EVT_IT_APRINICMP_NTC,      /*!< (02)全プロセス初期化完了通知                                       */
    EC_REC_C06_EVT_IT_CPRDSCNTC_E,        /*!< (03)CPRIリンク断通知(StateF未満)                                   */
    EC_REC_C06_EVT_IP_REFIL_IND,          /*!< (04)REファイル更新指示                                             */
    EC_REC_C06_EVT_IT_REFIL_RSP,          /*!< (05)REファイル更新応答                                             */
    EC_REC_C06_EVT_TO_REFIL_CNF,          /*!< (06)タイムアウト発生通知(REリセット確認タイマ)                     */
    EC_REC_C06_EVT_IT_NECHGSTA_REQ,       /*!< (07)N/E切替開始要求                                                */
    EC_REC_C06_EVT_IT_NECHGCMP_NTC,       /*!< (08)N/E切替完了通知                                                */
    EC_REC_C06_EVT_IT_NEDATSTA_REQ,       /*!< (09)データ引継ぎ開始通知                                           */
    EC_REC_C06_EVT_IT_NEDATCMP_NTC,       /*!< (10)データ引継ぎ完了通知                                           */
    EC_REC_C06_EVT_MAX,                   /*!< (11)イベントMAX                                                    */
    EC_REC_C06_EVT_NON                    /*!< (12)イベントなし                                                   */
}   ET_REC_C06_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(C07)
 */
typedef enum {
    EC_REC_C07_EVT_IT_THRINI_REQ,         /*!< (00)スレッド初期化要求                                             */
    EC_REC_C07_EVT_IT_ATHINICMP_NTC,      /*!< (01)全スレッド初期化完了通知                                       */
    EC_REC_C07_EVT_IT_APRINICMP_NTC,      /*!< (02)全プロセス初期化完了通知                                       */
    EC_REC_C07_EVT_IT_CPRSTSNTC_B,        /*!< (03)CPRIリンク状態変化通知(StateB)                                 */
    EC_REC_C07_EVT_TO_RERST_CNF,          /*!< (04)タイムアウト発生通知(REリセット確認タイマ)                     */
    EC_REC_C07_EVT_IP_RERST_REQ,          /*!< (05)REリセット要求                                                 */
    EC_REC_C07_EVT_IT_NECHGSTA_REQ,       /*!< (06)N/E切替開始要求                                                */
    EC_REC_C07_EVT_IT_NECHGCMP_NTC,       /*!< (07)N/E切替完了通知                                                */
    EC_REC_C07_EVT_IT_NEDATSTA_REQ,       /*!< (08)データ引継ぎ開始通知                                           */
    EC_REC_C07_EVT_IT_NEDATCMP_NTC,       /*!< (09)データ引継ぎ完了通知                                           */
    EC_REC_C07_EVT_MAX,                   /*!< (10)イベントMAX                                                    */
    EC_REC_C07_EVT_NON                    /*!< (11)イベントなし                                                   */
}   ET_REC_C07_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(C08)
 */
typedef enum {
    EC_REC_C08_EVT_IT_THRINI_REQ,         /*!< (00)スレッド初期化要求                                             */
    EC_REC_C08_EVT_IT_ATHINICMP_NTC,      /*!< (01)全スレッド初期化完了通知                                       */
    EC_REC_C08_EVT_IT_APRINICMP_NTC,      /*!< (02)全プロセス初期化完了通知                                       */
    EC_REC_C08_EVT_IT_CPRDSC_NTC,         /*!< (03)CPRIリンク断通知                                               */
    EC_REC_C08_EVT_IT_NEG_NTC,            /*!< (04)Negative通知                                                   */
    EC_REC_C08_EVT_IP_RELOGGET_REQ,       /*!< (05)RE 障害ログ取得要求                                            */
    EC_REC_C08_EVT_IP_RELOGSTP_REQ,       /*!< (06)RE 障害ログ中止要求                                            */
    EC_REC_C08_EVT_L3_RELOGGET_RSP,       /*!< (07)CPRI信号受信通知(RE 障害ログ取得応答)                          */
    EC_REC_C08_EVT_L3_RELOGSTP_RSP,       /*!< (08)CPRI信号受信通知(RE 障害ログ中止応答)                          */
    EC_REC_C08_EVT_L3_FILDATSND_NTC,      /*!< (09)CPRI信号受信通知(ファイルデータ送信)                           */
    EC_REC_C08_EVT_L3_FILDATCMP_NTC,      /*!< (10)CPRI信号受信通知(ファイルデータ送信完了)                       */
    EC_REC_C08_EVT_TO_RELOGGET_CNF,       /*!< (11)タイムアウト発生通知(RE 障害ログ取得確認タイマ)                */
    EC_REC_C08_EVT_TO_RELOGSTP_CNF,       /*!< (12)タイムアウト発生通知(RE 障害ログ中止確認タイマ)                */
    EC_REC_C08_EVT_IT_NECHGSTA_REQ,       /*!< (13)N/E切替開始要求                                                */
    EC_REC_C08_EVT_IT_NECHGCMP_NTC,       /*!< (14)N/E切替完了通知                                                */
    EC_REC_C08_EVT_IT_NEDATSTA_REQ,       /*!< (15)データ引継ぎ開始通知                                           */
    EC_REC_C08_EVT_IT_NEDATCMP_NTC,       /*!< (16)データ引継ぎ完了通知                                           */
    EC_REC_C08_EVT_MAX,                   /*!< (17)イベントMAX                                                    */
    EC_REC_C08_EVT_NON                    /*!< (18)イベントなし                                                   */
}   ET_REC_C08_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント(CFF)
 */
typedef enum {
    EC_REC_CFF_EVT_IT_THRINI_REQ,         /*!< (00)スレッド初期化要求                                             */
    EC_REC_CFF_EVT_IT_ATHINICMP_NTC,      /*!< (01)全スレッド初期化完了通知                                       */
    EC_REC_CFF_EVT_IT_APRINICMP_NTC,      /*!< (02)全プロセス初期化完了通知                                       */
    EC_REC_CFF_EVT_IP_TIMCHG_NTC,         /*!< (03)時刻変更通知                                                   */
    EC_REC_CFF_EVT_IP_RECRDCTL_REQ,       /*!< (04)REカード制御要求                                               */
    EC_REC_CFF_EVT_IP_RECRDDIA_REQ,       /*!< (05)REカード診断要求                                               */
    EC_REC_CFF_EVT_IP_REPRTCTL_REQ,       /*!< (06)RE PORT 制御要求                                               */
    EC_REC_CFF_EVT_IP_REVER_REQ,          /*!< (07)RE バージョン報告要求                                          */
    EC_REC_CFF_EVT_L3_RECRDCTL_RSP,       /*!< (08)CPRI信号受信通知(REカード制御応答)                             */
    EC_REC_CFF_EVT_L3_RECRDDIA_RSP,       /*!< (09)CPRI信号受信通知(REカード診断応答)                             */
    EC_REC_CFF_EVT_L3_REPRTCTL_RSP,       /*!< (10)CPRI信号受信通知(RE PORT 制御応答)                             */
    EC_REC_CFF_EVT_L3_REVER_RSP,          /*!< (11)CPRI信号受信通知(REバージョン報告応答)                         */
    EC_REC_CFF_EVT_TO_RECRDCTL_CNF,       /*!< (12)タイムアウト発生通知(REカード制御確認タイマ)                   */
    EC_REC_CFF_EVT_TO_RECRDDIA_CNF,       /*!< (13)タイムアウト発生通知(REカード診断確認タイマ)                   */
    EC_REC_CFF_EVT_TO_REPRTCTL_CNF,       /*!< (14)タイムアウト発生通知(RE PORT 制御確認タイマ)                   */
    EC_REC_CFF_EVT_TO_REVER_CNF,          /*!< (15)タイムアウト発生通知(REバージョン報告確認タイマ)               */
    EC_REC_CFF_EVT_IT_NECHGSTA_REQ,       /*!< (16)N/E切替開始要求                                                */
    EC_REC_CFF_EVT_IT_NECHGCMP_NTC,       /*!< (17)N/E切替完了通知                                                */
    EC_REC_CFF_EVT_IT_NEDATSTA_REQ,       /*!< (18)データ引継ぎ開始通知                                           */
    EC_REC_CFF_EVT_IT_NEDATCMP_NTC,       /*!< (19)データ引継ぎ完了通知                                           */
    EC_REC_CFF_EVT_IT_RESTSCLT_REQ,       /*!< (20)RE status collect request                                      */
    EC_REC_CFF_EVT_MAX,                   /*!< (21)イベントMAX                                                    */
    EC_REC_CFF_EVT_NON                    /*!< (22)イベントなし                                                   */
}   ET_REC_CFF_EVT;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  状態番号(C01)
 */
typedef enum {
    EC_REC_C01_STN_INI, /*!< (00)初期   */
    EC_REC_C01_STN_USE, /*!< (01)運用中 */
    EC_REC_C01_STN_NES, /*!< (02)N/E Switch */
    EC_REC_C01_STN_MAX  /*!< (03)MAX    */
}   ET_REC_C01_STN;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  状態番号(C02)
 */
typedef enum {
    EC_REC_C02_STN_INI,             /*!< (00)初期                                     */
    EC_REC_C02_STN_BEFLPB,          /*!< (01)LAPBリンク確立前                         */
    EC_REC_C02_STN_2ND_RDYING,      /*!< (02)二次リセット準備中                       */
    EC_REC_C02_STN_2ND_RDYCMP,      /*!< (03)二次リセット準備完了                     */
    EC_REC_C02_STN_RES_IDL,         /*!< (04)RE起動待ちアイドル                       */
    EC_REC_C02_STN_RES_L3ECMP,      /*!< (05)RE起動中(L3確立完了)                     */
    EC_REC_C02_STN_RES_RETIMSETCMP, /*!< (06)RE起動中(RE時刻設定完了)                 */
    EC_REC_C02_STN_RES_REPRMING,    /*!< (07)RE起動中(REシステムパラメータ更新中)     */
    EC_REC_C02_STN_RES_REPRMCMP,    /*!< (08)RE起動中(REシステムパラメータ更新完了)   */
    EC_REC_C02_STN_RES_REFILING,    /*!< (09)RE起動中(REファイル更新中)               */
    EC_REC_C02_STN_RES_REFILCMP,    /*!< (10)RE起動中(REファイル更新完了)             */
    EC_REC_C02_STN_RES_TOFVALING,   /*!< (11)RE起動中(Toffset値報告中)                */
    EC_REC_C02_STN_RES_TOFVALCMP,   /*!< (12)RE起動中(Toffset値報告完了)              */
    EC_REC_C02_STN_RES_ACTSETING,   /*!< (13)RE起動中(Active設定中)                   */
    EC_REC_C02_STN_RES_ACTSETCMP,   /*!< (14)RE起動中(Active設定完了)                 */
    EC_REC_C02_STN_RES_REEQPINFING, /*!< (15)RE起動中(RE装置構成情報報告中)           */
    EC_REC_C02_STN_RES_REEQPINFCMP, /*!< (16)RE起動中(RE装置構成情報報告完了)         */
    EC_REC_C02_STN_RES_RESLTINFING, /*!< (17)RE起動中(REスロット情報報告中)           */
    EC_REC_C02_STN_RES_RESLTINFCMP, /*!< (18)RE起動中(REスロット情報報告完了)         */
    EC_REC_C02_STN_RES_RESTSING,    /*!< (19)RE起動中(RE状態報告中)                   */
    EC_REC_C02_STN_RES_RESTSCMP,    /*!< (20)RE起動中(RE状態報告完了)                 */
    EC_REC_C02_STN_RES_REPRTSTSING, /*!< (21)RE起動中(RE PORT状態報告中)              */
    EC_REC_C02_STN_RES_REPRTSTSCMP, /*!< (22)RE起動中(RE PORT状態報告完了)            */
    EC_REC_C02_STN_RES_RECRDSSCING, /*!< (23)RE起動中(REカード状態報告(セクタ部)中)   */
    EC_REC_C02_STN_RES_RECRDSSCCMP, /*!< (24)RE起動中(REカード状態報告(セクタ部)完了) */
    EC_REC_C02_STN_RES_RECRDSCMING, /*!< (25)RE起動中(REカード状態報告(共通部)中)     */
    EC_REC_C02_STN_RES_RECRDSCMCMP, /*!< (26)RE起動中(REカード状態報告(共通部)完了)   */
    EC_REC_C02_STN_RES_RECRRSTSING, /*!< (27)RE起動中(REキャリア状態報告中)           */
    EC_REC_C02_STN_RES_RECRRSTSCMP, /*!< (28)RE起動中(REキャリア状態報告完了)         */
    EC_REC_C02_STN_RES_MTSETFING,   /*!< (29)RE起動中(MTアドレス設定(FLD-MT)中)       */
    EC_REC_C02_STN_RES_MTSETFCMP,   /*!< (30)RE起動中(MTアドレス設定(FLD-MT)完了)     */
    EC_REC_C02_STN_RES_MTSETSING,   /*!< (31)RE起動中(MTアドレス設定(SV-MT)中)        */
    EC_REC_C02_STN_RES_MTSETSCMP,   /*!< (32)RE起動中(MTアドレス設定(SV-MT)完了)      */
    EC_REC_C02_STN_USE,             /*!< (33)運用中                                   */
    EC_REC_C02_STN_USE_IDL,         /*!< (34)運用中アイドル                           */
    EC_REC_C02_STN_RERSTING,        /*!< (35)REリセット中                             */
    EC_REC_C02_STN_REFRCRSTING,     /*!< (36)RE強制リセット中                         */
    EC_REC_C02_STN_FRCSTP,          /*!< (37)強制停止                                 */
    EC_REC_C02_STN_NES,             /*!< (38)N/E切替中                                */
    EC_REC_C02_STN_MAX              /*!< (39)MAX                                      */
}   ET_REC_C02_STN;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  状態番号(C03)
 */
typedef enum {
    EC_REC_C03_STN_IDLE,    /*!< (00)アイドル            */
    EC_REC_C03_STN_TXRAING, /*!< (01)TRX解放(自律)中     */
    EC_REC_C03_STN_TXSAING, /*!< (02)TRX設定(自律)中     */
    EC_REC_C03_STN_MAX      /*!< (03)MAX                 */
}   ET_REC_C03_STN;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  状態番号(C04)
 */
typedef enum {
    EC_REC_C04_STN_INI,     /*!< (00)初期                         */
    EC_REC_C04_STN_NOFCMP,  /*!< (01)無変調キャリアOFF完了        */
    EC_REC_C04_STN_NONRDY,  /*!< (02)無変調キャリアON準備中       */
    EC_REC_C04_STN_NONING,  /*!< (03)無変調キャリアON中           */
    EC_REC_C04_STN_NONARDY, /*!< (04)無変調キャリアON(自律)準備中 */
    EC_REC_C04_STN_NONAING, /*!< (05)無変調キャリアON(自律)中     */
    EC_REC_C04_STN_NONCMP,  /*!< (06)無変調キャリアON完了         */
    EC_REC_C04_STN_NOFRDY,  /*!< (07)無変調キャリアOFF準備中      */
    EC_REC_C04_STN_NOFING,  /*!< (08)無変調キャリアOFF中          */
    EC_REC_C04_STN_NES,     /*!< (09)N/E切替中                    */
    EC_REC_C04_STN_MAX      /*!< (10)MAX                          */
}   ET_REC_C04_STN;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  状態番号(C05)
 */
typedef enum {
    EC_REC_C05_STN_INI,    /*!< (00)初期               */
    EC_REC_C05_STN_RSTIMP, /*!< (01)REリセット不可     */
    EC_REC_C05_STN_FRTPOS, /*!< (02)RE強制リセット可能 */
    EC_REC_C05_STN_RSTPOS, /*!< (03)REリセット可能     */
    EC_REC_C05_STN_RSTING, /*!< (04)REリセット中       */
    EC_REC_C05_STN_FRTING, /*!< (05)RE強制リセット中   */
    EC_REC_C05_STN_NES,    /*!< (06)N/E切替中          */
    EC_REC_C05_STN_MAX     /*!< (07)MAX                */
}   ET_REC_C05_STN;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  状態番号(C06)
 */
typedef enum {
    EC_REC_C06_STN_INI,         /*!< (00)初期                 */
    EC_REC_C06_STN_IDL,         /*!< (01)アイドル             */
    EC_REC_C06_STN_REFILING,    /*!< (02)REファイル更新中     */
    EC_REC_C06_STN_NES,         /*!< (05)N/E切替中            */
    EC_REC_C06_STN_MAX          /*!< (06)MAX                  */
}   ET_REC_C06_STN;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  状態番号(C07)
 */
typedef enum {
    EC_REC_C07_STN_INI,     /*!< (00)初期                 */
    EC_REC_C07_STN_IDL,     /*!< (01)アイドル             */
    EC_REC_C07_STN_RSTING,  /*!< (02)リセット中           */
    EC_REC_C07_STN_NES,     /*!< (03)N/E切替中            */
    EC_REC_C07_STN_MAX      /*!< (04)MAX                  */
}   ET_REC_C07_STN;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  状態番号(C08)
 */
typedef enum {
    EC_REC_C08_STN_INI,     /*!< (00)初期                 */
    EC_REC_C08_STN_IDL,     /*!< (01)アイドル             */
    EC_REC_C08_STN_DAT,     /*!< (02)RE 障害ログ取得中    */
    EC_REC_C08_STN_NES,     /*!< (03)N/E切替中            */
    EC_REC_C08_STN_MAX      /*!< (04)MAX                  */
}   ET_REC_C08_STN;

/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  MTアドレス設定状態
 */
typedef enum {
    EC_REC_MTADDSET_STN_INI,     /*!< (00)初期                 */
    EC_REC_MTADDSET_STN_RUNNING, /*!< (01)MTアドレス設定中     */
    EC_REC_MTADDSET_STN_RESOK,   /*!< (02)応答OK               */
    EC_REC_MTADDSET_STN_RESNG,   /*!< (03)応答NG               */
    EC_REC_MTADDSET_STN_MAX      /*!< (04)MAX                  */
}   ET_REC_MTADDSET_STN;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  状態番号(CFF)
 */
typedef enum {
    EC_REC_CFF_STN_INI, /*!< (00)初期   */
    EC_REC_CFF_STN_USE, /*!< (01)運用中 */
    EC_REC_CFF_STN_NES, /*!< (02)N/E切替中                                */
    EC_REC_CFF_STN_MAX  /*!< (03)MAX    */
}   ET_REC_CFF_STN;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  タイマ種別
 */
typedef enum {
    EC_REC_TMID_MIN,           /*!< MIN (unuse)                              */
    EC_REC_TMID_TOFVAL_CNF,    /*!< Toffset値報告確認タイマ                  */
    EC_REC_TMID_TOFVAL_CNF_END = EC_REC_TMID_TOFVAL_CNF + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
    EC_REC_TMID_REEQPINF_CNF,  /*!< RE装置構成情報報告確認タイマ             */
    EC_REC_TMID_REEQPINF_CNF_END = EC_REC_TMID_REEQPINF_CNF + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
    EC_REC_TMID_RESLTINF_CNF,  /*!< REスロット情報報告確認タイマ             */
    EC_REC_TMID_RESLTINF_CNF_END = EC_REC_TMID_RESLTINF_CNF + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
    EC_REC_TMID_RESTS_INT,     /*!< RE装置構成情報報告送信間隔タイマ                    */
    EC_REC_TMID_RESTS_INT_END = EC_REC_TMID_RESTS_INT + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
	EC_REC_TMID_RESTS_CNF,     /*!< RE状態報告確認タイマ                     */
    EC_REC_TMID_RESTS_CNF_END = EC_REC_TMID_RESTS_CNF + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
    EC_REC_TMID_REPRTSTS_CNF,  /*!< RE PORT状態報告確認タイマ                */
    EC_REC_TMID_REPRTSTS_CNF_END = EC_REC_TMID_REPRTSTS_CNF + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
    EC_REC_TMID_RECRDSSC_INT,  /*!< REカード状態報告(セクタ部)送信間隔タイマ     */
    EC_REC_TMID_RECRDSSC_INT_END = EC_REC_TMID_RECRDSSC_INT + CMD_MAXSLOTNO * D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
	EC_REC_TMID_RECRDSSC_CNF,  /*!< REカード状態報告(セクタ部)確認タイマ     */
    EC_REC_TMID_RECRDSSC_CNF_END = EC_REC_TMID_RECRDSSC_CNF + CMD_MAXSLOTNO * D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
    EC_REC_TMID_RECRRSTS_INT,  /*!< REキャリア状態報告送信間隔タイマ             */
    EC_REC_TMID_RECRRSTS_INT_END = EC_REC_TMID_RECRRSTS_INT + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
	EC_REC_TMID_RECRRSTS_CNF,  /*!< REキャリア状態報告確認タイマ             */
    EC_REC_TMID_RECRRSTS_CNF_END = EC_REC_TMID_RECRRSTS_CNF + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
    EC_REC_TMID_REAUTRST_WAT,  /*!< RE auto reset 送信待ちタイマ             */
    EC_REC_TMID_REAUTRST_WAT_END = EC_REC_TMID_REAUTRST_WAT + CMD_MAXSLOTNO * D_RRH_CPRINO_RE_MAX - CMD_NUM1 ,
	EC_REC_TMID_MT_ADD_SET, 	/*!< MTアドレス設定確認タイマ 				*/
    EC_REC_TMID_MT_ADD_SET_END = EC_REC_TMID_MT_ADD_SET + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
	EC_REC_TMID_RERST_CNF,		/* REリセット確認タイマ */
	EC_REC_TMID_RERST_CNF_END = EC_REC_TMID_RERST_CNF + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
	EC_REC_TMID_REFRCRST_CNF,	/* RE強制リセット確認タイマ */
	EC_REC_TMID_REFRCRST_CNF_END = EC_REC_TMID_REFRCRST_CNF + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
	EC_REC_TMID_2NDRST_WAT,			/* 二次リセット待ちタイマ */
	EC_REC_TMID_2NDRST_WAT_END = EC_REC_TMID_2NDRST_WAT + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
    EC_REC_TMID_TXS_CNF_3G,			/*!< TRX設定確認タイマ (3G)                   */
    EC_REC_TMID_TXS_CNF_3G_END = EC_REC_TMID_TXS_CNF_3G + D_RRH_CPRINO_RE_MAX * CMD_MAX_CARNUM_3G - CMD_NUM1 ,
    EC_REC_TMID_TXS_CNF_S3G,		/*!< TRX設定確認タイマ (S3G)                  */
    EC_REC_TMID_TXS_CNF_S3G_END = EC_REC_TMID_TXS_CNF_S3G + D_RRH_CPRINO_RE_MAX * CMD_MAX_CARNUM_S3G - CMD_NUM1 ,
    EC_REC_TMID_TXR_CNF_3G,			/*!< TRX解放確認タイマ (3G)                   */
    EC_REC_TMID_TXR_CNF_3G_END = EC_REC_TMID_TXR_CNF_3G + D_RRH_CPRINO_RE_MAX * CMD_MAX_CARNUM_3G - CMD_NUM1 ,
    EC_REC_TMID_TXR_CNF_S3G,		/*!< TRX解放確認タイマ (S3G)                  */
    EC_REC_TMID_TXR_CNF_S3G_END = EC_REC_TMID_TXR_CNF_S3G + D_RRH_CPRINO_RE_MAX * CMD_MAX_CARNUM_S3G - CMD_NUM1 ,
    EC_REC_TMID_REPRTSTS_INT,	/*!< RE PORT状態報告送信間隔タイマ */
	EC_REC_TMID_REPRTSTS_INT_END = EC_REC_TMID_REPRTSTS_INT + D_RRH_CPRINO_RE_MAX * CMD_MAX_SYSNUM - CMD_NUM1 ,
	EC_REC_TIMD_10MSEC_INT,		/*	10msec周期タイマ	*/
    EC_REC_TMID_MAX            /*!< MAX                                      */
}   ET_REC_TMID;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  タイマ種別
 */
typedef enum {
    EC_REC_TMK_RETIMSET_INT,  /*!< (00)RE時刻設定送信間隔タイマ                 */
    EC_REC_TMK_TOFVAL_INT,    /*!< (01)Toffset値報告送信間隔タイマ              */
    EC_REC_TMK_TOFVAL_CNF,    /*!< (02)Toffset値報告確認タイマ                  */
    EC_REC_TMK_REEQPINF_INT,  /*!< (03)RE装置構成情報報告送信間隔タイマ         */
    EC_REC_TMK_REEQPINF_CNF,  /*!< (04)RE装置構成情報報告確認タイマ             */
    EC_REC_TMK_RESLTINF_CNF,  /*!< (05)REスロット情報報告確認タイマ             */
    EC_REC_TMK_RESTS_INT,     /*!< (06)RE状態報告送信間隔タイマ                 */
    EC_REC_TMK_RESTS_CNF,     /*!< (07)RE状態報告確認タイマ                     */
    EC_REC_TMK_REPRTSTS_INT,  /*!< (08)RE PORT状態報告送信間隔タイマ            */
    EC_REC_TMK_REPRTSTS_CNF,  /*!< (09)RE PORT状態報告確認タイマ                */
    EC_REC_TMK_RECRDSSC_INT,  /*!< (10)REカード状態報告(セクタ部)送信間隔タイマ */
    EC_REC_TMK_RECRDSSC_CNF,  /*!< (11)REカード状態報告(セクタ部)確認タイマ     */
    EC_REC_TMK_RECRDSCM_INT,  /*!< (12)REカード状態報告(共通部)送信間隔タイマ   */
    EC_REC_TMK_RECRDSCM_CNF,  /*!< (13)REカード状態報告(共通部)確認タイマ       */
    EC_REC_TMK_RECRRSTS_INT,  /*!< (14)REキャリア状態報告送信間隔タイマ         */
    EC_REC_TMK_RECRRSTS_CNF,  /*!< (15)REキャリア状態報告確認タイマ             */
    EC_REC_TMK_RERST_CNF,     /*!< (16)REリセット確認タイマ                     */
    EC_REC_TMK_REFRCRST_CNF,  /*!< (17)RE強制リセット確認タイマ                 */
    EC_REC_TMK_2NDRST_WAT,    /*!< (18)二次リセット待ちタイマ                   */
    EC_REC_TMK_ACTCHG_WAT,    /*!< (19)Active切替指示送信待ちタイマ             */
    EC_REC_TMK_T14RTY_WAT,    /*!< (20)遅延補正値設定待ちタイマ                 */
    EC_REC_TMK_TXS_CNF,       /*!< (21)TRX設定確認タイマ                        */
    EC_REC_TMK_TXR_CNF,       /*!< (22)TRX解放確認タイマ                        */
    EC_REC_TMK_NMC_CNF,       /*!< (23)無変調キャリアON/OFF指定確認タイマ       */
    EC_REC_TMK_REVER_CNF,     /*!< (24)REバージョン報告確認タイマ               */
    EC_REC_TMK_RECRDCTL_CNF,  /*!< (25)REカード制御確認タイマ                   */
    EC_REC_TMK_RECRDDIA_CNF,  /*!< (26)REカード診断確認タイマ                   */
    EC_REC_TMK_REPRTCTL_CNF,  /*!< (27)RE PORT制御確認タイマ                    */
    EC_REC_TMK_RELOGGET_CNF,  /*!< (28)RE 障害ログ取得確認タイマ                */
    EC_REC_TMK_RELOGSTP_CNF,  /*!< (29)RE 障害ログ中止確認タイマ                */
    EC_REC_TMK_RELOGSND_CNF,  /*!< (30)RE 障害ログ送信確認タイマ                */
    EC_REC_TMK_REFILUP_CNF,   /*!< (31)REファイル更新確認タイマ                 */
    EC_REC_TMK_REAUTRST_WAT,  /*!< (32)RE auto reset 送信待ちタイマ             */
    EC_REC_TMK_ACTCHG_WAT2,   /*!< (33)Active切替指示送信待ちタイマ(M-LRE2)     */
    EC_REC_TMK_ACTCHG_WAT3,   /*!< (34)Active切替指示送信待ちタイマ(M-LRE3)     */
    EC_REC_TMK_ACTCHG_WAT4,   /*!< (35)Active切替指示送信待ちタイマ(M-LRE4)     */
    EC_REC_TMK_ACTCHG_WAT5,   /*!< (36)Active切替指示送信待ちタイマ(M-LRE5)     */
    EC_REC_TMK_ACTCHG_WAT6,   /*!< (37)Active切替指示送信待ちタイマ(M-LRE6)     */
    EC_REC_TMK_ACTCHG_WAT7,   /*!< (38)Active切替指示送信待ちタイマ(M-LRE7)     */
	EC_REC_TMK_MT_ADD_SET, 	  /*!< (39)MTアドレス設定確認タイマ 				*/
	EC_REC_TMK_10MSEC_INT,	  /*!< (40)10msec周期タイマ							*/
    EC_REC_TMK_MAX            /*!< (41)MAX                                      */
}   ET_REC_TMK;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  レジスタ種別
 */
typedef enum {
//    EC_REC_CRK_DOF, /*!< (00)上り／下り遅延補正値 1/8CHIP単位 ON/OFF	NE切替転送対象外レジスタ	*/
//    EC_REC_CRK_T14, /*!< (01)T14測定値									NE切替転送対象外レジスタ	*/
//    EC_REC_CRK_UDL, /*!< (02)上り遅延補正								NE切替転送対象レジスタ		*/
//    EC_REC_CRK_DDL, /*!< (03)下り遅延補正								NE切替転送対象レジスタ		*/
//    EC_REC_CRK_CRR, /*!< (04)キャリアON/OFF								NE切替転送対象レジスタ		*/
    EC_REC_CRK_RFR, /*!< (05)RE強制リセット								NE切替転送対象外レジスタ	*/
//    EC_REC_CRK_BAB, /*!< (06)帯域／系統数設定							NE切替転送対象レジスタ		*/
//#if !defined(OPT_CPRI_L2HARD)
//    EC_REC_CRK_DLY, /*!< (07)CA対応 RE送信フレーム位相調整値設定		NE切替転送対象レジスタ		*/
//    EC_REC_CRK_MAX  /*!< (08)MAX */
//#else
//    EC_REC_CRK_MAX  /*!< (07)MAX */
//#endif
    EC_REC_CRK_MAX  /*!< (07)MAX */
}   ET_REC_CRK;

#define D_REC_CRK_NESW_NOTRAS_NUM		3													/*!< NE切替転送対象外レジスタ数 */
#define D_REC_CRK_NESW_TRAS_NUM			(EC_REC_CRK_MAX - D_REC_CRK_NESW_NOTRAS_NUM)		/*!< NE切替転送対象レジスタ数   */

/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  レジスタ種別
 */
typedef enum {
    EC_REC_NRK_ERS, /*!< (00)エラー設定 */
    EC_REC_NRK_ERC, /*!< (01)エラー解除 */
    EC_REC_NRK_CRD, /*!< (02)カード実装の状態を表示 */
    EC_REC_NRK_MON, /*!< (03)T14割り込みマスクON    */
    EC_REC_NRK_MOF, /*!< (04)T14割り込みマスクOFF   */
#if defined(OPT_CPRI_L2HARD)
    EC_REC_NRK_INT, /*!< (05)T14割り込み要因        */
    EC_REC_NRK_MIN, /*!< (06)代表割り込みマスクOFF  */
    EC_REC_NRK_MAX  /*!< (07)MAX */
#else
    EC_REC_NRK_MAX  /*!< (05)MAX */
#endif
}   ET_REC_NRK;
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  ERR種別
 */
typedef enum {
    EC_REC_ERK_DLY, /*!< (00)遅延NG */
    EC_REC_ERK_STP, /*!< (01)RE強制停止 */
    EC_REC_ERK_MAX  /*!< (02)MAX */
}   ET_REC_ERK;
/****************************************************************************/

typedef enum {
	ET_REC_CARD_LINKNODE_ALLCARD,
	ET_REC_CARD_LINKNODE_CMNCARD,
	ET_REC_CARD_LINKNODE_SECCARD,
	ET_REC_CARD_LINKNODE_NUM,
}ET_REC_CARD_LINKNODE;

typedef enum {
	ET_REC_CPRILINK_LINKLIST_ALLCARD,
	ET_REC_CPRILINK_LINKLIST_SECCARD,
	ET_REC_CPRILINK_LINKLIST_NUM,
}ET_REC_CPRILINK_LINKNODE;

typedef enum {
	ET_REC_CPRILINK_PROPLIST_NEW,
	ET_REC_CPRILINK_PROPLIST_CRNT,
	ET_REC_CPRILINK_PROPLIST_NUM,
}ET_REC_CPRILINK_PROPLIST;

typedef enum {
	ET_REC_RE_LINKLIST_CPRILINK,
	ET_REC_RE_LINKLIST_CMNCARD,
	ET_REC_RE_LINKLIST_NUM,
}ET_REC_RE_LINKNODE;

typedef enum {
	ET_REC_ACTLNKGRP_LINKLIST_RE,
	ET_REC_ACTLNKGRP_LINKLIST_NUM,
}ET_REC_ACTLNKGRP_LINKLIST;

#define D_REC_ACTLNKGRP_INDEX_NUM 6
#define D_REC_CPRI_INDEX_NUM	6
#define D_REC_CPRI_INDEX_NUM	6
#define D_REC_CMNFB_NUM			6
#define D_REC_SECFB_NUM			6
#define D_REC_RE_INDEX_NUM		6
#define D_REC_BRC_NUM			4
#define D_REC_CARD_LINKNODE_NUM	3
#define D_REC_CRR_NUM			1
#define D_REC_CARD_INDEX_NUM	96

#define D_REC_RECARDINFO_MASK_CMN			0x007E
#define D_REC_RECARDINFO_MASK_SEC			0x1F80

#define D_REC_FBSTS_SETMODE_APPEND					0
#define D_REC_FBSTS_SETMODE_REMOVE					1

/****************************************************************************/
/*!
 * @brief  タイマ数MAX
 */
#define D_REC_TIM_MAX (\
/*!< (00)RE時刻設定送信間隔タイマ                 */ (CMD_CPRINO_MAX)                 +\
/*!< (01)Toffset値報告送信間隔タイマ              */ (CMD_CPRINO_MAX)                 +\
/*!< (02)Toffset値報告確認タイマ                  */ (CMD_CPRINO_MAX)                 +\
/*!< (03)RE装置構成情報報告送信間隔タイマ         */ (CMD_CPRINO_MAX)                 +\
/*!< (04)RE装置構成情報報告確認タイマ             */ (CMD_CPRINO_MAX)                 +\
/*!< (05)REスロット情報報告確認タイマ             */ (CMD_CPRINO_MAX)                 +\
/*!< (06)RE状態報告送信間隔タイマ                 */ (CMD_CPRINO_MAX)                 +\
/*!< (07)RE状態報告確認タイマ                     */ (CMD_CPRINO_MAX)                 +\
/*!< (08)RE PORT状態報告送信間隔タイマ            */ (CMD_CPRINO_MAX)                 +\
/*!< (09)RE PORT状態報告確認タイマ                */ (CMD_CPRINO_MAX)                 +\
/*!< (10)REカード状態報告(セクタ部)送信間隔タイマ */ (CMD_CPRINO_MAX*CMD_SLOTINF_MAX) +\
/*!< (11)REカード状態報告(セクタ部)確認タイマ     */ (CMD_CPRINO_MAX*CMD_SLOTINF_MAX) +\
/*!< (12)REカード状態報告(共通部)送信間隔タイマ   */ (CMD_CPRINO_MAX*CMD_SLOTINF_MAX) +\
/*!< (13)REカード状態報告(共通部)確認タイマ       */ (CMD_CPRINO_MAX*CMD_SLOTINF_MAX) +\
/*!< (14)REキャリア状態報告送信間隔タイマ         */ (CMD_CPRINO_MAX*CMD_CARNO_MAX)   +\
/*!< (15)REキャリア状態報告確認タイマ             */ (CMD_CPRINO_MAX*CMD_CARNO_MAX)   +\
/*!< (16)REリセット確認タイマ                     */ (CMD_CPRINO_MAX)                 +\
/*!< (17)RE強制リセット確認タイマ                 */ (CMD_CPRINO_MAX)                 +\
/*!< (18)二次リセット待ちタイマ                   */ (CMD_NUM1)                       +\
/*!< (19)Active切替指示送信待ちタイマ             */ (CMD_CPRINO_MAX)                 +\
/*!< (20)遅延補正値設定待ちタイマ                 */ (CMD_CPRINO_MAX)                 +\
/*!< (21)TRX設定確認タイマ                        */ (CMD_CPRINO_MAX*CMD_CARNO_MAX)   +\
/*!< (22)TRX解放確認タイマ                        */ (CMD_CPRINO_MAX*CMD_CARNO_MAX)   +\
/*!< (23)無変調キャリアON/OFF指定確認タイマ       */ (CMD_CPRINO_MAX*CMD_CARNO_MAX)   +\
/*!< (24)REバージョン報告確認タイマ               */ (CMD_CPRINO_MAX*CMD_SLOTINF_MAX) +\
/*!< (25)REカード制御確認タイマ                   */ (CMD_CPRINO_MAX*CMD_SLOTINF_MAX) +\
/*!< (26)REカード診断確認タイマ                   */ (CMD_CPRINO_MAX*CMD_SLOTINF_MAX) +\
/*!< (27)RE PORT制御確認タイマ                    */ (CMD_CPRINO_MAX)                 +\
/*!< (28)RE 障害ログ取得確認タイマ                */ (CMD_CPRINO_MAX)                 +\
/*!< (29)RE 障害ログ中止確認タイマ                */ (CMD_CPRINO_MAX)                 +\
/*!< (30)RE 障害ログ送信確認タイマ                */ (CMD_CPRINO_MAX)				  +\
/*!< (34)MTアドレス設定確認タイマ 				  */ (CMD_CPRINO_MAX))

/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  初期化種別
 */
#define D_REC_INK_INI 0 /*!< 初期化       */
#define D_REC_INK_DSC 1 /*!< CPRIリンク断 */
#define D_REC_INK_SHC 2 /*!< RE起動中HC OK  */
#define D_REC_INK_UHC 3 /*!< 運用中アイドル状態HC OK  */
#define D_REC_INK_NOA 4 /*!< No Active    */
#define D_REC_INK_RNG 5 /*!< Report NG */
#define D_REC_INK_RTO 6 /*!< 報告確認T.O. */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  Active状態種別
 */
#define D_REC_ACK_ACTCMP 0 /*!< Active       */
#define D_REC_ACK_NEGCMP 1 /*!< Negative     */
#define D_REC_ACK_ACTING 2 /*!< Active設定中 */
/****************************************************************************/
/*!
 * @brief  RE状態管理テーブル通知要因種別
 */
#define D_REC_RIK_DMY 0 /*!< ダミー */
#define D_REC_RIK_DSC 1 /*!< CPRIリンク断時 */
#define D_REC_RIK_CON 2 /*!< -  */
#define D_REC_RIK_NOA 3 /*!< No Active    */
#define D_REC_RIK_LOK 4 /*!< RE status collect response (CPRI link connect) */
#define D_REC_RIK_LNG 5 /*!< RE status collect response (CPRI link disconnect) */
/****************************************************************************/
/*!
 * @brief  RE種別
 */
#define D_REC_RETYP_ALL    0  /*! BRE.RRE.LRE */
#define D_REC_RETYP_REC    1  /*! REC  */
#define D_REC_RETYP_AMP    2  /*! BRE  */
#define D_REC_RETYP_OFTRX  3  /*! RRE/LRE  */
/****************************************************************************/
/*!
 * @brief  RE状態管理テーブル初期値情報
 */
#define D_REC_RETBLINI_SLOTNO  0xFFFF  /*! スロット番号(65535)  */
#define D_REC_RETBLINI_EQUINF  0xFFFF  /*! 装置構成情報(65535)  */
#define D_REC_RETBLINI_CNTCODE 0xFFFF  /*! 制御コード(65535)    */
/****************************************************************************/
/*!
 * @brief  RE状態管理テーブル(Function Block)
 */
#define D_REC_FBNUM_MAX   29  /*! Function block max number  */
#define D_REC_FBTYP_MAX   28  /*! Function block max type    */
#define D_REC_FBTYP_MIN   0   /*! Function block min type    */
#define D_REC_TRBSTS_MAX  3   /*! Function block trouble status */
/****************************************************************************/
/*!
 * @brief  RE状態管理テーブル(送信アンテナ数)
 */
#define D_REC_BRCNO_MIN     0   /*! Branch No min */
#define D_REC_BRCNO_MAX     7   /*! Branch No max */
#define D_REC_BRCNUM_MIN    0   /*! Branch number min */
#define D_REC_BRCNUM_MAX    8   /*! Branch number max */
/****************************************************************************/
/*!
 * @brief  RE装置構成情報報告(アンテナ数)
 */
#define D_REC_ANTNUM_MIN    1   /*! Antena number min */
#define D_REC_ANTNUM_MAX    8   /*! Antena number max */
/****************************************************************************/
/*!
 * @brief  REキャリア状態ALM要因種別
 */
#define D_REC_CRR_ALM_DSC 1 /*!< CPRIリンク断 */
#define D_REC_CRR_ALM_TNG 2 /*!< Auto TRX set/rel retry NG */
#define D_REC_CRR_ALM_RNG 3 /*!< Carrier report NG */
#define D_REC_CRR_ALM_RTO 4 /*!< Carrier report TO*/
/****************************************************************************/
/*!
 * @brief RE Trouble Use Data
 */
#define D_REC_FILE_TRBLOG    1 /*! RE Trouble */
#define D_REC_LOGSIZ_MAX  4060 /*! RE Log Package max size to OAMPF */
/****************************************************************************/
/*!
 * @brief CPRI link reset NG detail
 */
#define D_REC_CPRRST_CPRLNKDSC  1    /*! CPRI link disconnect */
#define D_REC_CPRRST_UNDPROC    2    /*! Reset under doing    */
#define D_REC_CPRRST_TRATO      3    /*! Reset confirm T.O.   */
/****************************************************************************/
/*!
 * @brief 遅延補正値設定結果
 */
#define D_REC_DLYOFT_SET  0    /*! 設定完了 */
#define D_REC_DLYOFT_DLY  1    /*! 設定遅延 */
/****************************************************************************/
/*!
 * @brief  レジスタアドレス
 */
#define D_REC_CRA_OFFSET    0x00000800 /*!< Register offset between links */

#if !defined(OPT_CPRI_L2HARD)
#define D_REC_CRA_DOF_CPR01 (0x00201020+(CMD_NUM0*D_REC_CRA_OFFSET)) /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI01) */
#define D_REC_CRA_DOF_CPR02 (0x00201020+(CMD_NUM1*D_REC_CRA_OFFSET)) /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI02) */
#define D_REC_CRA_DOF_CPR03 (0x00201020+(CMD_NUM2*D_REC_CRA_OFFSET)) /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI03) */
#define D_REC_CRA_DOF_CPR04 (0x00201020+(CMD_NUM3*D_REC_CRA_OFFSET)) /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI04) */
#define D_REC_CRA_DOF_CPR05 (0x00201020+(CMD_NUM4*D_REC_CRA_OFFSET)) /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI05) */
#define D_REC_CRA_DOF_CPR06 (0x00201020+(CMD_NUM5*D_REC_CRA_OFFSET)) /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI06) */
#else
#define D_REC_CRA_DOF_CPR01 D_CMM_FPGA_ADDR_T14_ONOFF_1_LTE /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI01) *//*	0x00102004	*/
#define D_REC_CRA_DOF_CPR02 D_CMM_FPGA_ADDR_T14_ONOFF_2_LTE /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI02) *//*	0x00102104	*/
#define D_REC_CRA_DOF_CPR03 0x00000000 /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI03) */
#define D_REC_CRA_DOF_CPR04 0x00000000 /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI04) */
#define D_REC_CRA_DOF_CPR05 0x00000000 /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI05) */
#define D_REC_CRA_DOF_CPR06 0x00000000 /* 上り／下り遅延補正値 1/8CHIP単位 ON/OFF (CPRI06) */
#endif

#if !defined(OPT_CPRI_L2HARD)
#define D_REC_CRA_T14_CPR01 (0x002010E0+(CMD_NUM0*D_REC_CRA_OFFSET)) /*!< T14測定値(CPRI01) */
#define D_REC_CRA_T14_CPR02 (0x002010E0+(CMD_NUM1*D_REC_CRA_OFFSET)) /*!< T14測定値(CPRI02) */
#define D_REC_CRA_T14_CPR03 (0x002010E0+(CMD_NUM2*D_REC_CRA_OFFSET)) /*!< T14測定値(CPRI03) */
#define D_REC_CRA_T14_CPR04 (0x002010E0+(CMD_NUM3*D_REC_CRA_OFFSET)) /*!< T14測定値(CPRI04) */
#define D_REC_CRA_T14_CPR05 (0x002010E0+(CMD_NUM4*D_REC_CRA_OFFSET)) /*!< T14測定値(CPRI05) */
#define D_REC_CRA_T14_CPR06 (0x002010E0+(CMD_NUM5*D_REC_CRA_OFFSET)) /*!< T14測定値(CPRI06) */
#else
#define D_REC_CRA_T14_CPR01 D_CMM_FPGA_ADDR_T14_STATE_1 /*!< T14測定値(CPRI01) *//*	0x00102000	*/
#define D_REC_CRA_T14_CPR02 D_CMM_FPGA_ADDR_T14_STATE_2 /*!< T14測定値(CPRI02) *//*	0x00102100	*/
#define D_REC_CRA_T14_CPR03 0x00000000 /*!< T14測定値(CPRI03) */
#define D_REC_CRA_T14_CPR04 0x00000000 /*!< T14測定値(CPRI04) */
#define D_REC_CRA_T14_CPR05 0x00000000 /*!< T14測定値(CPRI05) */
#define D_REC_CRA_T14_CPR06 0x00000000 /*!< T14測定値(CPRI06) */
#endif

#if !defined(OPT_CPRI_L2HARD)
#define D_REC_CRA_UDL_CPR01 (0x00201028+(CMD_NUM0*D_REC_CRA_OFFSET)) /*!< 上り遅延補正値(CPRI01) */
#define D_REC_CRA_UDL_CPR02 (0x00201028+(CMD_NUM1*D_REC_CRA_OFFSET)) /*!< 上り遅延補正値(CPRI02) */
#define D_REC_CRA_UDL_CPR03 (0x00201028+(CMD_NUM2*D_REC_CRA_OFFSET)) /*!< 上り遅延補正値(CPRI03) */
#define D_REC_CRA_UDL_CPR04 (0x00201028+(CMD_NUM3*D_REC_CRA_OFFSET)) /*!< 上り遅延補正値(CPRI04) */
#define D_REC_CRA_UDL_CPR05 (0x00201028+(CMD_NUM4*D_REC_CRA_OFFSET)) /*!< 上り遅延補正値(CPRI05) */
#define D_REC_CRA_UDL_CPR06 (0x00201028+(CMD_NUM5*D_REC_CRA_OFFSET)) /*!< 上り遅延補正値(CPRI06) */
#else
#define D_REC_CRA_UDL_CPR01 D_CMM_FPGA_ADDR_T14_HOSEI_1_UL_LTE /*!< 上り遅延補正値(CPRI01) *//*	0x0010200C	*/
#define D_REC_CRA_UDL_CPR02 D_CMM_FPGA_ADDR_T14_HOSEI_2_UL_LTE /*!< 上り遅延補正値(CPRI02) *//*	0x0010210C	*/
#define D_REC_CRA_UDL_CPR03 0x00000000 /*!< 上り遅延補正値(CPRI03) */
#define D_REC_CRA_UDL_CPR04 0x00000000 /*!< 上り遅延補正値(CPRI04) */
#define D_REC_CRA_UDL_CPR05 0x00000000 /*!< 上り遅延補正値(CPRI05) */
#define D_REC_CRA_UDL_CPR06 0x00000000 /*!< 上り遅延補正値(CPRI06) */
#endif

#if !defined(OPT_CPRI_L2HARD)
#define D_REC_CRA_DDL_CPR01 (0x00201024+(CMD_NUM0*D_REC_CRA_OFFSET)) /*!< 下り遅延補正値(CPRI01) */
#define D_REC_CRA_DDL_CPR02 (0x00201024+(CMD_NUM1*D_REC_CRA_OFFSET)) /*!< 下り遅延補正値(CPRI02) */
#define D_REC_CRA_DDL_CPR03 (0x00201024+(CMD_NUM2*D_REC_CRA_OFFSET)) /*!< 下り遅延補正値(CPRI03) */
#define D_REC_CRA_DDL_CPR04 (0x00201024+(CMD_NUM3*D_REC_CRA_OFFSET)) /*!< 下り遅延補正値(CPRI04) */
#define D_REC_CRA_DDL_CPR05 (0x00201024+(CMD_NUM4*D_REC_CRA_OFFSET)) /*!< 下り遅延補正値(CPRI05) */
#define D_REC_CRA_DDL_CPR06 (0x00201024+(CMD_NUM5*D_REC_CRA_OFFSET)) /*!< 下り遅延補正値(CPRI06) */
#else
#define D_REC_CRA_DDL_CPR01 D_CMM_FPGA_ADDR_T14_HOSEI_1_DL_LTE /*!< 下り遅延補正値(CPRI01) *//*	0x00102008	*/
#define D_REC_CRA_DDL_CPR02 D_CMM_FPGA_ADDR_T14_HOSEI_2_DL_LTE /*!< 下り遅延補正値(CPRI02) *//*	0x00102108	*/
#define D_REC_CRA_DDL_CPR03 0x00000000 /*!< 下り遅延補正値(CPRI03) */
#define D_REC_CRA_DDL_CPR04 0x00000000 /*!< 下り遅延補正値(CPRI04) */
#define D_REC_CRA_DDL_CPR05 0x00000000 /*!< 下り遅延補正値(CPRI05) */
#define D_REC_CRA_DDL_CPR06 0x00000000 /*!< 下り遅延補正値(CPRI06) */
#endif

#if !defined(OPT_CPRI_L2HARD)
#define D_REC_CRA_CRR_CPR01 (0x0020109C+(CMD_NUM0*D_REC_CRA_OFFSET)) /*!< キャリアON/OFF設定(CPRI01) */
#define D_REC_CRA_CRR_CPR02 (0x0020109C+(CMD_NUM1*D_REC_CRA_OFFSET)) /*!< キャリアON/OFF設定(CPRI02) */
#define D_REC_CRA_CRR_CPR03 (0x0020109C+(CMD_NUM2*D_REC_CRA_OFFSET)) /*!< キャリアON/OFF設定(CPRI03) */
#define D_REC_CRA_CRR_CPR04 (0x0020109C+(CMD_NUM3*D_REC_CRA_OFFSET)) /*!< キャリアON/OFF設定(CPRI04) */
#define D_REC_CRA_CRR_CPR05 (0x0020109C+(CMD_NUM4*D_REC_CRA_OFFSET)) /*!< キャリアON/OFF設定(CPRI05) */
#define D_REC_CRA_CRR_CPR06 (0x0020109C+(CMD_NUM5*D_REC_CRA_OFFSET)) /*!< キャリアON/OFF設定(CPRI06) */
#else
#define D_REC_CRA_CRR_CPR01 D_CMM_FPGA_ADDR_CPRI_CR_ONOFF_1_LTE /*!< キャリアON/OFF設定(CPRI01) *//*	0x00102400	*/
#define D_REC_CRA_CRR_CPR02 D_CMM_FPGA_ADDR_CPRI_CR_ONOFF_2_LTE /*!< キャリアON/OFF設定(CPRI02) *//*	0x00102600	*/
#define D_REC_CRA_CRR_CPR03 0x00000000 /*!< キャリアON/OFF設定(CPRI03) */
#define D_REC_CRA_CRR_CPR04 0x00000000 /*!< キャリアON/OFF設定(CPRI04) */
#define D_REC_CRA_CRR_CPR05 0x00000000 /*!< キャリアON/OFF設定(CPRI05) */
#define D_REC_CRA_CRR_CPR06 0x00000000 /*!< キャリアON/OFF設定(CPRI06) */
#endif

#define D_REC_CRA_RFR_CPR01 (0x81001080+(CMD_NUM0*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI01) */
#define D_REC_CRA_RFR_CPR02 (0x81001080+(CMD_NUM1*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI02) */
#define D_REC_CRA_RFR_CPR03 (0x81001080+(CMD_NUM2*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI03) */
#define D_REC_CRA_RFR_CPR04 (0x81001080+(CMD_NUM3*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI04) */
#define D_REC_CRA_RFR_CPR05 (0x81001080+(CMD_NUM4*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI05) */
#define D_REC_CRA_RFR_CPR06 (0x81001080+(CMD_NUM5*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI06) */
#define D_REC_CRA_RFR_CPR07 (0x81001080+(CMD_NUM6*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI07) */
#define D_REC_CRA_RFR_CPR08 (0x81001080+(CMD_NUM7*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI08) */
#define D_REC_CRA_RFR_CPR09 (0x81001080+(CMD_NUM8*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI09) */
#define D_REC_CRA_RFR_CPR10 (0x81001080+(CMD_NUM9*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI10) */
#define D_REC_CRA_RFR_CPR11 (0x81001080+(CMD_NUM10*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI11) */
#define D_REC_CRA_RFR_CPR12 (0x81001080+(CMD_NUM11*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI12) */
#define D_REC_CRA_RFR_CPR13 (0x81001080+(CMD_NUM12*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI13) */
#define D_REC_CRA_RFR_CPR14 (0x81001080+(CMD_NUM13*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI14) */
#define D_REC_CRA_RFR_CPR15 (0x81001080+(CMD_NUM14*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI15) */
#define D_REC_CRA_RFR_CPR16 (0x81001080+(CMD_NUM15*D_REC_CRA_OFFSET)) /*!< RE強制リセット(CPRI16) */

#if !defined(OPT_CPRI_L2HARD)
#define D_REC_CRA_BAB_CPR01 (0x002010D0+(CMD_NUM0*D_REC_CRA_OFFSET)) /*!< 帯域／系統数設定(CPRI01) */
#define D_REC_CRA_BAB_CPR02 (0x002010D0+(CMD_NUM1*D_REC_CRA_OFFSET)) /*!< 帯域／系統数設定(CPRI02) */
#define D_REC_CRA_BAB_CPR03 (0x002010D0+(CMD_NUM2*D_REC_CRA_OFFSET)) /*!< 帯域／系統数設定(CPRI03) */
#define D_REC_CRA_BAB_CPR04 (0x002010D0+(CMD_NUM3*D_REC_CRA_OFFSET)) /*!< 帯域／系統数設定(CPRI04) */
#define D_REC_CRA_BAB_CPR05 (0x002010D0+(CMD_NUM4*D_REC_CRA_OFFSET)) /*!< 帯域／系統数設定(CPRI05) */
#define D_REC_CRA_BAB_CPR06 (0x002010D0+(CMD_NUM5*D_REC_CRA_OFFSET)) /*!< 帯域／系統数設定(CPRI06) */
#else
#define D_REC_CRA_BAB_CPR01 D_CMM_FPGA_ADDR_CPRI_ANT_1_LTE /*!< 帯域／系統数設定(CPRI01) *//*	0x00102408	*/
#define D_REC_CRA_BAB_CPR02 D_CMM_FPGA_ADDR_CPRI_ANT_2_LTE /*!< 帯域／系統数設定(CPRI02) *//*	0x00102608	*/
#define D_REC_CRA_BAB_CPR03 0x00000000 /*!< 帯域／系統数設定(CPRI03) */
#define D_REC_CRA_BAB_CPR04 0x00000000 /*!< 帯域／系統数設定(CPRI04) */
#define D_REC_CRA_BAB_CPR05 0x00000000 /*!< 帯域／系統数設定(CPRI05) */
#define D_REC_CRA_BAB_CPR06 0x00000000 /*!< 帯域／系統数設定(CPRI06) */
#endif

#if !defined(OPT_CPRI_L2HARD)
#define D_REC_CRA_DLY_CPR01 (0x002010F0+(CMD_NUM0*D_REC_CRA_OFFSET)) /*!< CA対応 RE送信フレーム位相調整値設定(CPRI01) */
#define D_REC_CRA_DLY_CPR02 (0x002010F0+(CMD_NUM1*D_REC_CRA_OFFSET)) /*!< CA対応 RE送信フレーム位相調整値設定(CPRI02) */
#define D_REC_CRA_DLY_CPR03 (0x002010F0+(CMD_NUM2*D_REC_CRA_OFFSET)) /*!< CA対応 RE送信フレーム位相調整値設定(CPRI03) */
#define D_REC_CRA_DLY_CPR04 (0x002010F0+(CMD_NUM3*D_REC_CRA_OFFSET)) /*!< CA対応 RE送信フレーム位相調整値設定(CPRI04) */
#define D_REC_CRA_DLY_CPR05 (0x002010F0+(CMD_NUM4*D_REC_CRA_OFFSET)) /*!< CA対応 RE送信フレーム位相調整値設定(CPRI05) */
#define D_REC_CRA_DLY_CPR06 (0x002010F0+(CMD_NUM5*D_REC_CRA_OFFSET)) /*!< CA対応 RE送信フレーム位相調整値設定(CPRI06) */
#endif




#if !defined(OPT_CPRI_L2HARD)
#define D_REC_NRA_ERS       0x00001028   /*!< ERR設定 */
#define D_REC_NRA_ERC       0x00001094   /*!< ERR解除 */
#define D_REC_NRA_CRD       0x00002164   /*!< カード実装新状態表示 */
#define D_REC_NRA_MON       0x00200034   /*!< T14割り込みマスクON  */
#define D_REC_NRA_MOF       0x00200038   /*!< T14割り込みマスクOFF */
#else
#define D_REC_NRA_ERS       D_CMM_FPGA_ADDR_SOFT_ERR_ON_4_2   /*!< ERR設定 */
#define D_REC_NRA_ERC       D_CMM_FPGA_ADDR_SOFT_ERR_OFF_4_2   /*!< ERR解除 */
#define D_REC_NRA_CRD       0x00000000   /*!< カード実装新状態表示 */
#define D_REC_NRA_MON       D_CMM_FPGA_ADDR_T14_INT_MSK_ON_LTE   /*!< T14割り込みマスクON  */
#define D_REC_NRA_MOF       D_CMM_FPGA_ADDR_T14_INT_MSK_OFF_LTE   /*!< T14割り込みマスクOFF */
#endif

/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  レジスタ値
 * @note   上り／下り遅延補正値1/8CHIP単位ON
 */
#define D_REC_RGV_DOF_AON 0x00000011 /*!< 上り／下り遅延補正値1/8CHIP単位ON 設定 */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  レジスタ値
 * @note   T14測定値Status
 */
#if defined(OPT_CPRI_L2HARD)
#define D_REC_RGV_T14STS_MSK 0x00030000 /*!< T14 Status Mask  */
#define D_REC_RGV_T14VAL_MSK 0x0000FFFF /*!< T14 Value  Mask  */
#define D_REC_RGV_T14STS_OK  0x00020000 /*!< T14測定正常(OK)  */
#else
#define D_REC_RGV_T14STS_MSK    0x30000000 /*!< T14 Status Mask  */
#define D_REC_RGV_T14VAL32_MSK  0x00FFFF00 /*!< T14 Value  Mask(1/32chip)  */
#define D_REC_RGV_T14VAL640_MSK 0x0000001F /*!< T14 Value  Mask(1/640chip)  */
#define D_REC_RGV_T14STS_OK     0x10000000 /*!< T14測定正常(OK)  */
#define D_REC_RGV_T14STS_OVER   0x30000000 /*!< T14測定オーバ    */
#define D_REC_RGV_T14VAL32_SFT  8

#define D_REC_REG_T14VAL_OVER   2048
#endif
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  レジスタ値
 * @note   CA対応 RE送信フレーム位相調整値設定
 */
#if !defined(OPT_CPRI_L2HARD)
#define D_REC_RGV_DL_RE_P           0x00000000
#define D_REC_RGV_DL_RE_M           0x01000000
#define D_REC_RGV_DL_RE_DLY_FLG_ON  0x10000000
#define D_REC_RGV_DL_RE_DLY_FLG_OFF 0x00000000

#define D_REC_RGV_MUX_DL_RE_DLY_FLG_OFF 0x0000
#define D_REC_RGV_MUX_DL_RE_DLY_FLG_ON  0x8000
#define D_REC_RGV_MUX_DL_RE_M           0x8000
#define D_REC_RGV_MUX_DL_RE_P           0x0000

#define D_REC_DL_RE_DLY_MAX                128

#endif
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  レジスタ値
 * @note   キャリアON／OFF設定レジスタ
 */
#define D_REC_RGV_CRR_ON  0x00000001  /*!< キャリアリソース確保 */
#define D_REC_RGV_CRR_OF  0x00000000  /*!< キャリアリソース解放 */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  レジスタ値
 * @note   RE強制リセットレジスタ
 */
#define D_REC_RGV_RST_ON  0x00000001  /*!< リセット設定 */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  CPRIMUX カード実装新状態表示マシケ値
 * @note   CPRIMUX カード実装新状態表示マシケ
 */
#define D_REC_RGV_MUXINSTAL_MSK1  0x02000000  /*!< CPRIMUX#1 カード実装新状態表示マシケ */
#define D_REC_RGV_MUXINSTAL_MSK2  0x01000000  /*!< CPRIMUX#2 カード実装新状態表示マシケ */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  T14割り込みマスク値
 * @note   T14割り込みマスク
 */
#if !defined(OPT_CPRI_L2HARD)
#define D_REC_RGV_INT_MSK   0x0000003F  /*!< T14割り込みマスク  */
#else
#define D_REC_RGV_INT_MSK   0x00000003  /*!< T14割り込みマスク  */
#endif
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  CNT ERR2 register(REC use)
 * @note   Toffset delay ERR, RE force stop ERR
 */
#define D_REC_RGV_ERR2      0x3F888888  /*!< CNT ERR2 register value mask */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  帯域／系統数設定マスク値
 * @note   帯域／系統数設定マスク
 */
#define D_REC_RGV_BRCSPEC_MSK  0xFFFC  /* Branch Specification mask */
#define D_REC_RGV_BRCSPEC_00   0x0002  /* Brc#0 ON , Brc#1 OFF      */
#define D_REC_RGV_BRCSPEC_01   0x0001  /* Brc#0 OFF, Brc#1 ON       */
#define D_REC_RGV_BRCSPEC_02   0x0003  /* Brc#0 ON , Brc#1 ON       */


#define D_REC_RGV_SYSBAND_MSK  0xFFFC  /* System band mask          */
#define D_REC_RGV_SYSBAND_05   0x0000  /* System band  5MHz         */
#define D_REC_RGV_SYSBAND_10   0x0001  /* System band 10MHz         */
#define D_REC_RGV_SYSBAND_15   0x0002  /* System band 15MHz         */
#define D_REC_RGV_SYSBAND_20   0x0003  /* System band 20MHz         */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  ERR要因
 */
#if !defined(OPT_CPRI_L2HARD)
#define D_REC_ERF_DLY_CPR01  0x00000008  /*!< CPRI#01遅延NG */
#define D_REC_ERF_DLY_CPR02  0x00000080  /*!< CPRI#02遅延NG */
#define D_REC_ERF_DLY_CPR03  0x00000800  /*!< CPRI#03遅延NG */
#define D_REC_ERF_DLY_CPR04  0x00008000  /*!< CPRI#04遅延NG */
#define D_REC_ERF_DLY_CPR05  0x00080000  /*!< CPRI#05遅延NG */
#define D_REC_ERF_DLY_CPR06  0x00800000  /*!< CPRI#06遅延NG */
#else
#define D_REC_ERF_DLY_CPR01  0x00000002  /*!< CPRI#01遅延NG *//*	ERR要因2	*/
#define D_REC_ERF_DLY_CPR02  0x00020000  /*!< CPRI#02遅延NG *//*	ERR要因18	*/
#define D_REC_ERF_DLY_CPR03  0x00000000  /*!< CPRI#03遅延NG */
#define D_REC_ERF_DLY_CPR04  0x00000000  /*!< CPRI#04遅延NG */
#define D_REC_ERF_DLY_CPR05  0x00000000  /*!< CPRI#05遅延NG */
#define D_REC_ERF_DLY_CPR06  0x00000000  /*!< CPRI#06遅延NG */
#endif

#if !defined(OPT_CPRI_L2HARD)
#define D_REC_ERF_STP_CPR01  0x01000000  /*!< CPRI#1 強制停止状態 */
#define D_REC_ERF_STP_CPR02  0x02000000  /*!< CPRI#2 強制停止状態 */
#define D_REC_ERF_STP_CPR03  0x04000000  /*!< CPRI#3 強制停止状態 */
#define D_REC_ERF_STP_CPR04  0x08000000  /*!< CPRI#4 強制停止状態 */
#define D_REC_ERF_STP_CPR05  0x10000000  /*!< CPRI#5 強制停止状態 */
#define D_REC_ERF_STP_CPR06  0x20000000  /*!< CPRI#6 強制停止状態 */
#else
#define D_REC_ERF_STP_CPR01  0x00000004  /*!< CPRI#1 強制停止状態 *//*	ERR要因3	*/
#define D_REC_ERF_STP_CPR02  0x00040000  /*!< CPRI#2 強制停止状態 *//*	ERR要因19	*/
#define D_REC_ERF_STP_CPR03  0x00000000  /*!< CPRI#3 強制停止状態 */
#define D_REC_ERF_STP_CPR04  0x00000000  /*!< CPRI#4 強制停止状態 */
#define D_REC_ERF_STP_CPR05  0x00000000  /*!< CPRI#5 強制停止状態 */
#define D_REC_ERF_STP_CPR06  0x00000000  /*!< CPRI#6 強制停止状態 */
#endif

/****************************************************************************/

/****************************************************************************/
#define D_REC_REOBJ_IDX_INVALID		65535
#define D_REC_CPRIOBJ_IDX_INVALID	65535

/****************************************************************************/

/****************************************************************************/
#define D_REC_FBTYP_SEC			0
#define D_REC_FBTYP_CMN			1
#define D_REC_FBTYP_INVALID		65535

/****************************************************************************/

/****************************************************************************/
#define D_REC_FBIDX_DUP			00
#define D_REC_FBIDX_LNA			01
#define D_REC_FBIDX_TPA			02
#define D_REC_FBIDX_TRX			03
#define D_REC_FBIDX_TRXINF		04
#define D_REC_FBIDX_3GRFINF		05

#define D_REC_FBIDX_OARAINF		00
#define D_REC_FBIDX_EXOARAINF	01
#define D_REC_FBIDX_TILTINF		02
#define D_REC_FBIDX_PORTINF		03
#define D_REC_FBIDX_MTIF		04
#define D_REC_FBIDX_CLKINF		05

#define D_REC_FBIDX_INVALID		65535


/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  機能部種別
 */
#define D_REC_FBTYP_DUP			00
#define D_REC_FBTYP_LNA			01
#define D_REC_FBTYP_TPA			02
#define D_REC_FBTYP_TRX			03
#define D_REC_FBTYP_TRXINF		04
#define D_REC_FBTYP_OARAINF		17
#define D_REC_FBTYP_TILTINF		18
#define D_REC_FBTYP_PORTINF		19
#define D_REC_FBTYP_MTINF		20
#define D_REC_FBTYP_3GRFINF		25
#define D_REC_FBTYP_EXOARAINF	26
#define D_REC_FBTYP_CLKINF		27
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  機能部の故障状態
 */
#define D_REC_FBTRB_UNMOUNT  0      /*!< 未実装   */
#define D_REC_FBTRB_NORMAL   1      /*!< Normal   */
#define D_REC_FBTRB_HIGHALM  2      /*!< High-ALM */
#define D_REC_FBTRB_SYSALM   3      /*!< SYS-ALM  */
#define D_REC_FBTRB_NG       4      /*!< NG       */
#define D_REC_FBTRB_INVALID  65535  /*!< 無効     */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  機能部の故障状態 Bit
 */
#define D_REC_FBTRB_BIT_NG			0x0001	/*!< NG       */
#define D_REC_FBTRB_BIT_NORMAL		0x0002	/*!< Normal   */
#define D_REC_FBTRB_BIT_HIGHALM		0x0004	/*!< High-ALM */
#define D_REC_FBTRB_BIT_SYSALM		0x0008	/*!< SYS-ALM  */
#define D_REC_FBTRB_BIT_UNMOUNT		0x0010	/*!< 未実装   */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント変換CPRI番号
 */
#define D_REC_CPRINO_NON 0x0000 /*!< なし */
#define D_REC_CPRINO_ALL 0xFFFF /*!< 全て */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント変換キャリア番号
 */
#define D_REC_CARNO_NON 0x0000 /*!< なし */
#define D_REC_CARNO_ALL 0xFFFF /*!< 全て */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  イベント変換キャリア番号
 */
#define D_REC_CARNO_NON 0x0000 /*!< なし */
#define D_REC_CARNO_ALL 0xFFFF /*!< 全て */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  Active切替指示送信待ちタイマ値
 */
#define D_REC_ACTCHG_TIMVAL_MIN   10 /*!< Active切替指示送信待ちタイマ値MIN */
#define D_REC_ACTCHG_TIMVAL_MAX 1000 /*!< Active切替指示送信待ちタイマ値MAX */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  二次リセット待ちタイマ値
 */
#define D_REC_2NDRST_TIMVAL D_TCM_2NDRST_TIMVAL
                               /*!< (Sec)二次リセット待ちタイマ値 */

#define D_REC_2NDRST_STATC  1

/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  遅延補正値設定待ちタイマ値
 */
#define D_REC_T14RTY_TIMVAL 10 /*!< 遅延補正値設定待ちタイマ値 */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief Timer End Code
 */
 #define D_REC_TIM_NG  1       /*!< Timer Start/Stop NG      */
 #define D_REC_TIM_IV  2       /*!< Timer Invalid            */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief RE card status table update result
 */
 #define D_REC_OTRSTS_CHG  1      /*!< Other status changed  */
 #define D_REC_RFBSTS_CHG  2      /*!< Card and function block status changed  */
 #define D_REC_RFBSTS_CHG_MULTI 6
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  無効値
 */
#define D_REC_CPR_IV 0x0000 /*!< CPRI番号無効 */
#define D_REC_SLT_IV 0x0000 /*!< スロット番号無効 */
#define D_REC_CRR_IV 0x0000 /*!< キャリア番号無効 */
#define D_REC_BRC_IV 0xFFFF /*!< 系番号無効 */
#define D_REC_SCT_IV 0xFFFF /*!< セクタ番号無効 */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  パラメータテーブルアクセス
 */
#define D_REC_C02_CPR_NO() f_trp_rec_Wc02_prm_tbl.cpr_no /*!< (C02)CPRI番号 */
#define D_REC_C02_3G_CPR_NO() f_trp_rec_Wc02_prm_tbl_3g.cpr_no /*!< (C02)CPRI番号 */
#define D_REC_C03_CPR_NO() f_trp_rec_Wc03_prm_tbl.cpr_no /*!< (C03)CPRI番号 */
#define D_REC_C03_CRR_NO() f_trp_rec_Wc03_prm_tbl.crr_no /*!< (C03)キャリア番号 */
#define D_REC_C04_CPR_NO() f_trp_rec_Wc04_prm_tbl.cpr_no /*!< (C04)CPRI番号 */
#define D_REC_C04_CRR_NO() f_trp_rec_Wc04_prm_tbl.crr_no /*!< (C04)キャリア番号 */
#define D_REC_C05_CPR_NO() f_trp_rec_Wc05_prm_tbl.cpr_no /*!< (C05)CPRI番号 */
#define D_REC_C06_CPR_NO() f_trp_rec_Wc06_prm_tbl.cpr_no /*!< (C06)CPRI番号 */
#define D_REC_C07_CPR_NO() f_trp_rec_Wc07_prm_tbl.cpr_no /*!< (C07)CPRI番号 */
#define D_REC_C08_CPR_NO() f_trp_rec_Wc08_prm_tbl.cpr_no /*!< (C08)CPRI番号 */

#define D_REC_C03_3G_CPR_NO() f_trp_rec_Wc03_prm_tbl_3g.cpr_no /*!< (C03)CPRI番号 */
#define D_REC_C03_3G_CRR_NO() f_trp_rec_Wc03_prm_tbl_3g.crr_no /*!< (C03)キャリア番号 */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  CPRI番号<->Activeリンクグループ変換
 */
#define D_REC_C02_ACTLNK_GR() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc02_prm_tbl.cpr_no - CMD_NUM1].actlnk_gr)
#define D_REC_C03_ACTLNK_GR() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc03_prm_tbl.cpr_no - CMD_NUM1].actlnk_gr)
#define D_REC_C04_ACTLNK_GR() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc04_prm_tbl.cpr_no - CMD_NUM1].actlnk_gr)
#define D_REC_C05_ACTLNK_GR() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc05_prm_tbl.cpr_no - CMD_NUM1].actlnk_gr)
#define D_REC_C06_ACTLNK_GR() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc06_prm_tbl.cpr_no - CMD_NUM1].actlnk_gr)
#define D_REC_C07_ACTLNK_GR() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc07_prm_tbl.cpr_no - CMD_NUM1].actlnk_gr)
#define D_REC_C08_ACTLNK_GR() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc08_prm_tbl.cpr_no - CMD_NUM1].actlnk_gr)
#define D_REC_CPRI_TO_ACTLNK_CNV(cpr_no) (f_trp_com_WSyspAmpConnMngTbl[(cpr_no) - CMD_NUM1].actlnk_gr)
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  CPRI番号<->REタイプ(BRE or RRE/LRE or M-LRE)変換
 */
#define D_REC_C02_RE_SYS_TYPE() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc02_prm_tbl.cpr_no - CMD_NUM1].sys_re_type)
#define D_REC_C03_RE_SYS_TYPE() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc03_prm_tbl.cpr_no - CMD_NUM1].sys_re_type)
#define D_REC_C04_RE_SYS_TYPE() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc04_prm_tbl.cpr_no - CMD_NUM1].sys_re_type)
#define D_REC_C05_RE_SYS_TYPE() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc05_prm_tbl.cpr_no - CMD_NUM1].sys_re_type)
#define D_REC_C06_RE_SYS_TYPE() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc06_prm_tbl.cpr_no - CMD_NUM1].sys_re_type)
#define D_REC_C07_RE_SYS_TYPE() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc07_prm_tbl.cpr_no - CMD_NUM1].sys_re_type)
#define D_REC_C08_RE_SYS_TYPE() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc08_prm_tbl.cpr_no - CMD_NUM1].sys_re_type)
#define D_REC_CPRI_TO_RE_SYS_TYPE_CNV(cpr_no) (f_trp_com_WSyspAmpConnMngTbl[(cpr_no) - CMD_NUM1].sys_re_type)
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  CPRI番号<->REシリアルINDEX変換
 */
#define D_REC_C02_RE_SERIAL_IDX() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc02_prm_tbl.cpr_no - CMD_NUM1].re_serial_idx)
#define D_REC_C03_RE_SERIAL_IDX() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc03_prm_tbl.cpr_no - CMD_NUM1].re_serial_idx)
#define D_REC_C04_RE_SERIAL_IDX() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc04_prm_tbl.cpr_no - CMD_NUM1].re_serial_idx)
#define D_REC_C05_RE_SERIAL_IDX() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc05_prm_tbl.cpr_no - CMD_NUM1].re_serial_idx)
#define D_REC_C06_RE_SERIAL_IDX() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc06_prm_tbl.cpr_no - CMD_NUM1].re_serial_idx)
#define D_REC_C07_RE_SERIAL_IDX() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc07_prm_tbl.cpr_no - CMD_NUM1].re_serial_idx)
#define D_REC_C08_RE_SERIAL_IDX() (f_trp_com_WSyspAmpConnMngTbl[f_trp_rec_Wc08_prm_tbl.cpr_no - CMD_NUM1].re_serial_idx)
#define D_REC_CPRI_TO_RE_SERIAL_IDX_CNV(cpr_no) (f_trp_com_WSyspAmpConnMngTbl[(cpr_no) - CMD_NUM1].re_serial_idx)
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  単位変換
 */
#if !defined(OPT_CPRI_L2HARD)
#define D_REC_UNICNV_0001TO9600 9600 /* 1/   1から1/9600への変換 */
#define D_REC_UNICNV_0002TO9600 4800 /* 1/   2から1/9600への変換 */
#define D_REC_UNICNV_0004TO9600 2400 /* 1/   4から1/9600への変換 */
#define D_REC_UNICNV_0006TO9600 1600 /* 1/   6から1/9600への変換 */
#define D_REC_UNICNV_0008TO9600 1200 /* 1/   8から1/9600への変換 */
#define D_REC_UNICNV_0032TO9600  300 /* 1/  32から1/9600への変換 */
#define D_REC_UNICNV_0640TO9600   15 /* 1/ 640から1/9600への変換 */
#define D_REC_UNICNV_0100TO9600   96 /* 1/ 100から1/9600への変換 */
#define D_REC_UNICNV_0032TO0640   20 /* 1/  32から1/ 640への変換 */
#else
#define D_REC_UNICNV_0001TO4800 4800 /* 1/   1から1/4800への変換 */
#define D_REC_UNICNV_0002TO4800 2400 /* 1/   2から1/4800への変換 */
#define D_REC_UNICNV_0004TO4800 1200 /* 1/   4から1/4800への変換 */
#define D_REC_UNICNV_0006TO4800  800 /* 1/   6から1/4800への変換 */
#define D_REC_UNICNV_0008TO4800  600 /* 1/   8から1/4800への変換 */
#define D_REC_UNICNV_0032TO4800  150 /* 1/  32から1/4800への変換 */
#define D_REC_UNICNV_0100TO4800   48 /* 1/ 100から1/4800への変換 */
#endif
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  システム帯域幅
 */
#define D_REC_SYSBND_05MHZ 1 /*  5MHZ */
#define D_REC_SYSBND_10MHZ 2 /* 10MHZ */
#define D_REC_SYSBND_15MHZ 3 /* 15MHZ */
#define D_REC_SYSBND_20MHZ 4 /* 20MHZ */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief SDM side For RE file download
 */
#define D_REC_SDM_ACT 0 /*  ACT Side */
#define D_REC_SDM_A   1 /*  A   Side */
#define D_REC_SDM_B   2 /*  B   Side */
/****************************************************************************/


/****************************************************************************/
/*!
 * @brief  RUNHIS
 */
#define D_REC_RUNHIS_BSIZE   24   /*!< ボディサイズ */
#define D_REC_RUNHIS_NUM     512  /*!< 面数 */
#define D_REC_RUNHIS_RHK_RCV 0x00 /*!< 受信 */
#define D_REC_RUNHIS_RHK_SND 0x01 /*!< 送信 */
#define D_REC_RUNHIS_RHK_SNG 0x0F /*!< 送信NG */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  REGHIS
 */
#define D_REC_REGHIS_NUM     512 /*!< 面数 */
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  ログコマンド
 */
#define D_REC_LOGTBL_NUM 24 /*!< ログテーブル数 */
#define D_REC_LOGTBL_NUM_RUNHIS 21
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  RE Auto Reset information
 */
#define D_REC_REAUTORSTINFO_NONE       0
#define D_REC_REAUTORSTINFO_WAITRST    1
#define D_REC_REAUTORSTINFO_WAITRSTART 2
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  ビット設定
 */
#define D_REC_SETBIT(field, mask, onoff) {\
            if (onoff == 0) {field &= ~mask;} \
            else            {field |=  mask;} \
        }
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  ビット取得
 */
#define D_REC_GETBIT(field, mask) \
            ((field & mask)/mask)
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  バイト設定
 */
#define D_REC_SETBYTE(field, pos, dat) {\
            (*(((char*)(&field))+pos)) = dat; \
        }
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  バイト取得
 */
#define D_REC_GETBYTE(field, pos) \
            (*(((char*)(&field))+pos))
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  BCDコード変換
 */
#define D_REC_CNVBCD(hex) (\
    ((((hex)/10000000)   )<<28)+ \
    ((((hex)/ 1000000)%10)<<24)+ \
    ((((hex)/  100000)%10)<<20)+ \
    ((((hex)/   10000)%10)<<16)+ \
    ((((hex)/    1000)%10)<<12)+ \
    ((((hex)/     100)%10)<< 8)+ \
    ((((hex)/      10)%10)<< 4)+ \
    ((((hex)         )%10)    )  \
)
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  インクリメントループバック
 */
#define D_REC_INCLBK(dat, max) {\
    dat++;                     \
    if (dat >= max) {dat = 0;} \
}
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  デクリメントループバック
 */
#define D_REC_DECLBK(dat, max) {\
    if (dat == 0) {dat = max-1;} \
    else          {dat--;      } \
}
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  Invalid log 初期化
 */
#define D_REC_IVLINI() {\
    f_trp_com_Fmemclr(&f_trp_rec_Wcom_ivlprm_tbl, sizeof(f_trp_rec_Wcom_ivlprm_tbl));\
}
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  Invalid log 出力データ設定
 */
#define D_REC_IVLSET(bindata) {\
    if((f_trp_rec_Wcom_ivlprm_tbl.index + sizeof(bindata)) <= 48){\
        f_trp_com_Fmemcpy(\
            &f_trp_rec_Wcom_ivlprm_tbl.data[f_trp_rec_Wcom_ivlprm_tbl.index],\
            &(bindata),\
            sizeof(bindata)\
        );\
        f_trp_rec_Wcom_ivlprm_tbl.index += sizeof(bindata);\
    }else {\
        f_trp_com_Fmemcpy(\
            &f_trp_rec_Wcom_ivlprm_tbl.data[f_trp_rec_Wcom_ivlprm_tbl.index],\
            &(bindata),\
            (48 - f_trp_rec_Wcom_ivlprm_tbl.index)\
        );\
        f_trp_rec_Wcom_ivlprm_tbl.index = 48;\
    }\
}
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  Invalid log 出力データ設定(サイズ指定)
 */
#define D_REC_IVLSET_SIZE(bindatp, size) {\
    if((f_trp_rec_Wcom_ivlprm_tbl.index + size) <= 48){\
        f_trp_com_Fmemcpy(\
            &f_trp_rec_Wcom_ivlprm_tbl.data[f_trp_rec_Wcom_ivlprm_tbl.index],\
            bindatp,\
            size\
        );\
        f_trp_rec_Wcom_ivlprm_tbl.index += size;\
    }else{\
        f_trp_com_Fmemcpy(\
            &f_trp_rec_Wcom_ivlprm_tbl.data[f_trp_rec_Wcom_ivlprm_tbl.index],\
            bindatp,\
            (48 - f_trp_rec_Wcom_ivlprm_tbl.index)\
        );\
        f_trp_rec_Wcom_ivlprm_tbl.index = 48;\
    }\
}
/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  Invalid log 出力
 */
#ifdef DEBUG_TRACE
#define D_REC_IVLOUT(level, string) {\
	TRACE(" TRIF(REC):t_rec_assert: %s(%d) ####", string, level);\
    F_COMF_CMM_FINVDLOG(\
        level,\
        string,\
        f_trp_rec_Wcom_ivlprm_tbl.data,\
        f_trp_rec_Wcom_ivlprm_tbl.index\
    );\
}
#else
#define D_REC_IVLOUT(level, string) {\
    F_COMF_CMM_FINVDLOG(\
        level,\
        string,\
        f_trp_rec_Wcom_ivlprm_tbl.data,\
        f_trp_rec_Wcom_ivlprm_tbl.index\
    );\
}
#endif
/****************************************************************************/


#define D_REC_BRC2BRCNUM(brc) {\
	if (brc == 1)		brc = 1; \
	else if (brc <= 3)	brc = 2; \
	else if (brc <= 7)	brc = 3; \
	else if (brc <= 15)	brc = 4; \
}


#define	D_REC_SAVEKIND_TIMEOUT		1
#define	D_REC_SAVEKIND_TIMESTOPNG	2
#define	D_REC_SAVECOUNT_MAX			16
#define D_REC_MSGKIND_REQ			1
#define D_REC_MSGKIND_RSP			2

#define	D_REC_LOGKIND_MAX			2
#define	D_REC_LOGKIND_CARRIER		0
#define	D_REC_LOGKIND_CARD			1


#define D_REC_RECRDSTSREQ_RTNUM		(3)		/* RE カード状態報告応答 保護段数(3回)		*/
#define D_REC_RECARSTSREQ_RTNUM		(3)		/* RE キャリア状態報告応答 保護段数(3回)	*/
#define D_REC_RESTSREQ_RTNUM		(3)		/* RE 状態報告応答 保護段数(3回)			*/
#define D_REC_REPRTSTSREQ_RTNUM		(3)		/* RE PORT状態報告応答 保護段数(3回)		*/
#define D_REC_REEQPINREQF_RTNUM		(3)		/* RE 装置構成情報報告応答 保護段数(INVLOG用)	*/

#define EC_REC_C03_TRXREL_RTRY_TIME	7		/* キャリア状態不一致補正保護段数 */
#define EC_REC_C03_TRXSET_RTRY_TIME	7

/****************************************************************************/
/*!
 * @brief  ブランチ番号 Bit
 */
#define D_REC_BRCINF_BIT_0			0x0001	/*!< Brc No.0  */
#define D_REC_BRCINF_BIT_1			0x0002	/*!< Brc No.1  */
#define D_REC_BRCINF_BIT_2			0x0004	/*!< Brc No.2  */
#define D_REC_BRCINF_BIT_3			0x0008	/*!< Brc No.3  */

/****************************************************************************/
	
/****************************************************************************/
/*!
 * @brief  MTアドレス設定開始要因
 */
#define D_MT_ADD_SET_FACTOR_REC			1	/*!< NEC  */
#define D_MT_ADD_SET_FACTOR_FHM			2	/*!< FHM  */

/****************************************************************************/

/****************************************************************************/
/*!
 * @brief  MTアドレス設定要求(REC主導)再送回数最大値
 */
#define D_MT_ADD_SET_RETRY_MAX			3

/****************************************************************************/

/* @} */  /* group TRIF_REC */

#endif
/****************************************************************************/
/*
 */
/****************************************************************************/
