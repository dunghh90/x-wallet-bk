/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_reautrst_exe_3g.c
 * @brief  REカード自律リセット実施有無判定
 * @date   2014/07/10
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2014-2016
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
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_reautrst_exe(                  /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {

    struct bpf_timeval  rstBaseTim;                                 /* 時刻 */
	USHORT lwCprNo = D_REC_C02_CPR_NO();            /* CPRI番号インデックス */
    CHAR                      errdata[CMD_NUM48];

// [16B] add start
//    bpf_ru_hrtm_key_inf_t  key_inf;                       /* タイマキー情報 */
    UINT                key_inf;                          /* タイマキー情報 */
    T_REC_TIMELM        elm;                                  /* タイマ要素 */
    USHORT slt_no;                                          /* スロット番号 */
    USHORT slt_idx;                             /* スロット番号インデックス */
// [16B] add end

// [16B] add start
    /* キー情報抽出 */
//    key_inf = ((CMT_TSKIF_TIMOUTNTC*)bufp)->key_inf;
    key_inf = ((T_RRH_TIMERTO_NTC*)bufp)->data.userkey;

    /****************************/
    /* タイマキー情報→要素変換 */
    /****************************/
    f_trp_rec_Fcom_t_key_elm_cnv(                                 /* なし:R */
//        &key_inf,                                             /* キー情報:I */
        key_inf,                                              /* キー情報:I */
        &elm                                                      /* 要素:O */
    );

    /* Slot番号抽出 */
    slt_no  = elm.soc_no;

    /* スロット番号からIndex値を取得 */
    f_cmn_com_slotsrch_s3g(lwCprNo, slt_no, &slt_idx);
// [16B] add end

	/*REカード自律リセット待ちタイマON場合 */
	if( CMD_FLG_ON == f_trp_com_AutoResetMngTbl[lwCprNo - CMD_NUM1][slt_idx].ReAutoRstTimFlg )
	{
		/* REカード自律リセット待ちタイマOFF */
		f_trp_com_AutoResetMngTbl[lwCprNo - CMD_NUM1][slt_idx].ReAutoRstTimFlg = CMD_FLG_OFF;
		
#if 0  // [16B] del start
		/* RE自律リセット制御中フラグON */
		f_trp_com_AutoResetMngTbl[lwCprNo - CMD_NUM1].ReAutoRstCtlFlg = CMD_FLG_ON;
#endif // [16B] del end
		
		/* Get base time */
		(VOID)BPF_RU_UTTM_GETTIME( &rstBaseTim );
		
		/* RE自律リセット時刻保存 */
		f_trp_com_AutoResetMngTbl[lwCprNo - CMD_NUM1][slt_idx].resetTime = rstBaseTim.tv_sec;
		
		snprintf(errdata, CMD_NUM48, "[cpr:%02d][LTE]ReAutoRstCtlFlg on. resetTime set:%d",lwCprNo,(INT)rstBaseTim.tv_sec);
		D_REC_IVLINI();
        D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
		/****************************/
		/* L3(REカード制御要求)送信 */
		/****************************/
		f_trp_rec_Fcom_t_l3_recrdctlreq_snd(                    /* 終了コード:R */
			lwCprNo,                                   /* CPRI番号:I */
//			CMD_NUM1,                                         /* スロット番号:I */
			slt_no,                                           /* スロット番号:I */
			CMD_CARDRST                                           /* 制御種別:I */
		);
// [16B] add start
//		/* RE自律リセット制御中フラグOFF */
//		f_trp_com_AutoResetMngTbl[lwCprNo - CMD_NUM1][slt_idx].ReAutoRstCtlFlg = CMD_FLG_OFF;
   		/* RE自律リセット抑止フラグON */
   		f_trp_com_AutoResetMngTbl[lwCprNo - CMD_NUM1][slt_idx].ReAutoRstPreventFlg = CMD_FLG_ON;
// [16B] add end

	}else{
		/* 自律リセット48時間未満 */
	}
    return;
	
}
/** @} */
