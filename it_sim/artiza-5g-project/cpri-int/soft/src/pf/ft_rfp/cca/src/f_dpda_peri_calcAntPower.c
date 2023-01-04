/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_calcAntPower.c
 * @brief DPDAタスク アンテナ電力計算処理
 * @date  2013/11/24 ALPHA)加藤 Create
 * @date 2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/****************************************************************************************************/
/*!
 * @brief 関数機能概要: アンテナ電力計算処理を行う(3.5G TDD用)
 * @note  関数処理内容.
 *		-# 指定キャリアがOFF状態の場合
 *			-# 計算結果を0に設定する
 *		-# 指摘キャリアがOFF状態の以外の場合
 *			-# アンテナ電力を計算する
 *			-# アンテナ電力報告(SRE) = 10*log(REF電力 / 35467268) + 29.8
 *			-# アンテナ電力報告(RRE) = 10*log(REF電力 / 35467268) + 45.8
 * @param  uiAntNum				[in]UINT	アンテナ番号
 * @param  uiAntPow				[out]UINT*	AntPower
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/11/24 ALPHA)加藤 ZYNQ対応 Create
 * @date 2015/04/28 ALPHA)藤原 TDD対応 
 * @date 2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
/****************************************************************************************************/
void f_dpda_peri_calcAntPower(unsigned int uiAntNum, int *iAntPow)
{
	double dAntPow = 0;
	unsigned int uiRegVal = 0;
	
	
	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		/* sub6の場合 */
		
		/*  アンテナ電力報告値取得 */
		uiRegVal = f_dpdaw_regdataReal.regVal[f_dpdaw_index_AntPowPos + uiAntNum];
		
		if(uiRegVal == 0){
			/* レジスタ値 0の場合は、-34.22 とする(小数第二位までを整数化するため100倍して計算) */
			dAntPow = -34.22 * 100;
		}
		else{
			if(f_cmw_tra_inv_tbl->cmx_device_BW  == D_DU_EEP_DEV_FREQ_45G){
				/* 小数第二位までを整数化するため100倍して計算 */
				dAntPow = (10 * log10((double)uiRegVal / 30055235.73) + 29.9) * 100;
			}else{
				dAntPow = (10 * log10((double)uiRegVal / 30055235.73) + 28.5) * 100;
			}
		}
		
	}
	else{
		/* mmWの場合 */
		
		/*  アンテナ電力報告値取得 */
		uiRegVal = f_dpdaw_regdataReal.regVal[f_dpdaw_index_AntPowPos + uiAntNum];
		
		if(uiRegVal == 0){
			/* レジスタ値 0の場合は、-34.22 とする(小数第二位までを整数化するため100倍して計算) */
			dAntPow = -34.22 * 100;
		}
		else{
			/* 小数第二位までを整数化するため100倍して計算 */
			dAntPow = (10 * log10((double)uiRegVal / 37820813.7) + 15.2) * 100;
		}
	}
	
	/* 0.1dBステップでの報告値とする (例) 40.26dBm→403d, +0.5は四捨五入 */
	/* 最後に10で割って小数第二位を無くす */
	if(dAntPow < 0) {
		*iAntPow = (int)((dAntPow - 5)/10); 
	}
	else {
		*iAntPow = (int)((dAntPow + 5)/10); 
	}

	return;
}
/* @} */
