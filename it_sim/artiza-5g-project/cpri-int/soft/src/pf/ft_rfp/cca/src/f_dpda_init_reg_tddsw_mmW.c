/*!
 * @skip  $ld:$
 * @file  f_dpda_init_reg_tddsw_mmW.c
 * @brief DPDAタスク レジスタ初期設定
 * @date 2015/09/05 ALPHA)藤原 RRH-013-000 DCM_3.5G TDD-RRE(Zynq)対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:TDD制御レジスタ設定処理を行う
 * @note  関数処理内容.
 *       -# TDD制御レジスタ設定を実施する(TDレジスタ設定)
 * @param  none
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2015/09/05 ALPHA)藤原 RRH-013-000 DCM_3.5G TDD-RRE(Zynq)対応
 *
*/
VOID f_dpda_init_reg_tddsw_mmW()
{
	UINT					regCnt;
	UINT					setCnt;
 	/* TDD制御レジスタ設定を実施する(TDレジスタ設定) */
	for( regCnt=0; regCnt<D_DPDA_TDD_COMMON_REG_NUM_MMW; regCnt++ )
	{
		for( setCnt = 0; setCnt < f_dpdar_tdd_reg_set_common_mmW[regCnt].regSetNum; setCnt++ )
		{
			f_dpda_IO_write(f_dpdar_tdd_reg_set_common_mmW[regCnt].regAddr + setCnt * f_dpdar_tdd_reg_set_common_mmW[regCnt].regOffset,
						f_dpdar_tdd_reg_set_common_mmW[regCnt].regData,
						E_DPDA_BPF_NOMAL_MODE,
						D_RRH_LOG_REG_LV_WRITE );
		}

	}
	return;
}

/* @} */
