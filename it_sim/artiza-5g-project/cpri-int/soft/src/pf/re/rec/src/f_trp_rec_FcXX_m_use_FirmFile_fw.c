/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_FirmFile_fw.c
 * @brief  Forward ファームウェアファイル報告要求	l3/dlm -> re/rec -> l2/lpb
 *				   ファームウェアファイル報告応答	l2/lpb -> re/rec -> l3/dlm
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
 * @brief  Receive msg ファームウェアファイル報告要求 from l3/dlm
 * @note   Receive msg ファームウェアファイル報告要求 from l3/dlm
 *         - send [ファームウェアファイル報告要求] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01972
 * @date   2015/08/21 FPT)Yen create
 * @date   2015/11/09 FPT)Yen M-RRU-ZSYS-01972 fix bug IT2 No165 配下RE強制停止状態の場合は配下REへ転送しない 
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FirmFileReq_fw(                           /* なし:R */
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

	/* Send msg [ファームウェアファイル報告要求] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_FIRMFILEREQ) );

    return;
}

/****************************************************************************/
/*!
 * @brief  Receive msg ファームウェアファイル報告応答 from L2/lpb
 * @note   Receive msg ファームウェアファイル報告応答 from L2/lpb
 *         - send [ファームウェアファイル報告応答] to l3/dlm
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/21 FPT)Yen create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_FirmFileRes_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [ファームウェアファイル報告応答] to l3/dlm */
	f_trp_rec_Fcom_t_l3_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_FIRMFILERES) );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
