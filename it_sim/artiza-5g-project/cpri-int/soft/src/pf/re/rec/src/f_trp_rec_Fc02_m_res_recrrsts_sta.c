/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_recrrsts_sta.c
 * @brief  RE起動中キャリア状態報告開始処理
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
 * @brief  RE起動中キャリア状態報告開始処理
 * @note   RE起動中キャリア状態報告開始処理を行う
 *         - スレッド間((REキャリア状態報告プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(REキャリア状態報告中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/17 FFCS)Shihzh modify for S3G PRC.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_recrrsts_sta(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT tc;                                              /* 残トライ回数 */
    CHAR  errdata[CMD_NUM48];
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    /********************************************/
    /* 残トライ回数(REキャリア状態報告要求)取得 */
    /********************************************/
    tc = f_trp_rec_Fc02_t_tc_recrrstsreq_get(             /* 残トライ回数:R */
    );
    
    /* REキャリア状態報告要求トライ不可能な場合 */
    if (tc <= CMD_NUM0) {
        /****************************************/
        /* 残トライ回数(RE強制リセット送信)取得 */
        /****************************************/
        tc = f_trp_rec_Fc02_t_tc_refrcrstsnd_get(         /* 残トライ回数:R */
        );
        
        /* トライ不可能な場合 */
        if (tc <= CMD_NUM0) {
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d]RE Carrier Status NG.", D_REC_C02_CPR_NO());
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_RECRRSTSPRCIND));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

            /**************************************************************/
            /* スレッド間(MTアドレス設定(FLD-MT)プロシージャ実行指示)送信 */
            /**************************************************************/
            f_trp_rec_Fc02_t_it_mtsetfprc_snd(                    /* なし:R */
            );
            /******************/
            /* 自状態番号設定 */
            /******************/
            f_trp_rec_Fc02_t_ownstn_set(                          /* なし:R */
                EC_REC_C02_STN_RES_RECRRSTSCMP
                                          /* RE起動中(REキャリア状態完了):I */
            );

            
            /* 終了 */
            return;
        }
        else {
            snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]RE Carrier Status NG.", D_REC_C02_CPR_NO());
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_RECRRSTSPRCIND));
            D_REC_IVLOUT(D_TCM_INVLOGLV_WARN2, errdata);

            /******************************************************/
            /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
            /******************************************************/
            f_trp_rec_Fc02_t_it_refrcrstprc_snd(                  /* なし:R */
            );
            /* 終了 */
            return;
        }
    }
    
    /**********************************/
    /* L3(REキャリア状態報告要求)送信 */
    /**********************************/
    f_trp_rec_Fc02_t_l3_recrrstsreq_snd(                          /* なし:R */
    );

    /****************************************************/
    /* 残トライ回数(REキャリア状態報告要求)デクリメント */
    /****************************************************/
    f_trp_rec_Fc02_t_tc_recrrstsreq_dec(                          /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_RECRRSTSING  /* RE起動中(REキャリア状態報告中):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
