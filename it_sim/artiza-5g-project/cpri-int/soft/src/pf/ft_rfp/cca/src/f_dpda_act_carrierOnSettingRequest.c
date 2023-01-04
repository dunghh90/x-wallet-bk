
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"

/*!
 * @brief 関数機能概要:TXキャリア ON設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiAccellId   [in]unsigned int    ACELL_ID
 * @param  uiCcId       [in]unsigned int    CC_ID
 * @param  uiDuPortId   [in]unsigned int    PORT_ID
 * @param  uiCrCentFreq [in]unsigned int    frequency
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @param  uiScs        [in]unsigned int    SCS(0,2:未使用 1:30kHz 3:120kHz 4:240kHz)
 * @return int
 * @date 2018/09/11 KCN)吉木 Create
 */
int f_dpda_act_txCarrierOnSettingRequest(unsigned int uiAntNum,
                                         unsigned int uiCarrier,
                                         unsigned int uiAccellId,
                                         unsigned int uiCcId,
                                         unsigned int uiDuPortId,
                                         unsigned int uiCrCentFreq,
                                         unsigned int uiUnitType,
                                         unsigned int uiBw,
                                         unsigned int uiScs,
                                         long long int Gain
                                         ) 
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_txCarrierOnSettingRequest called, uiAntNum = %d, uiCarrier = %d, uiAccellId = %08x, uiCcId = %08x, uiDuPortId = %08x", uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId);
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_txCarrierOnSettingRequest called, uiCrCentFreq = %d, uiUnitType = %d, uiBw=%d, uiScs=%d", uiCrCentFreq, uiUnitType,uiBw,uiScs); 

    int ret = D_DPDA_RTN_COMPLETE;
    unsigned int uiCarOpp;
    unsigned int uiNumberOfCar;
    unsigned int uiScsn = uiScs; 


	/* SFN/FRM補正有無チェック  */
	if( f_comw_sfnfrm_tbl->sfnfrm_flag == D_RRH_ON ) {
		(void)clock_gettime( CLOCK_MONOTONIC_COARSE   , &f_dpdaw_sfnload_settime);	/* 時刻取得(bootup時刻) */

		if(f_comw_sfnfrm_tbl->ns_flag != D_DU_SFNRFN_CORRECTION_CALC_WAIT){

			BPF_COM_PLOG_TRACE_PTPLOG(E_TRC_LV_INFO,"Starting 1PPS SFN load after 2sec(cur:%d) by TxCarrier ON(CC:%d,ANT:%d) ...",f_comw_sfnfrm_tbl->ns_flag,uiCarrier,uiAntNum);
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "1PPS IRQ SFN load set(set:%ld qc:%d)",f_dpdaw_sfnload_settime.tv_sec,f_dpdaw_info_all->taskInfo.flg.qec_count);
		}
		/* 補正あり -> ns補正フラグ設定  */
		f_comw_sfnfrm_tbl->ns_flag = D_DU_SFNRFN_CORRECTION_CALC_WAIT;
		f_dpdaw_info_all->taskInfo.flg.sfpload_cmp = D_DPDA_1PPS_SET;

	}
    else {
        f_dpdaw_info_all->taskInfo.flg.sfpload_cmp = D_DPDA_1PPS_CMP;
    }

    //パラメータの設定
    ret = f_dpda_car_setTxParameter(uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId, uiUnitType, uiBw);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxParameter error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }
    
    //送信帯域、周波数等の設定
    ret = f_dpda_car_setTxBwandNco(uiAntNum, uiCarrier, uiCrCentFreq, uiBw, uiUnitType);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxBwandNco error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //位相補償テーブル設定
        ret = f_dpda_car_setTxPhaseCompensationTable(uiCarrier, uiCrCentFreq, uiScsn, uiUnitType);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxPhaseCompensationTable error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }
    else
    {
        /* ハソ変更(DCM5GDU-JIG-0036-181220) uiScsnに関わらず120kHz/240kHzどちらも設定する */
        uiScsn = 3;
        //位相補償テーブル設定 (uiScsn = 3 : 120kHz)
        ret = f_dpda_car_setTxPhaseCompensationTable(uiCarrier, uiCrCentFreq, uiScsn, uiUnitType);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxPhaseCompensationTable error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
        
        uiScsn = 4;
        //位相補償テーブル設定 (uiScsn = 4 : 240kHz)
        ret = f_dpda_car_setTxPhaseCompensationTable(uiCarrier, uiCrCentFreq, uiScsn, uiUnitType);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxPhaseCompensationTable error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }

	// LPF設定
	ret = f_dpda_car_setTxLPFGain(uiAntNum, uiCarrier, uiUnitType , Gain);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxLPFGain error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }
	

    //TDD設定
    ret = f_dpda_com_setTddConfig(uiUnitType);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTddConfig error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //DPD停止設定
        f_dpda_com_setDpdStop(uiAntNum);

        //CAL位相補正
        ret = f_dpda_car_setTxCalPhaseCorrection(uiCarrier,uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxCalPhaseCorrection error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        /* Sub6かつnon BFモードの場合、TXバイパス停波フラグを立てる */
		if(f_comw_bf_mode->flag == D_RRH_ON) {
            f_dpdaw_info_all->taskInfo.flg.txBypassStopWaveFlag = D_RRH_ON;
        }

        //TX停波処理
        ret = f_dpda_car_TxantcarStop(uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TxantcarStop error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        //LUT設定
        ret = f_dpda_car_lutUpdate(uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "lutUpdate error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        /********************************************************************************************/
        /*  TXキャリアONするANTのキャリアON/OFF状態を確認しインパルス係数設定の引数を決める         */
        /********************************************************************************************/
        uiCarOpp = 1 - uiCarrier;
        uiNumberOfCar = f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarOpp].CarOnOff + 1;

        //インパルス係数設定
        ret = f_dpda_com_setImpulseCoefficient(uiAntNum, uiNumberOfCar, uiCrCentFreq);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setImpulseCoefficient error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }

    //TXキャリアイネーブル設定
    ret = f_dpda_car_TxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 1);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TxcarEnDis error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    //TDD ENABLE設定
    ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 1);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TddAntEnDis error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //DPD ON設定
        ret = f_dpda_com_setDpdStart(uiAntNum, D_RRH_OFF);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStart error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }

    /* Sub6かつBF有りの場合、Ant.CまたはAnt.DのTxON後にTxCal高速モードフラグをONに設定する */
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6 && f_comw_bf_mode->flag == D_RRH_OFF) {
        if(uiAntNum == D_RRH_ANT_C) {
            f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_ON;
        }
        else if(uiAntNum == D_RRH_ANT_D) {
             f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_ON;
        }
    }

    /********************************************************************************************/
    /*  グローバル変数に設定したパラメータを保持                                                */
    /********************************************************************************************/
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].AcellId = uiAccellId;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].CcId = uiCcId;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].PortId = uiDuPortId;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].ChannelNum = uiCrCentFreq;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].BandWidth = (unsigned short)uiBw;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].Gain = (DL)Gain;

	/* MT設定 */
	f_com_mtCtl_carInfo(uiAntNum, uiCarrier, D_RRH_ON, uiCrCentFreq , 0x100000, D_RRH_ON);

	f_dpda_com_clearQEC();

    return D_DPDA_RTN_COMPLETE;
}


