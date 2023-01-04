/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ExtDatSndReq_fw.c
 * @brief  Receive msg [外部装置データ送信要求] from l3/rct, send [外部装置データ送信要求] to l2/lpb
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
 * @brief  Receive msg [外部装置データ送信要求] from l3/rct
 * @note   Receive msg [外部装置データ送信要求] from l3/rct
 *         - send [外部装置データ送信要求] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/24 FPT)DuongCD create
* @date  2015/10/19 FPT)Yen update (msg length)
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ExtDatSndReq_fw(                           /* なし:R */
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

	/* Send msg [[外部装置データ送信要求] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, ((CMT_TSKIF_CPRISND_EXTDATSNDREQ*)bufp)->head.uiLength );

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
