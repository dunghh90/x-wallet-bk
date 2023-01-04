/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_reprm_sta_3g.c
 * @brief  RE起動中REシステムパラメータ更新開始処理
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/08/28 FPT)Yen update for FHM
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE起動中REシステムパラメータ更新開始処理(3G)
 * @note   RE起動中REシステムパラメータ更新開始処理を行う
 *         - スレッド間(REシステムパラメータ更新要求)送信
 *         - RE起動中(REシステムパラメータ更新中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/08/28 FPT)Yen update for FHM
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_reprm_sta_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/************************************************/
    /* スレッド間(REシステムパラメータ更新要求)送信 */
    /************************************************/
    f_trp_rec_Fc02_t_it_reprmreq_snd(                             /* なし:R */
    	CMD_SYS_3G
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                               /* なし:R */
        EC_REC_C02_STN_RES_REPRMING
                                  /* RE起動中(REシステムパラメータ更新中):I */
    );
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s End \n",__LINE__,__FUNCTION__);
#endif
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
