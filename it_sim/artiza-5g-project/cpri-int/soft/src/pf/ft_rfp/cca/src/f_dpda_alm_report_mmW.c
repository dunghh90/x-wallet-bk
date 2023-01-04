/*!
 * @skip  $ld:$
* @file  f_dpda_alm_report_mmW.c (base: f_dpda_alm_report.c)
 * @brief アラームデータ報告処理
 * @date  2015/09/09 ALP)藤井 Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2015
 */

/*!
 * @addtogroup RRH_DPDA
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:アラーム報告処理(RRE用)
 * @note  関数処理内容.
 *	-# dpda検出の発生アラーム報告を行う
 *	-# dpda検出の回復アラーム報告を行う
 * @param  usNumOfAlarmOccur		[out]USHORT	アラーム発生数
 * @param  usNumOfAlarmClear		[out]USHORT	アラーム回復数
 * @return VOID
 * @date 2015/09/09 ALPHA)藤井 
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
VOID f_dpda_alm_report_mmW(	USHORT	usNumOfAlarmOccur,
							USHORT	usNumOfAlarmClear)
{
	UINT	almCnt;
	UINT	uiAlmHisLogFlg;
	UINT	uiIndex;

	/* この関数はmmW用 */
	
	uiAlmHisLogFlg = D_RRH_OFF;	/* Major ALMかつANT部分のFaultのみ対象とする、その他はinitで取得 */
	
	/* アラーム発生をSVレジスタに反映 */
	if(usNumOfAlarmOccur > 0){
		for(almCnt = 0; almCnt < usNumOfAlarmOccur; almCnt++){
			if(f_dpdaw_occurAlmInstanceId[almCnt] >= E_DPDA_ALM_MMW_NUM_MAX){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BUG Fault Error ID: %d.", f_dpdaw_occurAlmInstanceId[almCnt] );
				break;
			}
			uiIndex = f_dpdaw_occurAlmInstanceId[almCnt];
			if(f_dpdar_alarm_report_tbl_mmw[ uiIndex ].alm_mask == D_SYS_ON ){
				continue;
			}

			f_com_SVCtl(f_dpdar_alarm_report_tbl_mmw[ uiIndex ].ope_fid ,D_DU_SVCTL_ON,f_dpdar_alarm_report_tbl_mmw[ uiIndex ].fltid);
			uiAlmHisLogFlg = f_dpdar_alarm_report_tbl_mmw[ uiIndex ].alm_hislog;
		}
	}

	if(uiAlmHisLogFlg == D_RRH_ON){
		UINT					hwAlmFactAry[E_RRH_ALMHIS_ALM_FACTOR_NUM];
		BPF_HM_DEVC_REG_READ_BUFFER(0, D_DU_REG_CTB_SV1INF,sizeof(hwAlmFactAry), hwAlmFactAry) ;
		f_com_alarmHitoryLogSet( D_RRH_ALARM_FACTOR_TYPE_HW, 0, hwAlmFactAry );
	}
	
	/* アラーム回復をSVレジスタに反映 */
	if(usNumOfAlarmClear > 0){
		for(almCnt = 0; almCnt < usNumOfAlarmClear; almCnt++){
			switch(f_dpdaw_clearAlmInstanceId[almCnt]){
			case E_DPDA_ALM_MMW_TEMP_ALM_HIGH:
				f_com_SVCtl(E_DU_SVCTL_OPE_FID31_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_TEMP_ALM_HIGH);
				break;
			case E_DPDA_ALM_MMW_TEMP_ALM_LOW:
				f_com_SVCtl(E_DU_SVCTL_OPE_FID30_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_TEMP_ALM_LOW);
				break;
			default:
				break;
			}
		}
	}	
	return;
}

/* @} */
