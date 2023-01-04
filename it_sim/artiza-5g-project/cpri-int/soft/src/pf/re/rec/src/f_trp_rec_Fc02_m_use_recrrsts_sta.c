/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_recrrsts_sta.c
 * @brief  運用中REキャリア状態報告開始処理
 * @date   2008/07/21 FFCS)Shihzh Create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中REキャリア状態報告開始処理
 * @note   運用中REキャリア状態報告開始処理を行う
 *         - L3(REキャリア状態報告)送信
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh Create.
 * @date   2010/10/25 FJT)Koshida Modify for eNB-004 of TS-656案5対応.
 * @date   2015/08/11 FPT)Tan modify for FHM
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_recrrsts_sta(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************************/
    /* L3(REキャリア状態報告要求)送信 */
    /**********************************/
    f_trp_rec_Fc02_t_l3_recrrstsreq_snd(                          /* なし:R */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
