/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_timmng_tbl_del.c
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
 * @return 終了コード
 * @retval 0:OK
 * @retval 1:NG
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_timmng_tbl_del(                       /* 終了コード:R */
    bpf_ru_hrtm_key_inf_t* key_infp                              /* キー情報:I */
) {
    UINT num;                                               /* タイマ情報数 */
    UINT cnt;                                                   /* カウンタ */
    INT  cmp_ret;                                               /* 比較結果 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* タイマ情報数抽出 */
    num = f_trp_rec_Wcom_timmng_tbl.num;

    /* タイマ情報数分ループ */
    for (cnt = CMD_NUM0; cnt < num; cnt++) {
        /* キー情報比較 */
        BPF_RU_MACC_BCMP(                                   /* 終了コード:R */
            (VOID*)&(f_trp_rec_Wcom_timmng_tbl.inf[cnt].key_inf),
                                                                /* 比較先:I */
            (VOID*)key_infp,                                    /* 比較元:I */
            sizeof(bpf_ru_hrtm_key_inf_t),                  /* 比較サイズ:I */
            &cmp_ret                                          /* 比較結果:O */
        );
        /* キー情報一致しない場合 */
        if (cmp_ret != CMD_NUM0) {
            /* 次を検索 */
            continue;
        }
        /* 最終位置データを現位置にコピー */
        f_trp_com_Fmemcpy(                                  /* 終了コード:R */
            &(f_trp_rec_Wcom_timmng_tbl.inf[cnt]),            /* コピー先:I */
            &(f_trp_rec_Wcom_timmng_tbl.inf[num - CMD_NUM1]), /* コピー元:I */
            sizeof(f_trp_rec_Wcom_timmng_tbl.inf[cnt])    /* コピーサイズ:I */
        );
        /* 最終位置データクリア */
        f_trp_com_Fmemclr(                                  /* 終了コード:R */
            &(f_trp_rec_Wcom_timmng_tbl.inf[num - CMD_NUM1]), /* アドレス:I */
            sizeof(f_trp_rec_Wcom_timmng_tbl.inf[num - CMD_NUM1])
        );            
        /* 情報数デクリメント */
        f_trp_rec_Wcom_timmng_tbl.num--;
        /* 終了 */
        return CMD_OK;
    }

    return CMD_NG;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
