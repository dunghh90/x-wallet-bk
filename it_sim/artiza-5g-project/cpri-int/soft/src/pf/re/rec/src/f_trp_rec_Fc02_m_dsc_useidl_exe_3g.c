/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_dsc_useidl_exe_3g.c
 * @brief  RE状態運用中アイドル実行処理
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
 * @brief  RE状態運用中アイドル実行処理
 * @note   RE状態運用中アイドル実行処理を行う
 *         - RE状態(セクタ部)管理テーブル初期化
 *         - プロセス間(RE状態管理テーブル初期化通知)送信
 *         - 自状態番号設定(運用中アイドル)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/20 FFCS)Shihzh modify for S3G.
 * @date   2015/11/14 TDI)satou 問処番号No.183 強制リセット処理追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_dsc_useidl_exe_3g(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    T_RRH_LAYER3 shmLayer3;
    USHORT cpr_idx;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************************************/
    /* RE状態(セクタ部)管理テーブル初期化 */
    /**************************************/
    f_trp_rec_Fc02_t_remsc_tbl_ini_3g(                     /* なし:R */
        D_REC_INK_DSC                                     /* CPRIリンク断:I */
    );

//TBD    /* RRE/LRE case */
//    if (D_REC_C02_RE_SYS_TYPE() == D_TCM_SYS_RETYP_RRELRE) {
//        /********************************************/
//        /* RE状態(共通部)管理テーブル初期化         */
//        /********************************************/
//        uit_ret |= f_trp_rec_Fc02_t_remcm_tbl_ini(D_REC_INK_NOA);
//
//        /*********************************************/
//        /* プロセス間(FLD-MT Active状態変化通知)送信 */
//        /*********************************************/
//        f_trp_rec_Fc02_t_ip_fldactntc_snd(                        /* なし:R */
//            D_REC_ACK_NEGCMP                                   /* Negtive:I */
//        );
//    }


    
//	/************************/
//	/* RE未実装強化ログ待避 */
//	/************************/
//	f_trp_com_FLog_switch();
//
    /**********************/
    /* 送信間隔タイマ取消 */
    /**********************/
    f_trp_rec_Fc02_t_inttim_ccl_3g(                                  /* なし:R */
    );

    /* 共有メモリのLayer3が起動中状態であれば強制リセットする.
     * RECとの起動シーケンスの途中でHC断が発生し、そこでRECからシスパラ配信、
     * REのHC復旧となると、起動シーケンスが行われず、RECからのシスパラが反映
     * されない. それを防止する為のリセット. */
    f_cmn_com_layer3_get(CMD_SYS_3G, &shmLayer3);
    cpr_idx = D_REC_C02_3G_CPR_NO() - 1;
    if ((E_RRH_LAYER3_RE_IDLE  == shmLayer3.layer3_re[cpr_idx])
    ||  (E_RRH_LAYER3_RE_START == shmLayer3.layer3_re[cpr_idx]))
    {   /* 起動中→強制リセット */
        /********************************/
        /* 強制リセット実行プロシージャ */
        /********************************/
        f_trp_rec_Fc02_t_it_refrcrstprc_snd_3g();

        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set_3g(                               /* なし:R */
            EC_REC_C02_STN_REFRCRSTING                        /* 強制リセット:I */
        );
    }
    else
    {   /* 運用中→運用中アイドル */
        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set_3g(                               /* なし:R */
            EC_REC_C02_STN_USE_IDL                          /* 運用中アイドル:I */
        );
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
