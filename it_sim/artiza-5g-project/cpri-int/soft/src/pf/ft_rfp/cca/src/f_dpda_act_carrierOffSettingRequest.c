
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"


/*!
 * @brief 関数機能概要:TXキャリア OFF設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiCrCentFreq [in]unsigned int    frequency
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @return int
 * @date 2018/09/11 KCN)吉木 Create
 */
int f_dpda_act_txCarrierOffSettingRequest(unsigned int uiAntNum,
                                          unsigned int uiCarrier,
                                          unsigned int uiCrCentFreq,
                                          unsigned int uiUnitType)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_txCarrierOffSettingRequest called, uiAntNum = %d, uiCarrier = %d, uiCrCentFreq = %d, uiUnitType = %d", uiAntNum, uiCarrier, uiCrCentFreq, uiUnitType);

    int ret = D_DPDA_RTN_COMPLETE;
    unsigned int uiCarmax = 0;
    unsigned int uiNumberOfCar = 0;
    unsigned int uiCrCentFreqOpp = 0;

    /********************************************************************************************/
    /*  TXキャリアONするANTのキャリアON/OFF状態を確認しインパルス係数設定の引数を決める         */
    /********************************************************************************************/
    //sub6
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        uiCarmax = 2;
    }
    //mmw
    else
    {
        uiCarmax = 4;
    }
    //キャリア数ループ
    for(int i = 0; i< uiCarmax;i++)
    {
        //OFFするキャリア以外のON/OFF状態を確認する
        if(i == uiCarrier)
        {
            continue;
        }
        uiNumberOfCar = f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][i].CarOnOff;

        //キャリアON状態があれば、抜ける
        if(uiNumberOfCar != 0)
        {
            break;
        }
    }

    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //DPD停止設定
        ret = f_dpda_com_setDpdStop(uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStop error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        //NCO設定用にON状態で残すキャリアの周波数を取得
        if(uiNumberOfCar == 1)
        {
            uiCrCentFreqOpp = f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][1-uiCarrier].ChannelNum;
        }

        //インパルス係数設定
        ret = f_dpda_com_setImpulseCoefficient(uiAntNum, uiNumberOfCar, uiCrCentFreqOpp);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setImpulseCoefficient error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        //LUT設定
        ret = f_dpda_car_lutUpdate(uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "lutUpdate error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }

    //キャリアディセーブル設定
    ret = f_dpda_car_TxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 0);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TxcarEnDis error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    /* 対象ANTの全キャリアOFF状態 */
    if(uiNumberOfCar == 0)
    {
        //TDD DISABLE設定
        ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 0);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TddAntEnDis error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }
    else
    {
        if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
        {
            //DPD ON設定
            ret = f_dpda_com_setDpdStart(uiAntNum , D_RRH_OFF);
            if(ret != D_DPDA_RTN_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStart error=0x%08x", ret);
                return D_DPDA_RTN_NG;
            }
        }
    }

    /********************************************************************************************/
    /*  グローバル変数に設定したパラメータをクリア                                              */
    /********************************************************************************************/
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].AcellId = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].CcId = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].PortId = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].ChannelNum = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].BandWidth = 0;

	/* MT設定 */
	f_com_mtCtl_carInfo(uiAntNum, uiCarrier, D_RRH_OFF, 0 , 0, D_RRH_ON);

    return D_DPDA_RTN_COMPLETE;
}


/*!
 * @brief 関数機能概要:RXキャリア OFF設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiAccellId   [in]unsigned int    ACELL_ID
 * @param  uiCcId       [in]unsigned int    CC_ID
 * @param  uiDuPortId   [in]unsigned int    PORT_ID
 * @param  uiCrCentFreq [in]unsigned int    frequency
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @return int
 * @date 2018/09/11 KCN)吉木 Create
 */
