/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc02_t_recrdssc_i_merge.c
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態)集約(マージ)
 * @date   2016/10/05 KCN)Fukushima create.
 * @date   2017/04/10 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2016-2017
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "../../../l3/rct/inc/m_rc_def.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  RE状態(セクタ部)管理テーブル(REカード状態)集約(マージ)
 * @note   RE状態(セクタ部)管理テーブル(REカード状態)を集約(マージ)する
 * @param  なし
 * @return None
 * @date   2016/10/05 KCN)Fukushima create.
 * @date   2017/04/10 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_recrdssc_i_merge(                           /* 変化:R */
) {
	USHORT cpr_no_idx = D_REC_C02_CPR_NO() - CMD_NUM1;
	USHORT lwTotalCardSvitem = CMD_NUM0;		/* カード基本監視頁E岼			*/
	USHORT lwTotalCardExtSvitem = CMD_NUM0;		/* カード拡張監視頁E岼			*/
	USHORT lwTotalCardCntitem = CMD_NUM0;		/* カード制御項目 */
	USHORT slot_num;
	USHORT slot_cnt;
	USHORT alm_num = CMD_NUM0;
	USHORT err_num = CMD_NUM0;
	static T_RRH_CARDST_S3G_FUNC_EVERY	ltFuncEvery[RCD_FB_FHM_NUM];
	USHORT lwFuncNum = CMD_NUM0;
	USHORT lwFuncCnt;
	USHORT lwFuncKind;
	USHORT lwFuncId;
	USHORT lwSmallCarNum[RCD_FB_FHM_NUM] = {0};
	UINT   ldwMaxAlmNum = CMD_MAX_ALM_NUM;
	UINT   ldwMaxErrNum = CMD_MAX_ERR_NUM;
	USHORT lwCpyAlmNum;
	USHORT lwCpyErrNum;
	T_RRH_CARDST_S3G	ltCardStaSltLTE;			/* (LTE)REカード状態(Slot毎)	*/
	T_RRH_CARDST_S3G	ltCardStaLTE;				/* (LTE)REカード状態 			*/
	USHORT lwCpriFail = 0;
	USHORT lwSigKind = 0;
	USHORT lwResult = CMD_NG;

	cm_MemClr(&ltCardStaLTE.alm_inf[CMD_NUM0], sizeof(USHORT)*CMD_MAX_ALM_NUM_HIG_BITRAT);
	cm_MemClr(&ltCardStaLTE.err_inf[CMD_NUM0], sizeof(USHORT)*CMD_MAX_ERR_NUM_HIG_BITRAT);

	cm_MemClr( &ltFuncEvery[CMD_NUM0], sizeof(T_RRH_CARDST_S3G_FUNC_EVERY) * RCD_FB_FHM_NUM );

	/*S3Gは, ALM情報数とERR情報数の上限を5107とする*/
	ldwMaxAlmNum = CMD_MAX_ALM_NUM_HIG_BITRAT;
	ldwMaxErrNum = CMD_MAX_ERR_NUM_HIG_BITRAT;

	/* スロット数を抽出しておく */
    slot_num = f_trp_rec_Wcom_remsc_tbl.remsc[cpr_no_idx].resltinf.slt_num;

	/* 機能部単位情報(種別)を初期化 */
	for( lwFuncCnt = CMD_NUM0; lwFuncCnt < RCD_FB_FHM_NUM; lwFuncCnt++ )
	{
		ltFuncEvery[lwFuncCnt].func_kind = 0xFFFF;
	}

	/* REカード状態報告(Port集約用)管理テーブルにより最新のREカード状態を集約 */
	for( slot_cnt = CMD_NUM0; slot_cnt < slot_num ; slot_cnt++ )
	{
		/* 共有メモリ(3G)装置構成情報(全Slot)の呼び出し 取得用 */
		f_cmn_com_cardstslt_s3g_get( cpr_no_idx + CMD_NUM1, slot_cnt, &ltCardStaSltLTE );
		/* 信号種別が0の場合はDon't Care */
		if(( CMD_NUM0 == ltCardStaSltLTE.signal_kind ))
		{
			/*	無応答(TO)の場合、CPRIリンク異常として扱う	*/
			lwCpriFail++;
			continue;
		}
		/* 1スロット以上が信号種別が0以外の場合は、signal_kindを保持する */
		else
		{
			lwSigKind = ltCardStaSltLTE.signal_kind;
		}

		if (CMD_OK != ltCardStaSltLTE.result)
		{
			/*	NG応答の場合、CPRIリンク異常として扱う	*/
			lwCpriFail++;
			continue;
		}
		/* 1スロット以上がOK応答の場合は、OK応答を保持する */
		else
		{
			lwResult = ltCardStaSltLTE.result;
		}

		/* カード基本監視項目 */
		lwTotalCardSvitem |= ltCardStaSltLTE.card_svitem;
		/* カード拡張監視項目 */
		lwTotalCardExtSvitem |= ltCardStaSltLTE.card_extsvitem;

		/* カード制御項目 */
		lwTotalCardCntitem |= ltCardStaSltLTE.card_cntitem;

		lwFuncNum = ltCardStaSltLTE.func_num;
		/* 機能部単位情報 */
		for( lwFuncCnt = CMD_NUM0; lwFuncCnt < lwFuncNum; lwFuncCnt++ )
		{
			/* 機能部種別  */
			lwFuncKind = ltCardStaSltLTE.func_every[lwFuncCnt].func_kind;
			if (RCD_FB_DUP == lwFuncKind)
			{	/* 0:DUP/BPF/SWITCHは、集約対象外 */
				continue;
			}

			/* Convert func_kind to index of ltFuncEvery table */
			lwFuncId = f_trp_rec_Fc02_t_recrdssc_i_ConvFuncKindToId(lwFuncKind);

			if (lwFuncId < RCD_FB_FHM_NUM)
			{
				f_trp_rec_Fc02_t_recrdssc_i_GetFuncEvery(&ltFuncEvery[lwFuncId], ltCardStaSltLTE.func_every[lwFuncCnt], &lwSmallCarNum[lwFuncId]);
			}
		}
		/* RE# ALM */
		if( ( alm_num + ltCardStaSltLTE.alm_num ) > ldwMaxAlmNum )
		{
			lwCpyAlmNum = ldwMaxAlmNum - alm_num;
		}
		else
		{
			lwCpyAlmNum = ltCardStaSltLTE.alm_num;
		}
		/* 発生アラーム情報取得 */
		cm_MemCpy(	&ltCardStaLTE.alm_inf[alm_num],
					&ltCardStaSltLTE.alm_inf[CMD_NUM0],
					sizeof(USHORT) * lwCpyAlmNum);
		alm_num += lwCpyAlmNum;

		/* RE# ERR */
		if( ( err_num + ltCardStaSltLTE.err_num ) > ldwMaxErrNum )
		{
			lwCpyErrNum = ldwMaxErrNum - err_num;
		}
		else
		{
			lwCpyErrNum = ltCardStaSltLTE.err_num;
		}
		/* エラーコード取得 */
		cm_MemCpy(	&ltCardStaLTE.err_inf[err_num],
					&ltCardStaSltLTE.err_inf[CMD_NUM0],
					sizeof(USHORT) * lwCpyErrNum);
		err_num += lwCpyErrNum;
	}

	/* 機能部故障状態 */ /* TBD spec unclear Q&A*/
	for( lwFuncCnt = CMD_NUM0; lwFuncCnt < RCD_FB_FHM_NUM; lwFuncCnt++ )
	{
		if( 0xFFFF == ltFuncEvery[lwFuncCnt].func_kind )
		{
			continue;
		}

		/* 配下REがSYS-ALMかHigh-ALMかCPRIリンク異常のスロットがある場合 */
		if(( CMD_NUM0 != ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_SYS )) ||
		   ( CMD_NUM0 != ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_HIGH )) ||
		   ( CMD_NUM0 != lwCpriFail ))
		{
			/* 配下REの全スロットがSYS-ALMかHigh-ALMかCPRIリンク異常(normalと未実装がない)場合 */
			if(( CMD_NUM0 == ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_NOTINS ))&&
				( CMD_NUM0 == ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_NORMAL )))
			{
				/* ＜SYS-ALM＞ */
				ltFuncEvery[lwFuncCnt].func_failure = RCD_FB_ALM_SYS;
			}
			/* 配下REの一部スロットがSYS-ALMかHigh-ALMかCPRIリンク異常(normalと未実装がある)場合 */
			else
			{
				/* ＜HIGH-ALM＞ */
				ltFuncEvery[lwFuncCnt].func_failure = RCD_FB_ALM_HIGH;
			}
		}
		/* 配下REの全スロットがnormalか未実装の場合 */
		else
		{
			/* 配下REの1スロット以上がNormal */
			if( CMD_NUM0 != ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_NORMAL ))
			{
				/* ＜normal＞ */
				ltFuncEvery[lwFuncCnt].func_failure = RCD_FB_ALM_NORMAL;
			}
			/* 配下REの全スロットが未実装 */
			else
			{
				/* ＜未実装＞ */
				ltFuncEvery[lwFuncCnt].func_failure = RCD_FB_ALM_NOTINS;
			}
		}
	}

	ltCardStaLTE.signal_kind    = lwSigKind;	/*	全スロットが無応答の場合は0、それ以外は正常値(0x5721)	*/
	ltCardStaLTE.result         = lwResult;		/*	全スロットが無応答もしくはOK以外の場合はNG、それ以外はOK	*/
	ltCardStaLTE.card_svitem    = lwTotalCardSvitem;
	ltCardStaLTE.card_extsvitem = lwTotalCardExtSvitem;
	ltCardStaLTE.card_cntitem   = lwTotalCardCntitem;

	for( lwFuncCnt = CMD_NUM0; lwFuncCnt < RCD_FB_FHM_NUM; lwFuncCnt++ )
	{
	/* キャリア番号で昇順にSortする */
		f_trp_rec_Fc02_t_recrdssc_i_ushortsort_asc(
			ltFuncEvery[lwFuncCnt].car_num,
			sizeof(T_RRH_CARDST_S3G_CAR_EVERY)/sizeof(USHORT),
			(USHORT*)ltFuncEvery[lwFuncCnt].car_every
		);
	/* 対象キャリア数は配下REの中で最小のものを報告 */
		ltFuncEvery[lwFuncCnt].car_num = lwSmallCarNum[lwFuncCnt];
	}

	lwFuncNum = CMD_NUM0;
	for( lwFuncCnt = CMD_NUM0; lwFuncCnt < RCD_FB_FHM_NUM; lwFuncCnt++ )
	{
		if(ltFuncEvery[lwFuncCnt].func_kind < RCD_FB_NUM)
		{
			cm_MemCpy(	&ltCardStaLTE.func_every[lwFuncNum],
						&ltFuncEvery[lwFuncCnt],
						sizeof(T_RRH_CARDST_S3G_FUNC_EVERY));
			lwFuncNum ++;
		}
	}
	ltCardStaLTE.func_num       = lwFuncNum;

	ltCardStaLTE.alm_num        = alm_num;

	ltCardStaLTE.err_num        = err_num;

	/* 共有メモリ(3G)装置構成情報(集約結果)の呼び出し 取得用 */
	f_cmn_com_cardst_s3g_set( cpr_no_idx + CMD_NUM1, &ltCardStaLTE );

    return;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Convert function kind to index
 *  @note
 *  @param  awFuncKind [in] function kind
 *  @return Index of function kind
 *  @date   2016/10/05 KCN)Fukushima create.
