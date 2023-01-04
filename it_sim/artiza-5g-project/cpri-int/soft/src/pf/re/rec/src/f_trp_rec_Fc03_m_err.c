/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_m_err.c
 * @brief  Err
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Err
 * @note   Err
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc03_m_err(                                        /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    ET_REC_C03_EVT evt;                                         /* イベント */
    ET_REC_C03_STN stn;                                         /* 状態番号 */
    CHAR errdata[CMD_NUM48];
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* イベント取得 */
    evt = f_trp_rec_Fc03_t_evt_get();
    /* 状態番号取得 */
    stn = f_trp_rec_Fc03_t_ownstn_get();

    /* 無効処理履歴 */
    snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][crr:%02d][stn:%02d][evt:%02d]", D_REC_C03_CPR_NO(), D_REC_C03_CRR_NO(), stn, evt);
    D_REC_IVLINI();
    D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
    D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

    /* Err */
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
