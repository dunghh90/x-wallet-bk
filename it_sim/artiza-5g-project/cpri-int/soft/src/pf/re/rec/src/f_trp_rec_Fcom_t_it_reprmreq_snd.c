/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_it_reprmreq_snd.c
 * @brief  スレッド間(REシステムパラメータ更新要求)送信
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/08/28 FPT)Yen update for FHM.
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
 *         - バッファ取得
 *         - バッファ設定(データ)
 *         - スレッド間送信
 * @param  systerm_type [in]  Systerm type (CMD_SYS_S3G / CMD_SYS_3G)
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/08/28 FPT)Yen update for FHM.
 * @date   2015/09/16 FPT)Yen update cpri_no
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_it_reprmreq_snd(                            /* なし:R */
	USHORT systerm_type
) {
    CMT_TSKIF_RESYSNEWREQ  buf;
    CMT_TSKIF_RESYSNEWREQ* bufp = &buf;                 /* バッファポインタ */
    UINT   msglen = sizeof(CMT_TSKIF_RESYSNEWREQ);
                                                            /* メッセージ長 */
    INT    sit_ret;                                           /* 終了コード */
    CHAR  errdata[CMD_NUM48];
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start(systerm_type:%d) \n",__LINE__,__FUNCTION__, systerm_type);
#endif
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d \n", __FILE__, __FUNCTION__, __LINE__);
#endif

    /************************/
    /* バッファ設定(データ) */
    /************************/
    /* CPRI番号 */
    if( CMD_SYS_S3G == systerm_type )
    {
        bufp->cpri_no = D_REC_C02_CPR_NO();
    }
    else
    {
        bufp->cpri_no = D_REC_C02_3G_CPR_NO();
    }
    bufp->dummy = systerm_type;                         /* ダミー領域 : S3G/3G */
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s cpri_no:%d \n",__LINE__,__FUNCTION__, bufp->cpri_no);
#endif
    /******************/
    /* スレッド間送信 */
    /******************/
    /* REシステムパラメータ更新要求 */
    sit_ret = f_trp_rec_Fcom_t_msg_snd( bufp,
                            CMD_TSKIF_RESYSPARA_UPDATE_REQ,
                            D_RRH_PROCQUE_L3,
                            0, msglen);

    /* NGの場合 */
    if (sit_ret != CMD_OK) {
        /* 無効処理履歴 */
        snprintf(errdata, CMD_NUM48, "[ERR][cpr:%02d][ret:0x%02x]IT send NG.", bufp->cpri_no, sit_ret);
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(*bufp));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
    }

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