*/
/********************************************************************************************************************/

USHORT	f_trp_rec_Fc02_t_recrdssc_i_ConvFuncKindToId( USHORT awFuncKind )
{
	USHORT		lwFuncKind[RCD_FB_FHM_NUM] = { 	RCD_FB_DUP,
												RCD_FB_LNA,
												RCD_FB_T_PA,
												RCD_FB_TRX,
												RCD_FB_TRX_INF,
												RCD_FB_OA_RA_INF,
												RCD_FB_TILT_INF,
												RCD_FB_PORT_INF,
												RCD_FB_MT_INF,
												RCD_FB_3GRF_INF,
												RCD_FB_EX_OA_RA_INF,
												RCD_FB_CLK_INF
											};
	USHORT		lwIndex;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif
	for( lwIndex = CMD_NUM0; lwIndex < RCD_FB_FHM_NUM; lwIndex++ )
	{
		if( lwFuncKind[lwIndex] == awFuncKind )
		{
			break;
		}
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return lwIndex;
}
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  compare 配下REの機能部単位情報, get 機能部単位情報
 *  @note
 *  @param  atTotalFuncEvery_p [out] 機能部単位情報
 *  @param  atFuncEvery [in] 配下REの機能部単位情報
 *  @return VOID
 *  @date   2016/10/05 KCN)Fukushima create.
