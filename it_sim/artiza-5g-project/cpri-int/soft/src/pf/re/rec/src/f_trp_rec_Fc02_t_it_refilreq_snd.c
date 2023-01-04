/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_it_refilreq_snd.c
 * @brief  スレッド間(REファイル更新要求)送信
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2009
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  スレッド間(REファイル更新要求)送信
 * @note   スレッド間(REファイル更新要求)を送信する
 *         - スレッド間(REファイル更新要求)送信
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2009/03/04 FFCS)Shihzh CR-00031-005.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_refilreq_snd(                            /* なし:R */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************************************/
    /* スレッド間(REファイル更新要求)送信 */
    /**************************************/
    f_trp_rec_Fcom_t_it_refilreq_snd(                             /* なし:R */
        D_REC_C02_CPR_NO(),                                   /* CPRI番号:I */
        CMD_SYS_LTE,                                          /* SDM Side:I */
        CMD_FILUP_TYPE_STA                               /* Download Kind:I */
    );

    return;
}


/****************************************************************************/
/*!
 * @brief  スレッド間(REファイル更新要求)(3G)送信
 * @note   スレッド間(REファイル更新要求)(3G)を送信する
 *         - スレッド間(REファイル更新要求)送信
 * @param  -
 * @return None
 * @date   2015/10/08 FPT)Duong Create ( MKレビュー指摘No.243対応)
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_refilreq_snd_3g(                         /* なし:R */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************************************/
    /* スレッド間(REファイル更新要求)送信 */
    /**************************************/
    f_trp_rec_Fcom_t_it_refilreq_snd(                             /* なし:R */
        D_REC_C02_3G_CPR_NO(),                                   /* CPRI番号:I */
        CMD_SYS_3G,                                           /* SDM Side:I */
        CMD_FILUP_TYPE_STA                               /* Download Kind:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
