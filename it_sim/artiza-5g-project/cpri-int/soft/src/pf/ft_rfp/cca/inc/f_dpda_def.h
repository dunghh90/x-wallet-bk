/*!
 * @skip  $ld:$
 * @file  f_dpda_def.h
 * @brief DPDAタスク デファイン定義.
 * @date  2011/07/26 ALPHA)横山 Create
 * @date  2011/08/23 ALPHA)藤井 dpda IT1 No.1対処
 * @date  2011/08/26 ALPHA)藤井 dpda IT1 No.3対処
 * @date  2011/09/29 ALPHA)藤井 SRRU-2.1G-EVDO対応
 * @date  2011/10/20 ALPHA)藤井 M-RRU-ZSYS-00693対処
 * @date  2011/11/02 ALPHA)藤井 M-RRU-ZSYS-00703 対処
 * @date  2011/11/08 ALPHA)横山 M-RRU-ZSYS-00721 対処
 * @date  2011/11/15 ALPHA)藤井 M-RRU-ZSYS-00721 対処(アラーム仕様変更)
 * @date  2011/11/18 ALPHA)藤井 M-RRU-ZSYS-00734 対処
 * @date  2011/11/23 ALPHA)藤井 M-RRU-ZSYS-00739 対処
 * @date  2011/11/25 ALPHA)横山 M-RRU-ZSYS-00739 対処
 * @date  2011/11/28 ALPHA)横山 M-RRU-ZSYS-00742 対処
 * @date  2011/11/30 ALPHA)横山 M-RRU-ZSYS-00743 対処
 * @date  2011/12/01 ALPHA)横山 M-RRU-ZSYS-00744 対処
 * @date  2011/12/02 ALPHA)横山 M-RRU-ZSYS-00745 対処
 * @date  2011/12/06 ALPHA)横山 SRRU-2.1G-LTE対応
 * @date  2011/12/08 ALPHA)高橋 SRRU-2.1G-LTE対応
 * @date  2011/12/26 ALPHA)藤井 M-RRU-ZSYS-00756対処
 * @date  2012/01/10 ALPHA)高橋 M-RRU-ZSYS-00766対処
 * @date  2011/01/12 ALPHA)高橋 M-RRU-ZSYS-00772対処
 * @date  2011/01/13 ALPHA)高橋 M-RRU-ZSYS-00775対処
 * @date  2011/01/20 ALPHA)横山 M-RRU-ZSYS-00786対処
 * @date  2012/02/08 ALPHA)横山 M-RRU-ZSYS-00807対処
 * @date  2012/02/16 ALPHA)横山 M-RRU-ZSYS-00821対処
 * @date  2012/02/22 ALPHA)高橋 M-RRU-ZSYS-00824対処
 * @date  2012/03/01 ALPHA)高橋 M-RRU-ZSYS-00827対処
 * @date  2012/03/08 ALPHA)横山 M-RRU-ZSYS-00830対処
 * @date  2012/03/15 ALPHA)横山 N21GLTE対応
 * @date  2012/03/21 ALPHA)高橋 N21GLTE対応
 * @date  2012/04/02 ALPHA)藤井 N21GLTE M-RRU-ZSYS-00840(IT1問処No.2)
 * @date  2012/04/12 ALPHA)横山 M-RRU-ZSYS-00853対処
 * @date  2012/04/25 ALPHA)藤井 N21GLTE M-RRU-ZSYS-00863対応(ハソ15版)
 * @date  2012/04/26 ALPHA)横山 M-RRU-ZSYS-00864対処
 * @date  2012/04/27 ALPHA)横山 M-RRU-ZSYS-00866対処
 * @date  2012/05/02 ALPHA)横山 M-RRU-ZSYS-00867対処
 * @date  2012/05/08 ALPHA)高橋 M-RRU-ZSYS-00868対処
 * @date  2012/05/08 ALPHA)高橋 M-RRU-ZSYS-00869対処
 * @date  2012/05/09 ALPHA)高橋 M-RRU-ZSYS-00871対処
 * @date  2012/05/11 ALPHA)横山 M-RRU-ZSYS-00874対処
 * @date  2012/05/09 ALPHA)高橋 M-RRU-ZSYS-00871対処
 * @date  2012/05/14 ALPHA)高橋 M-RRU-ZSYS-00875対処
 * @date  2012/05/15 ALPHA)横山 M-RRU-ZSYS-00877対処
 * @date  2012/05/22 ALPHA)横山 M-RRU-ZSYS-00882対処
 * @date  2012/06/08 ALPHA)横山 M-RRU-ZSYS-00886対処
 * @date  2012/06/14 ALPHA)横山 M-RRU-ZSYS-00887対処
 * @date  2012/06/27 ALPHA)横山 M-RRU-ZSYS-00889対処
 * @date  2012/10/01 ALPHA)横山 MTCD対応
 * @date  2012/10/31 ALPHA)藤井 M-RRU-ZSYS-00896対処
 * @date  2012/11/15 ALPHA)横山 M-RRU-ZSYS-00905対処
 * @date  2012/12/05 ALPHA)横山 M-RRU-ZSYS-00916対処
 * @date  2012/12/18 ALPHA)横山 M-RRU-ZSYS-00921対処
 * @date  2012/12/19 ALPHA)藤井 M-RRU-ZSYS-00923対処
 * @date  2012/12/21 ALPHA)横山 M-RRU-ZSYS-00925対処
 * @date  2012/12/25 ALPHA)横山 M-RRU-ZSYS-00926対処
 * @date  2012/12/28 ALPHA)藤井 M-RRU-ZSYS-00927対処
 * @date  2013/01/08 ALPHA)横山 M-RRU-ZSYS-00928対処
 * @date  2013/01/10 ALPHA)横山 M-RRU-ZSYS-00930対処
 * @date  2013/01/15 ALPHA)藤井 M-RRU-ZSYS-00934対処
 * @date  2013/01/28 ALPHA)藤井 M-RRU-ZSYS-00936対処
 * @date  2013/03/21 ALPHA)藤井 M-RRU-ZSYS-00942対応
 * @date  2013/04/03 ALPHA)中村 ST800対応
 * @date  2013/11/26 ALPHA)加藤 ZYNQ対応
 * @date  2015/04/22 ALPHA)近間 RRH-013-000 DCM_3.5G対応
 * @date  2015/06/19 ALPHA)近間 RRH-007-000 DCM_3.5G対応
 * @date  2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 * @date  2018/09/08 KCN)吉木   5G-DU対応 
 * @date  2020/01/06 FJT)吉田   DCM5GDU-053-191227 [19BD] : キャリアリーク改善対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2020
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#ifndef F_DPDA_DEF_H
#define F_DPDA_DEF_H

/*!
 * @name DPDA戻り値
 * @note DPDA戻り値の定義
 * @{
 */
#define D_DPDA_RTN_COMPLETE						0				/*!< 正常終了							*/
#define D_DPDA_RTN_NG							1				/*!< 異常終了							*/
#define	D_DPDA_RTN_INPUTERR						256				/*!< 異常(インプットエラー)				*/
/* @} */


/*!
 * @name DPDAタスク状態
 * @note DPDAタスクの状態定義
 * @{
 */
#define D_DPDA_TASK_STS_INIT						0x0000		/*!< 初期状態							*/
#define D_DPDA_TASK_STS_ACT							0x0001		/*!< 運用状態							*/
#define D_DPDA_STATENUM								0x0002		/*!< DPDAタスク状態数					*/
/* @} */

#define D_DPDA_EDIT_OK								0x00000000
#define D_DPDA_EDIT_ERR								0x00000002
#define D_DPDA_EDIT_INPUT_ERR						0x00000003
#define D_DPDA_UPCNF_ERR_MSG_SIZE					32

/*!
 * @name DPDAタスクイベント
 * @note DPDAタスクのイベント定義
 * @{
 */
enum
{
	E_DPDA_EVE_INIT,
	E_DPDA_EVE_SYS_PARAM_SET_REQUEST,
	E_DPDA_EVE_CARRIER_ON_SETTING_REQUEST,
	E_DPDA_EVE_CARRIER_OFF_SETTING_REQUEST,
	E_DPDA_EVE_TXRXCAL_INUSE_REQUEST,
	E_DPDA_EVE_TXRXCAL_TRAINING_REQUEST,
	E_DPDA_EVE_POW_PERIODIC_NOTI,
	E_DPDA_EVE_TXON_REQUEST,
	E_DPDA_EVE_TXOFF_REQUEST,
	E_DPDA_EVE_RXON_REQUEST,
	E_DPDA_EVE_RXOFF_REQUEST,
	E_DPDA_EVE_TDDEN_CHG_NTC,	
	E_DPDA_EVE_MNT_POWER_ADJUST_REQ,
	E_DPDA_EVE_BEAMID_SET_REQ,
	E_DPDA_EVE_TXSLEEP_REQUEST,
	E_DPDA_EVE_RXSLEEP_REQUEST,
	E_DPDA_EVENTNUM								/*!< DPDAタスクイベント数				*/
};
/* @} */

/*!
 * @name DPDAタスク レジスタOn/Off
 * @note DPDAタスク レジスタOn/Off bit
 * @{
 */
#define D_DPDA_REG_OFF								0x00000000	/*!< OFF設定							*/
#define D_DPDA_REG_ON								0x00000001	/*!< ON設定								*/
/* @} */

/*!
 * @name DPDAタスク仮想レジスタ関連
 * @note DPDAタスク仮想レジスタ関連定義
 * @{
 */
#define	D_DPDA_DBG_VIRTUAL_REG_CNT					142			/*!< 仮想レジスタ数						*/

/* 仮想レジスタアドレスオフセット		*/
#define	D_DPDA_DBG_VREG_ADDR_OFFSET_CPLD			0x00000		/*!< CPLD								*/
#define	D_DPDA_DBG_VREG_ADDR_OFFSET_RX				0x10000		/*!< clemente RX						*/
#define	D_DPDA_DBG_VREG_ADDR_OFFSET_FEFB			0x20000		/*!< clemente FEFB						*/
#define	D_DPDA_DBG_VREG_ADDR_OFFSET_PLL				0x30000		/*!< PLL								*/
#define	D_DPDA_DBG_VREG_ADDR_OFFSET_TEMP			0x40000		/*!< temp								*/

#define	D_DPDA_DBG_VREG_SPI_CHK						0x40000000	/*!< SPI reg check						*/
/* @} */


/*!
 * @name DPDAタスクRun History関連
 * @note DPDAタスクRun History関連定義
 * @{
 */
#define	D_DPDA_MAX_RUNHISBODY						128			/*!< Maximum running history body		*/
#define	D_DPDA_MAX_RUNHISREC		D_COM_MAX_RUNHISREC			/*!< Maximum running history record		*/

#define	D_DPDA_RECIEVEFLG							0			/*!< recieve Flag						*/
#define	D_DPDA_SENDFLG								1			/*!< send flag							*/
/* @} */

/*!
 * @name 構造体初期化用
 * @note 構造体初期化用定義
 * @{
 */	
#define	D_DPDA_ZERO									0			/*!< 0									*/
/* @} */

/*!
 * @name DPDAタスク装置種別関連
 * @note DPDAタスク装置種別関連定義
 * @{
 */
#define	D_DPDA_CDMA 								0			/*!< CDMA								*/
#define	D_DPDA_LTE									1			/*!< LTE								*/
/* @} */

/*!
 * @name DPDAタスクSend Power Adjust Request関連
 * @note DPDAタスクSend Power Adjust Request関連定義
 * @{
 */
