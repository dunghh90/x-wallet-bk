/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_t_timer_ccl.c
 * @brief  タイマ取消
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
 * @brief  タイマ取消
 * @note   タイマを取り消す
 *         - タイマ取消
 *         - タイマ管理テーブル削除
 * @param  tmk    [in]  タイマ種別
 * @return 終了コード
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
UINT f_trp_rec_Fc03_t_timer_ccl_3g(                         /* 終了コード:R */
    ET_REC_TMK tmk                                          /* タイマ種別:I */
) {
    INT                    sit_ret;                           /* 終了コード */
    UINT                   uit_ret;                           /* 終了コード */
    T_REC_TIMELM           elm;                               /* タイマ要素 */
    bpf_ru_hrtm_key_inf_t  key_inf;                       /* タイマキー情報 */
    UINT                   tim_id;                          /* タイマ識別子 */
    CHAR                  errdata[CMD_NUM48]; 
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 要素設定 */
    elm.cls    = EC_REC_C03_3G;                                      /* クラス */
    elm.tmk    = tmk;                                         /* タイマ種別 */
    elm.cpr_no = D_REC_C03_3G_CPR_NO();                            /* CPRI番号 */
    elm.soc_no = D_REC_C03_3G_CRR_NO();                        /* キャリア番号 */

    /****************************/
    /* タイマ要素→キー情報変換 */
    /****************************/
    f_trp_rec_Fcom_t_elm_key_cnv(                                 /* なし:R */
        &elm,                                                     /* 要素:I */
        &key_inf                                              /* キー情報:O */
    );

    /**************************/
    /* タイマ管理テーブル検索 */
    /**************************/
    tim_id = f_trp_rec_Fcom_t_timmng_tbl_sch(             /* タイマ識別子:R */
        &key_inf                                              /* キー情報:O */
    );
    /* 一致情報なしの場合 */
    if (tim_id == CMD_NUM0) {
        /* 無効処理履歴 */
        return CMD_NG;
    }

    /**************/
    /* タイマ取消 */
    /**************/
    sit_ret = f_trp_com_Fcancel_tim(                        /* 終了コード:R */
        key_inf.keyinf1                                   /* タイマ識別子:I */
    );
    /* タイマ取消NGの場合 */
    if (sit_ret != CMD_OK) {
        /* すれ違いなので、ここでは何もしない */
        /* →タイムアウト発生通知を破棄       */
    }

    /**************************/
    /* タイマ管理テーブル削除 */
    /**************************/
    uit_ret = f_trp_rec_Fcom_t_timmng_tbl_del(              /* 終了コード:R */
        &key_inf                                              /* キー情報:I */
    );
    /* 一致情報なしの場合 */
    if (uit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][crr:%02d][ret:0x%02x]Timer tbale change unexcept.", D_REC_C03_CPR_NO(), D_REC_C03_CRR_NO(), sit_ret); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(&key_inf, sizeof(key_inf));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
        return CMD_NG;
    }


    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
