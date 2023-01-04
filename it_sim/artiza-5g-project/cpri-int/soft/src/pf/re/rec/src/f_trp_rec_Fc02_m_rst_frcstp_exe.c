/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_rst_frcstp_exe.c
 * @brief  強制停止実行処理
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
 * @brief  強制停止実行処理
 * @note   強制停止実行処理を行う
 *         - RE状態(セクタ部)管理テーブル初期化
 *         - 自状態番号設定(RE起動中(強制停止))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01760
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2015/10/12 FPT)Yen M-RRU-ZSYS-01760 update (clear MT status)
 * @date   2015/10/28 TDI)satou FHM技説-QA-113
 * @date   2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
 * @date   2015/11/13 FPT)Lay fix bug IT2 No 177
 * @date   2016/03/02 tdips)enoki 強制停止時に共有テーブル(カード状態/キャリア状態/RE状態)をクリアする
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_rst_frcstp_exe(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    CMT_TSKIF_FRCSTPPRCIND *ind;
    USHORT                 lwCarrNo;
	USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;

    ind = (CMT_TSKIF_FRCSTPPRCIND*)bufp;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
    /********************************/
    /* ALL Retry Counter Initialize */
    /********************************/
    f_trp_rec_Fc02_t_tc_all_ini(
    );

	/*Set MTアドレス設定状態*/
	f_trp_rec_Fc02_t_mtsetsts( D_REC_C02_CPR_NO(), EC_REC_MTADDSET_STN_INI );

    /* 各種タイマ停止 */
    f_trp_rec_Fc02_t_inttim_ccl();
	timeSetFlag[cpr_no_idx] = D_RRH_OFF;

	/* 共有テーブル(カード状態/キャリア状態/RE状態)をクリア */
	f_trp_rec_Fc02_t_remsc_tbl_ini( D_REC_INK_DSC );

	/* C03状態初期化 */
	for( lwCarrNo = CMD_CARNO_MIN; lwCarrNo <= CMD_CARNO_MAX; lwCarrNo++ )
    {
        f_trp_rec_Fcom_t_c03stn_set(                                  /* なし:R */
                D_REC_C02_CPR_NO(),                                   /* CPRI番号:I */
                lwCarrNo,                                         /* キャリア番号:I */
                EC_REC_C03_STN_IDLE                                /* C03状態番号:I */
            );
    }
    /********************************/
    /* スレッド間(強制停止通知)送信 */
    /********************************/
    f_trp_rec_Fc02_t_it_frcstpntc_snd(                            /* なし:R */
        D_TCM_THRINO_CPC,                         /* スレッド内部番号番号:I */
        ind->stopFactor
    );

    /**********************************/
    /* プロセス間(RE再開完了通知)送信 */
    /**********************************/
    f_trp_rec_Fc02_t_ip_rescmpntc_snd(                            /* なし:R */
        CMD_SYS_LTE,
        CMD_NG
    );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_FRCSTP                                 /* 強制停止:I */
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
