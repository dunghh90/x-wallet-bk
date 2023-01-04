/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_l3_recrrstsreq_snd.c
 * @brief  L3(REキャリア状態報告要求)送信
 * @date   2008/07/18 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  L3(REキャリア状態報告要求)送信
 * @note   L3(REキャリア状態報告要求)を送信する
 *         - バッファ取得
 *         - バッファ設定(CPRIヘッダ)
 *         - バッファ設定(CPRIデータ)
 *         - スレッド間送信
 * @param  cpr_no [in]  CPRI番号
 * @return 終了コード
 * @retval 0:OK
 * @retval 1:NG
 * @date   2008/07/18 FFCS)Shihzh create.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_l3_recrrstsreq_snd_3g(                   /* 終了コード:R */
    USHORT cpr_no                                             /* CPRI番号:I */
) {
    CMT_TSKIF_CPRISND_CARLSTSREQ* bufp;                 /* バッファポインタ */
	UINT   msglen  = sizeof(CMT_CPRIF_CARLSTSREQ);          /* メッセージ長 */
    UINT   msgid   = CMD_TSKIF_CPRISNDIND;                  /* メッセージID */
    USHORT thrino  = D_TCM_THRINO_LPB;                  /* スレッド内部番号 */
    //T_MSG  sndbf;
    //T_MSG* sndbf_p = &sndbf;
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************/
    /* バッファ取得 */
    /****************/
    sit_ret = f_trp_com_Falloc_buf(                         /* 終了コード:R */
        sizeof(*bufp),                                  /* バッファサイズ:I */
        (VOID*)&bufp                                  /* バッファアドレス:O */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
#ifndef D_OPT_TRIF_BUFNGLOG_INVALID
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][siz:%u][ret:0x%02x]Alloc buffer NG.", cpr_no, sizeof(*bufp), sit_ret);
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
        return CMD_NG;
    }

    /****************************/
    /* バッファ設定(CPRIヘッダ) */
    /****************************/
    bufp->cprisnd_inf.link_num = cpr_no;                        /* CPRI番号 */
    bufp->cprisnd_inf.length   = CMD_CPRLN_CARLSTSREQ;       /* buffer length */

    /****************************/
    /* バッファ設定(CPRIデータ) */
    /****************************/
    bufp->cpridat_carstsreq.signal_kind = CMD_CPRID_CARLSTSREQ;
                                                                /* 信号種別 */

    //sndbf_p->data = (VOID*)bufp;

    /******************/
    /* スレッド間送信 */
    /******************/
    sit_ret = f_trp_rec_Fcom_t_it_snd(                      /* 終了コード:R */
        //sndbf_p,                                         /* バッファポインタ:I */
        (VOID*)bufp,
        msgid,                                            /* メッセージID:I */
        thrino,                                       /* スレッド内部番号:I */
        msglen                                            /* メッセージ長:I */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IT send NG.", cpr_no, sit_ret);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

        /****************/
        /* バッファ解放 */
        /****************/
        f_trp_com_Ffree_buf(                                      /* なし:R */
            bufp                                      /* バッファアドレス:I */
        );

        return CMD_NG;
    }

	/*	キャリア状態報告要求送信ログ格納	*/
//	f_trp_rec_Fcom_t_l3_LogStore( D_REC_LOGKIND_CARRIER, cpr_no, D_REC_MSGKIND_REQ );

    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/