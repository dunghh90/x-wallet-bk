/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_reghis_dsp.c
 * @brief  REGHIS表示
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
 * @brief  REGHIS表示
 * @note   REGHISを表示する
 * @param  stream [in]  ストリーム
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/09/07 FPT)Lay modify for FHM
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_reghis_dsp(                           /* 終了コード:R */
    FILE* stream                                            /* ストリーム:I */
) {
    UINT          idx;                                /* REGHISインデックス */
    UINT          cnt;                                          /* カウンタ */
    T_REC_REGHIS* infp;                               /* REGHIS情報アドレス */
    CHAR          tim_as[CMD_NUM32];                         /* 時刻(ASCII) */
    CHAR          adr_as[CMD_NUM16];                     /* アドレス(ASCII) */
    CHAR          val_as[CMD_NUM16];                           /* 値(ASCII) */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* ストリームがNULLの場合 */
    if (stream == NULL) {
        /* 標準出力に変更 */
        stream = stdout;
    }

    fprintf( stream, "## REC REGHIS DSP START ##\n");

    /* REGHISインデックス取得 */
    idx = f_trp_rec_Wcom_reghis_tbl.wrt_idx + CMD_NUM1;
    /* REGHISテーブルループ */
    for (cnt = CMD_NUM0; cnt < D_REC_REGHIS_NUM - CMD_NUM1; cnt++) {
        /* REGHIS情報アドレス取得 */
        infp = &(f_trp_rec_Wcom_reghis_tbl.inf[idx]);
        /* 月が、0の場合 */
        if (infp->head.day == CMD_NUM0) {
            /* 表示しない */
            D_REC_INCLBK(idx, D_REC_REGHIS_NUM);
            continue;
        }
        /************************/
        /* 表示用キャラクタ作成 */
        /************************/
        /* 時刻 */
        sprintf( tim_as, "%02x/%02x/%02x %02x:%02x:%02x.%02x%02x",
            infp->head.year, infp->head.month, infp->head.day,   infp->head.hour,
            infp->head.min,  infp->head.sec,   infp->head.msec1, infp->head.msec2
        );
        /* レジスタアドレス */
        sprintf( adr_as, "0x%08x", (UINT)(infp->head.adr));
        /* レジスタ値 */
        sprintf( val_as, "0x%08x", (UINT)(infp->head.val));

        /**************/
        /* REGHIS表示 */
        /**************/
        fprintf( stream, "[%s][adr:%s][val:%s]\n", tim_as, adr_as, val_as);
        D_REC_INCLBK(idx, D_REC_REGHIS_NUM);
    }

    fprintf( stream, "## REC REGHIS DSP END   ##\n");

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
