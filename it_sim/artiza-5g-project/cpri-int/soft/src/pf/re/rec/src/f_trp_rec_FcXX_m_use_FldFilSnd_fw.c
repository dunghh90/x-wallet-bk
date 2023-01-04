/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_FldFilSnd_fw.c
 * @brief  Forward FLD-MTデータファイル転送送信要求 and FLD-MTデータファイル転送送信応答
 * @date   2015/07/27 FPT)Tunghv create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  Receive msg FLD-MTデータファイル転送送信要求 from L3/rct
 * @note   Receive msg FLD-MTデータファイル転送送信要求 from L3/rct
 *         - send [FLD-MTデータファイル転送送信要求] to L2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01751
 * @date   2015/07/27 FPT)Tunghv create
 * @date   2015/10/05 FPT)Yen M-RRU-ZSYS-01751 Fix bug IT2(update size of msg FLD-MTデータファイル転送送信要求)
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FldFilSndReq_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [REリセット要求] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp,((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ *)bufp)->head.uiLength );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg FLD-MTデータファイル転送送信応答 from L2/lpb
 * @note   Receive msg FLD-MTデータファイル転送送信応答 from L2/lpb
 *         - send [FLD-MTデータファイル転送送信応答] to L3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/27 FPT)Tunghv create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FldFilSndRes_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [REリセット要求] to L3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_FLDFILSNDRES) );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
