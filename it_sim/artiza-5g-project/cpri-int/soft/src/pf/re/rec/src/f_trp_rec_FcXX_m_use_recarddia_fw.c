
/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_recarddia_fw.c
 * @brief  Forward RE card diagnostic request and RE card diagnostic response
 * @date   2021/02/19 M&C)Tri.hn Create based on No.67 (sequence 3-30)
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2021
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg [RE card diagnostic request] from l3/rct
 * @note   Receive msg [RE card diagnostic request] from l3/rct
 *         - send [RE card diagnostic request] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2021/02/19 M&C)Tri.hn create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_recarddiareq_fw(VOID *bufp) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s start \n", __LINE__, __FUNCTION__ );
#endif

    if (NULL == bufp) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
        printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
        return;
    }

    /* Send msg [RE card diagnostic request] to l2/lpb */
    f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_RECRDDIAREQ) );

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
}

/****************************************************************************/
/*!
 * @brief  Receive msg [RE card diagnostic response] from L2/lpb
 * @note   Receive msg [RE card diagnostic response] from L2/lpb
 *         - send [RE card diagnostic response] to L3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2021/02/19 M&C)Tri.hn create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_recarddiares_fw(VOID *bufp) {
    CMT_TSKIF_CPRIRCV_RECRDDIARES *rcv_p = (CMT_TSKIF_CPRIRCV_RECRDDIARES*)bufp;

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s start \n", __LINE__, __FUNCTION__ );
#endif

    if (NULL == bufp) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
        printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
        return;
    }

    /* Send msg [RE card diagnostic response] to l3/rct */
    f_trp_rec_Fcom_t_l3_msg_fw( bufp, rcv_p->head.uiLength );

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
}

/*! @} */
