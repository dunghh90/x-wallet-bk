/*!
 * @skip  $ld:$
 * @file  f_dpda_act_powReportPeriodic_mmW.c
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

#include <stdarg.h>
#include "f_dpda_inc.h"

static UINT f_dpdaw_l_txpow_detect_calcount = 0;
static UINT f_dpdaw_l_txpow_detect_skipcount = 0;
static UINT f_dpdaw_l_txpow_detect_overcount = 0;
static UINT f_dpdaw_l_txpow_detect_initcount = 0;

/****************************************************************************************************/
/*!
 * @brief 関数機能概要:TX power log取得を行う
 * @note  関数処理内容:TX power log取得を行う
 * @param		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Rfp-006-003-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/07/12 FJT)Taniguchi Create
 */
/****************************************************************************************************/
static VOID f_dpda_txpower_log_mmw(UINT gainOutValidFlg , const char* Message_p, ... )
{
	T_RRH_TXPOW_DETECT_HIS_RECORD		*logp;
	UINT								index;
	va_list args;
	
	/* Check message */
	if(Message_p == NULL)
	{
		return;
	}
	index = f_dpdaw_txpow_log->index;
	logp = &(f_dpdaw_txpow_log->tRecord[index]);
	
	logp->validFlg = D_RRH_OFF;

	/* set message 			*/
	memset(logp->message, 0, sizeof(logp->message));
	va_start(args, Message_p);	/* pgr0039 */
	(VOID)vsnprintf((char*)logp->message, sizeof(logp->message), Message_p, args);
	va_end(args);

	if(gainOutValidFlg == D_RRH_ON){
		/* 電力情報コピー */
		memcpy(logp->gainOut,f_dpdaw_txpow_detect->gainOut,sizeof(logp->gainOut));
	}

	logp->gainOutValidFlg = gainOutValidFlg;

	/* Get System time			*/
	(VOID)BPF_HM_DEVC_GETTIME(&(logp->date));

	logp->validFlg = D_RRH_ON;

	/*	update index	*/
	f_dpdaw_txpow_log->index = (UINT)((index + 1) % D_DU_TXPOW_DETECT_HIS_RCD_NUM);

	f_dpdaw_txpow_log->validFlg = D_RRH_ON;

	return;
}

/****************************************************************************************************/
/*!
 * @brief 関数機能概要:TX power detector 監視用の準備を行う
 * @note  関数処理内容:TX power detector 監視用の準備を行う
 * @param		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Rfp-006-003-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/06/04 FJT)Taniguchi Create
 */
