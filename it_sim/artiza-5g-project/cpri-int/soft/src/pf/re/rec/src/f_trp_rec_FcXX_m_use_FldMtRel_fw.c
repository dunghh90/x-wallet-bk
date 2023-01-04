/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_FldMtRel_fw.c
 * @brief  Forward FLD-MT接続解放要求 and FLD-MT接続解放応答
 * @date   2015/07/23 FPT)Quynh create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg FLD-MT接続解放要求 from l2/lpb
 * @note   Receive msg FLD-MT接続解放要求 from l2/lpb
 *         - send [FLD-MT接続解放要求] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/30 FPT)Quynh create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FldMtRelReq_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif

	if( NULL == bufp )
	{
				
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
		return;
	}

	/* Send msg [FLD-MT接続解放要求] to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ) );
		
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg FLD-MT接続解放応答 from L3/rct
 * @note   Receive msg FLD-MT接続解放応答 from L3/rct
 *         - send [FLD-MT接続解放応答] to L2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/30 FPT)Quynh create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FldMtRelRes_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif

	if( NULL == bufp )
	{
				
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
		return;
	}

	/* Send msg [FLD-MT接続解放応答] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_FLMTCNCRELRES) );
		
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
