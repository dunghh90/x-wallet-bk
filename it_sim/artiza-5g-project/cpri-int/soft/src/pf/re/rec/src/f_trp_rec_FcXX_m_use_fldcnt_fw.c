/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_fldcnt_fw.c
 * @brief  Forward FLD-MT接続要求 and FLD-MT接続応答
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
 * @brief  Receive msg [RE接続要求] from l2/lpb
 * @note   Receive msg [RE接続要求] from l2/lpb
 *         - send [RE接続要求] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Son create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_refldcntreq_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [RE接続要求] to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_FLDMTCNCREQ) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg [rct接続応答] from l3/rct
 * @note   Receive msg [rct接続応答] from l3/rct
 *         - send [rct接続応答] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Son create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_refldcntrsp_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [rct接続応答] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_FLDMTCNCRES) );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
