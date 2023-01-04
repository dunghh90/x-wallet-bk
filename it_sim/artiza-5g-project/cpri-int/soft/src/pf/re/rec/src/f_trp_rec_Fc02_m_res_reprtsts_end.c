/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_reprtsts_end.c
 * @brief  起動中REスロット情報報告終了処理
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
 * @brief  起動中RE PORT状態報告終了処理
 * @note   起動中RE PORT状態報告終了処理を行う
 *         - RE状態(共通部)管理テーブル(RE状態情報)設定
 *         - スレッド間(REカード状態報告(セクタ部)要求プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(RE PORT状態報告完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2016/04/25 TDI)satou 共有メモリに応答内容を保存する処理を追加
 * @TBD_No 修正未完了
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_reprtsts_end(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	VOID	*datp;
	VOID	*l3_bufp;
    UINT   uit_code;
    UCHAR	received_rsp;
    USHORT	rsp_result;
    USHORT	rsp_portin;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    /**************/
    /* タイマ取消 */
    /**************/
    uit_code = f_trp_rec_Fc02_t_timer_ccl(                        /* なし:R */
        EC_REC_TMK_REPRTSTS_CNF,             /* RE PORT状態報告確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );
    
    if (uit_code == D_REC_TIM_IV) {
        return;
    }

    /**********************/
    /* 残トライ回数初期化 */
    /**********************/
    f_trp_rec_Fc02_t_use_reprtstsreq_ini(                         /* なし:R */
    );

    /**********************************************************************/
    /* 共有メモリに応答内容を保存. NGの場合、引数4はDon't Care */
    /**********************************************************************/
    datp       = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    l3_bufp    = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    rsp_result = ((CMT_CPRIF_REPORTSTSRES*)l3_bufp)->result;
    rsp_portin = ((CMT_CPRIF_REPORTSTSRES*)l3_bufp)->portsts_inf.port_in_sts;
    received_rsp = (CMD_NML == rsp_result) ? D_RRH_REPORTSTA_RSP_NORMAL : D_RRH_REPORTSTA_RSP_NG;
    f_cmn_com_RePortSta_s3g_set(D_REC_C02_CPR_NO(), received_rsp, rsp_result, rsp_portin);

    /**********************************************************************/
    /* スレッド間(REカード状態報告(セクタ部)要求プロシージャ実行指示)送信 */
    /**********************************************************************/
    f_trp_rec_Fc02_t_it_recrdsscprc_snd(                          /* なし:R */
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_REPRTSTSCMP   /* RE起動中(RE PORT状態報告完了):I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
