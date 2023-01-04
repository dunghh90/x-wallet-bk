/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_fldcomsndres_fw.c
 * @brief  Receive msg [FLD-MTデータコマンド転送送信応答] from l2/lpb, 
 		   send [FLD-MTデータコマンド転送送信応答] to l3/rct
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
 * @brief  Receive msg [FLD-MTデータコマンド転送送信応答] from l2/lpb
 * @note   Receive msg [FLD-MTデータコマンド転送送信応答] from l2/lpb
 *         - send [FLD-MTデータコマンド転送送信応答] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/27 FPT)DongPD2 create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_fldcomsndres_fw(                           /* なし:R */
    VOID* bufp                                       				 /* バッファポインタ:I */
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

	/* Send msg [FLD-MTデータコマンド転送送信応答] to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_FLDCOMSNDRES) );
	
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