/*!
 * @brief 関数機能概要:RXキャリア ON設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiAccellId   [in]unsigned int    ACELL_ID
 * @param  uiCcId       [in]unsigned int    CC_ID
 * @param  uiDuPortId   [in]unsigned int    PORT_ID
 * @param  uiCrCentFreq [in]unsigned int    frequency
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @param  uiScs        [in]unsigned int    SCS(0,2:未使用 1:30kHz 3:120kHz 4:240kHz)
 * @return int
 * @date 2018/09/11 KCN)吉木 Create
 */
int f_dpda_act_rxCarrierOnSettingRequest(unsigned int uiAntNum,
                                         unsigned int uiCarrier,
                                         unsigned int uiAccellId,
                                         unsigned int uiCcId,
                                         unsigned int uiDuPortId,
                                         unsigned int uiCrCentFreq,
                                         unsigned int uiUnitType,
                                         unsigned int uiBw,
                                         unsigned int uiScs) 
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxCarrierOnSettingRequest called, uiAntNum = %d, uiCarrier = %d, uiAccellId = %08x, uiCcId = %08x, uiDuPortId = %08x", uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId);
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxCarrierOnSettingRequest called, uiCrCentFreq = %d, uiUnitType = %d, uiBw=%d, uiScs=%d", uiCrCentFreq, uiUnitType,uiBw,uiScs); 

    int ret = D_DPDA_RTN_COMPLETE;
    unsigned int uiScsn = uiScs; 

    //パラメータの設定
    ret = f_dpda_car_setRxParameter(uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId, uiUnitType, uiBw);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setRxParameter error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    //受信帯域、周波数等の設定
    ret = f_dpda_car_setRxBwandNco(uiAntNum, uiCarrier, uiCrCentFreq, uiBw, uiUnitType);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setRxBwandNco error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //CAL位相補正
        ret = f_dpda_car_setRxCalPhaseCorrection(uiCarrier,uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setRxCalPhaseCorrection error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        /* Sub6かつBFモードの場合、停波処理を実施する */
		if(f_comw_bf_mode->flag == D_RRH_OFF) {
            ret = f_dpda_car_RxantcarStop(uiAntNum);
        }

    }

    //位相補償テーブル設定
    ret = f_dpda_car_setRxPhaseCompensationTable(uiCarrier, uiCrCentFreq, uiScsn, uiUnitType);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setRxPhaseCompensationTable error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    //TDD設定
    ret = f_dpda_com_setTddConfig(uiUnitType);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTddConfig error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    //RXキャリアイネーブル設定
    ret = f_dpda_car_RxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 1);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RxcarEnDis error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    //TDD ENABLE設定
    ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 1);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TddAntEnDis error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    /* Sub6かつBF有りの場合、Ant.CまたはAnt.DのRxON後にRxCal高速モードフラグをONに設定する */
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6 && f_comw_bf_mode->flag == D_RRH_OFF) {
        if(uiAntNum == D_RRH_ANT_C) {
            f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_ON;
        }
        else if(uiAntNum == D_RRH_ANT_D) {
            f_dpdaw_info_all->taskInfo.flg.rxcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_ON;
        }
    }

    /********************************************************************************************/
    /*  グローバル変数に設定したパラメータを保持                                                */
    /********************************************************************************************/
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].AcellId = uiAccellId;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].CcId = uiCcId;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].PortId = uiDuPortId;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].ChannelNum = uiCrCentFreq;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].BandWidth = (unsigned short)uiBw;

	/* MT設定 */
	f_com_mtCtl_carInfo(uiAntNum, uiCarrier, D_RRH_ON, uiCrCentFreq , 0x100000, D_RRH_OFF);

	f_dpda_com_clearQEC();

    return D_DPDA_RTN_COMPLETE;
}


