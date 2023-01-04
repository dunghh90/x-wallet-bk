/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_it_frcstpntc_snd_3g.c
 * @brief  スレッド間(強制停止通知)送信
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
 * @brief  スレッド間(強制停止通知)送信
 * @note   スレッド間(強制停止通知)を送信する
 *         - バッファ取得
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  thrino     [in]  スレッド内部番号
 * @param  cpr_no     [in]  CPRI番号
 * @param  stopFactor [in]  強制停止要因
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/08/22 FPT)Yen update
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_it_frcstpntc_snd_3g(                           /* なし:R */
    USHORT thrino,                                    /* スレッド内部番号:I */
    USHORT cpr_no,                                            /* CPRI番号:I */
    T_RRH_RESTOP_FACTOR stopFactor
) {
    CMT_TSKIF_RECMPSTPNTC  buf;
    CMT_TSKIF_RECMPSTPNTC* bufp = &buf;                 /* バッファポインタ */
    UINT   msgid  = CMD_TSKIF_RECMPSTPNTC;                  /* メッセージID */
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];
	USHORT	lwLength;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* バッファ設定(データ) */
    /************************/
    bufp->link_num = cpr_no;                                     /* CPRI番号 */
    bufp->system_type = CMD_SYS_3G;                          /* Systerm type */
    bufp->stopFactor = stopFactor;

    /******************/
    /* スレッド間送信 */
    /******************/
	lwLength = sizeof(CMT_TSKIF_RECMPSTPNTC);
	
	/* 強制停止通知を送信 */
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
