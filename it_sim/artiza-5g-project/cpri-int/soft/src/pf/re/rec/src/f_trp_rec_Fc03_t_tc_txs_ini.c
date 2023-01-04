/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_t_tc_txs_ini.c
 * @brief  TRX設定(自律)残トライ回数初期化
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2009
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  TRX設定(自律)残トライ回数初期化
 * @note   TRX設定(自律)残トライ回数を初期化する
 *         - 残トライ回数
 * @param  -
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2009/03/04 FFCS)Shihzh CR-00031-005.
 */
/****************************************************************************/
VOID f_trp_rec_Fc03_t_tc_txs_ini(                                 /* なし:R */
) {
    USHORT cpr_no_idx = D_REC_C03_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT crr_no_idx = D_REC_C03_CRR_NO() - CMD_NUM1;
                                                /* キャリア番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************************/
    /* TRX設定(自律)残トライ回数初期化  */
    /************************************/
    if(f_trp_com_Wsysprmp_PF != NULL)
    {
        f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].tc.txs
         = f_trp_com_Wsysprmp_PF[D_SYSPARA_PF_C17_TRXSETREQ_RTNUM] + CMD_NUM1;
    }
    else
    {
        /* set default value */
        f_trp_rec_Wc03_stsmng_tbl.sts[cpr_no_idx][crr_no_idx].tc.txs
         = D_TCM_SYSDFT_TRXSETREQ_RTNUM + CMD_NUM1;
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