#define D_DPDA_SEND_POW_ADJ_UP						0			/*!< 送信出力UP							*/
#define D_DPDA_SEND_POW_ADJ_DOWN					1			/*!< 送信出力DOWN						*/
#define D_DPDA_SEND_POW_CONST_MAX					10			/*!< 送信電力設定値の最大値				*/
/* @} */

/*!
 * @name DPDAタスクアンテナ関連
 * @note DPDAタスクアンテナ関連定義
 * @{
 */
#define	D_DPDA_ANTMAX								D_SYS_ANT_MAX	/*!< TX/RXのANT数					*/
#define	D_DPDA_ANTMAX_MMW							2				/*!< TX/RXのANT数(mmW用)			*/
#define	D_DPDA_ANTMAX_RX							D_SYS_ANT_MAX	/*!< RXのANT数						*/
#define	D_DPDA_ANTA 								0			/*!< AntA								*/
#define	D_DPDA_ANTB 								1			/*!< AntB								*/
#define	D_DPDA_ANTC 								2			/*!< AntC								*/
#define	D_DPDA_ANTD 								3			/*!< AntD								*/
#define	D_DPDA_ANTE 								4			/*!< AntD								*/
#define	D_DPDA_ANTF 								5			/*!< AntD								*/
#define	D_DPDA_ANTG 								6			/*!< AntD								*/
#define	D_DPDA_ANTH 								7			/*!< AntD								*/
#define	D_DPDA_ANT_TYPE_AB 							0			/*!< AntA,AntB							*/
#define D_DPDA_ANT_AC								0			/* アンテナA&C							*/ 
#define D_DPDA_ANT_BD								1			/* アンテナB&D							*/ 
#define	D_DPDA_ANT_TYPE_NUM 						2			/*!< Antenna type数						*/
#define	D_DPDA_ANT_ENABLE							0x80000000	/*!<アンテナON設定値					*/
#define	D_DPDA_ANT_FB_SYSCOMMON						D_DPDA_ANTA	/*!< FBの全系共通に使用するANT番号		*/
/* @} */

/*!
 * @name DPDAタスクアンテナ関連
 * @note DPDAタスクアンテナ関連定義
 * @{
 */
#define	D_DPDA_ANTKIND_TX							0			/*!< TX									*/
#define	D_DPDA_ANTKIND_RX							1			/*!< RX									*/
#define	D_DPDA_ANTKIND_NUM							2			/*!< TX/RX種別数						*/
#define	D_DPDA_ALM_ANTA 							0			/*!< ANT#A(Alarm用)						*/
#define	D_DPDA_ALM_ANTB 							1			/*!< ANT#B(Alarm用)						*/
#define	D_DPDA_ALM_ANTA_AND_B 						2			/*!< ANT#A and ANT#B(Alarm用)			*/
#define	D_DPDA_ANT0 								0			/*!< ANT#A or ANT#B						*/
#define	D_DPDA_ANT1 								1			/*!< ANT#C or ANT#D						*/
#define	D_DPDA_ANT2 								2			/*!< ANT#E or ANT#F						*/
#define	D_DPDA_ANT3 								3			/*!< ANT#G or ANT#H						*/
#define	D_DPDA_ANTNUM_1DEV							2			/*!< 1deviceあたりのアンテナ数			*/
#define	D_DPDA_ANT_TYPE_AB 							0			/*!< ANT#A,ANT#B						*/
#define	D_DPDA_ANT_TYPE_NUM 						2			/*!< Antenna type数						*/
#define	D_DPDA_ANT_ID0 								0			/*!< ANT#0								*/
#define	D_DPDA_ANT_ID1 								1			/*!< ANT#1								*/
/* @} */

#define	D_DPDA_NON_TEST								0			/*!< テストフラグ無効					*/
#define	D_DPDA_BWKHZ								1000		/*!< BWをHz→KHz変換					*/

#define	D_DPDA_CCID_CNT								4			/*!< CCIDの変換位置						*/
#define	D_DPDA_CELL_CNT								8			/*!< CELL-IDの変換位置					*/
#define	D_DPDA_POR_MSK								0x000F		/*!< ポートIDのBIT位置					*/
#define	D_DPDA_CCID_MSK								0x00F0		/*!< CCIDのBIT位置						*/
#define	D_DPDA_CELL_MSK								0x3F00		/*!< CELL IDのBIT位置					*/
#define	D_DPDA_CELL_INVALID							0x3F		/*!< CELL IDの無効値					*/

/*!
 * @name DPDAタスクキャリア関連
 * @note DPDAタスクキャリア関連定義
 * @{
 */
#define D_DPDA_CAR_OFF								0			/*!< キャリアOFF						*/
#define D_DPDA_CAR_ON								1			/*!< キャリアON							*/
#define D_DPDA_ANT_DIS								0x00000000	/*!< アンテナDisable					*/
#define D_DPDA_ANT_ENA								0x80000000	/*!< アンテナEnable						*/
#define	D_DPDA_CAR0									0			/*!< Carrier0							*/
#define	D_DPDA_CARMAX								D_SYS_CAR_MAX	/*!< Carrier番号最大値(mmW用)		*/
#define	D_DPDA_CARMAX_SUB6							1				/*!< Carrier番号最大値(sub6用)		*/
#define	D_DPDA_CARMAX_SUB6_45G						1				/*!< Carrier番号最大値(sub6用)		*/

#define	D_DPDA_TXCARMAX_PER_ANT						8			/*!< 1アンテナあたりの最大Txキャリア数	*/
#define	D_DPDA_TXCARMAX_PER_AFTERVER_MAX			38			/*!<【V03R04以降】ピーク抑圧設定インデックス	*/

#define D_DPDA_GAIN_CONST_MAX						11				/*!< キャリアゲイン微調設定係数最大値	*/
#define D_DPDA_GAIN_ADJ_COEF_DIV_NUM				1000000000.0	/*!< ゲイン微調設定係数(割る数)			*/

#define D_DPDA_CAR_NORMAL							0			/*!< 通常キャリア						*/
#define D_DPDA_CAR_NOMOD							1			/*!< 無変調キャリア						*/

#define D_DPDA_TX_NCO_DEFAULT						0xFFD		/*!< Tx NCO(default値)					*/

#define D_DPDA_RX_DELAY_SYSREF_VAL_NUM				2			/*!< 受信キャリアSYSREF位相差結果補正値Y(RRE)	*/
#define D_DPDA_RX_DELAY_SYSREF_VAL_SRE_NUM			4			/*!< 受信キャリアSYSREF位相差結果補正値Y(SRE)	*/

#define D_DPDA_TX_DELAY_BASE_VAL					0x0D38		/*!< 送信遅延基準値						*//* 4TRx 評価後設定値 */
#define D_DPDA_RX_DELAY_BASE_VAL					0x0D94		/*!< 受信遅延基準値						*/
 	
#define D_DPDA_APC_DETPOW_SIGNED_BIT				0x8000		/*!< DET POW(符号ビット)				*/
#define D_DPDA_APC_DETPOW_INT						0x0FF0		/*!< DET POW(整数部)					*/
#define D_DPDA_APC_DETPOW_FRAC_CHK_BIT				0x0008		/*!< DET POW(少数チェックビット)		*/
#define D_DPDA_APC_DETPOW_FRAC_BIT_NUM				4			/*!< DET POW(少数チェック数)			*/
    
#define D_DPDA_MASK_AT_ALL_TIME                     1           /*!< 常時マスク                         */
/* @} */

/* SRE対応 - Start */
#define D_DPDA_NOMOD_TX_GAIN_BASEVAL				0x1000		/*!< 基準設定値(CW時)					*/
#define D_DPDA_NOMOD_RX_GAIN_BASEVAL				0x200		/*!< 基準設定値(CW時)					*/

/*!
 * @name FB Powerレジスタ識別
 * @note FB Powerレジスタ識別
 * @{
 */
#define	D_DPDA_REGADDR_FBPOWER1							0xFF0			/*!< [D11:D4]					*/
#define	D_DPDA_REGADDR_FBPOWER2							0x00F			/*!< [D3:D0]					*/

/* @} */
/* SRE対応 - End */

/*!
 * @name DPDAタスク タスク間IF関連
 * @note DPDAタスク タスク間IF関連定義
 * @{
 */
#define	D_DPDA_ENABLE								1			/*!< User Enable						*/
#define	D_DPDA_DISABLE								0			/*!< User Disable						*/

enum
{
	E_DPDA_BW_IDX_5M = 0,										/*!< BandWidth=5M						*/
	E_DPDA_BW_IDX_10M,											/*!< BandWidth=10M						*/
	E_DPDA_BW_IDX_15M,											/*!< BandWidth=15M						*/
	E_DPDA_BW_IDX_20M,											/*!< BandWidth=20M						*/
	E_DPDA_BW_IDX_MAX											/*!< BandWidth数						*/
};
/* @} */

/* LTE帯域幅レジスタ設定値	*/
enum
{
	D_DPDA_REG_DATA_BW_5M = 2,								/*!< BandWidth=5M							*/
	D_DPDA_REG_DATA_BW_10M,									/*!< BandWidth=10M							*/
	D_DPDA_REG_DATA_BW_15M,									/*!< BandWidth=15M							*/
	D_DPDA_REG_DATA_BW_20M									/*!< BandWidth=20M							*/
};

/*!
 * @name DPDAスレッド RF PLL設定パラメータ関連
 * @note DPDAスレッド RF PLL設定パラメータ関連定義
 * @{
 */
enum
{
	E_DPDA_RFPLL_FREQ_BASE = 0,								/*!< 基準周波数								*/
	E_DPDA_RFPLL_FREQ_OFFSET,								/*!< 周波数オフセット						*/
	E_DPDA_RFPLL_NINT_MUL,									/*!< 計算式2(掛ける数)						*/
	E_DPDA_RFPLL_NINT_DIV,									/*!< 計算式2(割る数)						*/
	E_DPDA_RFPLL_NFRAC_MUL,									/*!< 計算式3(掛ける数)						*/
	E_DPDA_RFPLL_NUM										/*!< RF PLL設定パラメータ数					*/
};

/*!

 * @name DPDAタスク歪補償パラメータ設定関連
 * @note DPDAタスク歪補償パラメータ設定関連定義
 * @{
 */
/* LTEキャリアのみのBandWidth	*/
enum
{
	E_DPDA_STRAIN_CDMA_CAR0 = 0,							/*!< キャリア数:0							*/
	E_DPDA_STRAIN_BW03_CAR1,								/*!< BandWidth: 3M							*/
	E_DPDA_STRAIN_BW05_CAR1,								/*!< BandWidth: 5M							*/
	E_DPDA_STRAIN_BW10_CAR1,								/*!< BandWidth:10M							*/
	E_DPDA_STRAIN_BW15_CAR1,								/*!< BandWidth:15M							*/
	E_DPDA_STRAIN_BW20_CAR1,								/*!< BandWidth:20M							*/
	E_DPDA_STRAIN_MAX_BW									/*!< BandWidthMAX							*/
};

#define	D_DPDA_CAR_DIR_DL					0x1										/* キャリア方向(DL:Tx)	*/
#define	D_DPDA_CAR_DIR_UL					0x2										/* キャリア方向(UL:Rx)	*/
#define	D_DPDA_CAR_DIR_BOTH					(D_DPDA_CAR_DIR_DL|D_DPDA_CAR_DIR_UL)	/* キャリア方向(DL/UL)	*/

