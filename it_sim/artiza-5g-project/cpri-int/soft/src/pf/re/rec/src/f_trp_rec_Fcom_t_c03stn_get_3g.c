/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_c03stn_get_3g.c
 * @brief  C03状態番号取得
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
 * @brief  C03状態番号取得
 * @note   C03状態番号を取得する
 * @param  cpr_no [in]  CPRI番号
 * @param  crr_no [in]  キャリア番号
 * @return C03状態番号
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
ET_REC_C03_STN f_trp_rec_Fcom_t_c03stn_get_3g(                /* C03状態番号:R */
    USHORT cpr_no,                                            /* CPRI番号:I */
    USHORT crr_no                                         /* キャリア番号:I */
) {
    USHORT cpr_no_idx = cpr_no - CMD_NUM1;          /* CPRI番号インデックス */
    USHORT crr_no_idx = crr_no - CMD_NUM1;      /* キャリア番号インデックス */
    ET_REC_C03_STN stn;                                      /* C03状態番号 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*******************/
    /* C03状態番号取得 */
    /*******************/
    stn = f_trp_rec_Wc03_stsmng_tbl_3g.sts[cpr_no_idx][crr_no_idx].stn;

    return stn;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
