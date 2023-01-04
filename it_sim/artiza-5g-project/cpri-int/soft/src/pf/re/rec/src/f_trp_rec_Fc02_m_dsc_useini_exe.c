/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_dsc_useini_exe.c
 * @brief  RE状態初期化実行処理
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
 * @brief  RE状態初期化実行処理
 * @note   RE状態初期化実行処理を行う
 *         - RE状態(セクタ部)管理テーブル初期化
 *         - プロセス間(RE状態管理テーブル初期化通知)送信
 *         - 自状態番号設定(LAPBリンク確立前)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/20 FFCS)Shihzh modify for S3G PRC.
 * @date   2015/09/17 TDI)satou 状態遷移先を(04)RE起動待ちアイドルに
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_dsc_useini_exe(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
//    USHORT dlyoft;                          /* Delay Offset Set Wait Status */
	USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************************************/
    /* RE状態(セクタ部)管理テーブル初期化 */
    /**************************************/
    f_trp_rec_Fc02_t_remsc_tbl_ini(                     /* なし:R */
        D_REC_INK_DSC                                     /* CPRIリンク断:I */
    );
	
//TBD    /* RRE/LRE case */
//    if (D_REC_C02_RE_SYS_TYPE() == D_TCM_SYS_RETYP_RRELRE) {
//        /********************************************/
//        /* RE状態(共通部)管理テーブル初期化         */
//        /********************************************/
//        uit_ret |= f_trp_rec_Fc02_t_remcm_tbl_ini(D_REC_INK_NOA);
//
//        /*********************************************/
//        /* プロセス間(FLD-MT Active状態変化通知)送信 */
//        /*********************************************/
//        f_trp_rec_Fc02_t_ip_fldactntc_snd(                        /* なし:R */
//            D_REC_ACK_NEGCMP                                   /* Negtive:I */
//        );
//    }

//	/************************/
//	/* RE未実装強化ログ待避 */
//	/************************/
//	f_trp_com_FLog_switch();

    /**********************/
    /* 送信間隔タイマ取消 */
    /**********************/
    f_trp_rec_Fc02_t_inttim_ccl(                                  /* なし:R */
    );
    /*	koshida try	*/
	timeSetFlag[cpr_no_idx] = D_RRH_OFF;
	cm_Assert(	CMD_ASL_DBGLOW, timeSetFlag[cpr_no_idx], "f_trp_rec_Fc02_m_dsc_useini_exe Timer Stop");
    /******************/
    /* 確認タイマ取消 */
    /******************/
    f_trp_rec_Fc02_t_cnftim_ccl(                                  /* なし:R */
    );

//    /********************************/
//    /* 遅延補正値再設定待ち状態取得 */
//    /********************************/
//    dlyoft = f_trp_rec_Fc02_t_tc_dlyoft_get(               /* Wait Status:R */
//    );
//    if (dlyoft == CMD_ON) {
//        /**************/
//        /* タイマ取消 */
//        /**************/
//        f_trp_rec_Fc02_t_timer_ccl(                               /* なし:R */
//            EC_REC_TMK_T14RTY_WAT,            /* 遅延補正値設定待ちタイマ:I */
//            D_REC_SLT_IV                                  /* スロット番号:I */
//        );
//        /********************************/
//        /* 遅延補正値再設定待ち状態設定 */
//        /********************************/
//        f_trp_rec_Fc02_t_tc_dlyoft_set(
//            CMD_OFF
//        );    
//    }

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_RES_IDL                  /* (04)RE起動待ちアイドル:I */
    );

    /******************************************************************/
    /* PORT共有メモリ更新. CPRIリンク断の場合、引数3と4は0固定 */
    /******************************************************************/
    f_cmn_com_RePortSta_s3g_set(D_REC_C02_CPR_NO(), D_RRH_REPORTSTA_RSP_CPRIDSC, 0, 0);
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