/* 電力報告周期時	DPDA SPI(CPLD)収集情報レジスタ */
enum
{
	E_DPDA_SPICPLD_PSALM = 0,								/*!< PS ALM用情報							*/
	E_DPDA_SPICPLD_PAPS,									/*!< PA/PS設定								*/
	E_DPDA_SPICPLD_NUM
};

/* 電力報告周期時	DPDA SPI(Tx)収集情報レジスタ */
enum
{
	E_DPDA_SPITX_RF_CP_OVER_RANGE_VCO_LOCK = 0,				/*!< FW PLL ALM用情報						*/
	E_DPDA_SPITX_NUM
};

/* 電力報告周期時	DPDA SPI(Rx)収集情報レジスタ */
enum
{
	E_DPDA_SPIRX_RF_CP_OVER_RANGE_VCO_LOCK = 0,				/*!< RW PLL ALM用情報						*/
	E_DPDA_SPIRX_NUM
};
/* @} */

/****************************************************************************************************/

/*!
 * @name DPDAタスク報告値関連
 * @note DPDAタスク報告値関連定義
 * @{
 */
#define D_DPDA_CALC_ANT_TXPOWREP					50337792		/*!< TSSI報告値 送信電力							*/
#define D_DPDA_CALC_ANT_POWER_CONST					35467268		/*!< アンテナ電力算出用定数							*/
#define D_DPDA_CALC_ANT_RXPOWREP					678861469		/*!< RSSI報告値 受信電力							*/ /* 4TRxRRE */
#define D_DPDA_CALC_ANT_RXPOWREP_RRE				678861469		/*!< RSSI報告値 受信電力(RRE用)						*/

#define D_DPDA_CALC_ANT_RSSIREP_H					840564			/*!< RSSI報告値 電力積分(138538726を分割)			*/ /* 4TRxRRE */
#define D_DPDA_CALC_ANT_RSSIREP_L					781				/*!< RSSI報告値 電力積分							*/ /* 4TRxRRE */
#define D_DPDA_CALC_ANT_RSSIREP_H_RRE				840564			/*!< RSSI報告値 電力積分(840564781を分割)(RRE用)	*/
#define D_DPDA_CALC_ANT_RSSIREP_L_RRE				781				/*!< RSSI報告値 電力積分(RRE用)						*/

/* @} */

/*!
 * @name DPDA検出アラーム閾値 ID
 * @note DPDA検出アラーム閾値 ID定義
 * @{
 */
enum
{
	E_DPDA_ALM_THRESH_TX_GAIN_MAX,							/* TX out of order TX Gain ALM MAX LIMIT */
	E_DPDA_ALM_THRESH_TOVER_MAX,							/* TX out of order TOVER ALM MAX LIMIT */
	E_DPDA_ALM_THRESH_RX_GAIN_MIN,							/* RX out of order Rx Gain ALM MIN LIMIT */
	E_DPDA_ALM_THRESH_HIGH_TEMP,
	E_DPDA_ALM_THRESH_LOW_TEMP,
	E_DPDA_ALM_THRESH_TX_PWR,
	E_DPDA_ALM_THRESH_TX_PWR_AVR,
	E_DPDA_ALM_THRESH_VSWR,
	E_DPDA_ALM_THRESH_NUM_MAX
};


/*!
 * @name Alarm History Log関連情報
 * @note Alarm History Log関連情報 sub6
 * @{
 */
/* Alarm History log用 レジスタ情報 */
#define D_DPDA_REG_SUB6_NUM_MAX		D_RRH_ALMHIS_REG_NUM_SUB6
#define D_DPDA_REG_MMW_NUM_MAX		D_RRH_ALMHIS_REG_NUM_MMW

/*!
 * @name DPDA検出アラーム Alarm ID sub6用 (Instance ID)
 * @note DPDA検出アラーム Alarm ID定義
 * @{
 */
enum
{
	E_DPDA_ALM_SUB6_JCPLL_UNLOCK_ALM,
	E_DPDA_ALM_SUB6_PS_ALM,
	E_DPDA_ALM_SUB6_POW_ERR1,
	E_DPDA_ALM_SUB6_POW_ERR2,
	E_DPDA_ALM_SUB6_SYS_PLL_UNLOCK_LLB,
	E_DPDA_ALM_SUB6_IC_PLL_UNLOCK_LLB,
	E_DPDA_ALM_SUB6_ETH_PLL_UNLOCK_LLB,
	E_DPDA_ALM_SUB6_TRX1_PCIE_ERR1_LLB,
	E_DPDA_ALM_SUB6_TRX1_PCIE_ERR2_LLB,
	E_DPDA_ALM_SUB6_TRX1_SV_FAIL_LLB,
	E_DPDA_ALM_SUB6_ETHER_ALM,
	E_DPDA_ALM_SUB6_SRAM_CON_ERR,
	E_DPDA_ALM_SUB6_SRAM_UNCON_ERR,
	E_DPDA_ALM_SUB6_TRX1_IC_TX_ERR_LLB,
	E_DPDA_ALM_SUB6_TRX1_IC_RX_ERR_LLB,
	E_DPDA_ALM_SUB6_TRX1_IC_TX_ERR_TRX,
	E_DPDA_ALM_SUB6_TRX1_IC_RX_ERR_TRX,
	E_DPDA_ALM_SUB6_TRX1_AUTO_CR_HC_ERR,
	E_DPDA_ALM_SUB6_SYS_PLL_UNLOCK_TRX1,
	E_DPDA_ALM_SUB6_IC_PLL_UNLOCK_TRX1,
	E_DPDA_ALM_SUB6_PHYICPLLULCK1_TRX1,
	E_DPDA_ALM_SUB6_PHYICPLLULCK2_TRX1,
	E_DPDA_ALM_SUB6_PHYPOWALM_TRX1,
	E_DPDA_ALM_SUB6_PHYJS1PLLULCK_TRX1,
	E_DPDA_ALM_SUB6_PHYJS2PLLULCK_TRX1,
	E_DPDA_ALM_SUB6_TRX1_SV_FAIL_TRX1,
	E_DPDA_ALM_SUB6_R1_TX_JESD_ERR_TRX1,
	E_DPDA_ALM_SUB6_R1_FB_JESD_ERR_TRX1,
	E_DPDA_ALM_SUB6_R1_RX_JESD_ERR_TRX1,
	E_DPDA_ALM_SUB6_R2_TX_JESD_ERR_TRX1,
	E_DPDA_ALM_SUB6_R2_FB_JESD_ERR_TRX1,
	E_DPDA_ALM_SUB6_R2_RX_JESD_ERR_TRX1,
	E_DPDA_ALM_SUB6_LMB_COR_ERR,
	E_DPDA_ALM_SUB6_LMB_UNCOR_ERR,
	E_DPDA_ALM_SUB6_RFIC1_ALM,
	E_DPDA_ALM_SUB6_RFIC2_ALM,
	E_DPDA_ALM_SUB6_TX_GAIN_ALM_ANT_A,
	E_DPDA_ALM_SUB6_TX_GAIN_ALM_ANT_B,
	E_DPDA_ALM_SUB6_TX_GAIN_ALM_ANT_C,
	E_DPDA_ALM_SUB6_TX_GAIN_ALM_ANT_D,
	E_DPDA_ALM_SUB6_TOVER_ALM_ANT_A,
	E_DPDA_ALM_SUB6_TOVER_ALM_ANT_B,
	E_DPDA_ALM_SUB6_TOVER_ALM_ANT_C,
	E_DPDA_ALM_SUB6_TOVER_ALM_ANT_D,
	E_DPDA_ALM_SUB6_RX_GAIN_ALM_ANT_A,
	E_DPDA_ALM_SUB6_RX_GAIN_ALM_ANT_B,
	E_DPDA_ALM_SUB6_RX_GAIN_ALM_ANT_C,
	E_DPDA_ALM_SUB6_RX_GAIN_ALM_ANT_D,
	E_DPDA_ALM_SUB6_VSWR_ALM_ANT_A,
	E_DPDA_ALM_SUB6_VSWR_ALM_ANT_B,
	E_DPDA_ALM_SUB6_VSWR_ALM_ANT_C,
	E_DPDA_ALM_SUB6_VSWR_ALM_ANT_D,
	E_DPDA_ALM_SUB6_TEMP_ALM_HIGH,	
	E_DPDA_ALM_SUB6_TEMP_ALM_LOW,	
	E_DPDA_ALM_SUB6_NUM_MAX
};
/* @} */

/*!
 * @name DPDA検出アラーム Alarm ID mmW用 (Instance ID)
 * @note DPDA検出アラーム Alarm ID定義
 * @{
 */
enum
{
	E_DPDA_ALM_MMW_JCPLL_UNLOCK_ALM,
	E_DPDA_ALM_MMW_RFPLL_UNLOCK_ALM,
	E_DPDA_ALM_MMW_PS_ALM,
	E_DPDA_ALM_MMW_POW_ERR1,
	E_DPDA_ALM_MMW_POW_ERR2,
	E_DPDA_ALM_MMW_SYS_PLL_UNLOCK_LLB,
	E_DPDA_ALM_MMW_IC_PLL_UNLOCK_LLB,
	E_DPDA_ALM_MMW_ETH_PLL_UNLOCK_LLB,
	E_DPDA_ALM_MMW_TRX1_PCIE_ERR1_LLB,
	E_DPDA_ALM_MMW_TRX1_PCIE_ERR2_LLB,
	E_DPDA_ALM_MMW_TRX1_SV_FAIL_LLB,
	E_DPDA_ALM_MMW_ETHER_ALM,
	E_DPDA_ALM_MMW_SRAM_CON_ERR,
	E_DPDA_ALM_MMW_SRAM_UNCON_ERR,
	E_DPDA_ALM_MMW_TRX1_IC_TX_ERR_LLB,
	E_DPDA_ALM_MMW_TRX1_IC_RX_ERR_LLB,
	E_DPDA_ALM_MMW_TRX1_IC_TX_ERR_TRX,
	E_DPDA_ALM_MMW_TRX1_IC_RX_ERR_TRX,
	E_DPDA_ALM_MMW_SYS_PLL_UNLOCK_TRX1,
	E_DPDA_ALM_MMW_IC_PLL_UNLOCK_TRX1,
	E_DPDA_ALM_MMW_PHYICPLLULCK1_TRX1,
	E_DPDA_ALM_MMW_PHYICPLLULCK2_TRX1,
	E_DPDA_ALM_MMW_PHYPOWALM_TRX1,
	E_DPDA_ALM_MMW_PHYJS1PLLULCK_TRX1,
	E_DPDA_ALM_MMW_TRX1_SV_FAIL_TRX1,
	E_DPDA_ALM_MMW_R1_TX_JESD_ERR_TRX1,
	E_DPDA_ALM_MMW_R1_RX_JESD_ERR_TRX1,
	E_DPDA_ALM_MMW_LMB_COR_ERR,
	E_DPDA_ALM_MMW_LMB_UNCOR_ERR,
	E_DPDA_ALM_MMW_TXDAC_ALM,
	E_DPDA_ALM_MMW_RXADC_ALM,
	E_DPDA_ALM_MMW_TOVER_ALM_ANT_A,
	E_DPDA_ALM_MMW_TOVER_ALM_ANT_B,
	E_DPDA_ALM_MMW_RX_GAIN_ALM_ANT_A,
	E_DPDA_ALM_MMW_RX_GAIN_ALM_ANT_B,
	E_DPDA_ALM_MMW_TEMP_ALM_HIGH,
	E_DPDA_ALM_MMW_TEMP_ALM_LOW,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V01,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V02,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V03,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V04,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V05,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V06,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V07,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V08,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V09,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V10,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V11,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V12,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V13,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V14,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V15,
	E_DPDA_ALM_MMW_TX_PWR_ALM_V16,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H01,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H02,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H03,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H04,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H05,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H06,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H07,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H08,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H09,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H10,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H11,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H12,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H13,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H14,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H15,
	E_DPDA_ALM_MMW_TX_PWR_ALM_H16,
	E_DPDA_ALM_MMW_NUM_MAX
};
/* @} */
 	
 	
 	
