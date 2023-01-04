/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_refrcrst_sta_3g.c
 * @brief  RE強制リセット開始処理
 * @date   2008/07/18 FFCS)Shihzh create.
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
 * @date   2008/07/18 FFCS)Shihzh create.
 * @date   2015/10/19 TDI)satou ハソ-QA-051 二次リセット
 * @date   2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_refrcrst_sta_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    SHORT tc;                                               /* 残トライ回数 */
    CHAR errdata[CMD_NUM48];
    ET_REC_C02_STN stn;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);


    /******************/
    /* 確認タイマ取消 */
    /******************/
    f_trp_rec_Fc02_t_cnftim_ccl_3g(                                  /* なし:R */
    );


    /****************************************/
    /* 残トライ回数(RE強制リセット送信)取得 */
    /****************************************/
    tc = f_trp_rec_Fc02_t_tc_refrcrstsnd_get_3g(             /* 残トライ回数:R */
    );

    /* トライ不可能な場合 */
    if (tc <= CMD_NUM0) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[FRCSTP][cpr:%02d]RE Force Reset Sending Over.", D_REC_C02_3G_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_REFRCRSTPRCIND));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN2, errdata);
        
        /************************************************/
        /* スレッド間(強制停止プロシージャ実行指示)送信 */
        /************************************************/
        f_trp_rec_Fc02_t_it_frcstpprc_snd_3g(E_RRH_RESTOP_FACTOR_OTHER);

        /* 終了 */
        return;
    }

    /*
     * LTE側のマトリクスでHW(RE強制リセット)送信中の場合、HWに設定しない.
     * 強制リセットのレジスタは3G/LTEで共通のため二重送信防止.
     */
    stn = f_trp_rec_Fc02_t_ownstn_get();
    if (EC_REC_C02_STN_REFRCRSTING != stn)
    {
        /**************************/
        /* HW(RE強制リセット)送信 */
        /**************************/
        f_trp_rec_Fc02_t_hw_refrcrst_snd_3g(                         /* なし:R */
        );
    }

    /************************************************/
    /* 残トライ回数(RE強制リセット送信)デクリメント */
    /************************************************/
    f_trp_rec_Fc02_t_tc_refrcrstsnd_dec_3g(                          /* なし:R */
    );

    /********************************************/
    /* 残トライ回数(RE強制リセット)デクリメント */
    /********************************************/
    f_trp_rec_Fc02_t_tc_refrcrst_dec_3g(                             /* なし:R */
    );

//    /********************************************/
//    /* Active Switch (BRE/M-LRE case)           */
//    /********************************************/
//    if ((f_trp_rec_Fc02_t_ownact_get() == D_REC_ACK_ACTCMP) && 
//        (D_REC_C02_RE_SYS_TYPE() != D_TCM_SYS_RETYP_RRELRE)){
//        /**********************************/
//        /* スレッド間(Active切替指示)送信 */
//        /**********************************/
//        f_trp_rec_Fc02_t_it_actchgind_snd(                        /* なし:R */
//            CMD_CHGTRG_RERST                  /* 再active依頼通知に応じる:I */
//        );
//    }
    
    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_REFRCRSTING                    /* RE強制リセット中:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
