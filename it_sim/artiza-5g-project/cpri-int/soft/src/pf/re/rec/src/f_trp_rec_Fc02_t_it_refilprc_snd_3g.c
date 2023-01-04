/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_it_refilprc_snd_3g.c
 * @brief  スレッド間(REファイル更新プロシージャ実行指示)送信
 * @date   2015/09/26 TDIPS)sasaki create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  スレッド間(REファイル更新プロシージャ実行指示)送信
 * @note   スレッド間(REファイル更新プロシージャ実行指示)を送信する
 *         - バッファ取得
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  -
 * @return None
 * @date   2015/09/26 TDIPS)sasaki create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_refilprc_snd_3g(                         /* なし:R */
) {
    CMT_TSKIF_REFILPRCIND  buf;
    CMT_TSKIF_REFILPRCIND* bufp = &buf;                 /* バッファポインタ */
    UINT   msglen = sizeof(CMT_TSKIF_REFILPRCIND) - sizeof(T_MSGHEAD);
                                                            /* メッセージ長 */
    UINT   msgid  = CMD_TSKIF_REFILPRCIND_3G;               /* メッセージID */
    USHORT thrino = D_TCM_THRINO_REC;                   /* スレッド内部番号 */
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* バッファ設定(データ) */
    /************************/
    bufp->cpri_no = D_REC_C02_3G_CPR_NO();                      /* CPRI番号 */

    /******************/
    /* スレッド間送信 */
    /******************/
    sit_ret = f_trp_rec_Fcom_t_it_snd(                      /* 終了コード:R */
        bufp,                                         /* バッファポインタ:I */
        msgid,                                            /* メッセージID:I */
        thrino,                                       /* スレッド内部番号:I */
        msglen                                            /* メッセージ長:I */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IT send NG.", D_REC_C02_3G_CPR_NO(), sit_ret);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
