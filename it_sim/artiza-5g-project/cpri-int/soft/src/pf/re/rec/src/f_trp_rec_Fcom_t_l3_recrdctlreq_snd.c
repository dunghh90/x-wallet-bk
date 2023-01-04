/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_l3_recrdctlreq_snd.c
 * @brief  L3(REカード制御要求)送信
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
 * @brief  L3(REカード制御要求)送信
 * @note   L3(REカード制御要求)を送信する
 *         - バッファ取得
 *         - バッファ設定(CPRIヘッダ)
 *         - バッファ設定(CPRIデータ)
 *         - スレッド間送信
 * @param  cpr_no   [in]  CPRI番号
 * @param  slt_no   [in]  スロット番号
 * @param  ctl_kind [in]  制御種別
 * @return 終了コード
 * @retval 0:OK
 * @retval 1:NG
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2010/06/02 FJT)Tokunaga TS-514項 M-S3G-eNBSYS-01559
 * @date   2015/11/17 FPT)Tuan update.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_l3_recrdctlreq_snd(                   /* 終了コード:R */
    USHORT cpr_no,                                            /* CPRI番号:I */
    USHORT slt_no,                                        /* スロット番号:I */
    USHORT ctl_kind                                           /* 制御種別:I */
) {
    CMT_TSKIF_CPRISND_RECARDCNTREQ* bufp;               /* バッファポインタ */
	UINT   msglen  = sizeof(CMT_CPRIF_RECARDCNTREQ);		/* メッセージ長 */
    UINT   msgid   = CMD_TSKIF_CPRISNDIND;                  /* メッセージID */
    USHORT thrino  = D_TCM_THRINO_LPB;                  /* スレッド内部番号 */
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
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][slt:0x%02x][siz:0x%08x][ret:0x%02x]Alloc buffer NG.", cpr_no, slt_no, (UINT)sizeof(*bufp), sit_ret); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
        return CMD_NG;
    }

    /****************************/
    /* バッファ設定(CPRIヘッダ) */
    /****************************/
    bufp->cprisnd_inf.link_num = cpr_no;                        /* CPRI番号 */
    bufp->cprisnd_inf.length   = msglen;                   /* buffer length */

    /****************************/
    /* バッファ設定(CPRIデータ) */
    /****************************/
    bufp->cpridat_recardcntreq.signal_kind = CMD_CPRID_RECARDCNTREQ + CMD_NUM1;
                                                                /* 信号種別 */
    bufp->cpridat_recardcntreq.slot_no     = slt_no;        /* スロット番号 */
    bufp->cpridat_recardcntreq.cnt_kind    = ctl_kind;          /* 制御種別 */

	/******************/
    /* スレッド間送信 */
    /******************/
    sit_ret = f_trp_rec_Fcom_t_it_snd(                      /* 終了コード:R */
        (VOID*)bufp,                                  /* バッファポインタ:I */
        msgid,                                            /* メッセージID:I */
        thrino,                                       /* スレッド内部番号:I */
        msglen                                            /* メッセージ長:I */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][slt:0x%02x][ret:0x%02x]IT send NG.", cpr_no, slt_no, sit_ret);
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

    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
