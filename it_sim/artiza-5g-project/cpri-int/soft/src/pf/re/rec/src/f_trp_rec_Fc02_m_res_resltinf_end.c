/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_resltinf_end.c
 * @brief  運用中REスロット情報報告終了処理
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
 * @brief  運用中REスロット情報報告終了処理
 * @note   運用中REスロット情報報告終了処理を行う
 *         - RE状態(セクタ部/共通部)管理テーブル(REスロット情報)設定
 *         - スレッド間(RE状態報告要求プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(REスロット情報報告完了))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2015/09/24 FPT)Hai modify for FHM.
 * @date   2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_resltinf_end(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    VOID*  datp;
    VOID*  l3_bufp;                                   /* L3バッファポインタ */
    USHORT ret_code;                                          /* 結果コード */
    UINT   uit_code;
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__ );
#endif

    /* L3バッファポインタ抽出 */
    datp    = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    l3_bufp = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    /* 結果コード抽出 */
    ret_code = ((CMT_CPRIF_SLOTINFRES*)l3_bufp)->result;

    /**************/
    /* タイマ取消 */
    /**************/
    uit_code = f_trp_rec_Fc02_t_timer_ccl(                        /* なし:R */
        EC_REC_TMK_RESLTINF_CNF,          /* REスロット情報報告確認タイマ:I */
        D_REC_SLT_IV                                      /* スロット番号:I */
    );

    if (uit_code == D_REC_TIM_IV) {
        return;
    }
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d <ret_code>%d\n", __FILE__, __FUNCTION__, __LINE__,ret_code );
    ret_code = CMD_OK;
#endif
    /* OK以外の場合 */
    if (ret_code != CMD_OK) {
        /**********************************************************/
        /* スレッド間(REスロット情報報告プロシージャ実行指示)送信 */
        /**********************************************************/
        f_trp_rec_Fc02_t_it_resltinfprc_snd(E_RRH_RESTOP_FACTOR_OTHER);

        /******************/
        /* 自状態番号設定 */
        /******************/
        f_trp_rec_Fc02_t_ownstn_set(                              /* なし:R */
            EC_REC_C02_STN_RES_REEQPINFCMP 
                                      /* RE起動中(RE装置構成情報報告完了):I */
        );

        /* 終了 */
        return;
    }
	
   	ret_code = f_trp_rec_Fc02_t_cprpar_chk(datp);

	if( CMD_OK != ret_code)
	{
        /************************************************/
        /* スレッド間(強制停止プロシージャ実行指示)送信 */
        /************************************************/
        f_trp_rec_Fc02_t_it_frcstpprc_snd(E_RRH_RESTOP_FACTOR_OTHER);
        
        /* 終了 */
        return;
	}
	
    /************************************************************/
    /* RE状態(セクタ部/共通部)管理テーブル(REスロット情報)設定  */
    /************************************************************/
    f_trp_rec_Fc02_t_resltinf_set(                                /* 変化:R */
        l3_bufp                                     /* L3バッファポインタ:I */
    );

    /****************************************************************/
    /* スレッド間(RE状態報告要求プロシージャ実行指示)送信           */
    /****************************************************************/
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
