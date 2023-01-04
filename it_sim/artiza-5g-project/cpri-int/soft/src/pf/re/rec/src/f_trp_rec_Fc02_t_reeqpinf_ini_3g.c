/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_reeqpinf_ini_3g.c
 * @brief  RE状態(セクタ部)管理テーブル(RE装置構成情報)初期化
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE状態(セクタ部)管理テーブル(RE装置構成情報)初期化
 * @note   RE状態(セクタ部)管理テーブル(RE装置構成情報)を初期化する
 *         - 
 *           - 初期化,CPRIリンク断,報告確認T.O.以外の場合
 *             - 処理終了
 *         - 初期化
 * @param  ink    [in]  初期化種別
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_reeqpinf_ini_3g(                               /* 変化:R */
    USHORT ink                                              /* 初期化種別:I */
) {
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    T_REC_REEQPINF* tblp;                 /* テーブルRE装置構成情報アドレス */
    T_REC_REEQPINF* tbkp;             /* テーブル(BK)RE装置構成情報アドレス */
    USHORT          *offset;                              /* 参照オフセット */
    USHORT          cnt;                                /* 参照ープカウンタ */
    INT             cmp_ret;                                    /* 比較結果 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
    
    /* 初期化,CPRIリンク断,報告確認T.O.以外の場合 */
    if ( (ink != D_REC_INK_INI) && 
         (ink != D_REC_INK_DSC) && 
         (ink != D_REC_INK_RNG) && 
         (ink != D_REC_INK_RTO) ) {
        /* 初期化必要なし */
        /* 終了 */
        return CMD_OFF;
    }
    
    /* テーブルRE装置構成情報アドレス抽出 */
    tblp = &(f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_no_idx].reeqpinf);
    /* テーブル(BK)RE装置構成情報アドレス抽出 */
    tbkp = &(f_trp_rec_Wcom_remsc_tbk_3g.remsc[cpr_no_idx].reeqpinf);
    /* テーブル→テーブル(BK)にコピー */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        tbkp,                               /* (BK)RE装置構成情報アドレス:I */
        tblp,                                   /* RE装置構成情報アドレス:I */
        sizeof(*tblp)                         /* (BK)RE装置構成情報サイズ:I */
    );

    /**********/
    /* 初期化 */
    /**********/
    offset = &(tblp->mk_name);
    for (cnt = CMD_NUM0; cnt < CMD_EQUINF_MAX; cnt++) {
        *offset = CMD_NUM_0xFFFF;
        offset++;
    }
    
    /************/
    /* 新旧比較 */
    /************/
    BPF_RU_MACC_BCMP(                                       /* 終了コード:R */
        tbkp,                               /* (BK)RE装置構成情報アドレス:I */
        tblp,                                   /* RE装置構成情報アドレス:I */
        sizeof(*tblp),                        /* (BK)RE装置構成情報サイズ:I */
        &cmp_ret                                              /* 比較結果:O */
    );

    /* 変化なしの場合 */
    if (cmp_ret == CMD_NUM0) {
        return CMD_OFF;
    }
    /* 変化ありの場合 */
    else {
        return CMD_ON;
    }
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
