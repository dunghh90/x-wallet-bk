/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_inttim_ccl.c
 * @brief  送信間隔タイマ取消
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
 * @brief  送信間隔タイマ取消
 * @note   送信間隔タイマを取り消す
 *         - タイマ取消(RE状態報告送信間隔タイマ)
 *         - タイマ取消(REカード状態報告(セクタ部)送信間隔タイマ)
 *         - タイマ取消(REキャリア状態報告送信間隔タイマ)
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 * @date   2011/02/28 FJT)Tokunaga modify for M-S3G-eNBSYS-02253(TS111項【10B】)
 * @date   2015/09/04 FPT)Yen modify for FHM.
 * @date   2016/04/25 TDI)satou EC_REC_TMK_REPRTSTS_INTを追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_inttim_ccl(                                 /* なし:R */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************/
    /* タイマ取消 */
    /**************/
    f_trp_rec_Fc02_t_timer_ccl(                                   /* なし:R */
        EC_REC_TMK_RESTS_INT,                 /* RE状態報告送信間隔タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    /**************/
    /* タイマ取消 */
    /**************/
    f_trp_rec_Fc02_t_timer_ccl(                                   /* なし:R */
        EC_REC_TMK_RECRDSSC_INT,
                              /* REカード状態報告(セクタ部)送信間隔タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
   );

    /**************/
    /* タイマ取消 */
    /**************/
    f_trp_rec_Fc02_t_timer_ccl(                                   /* なし:R */
        EC_REC_TMK_RECRRSTS_INT,      /* REキャリア状態報告送信間隔タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    /**************/
    /* タイマ取消 */
    /**************/
    f_trp_rec_Fc02_t_timer_ccl(                                   /* なし:R */
    	EC_REC_TMK_REPRTSTS_INT,      /* RE PORT状態送信間隔タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

	return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
