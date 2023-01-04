/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_m_txr_idl_rcv_3g.c
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
 * @date   2015/10/20 TDI)satou 問処番号No.116水平展開
 */
/****************************************************************************/
VOID f_trp_rec_Fc03_m_txr_idl_rcv_3g(                                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* Receive msg from l2/lpb, send msg to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw(bufp, sizeof(CMT_TSKIF_CPRIRCV_TRXRELRES));

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
