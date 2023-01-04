/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_evt_set.c
 * @brief  イベント設定
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
 * @brief  イベント設定
 * @note   イベントを設定する
 * @param  evt [in] イベント
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_evt_set(                                    /* なし:R */
    ET_REC_C02_EVT evt                                     /* C02イベント:I */
) {
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*******************/
    /* C02イベント設定 */
    /*******************/
    f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].evt = evt;

#ifdef FHM_DEBUG_TIMER_FOR_IT1
    printf( "%s %s %d <cpri>%d <evt>%d\n", __FILE__, __FUNCTION__, __LINE__, cpr_no_idx+1, evt );
#endif

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
