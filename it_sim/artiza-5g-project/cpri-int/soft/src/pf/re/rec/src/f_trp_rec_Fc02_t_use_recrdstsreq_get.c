/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_use_recrdstsreq_get.c
 * @brief  残トライ回数(REカード状態報告要求)取得
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
 * @brief  残トライ回数(REカード状態報告要求)取得
 * @note   残トライ回数(REカード状態報告要求)を取得する
 *         - 残トライ回数リターン
 * @param  slt_no [in]  スロット番号
 * @return 残トライ回数
 * @date   2010/08/11 CMS)Takahashi create.
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_use_recrdstsreq_get(              /* 残トライ回数:R */
    USHORT slt_no                                         /* スロット番号:I */
) {
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT slt_no_idx = slt_no - CMD_NUM1;      /* スロット番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
    
    /************************/
    /* 残トライ回数リターン */
    /************************/
    return f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.m_recrdstsreq[slt_no_idx];
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
