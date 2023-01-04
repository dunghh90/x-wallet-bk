/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_t_frq_get.c
 * @brief  周波数取得
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
 * @brief  周波数取得
 * @note   周波数を取得する
 *         - 周波数取得
 * @param  -
 * @return 周波数
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
USHORT f_trp_rec_Fc03_t_frq_get(                                /* 周波数:R */
) {
    USHORT cpr_no_idx = D_REC_C03_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT crr_no_idx = D_REC_C03_CRR_NO() - CMD_NUM1;
                                                /* キャリア番号インデックス */
    USHORT frq;                                                   /* 周波数 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************/
    /* 周波数取得 */
    /**************/
    frq = f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].frq;

    return frq;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
