// ytakagi add
#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:RX-SLEEP(M-Plane)設定
 * @note  関数処理内容.
 * @param  *msgP        [in]void            受信メッセージ
 * @return int
 * @date 2020/01/30 FJT)長谷川哲 Create
 */
int f_dpda_act_rxsleep_request(VOID* msgP)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxsleep_request called");

    T_RFP_MPL_RX_SLEEP_SETTING_REQ*	msgOFF = NULL;
    unsigned int uiAntNum;
    unsigned int uiCarrier;
    unsigned int uiActive;
    int ret = D_DPDA_RTN_COMPLETE;

    msgOFF       = (T_RFP_MPL_RX_SLEEP_SETTING_REQ*)msgP;
    uiAntNum     = msgOFF->body.rx_array_car_t.ant ;
    uiCarrier    = msgOFF->body.rx_array_car_t.car ;
    uiActive     = msgOFF->body.rx_array_car_t.active ;

    switch (uiActive) {
    case E_UP_CAR_ACTIVE_SLEEP:
        ret = f_dpda_act_rxCarrierSleepSettingRequest(uiAntNum, uiCarrier);
        break;
    case E_UP_CAR_ACTIVE_ACTIVE:
        ret = f_dpda_act_rxCarrierSleepOffSettingRequest(uiAntNum, uiCarrier);
        break;
    default:
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxsleep_request state error=0x%08x", uiActive);
        ret = D_DPDA_RTN_NG;
        break;
    }

    if(ret != D_DPDA_RTN_COMPLETE) {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_act_rxsleep_request  error=0x%08x", ret);
    }

    return ret;
}

// ytakagi add end
