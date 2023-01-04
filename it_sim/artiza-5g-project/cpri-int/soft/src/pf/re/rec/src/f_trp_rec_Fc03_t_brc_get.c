/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_t_brc_get.c
 * @brief  送信系取得
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
 * @brief  送信系取得
 * @note   送信系を取得する
 *         - 送信系取得
 * @param  -
 * @return 送信系
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
USHORT f_trp_rec_Fc03_t_brc_get(                                /* 送信系:R */
) {
    USHORT cpr_no_idx = D_REC_C03_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT crr_no_idx = D_REC_C03_CRR_NO() - CMD_NUM1;
                                                /* キャリア番号インデックス */
    USHORT brc;                                                   /* 送信系 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************/
    /* システム帯域幅取得 */
    /**********************/
    brc = f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].brc;

    return brc;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
