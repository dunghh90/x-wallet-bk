/*!
 * @skip  $ld:$
 * @file  f_dpda_alm_logCollect.c
 * @brief DPDAタスク アラーム発生時解析用ログ収集（デバッグ用）処理
 * @date  2011/07/26 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2015
 */

/*!
 * @addtogroup RRH_DPDA
 * @{
 */

#include "f_dpda_inc.h"
//#include "RFIC_PMC.h"



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
 * @date 2015/07/09 ALPHA)藤原 DCM-TDD-ZYNQ ハソ変対応
 * @date 2015/09/04 ALPHA)藤原 DCM_3.5G TDD-RRE(Zynq)対応
 */
/****************************************************************************************************/
VOID f_dpda_alm_logCollect( USHORT	usAlarmNum )
{
	UINT							uiPosi;
	UINT							antCnt;
	UINT							antDirCnt;
	UINT							carCnt;
	T_DPDA_ALM_LOG_INFO				*almLogInfo;
	T_RRH_ALARM_HISTORYLOG_INFO		*almHisInfo;
	T_DPDA_CAR_INFO					*carInfo;
	T_RRH_SYSTEM_TIME				timeInfo;
	
	/************************************************************************************************/
	/* アラームヒストリー用ログ保存処理																*/
	/************************************************************************************************/
	uiPosi		= f_dpdaw_almLogForHistory->dataPosi;
	almHisInfo  = &(f_dpdaw_almLogForHistory->logInfo[uiPosi]);
	
	//Ant, Car情報
	for( carCnt=0; carCnt<f_dpdaw_carrier_max_num; carCnt++ )
	{
		for( antDirCnt=0; antDirCnt < D_SYS_ANT_DIRECTION_NUM; antDirCnt++ )
		{
			for( antCnt=0; antCnt<f_dpdaw_ant_max_num; antCnt++ )
			{
				carInfo	= &(f_dpdaw_antCarStateInfo.ant_info[ antCnt ].car_info[ antDirCnt ][ carCnt ]);
				
				almHisInfo->antInfo[antCnt].carInfo[carCnt][antDirCnt].CarState = carInfo->CarOnOff;
				almHisInfo->antInfo[antCnt].carInfo[carCnt][antDirCnt].Freq  = carInfo->ChannelNum;
				almHisInfo->antInfo[antCnt].carInfo[carCnt][antDirCnt].Bandwidth = carInfo->BandWidth;
				almHisInfo->antInfo[antCnt].carInfo[carCnt][antDirCnt].AcellId = carInfo->AcellId;
				almHisInfo->antInfo[antCnt].carInfo[carCnt][antDirCnt].CcId = carInfo->CcId;
				almHisInfo->antInfo[antCnt].carInfo[carCnt][antDirCnt].PortId = carInfo->PortId;
				if(antDirCnt == 1){
					almHisInfo->antInfo[antCnt].carInfo[carCnt][antDirCnt].Gain = carInfo->Gain;
				}

			}
		}
	}
	
	/* SV情報 */
	memcpy(	(VOID *) &(almHisInfo->svInfo),
			(VOID *) &(f_dpdaw_regdataReal.regVal[ f_dpdaw_index_SvBusPos ]),
			sizeof(almHisInfo->svInfo) );
	
	/* TSSI */
	memcpy(	(VOID *) almHisInfo->txAntInfo.tssi,
			(VOID *) (f_dpdaw_statusInfo.Tssi),
			sizeof( almHisInfo->txAntInfo.tssi ) );
	/* アンテナ送信電力 */
	memcpy(	(VOID *) almHisInfo->txAntInfo.refPow,
			(VOID *) (f_dpdaw_statusInfo.AntPower),
			sizeof( almHisInfo->txAntInfo.refPow ) );
	/* FB電力 */
	memcpy(	(VOID *) almHisInfo->txAntInfo.fbPow,
			(VOID *) (f_dpdaw_statusInfo.FbPower),
			sizeof( almHisInfo->txAntInfo.fbPow ) );

	/* 温度 */
	almHisInfo->txAntInfo.temp = f_dpdaw_statusInfo.Temperature;

	/* VSWR RL */
	memcpy(	(VOID *) almHisInfo->txAntInfo.vswrRL,
			(VOID *) (f_dpdaw_statusInfo.VswrRL),
			sizeof( almHisInfo->txAntInfo.vswrRL ) );

	/* txPwrPsum */
	memcpy(	(VOID *) almHisInfo->txAntInfo.txPwrPsum,
			(VOID *) (f_dpdaw_statusInfo.txPwrPsum),
			sizeof( almHisInfo->txAntInfo.txPwrPsum ) );

	/* txPwrCnt */
	memcpy(	(VOID *) almHisInfo->txAntInfo.txPwrCnt,
			(VOID *) (f_dpdaw_statusInfo.txPwrCnt),
			sizeof( almHisInfo->txAntInfo.txPwrCnt ) );

	/* txPwrMon */
	memcpy(	(VOID *) almHisInfo->txAntInfo.txPwrMon,
			(VOID *) (f_dpdaw_statusInfo.txPwrMon),
			sizeof( almHisInfo->txAntInfo.txPwrMon ) );

	/* txPwrPsum avg */
	memcpy(	(VOID *) almHisInfo->txAntInfo.txPwrPsum_avg,
			(VOID *) (f_dpdaw_statusInfo.txPwrPsum_avg),
			sizeof( almHisInfo->txAntInfo.txPwrPsum_avg ) );

	/* ADC受信電力 */
	memcpy(	(VOID *) almHisInfo->rxAntInfo.rxPower,
			(VOID *) (f_dpdaw_statusInfo.RxPower),
			sizeof( almHisInfo->rxAntInfo.rxPower) );
	/* キャリア受信電力 */
	memcpy(	(VOID *) almHisInfo->rxAntInfo.Rtwp,
			(VOID *) (f_dpdaw_statusInfo.Rtwp),
			sizeof( almHisInfo->rxAntInfo.Rtwp) );

	/* 温度補償制御RX 温度補償リダクション量[dBm] */
	almHisInfo->rxAntInfoExt.RxTempReduction = f_dpdaw_statusInfoExt.RxTempReduction;

	/* Rx Gain比較値[dBm] */
	memcpy(	(VOID *)almHisInfo->rxAntInfoExt.RxGainCompVal,
			(VOID *)f_dpdaw_statusInfoExt.RxGainCompVal,
			sizeof( almHisInfo->rxAntInfoExt.RxGainCompVal) );

	/* レジスタ生情報 */
	memcpy(	(VOID *) &(almHisInfo->regInfo),
			(VOID *)(f_dpdaw_regdataReal.regVal),
			sizeof(almHisInfo->regInfo) );
	
	/* 拡張レジスタ生情報 */
	almHisInfo->regExt.regVal[0] = f_dpdaw_extdataReal.regVal[0];
	almHisInfo->regExt.regVal[1] = f_dpdaw_extdataReal.regVal[1];
	almHisInfo->regExt.regVal[2] = f_dpdaw_extdataReal.regVal[2];
	almHisInfo->regExt.regVal[3] = f_dpdaw_extdataReal.regVal[3];
	almHisInfo->regExt.regVal[4] = f_dpdaw_extdataReal.regVal[4];
	
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
	/* Soft State情報(ANT/CarrierInfo)コピー	*/
	memcpy( &(almLogInfo->soft_info.softStateInfo),
			&(f_dpdaw_antCarStateInfo),
			sizeof(almLogInfo->soft_info.softStateInfo) );

	/* Status Information RPT情報コピー	*/
	memcpy( &(almLogInfo->soft_info.statusInfo),
			&(f_dpdaw_statusInfo),
			sizeof(almLogInfo->soft_info.statusInfo) );
	
	/* Status Information 拡張領域 RPT情報コピー	*/
	memcpy( &(almLogInfo->soft_info.statusInfoExt),
			&(f_dpdaw_statusInfoExt),
			sizeof(almLogInfo->soft_info.statusInfoExt) );
	
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
	
	/* 拡張レジスタ生情報 */
	almLogInfo->hard_info.ExtData.regVal[0] = f_dpdaw_extdataReal.regVal[0];
	almLogInfo->hard_info.ExtData.regVal[1] = f_dpdaw_extdataReal.regVal[1];
	almLogInfo->hard_info.ExtData.regVal[2] = f_dpdaw_extdataReal.regVal[2];
	almLogInfo->hard_info.ExtData.regVal[3] = f_dpdaw_extdataReal.regVal[3];
	almLogInfo->hard_info.ExtData.regVal[4] = f_dpdaw_extdataReal.regVal[4];
	
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
