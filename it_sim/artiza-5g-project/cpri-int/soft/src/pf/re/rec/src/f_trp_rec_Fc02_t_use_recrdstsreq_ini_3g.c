/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_use_recrdstsreq_ini_3g.c
 * @brief  残トライ回数(REカード状態報告要求)初期化
 * @date   2010/08/11 CMS)Takahashi create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  残トライ回数(REカード状態報告要求)初期化
 * @note   残トライ回数(REカード状態報告要求)を初期化する
 *         - 残トライ回数初期化
 * @param  slt_no [in]  スロット番号
 * @return None
 * @date   2010/08/11 CMS)Takahashi create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_use_recrdstsreq_ini_3g(                        /* なし:R */
    USHORT slt_no                                         /* スロット番号:I */
) {
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT slt_no_idx = slt_no - CMD_NUM1;      /* スロット番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************/
    /* 残トライ回数初期化 */
    /**********************/
    /* set default value */
    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].tc.m_recrdstsreq[slt_no_idx]
        = D_REC_RECRDSTSREQ_RTNUM - CMD_NUM1;

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
