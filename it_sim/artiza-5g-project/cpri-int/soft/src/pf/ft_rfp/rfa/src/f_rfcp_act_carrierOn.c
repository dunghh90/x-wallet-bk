/*!
 * @skip  $ld:$
* @file  f_rfcp_act_carrierOn.c(base: f_rfcp_act_pfDcTrActivate.c)
 * @brief pf_rfcpスレッド キャリア設定要求処理
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
 * @brief		f_rfcp_act_carrierOn
 * @note		キャリア設定受信時の処理を行う
 *				-# パラメータチェック実施
 *				-# 送信元にに"Carrier ON Setting Response"を送信
 *				-# pf_dpdaに"Carrier ON Setting Request"を送信
 *				-# キャリア情報テーブル更新
 *				-# パラメータチェックに送信系統数チェック処理追加
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/03/07 ALPHA)松延 Create
 * @date		2013/11/22 ALPHA)藤井 Modify for docomo zynq
 * @date		2015/04/22 ALPHA)藤原 TDD対応
 */
/****************************************************************************************************************************/
int f_rfcp_act_carrierOn(void* msgP)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_rfcp_act_carrierOn called");

    T_SYS_CARRIER_ON_SETTING_REQ *msgON = NULL;         /* キャリアONメッセージ */
    T_SYS_CARRIER_ON_SETTING_REQ_INFO carInfo;          /* キャリア情報         */

    msgON = (T_SYS_CARRIER_ON_SETTING_REQ*)msgP;
    carInfo.uiAntNum        = msgON->carrierOnSettingReqInfo.uiAntNum;
    carInfo.uiCarrier       = msgON->carrierOnSettingReqInfo.uiCarrier;
    carInfo.uiAccellId      = msgON->carrierOnSettingReqInfo.uiAccellId;
    carInfo.uiCcId          = msgON->carrierOnSettingReqInfo.uiCcId;
    carInfo.uiDuPortId      = msgON->carrierOnSettingReqInfo.uiDuPortId;
    carInfo.uiCrCentFreq    = msgON->carrierOnSettingReqInfo.uiCrCentFreq;
    carInfo.uiBw            = msgON->carrierOnSettingReqInfo.uiBw;
    carInfo.uiTxRx          = msgON->carrierOnSettingReqInfo.uiTxRx;
    carInfo.uiTestFlg       = msgON->carrierOnSettingReqInfo.uiTestFlg;
    carInfo.uiScs           = msgON->carrierOnSettingReqInfo.uiScs;

    /********************************************************************************************************/
    /* メッセージ送信(Carrier On Setting Request)                                                           */
    /********************************************************************************************************/
    (void)f_rfcp_sendCarrierOnSetting( &carInfo, D_RRH_PROCQUE_PF, D_SYS_THDQID_PF_RFA );

    return D_SYS_OK;
}

/* @} */
