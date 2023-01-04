/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_ownstn_set_3g.c
 * @brief  自状態番号設定
 * @date   2015/07/15 FPT)Yen create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  自状態番号設定
 * @note   自状態番号を設定する
 * @param  stn [in]  状態番号
 * @return None
 * @date   2015/07/15 FPT)Yen create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_ownstn_set_3g(                              /* なし:R */
    ET_REC_C02_STN stn                                     /* C02状態番号:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /*******************/
    /* C02状態番号設定 */
    /*******************/
    f_trp_rec_Fcom_t_c02stn_set_3g(                               /* なし:R */
        D_REC_C02_3G_CPR_NO(),                                   /* CPRI番号:I */ 
    	stn                                                /* C02状態番号:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
