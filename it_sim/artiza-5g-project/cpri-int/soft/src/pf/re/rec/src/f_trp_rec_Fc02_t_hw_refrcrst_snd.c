/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_hw_refrcrst_snd.c
 * @brief  HW(RE強制リセット)送信
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
 * @brief  HW(RE強制リセット)送信
 * @note   HW(RE強制リセット)を送信する
 *         - HW(RE強制リセット)送信
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_hw_refrcrst_snd(                            /* なし:R */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************************/
    /* HW(RE強制リセット)送信 */
    /**************************/
    f_trp_rec_Fcom_t_hw_refrcrst_snd(                       /* 終了コード:R */
        D_REC_C02_CPR_NO()                                    /* CPRI番号:I */
    );

    /**************/
    /* タイマ登録 */
    /**************/
    f_trp_rec_Fc02_t_timer_ent(                                   /* なし:R */
        EC_REC_TMK_REFRCRST_CNF,              /* RE強制リセット確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