/*!
 * @name DPDAアラーム関連
 * @note DPDAアラーム関連定義
 * @{
 */
#define	D_DPDA_ALM_NUM								E_DPDA_ALM_MMW_NUM_MAX	/*!< DPDAアラーム数		*/	//E_DPDA_ALM_SUB6_NUM_MAXとE_DPDA_ALM_MMW_NUM_MAXで大きい方をD_DPDA_ALM_NUMとする
#define	D_DPDA_ALM_FACTOR_NUM						9			/*!< DPDAアラーム要因数				*/
#define	D_DPDA_ALM_INSTID_CNVT_NUM					20			/*!< InstanceID変換アラーム数		*/
#define	D_DPDA_ALM_T14MASK_NUM						0x7FFFFFFF	/*!< T14測定値用マスク値([30:0]) 	*/
/* @} */

/*!
 * @name DPDAアラーム判定用情報 f_dpdar_almJdgInfo用
 * @note DPDAアラーム判定用情報定義
 * @{
 */
enum
{
	E_DPDA_ALM_BIT_JCPLL_UNLOCK_ALM,
	E_DPDA_ALM_BIT_RFPLL_UNLOCK_ALM,
	E_DPDA_ALM_BIT_PS_ALM,
	E_DPDA_ALM_BIT_POW_ERR1,
	E_DPDA_ALM_BIT_POW_ERR2,
	E_DPDA_ALM_BIT_SYS_PLL_UNLOCK_LLB,
	E_DPDA_ALM_BIT_IC_PLL_UNLOCK_LLB,
	E_DPDA_ALM_BIT_ETH_PLL_UNLOCK_LLB,
	E_DPDA_ALM_BIT_TRX1_PCIE_ERR1_LLB,
	E_DPDA_ALM_BIT_TRX1_PCIE_ERR2_LLB,
	E_DPDA_ALM_BIT_TRX1_SV_FAIL_LLB,
	E_DPDA_ALM_BIT_ETHER_ALM,
	E_DPDA_ALM_BIT_SRAM_CON_ERR,
	E_DPDA_ALM_BIT_SRAM_UNCON_ERR,
	E_DPDA_ALM_BIT_TRX1_IC_TX_ERR_LLB,
	E_DPDA_ALM_BIT_TRX1_IC_RX_ERR_LLB,
	E_DPDA_ALM_BIT_TRX1_IC_RX_ERR_LLB2,
	E_DPDA_ALM_BIT_TRX1_IC_TX_ERR_TRX,
	E_DPDA_ALM_BIT_TRX1_IC_RX_ERR_TRX,
	E_DPDA_ALM_BIT_TRX1_AUTO_CR_HC_ERR,
	E_DPDA_ALM_BIT_SYS_PLL_UNLOCK_TRX1,
	E_DPDA_ALM_BIT_IC_PLL_UNLOCK_TRX1,
	E_DPDA_ALM_BIT_PHYICPLLULCK1_TRX1,
	E_DPDA_ALM_BIT_PHYICPLLULCK2_TRX1,
	E_DPDA_ALM_BIT_PHYPOWALM_TRX1,
	E_DPDA_ALM_BIT_PHYJS1PLLULCK_TRX1,
	E_DPDA_ALM_BIT_PHYJS2PLLULCK_TRX1,
	E_DPDA_ALM_BIT_TRX1_SV_FAIL_TRX1,
	E_DPDA_ALM_BIT_R1_TX_JESD_ERR_TRX1,
	E_DPDA_ALM_BIT_R1_FB_JESD_ERR_TRX1,
	E_DPDA_ALM_BIT_R1_RX_JESD_ERR_TRX1,
	E_DPDA_ALM_BIT_R2_TX_JESD_ERR_TRX1,
	E_DPDA_ALM_BIT_R2_FB_JESD_ERR_TRX1,
	E_DPDA_ALM_BIT_R2_RX_JESD_ERR_TRX1,
	E_DPDA_ALM_BIT_LMB_COR_ERR,
	E_DPDA_ALM_BIT_LMB_UNCOR_ERR,
	E_DPDA_ALM_BIT_RFIC1_ALM,
	E_DPDA_ALM_BIT_RFIC2_ALM,
	E_DPDA_ALM_BIT_TXDAC_ALM1,
	E_DPDA_ALM_BIT_TXDAC_ALM2,
	E_DPDA_ALM_BIT_RXADC_ALM1,
	E_DPDA_ALM_BIT_RXADC_ALM2,

	E_DPDA_ALM_BIT_CAR_OFF,									/*!< キャリア状態(マスク条件判定用)			*/
	E_DPDA_ALM_BIT_ANT_ENA,									/*!< アンテナEnable状態(マスク条件判定用)		*/
	E_DPDA_ALM_BIT_REFPOW_THRETH,							/*!< TX Gain ALM Mask Value				*/
    E_DPDA_ALM_BIT_MASK_AT_ALL_TIME,                        /*!< 常時マスク                         */

    E_DPDA_ALM_BIT_RFSTOP,									/*!< TX GAIN,TOVER mask               */
    E_DPDA_ALM_BIT_RFBLK, 									/*!< RXGAIN mask                        */
	E_DPDA_ALM_BIT_HARD_PAOFF,								/*!< ハード自律PA-OFF[0] 1: Hard自律送信 停止中	*/

	E_DPDA_ALM_BIT_TOVER_A,									/*!< TOVER ALM ANT#A					*/
	E_DPDA_ALM_BIT_TOVER_B,									/*!< TOVER ALM ANT#B					*/
	E_DPDA_ALM_BIT_TOVER_C,									/*!< TOVER ALM ANT#C					*/
	E_DPDA_ALM_BIT_TOVER_D,									/*!< TOVER ALM ANT#D					*/

	E_DPDA_ALM_JDG_INFO_MAX_NUM								/*!< 収集アラーム数						*/
};

/* @} */

/*!
 * @name DPDAアラームPAon時のクリア条件関連
 * @note DPDAアラームPAon時のクリア条件関連定義
 * @{
 */
enum
{
	E_DPDA_ALM_PAON_CLEAR_NONE=0,							/*!< アラームクリア処理なし				*/
	E_DPDA_ALM_PAON_CLEAR_ANTA,								/*!< ANTA PA on時						*/
	E_DPDA_ALM_PAON_CLEAR_ANTB,								/*!< ANTB PA on時						*/
	E_DPDA_ALM_PAON_CLEAR_ANTA_OR_B,						/*!< ANTA or ANTB PA on時				*/
	E_DPDA_ALM_PAON_CLEAR_ANTA_AND_B,						/*!< ANTA and ANTB PA on時				*/
	E_DPDA_ALM_PAON_CLEAR_NUM								/*!< PA on時のアラームクリア条件数		*/

};
/* @} */

/*!
 * @name DPDAアラームPAoff時のクリア条件関連
 * @note DPDAアラームPAoff時のクリア条件関連定義
 * @{
 */
enum
{
	E_DPDA_ALM_PAOFF_CLEAR_NONE=0,							/*!< アラームクリア処理なし				*/
	E_DPDA_ALM_PAOFF_CLEAR_ANTA,							/*!< AntA PA off時						*/
	E_DPDA_ALM_PAOFF_CLEAR_ANTB,							/*!< AntB PA off時						*/
	E_DPDA_ALM_PAOFF_CLEAR_ANTA_OR_B,						/*!< AntA or AntB PA off時				*/
	E_DPDA_ALM_PAOFF_CLEAR_ANTA_AND_B,						/*!< AntA and AntB PA off時				*/
	E_DPDA_ALM_PAOFF_CLEAR_NUM								/*!< PA off時のアラームクリア条件数		*/
};
/* @} */

/*!
 * @name DPDAアラームキャリアoff時のクリア条件関連
 * @note DPDAアラームキャリアoff時のクリア条件関連定義
 * @{
 */
enum
{
	E_DPDA_ALM_RFOFF_CLEAR_NONE=0,							/*!< アラームクリア処理なし				*/
	E_DPDA_ALM_RFOFF_CLEAR_ANTA,							/*!< AntA RF off時						*/
	E_DPDA_ALM_RFOFF_CLEAR_ANTB,							/*!< AntB RF off時						*/
	E_DPDA_ALM_RFOFF_CLEAR_ANTC,							/*!< AntC RF off時						*/
	E_DPDA_ALM_RFOFF_CLEAR_ANTD,							/*!< AntD RF off時						*/
	E_DPDA_ALM_RFOFF_CLEAR_NUM								/*!< RF off時のアラームクリア条件数		*/
};
/* @} */

/*!
 * @name DPDAアラーム1秒周期ログ収集関連
 * @note DPDAアラーム1秒周期ログ収集関連定義
 * @{
 */
#define	D_DPDA_ALM_LOG_FACE						8			/*!< アラームログ収集面					*/
#define	D_DPDA_ALM_LOG_MAX						30			/*!< アラームログ収集数					*/
/* @} */


/*!
 * @name キャリア状態ログ収集関連
 * @note キャリア状態ログ収集関連定義
 * @{
 */
#define	D_DPDA_CARSTATE_LOG_MAX					1024		/*!< キャリア状態ログ最大収集数			*/
/* @} */



/*!
 * @name RXキャリアゲイン(CARRIER_BANDWIDTH)用定義
 * @note RXキャリアゲイン(CARRIER_BANDWIDTH)用定義
 * @{
 */
enum
{
	E_DPDA_RXGAIN_BAND_LTE3 = 0,								/*!< LTE3M, LTE5M, LTE10M, CDMA			*/
	E_DPDA_RXGAIN_BAND_LTE15,									/*!< LTE15M								*/
	E_DPDA_RXGAIN_BAND_LTE20,									/*!< LTE20M								*/
	E_DPDA_RXGAIN_BAND_MAX
};
/* @} */


/*!
 * @name DPDA使用マクロ
 * @note DPDA使用マクロ定義
 * @{
 */
#define	D_DPDA_MACRO_MIN(VAL1, VAL2)			( (VAL1) < (VAL2) ? (VAL1) : (VAL2) )			/*!< MIN														*/
/* @} */

/*!
 * @name パラメータ変更用定義(デバッグコマンド)
 * @note パラメータ変更用定義(デバッグコマンド)
 * @{
 */
#define		D_DPDA_INIT_FILENAME					"oem:dpda_init_parameter"										/*!< レジスタ初期化用ファイル名				*/
#define		D_DPDA_TX_STRAIN_FILENAME				"oem:dpda_tx_strain_parameter"									/*!< TX系歪補償パラメータ変更用ファイル名	*/
#define		D_DPDA_RX_STRAIN_FILENAME				"oem:dpda_rx_strain_parameter"									/*!< RX系歪補償パラメータ変更用ファイル名	*/

#define		D_DPDA_CHG_INI_PARAMETER				0x0001															/*!< レジスタ初期化実施有無					*/
#define		D_DPDA_CHG_TX_STRAIN_PARAMETER			0x0002															/*!< TX系歪補償パラメータ変更実施有無		*/
#define		D_DPDA_CHG_RX_STRAIN_PARAMETER			0x0004															/*!< RX系歪補償パラメータ変更実施有無		*/
 																											/*!< パラメータ変更無効bit					*/
