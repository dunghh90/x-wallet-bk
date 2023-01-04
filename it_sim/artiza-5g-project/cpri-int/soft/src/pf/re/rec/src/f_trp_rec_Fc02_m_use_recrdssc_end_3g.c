/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_recrdssc_end.c
 * @brief  運用中REカード状態報告(セクタ部)終了処理
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007-2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中REカード状態報告(セクタ部)終了処理
 * @note   運用中REカード状態報告(セクタ部)終了処理を行う
 *         - RE状態(セクタ部)管理テーブル(REカード状態)設定
 *           - 変化ありの場合
 *             - プロセス間(REカード状態変化通知(セクタ部))送信
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh mdify for S3G PRC. 
 * @date   2010/08/11 CMS)Takahashi modify for TS-656
 * @date   2015/10/08 FPT)Tan Update swap sending branch
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_recrdssc_end_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    VOID*  datp;
    VOID*  l3_bufp;                                   /* L3バッファポインタ */
    USHORT ret_code;                                          /* 結果コード */
    USHORT slt_no;                                          /* スロット番号 */
    UINT   uit_ret;                                           /* 終了コード */
    CHAR   errdata[CMD_NUM48];
	USHORT slot_mskbit[CMD_NUM16] = {0xFFFE,0xFFFD,0xFFFB,0xFFF7,0xFFEF,0xFFDF,0xFFBF,0xFF7F,
									0xFEFF,0xFDFF,0xFBFF,0xF7FF,0xEFFF,0xDFFF,0xBFFF,0x7FFF};
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
    USHORT slt_index;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* L3バッファポインタ抽出 */
    datp    = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    l3_bufp = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    /* 結果コード・スロット番号抽出 */
    ret_code = ((CMT_CPRIF_CARDSTSRES*)l3_bufp)->result;
    slt_no   = ((CMT_CPRIF_CARDSTSRES*)l3_bufp)->cardsts_inf.slot_no;

	/* Slot番号からスロットIndex値(0-15)を算出する 16B KCN add */
	f_cmn_com_slotsrch_3g(D_REC_C02_3G_CPR_NO(), slt_no, &slt_index);

	/*	カード状態報告応答受信ログ格納	*/
//	f_trp_rec_Fcom_t_l3_LogStore( D_REC_LOGKIND_CARD, D_REC_C02_3G_CPR_NO(), D_REC_MSGKIND_RSP );

    /**************/
    /* タイマ取消 */
    uit_ret = f_trp_rec_Fc02_t_timer_ccl_3g(                      /* なし:R */
        EC_REC_TMK_RECRDSSC_CNF,  /* REカード状態報告(セクタ部)確認タイマ:I */
        slt_no                                            /* スロット番号:I */
    );
    
    if (uit_ret == D_REC_TIM_IV) {
        return;
    }
     /* REカード状態報告(3G) 送信slotのBitをOFF 16B KCN add */
    timeRecrdSndCount3g[cpr_no_idx] &= slot_mskbit[slt_index];

    /**********************/
    /* 残トライ回数初期化 */
    /**********************/
    f_trp_rec_Fc02_t_use_recrdstsreq_ini_3g(                         /* なし:R */
        slt_no                                               /* スロット番号:I */
    );
    
    if (ret_code == CMD_NML) { 
        ret_code |= f_trp_rec_Fc02_t_cprpar_chk_3g(datp);
    }
    /* OK以外の場合 */
    if (ret_code != CMD_OK) {
        /****************************************************/
        /* RE状態(セクタ部)管理テーブル(REカード状態)初期化 */
        /****************************************************/
        uit_ret = f_trp_rec_Fc02_t_recrdssc_i_ini_3g(                /* 変化:R */
            D_REC_INK_RNG,                                   /* スロット番号:I */
            slt_no                                     /* L3バッファポインタ:I */
        );
        if (uit_ret == CMD_ON) {
            snprintf(errdata, CMD_NUM48, "[NOTICE][cpr:%02d][slt:%02d][r:%d][3G]CardStsRcvNG", D_REC_C02_3G_CPR_NO(), slt_no, ret_code);
            D_REC_IVLINI();
            D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
            D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
        }
    }
   
    /**************************************************/
    /* RE状態(セクタ部)管理テーブル(REカード状態)設定 */
    /**************************************************/
    uit_ret = f_trp_rec_Fc02_t_recrdssc_i_set_3g(             /* 変化:R */
        slt_no,                                       /* スロット番号:I */
        l3_bufp                                 /* L3バッファポインタ:I */
    );

//    /* 変化判定 */
//    if (uit_ret == CMD_OFF) {
//        /* 終了 */
//        return;
//    }
//    
//    if (uit_ret == D_REC_RFBSTS_CHG) {
//        /***********************************/
//        /* RE Function Block Status change */
//        /***********************************/
//        f_trp_rec_Fc02_t_refbstsntc_snd(                         /* なし:R */
//            CMD_ON                                         /* Own Notice:I */
//        );
//    }
//    else if ((uit_ret & D_REC_RFBSTS_CHG_MULTI) == D_REC_RFBSTS_CHG_MULTI) {
//
//        // OAMPFへREカード状態変化通知送信
//        f_trp_rec_Fc02_t_ip_recrdntc_multi_snd(CMD_ON);
//    }
//    else {
//        /**************************************************/
//        /* プロセス間(REカード状態変化通知(セクタ部))送信 */
//        /**************************************************/
//        f_trp_rec_Fc02_t_ip_recrdsscntc_snd(                      /* なし:R */
//            D_REC_C02_CPR_NO(),                               /* CPRI番号:I */
//            slt_no                                        /* スロット番号:I */
//        );
//    }
//    
//    if(ret_code != CMD_OK) {
//		/************************/
//		/* RE未実装強化ログ待避 */
//		/************************/
//		f_trp_com_FLog_switch();
//	}

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
