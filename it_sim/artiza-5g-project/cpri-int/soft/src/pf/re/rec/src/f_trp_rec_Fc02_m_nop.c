/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_nop.c
 * @brief  Nop
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
 * @brief  Nop
 * @note   Nop
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_nop(                                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif


    /* Nop */
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
