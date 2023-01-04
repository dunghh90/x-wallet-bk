/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_inttim_ent_3g.c
 * @brief  送信間隔タイマ登録
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
 * @brief  送信間隔タイマ登録
 * @note   送信間隔タイマを登録する
 *         - タイマ登録(RE状態報告送信間隔タイマ)
 *         - タイマ登録(REカード状態報告(セクタ部)送信間隔タイマ)
 *         - タイマ登録(REキャリア状態報告送信間隔タイマ)
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 * @date   2015/09/04 FPT)Yen modify for FHM.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_inttim_ent_3g(                                 /* なし:R */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************/
    /* タイマ登録 */
    /**************/
    f_trp_rec_Fc02_t_timer_ent_3g(                                   /* なし:R */
        EC_REC_TMK_RESTS_INT,                 /* RE状態報告送信間隔タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    /**************/
    /* タイマ登録 */
    /**************/
    f_trp_rec_Fc02_t_timer_ent_3g(                                   /* なし:R */
        EC_REC_TMK_RECRDSSC_INT,
                             /* REカード状態報告(セクタ部)送信間隔タイ マ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
   );

    /**************/
    /* タイマ登録 */
    /**************/
    f_trp_rec_Fc02_t_timer_ent_3g(                                   /* なし:R */
        EC_REC_TMK_RECRRSTS_INT,      /* REキャリア状態報告送信間隔タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
