/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_rests_sta.c
 * @brief  RE起動中RE状態報告開始処理
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
 * @brief  RE起動中RE状態報告開始処理
 * @note   RE起動中RE状態報告開始処理を行う
 *         - スレッド間(RE状態報告プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(RE状態報告中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/17 FFCS)Shihzh modify for S3G PRC.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_rests_sta(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT tc;                                              /* 残トライ回数 */
    USHORT actsts;                                          /* Active state */
    CHAR  errdata[CMD_NUM48];
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
    
    actsts = f_trp_rec_Fc02_t_ownact_get();
    /* Negative case  */
    if (actsts != D_REC_ACK_ACTCMP) {
        /***********************************************************************/
        /* スレッド間(REカード状態報告(セクタ部)報告プロシージャ実行指示)送信  */
        /***********************************************************************/
        f_trp_rec_Fc02_t_it_reprtstsprc_snd(                      /* なし:R */
        );
        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
            EC_REC_C02_STN_RES_RESTSCMP 
                                             /* RE起動中(RE 状態報告完了):I */
        );
        return;
    }
    
    /********************************************/
    /* 残トライ回数(RE状態報告要求)取得         */
    /********************************************/
    tc = f_trp_rec_Fc02_t_tc_restsreq_get(                /* 残トライ回数:R */
    );
    
    /* RE状態報告要求トライ不可能な場合 */
    if (tc <= CMD_NUM0) {
        /****************************************/
        /* 残トライ回数(RE強制リセット送信)取得 */
        /****************************************/
        tc = f_trp_rec_Fc02_t_tc_refrcrstsnd_get(         /* 残トライ回数:R */
        );
        /* トライ不可能な場合 */
        if (tc <= CMD_NUM0) {
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d]RE Status NG.", D_REC_C02_CPR_NO());
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_RESTSPRCIND));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

            /****************************************************/
            /* スレッド間(RE PORT 状態プロシージャ実行指示)送信 */
            /****************************************************/
            f_trp_rec_Fc02_t_it_reprtstsprc_snd(                  /* なし:R */
            );
            /******************/
            /* 自状態番号設定 */
            /******************/
            f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
                EC_REC_C02_STN_RES_RESTSCMP 
                                                 /* RE起動中(RE 状態報告完了):I */
            );

            /* 終了 */
            return;
        }
        else {
            snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]RE Status NG.", D_REC_C02_CPR_NO());
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_RESTSPRCIND));
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
    /* L3(RE状態報告要求)送信         */
    /**********************************/
    f_trp_rec_Fc02_t_l3_restsreq_snd(                             /* なし:R */
    );

    /****************************************************/
    /* 残トライ回数(RE状態報告要求)デクリメント         */
    /****************************************************/
    f_trp_rec_Fc02_t_tc_restsreq_dec(                             /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_RESTSING             /* RE起動中(RE状態報告中):I */
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
