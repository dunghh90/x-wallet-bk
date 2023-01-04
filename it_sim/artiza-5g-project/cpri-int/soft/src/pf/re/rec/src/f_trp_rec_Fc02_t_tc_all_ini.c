/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_all_ini.c
 * @brief  残トライ回数初期化(RE強制リセット送信含む)
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
 * @brief  残トライ回数初期化(RE強制リセット送信含む)
 * @note   残トライ回数を初期化する(RE強制リセット送信含む)
 *         - 残トライ回数(RE強制リセット送信)初期化
 *         - 残トライ回数(RE強制リセット送信以外)初期化
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_all_ini(                                 /* なし:R */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /******************************************/
    /* 残トライ回数(RE強制リセット送信)初期化 */
    /******************************************/
    f_trp_rec_Fc02_t_tc_refrcrstsnd_ini(                          /* なし:R */
    );

    /**********************************************/
    /* 残トライ回数(RE強制リセット送信以外)初期化 */
    /**********************************************/
    f_trp_rec_Fc02_t_tc_ini(                                      /* なし:R */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
