/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_rerstreq_ini_3g.c
 * @brief  残トライ回数(REリセット)初期化
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
 * @brief  残トライ回数(REリセット)初期化
 * @note   残トライ回数(REリセット)を初期化する
 *         - 残トライ回数初期化
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_rerstreq_ini_3g(                            /* なし:R */
) {
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************/
    /* 残トライ回数初期化 */
    /**********************/
    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].tc.rerst 
    = CMD_NUM1;

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
