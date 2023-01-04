/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_l3_tofvalreq_snd.c
 * @brief  L3(Toffset値報告要求)送信
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
 * @brief  L3(Toffset値報告要求)送信
 * @note   L3(Toffset値報告要求)を送信する
 *         - L3(Toffset値報告要求)送信
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_l3_tofvalreq_snd(                           /* なし:R */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*****************************/
    /* L3(Toffset値報告要求)送信 */
    /*****************************/
    f_trp_rec_Fcom_t_l3_tofvalreq_snd(                      /* 終了コード:R */
        D_REC_C02_CPR_NO()                                    /* CPRI番号:I */
    );

    /**************/
    /* タイマ登録 */
    /**************/
    f_trp_rec_Fc02_t_timer_ent(                                   /* なし:R */
        EC_REC_TMK_TOFVAL_CNF,                 /* Toffset値報告確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
