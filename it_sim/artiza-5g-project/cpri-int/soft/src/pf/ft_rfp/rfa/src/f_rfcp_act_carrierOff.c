/*!
 * @skip  $ld:$
 * @file  f_rfcp_act_carrierOff.c(base: f_rfcp_act_pfDcTrDeactivate)
 * @brief pf_rfcpスレッド キャリア設定解放処理
 * @date  2013/03/08 ALPHA)松延 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_rfcp_inc.h"
#include "f_sys_type.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_rfcp_act_carrierOff
 * @note		キャリア解放受信時の処理を行う
 *				-# パラメータチェック実施
 *				-# pf_dpdaに"Carrier OFF Setting Request"を送信
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-002-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/03/07 ALPHA)松延 Create
 * @date		2013/11/22 ALPHA)藤井 Modify for docomo zynq
 * @date		2015/04/22 ALPHA)藤原 TDD対応
 */
/****************************************************************************************************************************/
int f_rfcp_act_carrierOff(void* msgP)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_rfcp_act_carrierOff called");

    T_SYS_CARRIER_OFF_SETTING_REQ *msgOFF = NULL;         /* キャリアONメッセージ */
    T_SYS_CARRIER_OFF_SETTING_REQ_INFO carInfo;           /* キャリア情報         */

    msgOFF = (T_SYS_CARRIER_OFF_SETTING_REQ*)msgP;
    carInfo.uiAntNum        = msgOFF->carrierOffSettingReqInfo.uiAntNum;
    carInfo.uiCarrier       = msgOFF->carrierOffSettingReqInfo.uiCarrier;
    carInfo.uiCrCentFreq    = msgOFF->carrierOffSettingReqInfo.uiCrCentFreq;
    carInfo.uiTxRx          = msgOFF->carrierOffSettingReqInfo.uiTxRx;
    carInfo.uiTestFlg       = msgOFF->carrierOffSettingReqInfo.uiTestFlg;

    /********************************************************************************************************/
    /* メッセージ送信(Carrier Off Setting Request)                                                          */
    /********************************************************************************************************/
    (void)f_rfcp_sendCarrierOffSetting(	&carInfo, D_RRH_PROCQUE_PF, D_SYS_THDQID_PF_RFA );

    return D_SYS_OK;
}

/* @} */
