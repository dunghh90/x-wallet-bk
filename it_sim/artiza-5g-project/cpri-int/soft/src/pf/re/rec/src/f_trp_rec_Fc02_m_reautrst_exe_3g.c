/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_reautrst_exe_3g.c
 * @brief  REカード自律リセット実施有無判定
 * @date   2014/07/10
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2014
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */
/****************************************************************************/
/*!
 * @brief  (REカード制御要求)送信実行処理
 * @note   (REカード制御要求)送信実行処理を行う
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2015/08/05 FPT)Quynh create.
 * @date   2015/11/17 FPT)Tuan fix bug.
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_reautrst_exe_3g(                /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

    struct bpf_timeval  rstBaseTim;                                 /* 時刻 */
	USHORT lwCprNo = D_REC_C02_3G_CPR_NO();         /* CPRI番号インデックス */
    CHAR                      errdata[CMD_NUM48];

	/*REカード自律リセット待ちタイマON場合 */
	if( CMD_FLG_ON == f_trp_com_AutoResetMngTbl_3g[lwCprNo - CMD_NUM1].ReAutoRstTimFlg )
	{
		/* REカード自律リセット待ちタイマOFF */
		f_trp_com_AutoResetMngTbl_3g[lwCprNo - CMD_NUM1].ReAutoRstTimFlg = CMD_FLG_OFF;
		
		/* RE自律リセット制御中フラグON */
		f_trp_com_AutoResetMngTbl_3g[lwCprNo - CMD_NUM1].ReAutoRstCtlFlg = CMD_FLG_ON;
		
		/* Get base time */
		(VOID)BPF_RU_UTTM_GETTIME( &rstBaseTim );
		
		/* RE自律リセット時刻保存 */
		f_trp_com_AutoResetMngTbl_3g[lwCprNo - CMD_NUM1].resetTime = rstBaseTim.tv_sec;
		
		snprintf(errdata, CMD_NUM48, "[cpr:%02d][LTE]ReAutoRstCtlFlg on. resetTime set:%d",lwCprNo,(INT)rstBaseTim.tv_sec);
		D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
		/****************************/
		/* L3(REカード制御要求)送信 */
		/****************************/
		f_trp_rec_Fcom_t_l3_recrdctlreq_snd(                    /* 終了コード:R */
			lwCprNo,                                              /* CPRI番号:I */
			CMD_NUM1,                                         /* スロット番号:I */
			CMD_CARDRST                                           /* 制御種別:I */
		);
	}
    return;
	
}
/** @} */
