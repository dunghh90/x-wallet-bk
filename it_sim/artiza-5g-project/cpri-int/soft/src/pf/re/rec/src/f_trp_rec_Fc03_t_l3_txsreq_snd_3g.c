/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_t_l3_txsreq_snd_3g.c
 * @brief  L3(TRX設定要求)送信
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
 * @brief  L3(TRX設定要求)送信
 * @note   L3(TRX設定要求)を送信する
 *         - L3(TRX設定要求)送信
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc03_t_l3_txsreq_snd_3g(                              /* なし:R */
) {

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);   

    /***********************/
    /* L3(TRX設定要求)送信 */
    /***********************/
    f_trp_rec_Fcom_t_l3_txsreq_snd_3g(                         /* 終了コード:R */
        D_REC_C03_3G_CPR_NO(),                                   /* CPRI番号:I */
        D_REC_C03_3G_CRR_NO()                               /* キャリア番号:I */
    );

    /**************/
    /* タイマ登録 */
    /**************/
    f_trp_rec_Fc03_t_timer_ent_3g(                                   /* なし:R */
        EC_REC_TMK_TXS_CNF                              /* TRX設定確認タイマ:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/