int f_dpda_act_rxCarrierOffSettingRequest(unsigned int uiAntNum,
                                          unsigned int uiCarrier,
                                          unsigned int uiUnitType)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxCarrierOffSettingRequest called, uiAntNum = %d, uiCarrier = %d, uiUnitType = %d", uiAntNum, uiCarrier, uiUnitType);

    int ret = D_DPDA_RTN_COMPLETE;
    unsigned int uiCarmax = 0;
    unsigned int uiNumberOfCar = 0;

    //キャリアディセーブル設定
    ret = f_dpda_car_RxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 0);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RxcarEnDis error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    /********************************************************************************************/
    /*  RXキャリアONするANTのキャリアON/OFF状態を確認しインパルス係数設定の引数を決める         */
    /********************************************************************************************/
    //sub6
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        uiCarmax = 2;
    }
    //mmw
    else
    {
        uiCarmax = 4;
    }
    //キャリア数ループ
    for(int i = 0; i< uiCarmax;i++)
    {
        //OFFするキャリア以外のON/OFF状態を確認する
        if(i == uiCarrier)
        {
            continue;
        }
        uiNumberOfCar = f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][i].CarOnOff;

        //キャリアON状態があれば、抜ける
        if(uiNumberOfCar != 0)
        {
            break;
        }
    }
    /* 対象ANTの全キャリアOFF状態 */
    if(uiNumberOfCar == 0)
    {
        //TDD DISABLE設定
        ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 0);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RxcarEnDis error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }

    /********************************************************************************************/
    /*  グローバル変数に設定したパラメータをクリア                                              */
    /********************************************************************************************/
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].AcellId = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].CcId = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].PortId = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].ChannelNum = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[0][uiCarrier].BandWidth = 0;

	/* MT設定 */
	f_com_mtCtl_carInfo(uiAntNum, uiCarrier, D_RRH_OFF, 0 , 0, D_RRH_OFF);

    return D_DPDA_RTN_COMPLETE;
}


/*!
 * @brief 関数機能概要:TEST TXキャリア OFF設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @param  uiCrCentFreq [in]unsigned int    frequency
 * @param  uiUnitType   [in]unsigned int    装置種別(E_DPDA_REKIND_5GDU_SUB6, E_DPDA_REKIND_5GDU_MMW)
 * @return int
 * @date 2018/09/11 KCN)南條 Create
 */
int f_dpda_act_testtxCarrierOffSettingRequest(unsigned int uiAntNum,
                                              unsigned int uiCarrier,
                                              unsigned int uiCrCentFreq,
                                              unsigned int uiUnitType)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_testtxCarrierOffSettingRequest called, uiAntNum = %d, uiCarrier = %d, uiCrCentFreq = %d, uiUnitType = %d", uiAntNum, uiCarrier, uiCrCentFreq, uiUnitType);

    int ret = D_DPDA_RTN_COMPLETE;
    unsigned int uiCarmax = 0;
    unsigned int uiNumberOfCar = 0;
    unsigned int write_data = 0x0;
    unsigned int uiCrCentFreqOpp = 0;

    /********************************************************************************************/
    /*  TXキャリアONするANTのキャリアON/OFF状態を確認しインパルス係数設定の引数を決める         */
    /********************************************************************************************/
    //sub6
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        uiCarmax = 2;
    }
    //mmw
    else
    {
        uiCarmax = 4;
    }
    //キャリア数ループ
    for(int i = 0; i< uiCarmax;i++)
    {
        //OFFするキャリア以外のON/OFF状態を確認する
        if(i == uiCarrier)
        {
            continue;
        }
        uiNumberOfCar = f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][i].CarOnOff;

        //キャリアON状態があれば、抜ける
        if(uiNumberOfCar != 0)
        {
            break;
        }
    }

    //sub6
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        //DPD停止設定
        ret = f_dpda_com_setDpdStop(uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStop error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        //NCO設定用にON状態で残すキャリアの周波数を取得
        if(uiNumberOfCar == 1)
        {
            uiCrCentFreqOpp = f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][1-uiCarrier].ChannelNum;
        }

        //インパルス係数設定
        ret = f_dpda_com_setImpulseCoefficient(uiAntNum, uiNumberOfCar, uiCrCentFreqOpp);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setImpulseCoefficient error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }

        //LUT設定
        ret = f_dpda_car_lutUpdate(uiAntNum);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "lutUpdate error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }

    //キャリアディセーブル設定
    ret = f_dpda_car_TxcarEnDis(uiAntNum, uiCarrier, uiUnitType, 0);
    if(ret != D_DPDA_RTN_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TxcarEnDis error=0x%08x", ret);
        return D_DPDA_RTN_NG;
    }

    /* 対象ANTの全キャリアOFF状態 */
    if(uiNumberOfCar == 0)
    {
        //TDD DISABLE設定
        ret = f_dpda_com_TddAntEnDis(uiCarrier, uiAntNum, uiUnitType, 0);
        if(ret != D_DPDA_RTN_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "TddAntEnDis error=0x%08x", ret);
            return D_DPDA_RTN_NG;
        }
    }
    else
    {
        //sub6
        if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
        {
            //DPD ON設定
            ret = f_dpda_com_setDpdStart(uiAntNum , D_RRH_OFF);
            if(ret != D_DPDA_RTN_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "setDpdStart error=0x%08x", ret);
                return D_DPDA_RTN_NG;
            }
        }
    }

    //testtxoff状態保持
    //sub6
    if(uiUnitType == E_DPDA_REKIND_5GDU_SUB6)
    {
        f_dpdaw_teston_flag = f_dpdaw_teston_flag & ~(0x01 << (uiAntNum*2 + uiCarrier));
    }
    //mmw
    else
    {
        f_dpdaw_teston_flag = f_dpdaw_teston_flag & ~(0x01 << (uiAntNum*8 + uiCarrier + 16));
    }
    /* 全ANTの全キャリアTESTTXOFF状態 */
    if(f_dpdaw_teston_flag == 0)
    {
        //TXテストモードOFF設定
        write_data = 0;
        ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2010D00, &write_data);
        ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2010904, &write_data);
        ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2010900, &write_data);

    }

    /********************************************************************************************/
    /*  グローバル変数に設定したパラメータをクリア                                              */
    /********************************************************************************************/
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].AcellId = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].CcId = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].PortId = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].ChannelNum = 0;
    f_dpdaw_antCarStateInfo.ant_info[uiAntNum].car_info[1][uiCarrier].BandWidth = 0;

	/* MT設定 */
	f_com_mtCtl_carInfo(uiAntNum, uiCarrier, D_RRH_OFF, 0 , 0, D_RRH_ON);

    return D_DPDA_RTN_COMPLETE;
}


