/*!
 * @skip  $ld:$
 * @file  f_dpda_alm_detect.c
 * @brief アラーム検出処理
 * @date  2011/07/19 ALP)藤井 Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2011-2013
 */

/*!
 * @addtogroup RRH_DPDA
 * @{
 */
#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:アラーム情報収集処理
 * @note  関数処理内容.
 *	-# DPDAアラーム数分ループ
 *	-# アラーム要因数分ループし、アラーム要因の判定条件を検出
 *	-# アラーム要因のマージ処理を、テーブルに指定された識別子(AND,OR)に従って実施
 * @param  
 * @return VOID
 * @date 2011/07/19 ALPHA)藤井 Create
 * @date 2011/08/23 ALPHA)藤井 DPDA IT1 No.1対処
 * @date 2011/11/15 ALPHA)藤井 M-RRU-ZSYS-00721対処(アラーム仕様変更)]
 * @date 2013/06/03 ALPHA)中村 B25対応
 * @date 2015/09/09 ALPHA)藤井 RRH-013-000 DCM_3.5G TDD-RRE(Zynq)対応 Create
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
VOID f_dpda_alm_detect(VOID)
{
	UINT					almCnt;						/* アラーム情報のカウンタ			*/
	USHORT					almFactorCnt;				/* アラーム要因情報のカウンタ		*/
	UCHAR					almFactorResult;			/* アラーム要因判定結果(ON/OFF)		*/
	T_DPDA_ALM_STATE		*almState_p;				/* アラーム状態テーブル参照ポインタ	*/
	T_DPDA_ALM_INFO			*almInfo_p;					/* アラーム情報テーブル参照ポインタ	*/
	UINT					uiAlarmMaxNum;
	
	if(f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6){
		uiAlarmMaxNum = E_DPDA_ALM_SUB6_NUM_MAX;
	}
	else{
		uiAlarmMaxNum = E_DPDA_ALM_MMW_NUM_MAX;
	}

	/* ハードレジスタより、アラーム情報を収集 */
	for(almCnt=0; almCnt<uiAlarmMaxNum; almCnt++)
	{
		almState_p = &f_dpdaw_almState[almCnt];
		almInfo_p  = (T_DPDA_ALM_INFO *)&(f_dpdaw_almInfo[almCnt]);

		/* 今回の周期でアラーム発生状態を初期化 */
		almState_p->almStateNow = D_SYS_OFF;
		
		
		/* 複数のアラーム要因から、現時点のアラーム状態を判定する */
		for( almFactorCnt=0; almFactorCnt<(almInfo_p->numOfAlmFactor); almFactorCnt++)
		{
			almFactorResult = D_SYS_OFF;
			f_dpda_alm_factorJudge(	*(almInfo_p->almFactor[almFactorCnt].jdgInfo1),	/* アラーム判定情報1      */
									*(almInfo_p->almFactor[almFactorCnt].jdgInfo2),	/* アラーム判定情報2      */
									almInfo_p->almFactor[almFactorCnt].jdgOperator,	/* 判定演算子             */
									&almFactorResult);								/* アラーム要因(判定結果) */
			
			/* アラーム要因マージ処理 */
			/* アラーム要因をマージし、DPDAとしてのアラームとする */
			switch( almInfo_p->almFactor[almFactorCnt].mergeOperator )
			{
			case E_DPDA_ALM_FACTOR_OPE_AND:
				almState_p->almStateNow &= almFactorResult;		//1個目の判定はORにしとかないとずっと0？
				break;
			case E_DPDA_ALM_FACTOR_OPE_OR:
				almState_p->almStateNow |= almFactorResult;
				break;
			/* 該当アラームが未発生の場合、前半の条件のみ参照 */
			/* 発生中の場合、後半の条件のみ参照               */
			/* つまり、2つの条件があった場合、                */
			/* 前者の条件がアラーム発生条件                   */
			/* 後者の条件がアラーム回復条件となる             */
			case E_DPDA_ALM_FACTOR_OPE_OFF_BEF_ON_AFT:
				if(almState_p->almState)
				{
					/* アラーム発生中は後者の条件のみ参照 */
					almState_p->almStateNow = almFactorResult;
				}
				else
				{
					/* アラーム未発生中は前者の条件のみ参照 */
					/* 後者の条件は参照しない               */
				}

				break;
			default:
				break;
			}
		}
	}

	return;
}

/* @} */
