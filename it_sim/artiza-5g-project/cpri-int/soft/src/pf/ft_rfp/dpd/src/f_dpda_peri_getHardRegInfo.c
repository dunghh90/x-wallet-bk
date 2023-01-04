/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_getHardRegInfo.c
 * @brief DPDAタスク ハードレジスタ情報取得処理
 * @date  2014/02/25 ALPHA)中村 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2014
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:ハードレジスタ情報取得処理を行う
 * @note  関数処理内容.
 *		-# レジスタ情報取得
 *		-# SV情報取得
 *		-# 温度情報取得
 * 
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/02/25 ALPHA)中村 Create
 * @date 2015/04/28 ALPHA)藤原 TDD対応
 * @date 2015/08/06 TDI)satou 「ラッチ処理」を削除
 * @date 2015/08/06 TDI)satou 「リソースアラーム情報更新」処理を削除
 */
VOID f_dpda_peri_getHardRegInfo()
{
	UINT							uiRegCnt	= 0;
	UINT							uiRegVal	= 0;
	INT								iTempVal	= 0;
	
	/************************************************/
	/* レジスタ情報収集								*/
	/************************************************/
	for( uiRegCnt = 0; uiRegCnt < E_RRH_REG_NUM_MAX; uiRegCnt++ )
	{
		uiRegVal = f_dpda_IO_read( f_dpdar_reg_Tbl[uiRegCnt],
								   E_DPDA_BPF_NOMAL_MODE,
								   D_RRH_LOG_REG_LV_READ );
		
		f_dpdaw_regdataReal.regVal[uiRegCnt] = uiRegVal;
	}
	
	/************************************************/
	/* 温度情報:温度センサ(TMP112)					*/
	/*  レジスタ値12bit signed						*/
	/*  レジスタ値に0.0625かけた値が温度となる		*/
	/************************************************/
	iTempVal = f_dpda_IO_read( 0, E_DPDA_BPF_TMP_MODE, D_RRH_LOG_REG_LV_READ );
	
	f_dpdaw_statusInfo.Temperature = iTempVal;

	return;
}

/* @} */
