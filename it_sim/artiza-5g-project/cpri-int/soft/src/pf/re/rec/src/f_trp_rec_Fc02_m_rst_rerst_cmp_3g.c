/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_rst_rerst_cmp_3g.c
 * @brief  REリセット完了処理
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
 * @brief  REリセット完了処理
 * @note   REリセット完了処理を行う
 *         - 自状態番号設定(RE起動中(LAPBリンク確立前))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/09/17 TDI)satou 状態遷移先を(04)RE起動待ちアイドルに
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_rst_rerst_cmp_3g(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************/
    /* タイマ取消 */
    /**************/
    f_trp_rec_Fc02_t_timer_ccl_3g(                                   /* なし:R */
        EC_REC_TMK_RERST_CNF,                     /* REリセット確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_IDL                  /* (04)RE起動待ちアイドル:I    */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
