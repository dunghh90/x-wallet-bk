/*!
 * @skip  $ld:$
 * @file  f_dpda_alm_logCollect.c
 * @brief DPDAタスク アラーム発生時解析用ログ収集（デバッグ用）処理
 * @date  2011/07/26 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2013
 */

/*!
 * @addtogroup RRH_DPDA
 * @{
 */

#include "f_dpda_inc.h"

/****************************************************************************************************/
/*!
 * @brief 関数機能概要:アラーム発生時解析用ログ収集（デバッグ用）処理を行う
 * @note  関数処理内容.
 *		-# アラームヒストリー用ログ保存処理
 *		-# dpdaアラーログ保存処理
 *		-# 温度情報保存処理
 * @param	usAlarmNum				[in]USHORT		アラーム回復/発生数
 * @return VOID
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	PF-Rfp-006-003-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2011/07/26 ALPHA)横山 Create
 * @date 2011/09/27 ALPHA)横山 SRRU-2.1G対応
 * @date 2011/11/02 ALPHA)藤井 M-RRU-ZSYS-00703 対処
 * @date 2011/11/07 ALPHA)横山 M-RRU-ZSYS-00716 対処
 * @date 2011/12/07 ALPHA)横山 SRRU-2.1G-LTE対応
 * @date 2012/03/14 ALPHA)藤井 N21GLTE対応
 * @date 2012/10/05 ALPHA)横山 MTCD対応
 * @date 2013/06/07 ALPHA)中村 B25対応
 * @date 2013/11/22 ALPHA)藤井 DCM-ZYNQ対応
 * @date 2015/08/06 TDI)satou 「L3制御情報」削除
 * @date 2015/08/07 TDI)satou 「送信受信アンテナ情報,dpd情報」削除
 * @date 2015/08/07 TDI)satou 「Soft State情報(ANT/CarrierInfo)コピー」削除
 */
