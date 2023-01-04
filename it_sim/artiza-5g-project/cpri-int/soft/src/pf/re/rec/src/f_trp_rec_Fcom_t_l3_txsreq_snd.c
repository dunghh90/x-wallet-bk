/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_l3_txsreq_snd.c
 * @brief  L3(TRX設定要求)送信
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
 * @brief  L3(TRX設定要求)送信
 * @note   L3(TRX設定要求)を送信する
 *         - バッファ取得
 *         - バッファ設定(CPRIヘッダ)
 *         - バッファ設定(CPRIデータ)
 *         - スレッド間送信
 * @param  cpr_no [in]  CPRI番号
 * @param  crr_no [in]  キャリア番号
 * 
 * 
 * 
 * @return 終了コード
 * @retval 0:OK
 * @retval 1:NG
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/10/29 FPT)DuongCD update for LTE(FDD).
 * @date   2015/11/08 FPT)Yen fix bug IT3 No58.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_l3_txsreq_snd(                        /* 終了コード:R */
    USHORT cpr_no,                                            /* CPRI番号:I */
    USHORT crr_no                                        /* キャリア番号:I */
) {
    CMT_TSKIF_CPRIRCV_TRXSET_S3G_REQ* bufp;           /* バッファポインタ */
	UINT   msglen;         /* メッセージ長 */
    UINT   msgid  = CMD_TSKIF_CPRISNDIND;                   /* メッセージID */
    USHORT thrino = D_TCM_THRINO_LPB;                   /* スレッド内部番号 */
    //T_MSG  sndbf;
    //T_MSG* sndbf_p = &sndbf;
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];
	USHORT frq = CMD_NUM0;                                                /* 周波数:I */
    USHORT bnd = CMD_NUM0;                                         /* システム帯域幅:I */
    USHORT brc = CMD_NUM0;                                                 /* 送信系:I */
    USHORT tdd_up = CMD_NUM0;                                              /* 送信系:I */
    USHORT tdd_spe = CMD_NUM0;                                             /* 送信系:I */
    USHORT tdd_dwnlk = CMD_NUM0;                                            /* 送信系:I */
    T_TRXSET_PARA_LTE           ltTrxSetPara;
    UINT                        lwMsgSize;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* get frq, bnd, brc, tdd_up, tdd_spe, tdd_dwnlk from shared memory since TRX set OK */
    memset(&ltTrxSetPara, 0, sizeof(ltTrxSetPara));
    f_cmn_com_trxset_para_lte_get(crr_no, &ltTrxSetPara);

    frq = ltTrxSetPara.wDlFreqNo;
    bnd = ltTrxSetPara.wBandWidth;
    brc = ltTrxSetPara.wSend;
    tdd_up = ltTrxSetPara.wTddUlDlConfig;
    tdd_spe = ltTrxSetPara.wTddSpeSubConfig;
    tdd_dwnlk = ltTrxSetPara.wTddDlCycPreLen;
    msglen = ltTrxSetPara.wdatSize;
    lwMsgSize = msglen + sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF);

    /****************/
    /* バッファ取得 */
    /****************/
    sit_ret = f_trp_com_Falloc_buf(                         /* 終了コード:R */
        lwMsgSize,         /* バッファサイズ:I */
        (VOID*)&bufp                                  /* バッファアドレス:O */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
#ifndef D_OPT_TRIF_BUFNGLOG_INVALID
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][crr:%02d][siz:%u][ret:0x%02x]Alloc buffer NG.", cpr_no, crr_no, lwMsgSize, sit_ret); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
        return CMD_NG;
    }

    /****************************/
    /* バッファ設定(CPRIヘッダ) */
    /****************************/
    bufp->cprircv_inf.link_num = cpr_no;                        /* CPRI番号 */
    bufp->cprircv_inf.length   = msglen;                   /* buffer length */

    /****************************/
    /* バッファ設定(CPRIデータ) */
    /****************************/
    bufp->cpridat_trxsetreq.signal_kind = CMD_CPRID_TRXSETREQ + CMD_NUM1;  /* 信号種別 */
    bufp->cpridat_trxsetreq.carno       = crr_no;           /* キャリア番号 */
    bufp->cpridat_trxsetreq.dl_freq     = frq;                    /* 周波数 */
    bufp->cpridat_trxsetreq.bandwidth   = bnd;            /* システム帯域幅 */
    bufp->cpridat_trxsetreq.snd_br      = brc;                   /*  送信系 */
    /* TDDの場合 */
    if(msglen >= sizeof(CMT_CPRIF_TRXSETREQ_S3G))
    {
	    bufp->cpridat_trxsetreq.tdd_up_dwn_config = tdd_up;
	    bufp->cpridat_trxsetreq.tdd_spe_sf_config = tdd_spe;
	    bufp->cpridat_trxsetreq.tdd_dwnlk_cp_len  = tdd_dwnlk;
	}

    /******************/
    /* スレッド間送信 */
    /******************/
    sit_ret = f_trp_rec_Fcom_t_it_snd(                      /* 終了コード:R */
        (VOID*)bufp,
        msgid,                                            /* メッセージID:I */
        thrino,                                       /* スレッド内部番号:I */
        msglen                                            /* メッセージ長:I */
    );
    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][crr:%02d][ret:0x%02x]IT send NG.", cpr_no, crr_no, sit_ret);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, lwMsgSize);
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
