/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_use_restsreq_ini.c
 * @brief  運用中残トライ回数(RE状態報告要求)初期化
 * @date   2011/02/17 FJT)Koshida Create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2011-
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中残トライ回数(RE状態報告要求)初期化
 * @note   運用中残トライ回数(RE状態報告要求)を初期化する
 *         - 運用中残トライ回数初期化
 * @param  -
 * @return None
 * @date   2011/02/17 FJT)Koshida Create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_use_restsreq_ini_3g(							/*	なし:R	*/
) {
	USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
												/*	CPRI番号インデックス	*/
	
#ifdef FHM_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif

	/* 関数トレースログ */
	F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
	
	/**********************/
	/* 残トライ回数初期化 */
	/**********************/
	f_trp_rec_Wc02_stsmng_tbl_3g.sts[cpr_no_idx].tc.m_restsreq 
										= D_REC_RESTSREQ_RTNUM - CMD_NUM1;
	
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
