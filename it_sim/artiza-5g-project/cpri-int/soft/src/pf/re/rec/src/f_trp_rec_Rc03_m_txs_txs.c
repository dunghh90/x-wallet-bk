/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Rc03_m_txs_txs.c
 * @brief  TRX設定応答がNGかつ保護段数未満ならばREにTRX設定要求送信
 *         応答NGかつ保護段数到達ならば強制停止
 * @date
 *
 *
 */
/****************************************************************************/
#include "f_trp_rec.h"

/****************************************************************************/
/*!
 * @brief  TRX設定応答がNGかつ保護段数未満ならばREにTRX設定要求送信
 *         応答NGかつ保護段数到達ならば強制停止
 * @note   
 * @param  bufp [in]
 * @return none
 * @date   2015/10/02 FPT) Dung fix bug IT1(correct ret_code value)
 * @date   2015/11/20 FPT) Yen Update (強制停止)
 * @date   2016/02/25 TDI)satou 確認待ちタイマで再送を処理するので、ここでは再送を処理しないように
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_txs_txs(VOID *bufp) {
	USHORT	ret_code;							/* 結果コード */

    /*
     * NG応答を受信した場合、確認タイマがタイムアウトしたタイミングで再送を処理したいので、
     * ここでは再送せずに処理を抜け、確認タイマがタイムアウトするのを待つ.そのため、以下の
     * 処理をコメント化(#if 0).
     */
#if 0
	USHORT	tc;
	CHAR	errdata[CMD_NUM48];

	 /**************/
    /* タイマ取消 */
    /**************/
    f_trp_rec_Fc03_t_timer_ccl(					/* なし:R */
        EC_REC_TMK_TXS_CNF                           	/* TRX設定確認タイマ:I */
    );
#endif

	/* 結果コード抽出 */
    ret_code = ((CMT_TSKIF_CPRIRCV_TRXSETRES*)bufp)->cpridat_trxsetres.result;
	/* 応答結果が「0:正常の場合 */
	/* OKの場合 */
    if (ret_code == CMD_NML) {
   	    /**************/
        /* タイマ取消 */
        /**************/
        f_trp_rec_Fc03_t_timer_ccl(
            EC_REC_TMK_TXS_CNF
        );
    	/******************/
	    /* 自状態番号設定 */
	    /******************/
	    f_trp_rec_Fc03_t_ownstn_set(			/* なし:R */
	        EC_REC_C03_STN_IDLE					/* アイドル */
	    );

    	return;
    }

    /*
     * NG応答を受信した場合、確認タイマがタイムアウトしたタイミングで再送を処理したいので、
     * ここでは再送せずに処理を抜け、確認タイマがタイムアウトするのを待つ.そのため、以下の
     * 処理をコメント化(#if 0).
     */
#if 0
	tc = f_trp_rec_Fc03_t_tc_txs_get(                             /* なし:R */
    );
	/* retry over */
    if (tc <= CMD_NUM0)
	{
		snprintf(errdata, CMD_NUM48, "[FRCSTP][cpr:%02d]self set TRX Sending Over.", D_REC_C03_CPR_NO());
		D_REC_IVLINI();
		D_REC_IVLSET_SIZE(bufp, sizeof(T_RRH_TIMERTO_NTC));
		D_REC_IVLOUT(D_TCM_INVLOGLV_WARN2, errdata);
		/******************/
	    /* 自状態番号設定 */
	    /******************/
	    f_trp_rec_Fc03_t_ownstn_set(			/* なし:R */
	        EC_REC_C03_STN_IDLE					/* アイドル */
	    );
		/************************************************/
		/* スレッド間(強制停止プロシージャ実行指示)送信 */
		/************************************************/
		f_trp_rec_Fc02_t_it_frcstpprc_snd(E_RRH_RESTOP_FACTOR_MISMATCH_CARR);

		return;
    }

	/* L3(TRX設定要求)送信 */
	f_trp_rec_Fc03_t_l3_txsreq_snd();
	/*****************************/
	/* 残トライ回数デクリメント  */
	/*****************************/
	f_trp_rec_Fc03_t_tc_txs_dec(                                  /* なし:R */
	);
