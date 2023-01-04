/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_m_use_recrdssc_stp.c
 * @brief  運用中REカード状態報告(セクタ部)中止処理
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
 * @brief  運用中REカード状態報告(セクタ部)中止処理
 * @note   運用中REカード状態報告(セクタ部)中止処理を行う
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2008/07/21 FFCS)Shihzh modify for S3G PRC.
 * @date   2010/08/11 CMS)Takahashi modify for TS-656
 * @date   2010/09/02 CMS)Takahashi modify for M-S3G-eNBPF-03726
 * @date   2016/10/04 KCN)hfuku 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_m_use_recrdssc_stp(                           /* なし:R */
    VOID* bufp                                        /* バッファポインタ:I */
) {
    UINT                key_inf;                       /* タイマキー情報 */
    T_REC_TIMELM        elm;                              /* タイマ要素     */
    USHORT              slt_no;                           /* スロット番号   */
//    UINT                uit_ret;
    CHAR                errdata[CMD_NUM48];
    SHORT               tc;                               /* 残トライ回数   */
    USHORT              cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
    USHORT              slt_index;
	USHORT slot_mskbit[CMD_NUM16] = {0xFFFE,0xFFFD,0xFFFB,0xFFF7,0xFFEF,0xFFDF,0xFFBF,0xFF7F,
									0xFEFF,0xFDFF,0xFBFF,0xF7FF,0xEFFF,0xDFFF,0xBFFF,0x7FFF};

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /* キー情報抽出 */
    key_inf = ((T_RRH_TIMERTO_NTC*)bufp)->data.userkey;

    /****************************/
    /* タイマキー情報→要素変換 */
    /****************************/
    f_trp_rec_Fcom_t_key_elm_cnv(                                 /* なし:R */
        key_inf,                                             /* キー情報:I */
        &elm                                                      /* 要素:O */
    );

    /* Slot番号抽出 */
    slt_no = elm.soc_no;

	/******************************************/
	/* 残トライ回数(REカード状態報告要求)取得 */
	/******************************************/
    tc = f_trp_rec_Fc02_t_use_recrdstsreq_get(            /* 残トライ回数:R */
        slt_no                                            /* スロット番号:I */
    );

    snprintf(errdata, CMD_NUM48, "[cpr:%02d][slt:%02d][TRY:%d][LTE]CardSts T.O.", D_REC_C02_CPR_NO(), slt_no, tc);
    D_REC_IVLINI();
    D_REC_IVLSET_SIZE(bufp, sizeof(T_MSGHEAD));
    D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, errdata);

	/* 送受信ログの退避	*/
	f_trp_rec_Fcom_t_l3_LogSave( D_REC_LOGKIND_CARD, D_REC_C02_CPR_NO() - 1, D_REC_SAVEKIND_TIMEOUT );

	/* RE状態報告要求トライ可能な場合 */
	if (tc > CMD_NUM0) {
	    /**************************************************/
	    /* 残トライ回数(REカード状態報告要求)デクリメント */
	    /**************************************************/
	    f_trp_rec_Fc02_t_use_recrdstsreq_dec(                     /* なし:R */
	        slt_no                                        /* スロット番号:I */
	    );
		return;
	}

    /******************************************************/
    /* RE状態(セクタ部)管理テーブル(REカード状態)初期化   */
    /******************************************************/
    f_trp_rec_Fc02_t_recrdssc_i_ini(
        D_REC_INK_RTO, 
        slt_no
    );

	/* Slot番号からスロットIndex値(0-15)を算出する 16B KCN add */
	f_cmn_com_slotsrch_s3g(D_REC_C02_CPR_NO(), slt_no, &slt_index);

    /* REカード状態報告 送信slotのBitをOFF 16B KCN add */
    timeRecrdSndCount[cpr_no_idx] &= slot_mskbit[slt_index];

// TBD
//		/************************/
//		/* RE未実装強化ログ待避 */
//		/************************/
//		f_trp_com_FLog_switch();
//    }
    
    return;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
