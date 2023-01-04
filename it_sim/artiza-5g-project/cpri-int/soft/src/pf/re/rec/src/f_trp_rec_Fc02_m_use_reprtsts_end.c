/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_reprtsts_end.c
 * @brief  運用中RE PORT状態報告終了処理
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
 * @brief	運用中RE PORT状態報告終了処理
 * @note	運用中RE PORT状態報告終了処理を行う
 *			- 確認タイマの停止
 * 			- 共有メモリに応答内容を保存
 * @param	bufp [in]  バッファポインタ
 * @return	None
 * @date	2016/04/25 TDI)satou create
 * @date	2016/05/13 FJT)Koshida update 16A RE Port対応(ログ強化,3断保護追加).
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_reprtsts_end(VOID* bufp) {
	VOID	*datp;
	VOID	*l3_bufp;
    UINT	uit_code;
    UCHAR	received_rsp;
    USHORT	rsp_result;
    USHORT	rsp_portin;
    CHAR	errdata[CMD_NUM48];
    
    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /**************/
    /* タイマ取消 */
    /**************/
	uit_code = f_trp_rec_Fc02_t_timer_ccl(EC_REC_TMK_REPRTSTS_CNF, D_REC_SLT_IV);
    if (uit_code == D_REC_TIM_IV) {
        return;
    }
    /**********************/
    /* 残トライ回数初期化 */
    /**********************/
    f_trp_rec_Fc02_t_use_reprtstsreq_ini(                         /* なし:R */
    );

    /**********************************************************************/
    /* 共有メモリに応答内容を保存. NGの場合、引数4はDon't Care */
    /**********************************************************************/
    datp       = &((T_REC_MSGRCV_TBL*)bufp)->datp;
    l3_bufp    = &(((CMT_TSKIF_CPRIRCV_SIGNALGET*)datp)->signal_kind);
    rsp_result = ((CMT_CPRIF_REPORTSTSRES*)l3_bufp)->result;
    /*	NG応答の場合は、f_cmn_com_RePortSta_s3g_setにてPort入力情報を更新しないため	*/
    /*	ここでは、OK/NGに関係なく一律でREからの応答情報を格納する					*/
    rsp_portin = ((CMT_CPRIF_REPORTSTSRES*)l3_bufp)->portsts_inf.port_in_sts;
    received_rsp = (CMD_NML == rsp_result) ? D_RRH_REPORTSTA_RSP_NORMAL : D_RRH_REPORTSTA_RSP_NG;
    f_cmn_com_RePortSta_s3g_set(D_REC_C02_CPR_NO(), received_rsp, rsp_result, rsp_portin);
    
    /* OK以外の場合 */
    if (rsp_result != CMD_NML)
    {
        snprintf(errdata, sizeof(errdata), "[NOTICE][cpr:%02d][ret:0x%04x]RE port sts report NG.", D_REC_C02_CPR_NO(), rsp_result); /* pgr0570 */
        D_REC_IVLINI();
        D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);
    }
}

/* @} */  /* group TRIF_REC */
