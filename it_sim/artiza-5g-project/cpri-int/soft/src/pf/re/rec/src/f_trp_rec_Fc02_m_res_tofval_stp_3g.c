/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_tofval_stp_3g.c
 * @brief  運用中Toffset値報告中止処理
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
 * @brief  運用中Toffset値報告中止処理
 * @note   運用中Toffset値報告中止処理を行う
 *         - スレッド間(Toffset値報告プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(REファイル更新完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01821
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 * @date   2015/10/21 TDI)satou M-RRU-ZSYS-01821 問処番号No.123
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_tofval_stp_3g(                          /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*****************************************************/
    /* スレッド間(Toffset値報告プロシージャ実行指示)送信 */
    /*****************************************************/
    f_trp_rec_Fc02_t_it_tofvalprc_snd_3g(                          /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                               /* なし:R */
        EC_REC_C02_STN_RES_REFILCMP       /* RE起動中(REファイル更新完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
