/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrdssc_i_set.c
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態)設定
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
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態)設定
 * @note   RE状態(セクタ部)管理テーブル(REカード状態)を設定する
 *         - 受信値設定
 * @param  slt_no  [in]  スロット番号
 * @param  l3_bufp [in]  L3バッファポインタ
 * @return 変化
 * @retval 0:変化なし
 * @retval 1:変化あり
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/08/20 FFCS)Shihzh modify for S3G PRC.
 * @date   2009/01/26 FFCS)Shihzh M-S3G-eNBPF-01182.
 * @date   2009/04/11 FFCS)Shihzh CR-XXX-XXX CPRI Spec V1.0.7.
 * @date   2010/04/07 FJT)Tokunaga CR-XXX-XXX 局データに65535が設定された場合の動作(API Ver.1.3.7)
 * @date   2010/06/02 FJT)Tokunaga TS-514項 M-S3G-eNBSYS-01559
 * @date   2010/12/09 CMS)Kudou modify for M-S3G-eNBPF-03928
 * @date   2011/06/14 CMS)Yorozuya TS-14項  M-S3G-eNBSYS-02395
 * @date   2011/09/30 FJT)Tokunaga CeNB-F-075-040(DCM) REの機能部故障状態についての確認事項
 * @date   2015/10/22 FPT)yen update (set share memory)
 * @date   2015/10/27 FPT)Yen fix bug IT3 No36
 * @date   2015/11/11 FPT)Hai fix BUG_171_IT2_20_7_03
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_recrdssc_i_set(                             /* 変化:R */
    USHORT slt_no,                                        /* スロット番号:I */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {
    CARDSTS_INF_S3G*   rcvp;                       /* 受信REカード状態アドレス */
    USHORT*            oft;                            /* カード状態オフセット */
    USHORT             crr_num;                              /* 対象キャリア数 */
    USHORT             crr_idx;                /* 対象キャリア情報インデックス */
    USHORT             brc_num;                              /* 対象ブランチ数 */
    USHORT             brc_idx;                /* 対象ブランチ情報インデックス */
    USHORT             alm_num;                                   /* ALM情報数 */
    USHORT             alm_idx;                         /* ALM情報インデックス */
    USHORT             err_num;                                   /* ERR情報数 */
    USHORT             err_idx;                         /* ERR情報インデックス */
    USHORT             rcv_fbidx;
    USHORT             nonset_alm_num = CMD_NUM0;       /* 設定対象外ALM情報数 */
    T_RRH_CARDST_S3G   shmCadrSt = {0};
// [16B] add start
//    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
    USHORT slt_index;
// [16B] add end

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

// [16B] add start
//    f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].crdssc_num++;
// [16B] add end

    /*************************/
    /* CPRI番号→AMP/OFTRX変換 */
    /*************************/


    /* 受信REカード状態アドレス抽出 */
    rcvp = &(((CMT_CPRIF_CARDSTSRES_S3G*)l3_bufp)->cardsts_inf);

    /**************/
    /* 受信値設定 */
    /**************/
    shmCadrSt.signal_kind = ((CMT_CPRIF_CARDSTSRES_S3G*)l3_bufp)->signal_kind;
    shmCadrSt.result      = ((CMT_CPRIF_CARDSTSRES_S3G*)l3_bufp)->result;
    shmCadrSt.slot_no     = rcvp->slot_no;                       /* スロット番号 */
    f_trp_com_Fmemcpy(                                           /* 終了コード:R */
        shmCadrSt.card_name,                                      /* Card Name:I */
        rcvp->card_name,                                          /* Card Name:I */
        CMD_NUM8
    );
    shmCadrSt.year            = rcvp->year;                                   /* 年 */
    shmCadrSt.month_day       = rcvp->month_day;                            /* 月日 */
    shmCadrSt.hour            = rcvp->hour;                                   /* 時 */
    shmCadrSt.minsec          = rcvp->minsec;                               /* 分秒 */
    shmCadrSt.card_svitem     = rcvp->card_svitem;            /* カード基本監視項目 */
    shmCadrSt.card_extsvitem  = rcvp->card_extitem;           /* カード拡張監視項目 */
    shmCadrSt.card_cntitem    = rcvp->card_cntitem;               /* カード制御項目 */
    shmCadrSt.card_extcntitem = rcvp->card_extcntitem;            /* カード制御項目 */

    /*****************************************/
    /* REカード自律リセット実施有無判定      */
    /*****************************************/
    f_trp_rec_Fc02_t_autorst_chk( rcvp );

    /* オフセット設定 */
    oft = &(rcvp->card_extcntitem);
    oft++;
    /**************/
    /* 機能部情報 */
    /**************/
    /* 受信メッセージから機能部数を取得 */
    shmCadrSt.func_num = *oft;

    /* 機能部情報数分もしくは、取得可能機能部情報数分繰り返す */
    for (rcv_fbidx = CMD_NUM0; rcv_fbidx < shmCadrSt.func_num; rcv_fbidx++) {
        oft++;                                  /* 機能部種別OFFSET算出 */
        shmCadrSt.func_every[rcv_fbidx].func_kind = *oft;
        oft++;                              /* 機能部故障状態OFFSET算出 */
        shmCadrSt.func_every[rcv_fbidx].func_failure     = *oft;

        oft++;                          /* 機能部基本監視項目OFFSET算出 */
        shmCadrSt.func_every[rcv_fbidx].func_svitem = *oft;
        oft++;                          /* 機能部拡張監視項目OFFSET算出 */
        shmCadrSt.func_every[rcv_fbidx].func_extsvitem = *oft;

        oft++;                              /* 対象キャリア数OFFSET算出 */
        crr_num = *oft;
        shmCadrSt.func_every[rcv_fbidx].car_num  = crr_num;

        /* 対象キャリア情報数分もしくは、取得可能対象キャリア情報数分繰り返す */
        for (crr_idx = CMD_NUM0;
            (crr_idx < crr_num) && (crr_idx < CMD_CARNO_MAX);
             crr_idx++) {
            oft++;                            /* キャリア番号OFFSET算出 */
            shmCadrSt.func_every[rcv_fbidx].car_every[crr_idx].car_no     = *oft;
            oft++;                  /* キャリアシステム帯域幅OFFSET算出 */
            shmCadrSt.func_every[rcv_fbidx].car_every[crr_idx].band_width    = *oft;
            oft++;                    /* キャリア基本監視項目OFFSET算出 */
            shmCadrSt.func_every[rcv_fbidx].car_every[crr_idx].func_svitem = *oft;

            oft++;                    /* キャリア拡張監視項目OFFSET算出 */
            shmCadrSt.func_every[rcv_fbidx].car_every[crr_idx].func_extsvitem = *oft;

            oft++;                          /* 対象ブランチ数OFFSET算出 */
            brc_num = *oft;
            shmCadrSt.func_every[rcv_fbidx].car_every[crr_idx].br_num = brc_num;
            /* 対象ブランチ情報数分もしくは、取得可能対象ブランチ情報数分繰り返す */
            for (brc_idx = CMD_NUM0;
                (brc_idx < brc_num) && (brc_idx < D_REC_BRCNUM_MAX);
                brc_idx++) {
                oft++;                  /* 対象ブランチ番号OFFSET算出 */
                shmCadrSt.func_every[rcv_fbidx].car_every[crr_idx].br_every[brc_idx].br_no
                  = *oft; 
                oft++;          /* 対象ブランチ基本監視項目OFFSET算出 */
                shmCadrSt.func_every[rcv_fbidx].car_every[crr_idx].br_every[brc_idx].func_svitem
                  = *oft;
                oft++;          /* 対象ブランチ拡張監視項目OFFSET算出 */
                shmCadrSt.func_every[rcv_fbidx].car_every[crr_idx].br_every[brc_idx].func_extsvitem
                  = *oft;
            }
        }
    }

    /***********/
    /* ALM情報 */
    /***********/
    oft++;                                           /* ALM情報数OFFSET算出 */
    alm_num = *oft;                                        /* ALM情報数設定 */

    if (alm_num > CMD_CRDALM_INFNUMMAX_HIG) {
        /* ALM情報数をテーブル格納可能な16とする */
        shmCadrSt.alm_num = CMD_CRDALM_INFNUMMAX_HIG;
        /* 設定対象外ALM情報数を算出する */
        nonset_alm_num = (USHORT)(alm_num - shmCadrSt.alm_num);
    }
    else {
        shmCadrSt.alm_num = alm_num;
    }
    /* ALM情報数分繰り返す */
    for (alm_idx = CMD_NUM0; alm_idx < shmCadrSt.alm_num; alm_idx++) {
        oft++;                                              /* ALM情報OFFSET算出 */
        shmCadrSt.alm_inf[alm_idx] = *oft;                        /* ALM情報設定 */
    }

    /* 設定対象外ALM情報数分OFFSETをずらす */
    oft += nonset_alm_num;

    /***********/
    /* ERR情報 */
    /***********/
    oft++;                                           /* ERR情報数OFFSET算出 */
    err_num = *oft;                                        /* ERR情報数設定 */

    if (err_num > CMD_CRDERR_INFNUMMAX_HIG) {
        /* ERR情報数をテーブル格納可能な16とする */
        shmCadrSt.err_num = CMD_CRDERR_INFNUMMAX_HIG;
    }
    else {
        shmCadrSt.err_num = err_num;
    }
    /* ERR情報数分繰り返す */
    for (err_idx = CMD_NUM0; err_idx < shmCadrSt.err_num; err_idx++) {
        oft++;                                             /* ERR情報OFFSET算出 */
        shmCadrSt.err_inf[err_idx] = *oft;                       /* ERR情報設定 */
    }

	/* Slot番号からスロットIndex値(0-15)を算出する 16B KCN add */
	f_cmn_com_slotsrch_s3g(D_REC_C02_CPR_NO(), slt_no, &slt_index);

    /* share memmory 16B KCN chg */
    f_cmn_com_cardstslt_s3g_set( D_REC_C02_CPR_NO(), slt_index, &shmCadrSt );

    return D_REC_RFBSTS_CHG;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
