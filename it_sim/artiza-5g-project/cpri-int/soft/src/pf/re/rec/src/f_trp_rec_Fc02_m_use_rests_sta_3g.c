/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_rests_sta.c
 * @brief  運用中RE状態報告開始処理
 * @date   2008/07/21 FFCS)Shihzh Create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中RE状態報告開始処理
 * @note   運用中RE状態報告開始処理を行う
 *         - L3(RE状態報告要求)送信
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh Create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_rests_sta_3g(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    // USHORT actsts;                                         /* Active status */
    
    // /* 関数トレースログ */
    // F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    // actsts = f_trp_rec_Fc02_t_ownact_get_3g();
    // /* Negative case  */
    // if (actsts != D_REC_ACK_ACTCMP) {
    //    return;
    // }

    // /**********************************/
    // /* L3(RE状態報告要求)送信 */
    // /**********************************/
    // f_trp_rec_Fc02_t_l3_restsreq_snd_3g(                          /* なし:R */
    // );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
