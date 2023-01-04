/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_FirmFile_fw.c
 * @brief  Forward 運用中ファイルデータ送信完了報告通知	l3/dlm -> re/rec -> l2/lpb
 *				   運用中ファイルデータ送信完了報告応答	l2/lpb -> re/rec -> l3/dlm
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
 * @brief  Receive msg 運用中ファイルデータ送信完了報告通知 from l3/dlm
 * @note   Receive msg 運用中ファイルデータ送信完了報告通知 from l3/dlm
 *         - send [運用中ファイルデータ送信完了報告通知] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_UFilSndFinNtc_fw(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [運用中ファイルデータ送信完了報告通知] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_UFILSNDFINNTC) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg 運用中ファイルデータ送信完了報告応答 from L2/lpb
 * @note   Receive msg 運用中ファイルデータ送信完了報告応答 from L2/lpb
 *         - send [運用中ファイルデータ送信完了報告応答] to l3/dlm
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_UFilSndFinRes_fw(                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [運用中ファイルデータ送信完了報告応答] to l3/dlm */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_UFILSNDFINRES) );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
