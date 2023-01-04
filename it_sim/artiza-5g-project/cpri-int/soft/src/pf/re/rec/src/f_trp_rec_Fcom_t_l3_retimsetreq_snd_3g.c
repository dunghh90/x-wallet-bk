/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_l3_retimsetreq_snd_3g.c
 * @brief  L3(RE時刻設定要求)送信
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
 * @brief  L3(RE時刻設定要求)送信
 * @note   L3(RE時刻設定要求)を送信する
 *         - バッファ取得
 *         - バッファ設定(CPRIヘッダ)
 *         - バッファ設定(CPRIデータ)
 *         - スレッド間送信
 * @param  cpr_no [in]  CPRI番号
 * @return 終了コード
 * @retval 0:OK
 * @retval 1:NG
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_l3_retimsetreq_snd_3g(                   /* 終了コード:R */
    USHORT cpr_no                                             /* CPRI番号:I */
) {
    CMT_TSKIF_CPRISND_RETIMSETREQ* bufp;                /* バッファポインタ */
    UINT   msglen = sizeof(CMT_CPRIF_RETIMSETREQ);          /* メッセージ長 */
    UINT   msgid  = CMD_TSKIF_CPRISNDIND;                   /* メッセージID */
    USHORT thrino = D_TCM_THRINO_LPB;                   /* スレッド内部番号 */
    //T_MSG  sndbf;
    //T_MSG* sndbf_p = &sndbf;
    INT    sit_ret;                                           /* 終了コード */
    USHORT  year_bcd;                                      /* 年(BCDコード) */
    USHORT  mo___bcd;                                    /* 月__(BCDコード) */
    USHORT  __dy_bcd;                                    /* __日(BCDコード) */
    USHORT  mody_bcd;                                    /* 月日(BCDコード) */
    USHORT  hour_bcd;                                      /* 時(BCDコード) */
    USHORT  mn___bcd;                                    /* 分__(BCDコード) */
    USHORT  __sc_bcd;                                    /* __秒(BCDコード) */
    USHORT  mnsc_bcd;                                    /* 分秒(BCDコード) */
    CHAR  errdata[CMD_NUM48];
    T_RRH_SYSTEM_TIME sysTime;                              /* 年月日時分秒 */

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
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][siz:0x%08x][ret:0x%02x]Alloc buffer NG.", cpr_no, sizeof(*bufp), sit_ret); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_ALERT, errdata);
#endif /* D_OPT_TRIF_BUFNGLOG_INVALID */
        return CMD_NG;
    }

    /****************************/
    /* バッファ設定(CPRIヘッダ) */
    /****************************/
    bufp->cprisnd_inf.link_num = cpr_no;                        /* CPRI番号 */
    bufp->cprisnd_inf.length   = CMD_CPRLN_RETIMSETREQ;    /* buffer length */

    /****************************/
    /* バッファ設定(CPRIデータ) */
    /****************************/
    /* 時刻取得 */
    if (BPF_HM_DEVC_COMPLETE != BPF_HM_DEVC_GETTIME(&sysTime)) {
        return CMD_NG;
    }
    year_bcd = D_REC_CNVBCD(sysTime.year);
    mo___bcd = D_REC_CNVBCD(sysTime.month);
    __dy_bcd = D_REC_CNVBCD(sysTime.day);
    mody_bcd = (mo___bcd<<8) + __dy_bcd;
    hour_bcd = D_REC_CNVBCD(sysTime.hour);
    mn___bcd = D_REC_CNVBCD(sysTime.min);
    __sc_bcd = D_REC_CNVBCD(sysTime.sec);
    mnsc_bcd = (mn___bcd<<8) + __sc_bcd;

    bufp->cpridat_retimsetreq.signal_kind = CMD_CPRID_RETIMSETREQ;
                                                                /* 信号種別 */
    bufp->cpridat_retimsetreq.year        = year_bcd;                 /* 年 */
    bufp->cpridat_retimsetreq.month_day   = mody_bcd;               /* 月日 */
    bufp->cpridat_retimsetreq.hour        = hour_bcd;        /* (未使用+)時 */
    bufp->cpridat_retimsetreq.minute_sec  = mnsc_bcd;               /* 分秒 */

    //sndbf_p->data = (VOID*)bufp;

    /******************/
    /* スレッド間送信 */
    /******************/
    sit_ret = f_trp_rec_Fcom_t_it_snd(                      /* 終了コード:R */
        //sndbf_p,                                      /* バッファポインタ:I */
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

    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
