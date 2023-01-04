/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_UFileInf_fw.c
 * @brief  Forward 運用中ファイル情報報告通知	l3/dlm -> re/rec -> l2/lpb
 *				   運用中ファイル情報報告応答	l2/lpb -> re/rec -> l3/dlm
 * @date   2015/08/21 FPT)Yen create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg 運用中ファイル情報報告通知 from l3/dlm
 * @note   Receive msg 運用中ファイル情報報告通知 from l3/dlm
 *         - send [運用中ファイル情報報告通知] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_UFileInfReq_fw(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [運用中ファイル情報報告通知] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_UFILEINFREQ) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg 運用中ファイル情報報告応答 from L2/lpb
 * @note   Receive msg 運用中ファイル情報報告応答 from L2/lpb
 *         - send [運用中ファイル情報報告応答] to l3/dlm
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_UFileInfRes_fw(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [運用中ファイル情報報告応答] to l3/dlm */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_UFILEINFRES) );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
