/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Rc03_m_txr_to.c
 * @brief  自律TRX開放中のREからの応答待ちタイムアウト時
 *         保護段数未達ならばREにTRX開放要求送信
 *         保護段到達ならば強制停止
 * @date
 *
 *
 */
/****************************************************************************/
#include "f_trp_rec.h"

/****************************************************************************/
/*!
 * @brief  自律TRX開放中のREからの応答待ちタイムアウト時
 *         保護段数未達ならばREにTRX開放要求送信
 *         保護段到達ならば強制停止
 * @date
 * @date   2015/11/20 FPT)Yen Update (強制停止)
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_txr_to(VOID *bufp) {
	USHORT	tc;
	CHAR	errdata[CMD_NUM48];

	/*********************/
    /* 残トライ回数取得  */
    /*********************/
    tc = f_trp_rec_Fc03_t_tc_txr_get(                             /* なし:R */
    );
	/* TRX制御状態 = TRX解放(自律)中 */
	/* retry over */
    if (tc <= CMD_NUM0)
	{
		snprintf(errdata, CMD_NUM48, "[FRCSTP][cpr:%02d]self rel TRX Sending Over.", D_REC_C03_CPR_NO());
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

	/* L3(TRX解放要求)送信 */
	f_trp_rec_Fc03_t_l3_txrreq_snd();
	/*****************************/
	/* 残トライ回数デクリメント  */
	/*****************************/
	f_trp_rec_Fc03_t_tc_txr_dec(                                 /* なし:R */
	);
}

/****************************************************************************/
/*!
 * @brief  自律TRX開放中のREからの応答待ちタイムアウト時
 *         保護段数未達ならばREにTRX開放要求送信
 *         保護段到達ならば強制停止
 * @date
 * @date   2015/11/20 FPT)Yen Update (強制停止)
 */
/****************************************************************************/
VOID f_trp_rec_Rc03_m_txr_to_3g(VOID *bufp) {
	USHORT	tc;
	CHAR	errdata[CMD_NUM48];

	/*********************/
    /* 残トライ回数取得  */
    /*********************/
    tc = f_trp_rec_Fc03_t_tc_txr_get_3g(                             /* なし:R */
    );
	/* TRX制御状態 = TRX解放(自律)中 */
	/* retry over */
    if (tc <= CMD_NUM0)
	{
		snprintf(errdata, CMD_NUM48, "[FRCSTP][cpr:%02d]self rel TRX Sending Over.", D_REC_C03_3G_CPR_NO());
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

	/* L3(TRX解放要求)送信 */
	f_trp_rec_Fc03_t_l3_txrreq_snd_3g();
	/*****************************/
	/* 残トライ回数デクリメント  */
	/*****************************/
	f_trp_rec_Fc03_t_tc_txr_dec_3g(                                 /* なし:R */
	);
}
