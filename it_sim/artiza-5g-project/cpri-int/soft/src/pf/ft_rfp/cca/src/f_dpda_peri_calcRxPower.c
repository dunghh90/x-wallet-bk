/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_calcRxPower.c
 * @brief DPDAタスク 送信キャリア電力計算(RxPower) 計算処理
 * @date  2013/5/30 ALPHA)中村 Create
 * @date  2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

/*!
 * @addtogroup RRH_DPDA
 * @{
 */

#include	<math.h>
#include "f_dpda_inc.h"

/****************************************************************************************************/
/*!
 * @brief 関数機能概要: ANT毎受信電力(RxPower) 計算処理を行う
 * @note  関数処理内容.
 *		-# アンテナの全キャリアがOFF状態の場合以下の処理を行う
 *			-# 計算結果を0に設定する
 *		-# アンテナの何れかのキャリアがON状態場合以下の処理を行う
 *			-# RSSI報告値を取得する
 *			-# 下位4ビットを取り出し、ADC受信電力を計算
 *			-# ADC受信電力 = 10*log(リード値/245850010)-35.5+X
 * @param  uiCarNum				[in]UINT	アンテナ番号
 * @param  iExpPow				[out]INT*	RxPower
 * @return VOID
 * @date 2013/05/30 ALPHA)中村 B25対応
 * @date 2014/01/15 ALPHA)加藤 ZYNQ対応
 * @date 2015/05/12 ALPHA)藤原 TDD対応
 * @date 2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応 
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
/****************************************************************************************************/
void f_dpda_peri_calcRxPower( unsigned int uiAntNum, int *iRxPow)
{
	unsigned int uiRegVal = 0;
	double dRxPow = 0;
	
	
	
	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		/* sub6の場合 */
		
		/*	RX-ADC電力報告値取得 */
		uiRegVal = f_dpdaw_regdataReal.regVal[f_dpdaw_index_RxPowPos + uiAntNum];
		
		/* RX-ADC電力の計算を行う	*/
		if(uiRegVal == 0){
			/* レジスタ値 0の場合は、-115.99とする(小数第二位までを整数化するため100倍して計算) */
			dRxPow = (-115.99 - 26.5) * 100;
		}
		else{
			/* 小数第二位までを整数化するため100倍して計算 */
			dRxPow = (10 * log10((double)uiRegVal / 407123813.7) - 26.5) * 100;
		}
		
	}
	else{
		/* mmWの場合 */
		
		/*	キャリア電力報告値取得 */
		uiRegVal = f_dpdaw_regdataReal.regVal[f_dpdaw_index_RxPowPos + uiAntNum];
		
		/* RX-ADC電力の計算を行う	*/
		if(uiRegVal == 0){
			/* レジスタ値 0の場合は、-115.99とする(小数第二位までを整数化するため100倍して計算) */
			dRxPow = (-115.99 - 55.7) * 100;
		}
		else{
			/* 小数第二位までを整数化するため100倍して計算 */
			dRxPow = (10 * log10((double)uiRegVal / 2693268.75) - 55.7) * 100;
		}
	}
	
	/* 0.1dBステップでの報告値とする (例) 40.26dBm→403d, +0.5は四捨五入 */
	/* 最後に10で割って小数第二位を無くす */
	if(dRxPow < 0) {
		*iRxPow = (int)((dRxPow - 5)/10); 
	}
	else {
		*iRxPow = (int)((dRxPow + 5)/10); 
	}

	return;
}

/* @} */

