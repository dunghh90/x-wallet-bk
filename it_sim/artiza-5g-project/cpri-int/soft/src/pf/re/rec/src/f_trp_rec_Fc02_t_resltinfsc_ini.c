/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_resltinfsc_ini.c
 * @brief  RE状態(セクタ部)管理テーブル(REスロット情報)初期化
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE状態(セクタ部)管理テーブル(REスロット情報)初期化
 * @note   RE状態(セクタ部)管理テーブル(REスロット情報)を初期化する
 *         - 
 *           - 初期化,RE起動中HC OK以外の場合
 *             - 処理終了
 *         - 初期化
 *         - 暫定固定値入力
 * @param  ink [in]  初期化種別
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/20 FFCS)Shihzh modify for S3G PRC.
 * @date   2012/01/27 FJT)Tokunaga 12A-01-001 コメント票対応
                      CeNB-F-080-008(F)(F-1336(DCM)回答：Activeリンク切替時の動作について)
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_resltinfsc_ini(                             /* 変化:R */
    USHORT ink                                              /* 初期化種別:I */
) {
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    T_REC_RESLTINF* tblp;                 /* テーブルREスロット情報アドレス */
    T_REC_RESLTINF* tbkp;             /* テーブル(BK)REスロット情報アドレス */
    INT             cmp_ret;                                    /* 比較結果 */
// [16B] add start
    USHORT  cnt;
// [16B] add end

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* テーブルREスロット情報アドレス抽出 */
    tblp = &(f_trp_rec_Wcom_remsc_tbl.remsc[cpr_no_idx].resltinf);
    /* テーブル(BK)REスロット情報アドレス抽出 */
    tbkp = &(f_trp_rec_Wcom_remsc_tbk.remsc[cpr_no_idx].resltinf);
    /* テーブル→テーブル(BK)にコピー */
    f_trp_com_Fmemcpy(                                      /* 終了コード:R */
        tbkp,                               /* (BK)REスロット情報アドレス:I */
        tblp,                                   /* REスロット情報アドレス:I */
        sizeof(*tblp)                         /* (BK)REスロット情報サイズ:I */
    );

    /**********/
    /* 初期化 */
    /**********/

    /* リンク断の場合 */
    if ( (ink != D_REC_INK_INI) &&
         (ink != D_REC_INK_SHC) ) {

        /* BRE/M-LREの場合のみ、共通機能部のクリア (LRE/RREの場合はクリアしない) */
        if (D_REC_C02_RE_SYS_TYPE() == D_TCM_SYS_RETYP_RRELRE) {
            return CMD_OFF;
        }

// [16B] chg start
        // REカード機能部情報を初期化(16スロット)
        for ( cnt = 0; cnt < CMD_SLOTINF_MAX; cnt++ ) 
        {
//           tblp->crdcap &= 0xFF81;
           tblp->crdcap[cnt] &= 0xFF81;
        }
// [16B] chg end
    }
    /* 初期化,RE起動中HC OKの場合 */
    else{
        tblp->signal_kind = CMD_NUM0;                               /* 信号種別 */
        tblp->result = CMD_NUM0;                                    /* 応答結果 */
// [16B] chg start
        tblp->slt_num     = CMD_NUM0;                           /* スロット数   */

        // スロット番号とREカード機能部情報を初期化(16スロット)
        for ( cnt = 0; cnt < CMD_SLOTINF_MAX; cnt++ ) 
        {
//          tblp->slt_no = CMD_NUM0;                                /* スロット番号 */
//          tblp->crdcap = CMD_NUM_0xFFFF;                    /* REカード機能部情報 */
            tblp->slt_no[cnt] = CMD_NUM0;                           /* スロット番号 */
            tblp->crdcap[cnt] = CMD_NUM_0xFFFF;               /* REカード機能部情報 */
        }
        for ( cnt = 0; cnt < CMD_SLOTNO_MAX; cnt++ ) 
        {
            tblp->slt_idx[cnt] = CMD_NUM_0xFFFF;                 /* スロットIndex値 */
        }
// [16B] chg end
    }

    /************/
    /* 新旧比較 */
    /************/
    BPF_RU_MACC_BCMP(                                       /* 終了コード:R */
        tbkp,                               /* (BK)REスロット情報アドレス:I */
        tblp,                                   /* REスロット情報アドレス:I */
        sizeof(*tblp),                        /* (BK)REスロット情報サイズ:I */
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

