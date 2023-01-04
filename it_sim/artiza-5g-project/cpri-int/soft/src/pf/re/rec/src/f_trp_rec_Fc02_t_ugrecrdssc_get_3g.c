// [16B] add start
/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_ugrecrdssc_get_3g.c
 * @brief  REカード状態(セクタ部)未取得スロット番号取得
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
 * @brief  REカード状態(セクタ部)未取得スロット番号取得
 * @note   REカード状態(セクタ部)未取得スロット番号を取得する
 *         - スロット番号リターン
 * @param  -
 * @return スロット番号
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_ugrecrdssc_get_3g(                   /* スロット番号:R */
) {
    USHORT          cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
//    T_REC_SLTLST    slt_lst;                          /* スロット番号リスト */
    USHORT          slt_lst_idx;                /* スロット番号インデックス */
    USHORT          crdssc_get;
    USHORT          slt_num;                    // スロット数
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

//    /**********************************************************************/
//    /* RE状態(セクタ部)管理テーブル(REスロット情報)スロット番号リスト取得 */
//    /**********************************************************************/
//    f_trp_rec_Fc02_t_resltlsc_get_3g(                                /* なし:R */
//        &slt_lst                                    /* スロット番号リスト:O */
//    );

    slt_num = f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_no_idx].resltinf.slt_num;

    crdssc_get = f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].crdssc_num;

//    if (slt_lst.num <= crdssc_get) {
    if (slt_num <= crdssc_get) {
        /* 未取得なかった場合 */
        /**************/
        /* NGリターン */
        /**************/

        return CMD_NUM0;
    }

    slt_lst_idx = crdssc_get;

//    return (slt_lst.slt_no[slt_lst_idx]);
    return (f_trp_rec_Wcom_remsc_tbl_3g.remsc[cpr_no_idx].resltinf.slt_no[slt_lst_idx]);
}
// [16B] add end