/*!
 * @brief 関数機能概要:TEST TXキャリア ON設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiAccellId   [in]unsigned int    ACELL_ID
 * @param  uiCcId       [in]unsigned int    CC_ID
 * @param  uiDuPortId   [in]unsigned int    PORT_ID
 * @param  uiCrCentFreq [in]unsigned int    frequency
 * @param  uiBw         [in]unsigned int    Bandwidth
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @param  uiScs        [in]unsigned int    SCS(0~2:未使用 3:120kHz 4:240kHz)
 * @return int
 * @date 2018/09/11 KCN)南條 Create
 */
int f_dpda_act_testtxCarrierOnSettingRequest(unsigned int uiAntNum,
                                             unsigned int uiCarrier,
                                             unsigned int uiAccellId,
                                             unsigned int uiCcId,
                                             unsigned int uiDuPortId,
                                             unsigned int uiCrCentFreq,
                                             unsigned int uiUnitType,
                                             unsigned int uiBw,
                                             unsigned int uiScs) 
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_testtxCarrierOnSettingRequest called, uiAntNum = %d, uiCarrier = %d, uiAccellId = %08x, uiCcId = %08x, uiDuPortId = %08x", uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId);
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_testtxCarrierOnSettingRequest called, uiCrCentFreq = %d, uiUnitType = %d, uiBw=%d uiScs=%d", uiCrCentFreq, uiUnitType,uiBw,uiScs); 

    int ret = D_DPDA_RTN_COMPLETE;
    unsigned int uiCarOpp;
    unsigned int uiNumberOfCar;
    unsigned int uiScsn     = uiScs; 

    //パラメータの設定
   ret = f_dpda_car_setTxParameter(uiAntNum, uiCarrier, uiAccellId, uiCcId, uiDuPortId, uiUnitType, uiBw);
   if(ret != D_DPDA_RTN_COMPLETE)
   {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxParameter error=0x%08x", ret);
       return D_DPDA_RTN_NG;
    }

    //送信帯域、周波数等の設定
    ret = f_dpda_car_setTxBwandNco(uiAntNum, uiCarrier, uiCrCentFreq, uiBw, uiUnitType);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxBwandNco error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //位相補償テーブル設定
        ret = f_dpda_car_setTxPhaseCompensationTable(uiCarrier, uiCrCentFreq, uiScsn, uiUnitType);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxPhaseCompensationTable error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }
    else
    {
        /* ハソ変更(DCM5GDU-JIG-0036-181220) uiScsnに関わらず120kHz/240kHzどちらも設定する */
        uiScsn = 3;
        //位相補償テーブル設定 (uiScsn = 3 : 120kHz)
        ret = f_dpda_car_setTxPhaseCompensationTable(uiCarrier, uiCrCentFreq, uiScsn, uiUnitType);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxPhaseCompensationTable error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
        
        uiScsn = 4;
        //位相補償テーブル設定 (uiScsn = 4 : 240kHz)
        ret = f_dpda_car_setTxPhaseCompensationTable(uiCarrier, uiCrCentFreq, uiScsn, uiUnitType);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxPhaseCompensationTable error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }

	// LPF設定
	ret = f_dpda_car_setTxLPFGain(uiAntNum, uiCarrier, uiUnitType , 0xFFFFFFFF);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxLPFGain error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    //TDD設定
    ret = f_dpda_com_setTddConfig(uiUnitType);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTddConfig error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //DPD停止設定
        f_dpda_com_setDpdStop(uiAntNum);

        //CAL位相補正
        ret = f_dpda_car_setTxCalPhaseCorrection(uiCarrier,uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setTxCalPhaseCorrection error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        //LUT設定
        ret = f_dpda_car_lutUpdate(uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "lutUpdate error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        /********************************************************************************************/
        /*  TXキャリアONするANTのキャリアON/OFF状態を確認しインパルス係数設定の引数を決める         */
        /********************************************************************************************/
        uiCarOpp = 1 - uiCarrier;
        uiNumberOfCar = f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarOpp].CarOnOff + 1;

        //インパルス係数設定
        ret = f_dpda_com_setImpulseCoefficient(uiAntNum, uiNumberOfCar, uiCrCentFreq);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setImpulseCoefficient error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }

    //TXキャリアイネーブル設定
    ret = f_dpda_car_TxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 1);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TxcarEnDis error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    //TDD ENABLE設定
    ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 1);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TddAntEnDis error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    //testtxon状態保持
    //sub6
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //DPD ON設定
        ret = f_dpda_com_setDpdStart(uiAntNum, D_RRH_OFF);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStart error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        f_dpdaw_teston_flag = f_dpdaw_teston_flag | (0x01 << (uiAntNum*2 + uiCarrier));
    }
    //mmw
    else
    {
        f_dpdaw_teston_flag = f_dpdaw_teston_flag | (0x01 << (uiAntNum*8 + uiCarrier + 16));
    }

    /* Sub6かつBF有りの場合、Ant.CまたはAnt.DのRxON後にRxCal高速モードフラグをONに設定する */
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6 && f_comw_bf_mode->flag == D_RRH_OFF) {
        if(uiAntNum == D_RRH_ANT_C) {
            f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_AC] = D_RRH_ON;
        }
        else if(uiAntNum == D_RRH_ANT_D) {
            f_dpdaw_info_all->taskInfo.flg.txcal_ant_fast_mode_flg[D_DPDA_ANT_BD] = D_RRH_ON;
        }
    }
    
    /********************************************************************************************/
    /*  グローバル変数に設定したパラメータを保持                                                */
    /********************************************************************************************/
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].AcellId = uiAccellId;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].CcId = uiCcId;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].PortId = uiDuPortId;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].ChannelNum = uiCrCentFreq;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].BandWidth = (unsigned short)uiBw;

	/* MT設定 */
	f_com_mtCtl_carInfo(uiAntNum, uiCarrier, D_RRH_ON, uiCrCentFreq , 0x100000, D_RRH_ON);

	f_dpda_com_clearQEC();

    return D_DPDA_RTN_COMPLETE;
}


