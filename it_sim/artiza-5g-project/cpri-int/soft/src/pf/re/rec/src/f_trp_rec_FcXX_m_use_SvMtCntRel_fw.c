/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_SvMtCntRel_fw.c
 * @brief  Forward SV-MT/eNB-MT接続解放要求 and SV-MT/eNB-MT接続解放応答
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
 * @brief  Receive msg SV-MT/eNB-MT接続解放要求 from l2/lpb
 * @note   Receive msg SV-MT/eNB-MT接続解放要求 from l2/lpb
 *         - send [SV-MT/eNB-MT接続解放要求] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Tuan create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_SvMtCntRelReq_fw(                       /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	if( NULL == bufp )
	{
		return;
	}

	/* Send msg SV-MT/eNB-MT接続解放要求] to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_SVMTCNTRELREQ) );
#ifdef FHM_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg SV-MT/eNB-MT接続解放応答 from l3/rct
 * @note   Receive msg SV-MT/eNB-MT接続解放応答 from l3/rct
 *         - send [SV-MT/eNB-MT接続解放応答] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Tuan create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_SvMtCntRelRes_fw(                       /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [SV-MT/eNB-MT接続解放応答] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_SVMTCNTRELRES) );
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
