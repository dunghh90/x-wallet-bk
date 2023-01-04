// ytakagi add
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"


/*!
 * @brief 関数機能概要:TXキャリア SLEEP設定
 * @note  関数処理内容:TXキャリア SLEEP設定
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @return int
 * @date 2020/01/30 FJT)長谷川 Create
 */
int f_dpda_act_txCarrierSleepSettingRequest(unsigned int uiAntNum, unsigned int uiCarrier)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_txCarrierSleepSettingRequest called, uiAntNum = %d, uiCarrier = %d", uiAntNum, uiCarrier);

    int ret = D_DPDA_RTN_COMPLETE;
    /* tx-array-carrier毎スリープ制御 ON */

    BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, 0xB1000050+uiAntNum*4, 0x2);

    /* SVレジスタのSLEEP値を更新(ON)*/
    f_com_SVCtl((E_DU_SVCTL_OPE_USE_A + uiAntNum), D_DU_SVCTL_ON, (D_DU_SVCTL_ON_SLP_C0 << uiCarrier));

    return ret;
}

/*!
 * @brief 関数機能概要:TXキャリア SLEEP OFF設定
 * @note  関数処理内容:TXキャリア SLEEP OFF設定
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @return int
 * @date 2020/01/30 FJT)長谷川 Create
 */
int f_dpda_act_txCarrierSleepOffSettingRequest(unsigned int uiAntNum, unsigned int uiCarrier)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_txCarrierSleepOffSettingRequest called, uiAntNum = %d, uiCarrier = %d", uiAntNum, uiCarrier);

    int ret = D_DPDA_RTN_COMPLETE;
    /* tx-array-carrier毎スリープ制御 OFF */

    BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, 0xB1000050+uiAntNum*4, 0x2);

	f_dpda_com_clearQEC();

    /* SVレジスタのSLEEP値を更新(OFF)*/
    f_com_SVCtl((E_DU_SVCTL_OPE_USE_A + uiAntNum), D_DU_SVCTL_OFF, (D_DU_SVCTL_ON_SLP_C0 << uiCarrier));

    return ret;
}


/*!
 * @brief 関数機能概要:RXキャリア SLEEP設定
 * @note  関数処理内容:RXキャリア SLEEP設定
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @return int
 * @date 2020/01/30 FJT)長谷川 Create
 */
int f_dpda_act_rxCarrierSleepSettingRequest(unsigned int uiAntNum, unsigned int uiCarrier)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxCarrierSleepSettingRequest called, uiAntNum = %d, uiCarrier = %d", uiAntNum, uiCarrier);
    int ret = D_DPDA_RTN_COMPLETE;
    /* rx-array-carrier毎スリープ制御 ON */

    /* 処理なし */

    return ret;
}

/*!
 * @brief 関数機能概要:RXキャリア SLEEP OFF設定
 * @note  関数処理内容:RXキャリア SLEEP OFF設定
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @param  uiCarrier    [in]unsigned int    キャリア番号(0~7)
 * @return int
 * @date 2020/01/30 FJT)長谷川 Create
 */
int f_dpda_act_rxCarrierSleepOffSettingRequest(unsigned int uiAntNum, unsigned int uiCarrier)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_act_rxCarrierSleepOffSettingRequest called, uiAntNum = %d, uiCarrier = %d", uiAntNum, uiCarrier);
    int ret = D_DPDA_RTN_COMPLETE;
    /* rx-array-carrier毎スリープ制御 OFF */

    /* 処理なし */

	f_dpda_com_clearQEC();

    return ret;
}
// ytakagi add end
