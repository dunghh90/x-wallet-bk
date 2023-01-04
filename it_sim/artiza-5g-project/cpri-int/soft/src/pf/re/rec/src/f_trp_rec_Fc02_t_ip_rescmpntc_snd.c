/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_ip_rescmpntc_snd.c
 * @brief  プロセス間(RE再開完了通知)送信
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  プロセス間(RE再開完了通知)送信
 * @note   プロセス間(RE再開完了通知)を送信する
 *         - バッファ取得
 *         - バッファ設定(データ)
 *         - プロセス間送信
 *         - バッファ解放
 * @param  awSysType [in] system type
 * @param  awResult  [in] 結果
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/09/10 FPT)Yen update for FHM
 * @date   2015/11/13 TDIPS)enoki
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_ip_rescmpntc_snd(                           /* なし:R */
    USHORT awSysType,
    USHORT awResult
) {
    CMT_TSKIF_RESTR_RSP buf;                                    /* バッファ */
    CMT_TSKIF_RESTR_RSP* bufp = &buf;                   /* バッファポインタ */
    INT    sit_ret;                                           /* 終了コード */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif


    /* RCTに(0x4006)RE Startup Reponse送信	*/
    bufp->head.uiEventNo        = CMD_TSKIF_RE_STARTUP_RSP;         /* Event  ID         */
    bufp->head.uiSignalkind     = 0;                                /* Signal Kind       */
    bufp->head.uiDstPQueueID    = D_RRH_PROCQUE_L3;                 /* Destination P QID */
    bufp->head.uiDstTQueueID    = 0;                                /* Destination T QID */
    bufp->head.uiSrcPQueueID    = D_RRH_PROCQUE_RE;                 /* Source P QID      */
    bufp->head.uiDstTQueueID    = 0;                                /* Destination T QID */
    bufp->head.uiLength         = sizeof(CMT_TSKIF_RESTR_RSP);      /* Length            */

    /* CPRI番号設定 */
    bufp->link_num =  ( awSysType == CMD_SYS_LTE )? D_REC_C02_CPR_NO() :  D_REC_C02_3G_CPR_NO() ;
    /* Systerm type: LTE/3G */
    bufp->system_type =  awSysType;
    /* 応答結果: LTE */
    bufp->result =  awResult;

    /* apl_com_msgQSend(UINT msgQid,UINT msgid,void *msgp,UINT msgsz )	*/
    /* msgQid	:送信先QID*/
    /* msgid	:イベント番号	*/
    /* msgp		:送信MSG*/
    /* msgsz	:MSGサイズ*/
    sit_ret = apl_com_msgQSend(
    		D_RRH_PROCQUE_L3,		/* Thread queue ID*/
    		CMD_TSKIF_RE_STARTUP_RSP,	/* Event ID	*/
    		(VOID *)bufp,			/* The message to transmit	*/
    		sizeof(CMT_TSKIF_RESTR_RSP)	/* The size of message		*/
    	);
    /* checking parameter */
    if( sit_ret != D_RRH_OK ) {
    	cm_Assert(CMD_ASL_DBGLOW, sit_ret,
    	    (CHAR*)"[f_trp_rec_Fc02_t_ip_rescmpntc_snd] Send RE Startup Reponse  NG" );
    }

    return;
}
/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/

