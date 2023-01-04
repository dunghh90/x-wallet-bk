/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_refrcrst_dec_3g.c
 * @brief  残トライ回数(RE強制リセット)デクリメント
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
 * @brief  残トライ回数(RE強制リセット)デクリメント
 * @note   残トライ回数(RE強制リセット)をデクリメントする
 *         - 残トライ回数デクリメント
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/08/28 FPT)Yen create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_refrcrst_dec_3g(                            /* なし:R */
) {
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************************/
    /* 残トライ回数デクリメント */
    /****************************/
    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].tc.refrcrst--;

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
