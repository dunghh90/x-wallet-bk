/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_recrrsts_stp_3g.c
 * @brief  運用中REキャリア状態報告中止処理
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
 * @brief  運用中REキャリア状態報告中止処理
 * @note   運用中REキャリア状態報告中止処理を行う
 *         - スレッド間(REキャリア状態報告プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(REカード状態報告(共通部)完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_recrrsts_stp_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************************************************/
    /* スレッド間(REキャリア状態報告プロシージャ実行指示)送信 */
    /**********************************************************/
    f_trp_rec_Fc02_t_it_recrrstsprc_snd_3g(                          /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_RECRDSCMCMP        /* RE起動中(RE状態報告完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
