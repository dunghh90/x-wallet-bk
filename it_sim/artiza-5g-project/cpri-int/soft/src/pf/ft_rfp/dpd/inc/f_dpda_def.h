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
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2015
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#ifndef F_DPDA_DEF_H
#define F_DPDA_DEF_H

/*!
 * @name DPDAタスク状態
 * @note DPDAタスクの状態定義
 * @{
 */
#define D_DPDA_TASK_STS_INIT						0x0000		/*!< 初期状態							*/
#define D_DPDA_TASK_STS_ACT							0x0001		/*!< 運用状態							*/
#define D_DPDA_STATENUM								0x0002		/*!< DPDAタスク状態数					*/
/* @} */

/*!
 * @name DPDAタスクイベント
 * @note DPDAタスクのイベント定義
 * @date 2015/08/06 TDI)satou 周期監視報告だけ残して他を削除
 * @{
 */
enum
{
	E_DPDA_EVE_POW_PERIODIC_NOTI,
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

/*!
 * 仮想レジスタ数.
 * @date 2015/08/07 TDI)satou 64→40に変更
 */
#define	D_DPDA_DBG_VIRTUAL_REG_CNT					40

/* 仮想レジスタアドレスオフセット		*/
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
/* @} */

/*!
 * @name DPDAタスクアンテナ関連
 * @note DPDAタスクアンテナ関連定義
 * @{
 */
#define	D_DPDA_ANTMAX								1	        /*!< TX/RXのANT数					*/
#define	D_DPDA_ANTA 								0			/*!< AntA								*/
/* @} */

/*!
 * @name DPDAタイマ間隔定義
 * @note DPDAタイマ間隔関連定義
 * @{
 */
#define D_DPDA_ONE_SHOT_TIME_1SEC			1000
/* @} */

/****************************************************************************************************/

/*!
 * @name DPDA検出アラーム閾値 ID
 * @note DPDA検出アラーム閾値 ID定義
 * @date 2015/08/06 TDI)satou 「温度異常アラーム」以外を削除
 * @{
 */
enum
{
	E_DPDA_ALM_THRESH_HIGH_TEMP = 0,						/*!< 温度異常アラーム					*/
	E_DPDA_ALM_THRESH_NUM_MAX
};

/*!
 * @name DPDA検出アラーム Alarm ID
 * @note DPDA検出アラーム Alarm ID定義
 * @date 2015/08/06 TDI)satou 「温度異常アラーム」以外を削除
 * @{
 */
enum
{
	E_DPDA_ALM_HIGH_TEMP,									/*!< 温度異常アラーム					*/
	E_DPDA_ALM_NUM_MAX
};
/* @} */

/*!
 * @name DPDAアラーム関連
 * @note DPDAアラーム関連定義
 * @{
 */
#define	D_DPDA_ALM_NUM								E_DPDA_ALM_NUM_MAX	/*!< DPDAアラーム数				*/
#define	D_DPDA_ALM_FACTOR_NUM						1			/*!< DPDAアラーム要因数					*/
/* @} */

/*!
 * @name DPDA検出アラームInstance ID
 * @note DPDA検出アラームInstance ID定義
 * @{
 */
#define D_DPDA_ALM_INST_HIGH_TEMP               D_RRH_ALMCD_TEMP                /*!< 温度異常アラーム(Ant#A)        */
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
 * @name DPDA検出アラームInstance ID
 * @note DPDA検出アラームInstance ID定義
 * @{
 */
#define	D_DPDA_ALM_INST_HIGH_TEMP				D_RRH_ALMCD_TEMP				/*!< 温度異常アラーム(Ant#A)		*/
/* @} */

/*!
 * @name DPDA使用マクロ
 * @note DPDA使用マクロ定義
 * @{
 */
#define	D_DPDA_MACRO_MIN(VAL1, VAL2)			( (VAL1) < (VAL2) ? (VAL1) : (VAL2) )			/*!< MIN														*/
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
 * @name BPF READ/WRITE振り分け
 * @note BPF READ/WRITE振り分け用定義
 * @date 2015/08/07 TDI)satou 「通常read/write」「温度情報取得」以外削除
 * @{
 */
enum
{
	E_DPDA_BPF_NOMAL_MODE = 0,								/*!< 通常read/write						*/
	E_DPDA_BPF_TMP_MODE 									/*!< 温度情報取得						*/
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

#endif
/* @} */
