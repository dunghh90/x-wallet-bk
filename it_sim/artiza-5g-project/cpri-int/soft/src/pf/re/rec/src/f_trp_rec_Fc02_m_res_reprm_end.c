/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_reprm_end.c
 * @brief  RE起動中REシステムパラメータ更新終了処理
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
 * @brief  RE起動中REシステムパラメータ更新終了処理
 * @note   RE起動中REシステムパラメータ更新終了処理を行う
 *         - スレッド間(REファイル更新プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(REシステムパラメータ更新完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01700
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 * @date   2011/02/10 FJT)Tokunaga CeNB-F-062-033(DCM) 運用パラメータの反映元と反映タイミング
 * @date   2015/09/28 TDIPS)Lay M-RRU-ZSYS-01700 REファイル更新処理が開始されない不具合の修正
 * @date   2015/10/17 TDI)satou FHM技説-QA-013
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_reprm_end(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT ret_code;                                          /* 結果コード */
    CHAR  errdata[CMD_NUM48];
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 結果コード抽出 */
    ret_code = ((CMT_TSKIF_RESYSNEWRES*)bufp)->result_code;

    /* OK以外の場合 */
    if ((ret_code != CMD_OK) && (CMD_NG_PRIMARY != ret_code)) {
        snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]RE sys paramter download NG.", D_REC_C02_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_RESYSNEWRES));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

        /******************************************************/
        /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
        /******************************************************/
        f_trp_rec_Fc02_t_it_refrcrstprc_snd(                      /* なし:R */
        );
        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
            EC_REC_C02_STN_RES_REPRMCMP
                                /* RE起動中(REシステムパラメータ更新完了):I */
        );
   
        /* 終了 */
        return;
    }

    /* 配下REから優先システムがS3G以外で報告された場合、REリセットを送信 */
    if (CMD_NG_PRIMARY == ret_code) {
        /**************************************************/
        /* スレッド間(REリセットプロシージャ実行指示)送信 */
        /**************************************************/
        f_trp_rec_Fc02_t_it_rerstprc_snd();

        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
            EC_REC_C02_STN_RES_REPRMCMP
                                    /* RE起動中(REシステムパラメータ更新完了):I */
        );

        return;
    }

    if (CMD_OK == ret_code) {
/* NAGA KARI */
#ifndef D_REC_DL_SKIP
    /******************************************************/
    /* スレッド間(REファイル更新プロシージャ実行指示)送信 */
    /******************************************************/
    f_trp_rec_Fc02_t_it_refilprc_snd();

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_REPRMCMP
                                /* RE起動中(REシステムパラメータ更新完了):I */
    );
#else
    /*****************************************************/
    /* スレッド間(Toffset値報告プロシージャ実行指示)送信 */
    /*****************************************************/
    f_trp_rec_Fc02_t_it_tofvalprc_snd(                            /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_REFILCMP       /* RE起動中(REファイル更新完了):I */
    );
#endif
    }
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