/*!
 * @brief 関数機能概要:キャリア ON設定
 * @note  関数処理内容.
 * @param  *msgP        [in]void            受信メッセージ
 * @return int
 * @date 2018/09/11 KCN)吉木 Create
 */
int f_dpda_act_carrierOnSettingRequest( void* msgP )
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_carrierOnSettingRequest called");

    T_SYS_CARRIER_ON_SETTING_REQ *msgON = NULL;	/* キャリアONメッセージ		*/

    unsigned int uiAntNum;
    unsigned int uiCarrier;
    unsigned int uiAccellId;
    unsigned int uiCcId;
    unsigned int uiDuPortId;
    unsigned int uiCrCentFreq;
    unsigned int uiUnitType;
    unsigned int uiTxRx;
    unsigned int uiTestFlg;
    unsigned int uiBw;
    unsigned int uiScs;  
    int ret = D_DPDA_RTN_COMPLETE;

    msgON        = (T_SYS_CARRIER_ON_SETTING_REQ*)msgP;
    uiAntNum     = msgON->carrierOnSettingReqInfo.uiAntNum;
    uiCarrier    = msgON->carrierOnSettingReqInfo.uiCarrier;
    uiAccellId   = msgON->carrierOnSettingReqInfo.uiAccellId;
    uiCcId       = msgON->carrierOnSettingReqInfo.uiCcId;
    uiDuPortId   = msgON->carrierOnSettingReqInfo.uiDuPortId;
    uiCrCentFreq = msgON->carrierOnSettingReqInfo.uiCrCentFreq;
    uiUnitType   = f_dpdaw_reKind;
    uiTxRx       = msgON->carrierOnSettingReqInfo.uiTxRx;
    uiTestFlg    = msgON->carrierOnSettingReqInfo.uiTestFlg;
    uiBw         = msgON->carrierOnSettingReqInfo.uiBw;
    uiScs        = msgON->carrierOnSettingReqInfo.uiScs;

    if(uiTxRx == 0) // RX ON
    {
        if(uiTestFlg == 0) // 通常のRX ON
        {
            ret = f_dpda_act_rxCarrierOnSettingRequest(uiAntNum,
                                                       uiCarrier,
                                                       uiAccellId,
                                                       uiCcId,
                                                       uiDuPortId,
                                                       uiCrCentFreq,
                                                       uiUnitType,
                                                       uiBw,
                                                       uiScs);
            if(ret != D_DPDA_RTN_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxCarrierOnSettingRequest error=0x%08x", ret);
                return D_DPDA_RTN_NG;
            }
        }
    }
    else // TX ON
    {
        if(uiTestFlg == 0) // 通常のTX ON
        {
             ret = f_dpda_act_txCarrierOnSettingRequest(uiAntNum,
                                                        uiCarrier,
                                                        uiAccellId,
                                                        uiCcId,
                                                        uiDuPortId,
                                                        uiCrCentFreq,
                                                        uiUnitType,
                                                        uiBw,
                                                        uiScs,
                                                        0xFFFFFFFF
                                                        );
            if(ret != D_DPDA_RTN_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_txCarrierOnSettingRequest error=0x%08x", ret);
                return D_DPDA_RTN_NG;
            }
        }
        else // TEST TX ON
        {
            ret = f_dpda_act_testtxCarrierOnSettingRequest(uiAntNum,
                                                           uiCarrier,
                                                           uiAccellId,
                                                           uiCcId,
                                                           uiDuPortId,
                                                           uiCrCentFreq,
                                                           uiUnitType,
                                                           uiBw,
                                                           uiScs);
            if(ret != D_DPDA_RTN_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_testtxCarrierOnSettingRequest error=0x%08x", ret);
                return D_DPDA_RTN_NG;
            }
        }
    }

    return D_DPDA_RTN_COMPLETE;
}
