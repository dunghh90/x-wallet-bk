/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_it_redlstpind_snd.c
 * @brief  スレッド間(RE更新停止指示)送信
 * @date   2009/09/28 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2009
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  スレッド間(REファイル更新中止要求)送信
 * @note   スレッド間(REファイル更新中止要求)を送信する
 *         - バッファ取得
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  cpr_no   [in]  CPRI番号
 * @param  fact     [in]  Fact
 * @param  systerm  [in]  Systerm
 * @return None
 * @date   2009/09/28 FFCS)Shihzh create.
 * @date   2015/10/15 FPT)Duong update comment 291.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_it_redlstpind_snd(                          /* なし:R */
    USHORT cpr_no,                                            /* CPRI番号:I */
    USHORT fact,                                                   /* Fact:I */
    USHORT systerm                                              /* Systerm:I */
) {
    CMT_TSKIF_REFILNEWSTP  buf;
    CMT_TSKIF_REFILNEWSTP* bufp = &buf;                 /* バッファポインタ */
    UINT   msglen = sizeof(CMT_TSKIF_REFILNEWSTP);
                                                            /* メッセージ長 */
    UINT   msgid  = CMD_TSKIF_REFIRM_DL_STOP_REQ;           /* メッセージID */
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* バッファ設定(データ) */
    /************************/
    bufp->cpri_no  = cpr_no;                                    /* CPRI番号 */
//    bufp->fact     = fact;                                          /* Fact */
    bufp->systerm  = systerm;                                    /* Systerm */

    /******************/
    /* スレッド間送信 */
    /******************/
    /* RE更新停止指示を送信 */
	sit_ret = f_trp_rec_Fcom_t_msg_snd( bufp, msgid, D_RRH_PROCQUE_L3, 0, msglen);

    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IT send NG.", cpr_no, sit_ret);
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