#endif
}

/****************************************************************************/
/*!
 * @brief  TRX設定応答がNGかつ保護段数未満ならばREにTRX設定要求送信
 *         応答NGかつ保護段数到達ならば強制停止
 * @date
 * @date   2015/10/02 FPT) Dung fix bug IT1(correct ret_code value)
 * @date   2015/11/20 FPT) Yen Update (強制停止)
 * @date   2016/02/25 TDI)satou 確認待ちタイマで再送を処理するので、ここでは再送を処理しないように
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_txs_txs_3g(VOID *bufp) {
	USHORT	ret_code;							/* 結果コード */

    /*
     * NG応答を受信した場合、確認タイマがタイムアウトしたタイミングで再送を処理したいので、
     * ここでは再送せずに処理を抜け、確認タイマがタイムアウトするのを待つ.そのため、以下の
     * 処理をコメント化(#if 0).
     */
#if 0
	USHORT	tc;
	CHAR	errdata[CMD_NUM48];

	 /**************/
    /* タイマ取消 */
    /**************/
    f_trp_rec_Fc03_t_timer_ccl_3g(					/* なし:R */
        EC_REC_TMK_TXS_CNF                           	/* TRX設定確認タイマ:I */
    );
#endif

	/* 結果コード抽出 */
    ret_code = ((CMT_TSKIF_CPRIRCV_TRXSETRES*)bufp)->cpridat_trxsetres.result;
	/* 応答結果が「0:正常の場合 */
	/* OKの場合 */
    if (ret_code == CMD_NML) {
   	    /**************/
        /* タイマ取消 */
        /**************/
        f_trp_rec_Fc03_t_timer_ccl_3g(
            EC_REC_TMK_TXS_CNF
        );
    	/******************/
	    /* 自状態番号設定 */
	    /******************/
	    f_trp_rec_Fc03_t_ownstn_set_3g(			/* なし:R */
	        EC_REC_C03_STN_IDLE					/* アイドル */
	    );

    	return;
    }

    /*
     * NG応答を受信した場合、確認タイマがタイムアウトしたタイミングで再送を処理したいので、
     * ここでは再送せずに処理を抜け、確認タイマがタイムアウトするのを待つ.そのため、以下の
     * 処理をコメント化(#if 0).
     */
#if 0
	tc = f_trp_rec_Fc03_t_tc_txs_get_3g(                             /* なし:R */
    );
	/* retry over */
    if (tc <= CMD_NUM0)
	{
		snprintf(errdata, CMD_NUM48, "[FRCSTP][cpr:%02d]self set TRX Sending Over.", D_REC_C03_3G_CPR_NO());
		D_REC_IVLINI();
		D_REC_IVLSET_SIZE(bufp, sizeof(T_RRH_TIMERTO_NTC));
		D_REC_IVLOUT(D_TCM_INVLOGLV_WARN2, errdata);
		/******************/
	    /* 自状態番号設定 */
	    /******************/
	    f_trp_rec_Fc03_t_ownstn_set_3g(			/* なし:R */
	        EC_REC_C03_STN_IDLE					/* アイドル */
	    );
		/************************************************/
		/* スレッド間(強制停止プロシージャ実行指示)送信 */
		/************************************************/
		f_trp_rec_Fc02_t_it_frcstpprc_snd_3g(E_RRH_RESTOP_FACTOR_MISMATCH_CARR);

		return;
    }

	/* L3(TRX設定要求)送信 */
	f_trp_rec_Fc03_t_l3_txsreq_snd_3g();
	/*****************************/
	/* 残トライ回数デクリメント  */
	/*****************************/
	f_trp_rec_Fc03_t_tc_txs_dec_3g(                                  /* なし:R */
	);
#endif
}
