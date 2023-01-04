/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_resltinf_sta_3g.c
 * @brief  RE起動中REスロット情報報告開始処理
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
 * @brief  RE起動中REスロット情報報告開始処理
 * @note   RE起動中REスロット情報報告開始処理を行う
 *         - スレッド間(REスロット情報報告プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(REスロット情報報告中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/17 FFCS)Shihzh modify for S3G PRC.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_resltinf_sta_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT tc;                                              /* 残トライ回数 */
    CHAR  errdata[CMD_NUM48];
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /********************************************/
    /* 残トライ回数(REスロット情報報告要求)取得 */
    /********************************************/
    tc = f_trp_rec_Fc02_t_tc_resltinfreq_get_3g(             /* 残トライ回数:R */
    );
    
    /* トライ不可能な場合 */
    if (tc <= CMD_NUM0) {
        snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]RE Slotinf report NG.", D_REC_C02_3G_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_RESLTINFPRCIND));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN2, errdata);

        /******************************************************/
        /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
        /******************************************************/
        f_trp_rec_Fc02_t_it_refrcrstprc_snd_3g(                      /* なし:R */
        );
        
        /* 終了 */
        return;
    }
    
    /**********************************/
    /* L3(REスロット情報報告要求)送信 */
    /**********************************/
    f_trp_rec_Fc02_t_l3_resltinfreq_snd_3g(                          /* なし:R */
    );

    /****************************************************/
    /* 残トライ回数(REスロット情報報告要求)デクリメント */
    /****************************************************/
    f_trp_rec_Fc02_t_tc_resltinfreq_dec_3g(                          /* なし:R */
    ); 

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_RESLTINFING
                                        /* RE起動中(REスロット情報報告中):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
