/*!
 * @skip  $ld:$
 * @file  rrhApi_Rfa_Mnt_CarRel.c
 * @brief API : RF操作関連
 * @date  2013/11/22 ALPHA)ueda Create.
 * @date  2014/12/02 ALPHA)nakamura ppc対応
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2014
 */

/** @addtogroup RRH_PF_RFP_API
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "f_sys_def.h"
#include "BPF_RU_IPCM.h"
#include "BPF_COM_LOG.h"
#include "rrhApi_Com.h"
#include "rrhApi_Rfp.h"
#include "f_sys_type.h"

/********************************************************************************************************************/
/**
 *  @brief  API : Carrier解放要求
 *  @note   PF EVENT ID : 0xA0010005
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid         : response queue id (応答送信先QueueID)
 *  @param  wtime       : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p     : 取得データポインタ
 *  @param  branch      : ブランチ番号(0~7)
 *  @param  carrier     : キャリア番号(0~7)
 *  @return E_RRHAPI_RCODE
 *  @retval E_API_RCD_ERR_BPF_CALL
 *  @retval E_API_RCD_EPARAM
 *  @retval E_API_RCD_OK
 *  @warning	N/A
 *  @FeatureID	PF-Rfp-002-002-001
 *  @Bug_No		N/A
 *  @CR_No		N/A
 *  @TBD_No		N/A
 *  @date   2013/11/22 ALPHA)ueda Create.
 *  @date   2014/12/02 ALPHA)nakamura ppc対応
 *  @date   2015/04/21 ALPHA)fujiwara TDD対応
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Rfa_Mnt_CarRel(int qid,
                                     unsigned int uiAntNum,
                                     unsigned int uiCarrier,
                                     unsigned int uiCrCentFreq,
                                     unsigned int uiTxRx,
                                     unsigned int uiTestFlg)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "rrhApi_Rfa_Mnt_CarRel called, qid = %08x, uiAntNum = %d, uiCarrier = %d, uiCrCentFreq = %d, uiTxRx = %d, uiTestFlg = %d", qid, uiAntNum, uiCarrier, uiCrCentFreq, uiTxRx, uiTestFlg);

    T_SYS_CARRIER_OFF_SETTING_REQ* msgp = NULL;
    int errcd = 0;
    int ret = 0;

    /* 通信用共有メモリを取得する   */
    ret = BPF_RU_IPCM_PROCMSG_ADDRGET(  E_BPF_RU_IPCM_BUFF_KIND_TASK,               /* Buffer種別       */
                                        sizeof(T_SYS_CARRIER_OFF_SETTING_REQ),      /* Size             */
                                        (void**)&msgp,                              /* msgP             */
                                        &errcd );
    if( ret != E_API_RCD_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
        return E_API_RCD_ERR_BPF_CALL;
    }

    /* データの設定を行う           */
    /* Header部設定	*/
    msgp->head.uiEventNo 	 = D_API_RFP_MNT_CARRIER_RELEASE_REQ;       /* Carrier解放要求          */
    msgp->head.uiDstPQueueID = D_RRH_PROCQUE_PF;                        /* 送信先Process Queue ID   */
    msgp->head.uiDstTQueueID = D_SYS_THDQID_PF_RFA;                     /* 送信先Thread Queue ID    */
    msgp->head.uiSrcPQueueID = qid;                                     /* 送信元Process Queue ID   */
    msgp->head.uiSrcTQueueID = 0;                                       /* 送信元Thread Queue ID    */
    msgp->head.uiLength      = sizeof(T_SYS_CARRIER_OFF_SETTING_REQ);   /* Length(Head部 + Data部)  */

    //データ設定 Tx/Rx OFF情報
    msgp->carrierOffSettingReqInfo.uiAntNum      = uiAntNum;
    msgp->carrierOffSettingReqInfo.uiCarrier     = uiCarrier;
    msgp->carrierOffSettingReqInfo.uiCrCentFreq  = uiCrCentFreq;
    msgp->carrierOffSettingReqInfo.uiTxRx        = uiTxRx;
    msgp->carrierOffSettingReqInfo.uiTestFlg     = uiTestFlg;

    /* Carrier解放要求を送信する */
    ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PF,                      /* 送信先Process Queue ID   */
                                 (void*)msgp );                         /* 送信Message              */

    if( ret != E_API_RCD_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
        return E_API_RCD_ERR_BPF_CALL;
    }
    else
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "BPF_RU_IPCM_MSGQ_SEND OK %d", ret);
    }

    return E_API_RCD_OK;

}

/** @} */