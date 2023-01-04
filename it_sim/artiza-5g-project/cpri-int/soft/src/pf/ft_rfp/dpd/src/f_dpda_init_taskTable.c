/*!
 * @skip  $ld:$
 * @file  f_dpda_init_taskTable.c
 * @brief DPDAタスク タスク内テーブル初期化処理
 * @date  2013/03/07 ALPHA)松延 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:タスク内テーブル初期化処理を行う
 * @note  関数処理内容.
 *			-# 送信メッセージ格納テーブル初期化
 *			-# DPDAデバッグログ情報共有メモリアクセス
 *			-# ANT/CARstaus格納テーブル初期化
 *			-# レジスタデータ格納テーブル初期化
 *			-# システムパラメータ情報格納テーブル初期化
 * @return VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/03/07 ALPHA)松延 Create
 * @date 2013/11/29 ALPHA)加藤 ZYNQ対応
 * @date 2015/08/06 TDI)satou 「Peak抑圧設定テーブル初期化」削除
 * @date 2015/08/06 TDI)satou 「TXゲイン設定値格納テーブル初期化」削除
 * @date 2015/08/06 TDI)satou 「RXゲイン設定値格納テーブル初期化」削除
 * @date 2015/08/07 TDI)satou 「システムパラメータテーブル初期化」削除
 * @date 2015/08/07 TDI)satou 「DPDAデバッグ用情報ログtable」のフラグ情報初期化を削除
 * @date 2015/08/07 TDI)satou 「ゲイン微調設定値取得」削除
 * @date 2015/08/07 TDI)satou 「キャリアモード初期化」削除
 * @date 2016/01/08 FJT)harada 「温度ALM閾値をEEPROM保持」DCMFHM-0025-160108/DCM-FHMハソ仕様書(更新)対応
 */
VOID f_dpda_init_taskTable()
{
	UINT	uiRet = D_SYS_OK;
	INT		errcd;
	UINT	usAlmIndexCnt;
	UINT	uiAntCnt;
	UCHAR	*invTbl;
	UCHAR	tempalm_thr;

	/* アラームログ共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DPDA_ALM_LOG,(VOID**)&f_dpdaw_almLog,&errcd);

	/* DPDAデバッグログ情報共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DPDA_MNG_TBL,(VOID**)&f_dpdaw_info_all,&errcd);
	
	/* Alarm History log収集準備Table共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_ALM_HIS_PREPARE,(VOID**)&f_dpdaw_almLogForHistory,&errcd);
	
	/* Temprature Log Table共有メモリアクセス */
	uiRet |= BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_TEMP_LOG_TBL,(VOID**)&f_dpdaw_tempLog,&errcd);
	
	if(uiRet != BPF_RU_IPCM_OK)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Get shared memory failed [%d]", errcd);
	}

	/* レジスタデータ格納テーブル初期化 */
	memset(&f_dpdaw_regdataReal, 0x00, sizeof(f_dpdaw_regdataReal));

	/* ステータス情報格納テーブル初期化 */
	memset(&f_dpdaw_statusInfo, 0x00, sizeof(f_dpdaw_statusInfo));
	
	/* ALM閾値初期化	*/
	for( usAlmIndexCnt=0; usAlmIndexCnt<E_DPDA_ALM_THRESH_NUM_MAX; usAlmIndexCnt++ )
	{
		for( uiAntCnt=0; uiAntCnt<D_DPDA_ANTMAX; uiAntCnt++ )
		{
			f_dpdaw_info_all->almThreshold[usAlmIndexCnt][uiAntCnt]
				= f_dpdar_almThreshold[usAlmIndexCnt].Threshold;
		}
	}
	
	/* 温度ALM閾値をインベントリから読み出し */
	uiRet = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
	/* 共有メモリが読み出せない場合はデフォルト値 */
	if(BPF_RU_IPCM_OK != uiRet)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Get shared memory failed [%d]", errcd);
	}else{
		tempalm_thr = invTbl[D_RRH_EEP_TEMPALM];
		/* 0xFF以外なら閾値更新 */
		if( tempalm_thr != 0xFF)
		{
			for( uiAntCnt=0; uiAntCnt<D_DPDA_ANTMAX; uiAntCnt++ )
			{
				f_dpdaw_info_all->almThreshold[E_DPDA_ALM_THRESH_HIGH_TEMP][uiAntCnt] = tempalm_thr;
			}
		}
	}
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Set temp alm threshlod = %d", f_dpdaw_info_all->almThreshold[E_DPDA_ALM_THRESH_HIGH_TEMP][0]);
	
	/* 仮想レジスタ情報初期化 */
	memcpy( (VOID *) f_dpdaw_info_all->virtualRegData,
			(VOID *) f_dpdar_virtualRegData,
			sizeof(f_dpdar_virtualRegData) );
		
	/* 装置種別設定 */
	f_dpdaw_almLog->rrhKind = f_comw_rrhKind;
	
	/* 温度情報初期化 */
	/* 起動直後は測定情報を保存したいため 1を設定 */
	f_dpdaw_tempLog->usCurCnt = 1;
	f_dpdaw_tempLog->usTimerVal = D_RRH_LOG_TEMP_MEAS_PERIOD;
	
	/* EEPROMの値を参照して ALM を抑止する */
	/* TDDでは、SRE用EEPROMも存在しないため、Alarmマスク無しとする */
	f_dpdaw_info_all->taskInfo.almMaskEep = D_SYS_OFF;

	return;
}

/* @} */
