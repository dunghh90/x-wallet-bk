/********************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_dpda_peri_setQEC.c
 * @brief DPDAタスク QEC処理
 * @date 2019/06/17 Taniguchi
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */
/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

static const T_DPDA_QEC_TBL	f_dpdar_qec_reg_1st[] =
{
	{	0xB1203080 , 0x00000000		},
	{	0xB1203084 , 0x00000000		},
	{	0x00000000 , 0x0000000A		},	/* 10msec wait */
	{	0xB1000734 , 0x00000045		},
	{	0xB1000730 , 0x00000075		},
	{	0x00000000 , 0x0000000A		},	/* 10msec wait */
	{	0xB1000734 , 0x00000055		},
	{	0x00000000 , 0x0000000A		},	/* 10msec wait */
	{	0xB1000730 , 0x00000005		},
	{	0xB1203080 , 0x18000100		},
	{	0xB1203084 , 0x01000040		}
};

static const T_DPDA_QEC_TBL	f_dpdar_qec_reg_2nd[] =
{
	{	0xB1203080 , 0x00000000		},
	{	0xB1203084 , 0x00000000		},
	{	0x00000000 , 0x0000000A		},	/* 10msec wait */
	{	0xB100073C , 0x00000045		},
	{	0xB1000738 , 0x00000075		},
	{	0x00000000 , 0x0000000A		},	/* 10msec wait */
	{	0xB100073C , 0x00000055		},
	{	0x00000000 , 0x0000000A		},	/* 10msec wait */
	{	0xB1000738 , 0x00000005		},
	{	0xB1203080 , 0x18000100		},
	{	0xB1203084 , 0x01000040		}
};

/*!
 * @brief 関数機能概要:ANT ONの判定
 * @note  関数処理内容:ANT ONの判定を行う
 * @param	N/A
 * @return  VOID
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No
 * @date 2019/07/19 FJT)Hasegawa
 
 */
INT f_dpda_com_AntState(UINT uiAntNum)
{
	INT ret = D_DPDA_ANT_DIS, ret_ul = D_DPDA_ANT_DIS, ret_dl = D_DPDA_ANT_DIS;
    UINT count;

    //TXRX両方に1つ以上キャリアON状態のキャリアがあればONで抜ける
    for(count = 0; count < D_RRH_CAR_MAX; count++) {
        if(f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[D_SYS_ANT_UL][count].CarOnOff == D_DPDA_CAR_ON) {
		    ret_ul = D_DPDA_ANT_ENA;
        }
        if(f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[D_SYS_ANT_DL][count].CarOnOff == D_DPDA_CAR_ON) {
		    ret_dl = D_DPDA_ANT_ENA;
        }
    }
    if(ret_ul == D_DPDA_ANT_ENA && ret_dl == D_DPDA_ANT_ENA) {
        ret = D_DPDA_ANT_ENA;
    }

	return ret;
}

/*!
 * @brief 関数機能概要:全ANT キャリアスリープ状態の判定
 * @note  関数処理内容:全ANTのキャリアスリープ状態を判定する
 * @param	N/A
 * @return  1つでもキャリアスリープOFFのアンテナがある場合1を返却
 * @warning		N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No
 * @date 2020/08/05 KCN)Takagi
 
 */
INT f_dpda_com_AllAntCarState()
{
	UINT					uiRtc	 = D_SYS_OFF;
	UINT					uiAntCnt = 0;
	UINT					uiCarCnt = 0;
	T_DPDA_CAR_INFO*		carInfo  = 0;
	
    for(uiAntCnt = 0; uiAntCnt < D_DPDA_ANTMAX; uiAntCnt++) {
	
		/* PA ON/OFF計算結果判定 */
		if( D_SYS_ON != f_dpdaw_antCarStateInfo.ant_info[ uiAntCnt ].PaOnOff )
		{
			/* PA OFFの場合 */
			continue;
		}
		else
		{
			/* PA ONの場合は全キャリアチェック	*/
			/* キャリア状態確認	*/
//			for( uiCarCnt = 0; uiCarCnt < D_DPDA_CARMAX; uiCarCnt++)
			for( uiCarCnt = 0; uiCarCnt < f_dpdaw_carrier_max_num; uiCarCnt++)
			{
				/* 内部テーブルポインタ取得					*/
				carInfo = &(f_dpdaw_antCarStateInfo.ant_info[ uiAntCnt ].car_info[ D_SYS_ANT_DL ][ uiCarCnt ]);
				/* TXキャリアONかつTXキャリアスリープOFFの場合 */
				if(( D_DPDA_CAR_OFF != carInfo->CarOnOff ) &&
				   ( carInfo->CarSleep == E_UP_CAR_ACTIVE_ACTIVE)) 
				{
					/* 一つでもONがあれば抜ける */
					uiRtc = D_SYS_ON;
					return uiRtc;
				}
			}
		}
	}
	return uiRtc;
}

