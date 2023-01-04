/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_calcRssi.c
 * @brief DPDAタスク 受信電力(RSSI)計算処理
 * @date  2013/05/31 ALPHA)中村 Create
 * @date  2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:受信電力(RSSI)計算処理を行う
 * @note  関数処理内容.
 *		-# PA OFFまたは指定ANTの全キャリアがOFF状態の場合
 *			-# 計算結果を0に設定する
 *		-# PA ONまたは指定ANTにキャリアONのものがある場合
 *			-# RSSI(受信電力報告(dBm)]計算
 	*			-#  = 10*log(リード値/4917000202)-35.5+X
 * @param	uiAntNum				[in]UINT		アンテナ番号
 * @param	uiCarNum				[in]UINT		キャリア番号
 * @param	iRssi					[in]INT*		RSSI
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-005
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/05/31 ALPHA)中村 B25対応 Create
 * @date 2013/11/24 ALPHA)加藤 ZYNQ対応
 * @date 2015/04/28 ALPHA)藤原 TDD対応
 * @date 2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応
 * @date 2015/09/24 ALPHA)藤原 TDD-RRE(Zynq)ハソ変v2.1対応
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */


void f_dpda_peri_calcRtwp(	unsigned int uiAntNum,
							unsigned int uiCarNum,
							int *iRtwp)
{
	unsigned int  uiRegVal = 0;
	double dRtwp = 0;

	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		/* sub6の場合 */
		
		/*	キャリア電力報告値取得 */
		uiRegVal = f_dpdaw_regdataReal.regVal[f_dpdaw_index_RtwpPos + uiAntNum * 2 + uiCarNum];	//uiAntNum * 2 + uiCarNumの部分はテーブル定義後に要確認
		
		/* RTWPの計算を行う	*/
		if(uiRegVal == 0){
			/* レジスタ値 0の場合は、-116.92とする(小数第二位までを整数化するため100倍して計算) */
			dRtwp = (-116.92 - 26.5) * 100;
		}
		else{
			/* sub6-45G */
			if(f_cmw_tra_inv_tbl->cmx_device_BW  == D_DU_EEP_DEV_FREQ_45G){
				/* 小数第二位までを整数化するため100倍して計算 */
				dRtwp = (10 * log10((double)uiRegVal / 508904767.1) - 26.5) * 100;
			}
			/* sub6-37G */
			else{
				/* 小数第二位までを整数化するため100倍して計算 */
				dRtwp = (10 * log10((double)uiRegVal / 508904767.1) - 26.5) * 100;
			}
		}
	}
	else{
		/* mmWの場合 */
		
		/*	キャリア電力報告値取得 */
		uiRegVal = f_dpdaw_regdataReal.regVal[f_dpdaw_index_RtwpPos + uiAntNum * 4 + uiCarNum];	//uiAntNum * 8 + uiCarNumの部分はテーブル定義後に要確認
		
		/* RTWPの計算を行う	*/
		if(uiRegVal == 0){
			/* レジスタ値 0の場合は、-116.92とする(小数第二位までを整数化するため100倍して計算) */
			dRtwp = (-116.92 - 55.7) * 100;
		}
		else{
			/* 小数第二位までを整数化するため100倍して計算 */
			dRtwp = (10 * log10((double)uiRegVal / 3366585.938) - 55.7) * 100;
		}
	}

	/* 0.1dBステップでの報告値とする (例) 40.26dBm→403d, +0.5は四捨五入 */
	/* 最後に10で割って小数第二位を無くす */
	if(dRtwp < 0) {
		*iRtwp = (int)((dRtwp - 5)/10); 
	}
	else {
		*iRtwp = (int)((dRtwp + 5)/10); 
	}
	
	return;
}

/* @} */
