/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_2nd_rdy.c
 * @brief  二次リセット準備処理
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
 * @brief  二次リセット準備処理
 * @note   二次リセット準備処理を行う
 *         - 自状態番号設定(二次リセット準備完了)
 *           - 他に二次リセット準備中がある場合
 *             - 処理終了
 *         - スレッド間(二次リセットプロシージャ実行指示)送信
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2010/11/12 FJT)Tokunaga M-S3G-eNBSYS-xxxxx [TS-810項]
 * @date   2015/10/19 TDI)satou ハソ-QA-051 二次リセット
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_2nd_rdy(                                    /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_2ND_RDYCMP                 /* 二次リセット準備完了:I */
    );

    /******************************************************/
    /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
    /******************************************************/
    f_trp_rec_Fc02_t_it_refrcrstprc_snd(                          /* なし:R */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/