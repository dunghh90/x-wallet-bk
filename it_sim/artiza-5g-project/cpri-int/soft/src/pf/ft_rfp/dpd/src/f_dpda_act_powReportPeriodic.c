/*!
 * @skip  $ld:$
 * @file  f_dpda_act_PowReportPeriodic.c
 * @brief DPDAタスク 電力報告周期通知受信処理
 * @date  2011/07/26 ALPHA)横山 Create
 * @date  2013/11/22 ALPHA)加藤 ZYNQ対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2014
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/****************************************************************************************************/
/*!
 * @brief 関数機能概要:電力報告周期通知受信時の処理を行う
 * @note  関数処理内容.
 *		-# ハードレジスタ情報取得
 *		-# アラーム検知処理
 *		-# アラーム発生時解析用ログ収集処理
 *		-# アラーム通知処理
 * @param	msgP		[in]VOID*		周期監視報告
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	PF-Rfp-006-003-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2014/02/25 ALPHA)藤井 Create
 * @date 2015/04/27 ALPHA)藤原 TDD対応
 * @date 2015/08/06 TDI)satou 「温度補償」処理を削除
 * @date 2015/08/06 TDI)satou 「TSSI計算」処理を削除
 * @date 2015/08/06 TDI)satou 「受信電力報告(RSSI)」処理を削除
 * @date 2015/08/06 TDI)satou 「アンテナ電力報告」処理を削除
 * @date 2015/08/06 TDI)satou 「FB電力報告」処理を削除
 * @date 2015/08/06 TDI)satou 「ADC受信電力報告」処理を削除
 */
/****************************************************************************************************/
INT f_dpda_act_powReportPeriodic_fhm(VOID* msgP)
{
	USHORT				usAlarmNumOccur = 0;
	USHORT				usAlarmNumClear = 0;
	INT					ret;
	INT					errcd;

	/* 1sec timer start */
	ret = BPF_RU_HRTM_REGISTER(
			BPF_RU_HRTM_REG_ONCE,
			E_RRH_TIMID_DPD_PERIODIC,
			D_DPDA_ONE_SHOT_TIME_1SEC,
			D_SYS_MSGID_TIMOUTNTC,
			D_SYS_THDQID_PF_F_DPD,
			&errcd
		);
	
	if( ret != BPF_RU_HRTM_COMPLETE )
	{
		/* Assert log */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, 
							"Timer Register NG!ret = %d, errcd = %d", 
							ret, errcd);
	}
	
	/* ハードレジスタ情報取得 */
	f_dpda_peri_getHardRegInfo();
	
	/* アラーム検出 */
	f_dpda_alm_stateUpdate(&usAlarmNumOccur, &usAlarmNumClear);
	
	/* アラーム発生時解析用ログ収集 */
	f_dpda_alm_logCollect( (usAlarmNumOccur+usAlarmNumClear) );
	
	/* アラーム報告 */
	f_dpda_alm_report_fhm(usAlarmNumOccur, usAlarmNumClear);
	
	return D_SYS_OK;
}

/* @} */
