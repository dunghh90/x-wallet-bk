/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_hw_refrcrst_snd.c
 * @brief  HW(RE強制リセット)送信
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
 * @brief  HW(RE強制リセット)送信
 * @note   HW(RE強制リセット)を送信する
 *         - レジスタ書き込み
 * @param  cpr_no [in]  CPRI番号
 * @return 終了コード
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2010/06/02 FJT)Tokunaga TS-514項 M-S3G-eNBSYS-01559
 */
/****************************************************************************/
UINT f_trp_rec_Fcom_t_hw_refrcrst_snd(                      /* 終了コード:R */
    USHORT cpr_no                                             /* CPRI番号:I */
) {
    UINT   uit_ret;                                           /* 終了コード */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
    /********************/
    /* レジスタ書き込み */
    /********************/
    uit_ret = f_trp_rec_Fcom_t_crg_wrt(                     /* 終了コード:R */
        EC_REC_CRK_RFR,                                 /* RE強制リセット:I */
        cpr_no,                                               /* CPRI番号:I */
        D_REC_RGV_RST_ON                                  /* リセット設定:I */
    );

	return uit_ret;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
