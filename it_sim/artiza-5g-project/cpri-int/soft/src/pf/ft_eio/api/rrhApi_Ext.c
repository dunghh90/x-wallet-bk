/*!
 * @skip  $ld:$
 * @file  rrhApi_Ext.c
 * @brief API : 外部ポート操作
 * @date  2019/03/18 FJT)koshida Create.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019-
 */

/** @addtogroup RRH_PF_EIO_API
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
#include "rrhApi_Ext.h"
#include "BPF_HM_DEVC.h"
#include "f_sys_type.h"

/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Ext_Mnt_FanDbgModeChgNtc(INT		qid, 	INT		wtime, VOID *data_p)
{

    T_API_EXT_FAN_DGBMODE_CHG_NTC	* msgp;
    int errcd;
    int ret;

    //通信用共有メモリを取得する
    ret = BPF_RU_IPCM_PROCMSG_ADDRGET(  E_BPF_RU_IPCM_BUFF_KIND_TASK,          /* Buffer種別       */
                                        sizeof(T_API_EXT_FAN_DGBMODE_CHG_NTC), /* Size             */
                                        (void**)&msgp,                         /* msgP             */
                                        &errcd );
    if( ret != E_API_RCD_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
        return E_API_RCD_ERR_BPF_CALL;
    }

    //データ設定 Head
    msgp->header.uiEventNo     = D_API_MSGID_EXT_MNT_FANDBGMODE_CHG_NTC;	/* FANデバッグモード変更通知	*/
    msgp->header.uiDstPQueueID = D_RRH_PROCQUE_PF;							/* 送信先Process Queue ID   */
    msgp->header.uiDstTQueueID = D_SYS_THDQID_PF_EIO;						/* 送信先Thread Queue ID    */
    msgp->header.uiSrcPQueueID = qid;										/* 送信元Process Queue ID   */
    msgp->header.uiSrcTQueueID = 0;											/* 送信元Thread Queue ID    */
    msgp->header.uiLength      = sizeof(T_API_EXT_FAN_DGBMODE_CHG_NTC);		/* Length(Head部 + Data部)  */

    /* FANデバッグモード変更通知を送信する */
    ret = BPF_RU_IPCM_MSGQ_SEND(    D_RRH_PROCQUE_PF,                   /* 送信先Process Queue ID   */
                                    (void*)msgp );                      /* 送信Message              */

    if( ret != E_API_RCD_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
        return E_API_RCD_ERR_BPF_CALL;
    }
    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "BPF_RU_IPCM_MSGQ_SEND OK %d", ret);
    return E_API_RCD_OK;
}

/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Ext_Mnt_FanDacChgNtc(INT		qid, 	INT		wtime, VOID *data_p,
											UINT	dacNo, UINT	dacSetVal)
{

    T_API_EXT_FAN_DAC_CHG_NTC	* msgp;
    int errcd;
    int ret;

    //通信用共有メモリを取得する
    ret = BPF_RU_IPCM_PROCMSG_ADDRGET(  E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別       */
                                        sizeof(T_API_EXT_FAN_DAC_CHG_NTC),		/* Size             */
                                        (void**)&msgp,							/* msgP             */
                                        &errcd );
    if( ret != E_API_RCD_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
        return E_API_RCD_ERR_BPF_CALL;
    }

    //データ設定 Head
    msgp->header.uiEventNo		= D_API_MSGID_EXT_MNT_FANDAC_CHG_NTC;	/* FAN電圧制御DAC設定値変更通知	*/
    msgp->header.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID   */
    msgp->header.uiDstTQueueID	= D_SYS_THDQID_PF_EIO;					/* 送信先Thread Queue ID    */
    msgp->header.uiSrcPQueueID	= qid;									/* 送信元Process Queue ID   */
    msgp->header.uiSrcTQueueID	= 0;										/* 送信元Thread Queue ID    */
    msgp->header.uiLength		= sizeof(T_API_EXT_FAN_DAC_CHG_NTC);		/* Length(Head部 + Data部)  */
    
    msgp->fanDacInfo.dacNo		= dacNo;
    msgp->fanDacInfo.dacSetVal	= dacSetVal;

    /* FAN電圧制御DAC設定値変更通知を送信する */
    ret = BPF_RU_IPCM_MSGQ_SEND(    D_RRH_PROCQUE_PF,                   /* 送信先Process Queue ID   */
                                    (void*)msgp );                      /* 送信Message              */

    if( ret != E_API_RCD_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
        return E_API_RCD_ERR_BPF_CALL;
    }
    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "BPF_RU_IPCM_MSGQ_SEND OK %d", ret);
    return E_API_RCD_OK;
}

/** @} */