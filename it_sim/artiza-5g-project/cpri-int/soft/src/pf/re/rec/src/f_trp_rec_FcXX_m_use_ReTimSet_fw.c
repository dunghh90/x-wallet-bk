/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ReTimSet_fw.c
 * @brief  L3_RCTから中継されたRE時刻設定要求をREに送信する
 * @date   2015/09/02 TDI)satou create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  L3_RCTから受信したRE時刻設定要求をREに送信する
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/09/02 TDI)satou create
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ReTimSet_fw(VOID* bufp) {
    if (NULL == bufp) {
        return;
    }

    f_trp_rec_Fcom_t_l2_msg_fw( bufp, sizeof(CMT_TSKIF_CPRISND_RETIMSETREQ) );
}

/* @} */  /* group TRIF_REC */
