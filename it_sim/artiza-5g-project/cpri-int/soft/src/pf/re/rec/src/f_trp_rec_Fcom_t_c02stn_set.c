/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_c02stn_set.c
 * @brief  C02状態番号設定
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
 * @brief  C02状態番号設定
 * @note   C02状態番号を設定する
 * @param  cpr_no [in]  CPRI番号
 * @param  stn    [in]  C02状態番号
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_c02stn_set(                                 /* なし:R */
    USHORT cpr_no,                                            /* CPRI番号:I */
    ET_REC_C02_STN stn                                     /* C02状態番号:I */
) {
    USHORT cpr_no_idx = cpr_no - CMD_NUM1;          /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d <cpri>%d <stn>%d\n", __FILE__, __FUNCTION__, __LINE__, cpr_no, stn );
#endif

    /*******************/
    /* C02状態番号設定 */
    /*******************/
    f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].stn = stn;
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s : cpr_no=%d,  stn=%d\n",__LINE__,__FUNCTION__, cpr_no, stn);
#endif
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
