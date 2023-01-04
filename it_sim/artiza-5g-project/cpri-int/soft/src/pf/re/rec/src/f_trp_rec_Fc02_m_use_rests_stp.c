/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_rests_stp.c
 * @brief  運用中RE状態報告中止処理
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
 * @brief  運用中RE状態報告中止処理
 * @note   運用中RE状態報告中止処理を行う
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @Bug_No M-RRU-ZSYS-01825
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2011/02/17 FJT)Koshida modify for M-S3G-eNBSYS-02253(TS-111)対応.
 * @date   2015/10/23 FPT)Quynh M-RRU-ZSYS-01825 Fix bug IT2 (enoki_bug127_20151022)
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_rests_stp(                              /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    USHORT actsts;                                            /* Active状態 */
    CHAR   errdata[CMD_NUM48];
	USHORT cpr_no = D_REC_C02_CPR_NO();
	USHORT cpr_no_idx = (cpr_no - CMD_NUM1);
	SHORT tc;
	T_RRH_REST_S3G ltReStaArrLTE;
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
    
    /*	INVALIDログ	*/
    snprintf(errdata, CMD_NUM48, "[cpr:%02d][TRY=%d][LTE]RE sts report T.O.",
    		 cpr_no, f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.m_restsreq);
    D_REC_IVLINI();
    D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
    D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
    
    actsts = f_trp_rec_Fc02_t_ownact_get();
    /* Negative case  */
    if (actsts != D_REC_ACK_ACTCMP) {
       return;
    }
	/****************************/
	/*	運用中残トライチェック	*/
	/****************************/
	/*	運用中残トライ回数(RE状態報告要求)を取得する	*/
	tc = f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.m_restsreq;
	
	/*	RE状態報告要求トライ可能な場合	*/
	if(tc > CMD_NUM0)
	{
		/*	運用中残トライ回数(RE状態報告要求)をデクリメントする	*/
		f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.m_restsreq--;
		
		/*	管理テーブルの初期化せずに前状態保持として処理終了	*/
		return;
	}

    /********************************************/
    /* RE状態(共通部)管理テーブル(RE状態)初期化 */
    /********************************************/
    f_trp_rec_Fc02_t_rests_ini(D_REC_INK_RTO);

	//Update shared memory
	
	 /**********/
    /* 初期化 */
    /**********/
    f_trp_com_Fmemclr(                                   /* 終了コード:R     */
        &ltReStaArrLTE,                                  /* RE状態アドレス:I */
        sizeof(T_RRH_REST_S3G)                           /* RE状態サイズ:I   */
    );
	
	f_cmn_com_rest_s3g_set( cpr_no, &ltReStaArrLTE );
	
	
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
