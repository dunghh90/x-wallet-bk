/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_refrcrstsnd_get.c
 * @brief  残トライ回数(RE強制リセット送信)取得
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
 * @brief  残トライ回数(RE強制リセット送信)取得
 * @note   残トライ回数(RE強制リセット送信)を取得する
 *         - 残トライ回数リターン
 * @param  -
 * @return 残トライ回数
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_tc_refrcrstsnd_get(               /* 残トライ回数:R */
) {
    USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s cpr_no:%d, tc.refrcrstsnd:%d \n",__LINE__,__FUNCTION__,
		cpr_no_idx + 1, f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.refrcrstsnd);
#endif
    /************************/
    /* 残トライ回数リターン */
    /************************/
    return f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.refrcrstsnd;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
