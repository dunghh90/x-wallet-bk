/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_recrrsts_end.c
 * @brief  運用中REスロット情報報告終了処理
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "f_rrh_reg_dis.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中REキャリア状態報告終了処理
 * @note   運用中REキャリア状態報告終了処理を行う
 *         - RE状態(共通部)管理テーブル(REキャリア状態情報)設定
 *         - スレッド間(MTアドレス設定(FLD-MT)要求プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(REキャリア状態報告完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_recrrsts_end(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    VOID*  datp;
    VOID*  l3_bufp;                                   /* L3バッファポインタ */
    USHORT ret_code;                                          /* 結果コード */
    UINT   uit_code;
	//USHORT  carrier_inf;
//    USHORT  alm_inf_count;
//    //UINT    reg_sw_3g;
//    T_TRXSET_PARA_LTE   ltTrxSetPara;
//    UINT                      msg_id;
//    USHORT  re_cpri_num;
//    CMT_CPRIF_CARLSTSRES *bufp_t;
//    USHORT  carrier_count;
//    USHORT *offset;
//    USHORT  carrier_num[5];
//    USHORT  carrier_sts;
//    USHORT  branch_count;
//    USHORT  branch_num[8];
//    USHORT  branch_stsDL[8];
//    USHORT  branch_set;
//    UINT    reg_sw_s3g;
//    USHORT  i, j;
//    USHORT  card_count;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    /* L3バッファポインタ抽出 */
    datp    = &((T_REC_MSGRCV_TBL*)bufp)->datp;

    l3_bufp = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    /* 結果コード抽出 */
    ret_code = ((CMT_CPRIF_CARLSTSRES*)l3_bufp)->result;

	/*	キャリア状態報告応答受信ログ格納	*/
	f_trp_rec_Fcom_t_l3_LogStore( D_REC_LOGKIND_CARRIER, D_REC_C02_CPR_NO(), D_REC_MSGKIND_RSP );

    /**************/
    /* タイマ取消 */
    /**************/
    uit_code = f_trp_rec_Fc02_t_timer_ccl(                        /* なし:R */
        EC_REC_TMK_RECRRSTS_CNF,          /* REキャリア状態報告確認タイマ:I */
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
    if (ret_code != CMD_NML) {
        /*************************************************************/
        /* スレッド間(REキャリア状態報告プロシージャ実行指示)送信    */
        /*************************************************************/
        f_trp_rec_Fc02_t_it_recrrstsprc_snd(                      /* なし:R */
        );

        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
            EC_REC_C02_STN_RES_RECRDSCMCMP
                                /* RE起動中(REカード状態セクタ部報告完了):I */
        );

        /* 終了 */
        return;
    }

    /********************************************************/
    /* RE状態(共通部)管理テーブル(REキャリア状態情報)設定   */
    /********************************************************/
    f_trp_rec_Fc02_t_recrrsts_set(                                /* 変化:R */
        l3_bufp                                     /* L3バッファポインタ:I */
    );

    /************************************************************************/
    /* スレッド間(MTアドレス設定(FLD-MT)要求プロシージャ実行指示)送信       */
    /************************************************************************/
    f_trp_rec_Fc02_t_it_mtsetfprc_snd(                            /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_RECRRSTSCMP
                                      /* RE起動中(REキャリア状態報告完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
