/*!
 * @skip  $ld:$
 * @file  f_dpda_init_taskTable.c
 * @brief DPDAタスク タスク内テーブル初期化処理
 * @date  2018/08/13 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:温度ALM閾値の初期設定を行う
 * @note  関数処理内容.
 *			-# sub6,mmWの場合に起動される
 *			-# EEPROM(0x380)から温度ALM閾値をリードする
 *			-#  <リード異常、又は値が0xFFの場合>
 *			-#    閾値を100度で設定する
 *			-#  <値が0xFF以外の場合>
 *			-#    EEPROMから読み出した閾値を設定する
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/08/13 KCN)takagi Create
 */
VOID f_dpda_set_TempAlmThreshold()
{
	UINT	rtn_bpf = 0;
	UCHAR	temp_info = 0;
	UINT	uiAntCnt;

	/* EEPROM(0x380)から温度ALM閾値情報取得 */
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_TEMP_ALM_THRESHOLD, &temp_info);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
	{
		/* readNG。閾値にはデフォルト値(100℃)を設定 */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "TEMP_INFO error. rtn_bpf=%08x", rtn_bpf);
		temp_info = D_DPDA_DEFAULT_TEMP_ALM_THRESHOLD;
	}
	/* EEPROM 0x380のread値によって、値が決まる */
	/* 0xFF⇒100℃(デフォルト値)、左記以外⇒read値を閾値として設定する */
	if(temp_info == 0xFF)
	{
		temp_info = D_DPDA_DEFAULT_TEMP_ALM_THRESHOLD; /* 100度 */
	}
	for( uiAntCnt = 0; uiAntCnt < D_DPDA_ANTMAX; uiAntCnt++ )
	{
		f_dpdaw_info_all->almThreshold[E_DPDA_ALM_THRESH_HIGH_TEMP][uiAntCnt] = temp_info * 10;	/* 10倍して設定する */
	}
}

/*!
 * @brief 関数機能概要:Index情報を作成
 * @note  関数処理内容:Index情報を作成
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2019/03/29 FJT)Taniguchi
 */
