/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_timmng_tbl_sch.c
 * @brief  タイマ管理テーブル削除
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
 * @brief  タイマ管理テーブル削除
 * @note   タイマ管理テーブルから削除する
 *         - タイマ情報削除
 * @param  key_infp [in]  キー情報
 * @return タイマ識別子
 * @retval 1-:タイマ識別子
 * @retval 0 :NG
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_timmng_tbl_sch(                     /* タイマ識別子:R */
    bpf_ru_hrtm_key_inf_t* key_infp                              /* キー情報:I */
) {
    USHORT num;                                             /* タイマ情報数 */
    USHORT cnt;                                                 /* カウンタ */
    INT    cmp_ret;                                             /* 比較結果 */
    UINT   tim_id;                                          /* タイマ識別子 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* タイマ情報数抽出 */
    num = f_trp_rec_Wcom_timmng_tbl.num;

    /* タイマ情報数分ループ */
    for (cnt = CMD_NUM0; cnt < num; cnt++) {
        /* キー情報比較 */
        BPF_RU_MACC_BCMP(                                   /* 終了コード:R */
            key_infp,                                           /* 比較元:I */
            &(f_trp_rec_Wcom_timmng_tbl.inf[cnt].key_inf),      /* 比較先:I */
            sizeof(bpf_ru_hrtm_key_inf_t),                   /* 比較サイズ:I */
            &cmp_ret                                          /* 比較結果:O */
        );
        /* キー情報一致しない場合 */
        if (cmp_ret != CMD_NUM0) {
            /* 次を検索 */
            continue;
        }
        /* タイマ識別子抽出 */
        tim_id = f_trp_rec_Wcom_timmng_tbl.inf[cnt].tim_id;
        /* 終了 */
        return tim_id;
    }

    return CMD_NUM0;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
