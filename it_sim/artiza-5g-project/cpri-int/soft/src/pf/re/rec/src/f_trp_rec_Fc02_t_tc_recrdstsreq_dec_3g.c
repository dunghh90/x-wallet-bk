/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_recrdstsreq_dec.c
 * @brief  残トライ回数(REカード状態報告要求)デクリメント
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
 * @brief  残トライ回数(REカード状態報告要求)デクリメント
 * @note   残トライ回数(REカード状態報告要求)をデクリメントする
 *         - 残トライ回数デクリメント
 * @param  slt_no [in]  スロット番号
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_recrdstsreq_dec_3g(                         /* なし:R */
    USHORT slt_no                                         /* スロット番号:I */
) {
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT slt_no_idx = slt_no - CMD_NUM1;      /* スロット番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************************/
    /* 残トライ回数デクリメント */
    /****************************/
    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].tc.recrdstsreq[slt_no_idx]--;

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
