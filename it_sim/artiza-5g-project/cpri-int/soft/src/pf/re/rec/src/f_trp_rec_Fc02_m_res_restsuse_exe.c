/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_restsuse_exe.c
 * @brief  RE状態運用中実行処理
 * @date   2013/10/09 
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE状態運用中実行処理
 * @note   RE状態運用中実行処理を行う
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2013/10/09
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_restsuse_exe(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************/
    /* 送信間隔タイマ登録 */
    /**********************/
	if( timeSetFlag[cpr_no_idx] == D_RRH_OFF )
	{
	    f_trp_rec_Fc02_t_inttim_ent(                                  /* なし:R */
	    );
		timeSetFlag[cpr_no_idx] = D_RRH_ON;
	}
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
