/*!
 * @skip  $ld:$
 * @file  f_dpda_typ.h
 * @brief DPDAタスク 構造体宣言
 * @date  2011/07/26 ALPHA)横山 Create
 * @date  2011/09/20 ALPHA)藤井 SRRU-2.1G対応
 * @date  2011/11/02 ALPHA)藤井 M-RRU-ZSYS-00703 対処
 * @date  2011/11/04 ALPHA)横山 M-RRU-ZSYS-00715 対処
 * @date  2011/11/07 ALPHA)横山 M-RRU-ZSYS-00716 対処
 * @date  2011/11/15 ALPHA)藤井 M-RRU-ZSYS-00721 対処(アラーム仕様変更)
 * @date  2011/11/15 ALPHA)藤井 M-RRU-ZSYS-00734 対処
 * @date  2011/11/30 ALPHA)藤井 M-RRU-ZSYS-00726 対処
 * @date  2011/12/06 ALPHA)横山 SRRU-2.1G-LTE対応
 * @date  2012/03/06 ALPHA)横山 N21GLTE対応
 * @date  2012/04/10 ALPHA)高橋 N21GLTE M-RRU-ZSYS-00842対応(ハソ09版)
 * @date  2012/05/09 ALPHA)藤井 M-RRU-ZSYS-00871対処
 * @date  2015/04/22 ALPHA)近間 RRH-013-000 DCM_3.5G対応
 * @date  2015/06/19 ALPHA)近間 RRH-007-000 DCM_3.5G対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2015
 */

/***************************************************************************************************************************//**
 *
 * @addtogroup RRH_PF_RFP
 * @{
 *
 ******************************************************************************************************************************/
#ifndef F_DPDA_TYP_H
#define F_DPDA_TYP_H

/******************************************************************************************************************************/
/** register data table
 *  @note ※この構造体を修正するときはAlarm History Log用テーブル(T_COM_S_MEASREG_DATA)も修正すること
 *  @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 */
/******************************************************************************************************************************/
typedef struct{
    UINT regVal[D_RRH_ALMHIS_REG_VAL_NUM];                              /**< alarm his log用レジスタリード値                  */
}T_DPDA_REGDATA;

/******************************************************************************************************************************/
/** status information RPT table
 *  @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 *  @date 2015/08/07 TDI)satou 「RSSI」削除
 *  @date 2015/08/07 TDI)satou 「アンテナ電力[dBm]」削除
 *  @date 2015/08/07 TDI)satou 「FB電力」削除
 *  @date 2015/08/07 TDI)satou 「TSSI」削除
 *  @date 2015/08/07 TDI)satou 「Rx Power」削除
 */
/******************************************************************************************************************************/
typedef struct{
    INT Temperature;                                                    /**< Temperature                                      */
}T_DPDA_STATUS_INFO;

/******************************************************************************************************************************/
/*
 * TaskInfoテーブル
 */
/******************************************************************************************************************************/
/**
 * TaskInfoテーブル(T_DPDA_FLG).
 * @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 * @date 2015/08/07 TDI)satou 「FB電力計算処理フラグ」削除
 * @date 2015/08/07 TDI)satou 「送信APC実施制御フラグ」削除
 */
typedef struct{
    USHORT temperature;                                                 /**< 温度補償実施フラグ                               */
    USHORT dbgmode;                                                     /**< TPデバッグ用                                     */
}T_DPDA_FLG;

/**
 * TaskInfoテーブル(T_DPDA_SW_DBG_FLG).
 * @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 * @date 2015/08/07 TDI)satou 「debug time flg」削除
 * @date 2015/08/07 TDI)satou 「レジスタ設定値取得要否フラグ」削除
 * @date 2015/08/07 TDI)satou 「1秒周期処理Skipフラグ」削除
 */
typedef struct{
    USHORT virtualRegisterFlg;                                          /**< 擬似レジスタ参照フラグ                           */
}T_DPDA_SW_DBG_FLG;

