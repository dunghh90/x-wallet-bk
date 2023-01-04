/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_cnftim_ccl_3g.c
 * @brief  確認タイマ取消
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
 * @brief  確認タイマ取消
 * @note   確認タイマを取り消す
 *         - タイマ取消(RE時刻設定送信間隔タイマ)
 *         - タイマ取消(Toffset値報告送信間隔タイマ)
 *         - タイマ取消(REカード状態報告(セクタ部)送信間隔タイマ)
 *         - タイマ取消(REカード状態報告(共通部)送信間隔タイマ)
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_cnftim_ccl_3g(                                 /* なし:R */
) {
    UINT         num;                                       /* タイマ情報数 */
    UINT         cnt;                                           /* カウンタ */
    T_REC_TIMELM elm;                                         /* タイマ要素 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* タイマ情報数抽出 */
    num = f_trp_rec_Wcom_timmng_tbl.num;

    /* タイマ情報数分ループ */
    for (cnt = CMD_NUM0; cnt < num; cnt++) {

        /****************************/
        /* タイマキー情報→要素変換 */
        /****************************/
        f_trp_rec_Fcom_t_key_elm_cnv(                             /* なし:R */
            f_trp_rec_Wcom_timmng_tbl.inf[cnt].key_inf.keyinf1,    /* キー情報:I */
            &elm                                                  /* 要素:O */
        );

        /* 他クラスの場合 */
        if (elm.cls != EC_REC_C02_3G) {
            /* 次を検索 */
            continue;
        }

        /* 他CPRI番号の場合 */
        if (elm.cpr_no != D_REC_C02_3G_CPR_NO()) {
            /* 次を検索 */
            continue;
        }

        /* 確認タイマ以外の場合 */
        if ((elm.tmk != EC_REC_TMK_TOFVAL_CNF  ) &&
            (elm.tmk != EC_REC_TMK_REEQPINF_CNF) &&
            (elm.tmk != EC_REC_TMK_RESLTINF_CNF) &&
            (elm.tmk != EC_REC_TMK_RESTS_CNF   ) &&
            (elm.tmk != EC_REC_TMK_RECRDSSC_CNF) &&
            (elm.tmk != EC_REC_TMK_RECRDSCM_CNF) &&
            (elm.tmk != EC_REC_TMK_RECRRSTS_CNF) &&
            (elm.tmk != EC_REC_TMK_REPRTSTS_CNF)) {
            /* 次を検索 */
            continue;
        }

        /**************/
        /* タイマ取消 */
        /**************/
        f_trp_rec_Fc02_t_timer_ccl_3g(                               /* なし:R */
            elm.tmk,                                        /* タイマ種別:I */
            elm.soc_no                                    /* スロット番号:I */
        );
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
