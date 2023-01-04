/*!
 * @skip  $ld:$
 * @file  f_dpda_act_sysParaSetRequest.c
 * @brief DPDAタスク System Parameter Set Request受信処理
 * @date 2013/11/18 ALPHA)加藤 ZYNQ対応 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2015
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:System Parameter Set Request受信時の処理を行う
 * @note  関数処理内容.
 *		-# スレッド内テーブルの更新
 *		-# auxDAC設定
 *		-# 送信遅延設定
 *
 * @param		msgP		[in]VOID*		System Parameter Set Request
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-003-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/11/18 ALPHA)加藤 ZYNQ対応 Create 
 * @date 2015/04/29 ALPHA)藤井 TDD対応
 * @date 2015/11/06 ALPHA)藤原 TDD-SRE(Zynq)ハソ変対応 受信遅延処理追加
 */
INT f_dpda_act_sysParaSetRequest(VOID* msgP)
{

	return D_SYS_OK;
}

/* @} */
