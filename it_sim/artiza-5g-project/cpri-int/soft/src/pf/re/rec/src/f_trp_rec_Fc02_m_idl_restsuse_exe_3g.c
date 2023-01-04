/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_idl_restsuse_exe_3g.c
 * @brief  RE状態運用中実行処理
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
 * @brief  RE状態運用中実行処理
 * @note   RE状態運用中実行処理を行う
 *         - 自状態番号設定(運用中)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/20 FFCS)Shihzh modify for S3G PRC.
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 * @date   2021/02/08 M&C)Huan.dh intermediate disable 3G RE time setting
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_idl_restsuse_exe_3g(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
//    USHORT actsts;                                            /* Active状態 */
//    UINT   uit_ret;
//	USHORT actlnk_gr;
    USHORT cpr_no_idx = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************************************/
    /* RE状態(セクタ部)管理テーブル初期化 */
    /**************************************/
    /* uit_ret = */f_trp_rec_Fc02_t_remsc_tbl_ini_3g(                     /* なし:R */
        D_REC_INK_UHC                                     /* CPRIリンク断:I */
    );

//TBD	actlnk_gr = D_REC_C02_ACTLNK_GR();
//	
//    /* RRE/LRE case */
//    if(actlnk_gr == D_TCM_ACTIVE_LINK_GROUP_RRELRE) {
//        /********************************************/
//        /* RE状態(共通部)管理テーブル初期化         */
//        /********************************************/
//        uit_ret |= f_trp_rec_Fc02_t_remcm_tbl_ini(D_REC_INK_UHC);
//    }
//	else {
//	/* BRE/M-LRE */
//        actsts = f_trp_rec_Fc02_t_allact_get(actlnk_gr);
//        if (actsts == D_REC_ACK_NEGCMP) {
//            /**********************************/
//            /* スレッド間(Active切替指示)送信 */
//            /**********************************/
//            f_trp_rec_Fc02_t_it_actchgind_snd(                    /* なし:R */
//                CMD_CHGTRG_REACTREQ           /* 再active依頼通知に応じる:I */
//            );
//        }
//	}
//
//    if ((uit_ret & D_REC_RFBSTS_CHG) != CMD_OFF) {
//        /***********************************/
//        /* RE Function Block Status change */
//        /***********************************/
//        f_trp_rec_Fc02_t_refbstsntc_snd(                         /* なし:R */
//            CMD_OFF                                        /* Own Notice:I */
//        );
//    }

    /**********************************/
    /* プロセス間(RE再開完了通知)送信 */
    /**********************************/
    f_trp_rec_Fc02_t_ip_rescmpntc_snd(                            /* なし:R */
        CMD_SYS_3G,
        CMD_OK
    );


    /*******************/
    /* (L3)RE Time Set */ 
    /*******************/
    // f_trp_rec_Fc02_t_l3_retimsetreq_snd_3g(                          /* なし:R */
    // );
    
    /**********************/
    /* 送信間隔タイマ登録 */
    /**********************/
    f_trp_rec_Fc02_t_inttim_ent_3g(                                  /* なし:R */
    );

/* 16B KCN add */
    timeRecrdcscCount3g[cpr_no_idx] = CMD_NUM0;                 /* REカード状態報告(3G) 送信間隔カウンタの初期設定 */
    timeRecrdSndCount3g[cpr_no_idx] = CMD_NUM0;                 /* REカード状態報告(3G) 送信slot数カウンタの初期設定 */

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_USE                                      /* 運用中:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
