/*!
 * @skip  $ld:$
 * @file  f_dpda_act_powReportPeriodic.c
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
 *		-# 送信電力(TSSI)計算処理
 *		-# 受信電力(RSSI)計算処理
 *		-# アンテナ電力計算処理
 *		-# FB電力計算処理
 *		-# 温度補償処理
 *		-# 高速モード移行後処理
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
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
/****************************************************************************************************/
INT f_dpda_act_powReportPeriodic(VOID* msgP)
{
	UINT				uiAntNum	= 0;
	UINT				uiCarNum	= 0;
	USHORT				usAlarmNumOccur = 0;
	USHORT				usAlarmNumClear = 0;
	struct timespec		boottime;

	if( ((T_RRH_HEAD*)msgP)->uiSrcTQueueID == D_SYS_THDQID_PF_HD1PPS ){
		if( f_dpdaw_info_all->taskInfo.flg.sfpload_cmp == D_DPDA_1PPS_SET ){

			/* PTP完了後の1pps割り込み通知とキャリア設定直後のSFN load設定とのすれ違いを考慮する */
			/* SFNロード後、2～3秒まで*/
			(void)clock_gettime( CLOCK_MONOTONIC_COARSE   , &boottime);
			if( ( f_dpdaw_sfnload_settime.tv_sec + 1 ) >= boottime.tv_sec ){
				BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "1PPS IRQ discard SFN load comp(set:%ld cur:%ld qc:%d)",
					f_dpdaw_sfnload_settime.tv_sec,
					boottime.tv_sec,
					f_dpdaw_info_all->taskInfo.flg.qec_count);

				return D_SYS_OK;
			}

			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "1PPS IRQ rcv SFN load comp(set:%ld cur:%ld qc:%d )",
					f_dpdaw_sfnload_settime.tv_sec,
					boottime.tv_sec,
					f_dpdaw_info_all->taskInfo.flg.qec_count);

			f_dpdaw_info_all->taskInfo.flg.sfpload_cmp = D_DPDA_1PPS_CMP;

			/* SFNload完了後は高速CALを実行するため、高速CALフラグON(TX,RXは念のためON)、QECカウンタクリア */
			if(f_comw_bf_mode->flag == D_RRH_OFF) {
				f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_AC]  = D_RRH_ON;
				f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_BD]  = D_RRH_ON;
				f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_AC]  = D_RRH_ON;
				f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_BD]  = D_RRH_ON;
			}
			f_dpda_com_clearQEC();
		}
		return D_SYS_OK;
	}

	/* ハードレジスタ情報取得 */
	f_dpda_peri_getHardRegInfo();
	
	/* アンテナ数分ループ	*/
	for( uiAntNum = 0; uiAntNum < f_dpdaw_ant_max_num; uiAntNum++ )
	{
		/* キャリア数分ループ	*/
		for( uiCarNum = 0; uiCarNum < f_dpdaw_carrier_max_num; uiCarNum++ )
		{
			/****************************************************/
			/* TSSI計算											*/
			/****************************************************/
			f_dpda_peri_calcTssi( uiAntNum,uiCarNum,&(f_dpdaw_statusInfo.Tssi[ uiAntNum ][ uiCarNum ]) );
			/****************************************************/
			/* キャリア受信電力報告(RTWP)						*/
			/****************************************************/
			f_dpda_peri_calcRtwp( uiAntNum,uiCarNum,(INT*)&(f_dpdaw_statusInfo.Rtwp[ uiAntNum ][ uiCarNum ]) );
			
		}
		/****************************************************/
		/* アンテナ送信電力報告									*/
		/****************************************************/
		f_dpda_peri_calcAntPower( uiAntNum, &(f_dpdaw_statusInfo.AntPower[uiAntNum]) );
		
		/****************************************************/
		/* FB電力報告										*/
		/****************************************************/
		f_dpda_peri_calcFBPower(f_dpdaw_statusInfo.AntPower[uiAntNum],
								f_dpdaw_regdataReal.regVal[f_dpdaw_index_RefPowPos + uiAntNum],
								f_dpdaw_regdataReal.regVal[f_dpdaw_index_FbPowPos + uiAntNum],
								&f_dpdaw_statusInfo.FbPower[uiAntNum]);

	
		/****************************************************/
		/* VSWR	RL											*/
		/****************************************************/
		f_dpda_peri_calcVSWR( uiAntNum ,(DL*)&(f_dpdaw_statusInfo.VswrRL[uiAntNum]),(INT*)&(f_dpdaw_statusInfo.iVswrRL[uiAntNum]));

		/****************************************************/
		/* ADC受信電力報告									*/
		/****************************************************/
		f_dpda_peri_calcRxPower( uiAntNum, (INT*)&(f_dpdaw_statusInfo.RxPower[uiAntNum]) );
		
	}
	
	/****************************************************/
	/* 温度補償処理										*/
	/****************************************************/
	f_dpda_peri_setTemperature();

	/****************************************************/
	/* ハード自律PA-OFF時のDPD高速モード処理               */
	/****************************************************/
	f_dpda_peri_fastDpdMode();

	/****************************************************/
	/* QEC制御											*/
	/****************************************************/
	f_dpda_peri_setQEC();
	
	
	if( D_SYS_ON == f_dpdaw_info_all->almmsk ){
		return D_SYS_OK;	/* SKIP */
	}

	/* アラーム検出 */
	f_dpda_alm_stateUpdate(&usAlarmNumOccur, &usAlarmNumClear);	//ALMの判定を行う処理
	
	/* アラーム発生時解析用ログ収集 */
	f_dpda_alm_logCollect( (usAlarmNumOccur+usAlarmNumClear) );	//debug用のコマンド等で表示する情報をため込む処理
	
	/* アラーム報告 */
	f_dpda_alm_report(usAlarmNumOccur, usAlarmNumClear);		//SVレジスタに設定する処理
	
	return D_SYS_OK;

}

/* @} */
