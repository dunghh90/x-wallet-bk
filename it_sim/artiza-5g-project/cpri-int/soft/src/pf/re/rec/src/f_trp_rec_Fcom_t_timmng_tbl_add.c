/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_timmng_tbl_add.c
 * @brief  タイマ管理テーブル追加
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
 * @brief  タイマ管理テーブル追加
 * @note   タイマ管理テーブルに追加する
 *         - タイマ情報追加
 * @param  key_infp [in]  キー情報
 * @param  tim_id   [in]  タイマ識別子
 * @return 終了コード
 * @retval 0:OK
 * @retval 1:NG
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_timmng_tbl_add(                       /* 終了コード:R */
    bpf_ru_hrtm_key_inf_t* key_infp,                             /* キー情報:I */
    UINT tim_id                                           /* タイマ識別子:I */
) {
    USHORT num;                                             /* タイマ情報数 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* タイマ情報数抽出 */
    num = f_trp_rec_Wcom_timmng_tbl.num;

    /* タイマ情報数がMAXの場合 */
    if (num >= D_REC_TIM_MAX) {
        return CMD_NG;
    }

    /******************/
    /* タイマ情報追加 */
    /******************/
    f_trp_rec_Wcom_timmng_tbl.inf[num].key_inf = *key_infp;     /* キー情報 */
    f_trp_rec_Wcom_timmng_tbl.inf[num].tim_id  = tim_id;    /* タイマ識別子 */
    f_trp_rec_Wcom_timmng_tbl.num++;

    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
