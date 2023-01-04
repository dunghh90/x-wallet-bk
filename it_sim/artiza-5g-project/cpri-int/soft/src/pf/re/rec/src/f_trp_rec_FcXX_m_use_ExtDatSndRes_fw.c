/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ExtDatSndRes_fw.c
 * @brief  Receive msg [外部装置データ送信応答] from L2/lpb, send [外部装置データ送信応答] to L3/rct
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
 * @brief  Receive msg 外部装置データ送信応答 from L2/lpb
 * @note   Receive msg 外部装置データ送信応答 from L2/lpb
 *         - send [外部装置データ送信応答] to L3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/24 FPT)DuongCD create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ExtDatSndRes_fw(                           /* なし:R */
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

	/* Send msg [外部装置データ送信応答] to L3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_EXTDATSNDRES) );
	
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
