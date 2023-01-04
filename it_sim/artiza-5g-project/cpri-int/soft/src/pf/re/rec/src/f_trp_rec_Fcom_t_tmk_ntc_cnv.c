/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_tmk_ntc_cnv.c
 * @brief  タイマ種別→通知種別変換
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
 * @brief  タイマ種別→通知種別変換
 * @note   タイマ種別→通知種別に変換する
 * @param  tmk [in] タイマ種別
 * @return 通知種別
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
bpf_ru_hrtm_notify_type_t f_trp_rec_Fcom_t_tmk_ntc_cnv(       /* 通知種別:R */
    ET_REC_TMK tmk                                          /* タイマ種別:I */
) {
    bpf_ru_hrtm_notify_type_t ntc_kind;                         /* 通知種別 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************/
    /* 通知種別取得 */
    /****************/
    ntc_kind = f_trp_rec_Rcom_tmk_ntc_tbl.ntc[tmk].ntc_kind;

    return ntc_kind;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
