/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_res_mtset_sta_3g.c
 * @brief  RE起動中MTアドレス設定(FLD-MT)開始処理
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
 * @brief  RE起動中MTアドレス設定(FLD-MT)開始処理
 * @note   RE起動中MTアドレス設定(FLD-MT)開始処理を行う
 *         - スレッド間((MTアドレス設定(SV-MT)プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(MTアドレス設定(FLD-MT)中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/30 FPT)HieuNT
 * @date   2015/09/22 FPT)Yen update
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_mtsetf_sta_3g(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT stn;                                          /* Active state */
	/* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* Get MTアドレス設定状態 */
	stn = f_trp_rec_Fc02_t_mtgetsts_3g( D_REC_C02_3G_CPR_NO());
	/* status is 設定中*/
	if( EC_REC_MTADDSET_STN_RUNNING == stn )
	{
		return;
	}

	/* 残トライ回数(MTアドレス設定要求)設定 */
	f_trp_rec_Fc02_t_tc_mtsetfreq_set_3g( 0 );

    /**********************************************/
    /* スレッド間(MTアドレス設定(FLD-MT)要求)送信 */
    /**********************************************/
    f_trp_rec_Fc02_t_it_mtsetfreq_snd_3g(                            /* なし:R */
    );
	
	/*Set MTアドレス設定状態*/
	 f_trp_rec_Fc02_t_mtsetsts_3g( D_REC_C02_3G_CPR_NO(), EC_REC_MTADDSET_STN_RUNNING);
	
	/**************/
    /* タイマ登録 */
    /**************/
    f_trp_rec_Fc02_t_timer_ent_3g(                                   /* なし:R */
        EC_REC_TMK_MT_ADD_SET,                   /* MTアドレス設定確認タイマ:I */
        D_REC_CPR_IV                                         /* スロット番号:I */
    );
	
	/* MTアドレス設定開始要因設定:FHM*/
	f_trp_rec_Fc02_t_tc_mtsetffactor_set_3g(D_REC_C02_3G_CPR_NO(), D_MT_ADD_SET_FACTOR_FHM );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_MTSETFING
                                    /* RE起動中(MTアドレス設定(FLD-MT)中):I */
    );
	
	
    return;
}

/****************************************************************************/
/*!
 * @brief  RE起動中MTアドレス設定(SV-MT)開始処理
 * @note   RE起動中MTアドレス設定(SV-MT)開始処理を行う
 *         - スレッド間((MTアドレス設定(SV-MT)プロシージャ実行指示)送信
 *         - 自状態番号設定(RE起動中(MTアドレス設定(SV-MT)中))
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/07/30 FPT)HieuNT
 * @date   2015/09/22 FPT)Yen update
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_res_mtsets_sta_3g(                             /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT stn;                                          /* Active state */

	/* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

	/* Get MTアドレス設定状態 */
	stn = f_trp_rec_Fc02_t_mtgetsts_3g( D_REC_C02_3G_CPR_NO());
	/* status is 設定中*/
	if( EC_REC_MTADDSET_STN_RUNNING == stn )
	{
		return;
	}
	
	/* 残トライ回数(MTアドレス設定要求)設定 */
	f_trp_rec_Fc02_t_tc_mtsetfreq_set_3g( 0 );

	/*********************************************/
    /* スレッド間(MTアドレス設定(SV-MT)要求)送信 */
    /*********************************************/
    f_trp_rec_Fc02_t_it_mtsetsreq_snd_3g(                            /* なし:R */
    );
	
	/*Set MTアドレス設定状態*/
	 f_trp_rec_Fc02_t_mtsetsts_3g( D_REC_C02_3G_CPR_NO(), EC_REC_MTADDSET_STN_RUNNING);
	
	/**************/
    /* タイマ登録 */
    /**************/
    f_trp_rec_Fc02_t_timer_ent_3g(                                   /* なし:R */
        EC_REC_TMK_MT_ADD_SET,          /* MTアドレス設定確認タイマ:I */
        D_REC_CPR_IV                                      /* スロット番号:I */
    );
	
	/* MTアドレス設定開始要因設定:FHM*/
	f_trp_rec_Fc02_t_tc_mtsetffactor_set_3g(D_REC_C02_3G_CPR_NO(), D_MT_ADD_SET_FACTOR_FHM );

    /******************/
    /* 自状態番号設定 */
    /******************/
    f_trp_rec_Fc02_t_ownstn_set_3g(                                  /* なし:R */
        EC_REC_C02_STN_RES_MTSETSING
                                     /* RE起動中(MTアドレス設定(SV-MT)中):I */
    );
    
	
	
    return;
}
/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