VOID f_dpda_set_index(VOID)
{
	UINT	index;
	UINT	max;

	if (f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		max = D_DPDA_REG_SUB6_NUM_MAX;
		for(index=0 ; index < max ; index++){
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CTB_SV1INF){
				f_dpdaw_index_SvBusPos = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CC_ALPFTSSI0){
				f_dpdaw_index_TssiPos = index;
			}
			/*	アンテナ送信電力(POWREFRF->POWREFSW)	*//*	apdハソv1.13	*/
			/*	FB電力1(POWREFSW)	*/
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_FBANT_POWREFSW){
				f_dpdaw_index_AntPowPos = index;
				f_dpdaw_index_RefPowPos = index;
			}
			/*	FB電力2(POWFBKSW)	*/
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_FBANT_POWFBKSW){
				f_dpdaw_index_FbPowPos = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_FBANT_POWRVSSW){
				f_dpdaw_index_VswrPos  = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_FBANT_POWREFSW_VSWR){
				f_dpdaw_index_RefPowPos_VSWR  = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_RX_RXPOWREP){
				f_dpdaw_index_RxPowPos = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_RX_RTWPREP0){
				f_dpdaw_index_RtwpPos = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CA_TXANTEN){
				f_dpdaw_index_AntDisPos = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CA_TXCAREN){
				f_dpdaw_index_CarrierPos = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CTB_STA1){
				f_dpdaw_index_A000_0200 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CTB_STA2){
				f_dpdaw_index_A000_0210 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CTB_ICTXERR){
				f_dpdaw_index_A000_0B10 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CTB_ICRXERR){
				f_dpdaw_index_A000_0B20 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CTB_ICRXERR2){
				f_dpdaw_index_A000_0B30 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CTX_ICTXERR){
				f_dpdaw_index_B000_0B10 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CTX_ICRXERR){
				f_dpdaw_index_B000_0B20 = index;
			}
			if(f_dpdar_reg_Tbl[index] ==D_DU_REG_CTX_STA1){
				f_dpdaw_index_B000_0200 = index;
			}
			if(f_dpdar_reg_Tbl[index] ==D_DU_REG_CTX_STA2){
				f_dpdaw_index_B000_0210 = index;
			}

			if(f_dpdar_reg_Tbl[index] == D_DU_REG_CTX_MBCEUE){
				f_dpdaw_index_B000_02C0 = index;
			}

			if(f_dpdar_reg_Tbl[index] == D_DU_REG_TCX_TDANTENA){
				f_dpdaw_index_B100_0040 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_TCX_TDANTENB){
				f_dpdaw_index_B100_0044 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_TCX_TDANTENC){
				f_dpdaw_index_B100_0048 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_TCX_TDANTEND){
				f_dpdaw_index_B100_004C = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_TCB_TDDMYDSL08){
				f_dpdaw_index_A200_0720 = index;
			}
			if(f_dpdar_reg_Tbl[index] == 0xA20143C8){
				f_dpdaw_index_A201_43C8 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_TCX_TDANTEN2A){
				f_dpdaw_index_B100_0050 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_TCX_TDANTEN2B){
				f_dpdaw_index_B100_0054 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_TCX_TDANTEN2C){
				f_dpdaw_index_B100_0058 = index;
			}
			if(f_dpdar_reg_Tbl[index] == D_DU_REG_TCX_TDANTEN2D){
				f_dpdaw_index_B100_005C = index;
			}

		}
	}else{
		max = D_DPDA_REG_MMW_NUM_MAX;
		for(index=0 ; index < max ; index++){
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CTB_SV1INF){
				f_dpdaw_index_SvBusPos = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CC_ALPFTSSI0_MMW){
				f_dpdaw_index_TssiPos = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CA_TSSILAT){
				f_dpdaw_index_AntPowPos = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] ==D_DU_REG_RX_RXPOWREP){
				f_dpdaw_index_RxPowPos = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] ==D_DU_REG_RX_RTWPREP0){
				f_dpdaw_index_RtwpPos = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CA_TXANTEN){
				f_dpdaw_index_AntDisPos = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CA_TXCAREN){
				f_dpdaw_index_CarrierPos = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CTB_STA1){
				f_dpdaw_index_A000_0200 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] ==D_DU_REG_CTB_STA2){
				f_dpdaw_index_A000_0210 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CTB_ICTXERR){
				f_dpdaw_index_A000_0B10 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CTB_ICRXERR){
				f_dpdaw_index_A000_0B20 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CTB_ICRXERR2){
				f_dpdaw_index_A000_0B30 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CTX_ICTXERR){
				f_dpdaw_index_B000_0B10 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CTX_ICRXERR){
				f_dpdaw_index_B000_0B20 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] ==D_DU_REG_CTX_STA1){
				f_dpdaw_index_B000_0200 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] ==D_DU_REG_CTX_STA2){
				f_dpdaw_index_B000_0210 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_CTX_MBCEUE){
				f_dpdaw_index_B000_02C0 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_TCX_TDANTENA){
				f_dpdaw_index_B100_0040 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_TCX_TDANTENB){
				f_dpdaw_index_B100_0044 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_TCB_TDDMYDSL08){
				f_dpdaw_index_A200_0720 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == 0xA20143C8){
				f_dpdaw_index_A201_43C8 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_TCX_TDANTEN2A){
				f_dpdaw_index_B100_0050 = index;
			}
			if(f_dpdar_reg_Tbl_mmW[index] == D_DU_REG_TCX_TDANTEN2B){
				f_dpdaw_index_B100_0054 = index;
			}
		}
	}
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "INDEX TSSI:%d ANTPOW:%d REFPOW:%d FBPOW:%d VSWR:%d REFPOW_VS:%d RXPOW:%d RTWP:%d CAR:%d ANTDIS:%d SV:%d",
						f_dpdaw_index_TssiPos,f_dpdaw_index_AntPowPos,f_dpdaw_index_RefPowPos,f_dpdaw_index_FbPowPos,f_dpdaw_index_VswrPos, f_dpdaw_index_RefPowPos_VSWR,
						f_dpdaw_index_RxPowPos,f_dpdaw_index_RtwpPos,f_dpdaw_index_CarrierPos,f_dpdaw_index_AntDisPos,f_dpdaw_index_SvBusPos);
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "      A_20:%d A_21:%d A_B1:%d A_B2:%d B_B1:%d B_B2:%d B_B3:%d B_2C:%d B_20:%d B_21:%d B1_40:%d B1_44:%d B1_48:%d B1_4C:%d A2_72:%d", 
						f_dpdaw_index_A000_0200,f_dpdaw_index_A000_0210,f_dpdaw_index_A000_0B10,f_dpdaw_index_A000_0B20,
						f_dpdaw_index_B000_0B10,f_dpdaw_index_B000_0B20,f_dpdaw_index_A000_0B30,f_dpdaw_index_B000_02C0,
						f_dpdaw_index_B000_0200,f_dpdaw_index_B000_0210,
						f_dpdaw_index_B100_0040,f_dpdaw_index_B100_0044,f_dpdaw_index_B100_0048,f_dpdaw_index_B100_004C,
                        f_dpdaw_index_A200_0720
						);
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "      B1_50:%d B1_54:%d B1_58:%d B1_5C:%d", 
                        f_dpdaw_index_B100_0050,f_dpdaw_index_B100_0054,f_dpdaw_index_B100_0058,f_dpdaw_index_B100_005C
						);

	return;
}


