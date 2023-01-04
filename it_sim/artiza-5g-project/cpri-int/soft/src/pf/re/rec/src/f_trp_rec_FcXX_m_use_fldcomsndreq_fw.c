/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_fldcomsndreq_fw.c
 * @brief  Receive msg [FLD-MTデータコマンド転送送信要求] from l3/rct, 
 		   send [FLD-MTデータコマンド転送送信要求] to l2/lpb
 * @date   2015/07/27 FPT)DongPD2 create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  Receive msg [FLD-MTデータコマンド転送送信要求] from l3/rct
 * @note   Receive msg [FLD-MTデータコマンド転送送信要求] from l3/rct
 *         - send [FLD-MTデータファイル転送受信応答] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01751
 * @date   2015/07/27 FPT)DongPD2 create
 * @date   2015/10/05 FPT)Yen M-RRU-ZSYS-01751 Fix bug IT2(update size of msg FLD-MTデータコマンド転送送信要求)
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_fldcomsndreq_fw(                           /* なし:R */
    VOID* bufp                                       				 /* バッファポインタ:I */
) 
{
	#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [FLD-MTデータコマンド転送送信要求] (re/rec -> l2) */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, ((CMT_TSKIF_CPRISND_FLDCOMSNDREQ*)bufp)->head.uiLength );
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
