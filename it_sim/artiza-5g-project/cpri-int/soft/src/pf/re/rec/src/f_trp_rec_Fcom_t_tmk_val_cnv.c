/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_tmk_val_cnv.c
 * @brief  タイマ種別→タイマ値変換
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
 * @brief  タイマ種別→タイマ値変換
 * @note   タイマ種別→タイマ値に変換する
 * @param  tmk [in] タイマ種別
 * @return タイマ値
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_tmk_val_cnv(                            /* タイマ値:R */
    ET_REC_TMK tmk                                          /* タイマ種別:I */
) {
    UINT   tim_val;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************/
    /* タイマ値取得 */
    /****************/
    tim_val = f_trp_rec_Wcom_tmk_val_tbl.val[tmk].tim_val;

    return tim_val;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
