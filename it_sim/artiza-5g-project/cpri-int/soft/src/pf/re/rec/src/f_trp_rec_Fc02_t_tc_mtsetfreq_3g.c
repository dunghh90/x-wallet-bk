/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_mtsetfreq.c
 * @brief  残トライ回数(RE装置構成情報報告要求)
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
 * @brief  残トライ回数(RE装置構成情報報告要求)設定
 * @note   残トライ回数(RE装置構成情報報告要求)を設定する
 *         - 残トライ回数リターン
 * @param  awRetryCnt [in]  Retry count
 * @return -
 * @date   2015/07/30 FPT)Hieu create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_mtsetfreq_set_3g( USHORT awRetryCnt)
{
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* 残トライ回数 設定*/
    /************************/
    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].tc.mtaddsetcnt =  awRetryCnt;
}


/****************************************************************************/
/*!
 * @brief  残トライ回数(RE装置構成情報報告要求)取得
 * @note   残トライ回数(RE装置構成情報報告要求)を取得する
 *         - 残トライ回数リターン
 * @param  -
 * @return 残トライ回数
 * @date   2015/07/30 FPT)Hieu create.
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_tc_mtsetfreq_get_3g() 
{
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* 残トライ回数リターン */
    /************************/
    return f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].tc.mtaddsetcnt;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
