/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_it_redlstpind_snd.c
 * @brief  スレッド間(RE更新停止指示)送信
 * @date   2009/09/28 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2009
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  スレッド間(RE更新停止指示)送信
 * @note   スレッド間(RE更新停止指示)を送信する
 *         - バッファ取得
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  fact     [in]  Fact
 * @return None
 * @date   2009/09/28 FFCS)Shihzh create.
 * @date   2015/10/15 FPT)Duong update comment 291.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_redlstpind_snd(                          /* なし:R */
    USHORT fact                                                   /* Fact:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************************************/
    /* スレッド間(RE更新停止指示)送信 */
    /**************************************/
    f_trp_rec_Fcom_t_it_redlstpind_snd(                           /* なし:R */
        D_REC_C02_CPR_NO(),                                   /* CPRI番号:I */
        fact,                                                     /* Fact:I */
    	CMD_SYS_S3G
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
