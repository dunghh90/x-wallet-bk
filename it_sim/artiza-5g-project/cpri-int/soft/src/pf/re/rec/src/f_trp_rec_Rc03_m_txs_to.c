/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Rc03_m_txs_to.c
 * @brief  自律TRX設定中のREからの応答待ちタイムアウト時
 *         保護段数未達ならばREにTRX設定要求送信
 *         保護段到達ならば強制停止
 * @date
 *
 *
 */
/****************************************************************************/
#include "f_trp_rec.h"

/****************************************************************************/
/*!
 * @brief  自律TRX設定中のREからの応答待ちタイムアウト時
 *         保護段数未達ならばREにTRX設定要求送信
 *         保護段到達ならば強制停止
 * @date   2015/11/10 TDIPS)Takeuchi 強制停止を通知するように
 * @date   2015/11/20 FPT) Yen Update (強制停止)
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_txs_to(VOID *bufp) {
	USHORT	tc;
	CHAR	errdata[CMD_NUM48];

	/*********************/
    /* 残トライ回数取得  */
    /*********************/
    tc = f_trp_rec_Fc03_t_tc_txs_get(                             /* なし:R */
    );
	/* TRX制御状態 = TRX設定(自律)中 */
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
}

/****************************************************************************/
/*!
 * @brief  自律TRX設定中のREからの応答待ちタイムアウト時
 *         保護段数未達ならばREにTRX設定要求送信
 *         保護段到達ならば強制停止
 * @date   2015/11/10 TDIPS)Takeuchi 強制停止を通知するように
 * @date   2015/11/20 FPT) Yen Update (強制停止)
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_txs_to_3g(VOID *bufp) {
	USHORT	tc;
	CHAR	errdata[CMD_NUM48];

	/*********************/
    /* 残トライ回数取得  */
    /*********************/
    tc = f_trp_rec_Fc03_t_tc_txs_get_3g(                             /* なし:R */
    );
	/* TRX制御状態 = TRX設定(自律)中 */
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
}
