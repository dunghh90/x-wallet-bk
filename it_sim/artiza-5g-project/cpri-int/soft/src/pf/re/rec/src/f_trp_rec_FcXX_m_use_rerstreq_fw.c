/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_rerstreq_fw.c
 * @brief  Receive msg [REリセット要求] from l3/rct, send [REリセット要求] to l2/lpb
 * @date   2015/07/23 FPT)Yen create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Receive msg [REリセット要求] from l3/rct
 * @note   Receive msg [REリセット要求] from l3/rct
 *         - send [REリセット要求] to l2/lpb
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date  2015/07/23 FPT)Yen create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_rerstreq_fw(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

	if( NULL == bufp )
	{
		return;
	}

	/* Send msg [REリセット要求] to l2/lpb */
	f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_RERSTREQ) );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
