/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_nrg_red.c
 * @brief  レジスタ読み出し
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
 * @brief  レジスタ読み出し
 * @note   レジスタを読み出す
 *         - メモリ読み出し
 * @param  nrk    [in]  レジスタ種別
 * @param  valp   [out] レジスタ値
 * @return 終了コード
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/08/20 FFCS)Shihzh modify for S3G PRC.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_nrg_red(                              /* 終了コード:R */
    ET_REC_NRK nrk,                                       /* レジスタ種別:I */
    UINT*   valp                                            /* レジスタ値:O */
) {
    UINT   uit_ret;                                           /* 終了コード */
    UINT*  nra;                                         /* レジスタアドレス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* レジスタアドレス設定 */
    nra = f_trp_rec_Rcom_nra_tbl.adr[nrk];
    /******************/
    /* メモリ読み出し */
    /******************/
    uit_ret = BPF_HM_DEVC_REG_READ(
    	D_RRH_LOG_REG_LV_READ,
        (UINT)nra,                                      /* メモリアドレス:I */
        (VOID*)valp                                       /* メモリデータ:O */
    );

    return uit_ret;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
