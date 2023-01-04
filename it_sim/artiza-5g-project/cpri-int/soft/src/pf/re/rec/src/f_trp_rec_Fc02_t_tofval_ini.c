/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tofval_ini.c
 * @brief  RE状態(セクタ部)管理テーブル(Toffset値)初期化
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
 * @brief  RE状態(セクタ部)管理テーブル(Toffset値)初期化
 * @note   RE状態(セクタ部)管理テーブル(Toffset値)を初期化する
 *         - 
 *           - 初期化以外の場合
 *             - 処理終了
 *         - 初期化
 * @param  ink [in]  初期化種別
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/21 FFCS)Shihzh modify for S3G PRC.
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_tofval_ini(                                 /* 変化:R */
    USHORT ink                                              /* 初期化種別:I */
) {
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    T_REC_TOFVAL* tblp;                        /* テーブルToffset値アドレス */
    T_REC_TOFVAL* tbkp;                    /* テーブル(BK)Toffset値アドレス */
    INT           cmp_ret;                                      /* 比較結果 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* 初期化以外の場合 */
    if (ink != D_REC_INK_INI) {
        /* 初期化必要なし */
        /* 終了 */
        return CMD_OFF;
    }

    /* テーブルToffset値アドレス抽出 */
    tblp = &(f_trp_rec_Wcom_remsc_tbl.remsc[cpr_no_idx].tofval);
    /* テーブル(BK)Toffset値アドレス抽出 */
    tbkp = &(f_trp_rec_Wcom_remsc_tbk.remsc[cpr_no_idx].tofval);
    /* テーブル→テーブル(BK)にコピー */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        tbkp,                                    /* (BK)Toffset値アドレス:I */
        tblp,                                        /* Toffset値アドレス:I */
        sizeof(*tbkp)                              /* (BK)Toffset値サイズ:I */
    );

    /* 初期値設定 */
    tblp->tofval  = CMD_NUM_0xFFFF;                            /* Toffset値 */
    tblp->ueq_dly = CMD_NUM_0xFFFF;                     /* 上り装置内遅延値 */
    tblp->deq_dly = CMD_NUM_0xFFFF;                     /* 下り装置内遅延値 */
    tblp->cbl_dly = CMD_NUM_0xFFFF;

    /************/
    /* 新旧比較 */
    /************/
    BPF_RU_MACC_BCMP(                                       /* 終了コード:R */
        tbkp,                                    /* (BK)Toffset値アドレス:I */
        tblp,                                        /* Toffset値アドレス:I */
        sizeof(*tbkp),                             /* (BK)Toffset値サイズ:I */
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

