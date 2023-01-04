/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_SvMtDatRcv_fw.c
 * @brief  Forward SV-MT/eNB-MTデータ受信要求 and SV-MT/eNB-MTデータ受信応答
 * @date   2015/07/23 FPT)Tuan create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  Receive msg SV-MT/eNB-MTデータ受信要求 from l2/lbp
 * @note   Receive msg SV-MT/eNB-MTデータ受信要求 from l2/lbp
 *         - send [SV-MT/eNB-MTデータ受信要求] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01751
 * @date   2015/07/23 FPT)Tuan create
 * @date   2015/10/05 FPT)Yen M-RRU-ZSYS-01751 Fix bug IT2(update size of msg SV-MT/eNB-MTデータ受信要求)
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_SvMtDatRcvReq_fw(                       /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	if( NULL == bufp )
	{
		return;
	}

	/* Send msg SV-MT/eNB-MTデータ受信要求] to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, ((CMT_TSKIF_CPRISND_SVMTDATRCVREQ *)bufp)->head.uiLength );
#ifdef FHM_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg SV-MT/eNB-MTデータ受信応答 from l3/rct
 * @note   Receive msg SV-MT/eNB-MTデータ受信応答 from l3/rct
 *         - send [SV-MT/eNB-MTデータ受信応答] to l2/lbp
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Tuan create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_SvMtDatRcvRes_fw(                       /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	if( NULL == bufp )
	{
		return;
	}
	/* Send msg [SV-MT/eNB-MTデータ受信応答] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_SVMTDATRCVRES) );
#ifdef FHM_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
