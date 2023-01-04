/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_SvMtForceRel_fw.c
 * @brief  Forward eNB-MT/SV-MT接続強制解放要求 and eNB-MT/SV-MT接続強制解放応答
 * @date   2015/07/23 FPT)Hieu create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg eNB-MT/SV-MT接続強制解放要求 from l3/rct
 * @note   Receive msg eNB-MT/SV-MT接続強制解放要求 from l3/rct
 *         - send [eNB-MT接続強制解放要求] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Hieu create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_SvMtForceRelReq_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
)
{
	#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	if( NULL == bufp )
	{
		#ifdef FHM_DEBUG_CXX_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* Send msg [eNB-MT接続強制解放要求] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_SVMTFORRELREQ) );

	#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg eNB-MT/SV-MT接続強制解放応答 from L2/lpb
 * @note   Receive msg eNB-MT/SV-MT接続強制解放応答 from L2/lpb
 *         - send [eNB-MT接続強制解放応答] to L3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Hieu create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_SvMtForceRelRes_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) 
{
	#ifdef FHM_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif

	if( NULL == bufp )
	{
		#ifdef FHM_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* Send msg [eNB-MT/SV-MT接続強制解放応答] to l2/lpb */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_SVMTFORRELRES) );

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
