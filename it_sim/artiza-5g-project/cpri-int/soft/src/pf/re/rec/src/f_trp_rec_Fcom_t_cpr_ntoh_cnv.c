/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_cpr_ntoh_cnv.c
 * @brief  CPRI信号バイトオーダー(NTOH)変換
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
 * @brief  CPRI信号バイトオーダー(NTOH)変換
 * @note   CPRI信号バイトオーダー(NTOH)を変換する
 * @param  datp [i/o] データポインタ
 * @param  len  [in]  レングス
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2009/03/31 FFCS)Shihzh M-S3G-eNBPF-01639.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_cpr_ntoh_cnv(                               /* なし:R */
    VOID* datp,                                        /* データポインタ:IO */
    UINT  len                                                 /* レングス:I */
) {
    USHORT  sig_kind;                                           /* 信号種別 */
    CMT_CPRIF_REVERRES      *revp;  /* データポインタ(REバージョン報告応答) */
    CMT_CPRIF_REOBSLOGRES   *obsp;           /* RE Trouble Log Get Response */
    USHORT  rev_inf_num;                    /* 情報数(REバージョン報告応答) */
    USHORT  rev_inf_cnt;              /* インデックス(REバージョン報告応答) */
    REVER_INF* verp;                     /* データポインタ(バージョン情報詳細 */
    USHORT* ustp;                                  /* データポインタ(2byte) */
    UINT    dat_cnt;                                  /* データ設定カウンタ */
  
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 信号種別抽出 */
    sig_kind = ntohs(*((USHORT*)datp));
    /* 信号種別で分岐 */
    switch (sig_kind) {
    /* REバージョン報告応答 */
    case CMD_CPRID_REVERRES:
        /* 型の載せ換え */
        revp = datp;
        revp->signal_kind       = ntohs(revp->signal_kind);     /* 信号種別 */
        revp->result            = ntohs(revp->result);          /* 応答結果 */
        /* REバージョン報告詳細 */
                                             /* メーカ名はUCHARなので無変換 */
        revp->rests_inf.kind    = ntohs(revp->rests_inf.kind);  /* 対象指定 */
        revp->rests_inf.slot_no = ntohs(revp->rests_inf.slot_no);
                                                            /* スロット番号 */
        if (revp->result == CMD_NML) {
                                           /* ID情報識別はUCHARなので無変換 */
            revp->rests_inf.inf_num = ntohs(revp->rests_inf.inf_num); 
                                                                  /* 情報数 */
            /* REバージョン情報詳細 */
            rev_inf_num = revp->rests_inf.inf_num;
            verp = (REVER_INF*)(((USHORT*)(&(revp->rests_inf.inf_num))) + CMD_NUM1);
            for (rev_inf_cnt = CMD_NUM0;
                 rev_inf_cnt < rev_inf_num;
                 rev_inf_cnt++) {
                                                 /* 名称はUCHARなので無変換 */
                verp->mjr_ver   = ntohs(verp->mjr_ver);   
                                                      /* メジャーバージョン */
                verp->mir_ver   = ntohs(verp->mir_ver);  
                                                      /* マイナーバージョン */
                verp->year      = ntohs(verp->year);                  /* 年 */
                verp->month_day = ntohs(verp->month_day);           /* 月日 */
                verp++;
            }
        }
        break;

    /* RE Card Status Response */
    case CMD_CPRID_CARDSTSRES:
        /* 型の載せ換え */
        ustp = datp;
        /* signal_kind(2)--result(2)--slot no(2) */
        for (dat_cnt = CMD_NUM0; dat_cnt < CMD_NUM3; dat_cnt++) {
            *ustp = ntohs(*ustp);
            ustp++;
        }
        /* make name end (8) */
        ustp += CMD_NUM4;
        /* Year-> */
        for (dat_cnt += CMD_NUM4; dat_cnt < len/CMD_NUM2; dat_cnt++) {
            *ustp = ntohs(*ustp);
            ustp++;
        }
        break;

    /* RE trouble log get response */
    case CMD_CPRID_REOBSLOGRES:
        /* 型の載せ換え */
        obsp = datp;
        obsp->signal_kind = ntohs(obsp->signal_kind);
        obsp->result      = ntohs(obsp->result);
        obsp->datsize     = ntohl(obsp->datsize);
        break;

    /* File Data Sending */
    case CMD_CPRID_FILEDATSND:
        /* 型の載せ換え */
        ustp = datp;
        /* signal_kind(2)--segment No(4)--data size(4) */
        for (dat_cnt = CMD_NUM0; dat_cnt < CMD_NUM5; dat_cnt++) {
            *ustp = ntohs(*ustp);
            ustp++;
        }
        break;
        
    /* ALL2byte要素メッセージ */
    default:
        /* 型の載せ換え */
        ustp = datp;
        for (dat_cnt = CMD_NUM0; dat_cnt < len/CMD_NUM2; dat_cnt++) {
            *ustp = ntohs(*ustp);
            ustp++;
        }
        break;
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
