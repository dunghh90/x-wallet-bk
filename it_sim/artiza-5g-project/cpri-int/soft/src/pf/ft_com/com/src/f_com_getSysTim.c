/*!
 * @skip  $ld:$
 * @file  f_com_getSysTim.c
 * @brief RTC(local system time)獲得
 * @date  2008/09/01 ALPHA) 井上 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2013
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_com_inc.h"

/*!
 * @brief 関数機能概要:RTC(local system time)獲得関数
 * @note  関数処理内容.
 *       -# 装置時間を読み出してtm型で返す。
 * @param  systim	[in]T_COM_SYSTEM_TIME*			:システム時間
 * @return VOID
 * @retval VOID
 * @warning   N/A
 * @FeatureID PF-COM-001-001-001
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2008/09/01 ALPHA)井上 Create
 * @date 2009/05/07 ALPHA)藤井 M-RRU-ZSYS-00313 対処
 * @date 2009/10/15 ALPHA)藤井 M-RRU-ZSYS-00461 対処
 * @date 2013/03/16 ALPHA)fujiiy    modiry for B25
 * @date 2013/11/26 ALPHA)kondou    zynq化対応
 */
VOID f_com_getSysTim(
			T_RRH_SYSTEM_TIME*	systim )
{
	INT			a_ret;
	
	/* pgr0541	*/
	if(systim == NULL){
		/*	デバッグログ取得	*/
		BPF_COM_LOG_ASSERT ( D_RRH_LOG_AST_LV_WARNING, "systim = NULL" );
	}
	else
	{
		/* 時刻取得						*/
		a_ret = BPF_HM_DEVC_GETTIME(systim);
		if(a_ret != 0)
		{
			BPF_COM_LOG_ASSERT ( D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_GETTIME error code=%d", a_ret );
		}
	}
	
	return;
	
}

/*!
 * @brief 関数機能概要:RTC(local system time)獲得関数(ms以降除く)
 * @note  関数処理内容.
 *       -# 装置時間を読み出してtm型で返す。
 * @param  systim	[in]T_COM_SYSTEM_TIME*			:システム時間
 * @return VOID
 * @retval VOID
 * @warning   N/A
 * @FeatureID PF-COM-001-001-001
 * @Bug_No    N/A
 * @CR_No     N/A
 * @TBD_No    N/A
 * @date 2018/12/11 FJT)Yoshida  Create
 */
VOID f_com_getSysTim_notMs(
			T_RRH_SYSTEM_TIME*	systim )
{
	T_RRH_SYSTEM_TIME						tDateTime;


	/*** date and time設定  ***/
	memset(&tDateTime, D_RRH_ZERO, sizeof(tDateTime));
	(VOID)BPF_HM_DEVC_GETTIME(&tDateTime);

	systim->year  = tDateTime.year;
	systim->month = tDateTime.month;
	systim->day   = tDateTime.day;
	systim->hour  = tDateTime.hour;
	systim->min   = tDateTime.min;
	systim->sec   = tDateTime.sec;

	return;
}

/* @} */
