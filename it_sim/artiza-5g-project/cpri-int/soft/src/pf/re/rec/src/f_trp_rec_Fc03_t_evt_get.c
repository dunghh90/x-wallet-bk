/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_t_evt_get.c
 * @brief  イベント取得
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
 * @brief  イベント取得
 * @note   イベントを取得する
 * @param  -
 * @return イベント
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
ET_REC_C03_EVT f_trp_rec_Fc03_t_evt_get(                   /* C03イベント:R */
) {
    USHORT cpr_no_idx = D_REC_C03_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT crr_no_idx = D_REC_C03_CRR_NO() - CMD_NUM1;
                                                /* キャリア番号インデックス */
    ET_REC_C03_EVT evt;                                      /* C03イベント */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*******************/
    /* C03イベント取得 */
    /*******************/
    evt = f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].evt;

    return evt;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
