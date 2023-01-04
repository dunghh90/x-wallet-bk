/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_refil_end_3g.c
 * @brief  RE起動中REファイル更新終了処理
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
 * @brief  RE起動中REファイル更新終了処理
 * @note   RE起動中REファイル更新終了処理を行う
 *         - 結果コード・リセット要否抽出
 *           - リセット要の場合
 *             - L3(REリセット要求)送信
 *             - 自状態番号設定(REリセット中)
 *             - 処理終了
 *         - スレッド間(Toffset値報告プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(REファイル更新完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 * @date   2010/08/24 CMS)Takahashi modify for M-S3G-eNBPF-03711
 * @date   2010/09/03 CMS)Takahashi modify for M-S3G-eNBSYS-01907
 * @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.243対応
 * @date   2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_refil_end_3g(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT ret_code;                                          /* 結果コード */
    USHORT rst_flg;                                         /* リセット要否 */
    CHAR  errdata[CMD_NUM48];
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 結果コード・リセット要否抽出 */
    ret_code = ((CMT_TSKIF_REFILNEWRES*)bufp)->result_code;
    rst_flg  = ((CMT_TSKIF_REFILNEWRES*)bufp)->reset_jdg;

	/* REファイル更新応答(OK,更新対象CPRI#,更新あり) */
    /* リセット要の場合 */
    if ((ret_code == CMD_RLTCODE_OK) && (rst_flg == CMD_ON)) {
        /**************************************************/
        /* スレッド間(REリセットプロシージャ実行指示)送信 */
        /**************************************************/
        f_trp_rec_Fc02_t_it_rerstprc_snd_3g(                         /* なし:R */
        );
    }
	/* REファイル更新応答(結果(OK),更新なし) */
    else if((ret_code == CMD_RLTCODE_OK) && (rst_flg == CMD_OFF)) {
        /*****************************************************/
        /* スレッド間(Toffset値報告プロシージャ実行指示)送信 */
        /*****************************************************/
        f_trp_rec_Fc02_t_it_tofvalprc_snd_3g(                        /* なし:R */
        );
    }
    /* NGの場合 */
    else{
        snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]RE Firmware download NG.", D_REC_C02_3G_CPR_NO());
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_REFILNEWRES));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

        /************************************************/
        /* スレッド間(強制停止プロシージャ実行指示)送信 */
        /************************************************/
        f_trp_rec_Fc02_t_it_frcstpprc_snd_3g(E_RRH_RESTOP_FACTOR_DL_NG);
        
        /* 終了 */
        return;
    }

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_REFILCMP       /* RE起動中(REファイル更新完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