#define		D_DPDA_CHG_PARA_INVALID					~( D_DPDA_CHG_INI_PARAMETER | D_DPDA_CHG_TX_STRAIN_PARAMETER | D_DPDA_CHG_RX_STRAIN_PARAMETER )

#define		D_DPDA_WORDMAX							8																/*!< ファイルデータ文字数					*/
#define		D_DPDA_DATAMAX_TX						D_DPDA_ANTMAX * E_DPDA_STRAIN_MAX_BW * E_DPDA_STRAIN_MAX_TX		/*!< ファイルデータパラメータ数(TX)			*/
#define		D_DPDA_DATAMAX_RX						D_DPDA_ANTMAX * E_DPDA_STRAIN_MAX_BW * E_DPDA_STRAIN_MAX_RX		/*!< ファイルデータパラメータ数(RX)			*/
/* @} */

/*!
 * @name デバッグ用定義
 * @note デバッグ用定義
 * @{
 */
#define		D_DPDA_DBG_MODE_ON						1				/*!< Mode=ON							*/
#define		D_DPDA_DBG_MODE_OFF						2				/*!< Mode=OFF							*/
/* @} */

/*!
 * @name 設定タイミング定義
 * @note 設定タイミング定義
 * @{
 */
#define		D_DPDA_SET_TIMING_INI					0				/*!< INIT時							*/
#define		D_DPDA_SET_TIMING_PA					1				/*!< PA制御時						*/
#define		D_DPDA_SET_TIMING_NUM					2				/*!< 設定タイミング種別				*/
/* @} */

/*!
 * @name 高速モード割り込み関連定義
 * @note 高速モード割り込み関連定義
 * @{
 */

#define		D_DPDA_HSMODE_A							0				/*!< 高速モードA					*/
#define		D_DPDA_HSMODE_B							1				/*!< 高速モードB					*/
#define		D_DPDA_HSMODE_FIN						2				/*!< 高速モード完了					*/

#define		D_DPDA_HSMODE_WAIT						0				/*!< 高速モード移行(wait)			*/
#define		D_DPDA_HSMODE_NORMAL					1				/*!< 高速モード移行(Normal)			*/
#define		D_DPDA_HSMODE_HIGHSPEED					2				/*!< 高速モード移行(高速モード)		*/
#define		D_DPDA_HSMODE_CALIB						3				/*!< 高速モード移行(キャリブレーション)	*/
#define		D_DPDA_HSMODE_CNT_INIT					30				/*!< 高速モードカウンタ初期値		*/
#define		D_DPDA_HSMODE_FLAG_OFF					0				/*!< フラグOFF						*/
#define		D_DPDA_HSMODE_FLAG_ON					1				/*!< フラグON						*/
#define		D_DPDA_WAIT_TIME						20000			/*!< wait時間(20msec)				*/
/* @} */

/*!
 * @name 0振幅時ローカルリーク補正処理関連定義
 * @note 0振幅時ローカルリーク補正処理関連定義
 * @{
 */
#define		D_DPDA_ZERO_AMP_LOL_WAIT_CNT			3				/*!< LOL制御開始待ちカウント		*/
/* @} */

/*!
 * @name DPD状態定義
 * @note DPD状態定義
 * @{
 */
#define		D_DPDA_DPD_STATE_STOP					0				/*!< 停止							*/
#define		D_DPDA_DPD_STATE_NORMAL					1				/*!< 通常							*/
#define		D_DPDA_DPD_STATE_HIGHSPEED				2				/*!< 高速							*/
/* @} */

/*!
 * @name DPD SHELL動作
 * @note DPD SHELL動作
 * @{
 */
#define		D_DPDA_SHELL_PROC						0				/*!< プロセス動作					*/
#define		D_DPDA_SHELL_RUN_SHELL					1				/*!< SHELL動作						*/
#define		D_DPDA_SHELL_STOP_PROC					2				/*!< プロセス停止のみ(Shellも停止)	*/
#define		D_DPDA_SHELL_DOWNLOAD					3				/*!< Shellダウンロードで実行		*/
/* @} */

/*!
 * @name DPDAタスク LUT設定関連
 * @note DPDAタスク LUT設定関連定義
 * @{
 */
/* キャリアConfigによるLUT設定 占有帯域	*/
#define	D_DPDA_CAR_CONFIG_LUT_BW_NUM				11				/*!< キャリアConfigによるLUT設定 占有帯域数	*/

#define	D_DPDA_BW_CDMA								1250			/*!< CDMAのbandwidth						*/

#define	D_DPDA_LUT_LOW_POWER						0				/*!< LUT設定(低電力)						*/
#define	D_DPDA_LUT_HIGH_POWER						1				/*!< LUT設定(高電力)						*/

#define	D_DPDA_LUT_POW_HIGH							330				/*!< 高電力									*/
#define	D_DPDA_LUT_POW_MIDDLE						300				/*!< 通常電力								*/
#define	D_DPDA_LUT_POW_LOW							290				/*!< 低電力									*/

#define	D_DPDA_LUT_LOWPOW_CNT						10				/*!< 低電力カウンタ							*/

#define	D_DPDA_LUT_MAX								22				/*!< LUT設定時に設定するレジスタ数			*/
#define	D_DPDA_RX_ANTCAR_STOP_MAX					6				/*!< Rxの停波処理レジスタ数 */

/* @} */

/*!
 * @name ピーク抑圧(占有帯域)用定義
 * @note ピーク抑圧(占有帯域)用定義
 * @{
 */
enum
{
	E_DPDA_PEAK_BAND0 = 0,									/*!< 占有帯域0～30						*/
	E_DPDA_PEAK_BAND1,										/*!< 占有帯域30～40						*/
	E_DPDA_PEAK_BAND2,										/*!< 占有帯域40～50						*/
	E_DPDA_PEAK_BAND3,										/*!< 占有帯域50～						*/
	E_DPDA_PEAK_BAND_MAX
};
/* @} */

/* ピーク抑圧設定用	*/
enum
{
	E_DPDA_STRAIN_PEAK_TXPRM0 = 0,							/*!< TXPRMx_GAIN(car1)					*/
	E_DPDA_STRAIN_PEAK_TXPRM1,								/*!< TXPRMx_GAIN(car2)					*/
	E_DPDA_STRAIN_PEAK_LPFGAIN0,							/*!< LPF Gain(car1)						*/
	E_DPDA_STRAIN_PEAK_LPFGAIN1,							/*!< LPF Gain(car2)						*/
	E_DPDA_STRAIN_PEAK_ICFR,								/*!< ICFR								*/
	E_DPDA_STRAIN_PEAK_ECFR,								/*!< ECFR								*/
	E_DPDA_STRAIN_PEAK_MAX
};

/* ピーク抑圧閾値キャリア番号判定用	*/
enum
{
	E_DPDA_STRAIN_PEAK_CAR0 = 0,							/*!< ピーク抑圧閾値(キャリア0)			*/
	E_DPDA_STRAIN_PEAK_CAR1,								/*!< ピーク抑圧閾値(キャリア1)			*/
	E_DPDA_STRAIN_PEAK_CAR2,								/*!< ピーク抑圧閾値(キャリア2)			*/
	E_DPDA_STRAIN_PEAK_CAR3,								/*!< ピーク抑圧閾値(キャリア3)			*/
	E_DPDA_STRAIN_PEAK_CAR4,								/*!< ピーク抑圧閾値(キャリア4)			*/
	E_DPDA_STRAIN_PEAK_CAR5,								/*!< ピーク抑圧閾値(キャリア5)			*/
	E_DPDA_STRAIN_PEAK_CAR6,								/*!< ピーク抑圧閾値(キャリア6)			*/
	E_DPDA_STRAIN_PEAK_CAR7,								/*!< ピーク抑圧閾値(キャリア7)			*/
	E_DPDA_STRAIN_PEAK_CAR_MAX
};

enum
{
	E_DPDA_PEAK_OFF = 0,									/*!< キャリアOFF						*/
	E_DPDA_PEAK_ON,											/*!< キャリアON							*/
	E_DPDA_PEAK_MAX
};


/*!
 * @name BPF READ/WRITE振り分け
 * @note BPF READ/WRITE振り分け用定義
 * @{
 */
enum
{
	E_DPDA_BPF_NOMAL_MODE = 0,								/*!< 通常read/write						*/
	E_DPDA_BPF_BITON_MODE,									/*!< Bit ON write						*/
	E_DPDA_BPF_BITOFF_MODE,									/*!< Bit OFF write						*/
	E_DPDA_BPF_CPLD_MODE,									/*!< SPI(CPLD) read/write				*/
	E_DPDA_BPF_FEFB_MODE,									/*!< SPI(FEFB) read/write				*/
	E_DPDA_BPF_SPI_RX_MODE,									/*!< SPI(RX) read/write					*/
	E_DPDA_BPF_EEPROM_MODE,									/*!< EEPROM read/write					*/
	E_DPDA_BPF_TMP_MODE,									/*!< 温度情報取得(1度単位の取得)		*/
	E_DPDA_BPF_FEFB_MODE_NOLOG,								/*!< SPI(FEFB) read/write(log無し)		*/
	E_DPDA_BPF_SPI_RX_MODE_NOLOG,							/*!< SPI(RX) read/write(log無し)		*/
	E_DPDA_BPF_CPLD_MODE_NOLOG,								/*!< SPI(CPLD) read/write(log無し)		*/
	E_DPDA_BPF_TMP_MODE2,									/*!< 温度情報取得2(0.5度単位の取得)		*/ /* 4TRxRRE ハソv1.11対応 */
	E_DPDA_BPF_RFPLL_MODE,									/*!< RFPLL read							*/
	E_DPDA_BPF_TXDAC_MODE,									/*!< TXDAC read							*/
	E_DPDA_BPF_RXADC_MODE									/*!< RXADC read							*/
};
/* @} */

/*!
 * @name DPDAアラーム演算子
 * @note DPDAアラーム演算子定義
 * @{
 */
enum
{
	E_DPDA_ALM_OPE_AND=0,									/*!< AND								*/
	E_DPDA_ALM_OPE_OR,										/*!< OR									*/
	E_DPDA_ALM_OPE_GT,										/*!< greater than						*/
	E_DPDA_ALM_OPE_GE,										/*!< greater than equal to				*/
	E_DPDA_ALM_OPE_LT,										/*!< less than							*/
	E_DPDA_ALM_OPE_LE,										/*!< less than equal to					*/
	E_DPDA_ALM_OPE_EQ,										/*!< equal to							*/
	E_DPDA_ALM_OPE_NE,										/*!< not equal to						*/
	E_DPDA_ALM_OPE_BITOFF,									/*!< bit off							*/
	E_DPDA_ALM_OPE_BITON,									/*!< bit on								*/
	E_DPDA_ALM_OPE_GE_SIGNED,								/*!< greater than equal to(signed)		*/
	E_DPDA_ALM_OPE_GT_SIGNED,								/*!< greater than(signed)				*/
	E_DPDA_ALM_OPE_LE_SIGNED,								/*!< less than equal to(signed)			*/
	E_DPDA_ALM_OPE_LT_SIGNED,								/*!< less than(signed)					*/
	E_DPDA_ALM_OPE_NUM										/*!< アラーム演算子数					*/
};
/* @} */

/*!
 * @name DPDAアラーム演算子(要因マージ用)
 * @note DPDAアラーム演算子(要因マージ用)定義
 * @{
 */
