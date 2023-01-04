/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_stn_dsp.c
 * @brief  STN表示
 * @date   2007/04/25 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  STN表示
 * @note   STNを表示する
 * @param  stream [in]  ストリーム
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * @date   2015/09/07 FPT)Lay modify for FHM
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_stn_dsp(                                    /* なし:R */
    FILE* stream                                            /* ストリーム:I */
) {
    USHORT cpr_no;                                            /* CPRI番号:I */
    USHORT crr_no;                                        /* キャリア番号:I */
    USHORT cpr_no_idx;                            /* CPRI番号インデックス:I */
    USHORT crr_no_idx;                        /* キャリア番号インデックス:I */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* ストリームがNULLの場合 */
    if (stream == NULL) {
        /* 標準出力に変更 */
        stream = stdout;
    }

    fprintf( stream,  "## REC STN DSP START ##\n");

    /* C02 */
    for (cpr_no = CMD_CPRINO_MIN; cpr_no <= CMD_CPRINO_MAX; cpr_no++) {
        cpr_no_idx = cpr_no - CMD_NUM1;
        fprintf( stream, "[C02][cpr:0x%02x]          [stn:%02d][evt:%02d]\n",
            cpr_no,
            f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].stn,
            f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].evt);
    }

    /* C03 */
    for (cpr_no = CMD_CPRINO_MIN; cpr_no <= CMD_CPRINO_MAX; cpr_no++) {
        cpr_no_idx = cpr_no - CMD_NUM1;
        for (crr_no = CMD_CARNO_MIN; crr_no <= CMD_CARNO_MAX; crr_no++) {
            crr_no_idx = crr_no - CMD_NUM1;
            fprintf( stream, "[C03][cpr:0x%02x][crr:0x%02x][stn:%02d][evt:%02d][sct:0x%04x][frq:0x%04x][bnd:0x%04x]\n",
                cpr_no, crr_no,
                f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].stn,
                f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].evt,
                f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].sct_no,
                f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].frq,
                f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].bnd);
        }
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