/*!
 * @brief 関数機能概要:QECを行う
 * @note  関数処理内容:QECを行う
 * @param	N/A
 * @return  VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-002-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No
 * @date 2019/04/01 FJT)Taniguchi
 
 */
VOID f_dpda_peri_setQEC(VOID)
{
	USHORT		qec_count;
	UINT		count;
    INT         result; 
	T_DPDA_ANT_INFO		*antInfo;
	T_DPDA_CAR_INFO		*carInfo;
	UINT				uiAntCnt;
 	UINT				uiCarCnt;
	UINT	regval;

	/* デバッグモードの場合、カウンタをクリアしQEC/CALをスキップする */
	if(D_SYS_ON == f_dpdaw_info_all->taskInfo.flg.dbgmode) {
		f_dpda_com_clearQEC();
		return;
	}

	qec_count = f_dpdaw_info_all->taskInfo.flg.qec_count;

	/* ONのANTがあるかつハード自律PA offでない場合かつ1つでもキャリアスリープOFFのアンテナがある場合のみQEC、CALのルートに入る */
	if(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].ApdStopWaveState == D_SYS_OFF ||
		f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].ApdStopWaveState == D_SYS_OFF ||
		f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].ApdStopWaveState == D_SYS_OFF ||
		f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].ApdStopWaveState == D_SYS_OFF ||
        !(f_dpdaw_regdataReal.regVal[ f_dpdaw_index_A200_0720 ] & 0x1) ||
        (f_dpda_com_AllAntCarState() == D_SYS_ON) ) {

        /* QEC */
    	if(qec_count == 10) {
    		for(count = 0 ; count < (sizeof(f_dpdar_qec_reg_1st)/sizeof(f_dpdar_qec_reg_1st[0])) ;count++){
    		    if(f_dpdar_qec_reg_1st[count].regAddr == 0){
    			    f_com_taskDelay(f_dpdar_qec_reg_1st[count].regData,0,0,0);
    				    continue;
    			}
    			f_dpda_IO_write(f_dpdar_qec_reg_1st[count].regAddr,
    			    			f_dpdar_qec_reg_1st[count].regData,
    							E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_WRITE_POL);
    		}
        }
        else if(qec_count == 15) {
    	    for(count = 0 ; count < (sizeof(f_dpdar_qec_reg_2nd)/sizeof(f_dpdar_qec_reg_2nd[0])) ;count++){
    		    if(f_dpdar_qec_reg_2nd[count].regAddr == 0){
    			    f_com_taskDelay(f_dpdar_qec_reg_2nd[count].regData,0,0,0);
    				    continue;
    			}
    			f_dpda_IO_write(f_dpdar_qec_reg_2nd[count].regAddr,
    							f_dpdar_qec_reg_2nd[count].regData,
    							E_DPDA_BPF_NOMAL_MODE,D_RRH_LOG_REG_LV_WRITE_POL);
    		}
    	}
    	else if(f_dpdaw_info_all->taskInfo.flg.sfpload_cmp != D_DPDA_1PPS_CMP){
			;	/* do nothing */
		}
        /* 	BFの場合、運用中CAL */
        else if(f_comw_bf_mode->flag == D_RRH_OFF) {
            if(f_dpda_com_AntState(D_RRH_ANT_A) == D_DPDA_ANT_ENA &&
                f_dpda_com_AntState(D_RRH_ANT_C) == D_DPDA_ANT_ENA &&  
                f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].ApdStopWaveState == D_SYS_OFF && 
    		    f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].ApdStopWaveState == D_SYS_OFF &&
    		    (f_dpda_peri_checkPaCar(D_RRH_ANT_A) == D_SYS_ON && f_dpda_peri_checkPaCar(D_RRH_ANT_C) == D_SYS_ON)) {
    			if(f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_AC]  == D_RRH_ON) {
                    f_comw_rf_setup_flg = D_RRH_OFF;	/* 1秒以上かかりそうなので1sec割り込み抑止 */
    				f_dpda_txfastcal_operation(D_DPDA_ANT_AC);
                    if(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].txopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON && f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].txopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] ==  D_RRH_ON) {
        				f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_AC]  = D_RRH_OFF;

                        regval = 0;
                        BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1202000, &regval);
                        f_com_taskDelay(100,0,0,0);
                        for( uiAntCnt = 0; uiAntCnt < f_dpdaw_ant_max_num; uiAntCnt++ ){
                            antInfo = &(f_dpdaw_antCarStateInfo.ant_info[ uiAntCnt ]);
                            for( uiCarCnt=0; uiCarCnt < f_dpdaw_carrier_max_num; uiCarCnt++ ){
	                    		carInfo = &(antInfo->car_info[ D_SYS_ANT_DL ][ uiCarCnt ]);
	                    		if( D_DPDA_CAR_ON == carInfo->CarOnOff ){
                                    result = f_dpda_com_setDpdStart(uiAntCnt, D_RRH_OFF);
                                    if(result != D_DPDA_RTN_COMPLETE) {
                                        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStart error=0x%08x", result);
                                    }
                                }
                            }
                        }
                        f_com_taskDelay(1000,0,0,0);
                    }
                    f_comw_rf_setup_flg = D_RRH_ON;	/* 1秒以上かかりそうなので1sec割り込み抑止 終了*/
    			}
    			if(f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_AC] == D_RRH_ON) {
                    f_comw_rf_setup_flg = D_RRH_OFF;	/* 1秒以上かかりそうなので1sec割り込み抑止 */
    				f_dpda_rxfastcal_operation(D_DPDA_ANT_AC);
                    f_comw_rf_setup_flg = D_RRH_ON;	/* 1秒以上かかりそうなので1sec割り込み抑止 終了*/
                    if(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].rxopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON && f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].rxopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] ==  D_RRH_ON) {
    			    	f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_OFF;
                    }
    			}
    		}
    		if(f_dpda_com_AntState(D_RRH_ANT_B) == D_DPDA_ANT_ENA &&
                f_dpda_com_AntState(D_RRH_ANT_D) == D_DPDA_ANT_ENA &&  
                f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].ApdStopWaveState == D_SYS_OFF && 
    		    f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].ApdStopWaveState == D_SYS_OFF &&
    		    (f_dpda_peri_checkPaCar(D_RRH_ANT_B) == D_SYS_ON && f_dpda_peri_checkPaCar(D_RRH_ANT_D) == D_SYS_ON)) {
    			if(f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_BD] == D_RRH_ON) {
                    f_comw_rf_setup_flg = D_RRH_OFF;	/* 1秒以上かかりそうなので1sec割り込み抑止 */
    				f_dpda_txfastcal_operation(D_DPDA_ANT_BD);
                    if(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].txopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON && f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].txopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON) {
    				    f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_OFF;

                        regval = 0;
                        BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xB1202000, &regval);
                        f_com_taskDelay(100,0,0,0);                        
                        for( uiAntCnt = 0; uiAntCnt < f_dpdaw_ant_max_num; uiAntCnt++ ){
                            antInfo = &(f_dpdaw_antCarStateInfo.ant_info[ uiAntCnt ]);
                            for( uiCarCnt=0; uiCarCnt < f_dpdaw_carrier_max_num; uiCarCnt++ ){
	                    		carInfo = &(antInfo->car_info[ D_SYS_ANT_DL ][ uiCarCnt ]);
	                    		if( D_DPDA_CAR_ON == carInfo->CarOnOff ){
                                    result = f_dpda_com_setDpdStart(uiAntCnt, D_RRH_OFF);
                                    if(result != D_DPDA_RTN_COMPLETE) {
                                        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStart error=0x%08x", result);
                                    }
                                }
                            }
                        }
                        f_com_taskDelay(1000,0,0,0);
                    }
                    f_comw_rf_setup_flg = D_RRH_ON;	/* 1秒以上かかりそうなので1sec割り込み抑止 終了*/
    			}
    			if(f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_BD] == D_RRH_ON) {
                    f_comw_rf_setup_flg = D_RRH_OFF;	/* 1秒以上かかりそうなので1sec割り込み抑止 */
    				f_dpda_rxfastcal_operation(D_DPDA_ANT_BD);
                    f_comw_rf_setup_flg = D_RRH_ON;	/* 1秒以上かかりそうなので1sec割り込み抑止 終了*/
                    if(f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].rxopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] == D_RRH_ON && f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].rxopecal_info.cal_ant_flg[D_DPDA_FREQ_FAST] ==  D_RRH_ON) {
        				f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_OFF;
                    }
    			}
    		}
    		if(qec_count == 30){
    		    if(f_dpda_com_AntState(D_RRH_ANT_A) == D_DPDA_ANT_ENA &&
                    f_dpda_com_AntState(D_RRH_ANT_C) == D_DPDA_ANT_ENA && 
                    f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_A].ApdStopWaveState == D_SYS_OFF && 
                    f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_C].ApdStopWaveState == D_SYS_OFF &&
	    		    (f_dpda_peri_checkPaCar(D_RRH_ANT_A) == D_SYS_ON && f_dpda_peri_checkPaCar(D_RRH_ANT_C) == D_SYS_ON)) {
                    f_comw_rf_setup_flg = D_RRH_OFF;	/* 1秒以上かかりそうなので1sec割り込み抑止 */
    				result = f_dpda_txcal_operation(D_DPDA_ANT_AC);
                    /* TX CALが成功した場合のみRX CALを実施 */
                    if(result == D_SYS_OK) {
    			        result = f_dpda_rxcal_operation(D_DPDA_ANT_AC);
    					if(result == D_SYS_OK) {
    						qec_count += 14;
    					}
                    }
                    f_comw_rf_setup_flg = D_RRH_ON;	/* 1秒以上かかりそうなので1sec割り込み抑止 終了*/
    			}
    			else {
    			    f_dpdaw_info_all->taskInfo.flg.cal_ac_skip_count++; 
    			}
            }
            else if(qec_count == 45) {
    		    if(f_dpda_com_AntState(D_RRH_ANT_B) == D_DPDA_ANT_ENA &&
                    f_dpda_com_AntState(D_RRH_ANT_D) == D_DPDA_ANT_ENA && 
                    f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_B].ApdStopWaveState == D_SYS_OFF && 
                    f_dpdaw_antCarStateInfo.ant_info[D_RRH_ANT_D].ApdStopWaveState == D_SYS_OFF &&
	    		    (f_dpda_peri_checkPaCar(D_RRH_ANT_B) == D_SYS_ON && f_dpda_peri_checkPaCar(D_RRH_ANT_D) == D_SYS_ON)) {
                    f_comw_rf_setup_flg = D_RRH_OFF;	/* 1秒以上かかりそうなので1sec割り込み抑止 */
    				result = f_dpda_txcal_operation(D_DPDA_ANT_BD);
                    /* TX CALが成功した場合のみRX CALを実施 */
                    if(result == D_SYS_OK) {
    				    result = f_dpda_rxcal_operation(D_DPDA_ANT_BD);
    					if(result == D_SYS_OK) {
    						qec_count += 14;
    					}
                    }
                    f_comw_rf_setup_flg = D_RRH_ON;	/* 1秒以上かかりそうなので1sec割り込み抑止 終了*/
    			}
    			else {
    			    f_dpdaw_info_all->taskInfo.flg.cal_bd_skip_count++; 
    			}
            }
    		else if(f_dpdaw_info_all->taskInfo.flg.cal_ac_skip_count >= 60) {
    		    f_dpdaw_info_all->taskInfo.flg.cal_ac_skip_count = 0; 
    			f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_ON;
    			f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_ON;
    		}
    		else if(f_dpdaw_info_all->taskInfo.flg.cal_bd_skip_count >= 60) {
    		    f_dpdaw_info_all->taskInfo.flg.cal_bd_skip_count = 0; 
    			f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_ON;
    			f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_ON;
    		}
    	}
		/* non BFの場合、Tx停波解除 */
		else {
			if(f_dpdaw_info_all->taskInfo.flg.txBypassStopWaveFlag == D_RRH_ON) {
				for(count=0; count < D_RRH_ANT_MAX ; count++){
					f_dpda_car_TxantcarStopOff(count);
				}
				f_dpdaw_info_all->taskInfo.flg.txBypassStopWaveFlag = D_RRH_OFF;
			}
		}

        qec_count++;
        if(qec_count > 60) {
            qec_count=0;
        }
    }
    else {
        qec_count = 0;
    }

	f_dpdaw_info_all->taskInfo.flg.qec_count = qec_count;
	return;
}

/*!
 * @brief 関数機能概要:QECカウンタのクリアを行う
 * @note  関数処理内容:QECカウンタのクリアを行う
 * @param	N/A
 * @return  VOID
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-002-004
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No
 * @date 2019/04/01 FJT)Taniguchi
 
 */
VOID f_dpda_com_clearQEC(VOID)
{
	f_dpdaw_info_all->taskInfo.flg.qec_count = 0;
	f_dpdaw_info_all->taskInfo.flg.cal_ac_skip_count = 0;
	f_dpdaw_info_all->taskInfo.flg.cal_bd_skip_count = 0;
	return;
}