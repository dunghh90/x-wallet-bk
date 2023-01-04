/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_usrefil_end.c
 * @brief  運用中REファイル更新完了通知処理
 * @date   2015/08/21 FPT)Yen create.
 * @date   2015/09/30 TDIPS)sasaki update
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中REファイル更新完了通知処理
 * @note   運用中REファイル更新完了通知処理を行う
 *         - 結果コード抽出
 *           - OKの場合
 *             - 対RE状態#nをRE運用中へ遷移
 *           - NGの場合
 *             - 対RE状態#nを強制停止へ遷移
 *             - 強制停止指示
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create.
 * @date   2015/09/30 TDIPS)sasaki update 自状態遷移処理を削除
 * @date   2015/10/20 FPT)Yen fix bug IT2(No118)
 * @date   2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_usrefil_end(                            /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT ret_code;                                          /* 結果コード */
    USHORT cpri_no;                                          /* 結果コード */
    CHAR  errdata[CMD_NUM48];
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 結果コード抽出 */
    ret_code = ((CMT_TSKIF_UREFILNEWFINRES*)bufp)->result_code;
	cpri_no  = ((CMT_TSKIF_UREFILNEWFINRES*)bufp)->cpri_no;

	f_trp_rec_Wc02_prm_tbl.cpr_no = cpri_no;

	/* NGの場合 */
    if (ret_code != CMD_RLTCODE_OK) 
	{
        snprintf(errdata, CMD_NUM48, "[FRCRST][cpr:%02d]f_trp_rec_Fc02_m_res_usrefil_end NG.", D_REC_C02_CPR_NO()); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(CMT_TSKIF_UREFILNEWFINRES));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

        /************************************************/
        /* スレッド間(強制停止プロシージャ実行指示)送信 */
        /************************************************/
        f_trp_rec_Fc02_t_it_frcstpprc_snd(E_RRH_RESTOP_FACTOR_DL_NG);
        
        /* 終了 */
        return;
    }

//    /******************/
//    /* 自状態番号設定 */
//    /******************/
//    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
//        EC_REC_C02_STN_USE                                     /* 運用中:I */
//    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
