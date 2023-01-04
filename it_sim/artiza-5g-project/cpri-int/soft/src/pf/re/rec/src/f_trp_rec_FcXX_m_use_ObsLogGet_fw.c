/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ObsLogGet_fw.c
 * @brief  Forward RE障害ログ取得要求, RE障害ログ取得応答 and ファイルデータ送信要求
 * @date   2015/08/06 FPT)Tunghv create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg RE障害ログ取得要求 from L3/log
 * @note   Receive msg RE障害ログ取得要求 from L3/log
 *         - send [RE障害ログ取得要求] to L2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/08/06 FPT)Tunghv create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_TroublelogGetReq_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [RE障害ログ取得要求] to L2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_REOBSLOGREQ) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg RE障害ログ取得応答 from L2/lpb
 * @note   Receive msg RE障害ログ取得応答 from L2/lpb
 *         - send [RE障害ログ取得応答] to L3/log
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/08/06 FPT)Tunghv create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_TroublelogGetRes_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [RE障害ログ取得応答] to L3/log */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_REOBSLOGRES) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg ファイルデータ転送送信 from L2/lpb
 * @note   Receive msg ファイルデータ転送送信 from L2/lpb
 *         - send [ファイルデータ転送送信] to L3/log
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/08/06 FPT)Tunghv create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_TroubleLogFilSndNtc_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [障害ログファイルデータ転送送信] to L3/log */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, ((CMT_TSKIF_CPRISND_FILEDATSND *)bufp)->head.uiLength );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg ファイルデータ送信完了報告通知 from L2/lpb
 * @note   Receive msg ファイルデータ送信完了報告通知 from L2/lpb
 *         - send [ファイルデータ送信完了報告通知] to L3/log
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/08/06 FPT)Tunghv create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_TroubleLogFilSndFinNtc_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [障害ログファイルデータ送信完了報告通知] to L3/log */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINNTC) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg ファイルデータ送信完了報告通知応答 from re/rec
 * @note   Receive msg ファイルデータ送信完了報告通知応答 from re/rec
 *         - send [ファイルデータ送信完了報告通知応答] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/08/06 FPT)Tunghv create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_TroubleLogFilSndFinRes_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [RE障害ログ取得中止要求] to L2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_FILSNDFINRES) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg RE障害ログ取得中止要求 from L3/log
 * @note   Receive msg RE障害ログ取得中止要求 from L3/log
 *         - send [RE障害ログ取得中止要求] to L2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/08/06 FPT)Tunghv create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_TroublelogGetStopReq_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [RE障害ログ取得中止要求] to L2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_REOBLOGSTPREQ) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg RE障害ログ取得中止応答 from L2/lpb
 * @note   Receive msg RE障害ログ取得中止応答 from L2/lpb
 *         - send [RE障害ログ取得中止応答] to L3/log
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/08/06 FPT)Tunghv create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_TroublelogGetStopRes_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_CXX_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [RE障害ログ取得中止応答] to L3/log */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRIRCV_REOBLOGSTPRES) );

    return;
}

/* @} */  /* group TRIF_REC */

