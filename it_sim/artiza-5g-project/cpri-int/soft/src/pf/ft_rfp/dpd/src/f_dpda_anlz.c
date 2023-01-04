/*!
 * @skip  $ld:$
 * @file  f_dpda_anlz.c
 * @brief DPDAタスク 受信メッセージ解析処理
 * @date  2013/03/07 ALPHA)松延 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:受信メッセージ内イベント番号とタスク状態から処理関数を実行する
 * @note  関数処理内容.
 *       -# 受信メッセージ内イベント番号異常又はタスク状態異常の場合、アボート処理を実施する(f_dpda_abort())
 *       -# イベント番号とタスク状態をIndexとしたジャンプテーブルより該当処理をCallする
 * @param  *msgP            [in]VOID    受信メッセージ
 * @return INT
 * @retval D_SYS_OK     0:正常終了
 * @retval D_SYS_NG     1:異常終了
 * @warning     N/A
 * @FeatureID   PF-Rfp-001-001-001
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date 2013/03/07 ALPHA)松延 Create
 * @date 2013/11/26 ALPHA)加藤 ZYNQ対応
 * @date 2015/08/06 TDI)satou 「電力報告周期通知」以外を削除
 */
INT f_dpda_anlz(VOID* msgP)
{
    INT                 rtn = D_SYS_OK;         /* 復帰値                          */
    T_SYS_COMMON_THDIF* rcvMsgP;                /* 受信メッセージ                  */
    UINT                evtNo = 0;              /* イベント番号                       */

    rcvMsgP = (T_SYS_COMMON_THDIF*)msgP;

    /* 受信イベント番号からタスク内イベント番号へ変換 */
    switch( rcvMsgP->uiEventNo )
    {
        case D_SYS_MSGID_TIMOUTNTC:                 /* 電力報告周期通知                 */
            evtNo = E_DPDA_EVE_POW_PERIODIC_NOTI;
            break;

        default:
            /*  受信 起動要因異常によるAbort   */
            (VOID)f_dpda_abort_fhm(rcvMsgP);

            /*  異常のため処理中断   */
            return rtn;
    }

    /*  タスク状態異常の場合、アボート処理   */
    if(f_dpdaw_info_all->taskInfo.taskState >= D_DPDA_STATENUM)
    {
        /*  タスク状態異常によるAbort */
        (VOID)f_dpda_abort_fhm(rcvMsgP);
    }
    else
    {
        /* マトリクステーブルに状態、起動要因を設定 */
        (VOID)(*(f_dpdar_callSetRell_mtxtbl[f_dpdaw_info_all->taskInfo.taskState][evtNo].eventProcess_addr))(rcvMsgP);
    }

    return rtn;
}

/* @} */
