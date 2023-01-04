/*!
 * @skip  $ld:$
 * @file  f_dpda_init_reg_tddsw.c
 * @brief DPDAタスク レジスタ初期設定
 * @date  2018/8/10 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
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
 *       -# TDD制御 CPLD設定を実施する
 * @param  none
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/8/10 KCN)takagi Create
 *
*/
VOID f_dpda_init_reg_tddsw_sub6()
{
	UINT					regCnt;
	UINT					setCnt;
 	/* TDD制御レジスタ設定を実施する(TDレジスタ設定) */
	for( regCnt=0; regCnt<D_DPDA_TDD_COMMON_REG_NUM; regCnt++ )
	{
		for( setCnt = 0; setCnt < f_dpdar_tdd_reg_set_common[regCnt].regSetNum; setCnt++ )
		{
			f_dpda_IO_write(f_dpdar_tdd_reg_set_common[regCnt].regAddr + setCnt * f_dpdar_tdd_reg_set_common[regCnt].regOffset,
						f_dpdar_tdd_reg_set_common[regCnt].regData,
						E_DPDA_BPF_NOMAL_MODE,
						D_RRH_LOG_REG_LV_WRITE );
		}

	}
	return;
}

/* @} */
