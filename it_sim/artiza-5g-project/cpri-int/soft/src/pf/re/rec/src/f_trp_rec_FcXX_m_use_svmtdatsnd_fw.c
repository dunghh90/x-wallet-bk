/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_svmtdatsnd_fw.c
 * @brief  Forward SV-MTデータ送信要求 and SV-MTデータ送信応答
 * @date   2015/07/23 FPT)Son create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  Receive msg [rctデータ送信要求] from l3/rct
 * @note   Receive msg [rctデータ送信要求] from l3/rct
 *         - send [rctデータ送信要求] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01751
 * @date   2015/07/23 FPT)Son create
 * @date   2015/10/05 FPT)Yen M-RRU-ZSYS-01751 Fix bug IT2(update size of msg SV-MT/eNBデータ送信要求)
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_resvmtdatsndreq_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [rctデータ送信要求] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, ((CMT_TSKIF_CPRIRCV_SVMTDATSNDREQ *)bufp)->head.uiLength );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg [REデータ送信応答] from l2/lpb
 * @note   Receive msg [REデータ送信応答] from l2/lpb
 *         - send [REデータ送信応答] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Son create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_resvmtdatsndrsp_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [REデータ送信応答] to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_SVMTDATSNDRES) );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
