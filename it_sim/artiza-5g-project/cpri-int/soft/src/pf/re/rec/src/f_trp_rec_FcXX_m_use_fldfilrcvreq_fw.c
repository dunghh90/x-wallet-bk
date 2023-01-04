/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_fldfilrcvreq_fw.c
 * @brief  Receive msg [FLD-MTデータファイル転送受信要求] from l2/lpb, 
 		   send FLD-MTデータファイル転送受信要求] to l3/rct
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
 * @brief  Receive msg [FLD-MTデータファイル転送受信要求] from l2/lpb
 * @note   Receive msg [FLD-MTデータファイル転送受信要求] from l2/lpb
 *         - send [FLD-MTデータファイル転送受信要求] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01751
 * @date   2015/07/27 FPT)DongPD2 create
 * @date   2015/10/05 FPT)Yen M-RRU-ZSYS-01751 Fix bug IT2(update size of msg FLD-MTデータファイル転送受信要求)
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_fldfilrcvreq_fw(                           /* なし:R */
    VOID* bufp                                       				 /* バッファポインタ:I */
) 
{
	#ifdef FHM_DEBUG_CXX_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	CMT_TSKIF_CPRIRCV_SIGNALGET* tempParm = bufp;
	if( NULL == bufp )
	{
		return;
	}

	if(CMD_NUM0 == (tempParm->signal_kind  & CMD_SYS_CHK_CPRID))
	{
		/* Send msg [FLD-MTデータファイル転送受信要求(3G)] to l3/rct */
		f_trp_rec_Fcom_t_l3_msg_fw( bufp, ((CMT_TSKIF_CPRIRCV_FLDFILRCVREQ *)bufp)->head.uiLength );
		#ifdef FHM_DEBUG_CXX_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	else
	{
		/* Send msg [FLD-MTデータファイル転送受信要求(LTE)] to l3/rct */
		f_trp_rec_Fcom_t_l3_msg_fw( bufp, ((CMT_TSKIF_CPRIRCV_FLDFILRCVREQ_S3G *)bufp)->head.uiLength );
		#ifdef FHM_DEBUG_CXX_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