enum
{
	E_DPDA_ALM_FACTOR_OPE_AND=0,							/*!< AND								*/
	E_DPDA_ALM_FACTOR_OPE_OR,								/*!< OR									*/
	E_DPDA_ALM_FACTOR_OPE_OFF_BEF_ON_AFT,					/*!< 回復中:前半条件,発生中:後半条件	*/
	E_DPDA_ALM_FACTOR_OPE_NUM								/*!< アラーム演算子数(要因マージ用)		*/
};
/* @} */


/*!
 * @name DPDAアラームハード収集情報(ソフト整形) f_dpda_almCollectInfoで使用
 * @note DPDAアラームハード収集情報(ソフト整形)定義
 * @{
 */
enum
{
	E_DPDA_ALM_COLLECT_INFO_TX_GAIN_ANTA,					/* TX out of order TX Gain ALM#AntA Fb信号積分値/Ref信号電力値 */
	E_DPDA_ALM_COLLECT_INFO_TX_GAIN_ANTB,					/* TX out of order TX Gain ALM#AntB Fb信号積分値/Ref信号電力値 */
	E_DPDA_ALM_COLLECT_INFO_TX_GAIN_ANTC,					/* TX out of order TX Gain ALM#AntC Fb信号積分値/Ref信号電力値 */
	E_DPDA_ALM_COLLECT_INFO_TX_GAIN_ANTD,					/* TX out of order TX Gain ALM#AntD Fb信号積分値/Ref信号電力値 */
	
	E_DPDA_ALM_COLLECT_INFO_RX_CAR_A,		 				/*!< Rx Carrier状態(Ant A)				*/
	E_DPDA_ALM_COLLECT_INFO_RX_CAR_B,		 				/*!< Rx Carrier状態(Ant B)				*/
	E_DPDA_ALM_COLLECT_INFO_RX_CAR_C,		 				/*!< Rx Carrier状態(Ant C)				*/
	E_DPDA_ALM_COLLECT_INFO_RX_CAR_D,		 				/*!< Rx Carrier状態(Ant D)				*/

	E_DPDA_ALM_COLLECT_INFO_TX_CAR_A,		 				/*!< Tx Carrier状態(Ant A)				*/
	E_DPDA_ALM_COLLECT_INFO_TX_CAR_B,		 				/*!< Tx Carrier状態(Ant B)				*/
	E_DPDA_ALM_COLLECT_INFO_TX_CAR_C,		 				/*!< Tx Carrier状態(Ant C)				*/
	E_DPDA_ALM_COLLECT_INFO_TX_CAR_D,		 				/*!< Tx Carrier状態(Ant D)				*/

	E_DPDA_ALM_COLLECT_INFO_ANT_STAT_A,		 				/*!< アンテナ状態(Ant A)				*/
	E_DPDA_ALM_COLLECT_INFO_ANT_STAT_B,		 				/*!< アンテナ状態(Ant B)				*/
	E_DPDA_ALM_COLLECT_INFO_ANT_STAT_C,		 				/*!< アンテナ状態(Ant C)				*/
	E_DPDA_ALM_COLLECT_INFO_ANT_STAT_D,		 				/*!< アンテナ状態(Ant D)				*/

	E_DPDA_ALM_COLLECT_INFO_ANT_REF_A,
	E_DPDA_ALM_COLLECT_INFO_ANT_REF_B,
	E_DPDA_ALM_COLLECT_INFO_ANT_REF_C,
	E_DPDA_ALM_COLLECT_INFO_ANT_REF_D,

	E_DPDA_ALM_COLLECT_INFO_ANT_RXPOW_A,
	E_DPDA_ALM_COLLECT_INFO_ANT_RXPOW_B,
	E_DPDA_ALM_COLLECT_INFO_ANT_RXPOW_C,
	E_DPDA_ALM_COLLECT_INFO_ANT_RXPOW_D,

	E_DPDA_ALM_COLLECT_INFO_A000_0200,
	E_DPDA_ALM_COLLECT_INFO_A000_0210,
	E_DPDA_ALM_COLLECT_INFO_A000_0B10,
	E_DPDA_ALM_COLLECT_INFO_A000_0B20,
	E_DPDA_ALM_COLLECT_INFO_A000_0B30,
	E_DPDA_ALM_COLLECT_INFO_B000_0B10,
	E_DPDA_ALM_COLLECT_INFO_B000_0B20,
	E_DPDA_ALM_COLLECT_INFO_B000_0200,
	E_DPDA_ALM_COLLECT_INFO_B000_0210,
	E_DPDA_ALM_COLLECT_INFO_B000_02C0,

	E_DPDA_ALM_COLLECT_INFO_B100_0040,
	E_DPDA_ALM_COLLECT_INFO_B100_0044,
	E_DPDA_ALM_COLLECT_INFO_B100_0048,
	E_DPDA_ALM_COLLECT_INFO_B100_004C,

	E_DPDA_ALM_COLLECT_INFO_RFPLL_6E,						/*!< RF-PLL Unlock of Module Clock Faulty(mmW)	*/
	E_DPDA_ALM_COLLECT_INFO_TXDAC_281,						/*!< TXDAC ALM of RF module fault(mmW)				*/
	E_DPDA_ALM_COLLECT_INFO_TXDAC_7B5,						/*!< TXDAC ALM of RF module fault(mmW)				*/
	E_DPDA_ALM_COLLECT_INFO_RXADC_56F,						/*!< RXADC ALM of RF module fault(mmW)				*/
	E_DPDA_ALM_COLLECT_INFO_RXADC_11B,						/*!< RXADC ALM of RF module fault(mmW)				*/

	E_DPDA_ALM_COLLECT_INFO_A200_0720,						/*!< ハード自律PA-OFF[0] 1: Hard自律送信 停止中、0:通常	*/
	E_DPDA_ALM_COLLECT_INFO_A201_43C8,						/*!< ハード自律PA-OFF[0] 1: Hard自律送信 停止中、0:通常	*/
	
    E_DPDA_ALM_COLLECT_INFO_B100_0050,
	E_DPDA_ALM_COLLECT_INFO_B100_0054,
	E_DPDA_ALM_COLLECT_INFO_B100_0058,
	E_DPDA_ALM_COLLECT_INFO_B100_005C,

	E_DPDA_ALM_COLLECT_INFO_NUM								/*!< 収集情報数							*/
};
/* @} */

/*!
 * @name システムパラメータ設定範囲
 * @note システムパラメータ設定範囲定義
 * @{
 */
enum
{
	E_DPDA_SYS_PARA_SET_START=0,							/*!< 設定開始範囲							*/
	E_DPDA_SYS_PARA_SET_END,								/*!< 設定終了								*/
	E_DPDA_SYS_PARA_SET_NUM
};
/* @} */

/*!
 * @name VG設定範
 * @note VG設定定義
 * @{
 */
/* RRH用 */
enum
{
	E_DPDA_VG_YA1=0,
	E_DPDA_VG_YA2,
	E_DPDA_VG_PA1,
	E_DPDA_VG_PA2,
	E_DPDA_VG_CA1,
	E_DPDA_VG_CA2,
	E_DPDA_VG_TYPE_MAX
};

enum
{
	E_DPDA_VG1=0,
	E_DPDA_VG2,
	E_DPDA_VG_MAX
};
/* @} */

/* TSSI設定用		*/
enum
{
	E_DPDA_SET_TSSIX = 0,									/*!< TSSI パラメータX						*/
	E_DPDA_SET_TSSIY_H,										/*!< TSSI パラメータY(HIGH)					*/
	E_DPDA_SET_TSSIY_L,										/*!< TSSI パラメータY(LOW)					*/
	E_DPDA_SET_TSSI_MAX
};

/*!
 * @name 温度補正関連
 * @note 温度補正関連定義
 * @{
 */
#define	D_DPDA_TEMP_ADJ_COEF_NUM				221									/*!< 温度補償係数数							*/
#define	D_DPDA_TEMP_ADJ_COEF_DIV_VAL			1000000000.0						/*!< 温度補償係数(割る数)					*/
#define	D_DPDA_TEMP_ADJ_TEMP_MIN				0									/*!< 温度補償実施温度最少値					*/
#define	D_DPDA_TEMP_ADJ_TEMP_MAX				D_DPDA_TEMP_ADJ_COEF_NUM - 1		/*!< 温度補償実施温度最大値					*/
#define	D_DPDA_TEMP_ADJ_KIND_INDEX_37G			0									/*!< 温度補償INDEX							*/
#define	D_DPDA_TEMP_ADJ_KIND_INDEX_45G			1									/*!< 温度補償INDEX							*/
#define	D_DPDA_TEMP_ADJ_KIND_INDEX_280G			2									/*!< 温度補償INDEX							*/


enum
{
	E_DPDA_TEMP_ADJ_TMP=0,									/*!< センサー温度(℃)						*/
	E_DPDA_TEMP_ADJ_COEF,									/*!< 温度補正係数							*/
	E_DPDA_TEMP_ADJ_PARAM_MAX
};
/* @} */

/*!
 * @name 送信ランプアップ関連
 * @note 送信ランプアップ関連定義
 * @{
 */
#define	D_DPDA_TX_RAMPUP_NUM					5			/*!< 送信ランプアップ設定数					*/
/* @} */

/*!
 * @name キャリブレーション状態関連
 * @note キャリブレーション状態関連定義
 * @{
 */
#define	D_DPDA_CALSTATE_NONE					0			/*!< キャリブレーション未実施 				*/
#define	D_DPDA_CALSTATE_ZEROAMPLOLAD			1			/*!< 0振幅補正								*/
#define	D_DPDA_CALSTATE_QEC_LOC					2			/*!< 装置送信状態時のQEC(Quadrature Error Correction：直交誤差補正)およびLo Calibration		*/

#define	D_DPDA_CALSTATE_DEACTIVATE				0			/*!< キャリブレーション設定未実行状態		*/
#define	D_DPDA_CALSTATE_ACTIVATE				1			/*!< キャリブレーション設定実行中			*/

#define	D_DPDA_CALIB_STOP						0			/*!< 処理停止								*/
#define	D_DPDA_CALIB_CONT						1			/*!< 処理継続								*/
#define	D_DPDA_CALIB_SKIP						2			/*!< 処理をスキップする						*/

#define	D_DPDA_CAL_TX_DPHASFTSEL_OFFSET			0xA2014184	/*!< TX DPHASFTSELアドレス					*/
#define	D_DPDA_CAL_TX_DCALCOEFREA_OFFSET		0xA2014800	/*!< TX DCALCOEFREAアドレス					*/
#define	D_DPDA_CAL_TX_DCALENB_OFFSET		    0xA2014180	/*!< TX DCALENBアドレス				    	*/
#define	D_DPDA_CAL_TX_EEP_EQ_OFFSET				0x41A		/*!< TX_EQアドレス(EEPROM)					*/

#define	D_DPDA_CAL_RX_DPHASFTSEL_OFFSET			0xA2204184	/*!< RX DPHASFTSELアドレス					*/
#define	D_DPDA_CAL_RX_DCALCOEFREA_OFFSET		0xA2204800	/*!< RX DCALCOEFREAアドレス					*/
#define	D_DPDA_CAL_RX_DCALENB_OFFSET		    0xA2204180	/*!< RX DCALENBアドレス				    	*/
#define	D_DPDA_CAL_RX_RFCALREGSEL_OFFSET		0xA2401184	/*!< RX DPHASFTSELアドレス					*/
#define	D_DPDA_CAL_RX_RFCALCOEF_OFFSET			0xA2402000	/*!< RX DCALCOEFREAアドレス					*/
#define	D_DPDA_CAL_RX_RFCALENB_OFFSET		    0xA2401180	/*!< RX DCALENBアドレス				    	*/

