/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_it_reprmreq_snd.c
 * @brief  スレッド間(REシステムパラメータ更新要求)送信
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
 * @brief  スレッド間(REシステムパラメータ更新要求)送信
 * @note   スレッド間(REシステムパラメータ更新要求)を送信する
 *         - スレッド間(REシステムパラメータ更新要求)送信
 * @param  systerm_type [in] Systerm type (CMD_SYS_S3G / CMD_SYS_3G)
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/08/28 FPT)Yen update for FHM
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_it_reprmreq_snd(                            /* なし:R */
	USHORT systerm_type
) {
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d \n", __FILE__, __FUNCTION__, __LINE__);
#endif


    /************************************************/
    /* スレッド間(REシステムパラメータ更新要求)送信 */
    /************************************************/
    f_trp_rec_Fcom_t_it_reprmreq_snd(                             /* なし:R */
    	systerm_type
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
