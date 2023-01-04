/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_FileInf_fw.c
 * @brief  Forward ファイル情報報告通知	l3/dlm -> re/rec -> l2/lpb
 *				   ファイル情報報告応答	l2/lpb -> re/rec -> l3/dlm
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
 * @brief  Receive msg ファイル情報報告通知 from l3/dlm
 * @note   Receive msg ファイル情報報告通知 from l3/dlm
 *         - send [ファイル情報報告通知] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @Bug_No M-RRU-ZSYS-01972
 * @return None
 * @date   2015/08/21 FPT)Yen create
 * @date   2015/11/09 FPT)Yen M-RRU-ZSYS-01972 fix bug IT2 No165 配下RE強制停止状態の場合は配下REへ転送しない
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FileInfReq_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	USHORT		lwRet;

	if( NULL == bufp )
	{
		return;
	}

	lwRet = f_trp_rec_FcXX_m_frcstp_refilnewstp(bufp);
	if( CMD_OK == lwRet )
	{
		return;
	}

	/* Send msg [ファイル情報報告通知] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_FILEINFREQ) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg ファイル情報報告応答 from L2/lpb
 * @note   Receive msg ファイル情報報告応答 from L2/lpb
 *         - send [ファイル情報報告応答] to l3/dlm
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FileInfRes_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [ファイル情報報告応答] to l3/dlm */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_FILEINFRES) );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
