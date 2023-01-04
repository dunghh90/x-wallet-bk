/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_refrcrst_get_3g.c
 * @brief  残トライ回数(RE強制リセット)取得
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
 * @brief  残トライ回数(RE強制リセット)取得
 * @note   残トライ回数(RE強制リセット)を取得する
 *         - 残トライ回数リターン
 * @param  -
 * @return 残トライ回数
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2018/08/28 FPT)Yen create
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_tc_refrcrst_get_3g(                  /* 残トライ回数:R */
) {
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* 残トライ回数リターン */
    /************************/
    return f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].tc.refrcrst;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