/****************************************************************************************************/
VOID f_dpda_txpower_detector_mmw(VOID){
	
	UINT	regVal_pow;
	UINT	regVal_pgcpsum[32];
	UINT	regVal_pgccnt_vh[D_DPDA_ANTMAX_MMW][45];
	UINT	antCnt;
	UINT	vhIndex;
	UINT	beamIndex;
	UINT	regAddr;
	DL		N_VAL_ALL;
	DL		TCB_AVG_VAL;
	DL		POW_VAL;

	f_dpdaw_info_all->taskInfo.flg.txpower_detect_check = D_RRH_OFF;
	memset(f_dpdaw_statusInfo.txPwrPsum,0xFF,sizeof(f_dpdaw_statusInfo.txPwrPsum));
	memset(f_dpdaw_statusInfo.txPwrCnt,0xFF,sizeof(f_dpdaw_statusInfo.txPwrCnt));
	memset(f_dpdaw_statusInfo.txPwrMon,0,sizeof(f_dpdaw_statusInfo.txPwrMon));
	memset(f_dpdaw_statusInfo.txPwrPsum_avg,0,sizeof(f_dpdaw_statusInfo.txPwrPsum_avg));

	if(f_dpdaw_info_all->taskInfo.flg.txpower_detect_count == 0){

		f_dpda_IO_write(D_DU_REG_TCB_PGCPOW,0x80000000,E_DPDA_BPF_BITOFF_MODE,D_RRH_LOG_REG_LV_WRITE);
		f_dpda_IO_write(D_DU_REG_TCB_PGCPOW,0x00008000,E_DPDA_BPF_BITON_MODE ,D_RRH_LOG_REG_LV_WRITE);

		f_dpda_IO_write(M_DU_REG_CC_PGCP(0),0x00000003,E_DPDA_BPF_BITON_MODE ,D_RRH_LOG_REG_LV_WRITE);
		f_dpda_IO_write(M_DU_REG_CC_PGCP(1),0x00000003,E_DPDA_BPF_BITON_MODE ,D_RRH_LOG_REG_LV_WRITE);

		usleep(10);	/* 念のため10usec */

		f_dpda_IO_write(D_DU_REG_TCB_PGCPOW,0x80000000,E_DPDA_BPF_BITON_MODE,D_RRH_LOG_REG_LV_WRITE);

		f_dpdaw_info_all->taskInfo.flg.txpower_detect_count = 1;
		f_dpdaw_l_txpow_detect_initcount++;
		f_dpda_txpower_log_mmw(D_RRH_OFF,"STARTED(%u) TX Power Detect measurment...",f_dpdaw_l_txpow_detect_initcount);

		return;
	}

	f_dpdaw_info_all->taskInfo.flg.txpower_detect_count++;
	regVal_pow = f_dpda_IO_read(D_DU_REG_TCB_PGCPOW,E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ);
	if((regVal_pow & 0x00008000) == 0){
		f_dpdaw_l_txpow_detect_skipcount++;

		if(f_dpdaw_info_all->taskInfo.flg.txpower_detect_count > 20){	/* 20秒超えたら0からやり直しする、それ以外は待ち状態 */
			f_dpdaw_l_txpow_detect_overcount++;
			f_dpda_txpower_log_mmw(D_RRH_OFF,"LIMIT OVER(skip:%u over:%u init:%u cal:%u) TX Power Not Detected ",
												f_dpdaw_l_txpow_detect_skipcount,
												f_dpdaw_l_txpow_detect_overcount,
												f_dpdaw_l_txpow_detect_initcount,
												f_dpdaw_l_txpow_detect_calcount
												 );

			f_dpdaw_info_all->taskInfo.flg.txpower_detect_count = 0;
		}
		return;
	}

	BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_READ_BUF, D_DU_REG_TCB_PGCPSUM01,sizeof(regVal_pgcpsum),regVal_pgcpsum);
	BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_READ_BUF, D_DU_REG_TCB_PGCPCNTV,sizeof(regVal_pgccnt_vh[0]),&regVal_pgccnt_vh[0][0]);
	BPF_HM_DEVC_REG_READ_BUFFER(D_RRH_LOG_REG_LV_READ_BUF, D_DU_REG_TCB_PGCPCNTH,sizeof(regVal_pgccnt_vh[1]),&regVal_pgccnt_vh[1][0]);

	f_dpdaw_statusInfo.txPwrMon[D_DPDA_ANTA] = f_dpda_IO_read(M_DU_REG_CC_APGCPOWMON(D_DPDA_ANTA),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ);
	f_dpdaw_statusInfo.txPwrMon[D_DPDA_ANTB] = f_dpda_IO_read(M_DU_REG_CC_APGCPOWMON(D_DPDA_ANTB),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ);

	f_dpdaw_l_txpow_detect_calcount++;

	for( antCnt = 0 ; antCnt < D_DPDA_ANTMAX_MMW; antCnt++ ){
		f_dpdaw_txpow_detect->gainOut[antCnt].validFlg = D_RRH_OFF;
		memset(&f_dpdaw_txpow_detect->gainOut[antCnt],0,sizeof(f_dpdaw_txpow_detect->gainOut[antCnt]));

		if(antCnt == D_DPDA_ANTA){
			regAddr = D_DU_REG_TCB_PGCPCNTV;
		}else{
			regAddr = D_DU_REG_TCB_PGCPCNTH;
		}
		N_VAL_ALL = 0.0;
		for(beamIndex = 0 ; beamIndex < 45 ; beamIndex++){
			f_dpdaw_statusInfo.txPwrCnt[antCnt][beamIndex] = regVal_pgccnt_vh[ antCnt ][beamIndex] & 0x0FFFFFFF;
			f_dpdaw_txpow_detect->gainOut[antCnt].Reg_Cnt[beamIndex].regAddr = regAddr + (beamIndex * 4) ;
			f_dpdaw_txpow_detect->gainOut[antCnt].Reg_Cnt[beamIndex].regVal = f_dpdaw_statusInfo.txPwrCnt[antCnt][beamIndex];
			if(f_dpdaw_statusInfo.txPwrCnt[antCnt][beamIndex] != 0x0FFFFFFF){
				N_VAL_ALL += (f_dpdaw_txpow_detect->gainIn[beamIndex][antCnt].RVS_VH * f_dpdaw_statusInfo.txPwrCnt[antCnt][beamIndex]);
				f_dpdaw_txpow_detect->gainOut[antCnt].N_VAL[beamIndex] = (f_dpdaw_txpow_detect->gainIn[beamIndex][antCnt].RVS_VH * f_dpdaw_statusInfo.txPwrCnt[antCnt][beamIndex]);
			}
		}

		if(N_VAL_ALL != 0.0){
			f_dpdaw_txpow_detect->gainOut[antCnt].K_VAL = N_VAL_ALL / (DL)(0x20000);
		}

		TCB_AVG_VAL = 0.0;
		regAddr = D_DU_REG_TCB_PGCPSUM01 + (antCnt * 0x40);
		for(vhIndex = 0 ; vhIndex < 16 ; vhIndex++){
			f_dpdaw_statusInfo.txPwrPsum[antCnt][vhIndex] = regVal_pgcpsum[ (antCnt*16) + vhIndex ] & 0x0FFFFFFF;
			f_dpdaw_txpow_detect->gainOut[antCnt].Reg_Pow[vhIndex].regAddr = regAddr + (vhIndex * 4) ;
			f_dpdaw_txpow_detect->gainOut[antCnt].Reg_Pow[vhIndex].regVal = f_dpdaw_statusInfo.txPwrPsum[antCnt][vhIndex];
			if(f_dpdaw_statusInfo.txPwrPsum[antCnt][vhIndex] != 0){
				TCB_AVG_VAL += (DL)(f_dpdaw_statusInfo.txPwrPsum[antCnt][vhIndex])/131072.0;
			}
		}
		if( TCB_AVG_VAL != 0.0 ){
			f_dpdaw_txpow_detect->gainOut[antCnt].TCB_AVG_VAL = TCB_AVG_VAL / 16.0;
		}

		POW_VAL = f_dpdaw_txpow_detect->gainOut[antCnt].K_VAL * (( 0.106 * f_dpdaw_txpow_detect->gainOut[antCnt].TCB_AVG_VAL ) - 2.85);

		/* ※本計算結果がマイナスになった場合は0として扱う	*/
		/* ※計算結果は少数以下第2位を四捨五入				*/
		if( POW_VAL < 0.0 ){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "POW CLIP ZERO(cal:%u) Ant%d K_VAL:%f TCB_AVG_VAL:%f POW_VAL:%f",
																	f_dpdaw_l_txpow_detect_calcount,
																	antCnt,
																	f_dpdaw_txpow_detect->gainOut[antCnt].K_VAL,
																	f_dpdaw_txpow_detect->gainOut[antCnt].TCB_AVG_VAL,
																	POW_VAL);

			f_dpda_txpower_log_mmw(D_RRH_OFF,"CLIP ZERO(cal:%u) TX Power Detect Ant%d K_VAL:%f TCB_AVG_VAL:%f POW_VAL:%f",
																	f_dpdaw_l_txpow_detect_calcount,
																	antCnt,
																	f_dpdaw_txpow_detect->gainOut[antCnt].K_VAL,
																	f_dpdaw_txpow_detect->gainOut[antCnt].TCB_AVG_VAL,
																	POW_VAL);
			POW_VAL = 0.0;
		}else{
			if( POW_VAL > 0.0 ){
				POW_VAL = round( POW_VAL * 10.0 ) / 10.0;
			}
		}

		f_dpdaw_txpow_detect->gainOut[antCnt].POW_VAL = POW_VAL;
		f_dpdaw_txpow_detect->gainOut[antCnt].REF_VAL = f_dpdaw_statusInfo.txPwrMon[antCnt];

		f_dpdaw_txpow_detect->gainOut[antCnt].validFlg = D_RRH_ON;
		f_dpdaw_statusInfo.txPwrPsum_avg[antCnt] = (UINT)(POW_VAL * 10.0);

	}

	memcpy(&(f_dpdaw_txpow_detect->head.output_date_pre),&(f_dpdaw_txpow_detect->head.output_date),sizeof(f_dpdaw_txpow_detect->head.output_date_pre));
	(VOID)BPF_HM_DEVC_GETTIME(&(f_dpdaw_txpow_detect->head.output_date));

	f_dpda_txpower_log_mmw(D_RRH_ON,"CALCULATED(cal:%u) TX Power Detect Value P V:%f(mW) H:%f(mW)",
							f_dpdaw_l_txpow_detect_calcount,
							f_dpdaw_txpow_detect->gainOut[D_DPDA_ANTA].POW_VAL,
							f_dpdaw_txpow_detect->gainOut[D_DPDA_ANTB].POW_VAL);

	f_dpdaw_info_all->taskInfo.flg.txpower_detect_count = 0;
	f_dpdaw_info_all->taskInfo.flg.txpower_detect_check = D_RRH_ON;
	return;
}


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
INT f_dpda_act_powReportPeriodic_mmW(VOID* msgP)
{
	UINT				uiAntNum	= 0;
	UINT				uiCarNum	= 0;
	USHORT				usAlarmNumOccur = 0;
	USHORT				usAlarmNumClear = 0;

	/* ハードレジスタ情報取得 */
	f_dpda_peri_getHardRegInfo();

	/* PGC_tx power detector監視用レジスタ取得 */
	f_dpda_txpower_detector_mmw();
	
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
		/* ADC受信電力報告									*/
		/****************************************************/
		f_dpda_peri_calcRxPower( uiAntNum, (INT*)&(f_dpdaw_statusInfo.RxPower[uiAntNum]) );
		
	}
	
	/****************************************************/
	/* 温度補償処理										*/
	/****************************************************/
	f_dpda_peri_setTemperature();

	if( D_SYS_ON == f_dpdaw_info_all->almmsk ){
		return D_SYS_OK;	/* SKIP */
	}

	/* アラーム検出 */
	f_dpda_alm_stateUpdate(&usAlarmNumOccur, &usAlarmNumClear);	//ALMの判定を行う処理
	
	/* アラーム発生時解析用ログ収集 */
	f_dpda_alm_logCollect( (usAlarmNumOccur+usAlarmNumClear) );	//debug用のコマンド等で表示する情報をため込む処理
	
	/* アラーム報告 */
	f_dpda_alm_report_mmW(usAlarmNumOccur, usAlarmNumClear);		//SVレジスタに設定する処理
	
	return D_SYS_OK;

}

/* @} */