/**
 * TaskInfoテーブル(T_DPDA_INFO).
 * @note CAUTION : 本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 * @date 2015/08/07 TDI)satou 「Report Subscribe情報」削除
 * @date 2015/08/07 TDI)satou 「FSINFO(1秒前)」削除
 * @date 2015/08/07 TDI)satou 「System parameter情報」削除
 * @date 2015/08/07 TDI)satou 「高速モード状態」削除
 * @date 2015/08/07 TDI)satou 「高速モードカウンタ」削除
 * @date 2015/08/07 TDI)satou 「LUT State」削除
 * @date 2015/08/07 TDI)satou 「TxGain係数」削除
 * @date 2015/08/07 TDI)satou 「0振幅時ローカルリーク補正状態」削除
 * @date 2015/08/07 TDI)satou 「0振幅時ローカルリーク補正カウント」削除
 * @date 2015/08/07 TDI)satou 「キャリブレーション制御番号」削除
 * @date 2015/08/07 TDI)satou 「Abort状態」削除
 * @date 2015/08/07 TDI)satou 「キャリブレーション状態」削除
 * @date 2015/08/07 TDI)satou 「0振幅補正実行状態」削除
 * @date 2015/08/07 TDI)satou 「装置送信状態時のQEC(Quadrature Error Correction：直交誤差補正)およびLo Calibration実行状態」削除
 * @date 2015/08/07 TDI)satou 「Counter」削除
 */
typedef struct{
    UINT taskState;                                                     /**< Task State                                       */
    T_DPDA_FLG flg;                                                     /**< Flag                                             */
    T_DPDA_SW_DBG_FLG swDbgFlg;                                         /**< Debug Flag                                       */
    USHORT almMaskEep;                                                  /**< AlarmMask(EEPROM値)                              */
}T_DPDA_INFO;

/******************************************************************************************************************************/
/** DPDAデバッグログ情報保持テーブル
 *  @note 本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 *  @date 2015/08/07 TDI)satou 「デバッグ用 DL FSINFO」削除
 *  @date 2015/08/07 TDI)satou 「レジスタログ全取得フラグ」削除
 *  @date 2015/08/07 TDI)satou 「FW補正用Gain」削除
 *  @date 2015/08/07 TDI)satou 「温度補償RxGain調整値」削除
 *  @date 2015/08/07 TDI)satou 「キャリアモードフラグ(tp用)」削除
 */
/******************************************************************************************************************************/
typedef struct{
    T_DPDA_INFO taskInfo;                                               /**< dpda task information table                      */
    UINT almThreshold[E_DPDA_ALM_THRESH_NUM_MAX][D_DPDA_ANTMAX];        /**< 閾値                                             */
    UINT virtualRegData[D_DPDA_DBG_VIRTUAL_REG_CNT];                    /**< 仮想レジスタデータ格納テーブル                   */
    UINT startUpTime;                                                   /**< 装置起動時間                                     */
    UINT almmsk;                                                        /**< アラームマスク用(tp用)                           */
}T_DPDA_INFO_ALL;

/******************************************************************************************************************************/
/** dpda alarm state table
 *  @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 */
/******************************************************************************************************************************/
typedef struct{
    USHORT almState;                                                    /**< アラーム状態                                     */
    USHORT almStateNow;                                                 /**< アラーム状態(今回周期)                           */
    USHORT almOccurNum1sec;                                             /**< アラーム発生回数                                 */
    USHORT almClearNum1sec;                                             /**< アラーム回復回数                                 */
    USHORT almOccurNumTotal;                                            /**< アラーム発生累計                                 */
    USHORT almClearNumTotal;                                            /**< アラーム回復累計                                 */
    UINT almOccurNumTotal1sec;                                          /**< アラーム発生累計(1秒周期)                        */
    UINT almClearNumTotal1sec;                                          /**< アラーム回復累計(1秒周期)                        */
}T_DPDA_ALM_STATE;

/******************************************************************************************************************************/
/** DPDA Alarm Factor Information
 *
 */
/******************************************************************************************************************************/
typedef struct
{
    UINT mergeOperator;                                                 /**< アラーム要因マージ演算子                        */
    UINT* jdgInfo1;                                                     /**< 判定情報1                                       */
    UINT* jdgInfo2;                                                     /**< 判定情報2                                       */
    UINT jdgOperator;                                                   /**< 判定演算子                                      */
}T_DPDA_ALM_FACTOR;

/******************************************************************************************************************************/
/** DPDA Alarm Information
 *  @date 2015/08/07 TDI)satou 「PA on 時の回復条件」削除
 *  @date 2015/08/07 TDI)satou 「PA off 時の回復条件」削除
 *  @date 2015/08/07 TDI)satou 「キャリア off時の回復条件」削除
 */
