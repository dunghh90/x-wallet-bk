/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_runhis_del.c
 * @brief  RUNHIS削除
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
 * @brief  RUNHIS削除
 * @note   RUNHISを削除する
 * @param  rhk_ng_fct  [in] NG要因
 * @return 終了コード
 * @retval 0:書き込み有
 * @retval 1:書き込み無
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_runhis_del(                           /* 終了コード:R */
    USHORT rhk_ng_fct                                           /* NG要因:I */
) {
    UINT wrt_idx;                                           /* 書き込み位置 */
    UINT log_wrt_idx;                               /* 強化ログ書き込み位置 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /********************/
    /* トレースログ表示 */
    /********************/
    F_COM_CMM_FANCRACE("[REC_RUNHIS][NG]///////////////////////////////////");

    /* 書き込み位置復旧 */
    D_REC_DECLBK(f_trp_rec_Wcom_runhis_tbl.wrt_idx, D_REC_RUNHIS_NUM);
    /* 書き込み位置取り出し */
    wrt_idx = f_trp_rec_Wcom_runhis_tbl.wrt_idx;
    log_wrt_idx = f_trp_com_WlogWriteIndex;
    /* RUNHIS種別を送信NGに設定 */
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.rhk = D_REC_RUNHIS_RHK_SNG;
    f_trp_rec_Wcom_runhis_tbl.inf[log_wrt_idx][wrt_idx].head.rhk_ng_fct = rhk_ng_fct;
    /* 書き込み位置更新 */
    D_REC_INCLBK(f_trp_rec_Wcom_runhis_tbl.wrt_idx, D_REC_RUNHIS_NUM);

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
