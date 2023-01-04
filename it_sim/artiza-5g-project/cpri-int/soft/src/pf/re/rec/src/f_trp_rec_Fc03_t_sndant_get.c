/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_t_sndant_get.c
 * @brief  送信アンテナ数取得
 * @date   2008/07/20 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  送信アンテナ数取得
 * @note   送信アンテナ数を取得する
 *         - 送信アンテナ数取得
 * @param  -
 * @return 送信アンテナ数
 * @date   2008/07/20 FFCS)Shihzh create.
 */
/****************************************************************************/
USHORT f_trp_rec_Fc03_t_sndant_get(                     /* 送信アンテナ数:R */
) {
    USHORT cpr_no_idx = D_REC_C03_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT crr_no_idx = D_REC_C03_CRR_NO() - CMD_NUM1;
                                                /* キャリア番号インデックス */
    USHORT sndant;                                        /* 送信アンテナ数 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************/
    /* 送信アンテナ数取得 */
    /**********************/
    sndant = f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].sndant;

    return sndant;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
