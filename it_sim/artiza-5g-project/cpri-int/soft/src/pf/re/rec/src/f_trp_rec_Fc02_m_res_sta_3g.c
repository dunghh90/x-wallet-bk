/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_sta_3g.c
 * @brief  RE起動開始処理
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
 * @brief  RE起動開始処理
 * @note   RE起動開始処理を行う
 *         - L3(RE時刻設定要求)送信
 *         - スレッド間(RE時刻設定プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(L3確立完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/08/28 FPT)Yen create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_sta_3g(                                    /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT                 lwCarrNo;
 
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************************************/
    /* 残トライ回数(RE強制リセット送信以外)初期化 */
    /**********************************************/
    f_trp_rec_Fc02_t_tc_ini_3g(                                  /* なし:R */
    );

    /*****************************************************/
    /* RE状態(セクタ部)管理テーブル(REスロット情報初期化 */
    /*****************************************************/
    f_trp_rec_Fc02_t_resltinfsc_ini_3g(
        D_REC_INK_SHC
    );

    /***************************************************************/
    /* RE状態(共通部)管理テーブル(REキャリア状態報告)初期化        */
    /***************************************************************/
    f_trp_rec_Fc02_t_recrrsts_a_ini_3g(                              /* 変化:R */
        D_REC_INK_SHC                                         /* 初期化種別 */
    );

	/*	L3送受信ログクリア	*/
	f_trp_rec_Fcom_t_l3_LogClr( D_REC_C02_3G_CPR_NO() );

    /**************************************************/
    /* スレッド間(RE時刻設定プロシージャ実行指示)送信 */
    /**************************************************/
    f_trp_rec_Fc02_t_it_retimsetprc_snd_3g(                          /* なし:R */
    );

    /* MTアドレス設定開始要因初期化: FHM主導*/
    f_trp_rec_Fc02_t_tc_mtsetffactor_set( D_REC_C02_3G_CPR_NO(), D_MT_ADD_SET_FACTOR_FHM );
    f_trp_rec_Fc02_t_mtsetsts(  D_REC_C02_3G_CPR_NO(), EC_REC_MTADDSET_STN_INI );

	/* C03状態初期化 */
	for( lwCarrNo = CMD_CARNO_MIN; lwCarrNo <= CMD_CARNO_MAX_3G; lwCarrNo++ )
    {
        f_trp_rec_Fcom_t_c03stn_set_3g(                                   /* なし:R */
                D_REC_C02_3G_CPR_NO(),                                /* CPRI番号:I */
                lwCarrNo,                                         /* キャリア番号:I */
                EC_REC_C03_STN_IDLE                                /* C03状態番号:I */
            );
    }

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_L3ECMP                 /* RE起動中(L3確立完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
