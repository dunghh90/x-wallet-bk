/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ExtDatRcvReq_fw.c
 * @brief  Receive msg [外部装置データ受信要求] from l2/lpb, send [外部装置データ受信要求] to l3/rct
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
 * @brief  Receive msg [外部装置データ受信要求] from l2/lpb
 * @note   Receive msg [外部装置データ受信要求] from l2/lpb
 *         - send [外部装置データ受信要求] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/24 FPT)DuongCD create
 * @date  2015/10/19 FPT)Yen update(msg length)
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ExtDatRcvReq_fw(                           /* なし:R */
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

	/* Send msg [[外部装置データ受信要求] to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, ((CMT_TSKIF_CPRIRCV_EXTDATRCVREQ *)bufp)->head.uiLength );

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
