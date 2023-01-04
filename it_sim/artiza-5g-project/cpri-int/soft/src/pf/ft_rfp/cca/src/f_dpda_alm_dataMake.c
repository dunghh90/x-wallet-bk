/*!
 * @skip  $ld:$
 * @file  f_dpda_alm_dataMake.c
 * @brief アラームデータ作成処理
 * @date  2011/07/19 ALP)藤井 Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2011-2013
 */

/*!
 * @addtogroup RRH_DPDA
 * @{
 */
#include "f_dpda_inc.h"


static UINT f_dpda_alarm_TRX1_AUTO_CR_HC_ERR_cnt = 0;


/*!
 * @brief 関数機能概要:アラーム連続発生回数判定および報告データ作成処理
 * @note  関数処理内容.
 *	-# dpda検出の全アラーム分、処理を行う
 *	-# 回復→発生の変化あり、アラーム発生回数の条件を満たす場合、アラーム発生データ作成
 *	-# 発生→回復の変化あり、回復条件ありのアラームで、アラーム回復回数の条件を満たす場合、アラーム回復データ作成
 * @param  usNumOfAlarmOccur		[out]USHORT*	アラーム発生数
 * @param  usNumOfAlarmClear		[out]USHORT*	アラーム回復数
 * @return VOID
 * @date 2011/07/19 ALPHA)藤井 Create
 * @date 2012/01/26 ALPHA)藤井 M-RRU-ZSYS-00798対処
 * @date 2013/06/03 ALPHA)中村 B25対応
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
VOID f_dpda_alm_dataMake(	USHORT	*usNumOfAlarmOccur,
							USHORT	*usNumOfAlarmClear)
{
	UINT				almCnt;								/* DPDA管理アラームのカウンタ       */
	T_DPDA_ALM_STATE	*almState_p;						/* アラーム状態テーブル参照ポインタ */
	T_DPDA_ALM_INFO		*almInfo_p;							/* アラーム情報テーブル参照ポインタ */
	USHORT				numOfOccurAlm;						/* 発生アラーム数                   */
	USHORT				numOfClearAlm;						/* 回復アラーム数                   */
	UINT				uiAlarmMaxNum;	
	UINT				uiAntCnt;

	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		uiAlarmMaxNum = E_DPDA_ALM_SUB6_NUM_MAX;
	}
	else{
		uiAlarmMaxNum = E_DPDA_ALM_MMW_NUM_MAX;
	}

	/* 関数内変数初期化 */
	memset((VOID*)f_dpdaw_occurAlmInstanceId, 0x00, sizeof(f_dpdaw_occurAlmInstanceId));
	numOfOccurAlm=0;
	memset((VOID*)f_dpdaw_clearAlmInstanceId, 0x00, sizeof(f_dpdaw_clearAlmInstanceId));
	numOfClearAlm=0;
	
	f_dpdaw_info_all->startUpTime ++;
	
	for( almCnt=0; almCnt<uiAlarmMaxNum; almCnt++)
	{
		almState_p = &f_dpdaw_almState[almCnt];
		almInfo_p  = (T_DPDA_ALM_INFO *)&(f_dpdaw_almInfo[almCnt]);

		if (f_dpdaw_reKind == E_DPDA_REKIND_5GDU_MMW) {
			/* TX power detectの場合は、チェックが有効の場合のみチェックする。無効の場合は判定をskipする */
			if((almInfo_p->instanceId >= E_DPDA_ALM_MMW_TX_PWR_ALM_V01) && (almInfo_p->instanceId <= E_DPDA_ALM_MMW_TX_PWR_ALM_H16)){
				if(f_dpdaw_info_all->taskInfo.flg.txpower_detect_check == D_RRH_OFF){
					continue;
				}
			}
		}
		
		/* アラーム統計情報収集処理      */
		/* 1秒周期の発生回復数を保持する */
		if( almState_p->almStateNow == D_SYS_ON )
		{
			(almState_p->almOccurNumTotal1sec)++;
		}
		else
		{
			(almState_p->almClearNumTotal1sec)++;
		}
		
		/* 現時点でのアラーム発生状態と今回の周期でアラーム発生状態を比較し */
		/* 差分がある場合に、アラーム発生回復処理を実施する                 */
		if( almState_p->almState != almState_p->almStateNow )
		{
			/********************************************************************************************************/
			/*	アラーム発生処理																					*/
			/********************************************************************************************************/
			if( almState_p->almStateNow == D_SYS_ON )
			{
				/* アラームカウンタ設定                 */
				/* アラーム発生カウンタをインクリメント */
				almState_p->almOccurNum1sec++;

				/* 1秒周期アラーム発生のため、回復カウンタは0クリア */
				almState_p->almClearNum1sec=0;

				switch(almState_p->almOccurNum1sec){
					case 5:
						if (f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6) {
							switch(almInfo_p->instanceId){
								case  E_DPDA_ALM_SUB6_TX_GAIN_ALM_ANT_A:
								case  E_DPDA_ALM_SUB6_TX_GAIN_ALM_ANT_B:
								case  E_DPDA_ALM_SUB6_TX_GAIN_ALM_ANT_C:
								case  E_DPDA_ALM_SUB6_TX_GAIN_ALM_ANT_D:
									uiAntCnt = almInfo_p->instanceId - E_DPDA_ALM_SUB6_TX_GAIN_ALM_ANT_A;
									BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Starting DPD Failed Safe(Ant-%d)",uiAntCnt);
									f_dpda_com_setDpdFailSafe(uiAntCnt);
									break;
								default:
									break;
							}
						}
						break;
					case 3:
						if((f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6) && (almInfo_p->instanceId == E_DPDA_ALM_SUB6_TRX1_AUTO_CR_HC_ERR)) {
							f_dpda_alarm_TRX1_AUTO_CR_HC_ERR_cnt++;
							if(f_dpda_alarm_TRX1_AUTO_CR_HC_ERR_cnt > almInfo_p->almOccurNum ){
								almState_p->almOccurNum1sec = almInfo_p->almOccurNum;
							}else{
								almState_p->almOccurNum1sec = 0;
							}
						}
						break;
					default:
						break;
				}

				/* アラーム回数の条件を満たす場合、アラーム発生 */
				if( almState_p->almOccurNum1sec == almInfo_p->almOccurNum )
				{
					/* 発生アラームのInstance IDを保持 */
					f_dpdaw_occurAlmInstanceId[numOfOccurAlm] = almInfo_p->instanceId;

					numOfOccurAlm++;

					/* DPDA内部情報の更新(アラーム発生) */
					almState_p->almState = D_SYS_ON;
					(almState_p->almOccurNumTotal)++;

					/* 温度アラーム発生の場合 */
					if(memcmp(almInfo_p->Name, "Temperature too high", 20) == 0)
					{
						/* 温度アラームが発生したら回復させるために、閾値を-3度下げる */
						for( uiAntCnt=0; uiAntCnt<D_DPDA_ANTMAX; uiAntCnt++ )
						{
							f_dpdaw_info_all->almThreshold[E_DPDA_ALM_THRESH_HIGH_TEMP][uiAntCnt] -= D_DPDA_RECOVER_HIGH_TEMP_ALM;
						}
					}
				}
			
			}
			/********************************************************************************************************/
			/*	アラーム回復処理																					*/
			/********************************************************************************************************/
			else
			{
				/* 回復処理ありのアラームのみ、回復処理を実施 */
				if( almInfo_p->noClearFlag )
				{
					/* 回復なしアラームの場合は、回復処理を実施しない */
				}
				else
				{
					/* アラームカウンタ設定                 */
					/* アラーム回復カウンタをインクリメント */
					almState_p->almClearNum1sec++;
					f_dpda_alarm_TRX1_AUTO_CR_HC_ERR_cnt = 0;
					/* 1秒周期アラーム回復のため、発生カウンタは0クリア */
					almState_p->almOccurNum1sec=0;

					/* アラーム回数の条件を満たす場合、アラーム回復 */
					if( almState_p->almClearNum1sec == almInfo_p->almClearNum )
					{
						/* 回復アラームのInstance IDを保持 */
						f_dpdaw_clearAlmInstanceId[numOfClearAlm] = almInfo_p->instanceId;
						numOfClearAlm++;
						
						/* DPDA内部情報の更新(アラーム回復) */
						almState_p->almState = D_SYS_OFF;
						(almState_p->almClearNumTotal)++;

						/* 温度アラーム回復の場合 */
						if(memcmp(almInfo_p->Name, "Temperature too high", 20) == 0)
						{
							/* 温度アラームが回復したので、閾値を元の値(+3度上げる)に戻す */
							for( uiAntCnt=0; uiAntCnt<D_DPDA_ANTMAX; uiAntCnt++ )
							{
								f_dpdaw_info_all->almThreshold[E_DPDA_ALM_THRESH_HIGH_TEMP][uiAntCnt] += D_DPDA_RECOVER_HIGH_TEMP_ALM;
							}
						}
					}
				}
			}
		}
		/* アラームの変化がない場合は、アラームカウンタのクリア処理を行う       */
		/* アラーム変化なしは、アラーム連続発生回復が途切れたことを意味するため */
		/* アラーム回復中は発生カウンタをクリアする(発生中は回復カウンタを)     */
		else
		{
			/* アラーム発生状態でのアラーム発生 */
			/* アラーム回復カウンタの初期化   */
			if( almState_p->almState == D_SYS_ON )
			{
				almState_p->almClearNum1sec=0;
			}
			/* アラーム回復状態でのアラーム回復 */
			/* アラーム発生カウンタの初期化   */
			else
			{
				almState_p->almOccurNum1sec=0;
			}
		}
	}

	*usNumOfAlarmOccur = numOfOccurAlm;
	*usNumOfAlarmClear = numOfClearAlm;

	return;
}

/* @} */
