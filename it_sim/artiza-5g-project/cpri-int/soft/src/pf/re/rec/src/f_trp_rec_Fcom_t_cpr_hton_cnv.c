/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_cpr_hton_cnv.c
 * @brief  CPRI信号バイトオーダー(HTON)変換
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
 * @brief  CPRI信号バイトオーダー(HTON)変換
 * @note   CPRI信号バイトオーダー(HTON)を変換する
 * @param  datp [i/o] データポインタ
 * @param  len  [in]  レングス
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_cpr_hton_cnv(                               /* なし:R */
    VOID* datp,                                        /* データポインタ:IO */
    UINT  len                                                 /* レングス:I */
) {
    USHORT* ustp;                                  /* データポインタ(2byte) */
    UINT    dat_cnt;                                  /* データ設定カウンタ */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* ALL2byte要素メッセージ */
    /* 型の載せ換え */
    ustp = datp;
    for (dat_cnt = CMD_NUM0; dat_cnt < len/CMD_NUM2; dat_cnt++) {
        *ustp = htons(*ustp);
        ustp++;
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
