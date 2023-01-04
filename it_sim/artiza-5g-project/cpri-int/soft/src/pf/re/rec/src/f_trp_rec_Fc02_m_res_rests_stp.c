/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_rests_stp.c
 * @brief  運用中RE状態報告中止処理
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
 * @brief  運用中RE状態報告中止処理
 * @note   運用中RE状態報告中止処理を行う
 *         - スレッド間(RE状態報告プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(REスロット情報報告完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_rests_stp(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**********************************************************/
    /* スレッド間(RE状態報告プロシージャ実行指示)送信         */
    /**********************************************************/
    f_trp_rec_Fc02_t_it_restsprc_snd(                             /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_RESLTINFCMP      
                                      /* RE起動中(REスロット情報報告完了):I */
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
