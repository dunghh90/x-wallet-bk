/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_m_carsts_idl.c
 * @brief  Nop
 * @date   2021/01/18 M&C)Tri.hn Create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2021
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg from l2/lpb, send msg to l3/rct
 * @note   Receive carrier status resonse from l2/lpb, send msg to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2021/01/18 M&C)Tri.hn Create
 */
/****************************************************************************/
VOID f_trp_rec_Fc03_m_carstsrep_idl_rcv(                                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* Receive msg from l2/lpb, send msg to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw(bufp, sizeof(CMT_TSKIF_CPRIRCV_CARLSTSRES));

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg from l3/rct, send msg to l2/lpb
 * @note   Receive carrier status request from l3/rct, send msg to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2021/01/18 M&C)Tri.hn Create
 */
/****************************************************************************/
VOID f_trp_rec_Fc03_m_carstsrep_idl_fw(                                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* Receive msg from l3/rct, send msg to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw(bufp, sizeof(CMT_TSKIF_CPRIRCV_CARLSTSREQ));

    return;
}

/* @} */  /* group TRIF_REC */