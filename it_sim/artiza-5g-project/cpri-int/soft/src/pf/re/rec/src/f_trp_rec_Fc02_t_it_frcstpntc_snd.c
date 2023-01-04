/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_it_frcstpntc_snd.c
 * @brief  スレッド間(強制停止通知)送信
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
 * @brief  スレッド間(強制停止通知)送信
 * @note   スレッド間(強制停止通知)を送信する
 *         - スレッド間(強制停止通知)送信
 * @param  thrino     [in] スレッド内部番号番号
 * @param  stopFactor [in] 強制停止要因
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_frcstpntc_snd(                           /* なし:R */
    USHORT thrino,                                /* スレッド内部番号番号:I */
    T_RRH_RESTOP_FACTOR stopFactor
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /********************************/
    /* スレッド間(強制停止通知)送信 */
    /********************************/
    f_trp_rec_Fcom_t_it_frcstpntc_snd(                            /* なし:R */
        thrino,                                   /* スレッド内部番号番号:I */
        D_REC_C02_CPR_NO(),                                   /* CPRI番号:I */
        stopFactor
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
