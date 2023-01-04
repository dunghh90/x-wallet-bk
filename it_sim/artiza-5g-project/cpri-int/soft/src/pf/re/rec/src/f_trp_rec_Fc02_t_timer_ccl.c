/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_timer_ccl.c
 * @brief  タイマ取消
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
 * @brief  タイマ取消
 * @note   タイマを取り消す
 *         - タイマ取消
 *         - タイマ管理テーブル削除
 * @param  tmk    [in]  タイマ種別
 * @param  slt_no [in]  スロット番号
 * @return 終了コード
 * @retval D_REC_TIM_NG:1
 * @retval D_REC_TIM_IV:2
 * @date   2007/03/20 FJT)Nagasima create.
 * @date   2010/11/12 FJT)Tokunaga M-S3G-eNBSYS-xxxxx [TS-810項]
 */
/****************************************************************************/
UINT f_trp_rec_Fc02_t_timer_ccl(                            /* 終了コード:R */
    ET_REC_TMK tmk,                                         /* タイマ種別:I */
    USHORT slt_no                                         /* スロット番号:I */
) {
    INT                    sit_ret;                           /* 終了コード */
    UINT                   uit_ret;                           /* 終了コード */
    T_REC_TIMELM           elm;                               /* タイマ要素 */
    bpf_ru_hrtm_key_inf_t  key_inf;                       /* タイマキー情報 */
    UINT                   tim_id;                          /* タイマ識別子 */
    CHAR   errdata[CMD_NUM48];
	UINT				   logDat;

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);
#ifdef FHM_DEBUG_TIMER_FOR_IT1
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    /* 要素設定 */
    elm.cls    = EC_REC_C02;                                      /* クラス */
    elm.tmk    = tmk;                                         /* タイマ種別 */
	elm.cpr_no = D_REC_C02_CPR_NO();
    elm.soc_no = slt_no;                                    /* スロット番号 */

    /****************************/
    /* タイマ要素→キー情報変換 */
    /****************************/
    f_trp_rec_Fcom_t_elm_key_cnv(                                 /* なし:R */
        &elm,                                                     /* 要素:I */
        &key_inf                                              /* キー情報:O */
    );

    /**************************/
    /* タイマ管理テーブル検索 */
    /**************************/
    tim_id = f_trp_rec_Fcom_t_timmng_tbl_sch(             /* タイマ識別子:R */
        &key_inf                                              /* キー情報:O */
    );
    /* 一致情報なしの場合 */
    if (tim_id == CMD_NUM0) {

		/*	応答が遅れている為、送受信ログ退避	*/
		if(( tmk == EC_REC_TMK_RECRDSSC_CNF ) ||
		   ( tmk == EC_REC_TMK_RECRDSCM_CNF ) ){ /* カード状態報告応答待ちタイマ	*/
			f_trp_rec_Fcom_t_l3_LogSave( D_REC_LOGKIND_CARD, D_REC_C02_CPR_NO() - 1, D_REC_SAVEKIND_TIMESTOPNG );
		}

		else if( tmk == EC_REC_TMK_RECRRSTS_CNF ){	/* キャリア状態報告応答待ちタイマ	*/
			f_trp_rec_Fcom_t_l3_LogSave( D_REC_LOGKIND_CARRIER, D_REC_C02_CPR_NO() - 1, D_REC_SAVEKIND_TIMESTOPNG );
		}
#ifndef DEBUG_TRACE_T
        return D_REC_TIM_IV;
#else
        return CMD_OK;
#endif
    }

    if(tmk == EC_REC_TMK_REAUTRST_WAT)
    {
		snprintf(errdata, CMD_NUM48, "[NTC]RE AUTO RST timer stop.");
		D_REC_IVLINI();
		logDat = D_REC_C02_CPR_NO();
		D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
		logDat = elm.tmk;
		D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
		logDat = slt_no;
		D_REC_IVLSET_SIZE(&logDat, sizeof(logDat));
		D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
	}
    /**************/
    /* タイマ取消 */
    /**************/
    sit_ret = f_trp_com_Fcancel_tim(                        /* 終了コード:R */
        key_inf.keyinf1                                   /* タイマ識別子:I */
    );
    /* タイマ取消NGの場合 */
    if (sit_ret != CMD_OK) {
        /* すれ違いなので、ここでは何もしない */
        /* →タイムアウト発生通知を破棄       */

		/*	応答が遅れている為、送受信ログ退避	*/
		if(( tmk == EC_REC_TMK_RECRDSSC_CNF ) ||
		   ( tmk == EC_REC_TMK_RECRDSCM_CNF ) ){ /* カード状態報告応答待ちタイマ	*/
			f_trp_rec_Fcom_t_l3_LogSave( D_REC_LOGKIND_CARD, D_REC_C02_CPR_NO() - 1, D_REC_SAVEKIND_TIMESTOPNG );
		}

		else if( tmk == EC_REC_TMK_RECRRSTS_CNF ){	/* キャリア状態報告応答待ちタイマ	*/
			f_trp_rec_Fcom_t_l3_LogSave( D_REC_LOGKIND_CARRIER, D_REC_C02_CPR_NO() - 1, D_REC_SAVEKIND_TIMESTOPNG );
		}

    }

    /**************************/
    /* タイマ管理テーブル削除 */
    /**************************/
    uit_ret = f_trp_rec_Fcom_t_timmng_tbl_del(              /* 終了コード:R */
        &key_inf                                              /* キー情報:I */
    );
    /* 一致情報なしの場合 */
    if (uit_ret != CMD_OK) {
        /* 無効処理履歴 */
        D_REC_IVLINI();
        D_REC_IVLSET(key_inf);
        D_REC_IVLOUT(D_TCM_INVLOGLV_WARN1, "Timer management table changed unexcept.");
        return D_REC_TIM_NG;
    }

    return CMD_OK;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
