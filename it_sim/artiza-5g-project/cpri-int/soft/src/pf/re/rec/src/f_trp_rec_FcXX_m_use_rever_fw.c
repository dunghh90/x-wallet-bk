/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_rever_fw.c
 * @brief  Forward REバージョン報告要求 and REバージョン報告応答
 * @date   2015/09/28 TDI)satou create
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg REバージョン報告要求 from l3/rct
 * @note   Receive msg REバージョン報告要求 from l3/rct
 *         - send [REバージョン報告要求] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/09/28 TDI)satou create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_reverreq_fw(VOID *bufp) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s start \n", __LINE__, __FUNCTION__ );
#endif

    if (NULL == bufp) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
        printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
        return;
    }

    /* Send msg [REバージョン報告要求] to l2/lpb */
    f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_REVERREQ) );

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
}

/****************************************************************************/
/*!
 * @brief  Receive msg REバージョン報告応答 from L2/lpb
 * @note   Receive msg REバージョン報告応答 from L2/lpb
 *         - send [REバージョン報告応答] to L3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/09/28 TDI)satou create
 * @date  2015/10/03 TDI)satou 中継サイズを受信メッセージのヘッダに設定されているレングスを使用するように
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_reverres_fw(VOID *bufp) {
    CMT_TSKIF_CPRIRCV_REVERRES *rcv_p = (CMT_TSKIF_CPRIRCV_REVERRES*)bufp;

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s start \n", __LINE__, __FUNCTION__ );
#endif

    if (NULL == bufp) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
        printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
        return;
    }

    /* Send msg [REバージョン報告応答] to l3/rct */
    f_trp_rec_Fcom_t_l3_msg_fw( bufp, rcv_p->head.uiLength );

#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "[%d]%s end \n", __LINE__, __FUNCTION__ );
#endif
}

/*! @} */
