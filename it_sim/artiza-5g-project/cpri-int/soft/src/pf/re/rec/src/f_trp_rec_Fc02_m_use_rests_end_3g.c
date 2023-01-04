/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_rests_end.c
 * @brief  運用中RE状態報告終了処理
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中RE状態報告終了処理
 * @note   運用中RE状態報告終了処理を行う
 *         - RE状態(共通部)管理テーブル(RE状態情報)設定
 *           - 変化ありの場合
 *             - プロセス間(RE状態変化通知)送信
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create. 
 * @date   2011/02/17 FJT)Koshida modify for M-S3G-eNBSYS-02253(TS-111)対応.
 * @date   2015/10/07 FPT)DuongCD update comment
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_rests_end_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    // VOID*  datp;
    // VOID*  l3_bufp;                                   /* L3バッファポインタ */
    // USHORT actsts;                                        /*  Active status */
    // USHORT ret_code;                                          /* 結果コード */
    // UINT   uit_ret;                                           /* 終了コード */
    // CHAR   errdata[CMD_NUM48];

    // /* 関数トレースログ */
    // F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    // actsts = f_trp_rec_Fc02_t_ownact_get_3g();
    // /* Negative case  */
    // if (actsts != D_REC_ACK_ACTCMP) {
    //    return;
    // }

    // /* L3バッファポインタ抽出 */
    // datp    = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    // l3_bufp = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    // /* 結果コード抽出 */
    // ret_code = ((CMT_CPRIF_RESTSRES*)l3_bufp)->result;

    // /**************/
    // /* タイマ取消 */
    // /**************/
    // uit_ret = f_trp_rec_Fc02_t_timer_ccl_3g(                      /* なし:R */
    //     EC_REC_TMK_RESTS_CNF,                     /* RE状態報告確認タイマ:I */
    //     D_REC_SLT_IV                                      /* スロット番号:I */
    // );

    // if (uit_ret == D_REC_TIM_IV) {
    //     return;
    // }
	// /**********************/
	// /* 残トライ回数初期化 */
	// /**********************/
	// f_trp_rec_Fc02_t_use_restsreq_ini_3g(                         /* なし:R */
	// );
    
    // if (ret_code == CMD_NML) { 
    //     ret_code |= f_trp_rec_Fc02_t_cprpar_chk(datp);
    // }    
    // /* OK以外の場合 */
    // if (ret_code != CMD_NML) {
    //     /************************************************/
    //     /* RE状態(共通部)管理テーブル(RE状態報告)初期化 */
    //     /************************************************/
    //     uit_ret = f_trp_rec_Fc02_t_rests_ini_3g(                  /* 変化:R */
    //         D_REC_INK_RNG                                   /* 初期化種別:I */
    //     );
    //     if (uit_ret == CMD_ON) {
    //         snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][ret:0x%04x]RE sts report NG.", D_REC_C02_3G_CPR_NO(), ret_code);
    //         D_REC_IVLINI();
    //         D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
    //         D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
    //     }
    // }
    // /**********************************************/
    // /* RE状態(共通部)管理テーブル(RE状態報告)設定 */
    // /**********************************************/
    // uit_ret = f_trp_rec_Fc02_t_rests_set_3g(                  /* 変化:R */
    //     l3_bufp                                 /* L3バッファポインタ:I */
    // );

    // /* 変化判定 */
    // if (uit_ret == CMD_OFF) {
    //     /* 終了 */
    //     return;
    // }
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
