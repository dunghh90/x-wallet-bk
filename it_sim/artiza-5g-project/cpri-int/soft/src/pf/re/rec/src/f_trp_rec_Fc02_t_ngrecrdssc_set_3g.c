// [16B] add start
/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_ngrecrdssc_set_3g.c
 * @brief  REカード状態(セクタ部)取得NGスロット番号設定
 * @date   2016/10/06 KCN)Takagi create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  REカード状態(セクタ部)取得NGスロット番号設定
 * @note   REカード状態(セクタ部)取得NGスロット番号を設定する
 *         - スロット番号設定
 * @param  slt_no [in] スロット番号
 * @return None
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_ngrecrdssc_set_3g(                             /* なし:R */
    USHORT          slt_no                                /* スロット番号:I */
) {
    USHORT          cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
    UINT            uit_code;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************************************************/
    /* RE状態(セクタ部)管理テーブル(REカード状態)初期化 */
    /****************************************************/
    uit_code = f_trp_rec_Fc02_t_recrdssc_i_ini_3g(                   /* 変化:R */
        D_REC_INK_RNG,                                    /* スロット番号:I */
        slt_no                                      /* L3バッファポインタ:I */
    );
//    if (uit_code == D_REC_RFBSTS_CHG) {
//        /***********************************/
//        /* RE Function Block Status change */
//        /***********************************/
//        f_trp_rec_Fc02_t_refbstsntc_snd(                         /* なし:R */
//            CMD_OFF                                        /* Own Notice:I */
//        );

    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].crdssc_num++;

    return;
}
// [16B] add end
