/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_rst_rerst_sta.c
 * @brief  REリセット開始処理
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
 * @brief  REリセット開始処理
 * @note   REリセット開始処理を行う
 *         - L3(REリセット要求)送信
 *         - 自状態番号設定(RE起動中(REリセット中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_rst_rerst_sta(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    SHORT tc;                                               /* 残トライ回数 */
    CHAR  errdata[CMD_NUM48];
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************************/
    /* 残トライ回数(REリセット要求)取得 */
    /************************************/
    tc = f_trp_rec_Fc02_t_tc_rerstreq_get(                /* 残トライ回数:R */
    );

    /* トライ不可能な場合 */
    if (tc <= CMD_NUM0) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]RE reset NG.", D_REC_C02_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_RERSTPRCIND));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

        /******************************************************/
        /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
        /******************************************************/
        f_trp_rec_Fc02_t_it_refrcrstprc_snd(                      /* なし:R */
        );

        /* 終了 */
        return;
    }

    /**************************/
    /* L3(REリセット要求)送信 */
    /**************************/
    f_trp_rec_Fc02_t_l3_rerstreq_snd(                             /* なし:R */
    );

    /********************************************/
    /* 残トライ回数(REリセット要求)デクリメント */
    /********************************************/
    f_trp_rec_Fc02_t_tc_rerstreq_dec(                             /* なし:R */
    );
	
//    /********************************************/
//    /* Active Switch (BRE/M-LRE case)           */
//    /********************************************/
//    if ((f_trp_rec_Fc02_t_ownact_get() == D_REC_ACK_ACTCMP) && 
//    	(D_REC_C02_RE_SYS_TYPE() != D_TCM_SYS_RETYP_RRELRE)){
//        /**********************************/
//        /* スレッド間(Active切替指示)送信 */
//        /**********************************/
//        f_trp_rec_Fc02_t_it_actchgind_snd(                        /* なし:R */
//            CMD_CHGTRG_RERST                  /* 再active依頼通知に応じる:I */
//        );
//    }

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RERSTING                           /* REリセット中:I */
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
