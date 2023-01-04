/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_checkPaCar.c
 * @brief DPDAタスク PA ON/OFF 状態キャリアON・OFF状態確認処理
 * @date  2011/07/26 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:PA ON/OFF 状態キャリアON・OFF状態確認処理を行う
 * @note  関数処理内容.
 *		-# 返り値にONを設定する
 *		-# 指定ANTのPA ON・OFF状態がOFFの場合以下の処理を行う
 *			-# 返り値にOFFを設定する
 *		-# 指定ANTのPA ON・OFF状態がONの場合以下の処理を行う
 *			-# キャリア数分ループし以下の処理を行う
 *				-# キャリアONの場合はBreak
 *			-# 上記ループがBreakしないで抜けた場合は以下の処理を行う
 *				-# 返り値にOFFを設定する
 * @param  uiAntNum			[in]UINT	アンテナ番号
 * @return UINT			D_SYS_ON:PA ONまたはキャリアON、D_SYS_OFF:PAキャリア共にOFF
 * @date 2011/07/26 ALPHA)横山 Create
 * @date 2011/10/05 ALPHA)横山 SRRU-2.1G対応
 * @date 2012/03/19 ALPHA)横山 N21GLTE対応
 * @date 2012/10/19 ALPHA)横山 N21GLTE対応
 * @date 2013/04/05 ALPHA)中村 B25対応
 */
UINT f_dpda_peri_checkPaCar(UINT uiAntNum)
{
	UINT					uiRtc	 = D_SYS_OFF;
	UINT					uiCarCnt = 0;
	T_DPDA_CAR_INFO*		carInfo  = 0;
	
	/* PA ON/OFF計算結果判定 */
	if( D_SYS_ON != f_dpdaw_antCarStateInfo.ant_info[ uiAntNum ].PaOnOff )
	{
		/* PA OFFの場合はOFFにする	*/
		uiRtc = D_SYS_OFF;
	}
	else
	{
		/* PA ONの場合は全キャリアチェック	*/
		
		/* キャリア状態確認	*/
//		for( uiCarCnt = 0; uiCarCnt < D_DPDA_CARMAX; uiCarCnt++)
		for( uiCarCnt = 0; uiCarCnt < f_dpdaw_carrier_max_num; uiCarCnt++)
		{
			/* 内部テーブルポインタ取得					*/
			carInfo = &(f_dpdaw_antCarStateInfo.ant_info[ uiAntNum ].car_info[ D_SYS_ANT_DL ][ uiCarCnt ]);
			/* TXキャリアONかつTXキャリアスリープOFFの場合 */
//			if( D_DPDA_CAR_OFF != carInfo->CarOnOff )
			if(( D_DPDA_CAR_OFF != carInfo->CarOnOff ) &&
			   ( carInfo->CarSleep == E_UP_CAR_ACTIVE_ACTIVE)) 
			{
				/* 一つでもONがあれば抜ける */
				uiRtc = D_SYS_ON;
				return uiRtc;
			}
		}
	}
	
	return uiRtc;
}

/* @} */
