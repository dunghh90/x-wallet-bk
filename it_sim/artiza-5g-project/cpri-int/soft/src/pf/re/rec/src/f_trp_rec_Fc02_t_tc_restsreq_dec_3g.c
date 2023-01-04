/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_tc_restsreq_dec.c
 * @brief  残トライ回数(RE状態報告要求)デクリメント
 * @date   2008/07/18 FFCS)Shihzh Create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  残トライ回数(RRE状態報告要求)デクリメント
 * @note   残トライ回数(RE状態報告要求)をデクリメントする
 *         - 残トライ回数デクリメント
 * @param  -
 * @return None
 * @date   2008/07/18 FFCS)Shihzh Create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_tc_restsreq_dec_3g(                         /* なし:R */
) {
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
                                                    /* CPRI番号インデックス */
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /****************************/
    /* 残トライ回数デクリメント */
    /****************************/
    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].tc.restsreq--;

#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
    	
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
