/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_reprtsts_stp.c
 * @brief  運用中RE PORT状態報告中止処理
 * @date   2016/04/25 TDI)satou create
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2016
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief	運用中RE PORT状態報告中止処理
 * @note	運用中RE PORT状態報告中止処理を行う
 * @param	bufp [in]  バッファポインタ
 * @return	None
 * @date	2016/04/25 TDI)satou create
 * @date	2016/05/13 FJT)Koshida update 16A RE Port対応(3断保護追加).
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_reprtsts_stp(VOID* bufp) {
    CHAR errdata[CMD_NUM48];
    USHORT cpr_no = D_REC_C02_CPR_NO();
    USHORT cpr_no_idx = cpr_no -1;
    USHORT tc;
    
    /****************************************************/
    /* ログ保存 */
    /****************************************************/
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
    
    /*	INVALIDログ	*/
    snprintf(errdata, sizeof errdata, "[cpr:%02d][TRY=%d][LTE]RE port sts report T.O", 
    		 cpr_no, f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.m_reprtstsreq);
    D_REC_IVLINI();
    D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
    D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

	/****************************/
	/*	運用中残トライチェック	*/
	/****************************/
	/*	運用中残トライ回数(RE状態報告要求)を取得する	*/
	tc = f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.m_reprtstsreq;
	
	/*	RE状態報告要求トライ可能な場合	*/
	if(tc > CMD_NUM0)
	{
		/*	運用中残トライ回数(RE状態報告要求)をデクリメントする	*/
		f_trp_rec_Wc02_stsmng_tbl.sts[cpr_no_idx].tc.m_reprtstsreq--;
		
		/*	管理テーブルの初期化せずに前状態保持として処理終了	*/
		return;
	}
    /******************************************************************/
    /* 共有メモリ更新. T.Oの場合、引数3と4はDon't Care */
    /******************************************************************/
    f_cmn_com_RePortSta_s3g_set(cpr_no, D_RRH_REPORTSTA_RSP_TO, 0, 0);
}

/* @} */  /* group TRIF_REC */
