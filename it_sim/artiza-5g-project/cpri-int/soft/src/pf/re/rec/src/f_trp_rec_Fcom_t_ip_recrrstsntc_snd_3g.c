/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_ip_recrrstsntc_snd_3g.c
 * @brief  プロセス間(REキャリア状態変化通知)送信
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  プロセス間(REキャリア状態変化通知)送信
 * @note   プロセス間(REキャリア状態変化通知)を送信する
 *         - バッファ取得
 *         - バッファ設定(データ)
 *         - プロセス間送信
 *         - バッファ解放
 * @param  cpr_no [in] CPRI番号
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 * @TBD_No 修正未完了
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_ip_recrrstsntc_snd_3g(                         /* なし:R */
    USHORT cpr_no                                             /* CPRI番号:I */
) {
//    T_MSG_RECRRSTSCHG_NTC* bufp;                        /* バッファポインタ */
//    UINT   msglen = sizeof(T_MSG_RECRRSTSCHG_NTC) - sizeof(T_MSGHEAD); 
//                                                            /* メッセージ長 */
//    UINT   msgid  = D_MSG_RECRRSTSCHG_NTC;                  /* メッセージID */
//    USHORT dst_pno = D_PRCSID_OAMPF;                  /* 送信先プロセス番号 */
//    INT    sit_ret;                                           /* 終了コード */
//    CHAR  errdata[CMD_NUM48];
//
//    /* 関数トレースログ */
//    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
//
//    /****************/
//    /* バッファ取得 */
//    /****************/
//    sit_ret = f_trp_com_Falloc_buf(                         /* 終了コード:R */
//        sizeof(*bufp),                                  /* バッファサイズ:I */
//        (VOID*)&bufp                                  /* バッファアドレス:O */
//    );
//    /* NGの場合 */
//    if (sit_ret != CMD_OK) {
//#ifndef D_OPT_TRIF_BUFNGLOG_INVALID
//        /* 無効処理履歴 */
//        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][siz:0x%08x][ret:0x%02x]Alloc buffer NG.", D_REC_C02_3G_CPR_NO(), sizeof(*bufp), sit_ret);
//        D_REC_IVLINI();
//        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
//#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
//        return;
//    }
//    
//    /************************/
//    /* バッファ設定(データ) */
//    /************************/
//    f_trp_rec_Fcom_t_recrrsts_get(                                /* なし:R */
//        cpr_no,                                               /* CPRI番号:I */
//        (VOID*)&(bufp->data)                                    /* データ:O */
//    );
//    
//    /******************/
//    /* プロセス間送信 */
//    /******************/
//    sit_ret = f_trp_rec_Fcom_t_ip_snd(                      /* 終了コード:R */
//        (VOID*)bufp,                                  /* バッファポインタ:I */
//        msgid,                                            /* メッセージID:I */
//        dst_pno,                                    /* 送信先プロセス番号:I */
//        msglen                                            /* メッセージ長:I */
//    );
//    /* NGの場合 */
//    if (sit_ret != CMD_OK) {
//        /* 無効処理履歴 */
//        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IP send NG.", D_REC_C02_3G_CPR_NO(), sit_ret);
//        D_REC_IVLINI();
//        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
//        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
//    }
//
//    /****************/
//    /* バッファ解放 */
//    /****************/
//    f_trp_com_Ffree_buf(                                          /* なし:R */
//        bufp                                          /* バッファアドレス:I */
//    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
