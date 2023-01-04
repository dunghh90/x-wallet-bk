/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_CardInfo_fw.c
 * @brief  Forward REカード情報報告要求	l3/rct -> re/rec -> l2/lpb
 *				   REカード情報報告応答 l2/lpb -> re/rec -> l3/rct
 * @date   2015/08/24 tdips)enoki
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg REカード情報報告要求 from l3/rct
 * @note   Receive msg REカード情報報告要求 from l3/rct
 *         - send [REカード情報報告要求] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/24 tdips)enoki
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_CardInfoReq_fw(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [REカード情報報告要求] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_CARDINFREQ) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg REカード情報報告応答 from L2/lpb
 * @note   Receive msg REカード情報報告応答 from L2/lpb
 *         - send [REカード情報報告応答] to l3/rct
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/24 tdips)enoki
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_CardInfoRsp_fw(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [REカード情報報告応答] to l3/rct */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_CARDINFRES) );

    return;
}

/* @} */  /* group TRIF_REC */