/****************************************************************************************************/
VOID f_dpda_alm_logCollect( USHORT	usAlarmNum )
{
	UINT							uiPosi;
	T_DPDA_ALM_LOG_INFO				*almLogInfo;
	T_RRH_ALARM_HISTORYLOG_INFO		*almHisInfo;
	T_RRH_SYSTEM_TIME				timeInfo;
	
	/************************************************************************************************/
	/* アラームヒストリー用ログ保存処理																*/
	/************************************************************************************************/
	uiPosi		= f_dpdaw_almLogForHistory->dataPosi;
	almHisInfo  = &(f_dpdaw_almLogForHistory->logInfo[uiPosi]);

	/* CPRI情報 */
	almHisInfo->cpriInfo.cpriStatus = f_dpdaw_regdataReal.regVal[ E_RRH_REG_CPRI_CPSTAT0 ];
	
	/* SV情報 */
	memcpy(	(VOID *) &(almHisInfo->svInfo),
			(VOID *) &(f_dpdaw_regdataReal.regVal[ E_RRH_REG_CNT_SV1INF ]),
			sizeof(almHisInfo->svInfo) );
	
	/* 温度情報 */
	almHisInfo->txAntInfo.temp = f_dpdaw_statusInfo.Temperature;
	
	/* レジスタ生情報 */
	memcpy(	(VOID *) &(almHisInfo->regInfo),
			(VOID *)(f_dpdaw_regdataReal.regVal),
			sizeof(almHisInfo->regInfo) );
	
	/* 次回格納位置更新	*/
	f_dpdaw_almLogForHistory->dataPosi++;
	f_dpdaw_almLogForHistory->dataPosi %= D_RRH_ALMHIS_LOG_MEAS_NUM;
	
	/************************************************************************************************/
	/* dpdaアラーログ保存処理																		*/
	/************************************************************************************************/
	/* 収集面切り替え直後の場合に、参照位置の初期化を行う */
	if( f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].almLogNum == 0 )
	{
		f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].DataPosi = 0;
	}
	
	/* ログ格納位置取得	*/
	uiPosi = f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].DataPosi;
	
	/* テーブルポインタ取得 */
	almLogInfo = &(f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].alm_log_info[uiPosi]);
	
	/* 時刻獲得 */
	memset( &timeInfo, 0x00, sizeof(timeInfo) );
	f_com_getSysTim(&timeInfo);
	memcpy( &(almLogInfo->time), &timeInfo, sizeof(almLogInfo->time) );
	
	/********************************************/
	/* Soft情報取得								*/
	/********************************************/
	/* Status Information RPT情報コピー	*/
	memcpy( &(almLogInfo->soft_info.statusInfo),
			&(f_dpdaw_statusInfo),
			sizeof(almLogInfo->soft_info.statusInfo) );
	
	/* 各種カウンタ値/フラグ情報コピー	*/
	memcpy( &(almLogInfo->soft_info.dpdaInfo),
			&(f_dpdaw_info_all->taskInfo),
			sizeof(almLogInfo->soft_info.dpdaInfo) );
	
	/********************************************/
	/* Hard情報取得								*/
	/********************************************/
	/* レジスタ情報コピー	*/
	memcpy( &(almLogInfo->hard_info.RegData),
			&(f_dpdaw_regdataReal),
			sizeof(almLogInfo->hard_info.RegData) );
	
	/* 格納位置更新		*/
	uiPosi++;
	
	/* Maxまで行ったらサイクリック	*/
	if( D_DPDA_ALM_LOG_MAX == uiPosi )
	{
		uiPosi = 0;
	}
	
	/* ログ格納位置更新	*/
	f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].DataPosi = uiPosi;
	f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].almLogNum
		= D_DPDA_MACRO_MIN( (f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].almLogNum+1), D_DPDA_ALM_LOG_MAX);
	
	/* アラーム情報は1秒ごとではなく1面に1つ保持する */
	/* ALM閾値情報コピー	*/
	memcpy( &(f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].almThresholdInfo),
			&(f_dpdaw_info_all->almThreshold),
			sizeof(f_dpdaw_info_all->almThreshold) );
	
	/* ALM State情報コピー	*/
	memcpy( &(f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].almStateInfo),
			&(f_dpdaw_almState),
			sizeof(f_dpdaw_almState) );
	
	/************************************************************************/
	/* 面切替処理															*/
	/************************************************************************/
	/* アラームが発生/回復していた場合は次周期でのアラームログ収集面を切り替える	*/
	if( 0 != usAlarmNum )
	{
		/* 収集面切替	*/
		f_dpdaw_almLog->almLogFace++;
		
		/* 10面まで行ったらサイクリックする	*/
		if( D_DPDA_ALM_LOG_FACE <= f_dpdaw_almLog->almLogFace )
		{
			f_dpdaw_almLog->almLogFace = 0;
		}
		
		/* 切り替えた先の情報をクリアする(ログ格納位置とログ数)	*/
		f_dpdaw_almLog->almLog[ f_dpdaw_almLog->almLogFace ].almLogNum = 0;
	}
	
	/************************************************************************************************/
	/* 温度情報ログ保存処理																			*/
	/************************************************************************************************/
	f_dpdaw_tempLog->usCurCnt --;
	if( 0 == f_dpdaw_tempLog->usCurCnt )
	{
		f_dpdaw_tempLog->usCurCnt = f_dpdaw_tempLog->usTimerVal;
		
		uiPosi = f_dpdaw_tempLog->uiWriteIdx;
		memcpy( &(f_dpdaw_tempLog->tRecord[uiPosi].tDate),
				&timeInfo,
				sizeof(f_dpdaw_tempLog->tRecord[uiPosi].tDate) );
		f_dpdaw_tempLog->tRecord[uiPosi].tempVal =  f_dpdaw_statusInfo.Temperature;
		
		uiPosi++;
		if( uiPosi >= D_RRH_LOG_TEMP_LOG_NUM )
		{
			uiPosi=0;
			f_dpdaw_tempLog->uiCycleflg = D_SYS_ON;
		}
		
		f_dpdaw_tempLog->uiWriteIdx = uiPosi;
	}
	
	return;
}

/* @} */