#define	D_DPDA_CAL_RX_EEP_EQ_OFFSET				0x702		/*!< RX_EQアドレス(EEPROM)					*/

#define	D_DPDA_CAL_FREQ_NUM                     4           /*!< CALを行う周波数の数                */
#define D_DPDA_FREQ_F1                                 0           /*!< 周波数f1                */
#define D_DPDA_FREQ_F2                                 1           /*!< 周波数f2                */
#define D_DPDA_FREQ_F3                                 2           /*!< 周波数f3                */
#define D_DPDA_FREQ_F4                                 3           /*!< 周波数f4                */
#define D_DPDA_FREQ_FAST                               4           /*!< 周波数fast              */
#define	D_DPDA_CAL_COEF_NUM                          256           /*!< CAL係数反映設定先レジスタ数*/
/* @} */

/*!
 * @name レジスタ初期設定関連
 * @note レジスタ初期設定関連定義
 * @{
 */
#define D_DPDA_BEAMFORMING_MODE                 0x00        /*!< ビームフォーミング設定                 */
#define D_DPDA_INIT_REG_NUM_SUB6                47          /*!< 初期値設定 レジスタ数(sub6用)          */
#define D_DPDA_INIT_REG_NUM_MMW                 91          /*!< 初期値設定 レジスタ数(mmW用)           */
#define D_DPDA_INIT2_REG_NUM_SUB6              104          /*!< 初期値設定(BeamID設定追加(41項～61項)) レジスタ数(sub6用)          */
															/*!< (過出力フェールセーフ初期設定(1項～83項))	*//* @} */
#define D_DPDA_INIT2_REG_NUM_MMW               125          /*!< 初期値設定(過出力フェールセーフ初期設定(1項～125項)) レジスタ数(mmW用)          */



/*!
 * @name TDD関連
 * @note TDD関連定義
 * @{
 */
#define	D_DPDA_TDD_CONFIG_NUM					7			/*!< TDD Config Table数										*/
#define	D_DPDA_TDD_CONFIG_ADDR_NUM				10			/*!< TDD Config レジスタ数									*/
#define	D_DPDA_TDD_SPSUBFRAM_NORMAL_NUM			10			/*!< Special Subframe Configuration Downlink Normal CP		*/
#define	D_DPDA_TDD_SPSUBFRAM_EXTEND_NUM			8			/*!< Special Subframe Configuration Downlink Extended CP	*/
#define	D_DPDA_TDD_ENABLE						0x00000001	/*!< Tx-ENABLE設定：Enable									*/
#define	D_DPDA_TDD_DISABLE						0x00000000	/*!< Tx-ENABLE設定：Disable									*/
#define	D_DPDA_TDD_HARD_OFF_MON_INTERVAL		100			/*!< TDD 自律OFF周期監視間隔								*/
#define	D_DPDA_TDD_CONFIG_SWTIME_VAL			0x00025800	/*!< TDD Config Table設定時のSWTIME設定値					*/
#define	D_DPDA_TDD_CONFIG_DUL100_SWTIME_VAL		0x00025800	/*!< TDD Config DL/UL100%時のSWTIME設定値					*/
#define	D_DPDA_TDD_CONFIG_DL100					0xFFFF		/*!< TDD Config DL100%時のConfig Table Index				*/
#define	D_DPDA_TDD_COMMON_REG_NUM				51			/*!< TDD 共通設定設定 レジスタ数(sub6用)					*/
#define	D_DPDA_TDD_COMMON_REG_NUM_MMW			36			/*!< TDD 共通設定設定 レジスタ数(mmW用)						*/

/* @} */

/*!
 * @name NCO関連
 * @note NCO関連定義
 * @{
 */
#define	D_DPDA_TX_NCO_MAX						3071		/*!< Tx側NCO最大値							*/
#define	D_DPDA_TX_NCO_MIN						(-3072)		/*!< Tx側NCO最小値							*/
#define	D_DPDA_RX_NCO_MAX						1535		/*!< Rx側NCO最大値							*/
#define	D_DPDA_RX_NCO_MIN						(-1536)		/*!< Rx側NCO最小値							*/
#define	D_DPDA_NCO_THRESHOLD_35G				3500000		/*!< 3.5G(khz単位にした場合)				*/
#define	D_DPDA_TX_NCO_REGISTER_MASK				0x00001FFF	/*!< Tx側NCO設定時のマスク値				*/
#define	D_DPDA_RX_NCO_REGISTER_MASK				0x00000FFF	/*!< Rx側NCO設定時のマスク値				*/
#define	D_DPDA_NCO_THRESHOLD					20			/*!< NCO算出時の分母						*/
/* @} */

/*!
 * @name インパルス係数関連
 * @note インパルス係数関連定義
 * @{
 */
#define	D_DPDA_IMPULSE_NUM							255				/*!< インパルス係数の数					*/
#define	D_DPDA_IMPULSE_BANK_TOP_ADDRESS				0x82010000		/*!< IMPULSE設定のためbankのアドレス	*/ /* 4TRxRRE 82030000 -> 82010000 */
#define	D_DPDA_IMPULSE_BLOCK_NUM					12				/*!< IMPULSE設定するブロック数			*/
#define	D_DPDA_IMPULSE_BLOCK_OFFSET					0x00000800		/*!< bank内ブロック毎のオフセット		*/
#define	D_DPDA_IMPULSE_SET_OFFSET					0x00000300		/*!< ブロック内IMPULSE設定オフセット	*/
#define	D_DPDA_IMPULSE_START_UPDATE					(D_RRH_CC_IMPMEM_ACS|D_RRH_CC_IMPMEM_BEN)						/*!< bank設定開始						*/
#define	D_DPDA_IMPULSE_UPDATE_IMPULSE				(D_RRH_CC_IMPMEM_CNG|D_RRH_CC_IMPMEM_ACS|D_RRH_CC_IMPMEM_BEN)	/*!< bank切り替え						*/
#define	D_DPDA_IMPULSE_END_UPDATE					(D_RRH_CC_IMPMEM_BEN)											/*!< bank設定終了						*/
#define	D_DPDA_IMPULSE_BSY_CHK_INTERVAL				1				/*!< bsy bitチェック間隔(ms)			*/
#define	D_DPDA_IMPULSE_TABLE_TYPE_CAR2				0				/*!< Carrier数2の場合のインパルス係数テーブル			*/
#define	D_DPDA_IMPULSE_TABLE_TYPE_CAR1_NCO_IS_0		1				/*!< Carrier数1,NCO==0の場合のインパルス係数テーブル	*/
#define	D_DPDA_IMPULSE_TABLE_TYPE_NUM				2				/*!< インパルス係数テーブルの種類の数 */
#define	D_DPDA_IMPULSE_SET_ANT_ATOD_OFFSET          0xB1117000      /*!< インパルス応答更新メモリ(ANT:A~D) */
#define	D_DPDA_IMPULSE_SET_ANT_ETOH_OFFSET          0xB3117000      /*!< インパルス応答更新メモリ(ANT:A~D) */
#define	D_DPDA_IMPULSE_SET_CFR_OFFSET               0xB1100248      /*!< CFRオフセット */
#define	D_DPDA_IMPULSE_SET_ICFRB_OFFSET             0xB1100240      /*!< ICFRB */
#define	D_DPDA_IMPULSE_SET_ICFRA_OFFSET             0xB1100244      /*!< ICFRA */
#define	D_DPDA_IMPULSE_SET_ECFR_OFFSET              0xB1100280      /*!< ECFR  */
#define	D_DPDA_IMPULSE_SET_BASE_DATA                0x00030001      /*!< インパルスメモリ更新設定値 */
#define	D_DPDA_IMPULSE_SET_ICFR_DATA                0x02000C6C      /*!< ICFR設定値 */
#define	D_DPDA_IMPULSE_SET_ECFR_DATA                0x00000C6C      /*!< ECFR設定値 */
#define	D_DPDA_IMPULSE_SET_ICFR_DATA_45G            D_DPDA_IMPULSE_SET_ICFR_DATA      /*!< ICFR設定値 */
#define	D_DPDA_IMPULSE_SET_ECFR_DATA_45G            D_DPDA_IMPULSE_SET_ECFR_DATA      /*!< ECFR設定値 */


/* @} */

/*!
 * @name DPD関連
 * @note DPD関連定義
 * @{
 */
#define	D_DPDA_DPD_LUT_PHASE_OFF					0x00100000	/*!< DPD位相調整設定値OFF			*//* 4TRx apdハソ 5-6-2 指摘修正 */
#define	D_DPDA_DPD_LUT_PHASE_ON						0x00101000	/*!< DPD位相調整設定値ON			*/
#define	D_DPDA_DPD_START							0x00000037	/*!< DPD開始設定値					*/
#define	D_DPDA_DPD_STOP								0x00000000	/*!< DPD停止設定値					*/
/* @} */

/*!
 * @name aux設定関連
 * @note aux設定関連定義
 * @{
 */
/* aux設定データパターン */
enum
{
	E_DPDA_AUX_SET_DATA_4BR_2CAR = 0,							/*!< シスパラ(4br、cr1 有効、cr2 有効)		*/
	E_DPDA_AUX_SET_DATA_2BR_2CAR,								/*!< シスパラ(2br、cr1 有効、cr2 有効)		*/
	E_DPDA_AUX_SET_DATA_XBR_CAR1_ON,							/*!< シスパラ(Xbr、cr1 有効、cr2 無効)		*/
	E_DPDA_AUX_SET_DATA_XBR_CAR2_ON,							/*!< シスパラ(Xbr、cr1 無効、cr2 有効)		*/
	E_DPDA_AUX_SET_DATA_NUM										/*!< branch,carrier組み合わせ				*/
};

/* aux設定データ数 */
#define	D_DPDA_AUX_SET_PARAM_NUM					40			/*!< Aux設定データ数						*/
#define	D_DPDA_AUX_SET_PARAM_NUM_RRE				20			/*!< Aux設定データ数(RRE用)					*/

/* aux設定アドレスIndex */
enum
{
	E_DPDA_AUX_SET_ADDR_1CAR = 0,								/*!< 1キャリア構成							*/
	E_DPDA_AUX_SET_ADDR_2CAR,									/*!< 2キャリア構成							*/
	E_DPDA_AUX_SET_ADDR_NUM
};

/* CPRI Rate */
enum
{
	E_DPDA_CPRI_RATE_UNKNOWN = 0,
	E_DPDA_CPRI_RATE_49G,										/*!< CPRI Rate(4.9G)						*/
	E_DPDA_CPRI_RATE_98G,										/*!< CPRI Rate(9.8G)						*/
	E_DPDA_CPRI_RATE_NUM										/*!< CPRI Rate数							*/
};

/* 送信系統数 */
enum
{
	E_DPDA_TX_SYS_2 = 0,										/*!< 送信系統数(2)							*/
	E_DPDA_TX_SYS_4,											/*!< 送信系統数(4)							*/
	E_DPDA_TX_SYS_NUM											/*!< 送信系統数								*/
};
#define	D_DPDA_TX_SYS_NUM_VAL_2						2			/*!< 送信系統数(2)							*/
#define	D_DPDA_TX_SYS_NUM_VAL_4						4			/*!< 送信系統数(2)							*/

/* 送信系統数 */
enum
{
	E_DPDA_CAR_NUM_1 = 0,										/*!< キャリア数(1)							*/
	E_DPDA_CAR_NUM_2,											/*!< キャリア数(2)							*/
	E_DPDA_CAR_NUM												/*!< キャリア数								*/
};
#define	D_DPDA_CAR_NUM_1							1			/*!< キャリア数(1)							*/
#define	D_DPDA_CAR_NUM_2							2			/*!< キャリア数(2)							*/

