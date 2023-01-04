/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_rst_refrcrst_stp.c
 * @brief  RE強制リセット中止処理
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
 * @brief  RE強制リセット中止処理
 * @note   RE強制リセット中止処理を行う
 *         - スレッド間(RE強制リセットプロシージャ実行指示)送信
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_rst_refrcrst_stp(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    CHAR  errdata[CMD_NUM48];

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d]RE Force Reset confirm T.O.", D_REC_C02_CPR_NO());
    D_REC_IVLINI();
    D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
    D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

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
