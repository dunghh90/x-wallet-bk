/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_recrdssc_stp_3g.c
 * @brief  RE起動中REカード状態報告(セクタ部)中止処理
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
 * @brief  RE起動中REカード状態報告(セクタ部)中止処理
 * @note   RE起動中REカード状態報告(セクタ部)中止処理を行う
 *         - スレッド間(REカード状態報告(セクタ部)プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(RE PORT状態報告完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/17 FFCS)Shihzh modify for S3G PRC.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_recrdssc_stp_3g(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /******************************************************************/
    /* スレッド間(REカード状態報告(セクタ部)プロシージャ実行指示)送信 */
    /******************************************************************/
    f_trp_rec_Fc02_t_it_recrdsscprc_snd_3g(                        /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                               /* なし:R */
        EC_REC_C02_STN_RES_REPRTSTSCMP   /* RE起動中(RE PORT状態報告完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
