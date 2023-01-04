/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_bcdtim_get.c
 * @brief  BCD時刻取得
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
 * @brief  BCD時刻取得
 * @note   BCD時刻を取得する
 * @param  bcdtimp [out] 1バイト時刻
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_bcdtim_get(                           /* 終了コード:R */
    T_REC_BCDTIM* bcdtimp                                      /* BCD時刻:O */
) {
    struct bpf_tm tm;                                      /* 年月日時分秒 */
    INT            ms;                                            /* ミリ秒 */

    /* 時刻取得 */
    f_trp_com_Fget_precisetim(&tm, &ms);
    bcdtimp->year  = D_REC_CNVBCD(tm.tm_year-100);
    bcdtimp->month = D_REC_CNVBCD(tm.tm_mon+1);
    bcdtimp->day   = D_REC_CNVBCD(tm.tm_mday);
    bcdtimp->hour  = D_REC_CNVBCD(tm.tm_hour);
    bcdtimp->min   = D_REC_CNVBCD(tm.tm_min);
    bcdtimp->sec   = D_REC_CNVBCD(tm.tm_sec);
    bcdtimp->msec1 = D_REC_CNVBCD(ms/10);
    bcdtimp->msec2 = D_REC_CNVBCD(ms%10)<<4;

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
