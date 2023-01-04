/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_m_txs_idl_fw_3g.c
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
VOID f_trp_rec_Fc03_m_txs_idl_fw_3g(                                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* Receive msg from l3/rct, send msg to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw(bufp, sizeof(CMT_TSKIF_CPRIRCV_TRXSETREQ));

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
