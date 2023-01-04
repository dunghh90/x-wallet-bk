/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_autorst_chk_3g.c
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
 * @brief  REカード自律リセット実施有無判定
 * @note   REカード自律リセット実施有無判定する
 *
 * @param  rcvp         [in]  カード状態報告
 * @return None
 * @date   2015/08/5 FPT)Quynh Create
 * @date   2015/11/17 FPT)Tuan fix bug.
 * @date   2015/11/30 FPT)Duong fix bug IT3 No 99
 * @date   2015/12/04 FPT)Tuan fix bug IT3 119
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_autorst_chk_3g(                            /* 変化:R */
    CARDSTS_INF* rcvp                                    /* カード状態報告 */
) {
    USHORT          AutoResetTimeCheck = CMD_OFF; /* RE自律リセット実施時刻 */
    USHORT          lwCprNo;                        /* CPRI番号インデックス */
    CHAR                      errdata[CMD_NUM48];

	lwCprNo = D_REC_C02_3G_CPR_NO() - CMD_NUM1;
	/****************/
    /* 実装の場合 */
    /****************/
	if( CMD_FLG_ON == f_trp_com_AutoResetMngTbl_3g[lwCprNo].ReAutoRstCtlFlg )
	{
		//f_trp_com_AutoResetMngTbl_3g[lwCprNo].ReAutoRstCtlFlg = CMD_FLG_OFF;
		return;
	}

	/* 自律リセット抑止フラグOFF場合 */
	if( CMD_FLG_OFF == f_trp_com_AutoResetMngTbl_3g[lwCprNo].ReAutoRstPreventFlg )
	{
		/* REカード状態がALM場合  */
		if( ( CMD_CRDSVBIT_ALM == ( rcvp->card_svitem & CMD_CRDSVBIT_ALM ) ) &&
			( CMD_NUM0         == ( rcvp->card_svitem & CMD_CRDSVBIT_ACT ) ) )
		{
			/* If the timer is not running, start timer*/
			if( CMD_FLG_OFF == f_trp_com_AutoResetMngTbl_3g[lwCprNo].ReAutoRstTimFlg )
			{
				/**********************************************/
				/* タイマ登録(REカード自律リセット待ちタイマ) */
				/**********************************************/
				f_trp_rec_Fc02_t_timer_ent_3g(                            /* なし:R */
					EC_REC_TMK_REAUTRST_WAT,   /* RE Auto Reset 送信待ちタイマ:I */
 					CMD_NUM1                                   /* スロット番号:I */
				);
				/* REカード自律リセット待ちタイマON */
				f_trp_com_AutoResetMngTbl_3g[lwCprNo].ReAutoRstTimFlg = CMD_FLG_ON;
			}
		}
	}
	else
	{
		/* 自律リセット抑止フラグON場合 */
		/* ALM保護時間外場合 */
		AutoResetTimeCheck = f_trp_rec_Fc02_t_AutoResetTime_chk_3g();
		
		if( CMD_ON == AutoResetTimeCheck )
		{
			/* REカード状態が非ALM場合 */
			if( CMD_NUM0 == ( rcvp->card_svitem & CMD_CRDSVBIT_ALM ) )
			{
				/***********************************************/
				/* タイマ取消 (REカード自律リセット待ちタイマ)**/
				/***********************************************/
				f_trp_rec_Fc02_t_timer_ccl_3g(                            /* なし:R */
					EC_REC_TMK_REAUTRST_WAT,   /* RE Auto Reset 送信待ちタイマ:I */
			 		CMD_NUM1                                   /* スロット番号:I */
				);
				/* REカード自律リセット待ちタイマOFF */
				f_trp_com_AutoResetMngTbl_3g[lwCprNo].ReAutoRstTimFlg = CMD_FLG_OFF;
			}
			/* RE自律リセット抑止フラグOFF */ 
			f_trp_com_AutoResetMngTbl_3g[lwCprNo].ReAutoRstPreventFlg = CMD_FLG_OFF;
			/* RE自律リセット時刻クリア */
			f_trp_com_AutoResetMngTbl_3g[lwCprNo].resetTime = D_TCM_DEFAULT_RSTTIM;
			
			snprintf(errdata, CMD_NUM48, "[cpr:%02d][3G]ReAutoRstPreventFlg clear.",lwCprNo);
			D_REC_IVLINI();
			D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
		}
	}

    return;
}
/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