/* @} */


/*!
 * @name 周期電力収集ラッチ情報
 * @note 周期電力収集ラッチ情報
 * @{
 */
enum
{
	E_DPDA_PERI_POWER_LAT_TSSI = 0,		/* TSSI								*/
	E_DPDA_PERI_POWER_LAT_RXPOW,		/* Rx Power							*/
	E_DPDA_PERI_POWER_LAT_RTWP,			/* RTWP								*/
	E_DPDA_PERI_POWER_LAT_NUM			/* ラッチ情報数						*/
};
/* @} */


/*!
 * @name アンテナ電力計算用定数定義
 * @note アンテナ電力計算用定数定義
 * @{
 */
enum
{
	E_DPDA_PERI_ANT_POWER_Y = 0,				/*!< 定数Y							*/
	E_DPDA_PERI_ANT_POWER_Z,					/*!< 定数Z							*/
	E_DPDA_PERI_ANT_POWER_NUM					/*!< 定数の個数						*/
};

/*!
 * @name 送信電力計算用定数定義
 * @note 送信電力計算用定数定義
 * @{
 */
enum
{
	E_DPDA_PERI_TX_POWER_Y = 0,					/*!< 定数Y							*/
	E_DPDA_PERI_TX_POWER_Z,						/*!< 定数Z							*/
	E_DPDA_PERI_TX_POWER_NUM					/*!< 定数の個数						*/
};


/*!
 * @name ADC受信電力計算用定数定義
 * @note ADC受信電力計算用定数定義
 * @{
 */
enum
{
	E_DPDA_PERI_ADC_RX_POWER_Q0 = 0,				/*!< q=0のとき						*/
	E_DPDA_PERI_ADC_RX_POWER_Q1,					/*!< q=1のとき						*/
	E_DPDA_PERI_ADC_RX_POWER_Q2,					/*!< q=2のとき						*/
	E_DPDA_PERI_ADC_RX_POWER_OTHER_Q,				/*!< q=0～2以外						*/
	E_DPDA_PERI_ADC_RX_POWER_NUM					/*!< 定数の個数						*/
};

/*!
 * @name キャリア受信電力計算用定数定義
 * @note キャリア受信電力計算用定数定義
 * @{
 */
enum
{
	E_DPDA_PERI_CAR_RX_POWER_Q0 = 0,				/*!< q=0のとき						*/
	E_DPDA_PERI_CAR_RX_POWER_Q1,					/*!< q=1のとき						*/
	E_DPDA_PERI_CAR_RX_POWER_Q2,					/*!< q=2のとき						*/
	E_DPDA_PERI_CAR_RX_POWER_OTHER_Q,				/*!< q=0～2以外						*/
	E_DPDA_PERI_CAR_RX_POWER_NUM					/*!< 定数の個数						*/
};

enum
{
	E_DPDA_PERI_CAR_RX_POWER_REP_H = 0,				/*!< 定数Yの分割上部				*/
	E_DPDA_PERI_CAR_RX_POWER_REP_L,					/*!< 定数Yの分割下部				*/
	E_DPDA_PERI_CAR_RX_POWER_REP_NUM				/*!< 定数の個数						*/
};
/*!
 * @name フェールセーフ用定義
 * @note フェールセーフ用定義
 * @{
 */
#define	D_DPDA_FB_FSDOMASK_WRITE		0x00000025	/*!< FSDOMASKライト値				*/
#define	D_DPDA_FB_FSDOCTRL_WRITE		0x01007800	/*!< FSDOCTRLライト値				*/
#define	D_DPDA_FB_FSDOMODE_WRITE		0x03000002	/*!< FSDOMODEライト値				*/
#define	D_DPDA_FB_AFSDORPT_MASK			0x00000400	/*!< AFSDORPTリード用マスク値		*/

/*!
 * @name デフォルト温度ALM閾値定義
 * @note デフォルト温度ALM閾値定義
 * @{
 */
#define	D_DPDA_DEFAULT_TEMP_ALM_THRESHOLD	100		/*!< デフォルト温度ALM閾値 sub6,mmW用	*/


enum
{
	E_DPDA_REKIND_5GDU_SUB6 = 0,	/*!< 装置種別(sub6)				*/
	E_DPDA_REKIND_5GDU_MMW,			/*!< 装置種別(mmW)				*/
	E_DPDA_REKIND_5GDU_NUM			/*!< 装置種別の個数				*/
};

#define	D_DPDA_RECOVER_HIGH_TEMP_ALM	30			/*!< 温度異常アラーム回復用閾値								*/
													/*!< 温度アラーム閾値から引き算して回復用の閾値として使用	*/

enum
{
	E_DPDA_RUINDEX_SUB6_37G = 0,	/*!< 装置種別(sub6 3.7G)		*/
	E_DPDA_RUINDEX_SUB6_45G,		/*!< 装置種別(sub6 4.5G)		*/
	E_DPDA_RUINDEX_MMW_280G,		/*!< 装置種別(mmW 280G)			*/
	E_DPDA_RUINDEX_NUM				/*!< 装置種別の個数				*/
};

enum
{
	E_DPDA_SCS_15KHZ = 0 ,							/*!< SCS 15KHz未満(mmw未対応)			*/
	E_DPDA_SCS_30KHZ ,								/*!< SCS 30KHz未満(mmw未対応)			*/
	E_DPDA_SCS_60KHZ ,								/*!< SCS 60KHz未満(mmw未対応)			*/
	E_DPDA_SCS_120KHZ ,								/*!< SCS 120KHz							*/
	E_DPDA_SCS_240KHZ								/*!< SCS 240KHz							*/
};

enum
{
	E_DPDA_BW_10MHZ = 0,							/*!< BW 0:10MHz							*/
	E_DPDA_BW_20MHZ ,								/*!< BW 0:20MHz							*/
	E_DPDA_BW_30MHZ ,								/*!< BW 0:30MHz							*/
	E_DPDA_BW_40MHZ ,								/*!< BW 0:40MHz							*/
	E_DPDA_BW_50MHZ ,								/*!< BW 0:50MHz							*/
	E_DPDA_BW_60MHZ ,								/*!< BW 0:60MHz							*/
	E_DPDA_BW_70MHZ ,								/*!< BW 0:70MHz							*/
	E_DPDA_BW_80MHZ ,								/*!< BW 0:80MHz							*/
	E_DPDA_BW_90MHZ ,								/*!< BW 0:90MHz							*/
	E_DPDA_BW_100MHZ ,								/*!< BW 0:100MHz						*/
	E_DPDA_BW_200MHZ ,								/*!< BW 0:200MHz						*/
	E_DPDA_BW_400MHZ								/*!< BW 0:400MHz						*/
};

#define	D_DPDA_BW_10MHZ				10000000		/* 周波数が10MHz */
#define	D_DPDA_BW_20MHZ				20000000		/* 周波数が20MHz */
#define	D_DPDA_BW_30MHZ				30000000		/* 周波数が30MHz */
#define	D_DPDA_BW_40MHZ				40000000		/* 周波数が40MHz */
#define	D_DPDA_BW_50MHZ				50000000		/* 周波数が50MHz */
#define	D_DPDA_BW_60MHZ				60000000		/* 周波数が60MHz */
#define	D_DPDA_BW_70MHZ				70000000		/* 周波数が70MHz */
#define	D_DPDA_BW_80MHZ				80000000		/* 周波数が80MHz */
#define	D_DPDA_BW_90MHZ				90000000		/* 周波数が90MHz */
#define	D_DPDA_BW_100MHZ			100000000		/* 周波数が100MHz */
#define	D_DPDA_BW_200MHZ			200000000		/* 周波数が200MHz */
#define	D_DPDA_BW_400MHZ			400000000		/* 周波数が400MHz */

#define	D_DPDA_FFT_SAMPL_SUB6		4				/* 設定値			*/
#define	D_DPDA_FFT_SAMPL_MMW		4				/* 設定値			*/
#define	D_DPDA_FFT_SAMPL_MAX		0xFFFFFFFF		/* 設定値無効		*/
#define	D_DPDA_FFT_SAMPL_MAX_SUB6	0x0000007F		/* 設定最大値(sub6)	*/
#define	D_DPDA_FFT_SAMPL_MAX_MMW	0x00000047		/* 設定最大値(mmW)	*/

#define	D_DPDA_ULCPRMVMODE			0xa2204080		/* ULCPRMVMODEレジスタアドレス	*/
#define	D_DPDA_ULCPRMVMODE_SUB6		0xa2204080		/* Sub6用ULCPRMVMODEレジスタアドレス	*/
#define	D_DPDA_ULCPRMVMODE_MMW		0xa2204080		/* MMW用ULCPRMVMODEレジスタアドレス		*/
#define	D_DPDA_ULCPRMVMODE_ANT		0x02000000		/* MMW用相対ANTアドレス					*/

#define	D_DPDA_TA_OFFSET_DEV		4				/* 設定値/4			*/
#define	D_DPDA_TA_OFFSET_MASK		0x0000FFFF		/* マスクビット		*/
#define	D_DPDA_TA_OFFSET_MAX		0xFFFFFFFF		/* 設定値無効		*/

#define	D_DPDA_TCDLFRMAJ_ADDR_A		0xB1001000		/* TC UL-DL timing relation-A アドレス	*/
#define	D_DPDA_TCULFRMAJ_ADDR		0xA2000040		/* UL-DL timing relationアドレス		*/


/*!
 * @name TXCAL関連
 * @note TXCAL関連
 * @{
 */
#define	D_DPDA_TXANTCAL_REGINIT_NUM		30
#define	D_DPDA_TXANTCAL_SEND_NUM		37
#define	D_DPDA_TXANTCAL_PATH_NUM		5
#define	D_DPDA_TXANTCAL_END_NUM			19
#define	D_DPDA_ORXCAL_NUM				17
#define	D_DPDA_TXANTCAL_OPE_SEND_NUM    11      /* TXCAL信号送信設定  */
#define D_DPDA_TXANTCAL_OPE_PATH_NUM     5      /* TxCalPath設定     */
#define D_DPDA_TXANTCAL_OPE_PATH_RET_NUM 5      /* TxCalPath戻し設定 */
#define	D_DPDA_TXANTCAL_OPE_END_NUM     21      /* TXCAL信号送信終了 */

/*!
 * @name RXCAL関連
 * @note RXCAL関連
 * @{
 */
#define	D_DPDA_RXANTCAL_REGINIT_NUM	31
#define	D_DPDA_RXANTCAL_SEND_NUM	37
#define	D_DPDA_RXANTCAL_PATH_NUM	3
#define	D_DPDA_RXANTCAL_END_NUM		29
#define	D_DPDA_RXANTCAL_OPE_SEND_NUM     9      /* RXCAL信号送信設定 */
#define D_DPDA_RXANTCAL_OPE_PATH_NUM     5      /* RxCalPath設定     */
#define D_DPDA_RXANTCAL_OPE_PATH_RET_NUM 5      /* RxCalPath戻し設定 */
#define	D_DPDA_RXANTCAL_OPE_END_NUM      9      /* RXCAL信号送信終了 */

/*!
 * @name 1PPS SFNload関連
 * @note 1PPS SFNload関連
 * @{
 */
#define	D_DPDA_1PPS_INIT	0
#define	D_DPDA_1PPS_SET		1
#define	D_DPDA_1PPS_CMP		2

/* @} */
#endif
/* @} */
