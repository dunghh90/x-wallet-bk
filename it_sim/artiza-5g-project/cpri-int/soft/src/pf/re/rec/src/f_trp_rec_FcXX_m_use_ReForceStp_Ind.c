/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_FcXX_m_use_ReForceStp_Ind.c
 * @brief  RE Force Stop Indication(l3_rct->re_rec)
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
 * @brief  RE Force Stop Indication(l3_rct->re_rec)
 * @note  
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/09/07 FPT)Yen create.
 * @date   2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
 */
/****************************************************************************/
VOID f_trp_rec_FcXX_m_use_ReForceStp_Ind(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT                      lwReNo;
    USHORT                      lwSysType;
    T_RRH_RESTOP_FACTOR         stopFactor;

    lwReNo     = ((CMT_TSKIF_RE_FORCESTOP_IND*)bufp)->link_num;
    lwSysType  = ((CMT_TSKIF_RE_FORCESTOP_IND*)bufp)->system_type;
    stopFactor = ((CMT_TSKIF_RE_FORCESTOP_IND*)bufp)->stopFactor;

    if( CMD_SYS_3G == lwSysType )
    {
        f_trp_rec_Wc02_prm_tbl_3g.cpr_no = lwReNo;
        /************************************************/
        /* スレッド間(強制停止プロシージャ実行指示)送信 */
        /************************************************/
        f_trp_rec_Fc02_t_it_frcstpprc_snd_3g(stopFactor);

        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
            EC_REC_C02_STN_FRCSTP                                /* (37)強制停止:I */
        );
    }
    else
    {
        f_trp_rec_Wc02_prm_tbl.cpr_no = lwReNo;
        /************************************************/
        /* スレッド間(強制停止プロシージャ実行指示)送信 */
        /************************************************/
        f_trp_rec_Fc02_t_it_frcstpprc_snd(stopFactor);

        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
            EC_REC_C02_STN_FRCSTP                                   /*(37)強制停止:I */
        );
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
