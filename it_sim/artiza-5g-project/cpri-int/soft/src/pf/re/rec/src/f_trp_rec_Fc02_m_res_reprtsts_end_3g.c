/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_reprtsts_end_3g.c
 * @brief  起動中REスロット情報報告終了処理
 * @date   2015/07/30 FPT)DuongCD create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  起動中RE PORT状態報告終了処理
 * @note   起動中RE PORT状態報告終了処理を行う
 *         - RE状態(共通部)管理テーブル(RE状態情報)設定
 *         - スレッド間(REカード状態報告(セクタ部)要求プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(RE PORT状態報告完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 * @TBD_No 修正未完了
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_reprtsts_end_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    UINT				uit_code;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************/
    /* タイマ取消 */
    /**************/
    uit_code = f_trp_rec_Fc02_t_timer_ccl_3g(                        /* なし:R */
        EC_REC_TMK_REPRTSTS_CNF,             /* RE PORT状態報告確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );
    
    if (uit_code == D_REC_TIM_IV) {
        return;
    }

    /**********************/
    /* 残トライ回数初期化 */
    /**********************/
    f_trp_rec_Fc02_t_use_reprtstsreq_ini_3g(                        /* なし:R */
    );

    /**********************************************************************/
    /* スレッド間(REカード状態報告(セクタ部)要求プロシージャ実行指示)送信 */
    /**********************************************************************/
    f_trp_rec_Fc02_t_it_recrdsscprc_snd_3g(                          /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_REPRTSTSCMP   /* RE起動中(RE PORT状態報告完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/