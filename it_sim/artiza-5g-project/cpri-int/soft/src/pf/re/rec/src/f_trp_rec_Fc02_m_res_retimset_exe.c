/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_retimset_exe.c
 * @brief  RE起動中RE時刻設定実行処理
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
 * @brief  RE起動中RE時刻設定実行処理
 * @note   RE起動中RE時刻設定実行処理を行う
 *         - L3(RE時刻設定要求)送信
 *         - スレッド間(REシステムパラメータ更新プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(RE時刻設定完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_retimset_exe(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /**************************/
    /* L3(RE時刻設定要求)送信 */
    /**************************/
    f_trp_rec_Fc02_t_l3_retimsetreq_snd(                          /* なし:R */
    );

    /****************************************************************/
    /* スレッド間(REシステムパラメータ更新プロシージャ実行指示)送信 */
    /****************************************************************/
    f_trp_rec_Fc02_t_it_reprmprc_snd(                             /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_RETIMSETCMP        /* RE起動中(RE時刻設定完了):I */
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
