/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_brc_get.c
 * @brief  送信系取得
 * @date   2008/08/17 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2009
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Transmission system acquisition
 * @note   Get the transmission system
* - The transmission system acquisition
 * @param  cpr_no [in] CPRI番号
 * @param  crr_no [in] キャリア番号
 * @return 送信系
 * @date   2008/08/17 FFCS)Shihzh create.
 */
/****************************************************************************/
USHORT f_trp_rec_Fcom_t_brc_get(     //nnn                          /* 送信系:R */
    USHORT  cpr_no,                                           /* CPRI number:I */
    USHORT  crr_no                                        /* Carrier number:I */
) {
    USHORT cpr_no_idx = cpr_no - CMD_NUM1;                  /* Index */
    USHORT crr_no_idx = crr_no - CMD_NUM1;                  /* Index */
    USHORT brc;                                                   /* 送信系 */

    /* Function trace log */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************/
    /* System bandwidth acquisition */
    /**********************/
    brc = f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].brc;

    return brc;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