/*!
 * @brief 関数機能概要:タスク内テーブル初期化処理を行う
 * @note  関数処理内容.
 *			-# 送信メッセージ格納テーブル初期化
 *			-# DPDAデバッグログ情報共有メモリアクセス
 *			-# システムパラメータ格納テーブル初期化
 *			-# システムパラメータ格納テーブル初期化
 *			-# ANT/CARstaus格納テーブル初期化
 *			-# レジスタデータ格納テーブル初期化
 *			-# システムパラメータ情報格納テーブル初期化
 *			-# キャリア毎の微調整設定係数テーブル初期化
 *			-# TXゲイン設定値格納テーブル初期化
 *			-# RXゲイン設定値格納テーブル初期化
 *			-# フラグ初期化
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date  2018/08/13 KCN)takagi Create
 */
VOID f_dpda_init_taskTable()
{
	UINT	uiRet = D_SYS_OK;
	INT		errcd;
	UINT	usAlmIndexCnt;
//	UINT	uiAntCnt;
	UINT	uiAntCnt, uiCarCnt;
	T_DPDA_ALM_THRESHOLD_INFO	*almThresh;
	UCHAR	eep_data;
	UINT	PaOnOff;

	UINT	dlfCnt;
	UINT	dlf_GrpCnt;

	/* アラームログ共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DPDA_ALM_LOG,(VOID**)&f_dpdaw_almLog,&errcd);

	/* DPDAデバッグログ情報共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DPDA_MNG_TBL,(VOID**)&f_dpdaw_info_all,&errcd);
	
	/* Alarm History log収集準備Table共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_ALM_HIS_PREPARE,(VOID**)&f_dpdaw_almLogForHistory,&errcd);
	
	/* Temprature Log Table共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_TEMP_LOG_TBL,(VOID**)&f_dpdaw_tempLog,&errcd);
	
	/* キャリア状態ログ 共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CAR_STAT_HIS,(VOID**)&f_dpdaw_carStateLog,&errcd);

	/* TX Power情報共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_TXPOW_DETECT_TBL,(VOID**)&f_dpdaw_txpow_detect,&errcd);

	/* TX Powerログ共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_TXPOW_DETECT_LOG,(VOID**)&f_dpdaw_txpow_log,&errcd);

	/* LUTログ共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_LUT_LOG,(VOID**)&f_dpdaw_lut_log,&errcd);

	/* 主信号停波ログ共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_UPSIGSTOP_HIS,(VOID**)&f_dpdaw_upsigstop_log,&errcd);

	/* CALログ共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_CAL_HIS,(VOID**)&f_dpdaw_cal_log,&errcd);

	/* DLF統計情報ログ共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DLF_LOG,(VOID**)&f_dpdaw_dlf_log,&errcd);

	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Get shared memory failed [%d]", errcd);
	}

	/* 時刻初期化 */
	(VOID)BPF_HM_DEVC_GETTIME(&(f_dpdaw_txpow_detect->head.output_date_pre));
	(VOID)BPF_HM_DEVC_GETTIME(&(f_dpdaw_txpow_detect->head.output_date));

	f_dpdaw_upsigstop_log->devkind = f_dpdaw_reKind;


	/* T_RRH_ALM_LOG_FOR_HISTの初期化 */
	f_dpdaw_almLogForHistory->dataPosi=0;  /* 次回更新位置を初期化 */

	/* キャリアコンフィグ情報テーブル初期化 */
	memset(&f_dpdaw_antCarStateInfo, 0x00, sizeof(T_DPDA_ANTCARSTATEINFO));
	

	/* キャリア状態ログ初期化 */
	memset(f_dpdaw_carStateLog, 0x00, sizeof(T_DPDA_LOG_CAR_STATE));
	
	/* レジスタデータ格納テーブル初期化 */
	memset(&f_dpdaw_regdataReal, 0x00, sizeof(f_dpdaw_regdataReal));

	/* ステータス情報格納テーブル初期化 */
	memset(&f_dpdaw_statusInfo, 0x00, sizeof(f_dpdaw_statusInfo));

	/* ALM閾値初期化	*/
	almThresh = (T_DPDA_ALM_THRESHOLD_INFO *)f_dpdar_almThreshold_list[ f_dpdaw_ruIndex ];
	for( usAlmIndexCnt=0; usAlmIndexCnt<E_DPDA_ALM_THRESH_NUM_MAX; usAlmIndexCnt++ )
	{
		for( uiAntCnt=0; uiAntCnt<D_DPDA_ANTMAX; uiAntCnt++ )
		{
			f_dpdaw_info_all->almThreshold[usAlmIndexCnt][uiAntCnt]
				= almThresh[usAlmIndexCnt].Threshold;
		}
	}
	
	/* 仮想レジスタ情報初期化 */
	memcpy( (VOID *) f_dpdaw_info_all->virtualRegData,
			(VOID *) f_dpdar_virtualRegData,
			sizeof(f_dpdar_virtualRegData) );

	if(f_cmw_tra_inv_tbl->cmx_txgain > 10){
		f_dpdaw_info_all->taskInfo.txGainCoefNo = 0;
	}else{
		f_dpdaw_info_all->taskInfo.txGainCoefNo = (UINT)(f_cmw_tra_inv_tbl->cmx_txgain);
	}

	/* 温度ALM閾値の初期設定を行う */
	f_dpda_set_TempAlmThreshold();

	/* 装置種別によるアンテナ数、ユニット数、ALM情報テーブルの設定 */
	switch(f_dpdaw_reKind)
	{
		/* sub6の場合 */
		case E_DPDA_REKIND_5GDU_SUB6:
			f_dpdaw_ant_max_num = D_DPDA_ANTMAX;
			f_dpdaw_ant_unit_max_num = D_RRH_ANT_UNIT_MAX;
		
			if(f_cmw_tra_inv_tbl->cmx_device_BW == D_DU_EEP_DEV_FREQ_45G){
				f_dpdaw_carrier_max_num = D_DPDA_CARMAX_SUB6_45G;
			}else{
				f_dpdaw_carrier_max_num = D_DPDA_CARMAX_SUB6;
			}
			
			f_dpdaw_almInfo = (T_DPDA_ALM_INFO *)f_dpdar_almInfo_sub6;
			for( uiAntCnt=0; uiAntCnt<D_DPDA_ANTMAX; uiAntCnt++ ){
				f_dpdaw_info_all->taskInfo.fbGainCoefNo[uiAntCnt] = f_dpda_IO_read( M_DU_REG_FBANT_DEMGAIN(uiAntCnt),E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_READ );
			}

			break;
		
		/* mmWの場合 */
		case E_DPDA_REKIND_5GDU_MMW:
			f_dpdaw_ant_max_num = D_DPDA_ANTMAX_MMW;
			f_dpdaw_ant_unit_max_num = D_RRH_ANT_UNIT_MAX_MMW;
			f_dpdaw_carrier_max_num = D_DPDA_CARMAX;
			f_dpdaw_almInfo = (T_DPDA_ALM_INFO *)f_dpdar_almInfo_mmW;

			break;
		
		default:
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Impossible transition route. reKind=%d.", f_dpdaw_reKind );
			return;
	}
	
	f_dpda_set_index();

	/* 装置種別設定 */
	f_dpdaw_almLog->rrhKind = f_comw_rrhKind;
	
	/* 温度情報初期化 */
	/* 起動直後は測定情報を保存したいため 1を設定 */
	f_dpdaw_tempLog->usCurCnt = 1;
	f_dpdaw_tempLog->usTimerVal = D_RRH_LOG_TEMP_MEAS_PERIOD;
	
	/* キャリアモード初期化(TP用) */
	f_dpdaw_info_all->carModflg  = D_DPDA_CAR_NORMAL;

	/* キャリアスリープ初期化 */
	for( uiAntCnt=0; uiAntCnt<f_dpdaw_ant_max_num; uiAntCnt++ ){
		for( uiCarCnt=0; uiCarCnt<f_dpdaw_carrier_max_num; uiCarCnt++){
			f_dpdaw_antCarStateInfo.ant_info[uiAntCnt].car_info[D_SYS_ANT_DL][uiCarCnt].CarSleep = E_UP_CAR_ACTIVE_ACTIVE;
		}
	}

	BPF_HM_DEVC_EEPROM_READ( D_DU_EEP_PAONOFF, &eep_data );
	if(eep_data == 0){
		PaOnOff = D_SYS_OFF;
	}else{
		PaOnOff = D_SYS_ON;
	}

	/* ant info table初期化 */
	/* PA状態は初期状態を設定(デバッグ用) */
	for( uiAntCnt=0; uiAntCnt< f_dpdaw_ant_max_num; uiAntCnt++ ){
		f_dpdaw_antCarStateInfo.ant_info[uiAntCnt].PaOnOff = PaOnOff;
		f_dpdaw_antCarStateInfo.ant_info[uiAntCnt].ApdStopWaveState = D_SYS_OFF;
	}

	/* DLF統計情報取得初期化 */
	/* 最初起動以外の場合は次レコードを初期化する */
	if(f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].add_time != 0){
		f_dpdaw_dlf_log->uiWriteIdx++;	
		/* 1周終了したタイミングでリセットされた時、次周の最初に移す */
		if(f_dpdaw_dlf_log->uiWriteIdx >= D_DU_LOG_DLF_HIS_TIME){
			f_dpdaw_dlf_log->uiWriteIdx = 0;
			f_dpdaw_dlf_log->dlffin_flag = D_RRH_ON;
		}
	}
	f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].add_time = 0;
	for(dlfCnt = 0; dlfCnt < D_RRH_POLARIZATION_NUM; dlfCnt++){
		for(dlf_GrpCnt = 0; dlf_GrpCnt < D_DU_LOG_DLF_HIS_GRP_NUM; dlf_GrpCnt++){
			f_dpdaw_dlf_log->dlf_info[f_dpdaw_dlf_log->uiWriteIdx].regdat_sum[dlfCnt][dlf_GrpCnt] = 0;
		}
	}

	return;
}

/* @} */
