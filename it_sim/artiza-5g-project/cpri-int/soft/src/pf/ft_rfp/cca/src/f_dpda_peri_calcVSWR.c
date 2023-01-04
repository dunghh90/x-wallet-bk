/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_calcVSWR.c
 * @brief DPDAタスク VSWR計算処理
 * @date  2019/4/3 FJT)Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"
#include <math.h>
#include <limits.h>
#include <float.h>

/*!
 * @brief 関数機能概要:VSWR計算処理
 * @note  関数処理内容.
 * @param	uiAntNum				[in]UINT		アンテナ番号
 * @param	dlRL					[in]DL*		Return Loss
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-004-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A 
 * @date  2019/4/3 FJT)Taniguchi
 */
void f_dpda_peri_calcVSWR( unsigned int uiAntNum,DL *RL , int* iVSWR)
{
	unsigned int  POWREFSW = 0;
	unsigned int  POWRVSSW = 0;
	UINT		  uiSetData;
	DL dlDL 	= DBL_MIN;

	*iVSWR = INT_MIN;	/* -4.5 (dB) 以上がfaultであるため、無効値としてINT_MINとする */
	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		/* sub6の場合 */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_FBATC_SWCNT, &uiSetData);
		/* DPD ON時(SWCNT ON)のみ値を設定する */
		if((uiSetData & D_RRH_ON) != 0){
			/*	キャリア電力報告値取得 */
			POWREFSW = f_dpdaw_regdataReal.regVal[f_dpdaw_index_RefPowPos_VSWR + uiAntNum ];
			POWRVSSW = f_dpdaw_regdataReal.regVal[f_dpdaw_index_VswrPos + uiAntNum ];
			if(POWREFSW > 0x5000){
				dlDL =  (10 * log10((DL)POWRVSSW / (DL)POWREFSW)) - 3.0;
				*iVSWR = (INT)(dlDL*10);	/* 10倍して値を保持 */
			}
		}
	}
	*RL = dlDL;
	return;
}

/* @} */
