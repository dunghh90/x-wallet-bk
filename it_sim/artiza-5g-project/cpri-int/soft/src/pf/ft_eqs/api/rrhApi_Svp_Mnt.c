/*!
 * @skip  $ld:$
 * @file  rrhApi_Rfa_Mqr_CarSet.c
 * @brief API : RF操作関連
 * @date  2013/11/22 ALPHA)ueda Create.
 * @date  2014/12/02 ALPHA)nakamura ppc対応
 * @date  2015/04/21 ALPHA)fujiwara TDD対応
 * @date  2015/09/04 ALPHA)fujiwara TDD-RRE(Zynq)対応
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
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
#include "rrhApi_Svp.h"
#include "BPF_HM_DEVC.h"
#include "f_sys_type.h"


/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_Inhibit_stpWv_tp(INT	qid, INT	wtime, VOID	*data_p)
{
	T_API_SVP_CMD_STPWVMSK_NTC	*pSndMsg;
	int							errcd;
	int							ret;
	
    //通信用共有メモリを取得する
    ret = BPF_RU_IPCM_PROCMSG_ADDRGET(  E_BPF_RU_IPCM_BUFF_KIND_TASK,          /* Buffer種別       */
                                        sizeof(T_API_SVP_CMD_STPWVMSK_NTC),    /* Size             */
                                        (void**)&pSndMsg,                      /* msgP             */
                                        &errcd );
    if( ret != E_API_RCD_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
        return E_API_RCD_ERR_BPF_CALL;
    }
    //データ設定 Head
    pSndMsg->header.uiEventNo     = D_API_MSGID_SVP_CMD_STPWVMSK_NTC;	  /* SVCtl通知		          */
    pSndMsg->header.uiDstPQueueID = D_RRH_PROCQUE_PF;                    /* 送信先Process Queue ID   */
    pSndMsg->header.uiDstTQueueID = D_SYS_THDQID_PF_EQS;                 /* 送信先Thread Queue ID    */
    pSndMsg->header.uiSrcPQueueID = qid;                                 /* 送信元Process Queue ID   */
    pSndMsg->header.uiSrcTQueueID = 0;                                   /* 送信元Thread Queue ID    */
    pSndMsg->header.uiLength      = sizeof(T_API_SVP_CMD_STPWVMSK_NTC);  /* Length(Head部 + Data部)  */

    /* 停波制御抑止通知を送信する */
    ret = BPF_RU_IPCM_MSGQ_SEND(    D_RRH_PROCQUE_PF,                   /* 送信先Process Queue ID   */
                                    (void*)pSndMsg );                      /* 送信Message              */

    if( ret != E_API_RCD_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
        return E_API_RCD_ERR_BPF_CALL;
    }
    BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "BPF_RU_IPCM_MSGQ_SEND OK %d", ret);
    return E_API_RCD_OK;
}

/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Svp_Mnt_Dbg_svCtl(	
								INT		qid, 	INT		wtime, VOID *data_p,
								UINT	svOpt,
								UINT	svCnt,
								UINT	svVal)
{

    T_API_SVP_DBG_SVCTL_NTC	* msgp;
    int errcd;
    int ret;

    //通信用共有メモリを取得する
    ret = BPF_RU_IPCM_PROCMSG_ADDRGET(  E_BPF_RU_IPCM_BUFF_KIND_TASK,          /* Buffer種別       */
                                        sizeof(T_API_SVP_DBG_SVCTL_NTC),       /* Size             */
                                        (void**)&msgp,                         /* msgP             */
                                        &errcd );
    if( ret != E_API_RCD_OK )
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
        return E_API_RCD_ERR_BPF_CALL;
    }

    //データ設定 Head
    msgp->header.uiEventNo     = D_API_MSGID_SVP_DBG_SVCTL_NTC;		  /* SVCtl通知		          */
    msgp->header.uiDstPQueueID = D_RRH_PROCQUE_PF;                    /* 送信先Process Queue ID   */
    msgp->header.uiDstTQueueID = D_SYS_THDQID_PF_EQS;                 /* 送信先Thread Queue ID    */
    msgp->header.uiSrcPQueueID = qid;                                 /* 送信元Process Queue ID   */
    msgp->header.uiSrcTQueueID = 0;                                   /* 送信元Thread Queue ID    */
    msgp->header.uiLength      = sizeof(T_API_SVP_DBG_SVCTL_NTC);	  /* Length(Head部 + Data部)  */

    //データ設定 Data
    msgp->data.svOpt      = svOpt;
    msgp->data.svCnt      = svCnt;
    msgp->data.svVal      = svVal;

    /* SVバス制御通知を送信する */
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