/*!
 * @brief 関数機能概要:キャリア OFF設定
 * @note  関数処理内容.
 * @param  *msgP        [in]void            受信メッセージ
 * @return int
 * @date 2018/09/11 KCN)吉木 Create
 */
int f_dpda_act_carrierOffSettingRequest(VOID* msgP)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_carrierOffSettingRequest called");

    T_SYS_CARRIER_OFF_SETTING_REQ *msgOFF = NULL;
    unsigned int uiAntNum;
    unsigned int uiCarrier;
    unsigned int uiCrCentFreq;
    unsigned int uiUnitType;
    unsigned int uiTxRx;
    unsigned int uiTestFlg;
    int ret = D_DPDA_RTN_COMPLETE;

    msgOFF       = (T_SYS_CARRIER_OFF_SETTING_REQ*)msgP;
    uiAntNum     = msgOFF->carrierOffSettingReqInfo.uiAntNum;
    uiCarrier    = msgOFF->carrierOffSettingReqInfo.uiCarrier;
    uiCrCentFreq = msgOFF->carrierOffSettingReqInfo.uiCrCentFreq;
    uiUnitType   = f_dpdaw_reKind;
    uiTxRx       = msgOFF->carrierOffSettingReqInfo.uiTxRx;
    uiTestFlg    = msgOFF->carrierOffSettingReqInfo.uiTestFlg;

    if(uiTxRx == 0) // RX OFF
    {
        if(uiTestFlg == 0) // 通常のRX OFF
        {
            ret = f_dpda_act_rxCarrierOffSettingRequest(uiAntNum,
                                                        uiCarrier,
                                                        uiUnitType);
            if(ret != D_DPDA_RTN_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxCarrierOffSettingRequest error=0x%08x", ret);
                return D_DPDA_RTN_NG;
            }
        }
    }
    else // TX OFF
    {
        if(uiTestFlg == 0) // 通常のTX OFF
        {
             ret = f_dpda_act_txCarrierOffSettingRequest(uiAntNum,
                                                         uiCarrier,
                                                         uiCrCentFreq,
                                                         uiUnitType);
            if(ret != D_DPDA_RTN_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_txCarrierOffSettingRequest error=0x%08x", ret);
                return D_DPDA_RTN_NG;
            }
        }
        else // TEST TX OFF
        {
             ret = f_dpda_act_testtxCarrierOffSettingRequest(uiAntNum,
                                                             uiCarrier,
                                                             uiCrCentFreq,
                                                             uiUnitType);
            if(ret != D_DPDA_RTN_COMPLETE)
            {
                BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_testtxCarrierOffSettingRequest error=0x%08x", ret);
                return D_DPDA_RTN_NG;
            }
        }
    }



    return D_DPDA_RTN_COMPLETE;
}
