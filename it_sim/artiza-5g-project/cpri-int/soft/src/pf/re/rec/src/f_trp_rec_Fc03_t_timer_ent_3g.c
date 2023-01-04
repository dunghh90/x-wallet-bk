/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_t_timer_ent_3g.c
 * @brief  タイマ登録
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
 * @brief  タイマ登録
 * @note   タイマを登録する
 *         - タイマ登録
 *         - タイマ管理テーブル追加
 * @param  tmk    [in]  タイマ種別
 * @return 終了コード
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
UINT f_trp_rec_Fc03_t_timer_ent_3g(                            /* 終了コード:R */
    ET_REC_TMK tmk                                          /* タイマ種別:I */
) {
	INT                    sit_ret;                           /* 終了コード */
    UINT                   uit_ret;                           /* 終了コード */
    T_REC_TIMELM           elm;                               /* タイマ要素 */
    bpf_ru_hrtm_key_inf_t     key_inf;                       /* タイマキー情報 */
    bpf_ru_hrtm_notify_type_t ntc_type;                            /* 通知種別 */
    UINT                   tim_id;                          /* タイマ識別子 */
    UINT                   tim_val;                             /* タイマ値 */
    INT                    errcd;                           /* エラーコード */
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

    /****************************/
    /* タイマ種別→通知種別変換 */
    /****************************/
    ntc_type = f_trp_rec_Fcom_t_tmk_ntc_cnv(                  /* 通知種別:R */
        tmk                                                 /* タイマ種別:I */
    );

    /****************************/
    /* タイマ種別→タイマ値変換 */
    /****************************/
    tim_val = f_trp_rec_Fcom_t_tmk_val_cnv(                   /* タイマ値:R */
        tmk                                                 /* タイマ種別:I */
    );

    tim_id = f_trp_rec_Fcom_t_timmng_tbl03_get(
        elm                                                 /* タイマ要素:I  */
    );
    /* Time Id not found */
    if (CMD_NUM0 == tim_id) {
        return CMD_NG;
    }

    /**************/
    /* タイマ登録 */
    /**************/
    sit_ret = f_trp_com_Fregister_tim(                      /* 終了コード:R */
        ntc_type,                                             /* 通知種別:I */
        tim_val,                                              /* タイマ値:I */
        key_inf,                                              /* キー情報:I */
        tim_id,                                           /* タイマ識別子:I */
        &errcd                                            /* 詳細NGコード:O */
    );
    /* タイマ登録NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][crr:0x%02d][ret:0x%02x]Timer ent NG.", D_REC_C03_CPR_NO(), D_REC_C03_CRR_NO(), sit_ret);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(&key_inf, sizeof(key_inf));
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
        return CMD_NG;
    }

    /**************************/
    /* タイマ管理テーブル追加 */
    /**************************/
    uit_ret = f_trp_rec_Fcom_t_timmng_tbl_add(              /* 終了コード:R */
        &key_inf,                                             /* キー情報:I */
        tim_id                                            /* タイマ識別子:I */
    );
    /* タイマ管理テーブル追加NGの場合 */
    if (uit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][crr:%02d][ret:0x%02x]Timer table ent NG.", D_REC_C03_CPR_NO(), D_REC_C03_CRR_NO(), uit_ret); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(&key_inf, sizeof(key_inf));
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
        return CMD_NG;
    }

    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
