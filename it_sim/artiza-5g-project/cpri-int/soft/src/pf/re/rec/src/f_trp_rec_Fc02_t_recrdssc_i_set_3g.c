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
 * @date   2015/10/27 FPT)Yen fix bug IT3 No36
 * @date   2015/10/22 FPT)Tan fix bug BUG_122_IT2_20-7-1
 * @date   2015/11/11 FPT)Hai fix BUG_171_IT2_20_7_03
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_recrdssc_i_set_3g(                             /* 変化:R */
    USHORT slt_no,                                        /* スロット番号:I */
    VOID* l3_bufp                                   /* L3バッファポインタ:I */
) {

	USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    CARDSTS_INF*    rcvp;                       /* 受信REカード状態アドレス */
    T_REC_CRDSTS_3G*   tblp;                   /* テーブルREカード状態アドレス */
    USHORT          crdsts_idx;                   /* カード状態インデックス */
    USHORT*         oft;                            /* カード状態オフセット */
    USHORT          alm_num;                                   /* ALM情報数 */
    USHORT          alm_idx;                         /* ALM情報インデックス */
    USHORT          err_num;                                   /* ERR情報数 */
    USHORT          err_idx;                         /* ERR情報インデックス */
    USHORT          nonset_alm_num = CMD_NUM0;       /* 設定対象外ALM情報数 */
    USHORT          slt_index;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
// [16B] add start
//    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].crdssc_num++;
// [16B] add end

    /******************************************/
    /* REカード状態(セクタ部)インデックス取得 */
    /******************************************/
    crdsts_idx = CMD_NUM0;					/* only slot */

    /* 受信REカード状態アドレス抽出 */
    rcvp = &(((CMT_CPRIF_CARDSTSRES*)l3_bufp)->cardsts_inf);
    /* テーブルREカード状態アドレス抽出 */
    tblp = &(f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_no_idx].recrdsts.sts[crdsts_idx]);
    /* 一旦クリア */
    f_trp_com_Fmemclr(                                      /* 終了コード:R */
        tblp,                                       /* カード状態アドレス:I */
        sizeof(*tblp)                                 /* カード状態サイズ:I */
    );
	
	tblp->signal_kind = ((CMT_CPRIF_CARDSTSRES*)l3_bufp)->signal_kind;
	tblp->result = ((CMT_CPRIF_CARDSTSRES*)l3_bufp)->result;

    /**************/
    /* 受信値設定 */
    /**************/
    tblp->slt_no     = rcvp->slot_no;                       /* スロット番号 */
    tblp->y               = rcvp->year;                                   /* 年 */
    tblp->md              = rcvp->month_day;                            /* 月日 */
    tblp->h               = rcvp->hour;                                   /* 時 */
    tblp->ms              = rcvp->minsec;                               /* 分秒 */
    tblp->card_svitem     = rcvp->card_svitem;	      /* カード基本監視項目 */
    tblp->card_extsvitem  = rcvp->card_extitem;	      /* カード拡張監視項目 */
	tblp->card_cntitem	  = rcvp->card_cntitem;		  /* カード制御項目 */
    tblp->cnt_code 		  = rcvp->cnt_code;	      	  /* 制御コード */
    tblp->func_item       = rcvp->funcitem;		      /* 機能部固有制御項目 */
	tblp->car_inf	      = rcvp->car_inf;			  /* キャリア情報 */

	/*****************************************/
    /* REカード自律リセット実施有無判定      */
    /*****************************************/
    //f_trp_rec_Fc02_t_autorst_chk_3g( rcvp );

	oft = &(rcvp->car_inf);
    /***********/
    /* ALM情報 */
    /***********/
    oft++;                                           /* ALM情報数OFFSET算出 */
    alm_num = *oft;                                        /* ALM情報数設定 */

    if (alm_num > CMD_MAX_ALM_NUM) {
        /* ALM情報数をテーブル格納可能な16とする */
        tblp->alm_num = CMD_MAX_ALM_NUM;
        /* 設定対象外ALM情報数を算出する */
        nonset_alm_num = (USHORT)(alm_num - tblp->alm_num);
    }
    else {
        tblp->alm_num = alm_num;
    }
    /* ALM情報数分繰り返す */
    for (alm_idx = CMD_NUM0; alm_idx < tblp->alm_num; alm_idx++) {
        oft++;                                         /* ALM情報OFFSET算出 */
        tblp->alm_inf[alm_idx] = *oft;                        /* ALM情報設定 */
    }
    /* 設定対象外ALM情報数分OFFSETをずらす */
    oft += nonset_alm_num;

    /***********/
    /* ERR情報 */
    /***********/
    oft++;                                           /* ERR情報数OFFSET算出 */
    err_num = *oft;                                        /* ERR情報数設定 */

    if (err_num > CMD_MAX_ERR_NUM) {
        /* ERR情報数をテーブル格納可能な16とする */
        tblp->err_num = CMD_MAX_ERR_NUM;
    }
    else {
        tblp->err_num = err_num;
    }
    /* ERR情報数分繰り返す */
    for (err_idx = CMD_NUM0; err_idx < tblp->err_num; err_idx++) {
        oft++;                                        /* ERR情報OFFSET算出 */
        tblp->err_inf[err_idx] = *oft;                       /* ERR情報設定 */
    }

	/* Slot番号からスロットIndex値(0-15)を算出する 16B KCN add */
	f_cmn_com_slotsrch_3g(D_REC_C02_3G_CPR_NO(), slt_no, &slt_index);

	/* share memmory 16B KCN chg */
	f_cmn_com_cardstslt_3g_set( D_REC_C02_3G_CPR_NO(), slt_index, (T_RRH_CARDST_3G*)tblp );
    
    return D_REC_RFBSTS_CHG;

}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
