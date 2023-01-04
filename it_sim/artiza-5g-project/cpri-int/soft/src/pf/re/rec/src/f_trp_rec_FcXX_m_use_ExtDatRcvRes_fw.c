/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ExtDatRcvRes_fw.c
 * @brief  Receive msg [外部装置データ受信応答] from L3/rct, send [外部装置データ受信応答] to L2/lpb
 * @date   2015/07/24 FPT)DuongCD create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg 外部装置データ受信応答 from L3/rct
 * @note   Receive msg 外部装置データ受信応答 from L3/rct
 *         - send [外部装置データ受信応答] to L2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/24 FPT)DuongCD create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ExtRcvtSndRes_fw(                           /* なし:R */
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

	/* Send msg [外部装置データ受信応答] to L2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_EXTDATRCVRES) );
	
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
