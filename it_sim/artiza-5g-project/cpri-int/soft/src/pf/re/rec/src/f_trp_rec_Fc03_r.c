/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_r.c
 * @brief  ルート
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
 * @brief  ルート
 * @note   イベント受信時の処理を行う
 *         - イベント変換
 *         - 状態番号取得
 *         - マトリクス関数起動
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @TBD_No 修正未完了
 */
/****************************************************************************/
VOID f_trp_rec_Fc03_r(                                            /* なし:R */
   VOID* bufp                                         /* バッファポインタ:I */
) {
    ET_REC_C03_EVT evt;                                         /* イベント */
    ET_REC_C03_STN stn;                                         /* 状態番号 */
    USHORT         evt_cpr_no;                          /* イベントCPRI番号 */
    USHORT         cpr_no;                                      /* CPRI番号 */
    USHORT         cpr_no_min;                               /* CPRI番号MIN */
    USHORT         cpr_no_max;                               /* CPRI番号MAX */
    USHORT         evt_crr_no;                      /* イベントキャリア番号 */
    USHORT         crr_no;                                  /* キャリア番号 */
    USHORT         crr_no_min;                           /* キャリア番号MIN */
    USHORT         crr_no_max;                           /* キャリア番号MAX */
    CHAR          errdata[CMD_NUM48];

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************/
    /* イベント変換 */
    /****************/
    evt = f_trp_rec_Fc03_r_evt_cnv(                           /* イベント:R */
        bufp,                                         /* バッファポインタ:I */
        &evt_cpr_no,                                          /* CPRI番号:O */
        &evt_crr_no                                       /* キャリア番号:O */
    );
    /* イベントなしの場合 */
    if (evt == EC_REC_C03_EVT_NON) {
        /* 終了 */
        return;
    }

    /***********************/
    /* CPRI番号MIN/MAX設定 */
    /***********************/
    /* ALLの場合 */
    if (evt_cpr_no == D_REC_CPRINO_ALL) {
        cpr_no_min = CMD_CPRINO_MIN;
        cpr_no_max = CMD_CPRINO_MAX;
    }
    /* その他の場合 */
    else {
        cpr_no_min = evt_cpr_no;
        cpr_no_max = evt_cpr_no;
    }
    /***************************/
    /* キャリア番号MIN/MAX設定 */
    /***************************/
    /* ALLの場合 */
    if (evt_crr_no == D_REC_CARNO_ALL) {
        crr_no_min = CMD_CARNO_MIN;
        crr_no_max = CMD_CARNO_MAX;
    }
    /* その他の場合 */
    else {
        crr_no_min = evt_crr_no;
        crr_no_max = evt_crr_no;
    }
    if ( (cpr_no_min < CMD_CPRINO_MIN) ||
         (cpr_no_max > CMD_CPRINO_MAX) ||
         (crr_no_min < CMD_CARNO_MIN ) ||
         (crr_no_max > CMD_CARNO_MAX ) ){
         snprintf(errdata, CMD_NUM48, "[ERR][cpr_min:%02d][cpr_max:%02d][crr_min:%02d][crr_max:%02d] NG", cpr_no_min, cpr_no_max, crr_no_min, crr_no_max); /* pgr0570 */
         D_REC_IVLINI();
         D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
         D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
         return;
    }
    /* 必要CPRI番号数分ループ */
    for (cpr_no = cpr_no_min; cpr_no <= cpr_no_max; cpr_no++) {
        /* パラメータテーブル設定(CPRI番号) */
        f_trp_rec_Wc03_prm_tbl.cpr_no = cpr_no;
        /* 必要キャリア番号数分ループ */
        for (crr_no = crr_no_min; crr_no <= crr_no_max; crr_no++) {
            /* パラメータテーブル設定(キャリア番号) */
            f_trp_rec_Wc03_prm_tbl.crr_no = crr_no;

            /****************/
            /* イベント設定 */
            /****************/

            f_trp_rec_Fc03_t_evt_set(                            /* なし:R */
                evt                                           /*イベント:I */
            );

            /****************/
            /* 状態番号取得 */
            /****************/
            stn = f_trp_rec_Fc03_t_ownstn_get(                /* 状態番号:R */
            );

            /**********************/
            /* マトリクス関数起動 */
            /**********************/
            f_trp_rec_Rc03_m_fnc[evt][stn](bufp);
        }
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
