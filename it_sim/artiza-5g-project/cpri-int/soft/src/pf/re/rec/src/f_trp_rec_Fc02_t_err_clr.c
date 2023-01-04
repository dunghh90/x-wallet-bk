/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_err_clr.c
 * @brief  ERR設定
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
 * @brief  ERR解除
 * @note   ERRを解除する
 *         - レジスタ書き込み
 * @param  erk [in]  ERR種別
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2010/01/05 QNET)Nagasima M-S3G-eNBPF-02937.
 * @date   2010/01/20 QNET)Nagasima M-S3G-eNBPF-02937.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_err_clr(                                    /* なし:R */
    ET_REC_ERK erk                                             /* ERR種別:I */
) {
#if 0
	USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    UINT  uit_ret;                                            /* 終了コード */
    UINT  red_val;                                    /* 読み出しレジスタ値 */
    UINT  wrt_bit;                                    /* 書き込みレジスタ値 */
    CHAR errdata[CMD_NUM48];
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 書き込みレジスタビット取得 */
    wrt_bit = f_trp_rec_Rcom_erf_tbl.fct[erk][cpr_no_idx];

    /********************/
    /* レジスタ読み出し */
    /********************/
    uit_ret = f_trp_rec_Fcom_t_nrg_red(                     /* 終了コード:R */
        EC_REC_NRK_ERS,                                   /* レジスタ種別:I */
        &red_val                                            /* レジスタ値:O */
    );
    /* NGの場合 */
    if (uit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]Err clr register read Err.", D_REC_C02_CPR_NO(), uit_ret);
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
    }

    /* ERR解除済みの場合 */
    if (D_REC_GETBIT(red_val, wrt_bit) == CMD_OFF) {
        /* 終了 */
        return;
    }

    /********************/
    /* レジスタ書き込み */
    /********************/
    uit_ret = f_trp_rec_Fcom_t_nrg_wrt(                     /* 終了コード:R */
        EC_REC_NRK_ERC,                                   /* レジスタ種別:I */
        wrt_bit                                             /* レジスタ値:I */
        /* D_CMM_BIT_CNT_HARDERR36(CPRI#1 遅延パラメータ異常) */
        /* D_CMM_BIT_CNT_HARDERR40(CPRI#2 遅延パラメータ異常) */
        /* D_CMM_BIT_CNT_HARDERR44(CPRI#3 遅延パラメータ異常) */
        /* D_CMM_BIT_CNT_HARDERR48(CPRI#4 遅延パラメータ異常) */
        /* D_CMM_BIT_CNT_HARDERR52(CPRI#5 遅延パラメータ異常) */
        /* D_CMM_BIT_CNT_HARDERR56(CPRI#6 遅延パラメータ異常) */
        /* D_CMM_BIT_CNT_HARDERR57(CPRI#1 強制停止状態)       */
        /* D_CMM_BIT_CNT_HARDERR58(CPRI#2 強制停止状態)       */
        /* D_CMM_BIT_CNT_HARDERR59(CPRI#3 強制停止状態)       */
        /* D_CMM_BIT_CNT_HARDERR60(CPRI#4 強制停止状態)       */
        /* D_CMM_BIT_CNT_HARDERR61(CPRI#5 強制停止状態)       */
        /* D_CMM_BIT_CNT_HARDERR62(CPRI#6 強制停止状態)       */
    );
    /* NGの場合 */
    if (uit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]Err clr register write Err.", D_REC_C02_CPR_NO(), uit_ret);
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
    }
#endif
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
