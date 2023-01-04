/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_rests_end.c
 * @brief  運用中RE PORT状態報告終了処理
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
 * @brief  運用中RE状態報告終了処理
 * @note   運用中RE状態報告終了処理を行う
 *         - RE状態(共通部)管理テーブル(RE状態情報)設定
 *         - スレッド間(RE PORT状態報告要求プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(RE状態報告完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_rests_end(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    VOID*  datp;
    VOID*  l3_bufp;                                   /* L3バッファポインタ */
    USHORT ret_code;                                          /* 結果コード */
    UINT   uit_code;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__ );
#endif

    /* L3バッファポインタ抽出 */
    datp    = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    l3_bufp = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    /* 結果コード抽出 */
    ret_code = ((CMT_CPRIF_RESTSRES*)l3_bufp)->result;

    /**************/
    /* タイマ取消 */
    /**************/
    uit_code = f_trp_rec_Fc02_t_timer_ccl(                        /* なし:R */
        EC_REC_TMK_RESTS_CNF,                     /* RE状態報告確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    if (uit_code == D_REC_TIM_IV) {
        return;
    }

    if (ret_code == CMD_NML) { 
        ret_code |= f_trp_rec_Fc02_t_cprpar_chk(datp);
    }
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d <ret_code>%d\n", __FILE__, __FUNCTION__, __LINE__,ret_code );
#endif
    /* OK以外の場合 */
    if (ret_code != CMD_OK) {
        /**********************************************************/
        /* スレッド間(RE状態報告プロシージャ実行指示)送信         */
        /**********************************************************/
        f_trp_rec_Fc02_t_it_restsprc_snd(                         /* なし:R */
        );

        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
            EC_REC_C02_STN_RES_RESLTINFCMP 
                                      /* RE起動中(REスロット情報報告完了):I */
        );

        /* 終了 */
        return;
    }

    /***************************************************/
    /* RE状態(共通部)管理テーブル(RE状態情報)設定      */
    /***************************************************/
    f_trp_rec_Fc02_t_rests_set(                                   /* 変化:R */
        l3_bufp                                     /* L3バッファポインタ:I */
    );

    /****************************************************************/
    /* スレッド間(RE PORT状態報告要求プロシージャ実行指示)送信      */
    /****************************************************************/
    f_trp_rec_Fc02_t_it_reprtstsprc_snd(                          /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_RESTSCMP           /* RE起動中(RE状態報告完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
