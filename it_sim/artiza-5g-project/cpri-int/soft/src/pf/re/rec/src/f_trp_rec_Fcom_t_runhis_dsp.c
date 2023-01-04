/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_runhis_dsp.c
 * @brief  RUNHIS表示
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
 * @brief  RUNHIS表示
 * @note   RUNHISを表示する
 * @param  stream [in]  ストリーム
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/09/03 FPT)Lay modify for dump Run History to file
 * @date   2015/11/10 FPT)Yen IT2問処170の対応
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_runhis_dsp(                                 /* なし:R */
    FILE* stream                                            /* ストリーム:I */
) {
    UINT          idx;                                /* RUNHISインデックス */
    UINT          cnt;                                          /* カウンタ */
    T_REC_RUNHIS* infp;                               /* RUNHIS情報アドレス */
    CHAR          tim_as[CMD_NUM32];                         /* 時刻(ASCII) */
    CHAR          msg_as[CMD_NUM16];                 /* メッセージID(ASCII) */
    CHAR          sig_as[CMD_NUM8];                      /* 信号種別(ASCII) */
    CHAR          rek_as[CMD_NUM8];                        /* RE番号(ASCII) */
    CHAR          cpr_as[CMD_NUM8];                      /* CPRI番号(ASCII) */
    CHAR          soc_as[CMD_NUM8];    /* スロット番号orキャリア番号(ASCII) */
    CHAR          ret_as[CMD_NUM8];                    /* 結果コード(ASCII) */
    CHAR          fbn_as[CMD_NUM16];                       /* FB番号(ASCII) */
    CHAR          ngf_as[CMD_NUM8];                        /* NG要因(ASCII) */
    UINT          log_idx;                          /* 強化ログインデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* ストリームがNULLの場合 */
    if (stream == NULL) {
        /* 標準出力に変更 */
        stream = stdout;
    }

//    printf("## REC RUNHIS DSP START ##\n");

    /* RUNHISインデックス取得 */
    idx = f_trp_rec_Wcom_runhis_tbl.wrt_idx;// + CMD_NUM1;
	D_REC_INCLBK(idx, D_REC_RUNHIS_NUM);
    log_idx = f_trp_com_WlogWriteIndex;
    
    /* RUNHISテーブルループ */
    for (cnt = CMD_NUM0; cnt < D_REC_RUNHIS_NUM - CMD_NUM1; cnt++) {
        /* RUNHIS情報アドレス取得 */
        infp = &(f_trp_rec_Wcom_runhis_tbl.inf[log_idx][idx]);
        /* 月が、0の場合 */
        if (infp->head.day == CMD_NUM0) {
            /* 表示しない */
            D_REC_INCLBK(idx, D_REC_RUNHIS_NUM);
            continue;
        }
        /************************/
        /* 表示用キャラクタ作成 */
        /************************/
        /* 時刻 */
        sprintf(tim_as, "%02x/%02x/%02x %02x:%02x:%02x.%02x%02x",
            infp->head.year, infp->head.month, infp->head.day,   infp->head.hour,
            infp->head.min,  infp->head.sec,   infp->head.msec1, infp->head.msec2
        );
        /* メッセージID */
        sprintf(msg_as, "0x%08x", infp->head.msgid);
        /* 信号種別 */
        if (infp->head.sig_kind == CMD_ALLF_SHORT) {
            sprintf(sig_as, "      ");
        }
        else {
            sprintf(sig_as, "0x%04x", infp->head.sig_kind);
        }
        /* RE Kind */
        if (infp->head.re_kind == CMD_ALLF_CHAR) {
            sprintf(rek_as, "    ");
        }
        else {
            sprintf(rek_as, "0x%02x", infp->head.re_kind);
        }
        /* CPRI番号 */
        if (infp->head.cpr_no == CMD_ALLF_CHAR) {
            sprintf(cpr_as, "    ");
        }
        else {
            sprintf(cpr_as, "0x%02x", infp->head.cpr_no);
        }
        /* スロット番号orキャリア番号 */
        if (infp->head.soc_no == CMD_ALLF_CHAR) {
            sprintf(soc_as, "    ");
        }
        else {
            sprintf(soc_as, "0x%02x", infp->head.soc_no);
        }
        /* 結果コード */
        if (infp->head.ret_code == CMD_ALLF_CHAR) {
            sprintf(ret_as, "    ");
        }
        else {
            sprintf(ret_as, "0x%02x", infp->head.ret_code);
        }
        /* FB番号 */
        sprintf(fbn_as, "0x%08x", infp->head.fb_no);
        /* NG要因 */
        sprintf(ngf_as, "0x%04x", infp->head.rhk_ng_fct);

        /**************/
        /* RUNHIS表示 */
        /**************/
        /* RUNHIS種別で分岐 */
        switch (infp->head.rhk) {
        /* 受信 */
        case D_REC_RUNHIS_RHK_RCV:
            fprintf( stream, "[%s]<---[msg:%s][sig:%s][rek:%s][cpr:%s][soc:%s][ret:%s]----[src:%s]\n",
                tim_as, msg_as, sig_as, rek_as, cpr_as, soc_as, ret_as, fbn_as);
            break;
        /* 送信 */
        case D_REC_RUNHIS_RHK_SND:
            fprintf( stream, "[%s]----[msg:%s][sig:%s][rek:%s][cpr:%s][soc:%s][ret:%s]--->[dst:%s]\n",
                tim_as, msg_as, sig_as, rek_as, cpr_as, soc_as, ret_as, fbn_as);
            break;
        /* 送信NG */
        case D_REC_RUNHIS_RHK_SNG:
            fprintf( stream,  "[%s]--->[msg:%s][sig:%s][cpr:%s][soc:%s][ret:%s]--->[NG][ngf:%s]\n",
                tim_as, msg_as, sig_as, cpr_as, soc_as, ret_as, ngf_as);
            break;
        }
        D_REC_INCLBK(idx, D_REC_RUNHIS_NUM);
    }

//    printf("## REC RUNHIS DSP END   ##\n");

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
