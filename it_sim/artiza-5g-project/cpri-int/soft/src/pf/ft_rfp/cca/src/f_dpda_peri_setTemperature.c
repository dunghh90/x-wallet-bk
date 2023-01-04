/********************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_setTemperature.c
 * @brief DPDAタスク 温度補償処理
 * @date 2014/01/20 ALPHA)中村 ZYNQ対応 Create
 * @date 2015/04/28 ALPHA)藤原 TDD対応
 * @date 2015/09/05 ALPHA)藤原 TDD-RRE(Zynq)対応
 * @date 2019/04/01 FJT)Taniguchi 5G対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-2019
 */
/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include <math.h>
#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:温度補償処理を行う
 * @note  関数処理内容.
 *		-# 温度補償処理を行う
 * @param	N/A
 * @return  VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-002-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No
 * @date 2019/04/01 FJT)Taniguchi
 
 */
VOID f_dpda_peri_setTemperature()
{
	UINT				antCnt;
	UINT				tempIndex;
	UINT				count;

	/* 温度補償フラグONのときは処理をスキップする */
	if( D_SYS_ON == f_dpdaw_info_all->taskInfo.flg.temperature )
	{
		return;
	}
	
	
	if( f_dpdaw_statusInfo.Temperature < f_dpdar_tempAdjCoef[D_DPDA_TEMP_ADJ_TEMP_MIN].tempVal){
		tempIndex = D_DPDA_TEMP_ADJ_TEMP_MIN;
	}else if(f_dpdaw_statusInfo.Temperature > f_dpdar_tempAdjCoef[D_DPDA_TEMP_ADJ_TEMP_MAX].tempVal){
		tempIndex = D_DPDA_TEMP_ADJ_TEMP_MAX;
	}else{
		tempIndex = D_DPDA_TEMP_ADJ_COEF_NUM;
		for(count = 0; count < D_DPDA_TEMP_ADJ_COEF_NUM ;count++){
			if(f_dpdaw_statusInfo.Temperature == f_dpdar_tempAdjCoef[count].tempVal){
				tempIndex = count;
				break;
			}
		}
		if(tempIndex == D_DPDA_TEMP_ADJ_COEF_NUM){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR, "tempindex not found %d",f_dpdaw_statusInfo.Temperature);
			return;
		}
	}

	/* アンテナ数分ループ	*/
	for( antCnt = 0; antCnt < f_dpdaw_ant_max_num; antCnt++ ){
		f_dpda_ld_TxgainSetting(tempIndex , antCnt , f_dpdaw_info_all->taskInfo.txGainCoefNo );
	}

	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		for( antCnt = 0; antCnt < f_dpdaw_ant_max_num; antCnt++ ){
			f_dpda_ld_FbgainSetting(tempIndex , antCnt);
		}
	}
	for( antCnt = 0; antCnt < f_dpdaw_ant_max_num; antCnt++ ){
		f_dpda_ld_RxgainSetting(tempIndex , antCnt);
	}

	/* mmWの場合のみ、RxGain判定パラメータ収集	*/
	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_MMW){
		f_dpdaw_statusInfoExt.RxTempReduction = (INT)round( (20 * log10(f_dpdar_tempAdjCoef[tempIndex].tempCoef_rx[f_dpdaw_ruIndex]) * 10.0) );
		for( antCnt = 0; antCnt < f_dpdaw_ant_max_num; antCnt++ ){
			f_dpdaw_statusInfoExt.RxGainCompVal[antCnt] = f_dpdaw_statusInfo.RxPower[antCnt] + f_dpdaw_statusInfoExt.RxTempReduction;
		}
	}
	return;
}
