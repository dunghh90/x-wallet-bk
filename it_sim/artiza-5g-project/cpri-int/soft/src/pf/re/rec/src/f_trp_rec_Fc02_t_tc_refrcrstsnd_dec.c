/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_refrcrstsnd_dec.c
 * @brief  残トライ回数(RE強制リセット送信)デクリメント
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
 * @brief  残トライ回数(RE強制リセット送信)デクリメント
 * @note   残トライ回数(RE強制リセット送信)をデクリメントする
 *         - 残トライ回数デクリメント
 * @param  -
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_refrcrstsnd_dec(                         /* なし:R */
) {
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************************/
    /* 残トライ回数デクリメント */
    /****************************/
    f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.refrcrstsnd--;
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s cpr_no:%d, tc.refrcrstsnd:%d \n",__LINE__,__FUNCTION__,
		cpr_no_idx + 1, f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.refrcrstsnd);
#endif
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