/******************************************************************************************************************************/
typedef struct
{
    CHAR Name[32];                                                      /**< アラーム名称                                     */
    T_DPDA_ALM_FACTOR almFactor[D_DPDA_ALM_FACTOR_NUM];                 /**< アラーム要因                                     */
    USHORT numOfAlmFactor;                                              /**< アラーム要因数                                   */
    USHORT almOccurNum;                                                 /**< アラーム発生回数                                 */
    USHORT almClearNum;                                                 /**< アラーム回復回数                                 */
    USHORT noClearFlag;                                                 /**< アラーム回復なしフラグ                           */
    UINT instanceId;                                                    /**< Instance ID                                      */
}T_DPDA_ALM_INFO;

/******************************************************************************************************************************/
/** Soft側保持情報テーブル
 *  @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 *  @date 2015/08/07 TDI)satou 「SoftState情報」削除
 */
/******************************************************************************************************************************/
typedef struct{
    T_DPDA_INFO dpdaInfo;                                               /**< DPDAタスク情報                                   */
    T_DPDA_STATUS_INFO statusInfo;                                      /**< Status Info                                      */
}T_DPDA_SOFT_INFO;

/******************************************************************************************************************************/
/** Hard側保持情報テーブル
 *  @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 */
/******************************************************************************************************************************/
typedef struct{
    T_DPDA_REGDATA RegData;                                             /**< レジスタデータ                                   */
}T_DPDA_HARD_INFO;

/******************************************************************************************************************************/
/** アラーム解析用ログ情報テーブル
 *  @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 */
/******************************************************************************************************************************/
typedef struct{
    T_RRH_SYSTEM_TIME time;                                             /**< 時刻情報                                         */
    T_DPDA_SOFT_INFO soft_info;                                         /**< ソフト側保持情報                                 */
    T_DPDA_HARD_INFO hard_info;                                         /**< ハード側保持情報                                 */
}T_DPDA_ALM_LOG_INFO;

/******************************************************************************************************************************/
/** アラーム発生時解析用ログテーブル
 *  @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 */
/******************************************************************************************************************************/
typedef struct{
    UINT DataPosi;                                                      /**< 次回格納位置                                     */
    UINT almLogNum;                                                     /**< 格納済みアラームログ数                           */
    UINT almThresholdInfo[E_DPDA_ALM_THRESH_NUM_MAX][D_DPDA_ANTMAX];    /**< Alm閾値情報(Alm情報は1面1log)                    */
    T_DPDA_ALM_STATE almStateInfo[D_DPDA_ALM_NUM];                      /**< AlmState情報(Alm情報は1面1log)                   */
    T_DPDA_ALM_LOG_INFO alm_log_info[D_DPDA_ALM_LOG_MAX];               /**< アラーム解析用ログ情報                           */
}T_DPDA_ALM_LOG;

/******************************************************************************************************************************/
/** アラームログ共有メモリ参照用テーブル
 *  @note CAUTION：本構造体を修正する場合はf_comr_shmmemCreMngにある共有メモリサイズを見直すこと
 */
/******************************************************************************************************************************/
typedef struct{
    UINT almLogFace;                                                    /**< アラームログ収集面                               */
    UINT rrhKind;                                                       /**< 装置種別                                         */
    UINT dummy[2];                                                      /**< dummy                                            */
    T_DPDA_ALM_LOG almLog[D_DPDA_ALM_LOG_FACE];                         /**< アラーム発生時解析用ログtable                    */
}T_DPDA_ALM_LOG_DEBUG;

/******************************************************************************************************************************/
/** alm threshold information table(for rom)
 *
 */
/******************************************************************************************************************************/
typedef struct{
    USHORT AntNum;                                                      /**< アンテナ数                                       */
    USHORT CarNum;                                                      /**< キャリア数                                       */
    CHAR Name[32];                                                      /**< 閾値名称                                         */
    UINT Threshold;                                                     /**< 閾値(初期値)                                     */
}T_DPDA_ALM_THRESHOLD_INFO;

/******************************************************************************************************************************/
/** alm mask check table(for rom)
 *
 */
/******************************************************************************************************************************/
typedef struct{
    UINT almmsk;                                                        /**< アラームマスク                                   */
    UINT dbgmode;                                                       /**< デバッグモード                                   */
} T_DPDA_ALM_MSK_CHECK;

#endif
/***************************************************************************************************************************//**
 *
 * @} addtogroup RRH_PF_RFP
 *
 ******************************************************************************************************************************/
