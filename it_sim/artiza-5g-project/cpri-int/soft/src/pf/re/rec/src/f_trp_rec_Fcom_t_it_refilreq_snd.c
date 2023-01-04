/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_it_refilreq_snd.c
 * @brief  スレッド間(REファイル更新要求)送信
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2009
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  スレッド間(REファイル更新要求)送信
 * @note   スレッド間(REファイル更新要求)を送信する
 *         - バッファ取得
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  cpr_no   [in]  CPRI番号
 * @param  sys_type [in]  Systerm type ( 3G/LTE)
 * @param  kind     [in]  Download Kind
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2009/03/04 FFCS)Shihzh CR-00031-005.
 * @date   2015/08/21 FPT)Yen update for FHM
 * @date   2015/10/08 FPT)Duong Update MKレビュー指摘No.243対応
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_it_refilreq_snd(                            /* なし:R */
    USHORT cpr_no,                                            /* CPRI番号:I */
    USHORT sys_type,                                      /* Systerm type:I */
    USHORT kind                                          /* Download Kind:I */
) {
    CMT_TSKIF_REFILNEWREQ  buf;
    CMT_TSKIF_REFILNEWREQ* bufp = &buf;                 /* バッファポインタ */
    UINT   msgid  = CMD_TSKIF_REFIRM_DL_REQ;                /* メッセージID */
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];
	USHORT	lwLength;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* バッファ設定(データ) */
    /************************/
    bufp->cpri_no  = cpr_no;                                    /* CPRI番号 */
    bufp->sys_type = sys_type;                              /* Systerm type */
    bufp->kind     = kind;                                          /* kind */

    /******************/
    /* スレッド間送信 */
    /******************/
	lwLength = sizeof(CMT_TSKIF_REFILNEWREQ);

	/* REファイル更新要求を送信 */
	sit_ret = f_trp_rec_Fcom_t_msg_snd( bufp, msgid, D_RRH_PROCQUE_L3, 0, lwLength);

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
