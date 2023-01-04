/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_fldfilrcvres_fw.c
 * @brief  Receive msg [FLD-MTデータファイル転送受信応答] from l3/rct, 
 		   send [FLD-MTデータファイル転送受信応答] to l2/lpb
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
 * @brief  Receive msg [FLD-MTデータファイル転送受信応答] from l3/rct
 * @note   Receive msg [FLD-MTデータファイル転送受信応答] from l3/rct
 *         - send [FLD-MTデータファイル転送受信応答] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/27 FPT)DongPD2 create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_fldfilrcvres_fw(                           /* なし:R */
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

	/* Send msg [FLD-MTデータファイル転送受信応答] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_FLDFILRCVRES) );
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
