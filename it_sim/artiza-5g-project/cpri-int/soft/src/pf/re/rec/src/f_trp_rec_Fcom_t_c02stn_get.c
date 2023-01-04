/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_c02stn_get.c
 * @brief  C02状態番号取得
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
 * @brief  C02状態番号取得
 * @note   C02状態番号を取得する
 * @param  cpr_no [in]  CPRI番号
 * @return C02状態番号
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
ET_REC_C02_STN f_trp_rec_Fcom_t_c02stn_get(                /* C02状態番号:R */
    USHORT cpr_no                                             /* CPRI番号:I */
) {
    USHORT cpr_no_idx = cpr_no - CMD_NUM1;          /* CPRI番号インデックス */
    ET_REC_C02_STN stn;                                      /* C02状態番号 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*******************/
    /* C02状態番号取得 */
    /*******************/
    stn = f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].stn;

#ifdef FHM_DEBUG_TIMER_FOR_IT1
    printf( "%s %s %d <cpri>%d <stn>%d\n", __FILE__, __FUNCTION__, __LINE__, cpr_no, stn );
#endif

    return stn;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
