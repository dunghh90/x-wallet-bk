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
//#include "talise_extern.h"

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
 * @date 2018/09/08 KCN)吉木   5G-DU対応
 */
VOID f_dpda_alm_report(	USHORT	usNumOfAlarmOccur,
						USHORT	usNumOfAlarmClear)
{
	UINT	almCnt;
	UINT	uiAlmHisLogFlg;
	UINT	uiIndex;
	USHORT	log[D_RRH_ANT_UNIT_MAX] = {0,0};

	UINT	uiGetTxQecStatus[5];
	UINT	uiGetTxQecStatusFlg = D_RRH_OFF;
	UINT	uiUnitIndex;
	UINT	uiChanelIndex;
	INT		ret;

	/* この関数はsub6用 */
	
	uiAlmHisLogFlg = D_RRH_OFF;	/* Major ALMかつANT部分のFaultのみ対象とする、その他はinitで取得 */
	
	/* アラーム発生をSVレジスタに反映 */
	if(usNumOfAlarmOccur > 0){
		for(almCnt = 0; almCnt < usNumOfAlarmOccur; almCnt++){
			if(f_dpdaw_occurAlmInstanceId[almCnt] >= E_DPDA_ALM_SUB6_NUM_MAX){
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BUG Fault Error ID: %d.", f_dpdaw_occurAlmInstanceId[almCnt] );
				break;
			}
			uiIndex = f_dpdaw_occurAlmInstanceId[almCnt];
			if(f_dpdar_alarm_report_tbl[ uiIndex ].alm_mask == D_SYS_ON ){
				continue;
			}
			/* RFIC#n ALM発生の場合 */
			if(f_dpdar_alarm_report_tbl[ uiIndex ].ope_fid == E_DU_SVCTL_OPE_FID08_ALM)
			{
//				(VOID)rrh_cmd_talise_getGpIntStatus_unit(D_RRH_ANT_UNIT_AB, &log[D_RRH_ANT_UNIT_AB]);
//				(VOID)rrh_cmd_talise_getGpIntStatus_unit(D_RRH_ANT_UNIT_CD, &log[D_RRH_ANT_UNIT_CD]);
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC#%d fault, talise_getGpIntStatus=(unit0:0x%x,unit1:0x%x)", 
									f_dpdar_alarm_report_tbl[ uiIndex ].fltid, log[D_RRH_ANT_UNIT_AB], log[D_RRH_ANT_UNIT_CD]);
			}

			if(uiGetTxQecStatusFlg == D_RRH_OFF){
				for(uiUnitIndex = 0; uiUnitIndex <= RFIC_TALISE_UNIT_MAX; uiUnitIndex++){
					for(uiChanelIndex = RFIC_TALISE_TX1 ; uiChanelIndex <= RFIC_TALISE_TX2 ; uiChanelIndex++){
//						ret = RFIC_TAL_GETTXQECSTATUS( uiUnitIndex , uiChanelIndex , sizeof(uiGetTxQecStatus) , (void*)&uiGetTxQecStatus[0] );
						BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "RFIC talise_getTxQecStatus(%d,%d) ret:%d errCd:0x%x pCmp:0x%x crM:0x%x iCnt:0x%x upCnt:0x%x",
							 uiUnitIndex,uiChanelIndex, ret ,uiGetTxQecStatus[0],uiGetTxQecStatus[1],uiGetTxQecStatus[2],uiGetTxQecStatus[3],uiGetTxQecStatus[4]);
					}
				}
				uiGetTxQecStatusFlg = D_RRH_ON;	/* １回のALMで何度もとらないように */
			}
			f_com_SVCtl(f_dpdar_alarm_report_tbl[ uiIndex ].ope_fid ,D_DU_SVCTL_ON,f_dpdar_alarm_report_tbl[ uiIndex ].fltid);
			uiAlmHisLogFlg = f_dpdar_alarm_report_tbl[ uiIndex ].alm_hislog;

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
			case E_DPDA_ALM_SUB6_TEMP_ALM_HIGH:
				f_com_SVCtl(E_DU_SVCTL_OPE_FID31_ALM, D_DU_SVCTL_OFF, D_DU_FLTID_TEMP_ALM_HIGH);
				break;
			case E_DPDA_ALM_SUB6_TEMP_ALM_LOW:
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
