/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_c03stn_set.c
 * @brief  C03状態番号設定
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
 * @brief  C03状態番号設定
 * @note   C03状態番号を設定する
 * @param  cpr_no [in]  CPRI番号
 * @param  crr_no [in]  キャリア番号
 * @param  stn    [in]  C03状態番号
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_c03stn_set(                                 /* なし:R */
    USHORT cpr_no,                                            /* CPRI番号:I */
    USHORT crr_no,                                        /* キャリア番号:I */
    ET_REC_C03_STN stn                                       /* 03F状態番号 */
) {
    USHORT cpr_no_idx = cpr_no - CMD_NUM1;          /* CPRI番号インデックス */
    USHORT crr_no_idx = crr_no - CMD_NUM1;      /* キャリア番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*******************/
    /* C03状態番号設定 */
    /*******************/
    f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].stn = stn;

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
