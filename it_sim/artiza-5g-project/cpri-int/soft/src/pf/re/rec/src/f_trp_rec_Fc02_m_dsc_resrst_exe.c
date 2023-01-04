/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_dsc_resrst_exe.c
 * @brief  RE状態起動待ちアイドル,RE状態起動中CPRIリンク断処理
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE状態起動待ちアイドル,RE状態起動中CPRIリンク断処理
 * @note   RE状態起動待ちアイドル,RE状態起動中CPRIリンク断処理を行う
 *         - RE強制リセットプロシージャ実行指示送信
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_dsc_resrst_exe(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    ET_REC_C02_STN stn;                                       /*  own state */
   
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /******************/
    /* 確認タイマ取消 */
    /******************/
    f_trp_rec_Fc02_t_cnftim_ccl(                                  /* なし:R */
    );

    /*****************/
    /* Get Own state */
    /*****************/
    stn = f_trp_rec_Fc02_t_ownstn_get(                            /* None:R */
    );
    if ((stn == EC_REC_C02_STN_RES_REPRMING) ||
        (stn == EC_REC_C02_STN_RES_REFILING) ){
        /*************************************/
        /* RE更新停止指示                    */
        /*************************************/
        f_trp_rec_Fc02_t_it_redlstpind_snd(                       /* なし:R */
            CMD_DLSTP_DISC                                        /* Fact:I */
        );
    }

    /**************************************/
    /* RE状態(セクタ部)管理テーブル初期化 */
    /**************************************/
    f_trp_rec_Fc02_t_remsc_tbl_ini(                     /* なし:R */
        D_REC_INK_DSC                                     /* CPRIリンク断:I */
    );

    /******************************************************/
    /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
    /******************************************************/
    f_trp_rec_Fc02_t_it_refrcrstprc_snd(                         /* なし:R */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
