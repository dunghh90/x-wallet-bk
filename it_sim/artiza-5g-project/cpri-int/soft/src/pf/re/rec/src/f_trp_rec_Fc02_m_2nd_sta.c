/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_2nd_sta.c
 * @brief  二次リセット開始処理
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
 * @brief  二次リセット開始処理
 * @note   二次里セット開始処理を行う
 *         - 自状態番号設定(二次リセット準備中)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 * @date   2010/11/12 FJT)Tokunaga M-S3G-eNBSYS-xxxxx [TS-810項]
 * @date   2015/10/19 TDI)satou ハソ-QA-051 二次リセット
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_2nd_sta(                                    /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_2ND_RDYING                   /* 二次リセット準備中:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
