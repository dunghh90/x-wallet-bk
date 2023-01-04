/********************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_dpda_alm_infoCollect.c
 * @brief アラーム情報収集処理
 * @date  2011/07/19 ALP)藤井 Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2011-2015
 */
/********************************************************************************************************************/

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */
#include "f_dpda_inc.h"

/********************************************************************************************************************/
/*!
 * @brief 関数機能概要: アラーム情報収集処理
 * @note  関数処理内容.
 *		-# アラーム判定に必要な情報を収集し、判定しやすい型に整形する
 *			-# アラーム閾値情報設定
 *			-# Tx/Rxキャリア情報
 *			-# Tx Gainアラーム関連設定
 *			-# アラーム状態取得
 *			-# TDDEN状態取得
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-011-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2011/07/27 ALPHA)藤井 Create
 * @date 2012/03/21 ALPHA)藤井 N21GLTE対応
 * @date 2012/04/02 ALPHA)藤井 M-RRU-ZSYS-00840対処(IT1問処No.2)
 * @date 2012/10/30 ALPHA)藤井 MTCD対応
 * @date 2012/10/31 ALPHA)藤井 M-RRU-ZSYS-00896対処
 * @date 2013/01/10 ALPHA)藤井 M-RRU-ZSYS-00930対処
 * @date 2013/01/15 ALPHA)藤井 M-RRU-ZSYS-00933対処
 * @date 2013/01/28 ALPHA)藤井 M-RRU-ZSYS-00936対処
 * @date 2013/06/03 ALPHA)中村 B25対応
 * @date 2013/12/16 ALPHA)藤井 DCM-Zynq対応
 * @date 2014/02/28 ALPHA)藤井 DCM-SRE対応
 * @date 2015/08/07 TDI)satou 「Tx/Rxキャリア情報」削除
 * @date 2015/08/07 TDI)satou 「Tx Gainアラーム関連設定」削除
 * @date 2015/08/07 TDI)satou 「MASK Alarm(TX/RX)」削除
 * @date 2015/08/07 TDI)satou 「TDDEN状態取得」削除
 */
/********************************************************************************************************************/
VOID f_dpda_alm_infoCollect(VOID)
{
	/****************************************************************************************************************/
	/*	アラーム閾値情報設定																						*/
	/****************************************************************************************************************/
	/* f_dpdar_almInfo で共有メモリを直接参照できないため、ここでコピーする */
	memcpy( (VOID *)f_dpdaw_almThreshold, (VOID *)f_dpdaw_info_all->almThreshold, sizeof(f_dpdaw_almThreshold) );
	
	return;
}

/* @} */
