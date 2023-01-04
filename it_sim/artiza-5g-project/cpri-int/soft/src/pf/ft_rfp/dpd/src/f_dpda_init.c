/*!
 * @skip  $ld:$
 * @file  f_dpda_init.c
 * @brief DPDAタスク 初期化処理
 * @date  2011/07/26 ALPHA)横山 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2011-2014
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:DPDAタスクの初期化処理を行う
 * @note  関数処理内容.
 *       -# DPDAタスクの内部テーブル初期化を行う
 *       -# 初期化タスクに初期化完了通知(Task Init completion notice)を送信する(f_com_msgQSend())
 *       -# タスク状態を"運用状態"に遷移させる
 * @param  msgP             [in]VOID    受信メッセージ
 * @return INT
 * @retval D_SYS_OK     0:正常終了
 * @retval D_SYS_NG     1:異常終了
 * @warning     N/A
 * @FeatureID   PF-Rfp-001-001-001
 * @Bug_No      N/A
 * @CR_No       N/A
 * @TBD_No      N/A
 * @date 2014/02/25 ALPHA)藤井 Create
 * @date 2015/04/23 ALPHA)藤井 TDD対応
 * @date 2015/08/06 TDI)satou 「TDD自律OFF」処理を削除
*/
INT f_dpda_init_fhm(VOID* msgP)
{
    T_SYS_INIT_COMP_NTC*            sndMsg;                 /* 送信メッセージ(初期化完了通知)                     */
    INT                             ret;
    INT                             errcd;

    /****************************************************************************************************************/
    /* DPDAタスク内部テーブル初期化                                                                                 */
    /****************************************************************************************************************/
    f_dpda_init_taskTable();

    /****************************************************************************************************************/
    /* 初期化完了通知送信                                                                                            */
    /****************************************************************************************************************/
    /* 通信用共有メモリを取得する    */
    ret = BPF_RU_IPCM_PROCMSG_ADDRGET(  E_BPF_RU_IPCM_BUFF_KIND_TASK,           /* Buffer種別                 */
                                        sizeof(T_SYS_INIT_COMP_NTC),            /* Size                         */
                                        (VOID **)&sndMsg,                       /* msgP                         */
                                        &errcd );
    if( ret != D_SYS_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR,
                            "failed get process message address %d\n",
                            errcd);
        return D_SYS_NG;
    }

    /* 送信MSG作成 */
    sndMsg->head.uiEventNo          = D_SYS_MSGID_INIT_COMP_NTC;        /* イベント番号                       */
    sndMsg->head.uiSignalkind       = 0;                                /* SignalKind                       */
    sndMsg->head.uiDstPQueueID      = D_RRH_PROCQUE_F_PF;                 /* 最終送信先Process Queue ID        */
    sndMsg->head.uiDstTQueueID      = D_SYS_THDQID_PF_F_MAIN;             /* 最終送信先Thread  Queue ID        */
    sndMsg->head.uiSrcPQueueID      = D_RRH_PROCQUE_F_PF;                 /* 送信元    Process Queue ID      */
    sndMsg->head.uiSrcTQueueID      = D_SYS_THDQID_PF_F_DPD;              /* 送信元    Thread  Queue ID      */
    sndMsg->head.uiLength           = sizeof(T_SYS_INIT_COMP_NTC);      /*Length(Head部 + Data部)         */

    BPF_RU_IPCM_MSGQ_SEND(  D_SYS_THDQID_PF_F_MAIN,                       /* 送信MSGキューID                   */
                            sndMsg);                                    /* 送信MSG                            */

    /* タスク状態遷移(運用状態) */
    f_dpdaw_info_all->taskInfo.taskState= D_DPDA_TASK_STS_ACT;

    return D_SYS_OK;
}
/* @} */
