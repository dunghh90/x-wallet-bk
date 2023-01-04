/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_fldforcerel_fw.c
 * @brief  Forward FLD-MT接続強制解放要求 and FLD-MT接続強制解放応答
 * @date   2015/07/23 FPT)Son create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg [REC接続強制解放要求] from l3/rct
 * @note   Receive msg [REC接続強制解放要求] from l3/rct
 *         - send [REC接続強制解放要求] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Son create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_refldfrrelcntreq_fw(                           /* なし:R */
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

	/* Send msg [REC接続強制解放要求] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_FLMTCMPRELREQ) );

	#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg [RE接続強制解放応答] from l2/lpb
 * @note   Receive msg [RE接続強制解放応答] from l2/lpb
 *         - send [RE接続強制解放応答] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Son create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_refldfrrelcntrsp_fw(                           /* なし:R */
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

	/* Send msg [REリセット要求] to l2/lpb */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_FLMTCMPRELRES) );

	#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
