/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_mtsetffactor.c
 * @brief  MTアドレス設定開始要因
 * @date   2015/07/30 FPT)Hieu create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  MTアドレス設定開始要因設定
 * @note   MTアドレス設定開始要因設定
 * @param  awCpriNo [in]  CPRI number  
 * @param  awFactor [in]  MTアドレス設定開始要因
 * @return -
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/22 FPT)Yen update
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_mtsetffactor_set_3g(USHORT awCpriNo, USHORT awFactor)
{
    USHORT cpr_no_idx = awCpriNo - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* 残トライ回数 設定*/
    /************************/
    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].mtsetaddfactor =  awFactor;
}

/****************************************************************************/
/*!
 * @brief  MTアドレス設定開始要因取得
 * @note   MTアドレス設定開始要因取得
 *   
 * @param  awCpriNo [in] CPRI number
 * @return MTアドレス設定開始要因
 * @date   2015/07/30 FPT)Hieu create.
 * @date   2015/09/22 FPT)Yen update
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_tc_mtsetffactor_get_3g(USHORT awCpriNo)
{
    USHORT cpr_no_idx = awCpriNo - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* return MTアドレス設定開始要因*/
    /************************/
    return f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].mtsetaddfactor;
}


/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
