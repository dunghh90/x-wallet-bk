/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_calcFBPower.c
 * @brief DPDAタスク FB電力計算計算処理
 * @date  2013/11/24 ALPHA)加藤 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/****************************************************************************************************/
/*!
 * @brief 関数機能概要: FB電力計算処理を行う
 * @note  関数処理内容.
 *		-# 指定キャリアがOFF状態の場合
 *			-# 計算結果を0に設定する
 *		-# 指定キャリアがOFF状態の以外の場合
 *			-# FB電力を計算する
 *			-# 出力電力[dBm] = 10*log(SW連動FB電力　/ SW連動REF電力) + アンテナ電力報告値
 * @param  uiCarNum				[in]UINT	アンテナ番号
 * @param  uiAntPow				[in]UINT	アンテナ電力
 * @param  uiPowSwRefVal		[in]ULLINT	SW連動REF電力
 * @param  uiPowSwFbVal			[in]ULLINT	SW連動FB電力
 * @param  uiExpPow				[out]UINT*	アンテナ電力値
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2013/11/24 ALPHA)加藤 ZYNQ対応 Create
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
/****************************************************************************************************/
void f_dpda_peri_calcFBPower( int iAntPow,
							  unsigned int uiPowSwRefVal,
							  unsigned int uiPowSwFbVal,
							  int *iFbPow)
{
	double dFbPow = 0;
	
	
	/* FB電力報告はsub6の場合のみ実施する */
	
	/* FbPowの計算を行う	*/
	if((uiPowSwRefVal == 0) || (uiPowSwFbVal == 0)){
		/* レジスタ値 0の場合は、アンテナ電力報告値とする */
		*iFbPow = iAntPow;
	}
	else{
		/* 小数第二位までを整数化するため100倍して計算 */
		dFbPow = (10 * log10((double)uiPowSwFbVal / uiPowSwRefVal)) * 100;

		/* 0.1dBステップでの報告値とする (例) 40.26dBm→403d, +0.5は四捨五入 */
		/* 最後に10で割って小数第二位を無くす */
		/* iAntPow(アンテナ電力報告値は四捨五入されているので最後に加算する */
		if(dFbPow < 0) {
			*iFbPow = (int)((dFbPow - 5)/10 + iAntPow); 
		}
		else {
			*iFbPow = (int)((dFbPow + 5)/10 + iAntPow); 
		}
	}
	
	return;
}
/* @} */
