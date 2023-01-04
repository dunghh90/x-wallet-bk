/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_retimset_exe_3g.c
 * @brief  RE起動中RE時刻設定実行処理
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2021/02/08 M&C)Huan.dh intermediate disable 3G RE time setting
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE起動中RE時刻設定実行処理
 * @note   RE起動中RE時刻設定実行処理を行う
 *         - L3(RE時刻設定要求)送信
 *         - スレッド間(REシステムパラメータ更新プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(RE時刻設定完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_retimset_exe_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************************/
    /* L3(RE時刻設定要求)送信 */
    /**************************/
    // f_trp_rec_Fc02_t_l3_retimsetreq_snd_3g(                          /* なし:R */
    // );

/* NAGA KARI */
#ifndef D_REC_DL_SKIP
    /****************************************************************/
    /* スレッド間(REシステムパラメータ更新プロシージャ実行指示)送信 */
    /****************************************************************/
    f_trp_rec_Fc02_t_it_reprmprc_snd_3g(                             /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_RETIMSETCMP        /* RE起動中(RE時刻設定完了):I */
    );
#else
    /*****************************************************/
    /* スレッド間(Toffset値報告プロシージャ実行指示)送信 */
    /*****************************************************/
    f_trp_rec_Fc02_t_it_tofvalprc_snd_3g(                            /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_REFILCMP       /* RE起動中(REファイル更新完了):I */
    );
#endif

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
