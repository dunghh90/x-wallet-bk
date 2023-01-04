/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ReForceRst_Ind.c
 * @brief  RE Force Reset Indication(l3_rct->re_rec)
 * @date   2015/09/07 FPT)Yen create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE Force Reset Indication(l3_rct->re_rec)
 * @note  
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/09/07 FPT)Yen create.
 * @date   2021/01/21 M&C)Tri.hn Update based on No.29 (seq 9-3)
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ReForceRst_Ind(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT                      lwReNo;

    lwReNo = ((CMT_TSKIF_RE_FORCERESET_IND*)bufp)->link_num;

    f_trp_rec_Wc02_prm_tbl.cpr_no = lwReNo;
    f_trp_rec_Wc02_prm_tbl_3g.cpr_no = lwReNo;

    /******************************************************/
    /* スレッド間(RE強制リセットプロシージャ実行指示)送信 */
    /******************************************************/
    f_trp_rec_Fc02_t_it_refrcrstprc_snd(                          /* なし:R */
    );

    f_trp_rec_Fc02_t_it_refrcrstprc_snd_3g(                        /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_REFRCRSTING                /* (36)RE強制リセット中:I */
    );

    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_REFRCRSTING                    /*(36)RE強制リセット中:I */
    );

/* 2021/01/21 M&C)Tri.hn Add based on No.29 (seq 9-3) - Add start */
    /* Reboot the intermediate device */
    // Tri.hn FIXME
    // m_rc_CpriRestartNtc();
/* 2021/01/21 M&C)Tri.hn Add based on No.29 (seq 9-3) - Add end */

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
