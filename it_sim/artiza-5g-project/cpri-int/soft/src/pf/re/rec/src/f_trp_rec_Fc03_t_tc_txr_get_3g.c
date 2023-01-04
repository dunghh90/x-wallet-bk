/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_t_tc_txr_get_3g.c
 * @brief  残トライ回数取得
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  TRX解放(自律)残トライ回数取得
 * @note   TRX解放(自律)残トライ回数を取得する
 * @param  -
 * @return 残トライ回数
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
USHORT f_trp_rec_Fc03_t_tc_txr_get_3g(                               /* なし:R */
) {
    USHORT tc;
    USHORT cpr_no_idx = D_REC_C03_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
    USHORT crr_no_idx = D_REC_C03_3G_CRR_NO() - CMD_NUM1;
                                                /* キャリア番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /************************/
    /* 残トライ回数リターン */
    /************************/
    tc = f_trp_rec_Wc03_stsmng_tbl_3g.sts[cpr_no_idx][crr_no_idx].tc.txr;
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s cpr_no:%d, crr_no:%d, tc.txr:%d \n",__LINE__,__FUNCTION__,
		cpr_no_idx + 1, crr_no_idx + 1,  f_trp_rec_Wc03_stsmng_tbl_3g.sts[cpr_no_idx][crr_no_idx].tc.txr);
#endif
    return tc;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
