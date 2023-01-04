/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_calcTssi.c
 * @brief DPDAタスク TSSI計算処理
 * @date  2013/05/30 ALPHA)中村 Create
 * @date  2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"



/*!
 * @brief 関数機能概要:TSSI計算処理を行う(3.5G TDD用)
 * @note  関数処理内容.
 *		-# キャリア状態がOFFの場合
 *			-# 計算結果を0に設定する
 *		-# キャリア状態がONの場合
 *			-# TSSIの取得データが0の場合
 *				-# 0計算結果を0に設定する
 *			-# TSSIの取得データが0以外の場合
 *				-# キャリア個別電力の算出を行う(10*log(REF電力/50337792)+26.8)
 *				-# キャリア個別電力の算出を行う(10*log(REF電力/50337792)+42.8) (RRE時)
 *			-# 計算結果がマイナスの場合
 *					-# 報告値に0を設定する
 * @param	uiAntNum				[in]UINT		アンテナ番号
 * @param	uiCarNum				[in]UINT		キャリア番号
 * @param	uiTssi					[in]UINT*		TSSI
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date 2013/11/22 ALPHA)加藤 ZYNQ対応 Create
 * @date 2015/04/28 ALPHA)藤原 TDD対応
 * @date 2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
void f_dpda_peri_calcTssi( unsigned int uiAntNum,
						   unsigned int uiCarNum,
						   int *iTssi)
{
	unsigned int  uiRegVal = 0;
	double dTssi = 0;
	
	
	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		/* sub6の場合 */
		
		/*	キャリア電力報告値取得 */
		uiRegVal = f_dpdaw_regdataReal.regVal[f_dpdaw_index_TssiPos + uiAntNum * 2 + uiCarNum];
		
		/* TSSIの計算を行う	*/
		if(uiRegVal == 0){
			/* レジスタ値 0の場合は、-34.22 とする(小数第二位までを整数化するため100倍して計算) */
			dTssi = -34.22 * 100;
		}
		else{
			
			if(f_cmw_tra_inv_tbl->cmx_device_BW == D_DU_EEP_DEV_FREQ_45G){
				/* 小数第二位までを整数化するため100倍して計算 */
				dTssi = (10 * log10((double)uiRegVal / 1878452.2) + 29.9) * 100;
			}else{
				dTssi = (10 * log10((double)uiRegVal / 1878452.2) + 28.5) * 100;
			}
		}
		
	}
	else{
		/* mmWの場合 */
		
		/*	キャリア電力報告値取得 */
		uiRegVal = f_dpdaw_regdataReal.regVal[f_dpdaw_index_TssiPos + uiAntNum * 4 + uiCarNum];
		
		/* TSSIの計算を行う	*/
		if(uiRegVal == 0){
			/* レジスタ値 0の場合は、-34.22 とする(小数第二位までを整数化するため100倍して計算) */
			dTssi = -34.22 * 100;
		}
		else{
			/* 小数第二位までを整数化するため100倍して計算 */
			dTssi = (10 * log10((double)uiRegVal / 37820813.7) + 15.2) * 100;
		}
	}

	/* 0.1dBステップでの報告値とする (例) 40.26dBm→403d, +0.5は四捨五入 */
	/* 最後に10で割って小数第二位を無くす */
	if(dTssi < 0) {
		*iTssi = (int)((dTssi - 5)/10); 
	}
	else {
		*iTssi = (int)((dTssi + 5)/10); 
	}

	return;
}

/* @} */
