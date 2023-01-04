/*!
 * @skip  $ld:$
 * @file  f_dpda_act_tdden.c
 * @brief TDD EN Change
 * @date  2019/3/29 FJT)Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:TDD EN CHG処理
 * @note  関数処理内容:TDD EN CHG処理
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date  2019/3/29 FJT)Taniguchi
 */
INT f_dpda_act_tdden_ntc(VOID* rcvMsg_p)
{
	INT							rtn;			/*	処理結果格納変数		*/
	T_SYS_RF_TDDEN_CHG_NTC*		msgPtr;			/*	メッセージ変数			*/
	UINT						regAddr;
	UINT						regVal;
	CHAR						antStr[1];
	UINT						PaOnOff;

	rtn = D_SYS_OK;

	msgPtr = (T_SYS_RF_TDDEN_CHG_NTC*)rcvMsg_p;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER, "%s call AntNo:%d VAL:0x%x",__func__,msgPtr->tddEn.uiAntNo,msgPtr->tddEn.uichgVal);

	switch(msgPtr->tddEn.uiAntNo){
		case D_DPDA_ANTA:
			regAddr = D_DU_REG_TCX_TDANTENA;
			antStr[0] = 'A';
			break;
		case D_DPDA_ANTB:
			regAddr = D_DU_REG_TCX_TDANTENB;
			antStr[0] = 'B';
			break;
		case D_DPDA_ANTC:
			regAddr = D_DU_REG_TCX_TDANTENC;
			antStr[0] = 'C';
			break;
		case D_DPDA_ANTD:
			regAddr = D_DU_REG_TCX_TDANTEND;
			antStr[0] = 'D';
			break;
		default:
			return D_SYS_OK;
			break;
	}

	if( msgPtr->tddEn.uichgVal < 0x10 ){	/* 0x0 or 0x1 or 0x3 */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,regAddr,&regVal);
		if(regVal != msgPtr->tddEn.uichgVal){
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,regAddr,&(msgPtr->tddEn.uichgVal));

			if(((regVal & 0x02 ) == 0) && (( msgPtr->tddEn.uichgVal & 0x02 ) != 0)){

				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Ant-%c TDD ANT ENABLE CHANGE 0x%x -> 0x%x (PA-ON)",antStr[0],regVal, msgPtr->tddEn.uichgVal );
				PaOnOff = D_SYS_ON;

			}else if(((regVal & 0x02 ) != 0 ) && (( msgPtr->tddEn.uichgVal & 0x02 ) == 0)){

				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Ant-%c TDD ANT ENABLE CHANGE 0x%x -> 0x%x (PA-OFF)",antStr[0],regVal, msgPtr->tddEn.uichgVal );
				PaOnOff = D_SYS_OFF;

			}else{

				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "Ant-%c TDD ANT ENABLE CHANGE 0x%x -> 0x%x (PA-NoChange)",antStr[0],regVal, msgPtr->tddEn.uichgVal );
				PaOnOff = f_dpdaw_antCarStateInfo.ant_info[msgPtr->tddEn.uiAntNo].PaOnOff;
			}
			
			f_dpdaw_antCarStateInfo.ant_info[msgPtr->tddEn.uiAntNo].PaOnOff = PaOnOff;

		}

	}
	
	if( f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6 ){
		UINT				uiCarCnt;
		T_DPDA_ANT_INFO		*antInfo;
		T_DPDA_CAR_INFO		*carInfo;
        UINT                ant_pair[D_DPDA_ANTMAX/2]={};
        UINT                uiAntCnt;
        UINT                polaCnt = 0;

		antInfo = &(f_dpdaw_antCarStateInfo.ant_info[ msgPtr->tddEn.uiAntNo ]);
		for( uiCarCnt=0; uiCarCnt < f_dpdaw_carrier_max_num; uiCarCnt++ ){
			carInfo = &(antInfo->car_info[ D_SYS_ANT_DL ][ uiCarCnt ]);
			if(( D_DPDA_CAR_ON == carInfo->CarOnOff ) && (carInfo->CarSleep == E_UP_CAR_ACTIVE_ACTIVE)){
				f_dpda_com_setDpdStop( msgPtr->tddEn.uiAntNo );
				BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "Ant-%c DPD was turned OFF by ceasing StopWave(0x%x)",antStr[0],msgPtr->tddEn.uichgVal);
				/* 条件A契機 */
				if(msgPtr->tddEn.uiDpdCtl == D_SYS_ON){
					regVal = 0x0;
					BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,M_DU_REG_CC_ACWEN(msgPtr->tddEn.uiAntNo),&regVal);
					f_dpda_com_setDpdStart( msgPtr->tddEn.uiAntNo , D_RRH_OFF);
					BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "Ant-%c DPD was turned ON by ceasing StopWave(0x%x)",antStr[0],msgPtr->tddEn.uichgVal);
				}
				/* 条件B契機 */
				else{
					regVal = 0x0;
					BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,M_DU_REG_CC_ACWAMPI(msgPtr->tddEn.uiAntNo),&regVal);
					BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,M_DU_REG_CC_ACWAMPQ(msgPtr->tddEn.uiAntNo),&regVal);
					regVal = 0x3;
					BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,M_DU_REG_CC_ACWEN(msgPtr->tddEn.uiAntNo),&regVal);

                    switch(msgPtr->tddEn.uiAntNo) {
                        case D_DPDA_ANTA:
                        case D_DPDA_ANTC:
                            ant_pair[0] = D_DPDA_ANTA;
                            ant_pair[1] = D_DPDA_ANTC;
                            polaCnt     = D_DPDA_ANT_AC;
                        break;
                        case D_DPDA_ANTB:
                        case D_DPDA_ANTD:
                            ant_pair[0] = D_DPDA_ANTB;
                            ant_pair[1] = D_DPDA_ANTD;
                            polaCnt     = D_DPDA_ANT_BD;
                        break;
                        default:
                            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Ant no:%d error", msgPtr->tddEn.uiAntNo);
                            return D_DPDA_RTN_NG;
                            break;
                    }
					/* BFモードチェック (BFモード時のみ処理実行)	*/
					if( f_cmw_tra_inv_tbl->cmx_bfmode != D_RRH_OFF ) {
                        for(uiAntCnt=0; uiAntCnt<D_DPDA_ANTMAX/2; uiAntCnt++) {
                            rtn = f_dpda_car_TxantcarStop(ant_pair[uiAntCnt]);
                            if(rtn != D_DPDA_RTN_COMPLETE) {
                                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TxantcarStop error=0x%08x ant=%d", rtn, ant_pair[uiAntCnt]);
                                return D_DPDA_RTN_NG;
                            }
                            rtn = f_dpda_car_RxantcarStop(ant_pair[uiAntCnt]);
                            if(rtn != D_DPDA_RTN_COMPLETE) {
                                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RxantcarStop error=0x%08x ant=%d", rtn, ant_pair[uiAntCnt]);
                                return D_DPDA_RTN_NG;
                            }
                        }
                        f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[polaCnt] = D_RRH_ON;
                        f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[polaCnt] = D_RRH_ON;
					}
				}
				break;
			}
		}
		if(msgPtr->tddEn.uiDpdCtl == D_SYS_ON) {
			f_dpdaw_antCarStateInfo.ant_info[msgPtr->tddEn.uiAntNo].ApdStopWaveState = D_SYS_OFF;
            f_dpda_com_clearQEC();
		}
		else {
			f_dpdaw_antCarStateInfo.ant_info[msgPtr->tddEn.uiAntNo].ApdStopWaveState = D_SYS_ON;
		}
	}

	return rtn;
}

/* @} */
