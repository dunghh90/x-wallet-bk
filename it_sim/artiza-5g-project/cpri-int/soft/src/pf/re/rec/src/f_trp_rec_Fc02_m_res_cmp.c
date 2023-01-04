/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_cmp.c
 * @brief  RE起動完了処理
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
 * @brief  RE起動完了処理
 * @note   RE起動完了処理を行う
 *         - プロセス間(RE再開完了通知)送信
 *         - 自状態番号設定(運用中)
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/18 FFCS)Shihzh modify for S3G PRC.
 * @date   2010/06/02 FJT)Tokunaga TS-514項 M-S3G-eNBSYS-01559
 * @date   2010/11/17 CMS)Kudou M-S3G-eNBPF-03898
 * @date   2011/02/10 FJT)Tokunaga CeNB-F-062-033(DCM) 運用パラメータの反映元と反映タイミング
 * @date   2011/11/17 FJT)K.Koshida D-MICRO-008 3002形対応(RE自律リセット,H/S inf変更)
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_cmp(                                    /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
	USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
	T_RRH_LAYER3	ltReLayer3Stas3g;
	
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    /**********************************************/
    /* 残トライ回数初期化(RE強制リセット送信含む) */
    /**********************************************/
    f_trp_rec_Fc02_t_tc_all_ini(                                  /* なし:R */
    );

//	/* CPRI-MUX レジスタ設定処理 */
//	f_trp_com_cprimux_reg_set(cpr_no_idx, D_TRP_CMUX_SEM_GIVEREQ_REC_RECMP);

    /**********************************/
    /* プロセス間(RE再開完了通知)送信 */
    /**********************************/
    f_trp_rec_Fc02_t_ip_rescmpntc_snd(                            /* なし:R */
        CMD_SYS_LTE,
        CMD_OK
    );

//    if (f_trp_com_Wapscmp_flg == CMD_OFF) {
//        /**************************************/
//        /* プロセス間(AP起動条件完了通知)送信 */
//        /**************************************/
//        f_trp_rec_Fc02_t_ip_lnkestntc_snd(                        /* なし:R */
//            D_PRCSID_DRVCTL                                     /* DRVCTL:I */
//        );
//    }
    /*	koshida try	*/
	(VOID)f_cmn_com_layer3_get( CMD_SYS_S3G, &ltReLayer3Stas3g );
	/*	対RECのL3状態が運用中状態の場合	*/
	/*	対RECとの起動開始後にタイマ生成する	*/
	if( (ltReLayer3Stas3g.layer3_rec == E_RRH_LAYER3_REC_OPE) 
		&& (timeSetFlag[cpr_no_idx] == D_RRH_OFF) )
	{
	    /**********************/
	    /* 送信間隔タイマ登録 */
	    /**********************/
	    f_trp_rec_Fc02_t_inttim_ent(                                  /* なし:R */
	    );
		timeSetFlag[cpr_no_idx] = D_RRH_ON;

/* 16B KCN add */
		timeRecrdcscCount[cpr_no_idx] = CMD_NUM0;               /* REカード状態報告 送信間隔カウンタの初期設定 */
		timeRecrdSndCount[cpr_no_idx] = CMD_NUM0;               /* REカード状態報告 送信slot数カウンタの初期設定 */

		cm_Assert(	CMD_ASL_USELOW, ltReLayer3Stas3g.layer3_rec, "f_trp_rec_Fc02_m_res_cmp Timer Start");
	}

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set(                                  /* なし:R */
        EC_REC_C02_STN_USE                                      /* 運用中:I */
    );

    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
