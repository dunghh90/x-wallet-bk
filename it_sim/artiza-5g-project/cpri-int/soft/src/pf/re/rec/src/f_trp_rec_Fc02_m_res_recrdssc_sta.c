/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_recrdssc_sta.c
 * @brief  RE起動中REカード状態報告(セクタ部)開始処理
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
 * @brief  RE起動中REカード状態報告(セクタ部)開始処理
 * @note   RE起動中REカード状態報告(セクタ部)開始処理を行う
 *         - REカード状態(セクタ部)未取得スロット番号取得
 *           - 未取得カード情報なしの場合
 *             - スレッド間(REカード状態報告(共通部)プロシージャ実行指示)送信
 *             - 自状態番号設定(RE起動中((REカード状態報告(セクタ部)完了)))
 *             - 処理終了
 *         - L3(REカード状態報告要求(セクタ部))送信
 *         - 自状態番号設定(RE起動中(RE REカード状態報告(セクタ部)中)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/17 FFCS)Shihzh modify for S3G PRC.
 * @date   2009/01/26 FFCS)Shihzh M-S3G-eNBPF-01182.
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_recrdssc_sta(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT slt_no;                                    /* 未取得スロット番号 */
    SHORT  tc;                                              /* 残トライ回数 */
    CHAR  errdata[CMD_NUM48];
    
// [16B] add start
//    USHORT slt_no_idx;
// [16B] add end

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

#if 0  // [16B] del start
	slt_no = CMD_SLTNO_01;
#endif // [16B] del end

// [16B] add start
    /************************************************/
    /* REカード状態(セクタ部)未取得スロット番号取得 */
    /************************************************/
    slt_no = f_trp_rec_Fc02_t_ugrecrdssc_get(       /* 未取得スロット番号:R */
    );

// [16B] add start
//	/* Slot番号からSlotのIndex値(0-15)を算出する 16B KCN add */
//	f_cmn_com_slotsrch_s3g(D_REC_C02_CPR_NO(), slt_no, &slt_no_idx);
// [16B] add end

    /* 未取得カード情報なしの場合 */
    // 全スロットのカード状態の取得が完了した場合
    if (slt_no == CMD_NUM0) {

        // カード状態をポート集約する
        f_trp_rec_Fc02_t_recrdssc_i_merge();

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
    }
// [16B] add end

    /******************************************/
    /* 残トライ回数(REカード状態報告要求)取得 */
    /******************************************/
    tc = f_trp_rec_Fc02_t_tc_recrdstsreq_get(           /* 残トライ回数:R */
        slt_no                                          /* スロット番号:I */
    );

    /* RE状態報告要求トライ不可能な場合 */
    if (tc <= CMD_NUM0) {

// [16B] add start
        /****************************************/
        /* 残トライ回数(RE強制リセット送信)取得 */
        /****************************************/
        tc = f_trp_rec_Fc02_t_tc_refrcrstsnd_get(        /* 残トライ回数:R */
        );

        /* トライ不可能な場合 */
        if (tc <= CMD_NUM0) {
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][slt:0x%02x]RE Card Status NG.", D_REC_C02_CPR_NO(), slt_no);
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_RECRDSSCPRCIND));
            D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);

            /**************************************************/
            /* REカード状態(セクタ部)取得NGのスロット番号設定 */
            /**************************************************/
            f_trp_rec_Fc02_t_ngrecrdssc_set(                     /* なし:R */
                slt_no
//                slt_no_idx                            /* スロットIndex値:I */
            );

            /******************************************************************/
            /* スレッド間(REカード状態報告(セクタ部)プロシージャ実行指示)送信 */
            /******************************************************************/
            f_trp_rec_Fc02_t_it_recrdsscprc_snd(                 /* なし:R */
            );

            /******************/
            /* 自状態番号設定 */
            /******************/
            f_trp_rec_Fc02_t_ownstn_set(                         /* なし:R */
                EC_REC_C02_STN_RES_REPRTSTSCMP   
                                        /* RE起動中(RE PORT状態報告完了):I */
            );

            return;
        }
        else {
// [16B] add end

            snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d][slt:0x%02x]RE Card Status NG.", D_REC_C02_CPR_NO(), slt_no);
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_RECRDSSCPRCIND));
            D_REC_IVLOUT(D_TCM_INVLOGLV_WARN2, errdata);
    
            /******************************************************/
            /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
            /******************************************************/
             f_trp_rec_Fc02_t_it_refrcrstprc_snd(                 /* なし:R */
             );

            /* 終了 */
            return;
// [16B] add start
		}
// [16B] add end
    }

    /******************************************/
    /* L3(REカード状態報告要求(セクタ部))送信 */
    /******************************************/
    f_trp_rec_Fc02_t_l3_recrdsscreq_snd(                          /* なし:R */
        slt_no                                            /* スロット番号:I */
//        slt_no_idx                                            /* スロット番号:I */
    );

    /**************************************************/
    /* 残トライ回数(REカード状態報告要求)デクリメント */
    /**************************************************/
    f_trp_rec_Fc02_t_tc_recrdstsreq_dec(                          /* なし:R */
        slt_no                                            /* スロット番号:I */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_RECRDSSCING
                                /* RE起動中(REカード状態報告(セクタ部)中):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
