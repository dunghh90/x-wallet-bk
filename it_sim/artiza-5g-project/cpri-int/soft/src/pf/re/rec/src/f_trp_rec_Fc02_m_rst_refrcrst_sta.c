/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_rst_refrcrst_sta.c
 * @brief  RE強制リセット開始処理
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
 * @brief  RE強制リセット開始処理
 * @note   RE強制リセット開始処理を行う
 *         - HW(RE強制リセット)送信
 *         - 自状態番号設定(RE起動中(RE強制リセット中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_rst_refrcrst_sta(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    SHORT tc;                                               /* 残トライ回数 */
    CHAR errdata[CMD_NUM48];

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************************/
    /* 残トライ回数(RE強制リセット)取得 */
    /************************************/
    tc = f_trp_rec_Fc02_t_tc_refrcrst_get(                /* 残トライ回数:R */
    );

    /* トライ不可能な場合 */
    if (tc <= CMD_NUM0) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[FRCSTP][cpr:%02d]Force reset NG.", D_REC_C02_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_REFRCRSTPRCIND));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

        /************************************************/
        /* スレッド間(強制停止プロシージャ実行指示)送信 */
        /************************************************/
        f_trp_rec_Fc02_t_it_frcstpprc_snd(E_RRH_RESTOP_FACTOR_OTHER);

        /* 終了 */
        return;
    }

    /**************************/
    /* HW(RE強制リセット)送信 */
    /**************************/
    f_trp_rec_Fc02_t_hw_refrcrst_snd(                             /* なし:R */
    );

    /********************************************/
    /* 残トライ回数(RE強制リセット)デクリメント */
    /********************************************/
    f_trp_rec_Fc02_t_tc_refrcrst_dec(                             /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_REFRCRSTING                    /* RE強制リセット中:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
