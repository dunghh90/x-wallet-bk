/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_autorst_chk.c
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
 * @brief  REカード自律リセット実施有無判定
 * @note   REカード自律リセット実施有無判定する
 *
 * @param  rcvp         [in]  カード状態報告
 * @return None
 * @date   2015/08/5 FPT)Quynh Create
 * @date   2015/11/30 FPT)Duong fix bug IT3 No 99
 * @date   2016/10/06 KCN)Takagi 16B機能追加
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_autorst_chk(                                /* 変化:R */
    CARDSTS_INF_S3G* rcvp                                 /* カード状態報告 */
) {
    USHORT          AutoResetTimeCheck = CMD_OFF; /* RE自律リセット実施時刻 */
    USHORT          lwCprNo;                        /* CPRI番号インデックス */
    CHAR                      errdata[CMD_NUM48];
// [16B] add start
    USHORT          slot_no;                                   // スロット番号
    USHORT          slt_idx;                    /* スロット番号インデックス */
// [16B] add end

	lwCprNo = D_REC_C02_CPR_NO() - CMD_NUM1;
// [16B] add start
    slot_no = rcvp->slot_no;
//    slt_idx = slot_no - CMD_NUM1;
    f_cmn_com_slotsrch_s3g(D_REC_C02_CPR_NO(), slot_no, &slt_idx);
// [16B] add end

#if 0 // [16B] del start
	/****************/
    /* 実装の場合 */
    /****************/
	if( CMD_FLG_ON == f_trp_com_AutoResetMngTbl[lwCprNo].ReAutoRstCtlFlg )
	{
		//f_trp_com_AutoResetMngTbl[lwCprNo].ReAutoRstCtlFlg = CMD_FLG_OFF;
		return;
	}
#endif // [16B] del end

// [16B] add start
    /* 未実装 */
    /* 自律リセットしないケース */
    if(rcvp->card_svitem == CMD_CRDSVBIT_NOEQP)
    {
        // 制御項目がリモートリセットでない場合
//        if((rcvp->card_cntitem & CMD_CRDCTBIT_R_RST) != CMD_CRDCTBIT_R_RST)
        // ★→自律リセット待ち状態の場合
    	if( CMD_FLG_ON == f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstTimFlg )
        {
			/***********************************************/
			/* タイマ取消 (REカード自律リセット待ちタイマ)**/
			/***********************************************/
			f_trp_rec_Fc02_t_timer_ccl(                            /* なし:R */
				EC_REC_TMK_REAUTRST_WAT,   /* RE Auto Reset 送信待ちタイマ:I */
		 		slot_no                                    /* スロット番号:I */
			);
			// 該当スロットの自律リセットの情報をクリアする
			 f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstTimFlg = CMD_FLG_OFF;
//			 f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstCtlFlg = CMD_FLG_OFF;
#if 0  // 初期化されており冗長なため処理削除
			 f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].resetTime = D_TCM_DEFAULT_RSTTIM;
			 f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstPreventFlg = CMD_FLG_OFF;
#endif // 

        }
    }
// [16B] add end

	/* 自律リセット抑止フラグOFF場合 */
	if( CMD_FLG_OFF == f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstPreventFlg )
	{
		/* REカード状態がALM場合  */
		if( ( CMD_CRDSVBIT_ALM == ( rcvp->card_svitem & CMD_CRDSVBIT_ALM ) ) &&
			( CMD_NUM0         == ( rcvp->card_svitem & CMD_CRDSVBIT_ACT ) ) )
		{

			/* If the timer is not running, start timer*/
			if( CMD_FLG_OFF == f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstTimFlg )
			{
				/**********************************************/
				/* タイマ登録(REカード自律リセット待ちタイマ) */
				/**********************************************/
				f_trp_rec_Fc02_t_timer_ent(                            /* なし:R */
					EC_REC_TMK_REAUTRST_WAT,   /* RE Auto Reset 送信待ちタイマ:I */
 					slot_no                                    /* スロット番号:I */
				);
				/* REカード自律リセット待ちタイマON */
				f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstTimFlg = CMD_FLG_ON;

			}
		}
	}
	else if(CMD_FLG_ON == f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstPreventFlg)
	{
		/* 自律リセット抑止フラグON場合 */
		/* ALM保護時間外場合(48時間経過している場合) */
// [16B] chg start
//		AutoResetTimeCheck = f_trp_rec_Fc02_t_AutoResetTime_chk();
		AutoResetTimeCheck = f_trp_rec_Fc02_t_AutoResetTime_chk(slt_idx);  // slt_idx(0-15)
// [16B] chg end
		
		if( CMD_ON == AutoResetTimeCheck )
		{

// [16B] chg start
//			/* REカード状態が非ALM場合 */
//			if( CMD_NUM0 == ( rcvp->card_svitem & CMD_CRDSVBIT_ALM ) )
            /* ALM Bit OFF または ACT Bit ON */
			if( (CMD_NUM0 == ( rcvp->card_svitem & CMD_CRDSVBIT_ALM )) ||
			    (CMD_CRDSVBIT_ACT == ( rcvp->card_svitem & CMD_CRDSVBIT_ACT )) )
// [16B] chg end
			{
				/***********************************************/
				/* タイマ取消 (REカード自律リセット待ちタイマ)**/
				/***********************************************/
				f_trp_rec_Fc02_t_timer_ccl(                            /* なし:R */
					EC_REC_TMK_REAUTRST_WAT,   /* RE Auto Reset 送信待ちタイマ:I */
			 		slot_no                                    /* スロット番号:I */
				);
				/* REカード自律リセット待ちタイマOFF */
				f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstTimFlg = CMD_FLG_OFF;
			}
			/* RE自律リセット抑止フラグOFF */
			f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstPreventFlg = CMD_FLG_OFF;
			/* RE自律リセット時刻クリア */
			f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].resetTime = D_TCM_DEFAULT_RSTTIM;
			
			snprintf(errdata, CMD_NUM48, "[cpr:%02d][LTE]ReAutoRstPreventFlg clear.",lwCprNo+1);
			D_REC_IVLINI();
			D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
		}else{
			/* 48時間以内にALM発生した場合は保持する */
			if( CMD_CRDSVBIT_ALM == ( rcvp->card_svitem & CMD_CRDSVBIT_ALM ) )
			{
				f_trp_com_AutoResetMngTbl[lwCprNo][slt_idx].ReAutoRstPreventFlg = CMD_ALM_HOLIDING;
				snprintf(errdata, CMD_NUM48, "[cpr:%02d][LTE]ReAutoRstPreventFlg alm holding.",lwCprNo+1);
				D_REC_IVLINI();
				D_REC_IVLOUT(D_TCM_INVLOGLV_NOTICE, errdata);
			}
		}
	}else{
		// HOLDINGの場合は何もしない
	}

    return;
}
/* @} */  /* group TRIF_REC */

/****************************************************************************/

