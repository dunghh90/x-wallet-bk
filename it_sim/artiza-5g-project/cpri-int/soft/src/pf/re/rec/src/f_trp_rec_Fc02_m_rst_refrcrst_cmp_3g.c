/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_rst_refrcrst_cmp_3g.c
 * @brief  RE強制リセット完了処理
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
 * @brief  RE強制リセット完了処理
 * @note   RE強制リセット完了処理を行う
 *         - 自状態番号設定(RE起動中(LAPBリンク確立前))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01760
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/09/17 TDI)satou 状態遷移先を(04)RE起動待ちアイドルに
 * @date   2015/10/12 FPT)Yen M-RRU-ZSYS-01760 update (clear MT status)
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_rst_refrcrst_cmp_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************/
    /* タイマ取消 */
    /**************/
    f_trp_rec_Fc02_t_timer_ccl_3g(                                   /* なし:R */
        EC_REC_TMK_REFRCRST_CNF,              /* RE強制リセット確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    /**************************************/
    /* 残トライ回数(RE強制リセット)初期化 */
    /**************************************/
    f_trp_rec_Fc02_t_tc_refrcrst_ini_3g(                             /* なし:R */
    );

	/*Set MTアドレス設定状態←未処理*/
	f_trp_rec_Fc02_t_mtsetsts_3g( D_REC_C02_3G_CPR_NO(), EC_REC_MTADDSET_STN_INI );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_IDL                  /* (04)RE起動待ちアイドル:I    */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
