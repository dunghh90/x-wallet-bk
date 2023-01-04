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
 */
VOID f_dpda_alm_dataMake(	USHORT	*usNumOfAlarmOccur,
							USHORT	*usNumOfAlarmClear)
{
	USHORT				almCnt;								/* DPDA管理アラームのカウンタ       */
	T_DPDA_ALM_STATE	*almState_p;						/* アラーム状態テーブル参照ポインタ */
	T_DPDA_ALM_INFO		*almInfo_p;							/* アラーム情報テーブル参照ポインタ */
	USHORT				numOfOccurAlm;						/* 発生アラーム数                   */
	USHORT				numOfClearAlm;						/* 回復アラーム数                   */

	/* 関数内変数初期化 */
	memset((VOID*)f_dpdaw_occurAlmInstanceId, 0x00, sizeof(f_dpdaw_occurAlmInstanceId));
	numOfOccurAlm=0;
	memset((VOID*)f_dpdaw_clearAlmInstanceId, 0x00, sizeof(f_dpdaw_clearAlmInstanceId));
	numOfClearAlm=0;
	
	f_dpdaw_info_all->startUpTime ++;
	
	for( almCnt=0; almCnt<D_DPDA_ALM_NUM; almCnt++)
	{
		almState_p = &f_dpdaw_almState[almCnt];
		almInfo_p  = (T_DPDA_ALM_INFO *)&f_dpdar_almInfo[almCnt];
		
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

				/* アラーム回数の条件を満たす場合、アラーム発生 */
				if( almState_p->almOccurNum1sec == almInfo_p->almOccurNum )
				{
					/* 発生アラームのInstance IDを保持 */
					f_dpdaw_occurAlmInstanceId[numOfOccurAlm] = almInfo_p->instanceId;
					numOfOccurAlm++;

					/* DPDA内部情報の更新(アラーム発生) */
					almState_p->almState = D_SYS_ON;
					(almState_p->almOccurNumTotal)++;
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