*/
/********************************************************************************************************************/

VOID	f_trp_rec_Fc02_t_recrdssc_i_GetFuncEvery( 	T_RRH_CARDST_S3G_FUNC_EVERY* atTotalFuncEvery_p,
													T_RRH_CARDST_S3G_FUNC_EVERY atFuncEvery,
													USHORT *awSmallCarNum )
{
	USHORT		lwCarNum;
	USHORT		lwCarCnt1;
	USHORT		lwCarNo;
	USHORT		lwBrNum;
	USHORT		lwCarCnt2;
	USHORT		lwBrCnt1;
	USHORT		lwBrNo;
	USHORT		lwBrCnt2;


#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif
	/* 対象キャリア数 */
	if (0xFFFF == atTotalFuncEvery_p->func_kind)
	{
		*awSmallCarNum = atFuncEvery.car_num;
	}
	else
	{
		if (*awSmallCarNum > atFuncEvery.car_num)
		{
			*awSmallCarNum = atFuncEvery.car_num;
		}
	}

	/* 機能部種別 */
	atTotalFuncEvery_p->func_kind = atFuncEvery.func_kind;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf(" [%d]%s func_kind: %d, svItem:0x%x\n ", __LINE__, __FUNCTION__, atFuncEvery.func_kind, atFuncEvery.func_svitem);
#endif
	/* 機能部故障状態 */
	switch( atFuncEvery.func_failure )
	{
		/* 未実装 */
		case RCD_FB_ALM_NOTINS:
			atTotalFuncEvery_p->func_failure |= RCD_FB_ALM_FLG_NOTINS;
			break;
		/* Normal */
		case RCD_FB_ALM_NORMAL:
			atTotalFuncEvery_p->func_failure |= RCD_FB_ALM_FLG_NORMAL;
			break;
		/* High-ALM */
		case RCD_FB_ALM_HIGH:
			atTotalFuncEvery_p->func_failure |= RCD_FB_ALM_FLG_HIGH;
			break;
		/* SYS-ALM */
		case RCD_FB_ALM_SYS:
			atTotalFuncEvery_p->func_failure |= RCD_FB_ALM_FLG_SYS;
			break;
		/* SYS-ALM */
		default:
			break;
	}

	/* 機能部基本監視項目(新) */
	atTotalFuncEvery_p->func_svitem |= atFuncEvery.func_svitem;
	/* 機能部拡張監視項目 */
	atTotalFuncEvery_p->func_extsvitem |= atFuncEvery.func_extsvitem;
	
	/* 対象キャリア数n */
	lwCarNum = atFuncEvery.car_num;
	/********************/
	/* キャリア単位情報 */
	/********************/
	for( lwCarCnt1 = CMD_NUM0; lwCarCnt1 < lwCarNum; lwCarCnt1++ )
	{
		/* キャリア番号 */
		lwCarNo = atFuncEvery.car_every[lwCarCnt1].car_no;
		/* 対象ブランチ数m */
		lwBrNum = atFuncEvery.car_every[lwCarCnt1].br_num;

		/* loop all carrier of atTotalFuncEvery_p */
		/* find キャリア番号 */
		for( lwCarCnt2 = CMD_NUM0; lwCarCnt2 < atTotalFuncEvery_p->car_num; lwCarCnt2++ )
		{
			if( lwCarNo == atTotalFuncEvery_p->car_every[lwCarCnt2].car_no )
			{	/* システム帯域幅 : 配下REから報告された中で最小のもの */
				if (atTotalFuncEvery_p->car_every[lwCarCnt2].band_width > atFuncEvery.car_every[lwCarCnt1].band_width)
				{
					atTotalFuncEvery_p->car_every[lwCarCnt2].band_width = atFuncEvery.car_every[lwCarCnt1].band_width;
				}
				break;
			}
			/* if キャリア番号 not match -> continue */
		}
		/* if キャリア番号 not exist */
		if( lwCarCnt2 == atTotalFuncEvery_p->car_num )
		{
			/* Add キャリア番号 to atTotalFuncEvery_p */
			atTotalFuncEvery_p->car_every[lwCarCnt2].car_no = lwCarNo;
			/* システム帯域幅 */
			atTotalFuncEvery_p->car_every[lwCarCnt2].band_width = atFuncEvery.car_every[lwCarCnt1].band_width;
			/* 対象キャリア数n */
			atTotalFuncEvery_p->car_num++;
		}

		/* 機能部基本監視項目(新) */
		atTotalFuncEvery_p->car_every[lwCarCnt2].func_svitem |=
						atFuncEvery.car_every[lwCarCnt1].func_svitem;
		/* 機能部拡張監視項目 */
		atTotalFuncEvery_p->car_every[lwCarCnt2].func_extsvitem |=
							atFuncEvery.car_every[lwCarCnt1].func_extsvitem;

		/********************/
		/* ブランチ単位情報 */
		/********************/
		for( lwBrCnt1 = CMD_NUM0; lwBrCnt1 < lwBrNum; lwBrCnt1++ )
		{
			/* ブランチ番号 */
			lwBrNo = atFuncEvery.car_every[lwCarCnt1].br_every[lwBrCnt1].br_no;
			/* loop all branch of atTotalFuncEvery_p->car_every[lwCarCnt2] */
			/* find ブランチ番号 */
			for( lwBrCnt2 = CMD_NUM0; lwBrCnt2 < atTotalFuncEvery_p->car_every[lwCarCnt2].br_num; lwBrCnt2++ )
			{
				if( lwBrNo == atTotalFuncEvery_p->car_every[lwCarCnt2].br_every[lwBrCnt2].br_no )
				{
					break;
				}
				/* if ブランチ番号 not match -> continue */
			}
			/* if ブランチ番号 not exist */
			if( lwBrCnt2 == atTotalFuncEvery_p->car_every[lwCarCnt2].br_num )
			{
				/* Add キャリア番号 to atTotalFuncEvery_p->car_every[lwCarCnt2] */
				atTotalFuncEvery_p->car_every[lwCarCnt2].br_every[lwBrCnt2].br_no = lwBrNo;
				atTotalFuncEvery_p->car_every[lwCarCnt2].br_num++;
			}
			/* 機能部基本監視項目(新) */
			atTotalFuncEvery_p->car_every[lwCarCnt2].br_every[lwBrCnt2].func_svitem |=
							atFuncEvery.car_every[lwCarCnt1].br_every[lwBrCnt1].func_svitem;
			/* 機能部拡張監視項目 */
			atTotalFuncEvery_p->car_every[lwCarCnt2].br_every[lwBrCnt2].func_extsvitem |=
								atFuncEvery.car_every[lwCarCnt1].br_every[lwBrCnt1].func_extsvitem;
		}
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
}

/****************************************************************************/
/*!
 * @brief  昇順ソート関数(unsigned short用)
 * @note   運用中動作パターンに応じた処理を行う
 * @param  elm_num [in] ソート対象要素数
 * @param  elm_siz [in] ソート対象要素数
 * @param  elm     [in] ソート対象要素配列アドレス
 * @return None
 * @date   2016/11/17 KCN)hfuku
 */
/****************************************************************************/
VOID f_trp_rec_Fc02_t_recrdssc_i_ushortsort_asc(                             /* なし:R */
    USHORT elm_num,                                   /*  ソート対象要素数  */
    USHORT elm_siz,                                   /*  ソート対象要素数  */
    USHORT *elm                             /*  ソート対象要素配列アドレス  */
) 
{
    USHORT* elm_wk;                                 /* 要素入れ替え用ワーク */
    USHORT  i;                                            /* ループカウンタ */
    USHORT  j;                                            /* ループカウンタ */

	if ((elm_num == CMD_NUM0) || (elm_siz == CMD_NUM0)) {
        return;
    }

    /****************/
    /* バッファ取得 */
    /****************/
    elm_wk = malloc(                         /* 終了コード:R */
        elm_siz * sizeof(USHORT)                       /* バッファサイズ:I */
    );
	cm_MemClr(elm_wk, elm_siz * sizeof(USHORT));


    for(i = CMD_NUM0; i < (USHORT)(elm_num-CMD_NUM1); i++) {
        for(j = elm_num-CMD_NUM1; j > i; j--) {
            if(elm[j*elm_siz] < elm[(j-CMD_NUM1)*elm_siz]){
                /* elm_wk   = elm[j];   */
                cm_MemCpy(
                    elm_wk, 
                    &elm[j*elm_siz],
                    elm_siz * sizeof(USHORT)
                );
                /* elm[j]   = elm[j-1]; */
                cm_MemCpy(
                    &elm[j*elm_siz],
                    &elm[(j-CMD_NUM1)*elm_siz],
                    elm_siz * sizeof(USHORT)
                );
                /* elm[j-1] = elm_wk;   */
                cm_MemCpy(
                    &elm[(j-CMD_NUM1)*elm_siz],
                    elm_wk,
                    elm_siz * sizeof(USHORT)
                );
            }
        }
    }
    
    /****************/
    /* バッファ解放 */
    /****************/
    free(                                             /* なし:R */
        elm_wk                                        /* バッファアドレス:I */
    );
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
