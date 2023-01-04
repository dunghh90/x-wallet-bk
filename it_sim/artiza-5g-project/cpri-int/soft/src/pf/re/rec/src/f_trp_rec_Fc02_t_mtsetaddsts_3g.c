/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_mtsetaddsts.c
 * @brief  MTアドレス設定状態
 * @date   2015/08/03 FPT)Hieu create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  MTアドレス設定状態設定
 * @note   MTアドレス設定状態設定
 * @param  awReNo [in] RE number
 * @param  awStn  [in] 状態番号
 * @return None
 * @date   2015/08/03 FPT)Hieu create
 * @date   2015/09/22 FPT)Yen update
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_mtsetsts_3g( USHORT awReNo, USHORT awStn) 
{

	
	USHORT cpr_no_idx = awReNo - CMD_NUM1;          /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
	
	/*Set MTアドレス設定状態 */
    f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].mtsetaddsts = awStn;

    return;
}

/****************************************************************************/
/*!
 * @brief  Get MTアドレス設定状態
 * @note   Get MTアドレス設定状態
 * @param  awReNo [in] RE number
 * @return None
 * @date   2015/08/03 FPT)Hieu create
 * @date   2015/09/22 FPT)Yen update
 */
/****************************************************************************/
USHORT f_trp_rec_Fc02_t_mtgetsts_3g( USHORT awReNo ) 
{

	
	USHORT cpr_no_idx = awReNo - CMD_NUM1;          /* CPRI番号インデックス */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

   return f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].mtsetaddsts;

}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
