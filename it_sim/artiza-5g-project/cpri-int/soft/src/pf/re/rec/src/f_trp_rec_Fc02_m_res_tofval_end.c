/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_tofval_end.c
 * @brief  運用中Toffset値報告終了処理
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
 * @brief  運用中Toffset値報告終了処理
 * @note   運用中Toffset値報告終了処理を行う
 *         - RE状態(セクタ部)管理テーブル(Toffset値)設定
 *         - スレッド間(Active設定プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(Toffset値報告完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/10/19 TDI)satou 問処番号No.62
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_tofval_end(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    VOID*  datp;
    VOID*  l3_bufp;                                   /* L3バッファポインタ */
    USHORT ret_code;                                          /* 結果コード */
    USHORT uit_ret;
    
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__ );
#endif

    /* L3バッファポインタ抽出 */
    datp    = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    l3_bufp = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    /* 結果コード抽出 */
    ret_code = ((CMT_CPRIF_TOFFSETINFRES*)l3_bufp)->result;
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s ret_code:%d \n",__LINE__,__FUNCTION__,ret_code);
#endif
    /**************/
    /* タイマ取消 */
    /**************/
    uit_ret = f_trp_rec_Fc02_t_timer_ccl(                         /* なし:R */
        EC_REC_TMK_TOFVAL_CNF,                 /* Toffset値報告確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    if (uit_ret == D_REC_TIM_IV) {
        return;
    }
    
    /* OK以外の場合 */
    if (ret_code != CMD_NML) {
        /*****************************************************/
        /* スレッド間(Toffset値報告プロシージャ実行指示)送信 */
        /*****************************************************/
        f_trp_rec_Fc02_t_it_tofvalprc_snd(                        /* なし:R */
        );

        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
            EC_REC_C02_STN_RES_REFILCMP   /* RE起動中(REファイル更新完了):I */
        );

        /* 終了 */
        return;
    }

    /***********************************************/
    /* RE状態(セクタ部)管理テーブル(Toffset値)設定 */
    /***********************************************/
    f_trp_rec_Fc02_t_tofval_set(                                  /* 変化:R */
        l3_bufp                                     /* L3バッファポインタ:I */
    );

    /**********************************************************/
    /* スレッド間(RE装置構成情報報告プロシージャ実行指示)送信 */
    /**********************************************************/
    f_trp_rec_Fc02_t_it_reeqpinfprc_snd(                        /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_TOFVALCMP       /* RE起動中(Toffset値報告完了):I */
    );
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s End \n",__LINE__,__FUNCTION__);
#endif
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
