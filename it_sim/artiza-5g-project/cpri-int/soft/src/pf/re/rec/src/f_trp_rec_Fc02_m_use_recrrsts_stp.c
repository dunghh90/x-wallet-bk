/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_recrrsts_stp.c
 * @brief  運用中REキャリア状態報告中止処理
 * @date   2008/07/21 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2010
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  運用中REキャリア状態報告中止処理
 * @note   運用中REキャリア状態報告中止処理を行う
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2008/07/21 FFCS)Shihzh create.
 * @date   2010/08/11 CMS)Takahashi modify for TS-656
 * @date   2010/09/02 CMS)Takahashi modify for M-S3G-eNBPF-03726
 * @TBD_No 修正未完了
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_recrrsts_stp(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
//    USHORT       sit_ret;
    CHAR         errdata[CMD_NUM48];
    SHORT        tc;                                        /* 残トライ回数 */

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /********************************************/
    /* 残トライ回数(REキャリア状態報告要求)取得 */
    /********************************************/
    tc = f_trp_rec_Fc02_t_use_recrrstsreq_get(            /* 残トライ回数:R */
    );

    snprintf(errdata, CMD_NUM48, "[cpr:%02d][TRY:%d][LTE]RE Carr sts report T.O.", D_REC_C02_CPR_NO(), tc);
    D_REC_IVLINI();
    D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
    D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

	/* 送受信ログの退避	*/
	f_trp_rec_Fcom_t_l3_LogSave( D_REC_LOGKIND_CARRIER, D_REC_C02_CPR_NO() - 1, D_REC_SAVEKIND_TIMEOUT );

	/* REキャリア状態報告要求トライ可能な場合 */
	if (tc > CMD_NUM0) {
	    /**************************************************/
	    /* 残トライ回数(REカード状態報告要求)デクリメント */
	    /**************************************************/
	    f_trp_rec_Fc02_t_use_recrrstsreq_dec(                     /* なし:R */
	    );
		return;
	}
    /******************************************************/
    /* RE状態(セクタ部)管理テーブル(REキャリア状態)初期化 */
    /******************************************************/
    f_trp_rec_Fc02_t_recrrsts_a_ini(D_REC_INK_RTO);
    
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
