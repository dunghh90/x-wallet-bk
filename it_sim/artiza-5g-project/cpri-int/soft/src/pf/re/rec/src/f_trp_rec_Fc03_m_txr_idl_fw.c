/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_m_txr_idl_fw.c
 * @brief  Nop
 * @date   2007/03/20 FPT)Dung create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Nop
 * @note   Nop
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FPT)Dung create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc03_m_txr_idl_fw(                                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* Receive msg from l3/rct, send msg to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw(bufp, sizeof(CMT_TSKIF_CPRIRCV_TRXRELREQ));

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg from l3/rct, send msg to l2/lpb
 * @note   Receive CARONOFF msg from l3/rct, send msg to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2021/02/04 M&C)Huan.dh create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc03_m_caronoff_idl_fw(                                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* Receive msg from l3/rct, send msg to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw(bufp, sizeof(CMT_TSKIF_CPRIRCV_CARONOFF_S3G_REQ));

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
