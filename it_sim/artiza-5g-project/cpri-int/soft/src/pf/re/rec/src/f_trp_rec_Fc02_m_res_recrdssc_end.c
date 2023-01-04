/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_recrdssc_end.c
 * @brief  RE起動中REカード状態報告(セクタ部)終了処理
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE起動中REカード状態報告(セクタ部)終了処理
 * @note   RE起動中REカード状態報告(セクタ部)終了処理を行う
 *         - RE状態(セクタ部)管理テーブル(REカード状態)設定
 *         - スレッド間(REカード状態報告(セクタ部)プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(RE PORT状態報告完了)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_recrdssc_end(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    VOID*  datp;
    VOID*  l3_bufp;                                   /* L3バッファポインタ */
    USHORT ret_code;                                          /* 結果コード */
    USHORT slt_no;                                          /* スロット番号 */
    UINT   uit_code;
// [16B] add start
//    USHORT slot_idx;
    USHORT link_no;
// [16B] add end

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__ );
#endif

    /* L3バッファポインタ抽出 */
    datp    = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    l3_bufp = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    /* 結果コード・スロット番号抽出 */
// [16B] add start
    link_no  = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->cprircv_inf.link_num;
// [16B] add end
    ret_code = ((CMT_CPRIF_CARDSTSRES_S3G*)l3_bufp)->result;
    slt_no   = ((CMT_CPRIF_CARDSTSRES_S3G*)l3_bufp)->cardsts_inf.slot_no;

	/*	カード状態報告応答受信ログ格納	*/
	f_trp_rec_Fcom_t_l3_LogStore( D_REC_LOGKIND_CARD, D_REC_C02_CPR_NO(), D_REC_MSGKIND_RSP );

    /**************/
    /* タイマ取消 */
    /**************/
    uit_code = f_trp_rec_Fc02_t_timer_ccl(                        /* なし:R */
        EC_REC_TMK_RECRDSSC_CNF,  /* REカード状態報告(セクタ部)確認タイマ:I */
        slt_no                                            /* スロット番号:I */
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

// [16B] add start
//    f_cmn_com_slotsrch_s3g(link_no, slt_no, &slot_idx);
// [16B] add end

    /* OKの場合 */
    if (ret_code == CMD_NML) {
        /**************************************************/
        /* RE状態(セクタ部)管理テーブル(REカード状態)設定 */
        /**************************************************/
        uit_code = f_trp_rec_Fc02_t_recrdssc_i_set(               /* 変化:R */
// [16B] chg start
            slt_no,                                       /* スロット番号:I */
// [16B] chg end
            l3_bufp                                 /* L3バッファポインタ:I */
        );

// [16B] add start
        f_trp_rec_Wc02_stsmng_tbl.sts[link_no-1].crdssc_num++;
// [16B] add end


#if 0  // [16B] del start
		/**********************************************************/
        /* スレッド間(REキャリア状態報告プロシージャ実行指示)送信 */
        /**********************************************************/
        f_trp_rec_Fc02_t_it_recrrstsprc_snd(                      /* なし:R */
        );
        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
            EC_REC_C02_STN_RES_RECRDSCMCMP
                              /* RE起動中((REカード状態報告(共通部)完了)):I */
        );
        /* 終了 */
        return;
#endif // [16B] del end
    }
	
	/******************************************************************/
    /* スレッド間(REカード状態報告(セクタ部)プロシージャ実行指示)送信 */
    /******************************************************************/
    f_trp_rec_Fc02_t_it_recrdsscprc_snd(                          /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_REPRTSTSCMP   /* RE起動中(RE PORT状態報告完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
