/*!
 * @skip  $ld:$
 * @file  f_com_dpda_init_trainingDataSet_Tx_mmW.c
 * @brief DPDAタスク TX部トレーニングデータ設定処理
 * @date  2018/08/10 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_COM
 * @{
 */

//#include "f_com_inc.h"
#include "f_dpda_inc.h"

/************************************************************************************************************/
/*!
 * @brief 関数機能概要:mmW TX部トレーニングデータの設定処理を行う
 * @note  関数処理内容.
 *       -# 現状処理がないため空関数
 * @param  none
 * @return VOID
 * @warning   N/A
 * @FeatureID PF-Rfp-001-001-001
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2018/08/10 KCN)takagi Create
 */
/************************************************************************************************************/
VOID f_com_dpda_init_trainingDataSet_Tx_mmW()
{

	memcpy( &f_comw_trainingData->traVal_vt, &f_comw_rftrainingData->traVal, D_RRH_RF_TRAING_DATA_SIZE);

	return;
}

/* @} */
