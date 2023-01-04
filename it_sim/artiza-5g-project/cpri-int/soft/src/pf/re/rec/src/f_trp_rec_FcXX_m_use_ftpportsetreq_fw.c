/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_rever_fw.c
 * @brief  Forward PORT number setting request for FTP data transmission and PORT number setting request for FTP data transmission
 * @date   2021/02/04 M&C)Tri.hn create
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2021
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg [PORT number setting request for FTP data transmission] from l3/rct
 * @note   Receive msg [PORT number setting request for FTP data transmission] from l3/rct
 *         - send [PORT number setting request for FTP data transmission] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2021/02/04 M&C)Tri.hn create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ftpportsetreq_fw(VOID *bufp) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s start \n", __LINE__, __FUNCTION__ );
#endif

    if (NULL == bufp) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
        printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
        return;
    }

    /* Send msg [PORT number setting request for FTP data transmission] to l2/lpb */
    f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_PORTSETREQ) );

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
}

/****************************************************************************/
/*!
 * @brief  Receive msg [PORT number setting request for FTP data transmission] from L2/lpb
 * @note   Receive msg [PORT number setting request for FTP data transmission] from L2/lpb
 *         - send [PORT number setting request for FTP data transmission] to L3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2021/02/04 M&C)Tri.hn create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ftpportsetres_fw(VOID *bufp) {
    CMT_TSKIF_CPRISND_PORTSETRES *rcv_p = (CMT_TSKIF_CPRISND_PORTSETRES*)bufp;

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s start \n", __LINE__, __FUNCTION__ );
#endif

    if (NULL == bufp) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
        printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
        return;
    }

    /* Send msg [PORT number setting request for FTP data transmission] to l3/rct */
    f_trp_rec_Fcom_t_l3_msg_fw( bufp, rcv_p->head.uiLength );

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
}

/*! @} */
