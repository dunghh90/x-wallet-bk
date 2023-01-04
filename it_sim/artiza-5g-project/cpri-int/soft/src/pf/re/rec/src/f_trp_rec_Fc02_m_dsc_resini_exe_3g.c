/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_dsc_resini_exe_3g.c
 * @brief  RE状態初期化実行処理
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
 * @brief  RE状態初期化実行処理
 * @note   RE状態初期化実行処理を行う
 *         - RE状態(セクタ部)管理テーブル初期化
 *         - 自状態番号設定(LAPBリンク確立前)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/21 FFCS)Shihzh modify for S3G PRC.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_dsc_resini_exe_3g(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	ET_REC_C02_STN stn;                                       /*  own state */
//    USHORT dlyoft;                          /* Delay Offset Set Wait Status */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*****************/
    /* Get Own state */
    /*****************/
    stn = f_trp_rec_Fc02_t_ownstn_get_3g(                            /* None:R */
    );
    if ((stn == EC_REC_C02_STN_RES_REPRMING) ||
        (stn == EC_REC_C02_STN_RES_REFILING) ){
        /*************************************/
        /* RE更新停止指示                    */
        /*************************************/
        f_trp_rec_Fc02_t_it_redlstpind_snd_3g(                       /* なし:R */
            CMD_DLSTP_DISC                                        /* Fact:I */
        );
    }

    /**************************************/
    /* RE状態(セクタ部)管理テーブル初期化 */
    /**************************************/
    f_trp_rec_Fc02_t_remsc_tbl_ini_3g(                     /* なし:R */
        D_REC_INK_DSC                                     /* CPRIリンク断:I */
    );

//    /* RRE/LRE case */
//    if (D_REC_C02_RE_SYS_TYPE() == D_TCM_SYS_RETYP_RRELRE) {
//        /********************************************/
//        /* RE状態(共通部)管理テーブル初期化         */
//        /********************************************/
//        uit_ret |= f_trp_rec_Fc02_t_remcm_tbl_ini(
//            D_REC_INK_NOA
//        );
//
//        /*********************************************/
//        /* プロセス間(FLD-MT Active状態変化通知)送信 */
//        /*********************************************/
//        f_trp_rec_Fc02_t_ip_fldactntc_snd(                        /* なし:R */
//            D_REC_ACK_NEGCMP                                   /* Negtive:I */
//        );
//    }

    /******************/
    /* 確認タイマ取消 */
    /******************/
    f_trp_rec_Fc02_t_cnftim_ccl_3g(                                  /* なし:R */
    );
    
//    /********************************/
//    /* 遅延補正値再設定待ち状態取得 */
//    /********************************/
//    dlyoft = f_trp_rec_Fc02_t_tc_dlyoft_get(               /* Wait Status:R */
//    );
//    if (dlyoft == CMD_ON) {
//        /**************/
//        /* タイマ取消 */
//        /**************/
//        f_trp_rec_Fc02_t_timer_ccl(                               /* なし:R */
//            EC_REC_TMK_T14RTY_WAT,            /* 遅延補正値設定待ちタイマ:I */
//            D_REC_SLT_IV                                  /* スロット番号:I */
//        );
//        /********************************/
//        /* 遅延補正値再設定待ち状態設定 */
//        /********************************/
//        f_trp_rec_Fc02_t_tc_dlyoft_set(
//            CMD_OFF
//        );    
//    }

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_IDL                         /* LAPBリンク確立前:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
