
#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:TX-SLEEP(M-Plane)設定
 * @note  関数処理内容.
 * @param  *msgP        [in]void            受信メッセージ
 * @return int
 * @date 2020/01/30 FJT)長谷川哲 Create
 * @date 2020/07/31 KCN)福島 modify for 20A SRU
 */
int f_dpda_act_txsleep_request(VOID* msgP)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_txsleep_request called");

    T_RFP_MPL_TX_SLEEP_SETTING_REQ*	msgOFF = NULL;
    unsigned int uiAntNum;
    unsigned int uiCarrier;
    unsigned int uiActive;
    unsigned int regVal;
    unsigned int antCnt;
    int ret = D_DPDA_RTN_COMPLETE;
    int rtn = D_DPDA_RTN_COMPLETE;
    UINT	antGrp[D_DPDA_ANTMAX] = {
				D_DPDA_ANT_AC, D_DPDA_ANT_BD, D_DPDA_ANT_AC, D_DPDA_ANT_BD };

    msgOFF       = (T_RFP_MPL_TX_SLEEP_SETTING_REQ*)msgP;
    uiAntNum     = msgOFF->body.tx_array_car_t.ant ;
    uiCarrier    = msgOFF->body.tx_array_car_t.car ;
    uiActive     = msgOFF->body.tx_array_car_t.active ;

    switch (uiActive) {
    case E_UP_CAR_ACTIVE_SLEEP:
        ret = f_dpda_act_txCarrierSleepSettingRequest(uiAntNum, uiCarrier);
        /* グローバル変数に格納 */
        f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[D_SYS_ANT_DL][uiCarrier].CarSleep = uiActive;

		/* sub6のみ実施 */
		if( f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6 ){
		    /*	該当ANTを他要因で停波していない場合	*/
			if(f_dpdaw_antCarStateInfo.ant_info[uiAntNum].ApdStopWaveState == D_SYS_OFF)
		    {	/* 条件B契機 */
				
				/*	(1)	DPD停止設定を行う	*/
				f_dpda_com_setDpdStop( uiAntNum );
				
				/*	(2)	CW送信設定レジスタに0振幅CW実施値を設定する	*/
				regVal = 0x0;
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,M_DU_REG_CC_ACWAMPI(uiAntNum),&regVal);
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,M_DU_REG_CC_ACWAMPQ(uiAntNum),&regVal);
				regVal = 0x3;
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,M_DU_REG_CC_ACWEN(uiAntNum),&regVal);
				
				/* BFモードチェック (BFモード時のみ処理実行)	*/
				if( f_cmw_tra_inv_tbl->cmx_bfmode != D_RRH_OFF ) {
					/*	(3)	TX停波処理」「RX停波処理」を該当偏波に属するANT(AC or BD)に対して実施する*/
					/* TX停波処理、RX停波処理は同時に実行 */
					for( antCnt = 0; antCnt < f_dpdar_polarizationInf[antGrp[uiAntNum]].antNum; antCnt++ ){
						/* TX停波処理 ANT(AC or BD) */
						rtn = f_dpda_car_TxantcarStop(f_dpdar_polarizationInf[antGrp[uiAntNum]].antNo[antCnt]);
						if(rtn != D_DPDA_RTN_COMPLETE) {
							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TxantcarStop error=0x%08x", rtn);
						}
						/* RX停波処理 ANT(AC or BD) */
						rtn = f_dpda_car_RxantcarStop(f_dpdar_polarizationInf[antGrp[uiAntNum]].antNo[antCnt]);
						if(rtn != D_DPDA_RTN_COMPLETE) {
							BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RxantcarStop error=0x%08x", rtn);
						}
			 		}
					/*	(4)	TXCAL高速モードフラグONに設定 ANT(AC or BD) */
					f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[antGrp[uiAntNum]] = D_RRH_ON;
					/*	(4)	RXCAL高速モードフラグONに設定 ANT(AC or BD)	*/
					f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[antGrp[uiAntNum]] = D_RRH_ON;
//					/* 	(4)	Txcal係数計算完了フラグ#A#C#E#G or #B#D#F#HをOFFに設定  */    
//					for(antCnt = 0; antCnt < f_dpdar_polarizationInf[antGrp[uiAntNum]].antNum; antCnt++){
//						f_dpdaw_info_all->taskInfo.flg.txcal_ant_fin_flg[f_dpdar_polarizationInf[antGrp[uiAntNum]].antNo[antCnt]] = D_RRH_OFF;
//					}
				}
			}
		}
        break;
    case E_UP_CAR_ACTIVE_ACTIVE:
        ret = f_dpda_act_txCarrierSleepOffSettingRequest(uiAntNum, uiCarrier);
        /* グローバル変数に格納 */
        f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[D_SYS_ANT_DL][uiCarrier].CarSleep = uiActive;

		/* sub6のみ実施 */
		if( f_dpdaw_reKind == E_DPDA_REKIND_5GDU_SUB6 ){
		    /*	該当ANTを他要因で停波していない場合	*/
		    if(f_dpdaw_antCarStateInfo.ant_info[uiAntNum].ApdStopWaveState == D_SYS_OFF)
			{	/* 条件A契機 */
				
				/*	(1)	DPD停止設定を行う	*/
				f_dpda_com_setDpdStop( uiAntNum );
				
				/*	(2)	CW送信設定レジスタに0振幅CW解除値を設定する	*/
				regVal = 0x0;
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,M_DU_REG_CC_ACWEN(uiAntNum),&regVal);
				
				/*	(3)	DPD_ON設定を行う	*/
				f_dpda_com_setDpdStart( uiAntNum , D_RRH_OFF);
			}
		}
        break;
    default:
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_txsleep_request state error=0x%08x", uiActive);
        ret = D_DPDA_RTN_NG;
        break;
    }

    if(ret != D_DPDA_RTN_COMPLETE) {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_txsleep_request  error=0x%08x", ret);
    }

    return ret;
}

