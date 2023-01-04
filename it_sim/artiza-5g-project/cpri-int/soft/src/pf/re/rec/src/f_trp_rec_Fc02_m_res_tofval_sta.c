/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_tofval_sta.c
 * @brief  運用中Toffset値報告開始処理
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
 * @brief  運用中Toffset値報告開始処理
 * @note   運用中Toffset値報告開始処理を行う
 *         - L3(Toffset値報告要求)送信
 *         - 自状態番号設定(RE起動中(Toffset値報告中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_tofval_sta(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    SHORT tc;                                               /* 残トライ回数 */
    CHAR errdata[CMD_NUM48];
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /***************************************/
    /* 残トライ回数(Toffset値報告要求)取得 */
    /***************************************/
    tc = f_trp_rec_Fc02_t_tc_tofvalreq_get(               /* 残トライ回数:R */
    );

    /* トライ不可能な場合 */
    if (tc <= CMD_NUM0) {
        snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]Tofset report NG.", D_REC_C02_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_TOFVALPRCIND));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN2, errdata);

        /******************************************************/
        /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
        /******************************************************/
        f_trp_rec_Fc02_t_it_refrcrstprc_snd(                      /* なし:R */
        );
        
        /* 終了 */
        return;
    }

    /*****************************/
    /* L3(Toffset値報告要求)送信 */
    /*****************************/
    f_trp_rec_Fc02_t_l3_tofvalreq_snd(                            /* なし:R */
    );

    /***********************************************/
    /* 残トライ回数(Toffset値報告要求)デクリメント */
    /***********************************************/
    f_trp_rec_Fc02_t_tc_tofvalreq_dec(                            /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_TOFVALING         /* RE起動中(Toffset値報告中):I */
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
