/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_reghis_add.c
 * @brief  REGHIS追加
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
 * @brief  REGHIS追加
 * @note   REGHISを追加する
 * @param  adr  [in]  レジスタアドレス
 * @param  val  [in]  レジスタ値
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_reghis_add(                           /* 終了コード:R */
    UINT* adr,                                        /* レジスタアドレス:I */
    UINT  val                                               /* レジスタ値:I */
) {
    UINT         wrt_idx;                                   /* 書き込み位置 */
    T_REC_BCDTIM bcdtim;                                         /* BCD時刻 */
    CHAR         tim_as[CMD_NUM32];                          /* 時刻(ASCII) */
    CHAR         adr_as[CMD_NUM16];                      /* アドレス(ASCII) */
    CHAR         val_as[CMD_NUM16];                            /* 値(ASCII) */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************************/
    /* 収集データテーブル書込み */
    /****************************/
    /* BCD時刻取得 */
    f_trp_rec_Fcom_t_bcdtim_get(&bcdtim);
    /* 書き込み位置取得 */
    wrt_idx = f_trp_rec_Wcom_reghis_tbl.wrt_idx;
    /* ヘッダデータ書き込み */
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.year     = bcdtim.year;
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.month    = bcdtim.month;
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.day      = bcdtim.day;
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.hour     = bcdtim.hour;
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.min      = bcdtim.min;
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.sec      = bcdtim.sec;
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.msec1    = bcdtim.msec1;
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.msec2    = bcdtim.msec2;
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.adr      = adr;
    f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx].head.val      = val;

    /************************/
    /* 表示用キャラクタ作成 */
    /************************/
    /* 時刻 */
    sprintf(tim_as, "%02x/%02x/%02x %02x:%02x:%02x.%02x%02x",
        bcdtim.year, bcdtim.month, bcdtim.day,   bcdtim.hour,
        bcdtim.min,  bcdtim.sec,   bcdtim.msec1, bcdtim.msec2
    );
    /* レジスタアドレス */
    sprintf(adr_as, "0x%08x", (UINT)(adr));
    /* レジスタ値 */
    sprintf(val_as, "0x%08x", (UINT)(val));

    /********************/
    /* トレースログ表示 */
    /********************/
    F_COM_CMM_FANCRACE("[REC_REGHIS][%s][adr:%s][val:%s]\n", tim_as, adr_as, val_as);

    /**********************/
    /* 次走行履歴書込準備 */
    /**********************/
    /* 書き込み位置更新 */
    D_REC_INCLBK(f_trp_rec_Wcom_reghis_tbl.wrt_idx, D_REC_REGHIS_NUM);
    /* 次エリア0クリア */
    wrt_idx = f_trp_rec_Wcom_reghis_tbl.wrt_idx;
    f_trp_com_Fmemclr(
        &(f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx]),
        sizeof(f_trp_rec_Wcom_reghis_tbl.inf[wrt_idx])
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
