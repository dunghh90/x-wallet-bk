/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_evt_get_3g.c
 * @brief  イベント取得
 * @date   2015/07/15 FPT)Yen create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  イベント取得
 * @note   イベントを取得する
 * @param  -
 * @return イベント
 * @date   2015/07/15 FPT)Yen create.
 */
/****************************************************************************/
ET_REC_C02_EVT f_trp_rec_Fc02_t_evt_get_3g(                /* C02イベント:R */
) {
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    ET_REC_C02_EVT evt;                                      /* C02イベント */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*******************/
    /* C02イベント取得 */
    /*******************/
    evt = f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].evt;

    return evt;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
