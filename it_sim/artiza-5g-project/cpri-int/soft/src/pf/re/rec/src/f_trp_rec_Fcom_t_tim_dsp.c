/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_tim_dsp.c
 * @brief  タイマ表示
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
 * @brief  タイマ表示
 * @note   タイマを表示する
 * @param  stream [in]  ストリーム
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2007/06/07 FJT)Nagasima CR-00019-001
 * @date   2015/09/07 FPT)Lay modify for FHM
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_tim_dsp(                                    /* なし:R */
    FILE* stream                                            /* ストリーム:I */
) {
    USHORT         tim_num;                                     /* タイマ数 */
    USHORT         tim_idx;                           /* タイマインデックス */
    T_REC_TIMMNG_INF* infp;                           /* タイマ情報ポインタ */
    T_REC_TIMELM   elm;                                       /* タイマ要素 */
    CHAR           cpr_as[CMD_NUM16];                    /* CPRI番号(ASCII) */
    CHAR           soc_as[CMD_NUM16];  /* スロット番号orキャリア番号(ASCII) */
    CHAR           cls_nm[EC_REC_CLS_MAX][CMD_NUM4] = {         /* クラス名 */
        "COM",
        "C01",
        "C02",
        "C03",
        "C04",
        "C05",
        "C06",
        "C07",
        "C08",
        "CFF"
    };
    CHAR           tmk_nm[EC_REC_TMK_MAX][CMD_NUM16] = {    /* タイマ種別名 */
        "RETIMSET_INT",  /* 00 */
        "__TOFVAL_INT",  /* 01 */
        "__TOFVAL_CNF",  /* 02 */
        "REEQPINF_INT",  /* 03 */
        "REEQPINF_CNF",  /* 04 */
        "RESLTINF_CNF",  /* 05 */
        "___RESTS_INT",  /* 06 */
        "___RESTS_CNF",  /* 07 */
        "REPRTSTS_INT",  /* 08 */
        "REPRTSTS_CNF",  /* 09 */
        "RECRDSSC_INT",  /* 10 */
        "RECRDSSC_CNF",  /* 11 */
        "RECRDSCM_INT",  /* 12 */
        "RECRDSCM_CNF",  /* 13 */
        "RECRRSTS_INT",  /* 14 */
        "RECRRSTS_CNF",  /* 15 */
        "___RERST_CNF",  /* 16 */
        "REFRCRST_CNF",  /* 17 */
        "__2NDRST_WAT",  /* 18 */
        "__ACTCHG_WAT",  /* 19 */
        "__T14RTY_WAT",  /* 20 */
        "_____TXS_CNF",  /* 21 */
        "_____TXR_CNF",  /* 22 */
        "_____NMC_CNF",  /* 23 */
        "___REVER_CNF",  /* 24 */
        "RECRDCTL_CNF",  /* 25 */
        "RECRDDIA_CNF",  /* 26 */
        "REPRTCTL_CNF",  /* 27 */
        "RELOGGET_CNF",  /* 28 */
        "RELOGSTP_CNF",  /* 29 */
        "RELOGSND_CNF",  /* 30 */
        "_REFILUP_CNF",  /* 31 */
        "REAUTRST_WAT"   /* 32 */
    };

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* ストリームがNULLの場合 */
    if (stream == NULL) {
        /* 標準出力に変更 */
        stream = stdout;
    }

    fprintf( stream, "## REC TIM DSP START ##\n");

    /* タイマテーブル検索 */
    tim_num = f_trp_rec_Wcom_timmng_tbl.num;
    for (tim_idx = 0; tim_idx < tim_num; tim_idx++) {
        infp = &f_trp_rec_Wcom_timmng_tbl.inf[tim_idx];
        /* タイマキー情報→要素変換 */
        f_trp_rec_Fcom_t_key_elm_cnv(infp->key_inf.keyinf1, &elm);
        /* 初期値(ASCII)設定 */
        strcpy(cpr_as, "        ");
        strcpy(soc_as, "        ");
        /* タイマ種別で分岐 */
        switch (elm.tmk) {
        case EC_REC_TMK_2NDRST_WAT:
            /* なし */
            break;
        case EC_REC_TMK_TXS_CNF:
        case EC_REC_TMK_TXR_CNF:
        case EC_REC_TMK_NMC_CNF:
            /* CPRI番号・キャリア番号 */
            sprintf(cpr_as, "cpr:0x%02x", elm.cpr_no);
            sprintf(soc_as, "crr:0x%02x", elm.soc_no);
            break;
        case EC_REC_TMK_REVER_CNF:
        case EC_REC_TMK_RECRDCTL_CNF:
        case EC_REC_TMK_RECRDSSC_CNF:
        case EC_REC_TMK_RECRDSCM_CNF:
            /* CPRI番号・スロット番号 */
            sprintf(cpr_as, "cpr:0x%02x", elm.cpr_no);
            sprintf(soc_as, "slt:0x%02x", elm.soc_no);
            break;
        default:
            /* CPRI番号 */
            sprintf(cpr_as, "cpr:0x%02x", elm.cpr_no);
            break;
        }
        /* 表示 */
        fprintf( stream, "[%s][%s][%s][%s]][tid:0x%08x]\n",
            cls_nm[elm.cls], tmk_nm[elm.tmk], cpr_as, soc_as, infp->tim_id);
    }

    fprintf( stream, "## REC TIM DSP END   ##\n");

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
