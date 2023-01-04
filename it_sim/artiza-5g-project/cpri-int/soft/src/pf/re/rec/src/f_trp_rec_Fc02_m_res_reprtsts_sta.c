/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_reprtsts_sta.c
 * @brief  RE起動中RE PORT状態報告開始処理
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
 * @brief  RE起動中RE PORT状態報告開始処理
 * @note   RE起動中RE PORT状態報告開始処理を行う
 *         - スレッド間(RE PORT状態報告プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(RE PORT状態報告中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/17 FFCS)Shihzh modify for S3G PRC.
 * @date   2016/04/25 TDI)satou リトライオーバー時、強制リセットではなく、次の正常起動シーケンスに進めるように修正
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_reprtsts_sta(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT lwTrCnt;                                              /* 残トライ回数 */
//    USHORT actsts;                                          /* Active state */	Reserved variable(TBD)
    CHAR  errdata[CMD_NUM48];
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

//    actsts = f_trp_rec_Fc02_t_ownact_get();
//    /* Negative case  */
//    if (actsts != D_REC_ACK_ACTCMP) {
//        /***********************************************************************/
//        /* スレッド間(REカード状態報告(セクタ部)報告プロシージャ実行指示)送信  */
//        /***********************************************************************/
//        f_trp_rec_Fc02_t_it_recrdsscprc_snd(                      /* なし:R */
//        );
//        /******************/
//        /* 自状態番号設定 */
//        /******************/
//        f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
//            EC_REC_C02_STN_RES_REPRTSTSCMP 
//                                         /* RE起動中(RE PORT状態報告完了):I */
//        );
//        return;
//    }

    /********************************************/
    /* 残トライ回数(RE PORT状態報告要求)取得    */
    /********************************************/
    lwTrCnt = f_trp_rec_Fc02_t_tc_reprtstsreq_get(             /* 残トライ回数:R */
    );
    
    /* トライ不可能な場合 */
    if (lwTrCnt <= CMD_NUM0) {
        snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d]RE Port Status NG.", D_REC_C02_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_REPRTSTSPRCIND));
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

        /**********************************************************************/
        /* スレッド間(REカード状態報告(セクタ部)要求プロシージャ実行指示)送信 */
        /**********************************************************************/
        f_trp_rec_Fc02_t_it_recrdsscprc_snd();

        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
            EC_REC_C02_STN_RES_REPRTSTSCMP 
                                         /* RE起動中(RE PORT状態報告完了):I */
        );
        
        /* 終了 */
        return;
    }
    
    /**********************************/
    /* L3(RE PORT状態報告要求)送信    */
    /**********************************/
    f_trp_rec_Fc02_t_l3_reprtstsreq_snd(                          /* なし:R */
    );

    /****************************************************/
    /* 残トライ回数(RE PORT状態報告要求)デクリメント    */
    /****************************************************/
    f_trp_rec_Fc02_t_tc_reprtstsreq_dec(                          /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_REPRTSTSING   /* RE起動中((RE PORT状態報告中)):I */
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
