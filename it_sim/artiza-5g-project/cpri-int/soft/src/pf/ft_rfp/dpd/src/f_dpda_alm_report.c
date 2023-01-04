/*!
 * @skip  $ld:$
 * @file  f_dpda_alm_report.c
 * @brief アラームデータ報告処理
 * @date  2012/01/26 ALP)藤井 Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */

/*!
 * @addtogroup RRH_DPDA
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:アラーム報告処理
 * @note  関数処理内容.
 *	-# dpda検出の発生アラーム報告を行う
 *	-# dpda検出の回復アラーム報告を行う
 * @param  usNumOfAlarmOccur		[out]USHORT	アラーム発生数
 * @param  usNumOfAlarmClear		[out]USHORT	アラーム回復数
 * @return VOID
 * @date 2011/07/19 ALPHA)藤井 
 * @date 2012/01/26 ALPHA)藤井 Create(M-RRU-ZSYS-00798対処)
 * @date 2013/06/03 ALPHA)中村 B25対応
 */
VOID f_dpda_alm_report_fhm( USHORT	usNumOfAlarmOccur,
						   USHORT	usNumOfAlarmClear)
{
	UINT	almCnt;
	
	/* アラーム発生回復情報の通知 */
	if( usNumOfAlarmOccur > 0 )
	{
		for( almCnt=0; almCnt < usNumOfAlarmOccur; almCnt++ )
		{
			if( D_SYS_ON == f_dpdaw_info_all->taskInfo.almMaskEep )
			{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "stop alarm report(0x%04x)", f_dpdaw_occurAlmInstanceId[ almCnt ] );
				continue;
			}
			
			f_com_almReportFHM( D_SYS_THDID_PF_F_DPD, f_dpdaw_occurAlmInstanceId[ almCnt ] );
		}
	}
	
	/* DCM装置にはアラーム回復なし */
	
	return;
}
/* @} */
