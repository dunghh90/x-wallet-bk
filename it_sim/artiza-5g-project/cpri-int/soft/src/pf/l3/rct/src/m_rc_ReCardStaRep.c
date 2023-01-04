/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReCardStaRep.c
 *  @brief  RE card status report process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2008/12/08 FFCS)Linlj (M-S3G-eNBSYS-00016) ☆ST_RE☆RREに対し「閉塞→閉塞解除」を実施後、
 *             TRX設定をしたところ、3283:カード状態監視開始6のメール通知内の「カード制御項目」及び「制御コード」
 *              の値が期待値("0"または"65535")以外になっている。
 *  @date   2009/04/08 FFCS)Linlj (M-S3G-eNBPF-01676) RE card status report of 3G report card error(0x05A0), but no error code
 *  @date   2009/04/12 FFCS)Linlj Modify for S3G PRC CR-xxx-xxx (CPRI spec V1.07)
 *  @date   2009/06/08 FJT)Kabasima (M-S3G-eNBPF-02041) REカード状態報告(S3G)における機能部#1対象キャリア数n設定
 *  @date   2009/08/08 FFCS)Wangjuan modify for
 *							  CR-00058-005(CPRI spec V1.0.9) and CR-00054-005(CPRI spec V1.0.8)
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2010/01/06 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2010/02/19 QNET)Kabasima M-S3G-eNBSYS-01090:☆ST_ENB☆TS232項：機能部のUSE bit ON条件について
 *  @date   2011/09/17 FJT)Tokunaga modify for M-S3G-eNBSYS-02497 TS099項対応
 *  @date   2011/10/10 FFCS)Linlj  modify for 省電力対応
 *  @date   2011/10/21 FJT)Tokunaga M-S3G-eNBPF-04071対応
 *  @date   2011/10/23 FJT)Tokunaga M-S3G-eNBPF-04075対応
 *  @date   2011/10/23 COMAS)Uchida M-S3G-eNBPF-04074対応
 *  @date   2011/11/22 FJT)Tokunaga M-S3G-eNBSYS-02539対応
 *  @date   2012/03/15 ALP)Yamamoto 12A-01-006/M-S3G-eNBSYS-02601：SLC-RREにおけるALM/ERRコード処理変更について
 *	@date   2011/06/28 FFCS)Xut modify for CeNB-F-084-014
 *  @date   2012/06/28 FFCS)Xut modify for CeNB-F-084-019
 *  @date   2012/07/11 FFCS)Xut modify for CeNB-F-083-028
 *  @date   2012/08/24 FFCS)Zhangfeng modify for 12B-01-010
 *  @date   2012/10/16 FFCS)Zhangfeng modify for M-S3G-eNBSYS-02684
 *  @date   2012/11/06 FFCS)Xut modify for M-S3G-eNBSYS-04300
 *  @date   2013/11/05 FFCS)Xut modify for ZYNQ.
 *  @date   2015/04/16 FFCS)zhangxq modify for TDD-SRE.
 *  @date   2015/04/16 M&C)Huan.dh modify m_rc_ReCardStaRep with NR, LTE+NR connected operation mode
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2017
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"
#include "l3_com_inc.h"
#include "m_rc_ftyp.h"
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#include "f_rrh_reg_dis.h"
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    RE card status report process for 3G
 *  @note     RE card status report process for 3G
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)linlj create
 *  @date   2008/12/08 FFCS)Linlj (M-S3G-eNBSYS-00016) ☆ST_RE☆RREに対し「閉塞→閉塞解除」を実施後、
 *             TRX設定をしたところ、3283:カード状態監視開始6のメール通知内の「カード制御項目」及び「制御コード」
 *              の値が期待値("0"または"65535")以外になっている。
 *  @date   2009/04/08 FFCS)Linlj (M-S3G-eNBPF-01676) RE card status report of 3G report card error(0x05A0), but no error code
 *  @date   2010/01/06 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2011/10/10 FFCS)Linlj  modify for 省電力対応
 *  @date	2011/10/24 FJT)Tokunaga modify for M-S3G-eNBPF-04078
 *  @date	2011/10/31 FJT)Tokunaga modify for M-S3G-eNBPF-04081
 *  @date	2012/05/01 FJT)Tokunaga modify for M-S3G-eNBPF-04116;REが検出するERRの通知先システムについて
 *  @date   2012/07/11 FFCS)Xut modify for CeNB-F-083-028
 *  @date	2012/08/24 FFCS)Zhangfeng modify for 12B-01-010
 *  @date   2012/10/16 FFCS)Zhangfeng modify for M-S3G-eNBSYS-02684
 */
/********************************************************************************************************************/

extern RCT_CARDFB_DEVKIND_T	rcr_cardFb_tdd_sre;
extern USHORT gw_3g_err_code[CMD_MAX_ERR_NUM];
extern USHORT gw_3g_alm_code[CMD_MAX_ALM_NUM];
extern USHORT gw_lte_err_code[CMD_MAX_ERR_NUM_HIG_BITRAT];
extern USHORT gw_lte_alm_code[CMD_MAX_ALM_NUM_HIG_BITRAT];

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    RE card status report process
 *  @note     RE card status report proces
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   None
 *  @retval   -
 *  @date   2015/08/03 FPT)tan Create
 *  @date   2021/01/14 M&C)Huan.dh Connected operation mode handle 
 */
/********************************************************************************************************************/
VOID m_rc_ReCardStaRep(CMT_TSKIF_CPRIRCV_CARDSTSREQ *parm_p)
{
	UINT   sigchk_rslt;						/* result of checking signal */
	USHORT sigsys_rslt;					 /* system after checking signal */

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif

	if( NULL == parm_p )
	{
		return;
	}
	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_cardstsreq.signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt,
							"validate signal NG");
	}

	if (CMD_SYS_3G == sigsys_rslt)
	{
		m_rc_ReCardStaRep_3G(parm_p);
	}
	else
	{
		//NR independent mode or LTE + NR mode
		if (cmw_connectoperatemode > CMD_OP_LTE)
		{
			m_rc_ReCardStaRep_S3G(parm_p);
		}
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return;
}
/* @} */



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    RE card status report process for 3G
 *  @note     RE card status report process for 3G
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   None
 *  @retval   -
 *  @date	  2015/08/03 FPT)tan Modify
 *  @date	  2015/10/22 FPT)tan fix bug BUG_122_IT2_20-7-1
 *  @date     2015/10/22 TDI)satou FHM技説102-005, FHM技説-設計変更-024
 *  @date     2015/10/27 FPT)Yen fix bug IT3 No36
 *  @date     2015/10/29 TDI)satou FHM技説-QA-111
 *	@date	  2015/11/09 FPT)Hai fix その他未実装-014
 *  @date	  2015/11/11 FPT)Hai fix BUG_171_IT2_20_7_03
 *  @date     2015/11/18 FPT)Yen fix bug IT3 No90
 *  @date     2016/03/24 tdips)enoki 仕様変更 拡張監視項目の集約条件変更(OR条件⇒AND条件)
 *  @date     2016/04/17 tdips)enoki パラメータ範囲外または全配下REからNG応答受信によるRECへのNG応答送信時のタスク間ヘッダのデータレングス修正
 */
/********************************************************************************************************************/

VOID m_rc_ReCardStaRep_3G(CMT_TSKIF_CPRIRCV_CARDSTSREQ *parm_p)
{
	CMT_TSKIF_CPRISND_CARDSTSRES *rsp_p;		/* タスク間フレームポインタ		*/
	// T_RRH_SYSTEM_TIME		rtc_data_p;			/* RTC読出しデータ				*/

	// USHORT datalength;							/* データ長						*/
	// USHORT set_data_cnt       = CMD_NUM0;		/* 設定データカウンタ			*/
	// USHORT svitem;								/* FHMカード基本監視項目		*/
	// USHORT alm_num;								/* 発生ALM数					*/
	// USHORT err_num;								/* 発生ERR数					*/

	USHORT setdata [CMD_NUM1			+		/* ALM情報数					*/
					CMD_CRDALM_INFNUMMAX+		/* ALM情報						*/
					CMD_NUM1			+		/* ERR情報数					*/
					CMD_CRDERR_INFNUMMAX];		/* ERR情報						*/

	//USHORT err_code[CMD_MAX_ERR_NUM];			/* ERRコード格納用ワークエリア	*/
	//USHORT alm_code[CMD_MAX_ALM_NUM];			/* 発生ALM情報格納用ワークエリア*/


	UINT   bufget_rslt;								/* バッファ獲得結果				*/
	UINT   ressnd_rslt __attribute__((unused));		/* 応答送信結果					*/

	// USHORT lwReCnt;
	// USHORT lwTotalCardSvitem = CMD_NUM0;		/* カード基本監視頁E岼			*/
	// USHORT lwTotalCardExtSvitem = CMD_NUM0;		/* カード拡張監視頁E岼			*/
	// USHORT lwTotalCardCntitem = CMD_NUM0;		/* カード制御項目 */
	// USHORT lwTotalCntCode = CMD_NUM0;			/* 制御コード					*/
	// USHORT lwTotalCarInf = CMD_NUM0;			/* キャリア情報					*/
	// USHORT lwCpyAlmNum;
	// USHORT lwCpyErrNum;

	USHORT result = CMD_NML ;
	// T_RRH_LAYER3				ltLayer3Sta3G;
	// T_RRH_CARDST_3G				ltCardSta3G;	/* (3G)REカード状態 			*/
	// T_RRH_FHM_STATE				ltFhmSta;
	// USHORT						lwDataExist = CMD_OFF;
	// USHORT                      lwAllErrRsp = CMD_ON;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif

	/**********/
	/* 初期化 */
	/**********/

	/* 可変長設定用データ	*/
	// cm_MemClr(gw_3g_err_code, sizeof(gw_3g_err_code));
	// cm_MemClr(gw_3g_alm_code, sizeof(gw_3g_alm_code));

	/* 可変長設定用データ	*/
	cm_MemClr(setdata, sizeof(setdata));

	/* ポインタ初期化 */
	rsp_p = NULL;

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 取りうる最大のデータ長でバッファを獲得する */
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_CARDSTSRES)	+/* キャリア情報まで	*/
								sizeof(setdata)						,
								CMD_NUM1,
								(VOID **)&rsp_p	);
	/********************/
	/* バッファ獲得失敗 */
	/********************/
	if(bufget_rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					bufget_rslt,
					"m_rc_ReCardStaRep_3G cm_L2BfGet NG");

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}

	/********************/
	/* CPRI信号種別設定 */
	/********************/
	rsp_p->cpridat_cardstsres.signal_kind = CMD_CPRID_CARDSTSRES;

	/******************************/
	/* CPRI信号応答結果にOKを設定 */
	/******************************/
	rsp_p->cpridat_cardstsres.result = result;

	/****************/
	/* スロット番号 */
	/****************/
	rsp_p->cpridat_cardstsres.cardsts_inf.slot_no
							= parm_p->cpridat_cardstsreq.slot_no;

	/****************/
	/* 条件チェック */
	/****************/

	/* パラメータ範囲異常(スロット番号が1以外)の場合 */
	//if( CMD_NUM1 != parm_p->cpridat_cardstsreq.slot_no )
	//{
	/* CPRI信号応答結果 2:報告NG */
	rsp_p->cpridat_cardstsres.result = CMD_RPT_NG;
	/* 異常ルートは可変長の長さを表すパラメータに0を設定	*/
	cm_MemClr(&(rsp_p->cpridat_cardstsres.cardsts_inf.slot_no) + CMD_NUM1,				/* スロット番号は正しく返す必要がある */
		(sizeof(CARDSTS_INF) - sizeof(rsp_p->cpridat_cardstsres.cardsts_inf.slot_no)) +	/* カード状態詳細 - スロット番号	*/
		(sizeof(USHORT) * CMD_NUM3));													/* 機能部数+ALM情報数+ERR情報数		*/

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)	+		/* CPRI信号送信情報部		*/
								sizeof(CMT_CPRIF_CARDSTSRES) +						/* 信号種別～カード拡張制御項目	*/
								(sizeof(USHORT) * CMD_NUM2);							/* ALM情報数+ERR情報数		*/

	/************/
	/* 応答送信 */
	/************/
	ressnd_rslt = m_cm_L3MsgSend_REC(rsp_p);


#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return ;
}

// 	/* RTC読み出しデータ初期化 */
// 	cm_MemClr(&rtc_data_p, sizeof(T_RRH_SYSTEM_TIME));

// 	/****************************************/
// 	/* 「API関数: 時刻取得要求取得」コール  */
// 	/****************************************/
// 	cm_RtcGet(&rtc_data_p);

// 	/**********/
// 	/* 年設定 */
// 	/**********/
// 	rsp_p->cpridat_cardstsres.cardsts_inf.year = rtc_data_p.year;

// 	/************/
// 	/* 月日設定 */
// 	/************/
// 	rsp_p->cpridat_cardstsres.cardsts_inf.month_day
// 		= (USHORT)((rtc_data_p.month << CMD_BIT8) | rtc_data_p.day);

// 	/**********/
// 	/* 時設定 */
// 	/**********/
// 	rsp_p->cpridat_cardstsres.cardsts_inf.hour = rtc_data_p.hour;

// 	/************/
// 	/* 分秒設定 */
// 	/************/
// 	rsp_p->cpridat_cardstsres.cardsts_inf.minsec
// 		= (USHORT)((rtc_data_p.min << CMD_BIT8) | rtc_data_p.sec);

// 	/* 共用メモリFHM状態の呼び出し 取得用 */
// 	f_cmn_com_fhm_state_get( &ltFhmSta );
// 	svitem = ltFhmSta.sys[CMD_SYS_3G].FhmBase;

// 	/* REカード状態報告(LTE)は、S3GキャリアがONの時USE報告 */
// 	if (0 != (svitem & CMD_CARDSV_USE_BIT))
// 	{
// 	    if (0 == (cmw_tra_sv_tbl.carr_onoff & CMD_CARDSV_3G_USE_BIT))
// 	    {
// 	        svitem &= ~CMD_CARDSV_USE_BIT;
// 	    }
// 	}

// 	/* ALM FMH */
// 	lwCpyAlmNum = ltFhmSta.sys[CMD_SYS_3G].alm_num;
// 	alm_num = ltFhmSta.sys[CMD_SYS_3G].alm_num;
// 	/* エラーコード取得 */
// 	cm_MemCpy(	&gw_3g_alm_code[CMD_NUM0],
// 				&ltFhmSta.sys[CMD_SYS_3G].alm_inf[CMD_NUM0],
// 				sizeof(USHORT) * lwCpyAlmNum);

// 	/* ERR FHM */
// 	lwCpyErrNum = ltFhmSta.sys[CMD_SYS_3G].err_num;
// 	err_num = ltFhmSta.sys[CMD_SYS_3G].err_num;
// 	/* エラーコード取得 */
// 	cm_MemCpy(	&gw_3g_err_code[CMD_NUM0],
// 				&ltFhmSta.sys[CMD_SYS_3G].err_inf[CMD_NUM0],
// 				sizeof(USHORT) * lwCpyErrNum);

// 	lwTotalCardSvitem = 0;
// 	lwTotalCardExtSvitem = (RCD_TRA_WARMUP | RCD_TRA_AMPDWN_3G);

// 	/* 共用メモリLayer3の呼び出し 取得用 */
// 	f_cmn_com_layer3_get( CMD_SYS_3G, &ltLayer3Sta3G );

// 	/* REカード状態報告管理テーブルにより最新のREカード状態を構築 */
// 	for( lwReCnt = CMD_NUM0; lwReCnt < D_RRH_CPRINO_RE_MAX ; lwReCnt++ )
// 	{
// 		/* Layer3状態が「②RE起動中状態」、または「④運用中状態」の配下REだけが対象処理 */
// 		if(( D_L3_STA_2 != ltLayer3Sta3G.layer3_re[lwReCnt] ) &&
// 		   ( D_L3_STA_4 != ltLayer3Sta3G.layer3_re[lwReCnt] ))
// 		{
// 			continue;
// 		}
// 		/* 共有メモリ(3G)装置構成情報の呼び出し 取得用 */
// 		f_cmn_com_cardst_3g_get( lwReCnt + CMD_NUM1, &ltCardSta3G );
// 		/* 信号種別が0の場合はDon't Care */
// 		if(( CMD_NUM0 == ltCardSta3G.signal_kind ))
// 		{
// 			continue;
// 		}
// 		lwDataExist = CMD_ON;

// 		if (CMD_OK != ltCardSta3G.result)
// 		{
// 		   continue;
// 		}
// 		lwAllErrRsp = CMD_OFF;

// 		/* カード基本監視項目 */
// 		m_rc_GetSvitem( &lwTotalCardSvitem, ltCardSta3G.card_svitem );
// 		/* カード拡張監視項目 */
// 		m_rc_GetExtSvitem_3G( &lwTotalCardExtSvitem, ltCardSta3G.card_extsvitem );
// 		/* カード制御項目 */
// 		lwTotalCardCntitem |= ltCardSta3G.card_cntitem;
// 		/* 制御コード */
// 		lwTotalCntCode     |= ltCardSta3G.cnt_code;
// 		/* キャリア情報 */
// 		lwTotalCarInf |=  ltCardSta3G.car_inf;
// 		/* RE# ALM */
// 		if( ( alm_num + ltCardSta3G.alm_num ) > CMD_MAX_ALM_NUM )
// 		{
// 			lwCpyAlmNum = CMD_MAX_ALM_NUM - alm_num;
// 		}
// 		else
// 		{
// 			lwCpyAlmNum = ltCardSta3G.alm_num;
// 		}
// 		/* 発生アラーム情報取得 */
// 		cm_MemCpy(	&gw_3g_alm_code[alm_num],
// 					&ltCardSta3G.alm_inf[CMD_NUM0],
// 					sizeof(USHORT) * lwCpyAlmNum);
// 		alm_num += lwCpyAlmNum;

// 		/* RE# ERR */
// 		if( ( err_num + ltCardSta3G.err_num ) > CMD_MAX_ERR_NUM )
// 		{
// 			lwCpyErrNum = CMD_MAX_ERR_NUM - err_num;
// 		}
// 		else
// 		{
// 			lwCpyErrNum = ltCardSta3G.err_num;
// 		}
// 		/* エラーコード取得 */
// 		cm_MemCpy(	&gw_3g_err_code[err_num],
// 					&ltCardSta3G.err_inf[CMD_NUM0],
// 					sizeof(USHORT) * lwCpyErrNum);
// 		err_num += lwCpyErrNum;
// 	}

// 	/* 応答があり、かつ、配下REからの最新応答が全配下REにてNG応答を受信 */
// 	if( (CMD_ON == lwDataExist) && (CMD_ON == lwAllErrRsp) )
// 	{
// 		/* CPRI信号応答結果 2:報告NG */
// 		rsp_p->cpridat_cardstsres.result = CMD_RPT_NG;
// 		/* 異常ルートは可変長の長さを表すパラメータに0を設定	*/
// 		cm_MemClr(&(rsp_p->cpridat_cardstsres.cardsts_inf.slot_no) + CMD_NUM1,				/* スロット番号は正しく返す必要がある */
// 			(sizeof(CARDSTS_INF) - sizeof(rsp_p->cpridat_cardstsres.cardsts_inf.slot_no)) +	/* カード状態詳細 - スロット番号	*/
// 			(sizeof(USHORT) * CMD_NUM3));													/* 機能部数+ALM情報数+ERR情報数		*/

// 		/**********************/
// 		/* タスク間ヘッダ作成 */
// 		/**********************/
// 		rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)	+		/* CPRI信号送信情報部		*/
// 									sizeof(CMT_CPRIF_CARDSTSRES) +						/* 信号種別～カード拡張制御項目	*/
// 									(sizeof(USHORT) * CMD_NUM2);							/* ALM情報数+ERR情報数		*/

// 		/************/
// 		/* 応答送信 */
// 		/************/
// 		ressnd_rslt = m_cm_L3MsgSend_REC(rsp_p);


// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
// #endif

// 		return ;
//	}

// 	/* カード基本監視項目 */
// 	/* 使用中(USE) : FHMにおいて、3GのTRX設定がなされてリソースが使用中の場合に「1:使用中」を報告します。*/ /* TBD Q&A */
// 	/* 故障(ALM) : FHMがALM、若しくはすべての配下REがALMの場合に「1:故障発生」を報告します。*/
// 	/* エラー(ERR) :FHMがERR、若しくは一部の配下REがALMの場合に「1:ERR発生中」を報告します。*/
// 	/* 運用(ACT) : FHMの状態を報告します。*/
// 	lwTotalCardSvitem |= ( svitem & (CMD_CARDSV_USE_BIT | CMD_CARDSV_ALM_BIT | CMD_CARDSV_ERR_BIT | CMD_CARDSV_ACT_BIT) ) |
// 							CMD_CARDSV_IMP_BIT ;
// 	/* 運用(ACT) : FHMの状態を報告します。*/
// 	if( svitem & CMD_CARDSV_ACT_BIT )
// 	{
// 		lwTotalCardSvitem |= CMD_CARDSV_ACT_BIT;
// 	}
// 	else
// 	{
// 		lwTotalCardSvitem &= ~CMD_CARDSV_ACT_BIT;
// 	}
// 	/* clear bit D15, D14, D13, D12, D11, D6, D4, D3, D2, D1, D0 */
// 	lwTotalCardSvitem &= 0x07A0;

// 	rsp_p->cpridat_cardstsres.cardsts_inf.card_svitem = lwTotalCardSvitem;
// 	/* REカード拡張監視項目 */
// 	rsp_p->cpridat_cardstsres.cardsts_inf.card_extitem = (CMD_ON == lwDataExist) ? lwTotalCardExtSvitem : 0;
// 	/* カード基本制御項目 */
// 	rsp_p->cpridat_cardstsres.cardsts_inf.card_cntitem = (lwTotalCardCntitem & (CMD_CRDCTBIT_L_RST | CMD_CRDCTBIT_AUTO_RST | CMD_CRDCTBIT_R_RST));
// 	/* 制御コード */
// 	rsp_p->cpridat_cardstsres.cardsts_inf.cnt_code = lwTotalCntCode;
// 	/* 機能部固有制御項目 */
// 	rsp_p->cpridat_cardstsres.cardsts_inf.funcitem = CMD_NUM0;
// 	/* キャリア情報 */
// 	rsp_p->cpridat_cardstsres.cardsts_inf.car_inf = lwTotalCarInf;

// 	/*******************************************/
// 	/* ALM情報数/ALM情報/ERR情報数/ERR情報設定 */
// 	/*******************************************/
// 	/*
// 	 * FHMもしくは配下REのいずれかでALMが通知された場合、ALM情報数1、ALMコード0xFFを報告する。
//      * FHM、またすべての配下REについてALMが発生していない場合、ALM情報数0で報告する。
//      * つまり、ALM情報数は0か1しかない。
// 	 */
// 	if (0 == alm_num)
// 	{
// 	    setdata[set_data_cnt] = 0;
// 	    set_data_cnt++;
// 	}
// 	else
// 	{
// 	    setdata[set_data_cnt] = 1;
// 	    set_data_cnt++;
// 	    setdata[set_data_cnt] = 0xFF;
// 	    set_data_cnt++;
// 	}

// 	setdata[set_data_cnt] = err_num;
// 	set_data_cnt++;

// 	if( CMD_NUM0 != err_num )
// 	{
// 		/* Sort data */
// //	    l3_com_heapsort(gw_3g_err_code, err_num, sizeof(USHORT), l3_com_cmp_ushort, l3_com_swap_ushort);
// 		cm_MemCpy(&setdata[set_data_cnt], gw_3g_err_code, sizeof(USHORT) * err_num);
// 		set_data_cnt += err_num;
// 	}

// 	/* 応答データに可変長データを設定 */
// 	cm_MemCpy(&(rsp_p->cpridat_cardstsres.cardsts_inf.car_inf) + CMD_NUM1,
// 			  setdata,
// 			  sizeof(USHORT) * (set_data_cnt));

// 	/****************/
// 	/* データ長設定 */
// 	/****************/
// 	datalength = (USHORT)(	(sizeof(CMT_CPRISIGSND_INF)
// 						+	sizeof(CMT_CPRIF_CARDSTSRES))	/* 信号種別～キャリア情報	*/
// 						+	(set_data_cnt * CMD_NUM2));		/* ALM情報数+ALM情報	+	*/
// 															/* ERR情報数+ERR情報		*/

// 	/**********************/
// 	/* タスク間ヘッダ作成 */
// 	/**********************/
// 	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + datalength;

// 	/************/
// 	/* 応答送信 */
// 	/************/
// 	ressnd_rslt = m_cm_L3MsgSend_REC(rsp_p);

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
// #endif
// 	return ;
// }

/* @} */

/* @{ */

/********************************************************************************************************************/
/**
 *  @brief    RE card status report process for LTE
 *  @note     RE card status report process for LTE
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   None
 *  @date     2021/01/15 M&C)Huan.dh RE Card status in LTE connected operation mode
 */
/********************************************************************************************************************/
VOID m_rc_ReCardStaRep_LTE(CMT_TSKIF_CPRIRCV_CARDSTSRES *parm_p)
{
	UINT   ressnd_rslt __attribute__((unused));	/* 応答送信結果					*/
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif

if(cmw_filedatsnd_flag !=0)
{
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "card state req Start:flag=%d", cmw_filedatsnd_flag);
}

	//Forward response 
	ressnd_rslt = m_cm_L3MsgSend_REC(parm_p);

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return ;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief     RE card status report process for S3G
 *  @note     RE card status report process for S3G
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   None
 *  @Bug_No   M-RRU-ZSYS-01964
 *  @Bug_No   M-RRU-ZSYS-01981
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2009/04/08 FFCS)Linlj (M-S3G-eNBPF-01676) RE card status report of 3G report card error(0x05A0), but no error code
 *  @date     2009/08/08 FFCS)Wangjuan modify for
 *							  CR-00054-005(CPRI spec V1.0.8)
 *  @date     2011/10/10 FFCS)Linlj  modify for 省電力対応
 *  @date     2011/10/24 FJT)Tokunaga modify for M-S3G-eNBPF-04078
 * 	@date     2012/05/01 FJT)Tokunaga modify for M-S3G-eNBPF-04116;REが検出するERRの通知先システムについて
 *  @date     2012/07/11 FFCS)Xut modify for CeNB-F-083-028
 *  @date     2012/08/24 FFCS)Zhangfeng modify for 12B-01-010
 *  @date     2012/10/16 FFCS)Zhangfeng modify for M-S3G-eNBSYS-02684
 *  @date     2013/11/05 FFCS)Xut modify for ZYNQ.
 *  @date     2015/04/16 FFCS)zhangxq modify for TDD-SRE
 *  @date     2015/08/02 FPT)Yen create
 *  @date     2015/10/22 TDI)satou FHM技説102-005, FHM技説-設計変更-024
 *  @date     2015/10/27 FPT)Yen fix bug IT3 No36
 *	@date	  2015/11/02 FPT)DungLH2 fix request 送信系切替制御
 *  @date     2015/11/06 TDIPS)Lay M-RRU-ZSYS-01964 機能部情報編集処理におけるインデックス変数の代入値の処理を以前の実装に戻す 
 *	@date	  2015/11/09 FPT)Hai fix その他未実装-014
 *	@date	  2015/11/13 TDIPS)sasaki M-RRU-ZSYS-01981 update IT2問処No.175暫定対処 大きいサイズの変数をstaticに
 *	@date	  2015/12/07 TDIPS)sasaki update IT3問処No.129暫定対処(ALM/ERR情報の整列アルゴリズム変更:バブル -> ヒープ)
 *  @date     2015/12/08 FPT)Yen fix bug IT3 No133
 *  @date     2016/03/10 TDI)satou 0:DUP/BPF/SWITCHの機能部は、集約対象外に
 *  @date     2016/03/24 tdips)enoki 仕様変更 拡張監視項目の集約条件変更(OR条件⇒AND条件)
 *  @date     2016/04/08 TDI)satou 仕様変更 機能部基本監視項目の集約条件変更
 *  @date     2016/06/10 FJT)koshida 16A'(FHM技説v1.1.0)対応 機能部基本監視項目の集約条件変更
 *  @date     2017/04/07 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 *  @date     2017/04/13 FJT)koshida RE未使用時の擬似ALM動作変更
 *  @date     2021/02/02 M&C)Huan.dh Abnormality is detected in the functional part state of the RE
 */
/********************************************************************************************************************/
VOID m_rc_ReCardStaRep_S3G(CMT_TSKIF_CPRIRCV_CARDSTSREQ *parm_p)
{
	CMT_TSKIF_CPRISND_CARDSTSRES_S3G *rsp_p;	/* タスク間フレームポインタ		*/
	T_RRH_SYSTEM_TIME		rtc_data_p;			/* RTC読出しデータ				*/

	USHORT datalength;							/* データ長						*/
	USHORT set_data_cnt       = CMD_NUM0;		/* 設定データカウンタ			*/
	USHORT svitem;								/* FHMカード基本監視項目		*/
	USHORT lwTotalCardSvitem = CMD_NUM0;		/* カード基本監視項目			*/
	USHORT lwTotalCardExtSvitem = CMD_NUM0;		/* カード拡張監視項目			*/
	USHORT alm_num;								/* 発生ALM数					*/
	USHORT err_num;								/* 発生ERR数					*/
	USHORT lwTotalCardCntitem = CMD_NUM0;		/* カード制御項目 */

	USHORT* setdata;

	//USHORT err_code[CMD_MAX_ERR_NUM_HIG_BITRAT];			/* ERRコード格納用ワークエリア	*/
	//USHORT alm_code[CMD_MAX_ALM_NUM_HIG_BITRAT];			/* 発生ALM情報格納用ワークエリア*/

	UINT   bufget_rslt;							/* バッファ獲得結果				*/
	UINT   ressnd_rslt __attribute__((unused));	/* 応答送信結果					*/

	USHORT lwCarrCnt;
	USHORT lwReCnt;
	USHORT result = CMD_NML ;
	T_RRH_LAYER3						ltLayer3StaLte;
	static T_RRH_CARDST_S3G				ltCardStaLte;
	T_RRH_FHM_STATE						ltFhmSta;
	static T_RRH_CARDST_S3G_FUNC_EVERY	ltFuncEvery[RCD_FB_FHM_NUM];
	USHORT								lwFuncNum = CMD_NUM0;
	USHORT								lwTotalFuncNum = CMD_NUM0;
	USHORT								lwCpyAlmNum;
	USHORT								lwCpyErrNum;
	USHORT								lwBrCnt;
	USHORT								lwFuncCnt;
	USHORT 								lwFuncKind;
	USHORT								lwFuncId;
	USHORT								lwDataExist = CMD_OFF;
	USHORT                              lwAllErrRsp = CMD_ON;
	UINT				data_size;
	USHORT              data_length;
	UINT								ldwMaxAlmNum = CMD_MAX_ALM_NUM;
	UINT								ldwMaxErrNum = CMD_MAX_ERR_NUM;
	USHORT								lwCurrLength = CMD_NUM0;
	UINT								ldwIndexAlmNum = CMD_NUM0;
	UINT								ldwIndexErrNum = CMD_NUM0;
	USHORT 								set_data_cnt_old = CMD_NUM0;
	USHORT								loopcnt;
	USHORT								loopcnt2;
	USHORT								lwSmallCarNum[RCD_FB_FHM_NUM] = {0};
	USHORT								sysAlmOFF = D_RRH_OFF;
	USHORT								lwCpriFail = 0;		/*	CPRI使用and(CPRIリンク断or応答TOor応答NG)	*/
	T_RRH_CONN_DISC						shmConnDisc;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif
	/**********/
	/* 初期化 */
	/**********/
	if(cmw_filedatsnd_flag !=0)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "card state req Start:flag=%d", cmw_filedatsnd_flag);
	}

	data_size = sizeof(USHORT) *
				(CMD_NUM1			+			/* 機能部数						*/
				((((CMD_NUM3 * CMD_NUM8) + CMD_NUM5) * CMD_NUM5 + CMD_NUM5) * CMD_NUM29) +
											/* 機能部情報					*/
				CMD_NUM1			+		/* ALM情報数					*/
				CMD_CRDALM_INFNUMMAX_HIG+		/* ALM情報						*/
				CMD_NUM1			+		/* ERR情報数					*/
				CMD_CRDERR_INFNUMMAX_HIG);		/* ERR情報						*/

	/* 可変長設定用データ	*/
	cm_MemClr(gw_lte_err_code, sizeof(gw_lte_err_code));
	cm_MemClr(gw_lte_alm_code, sizeof(gw_lte_alm_code));

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s data size: %d\n ", __LINE__, __FUNCTION__, data_size);
#endif
	/* ポインタ初期化 */
	rsp_p = NULL;

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 取りうる最大のデータ長でバッファを獲得する */
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_CARDSTSRES_S3G)	+/* キャリア情報まで	*/
								data_size						,
								CMD_NUM1,
								(VOID **)&rsp_p	);
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s size Msg:%d\n ", __LINE__, __FUNCTION__,sizeof(CMT_TSKIF_CPRISND_CARDSTSRES_S3G)	+data_size);
#endif
	/********************/
	/* バッファ獲得失敗 */
	/********************/
	if(bufget_rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					bufget_rslt,
					"m_rc_ReCardStaRep_S3G cm_L2BfGet NG");

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}

	/********************/
	/* CPRI信号種別設定 */
	/********************/
	rsp_p->cpridat_cardstsres.signal_kind = CMD_CPRID_CARDSTSRES + CMD_NUM1;

	/******************************/
	/* CPRI信号応答結果にOKを設定 */
	/******************************/
	rsp_p->cpridat_cardstsres.result = result;

	/****************/
	/* スロット番号 */
	/****************/
	rsp_p->cpridat_cardstsres.cardsts_inf.slot_no
							= parm_p->cpridat_cardstsreq.slot_no;

	/****************/
	/* 条件チェック */
	/****************/

	/* パラメータ範囲異常(スロット番号が1以外)の場合 */
	if( CMD_NUM1 != parm_p->cpridat_cardstsreq.slot_no )
	{
		/* CPRI信号応答内容設定 */
	        /* CPRI信号応答結果 51:処理NG(RE) */
		rsp_p->cpridat_cardstsres.result = CMD_HDL_NG;
		/* 異常ルートは可変長の長さを表すパラメータに0を設定	*/
		cm_MemClr(&(rsp_p->cpridat_cardstsres.cardsts_inf.slot_no) + CMD_NUM1,	/* スロット番号は正しく返す必要がある */
			(sizeof(CARDSTS_INF_S3G) - sizeof(rsp_p->cpridat_cardstsres.cardsts_inf.slot_no)) +	/* カード状態詳細 - スロット番号	*/
			(sizeof(USHORT) * CMD_NUM3));														/* 機能部数+ALM情報数+ERR情報数		*/

		/**********************/
		/* タスク間ヘッダ作成 */
		/**********************/
		rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)	+		/* CPRI信号送信情報部		*/
									sizeof(CMT_CPRIF_CARDSTSRES_S3G) +	/* 信号種別～カード拡張制御項目	*/
									(sizeof(USHORT) * CMD_NUM3);		/* 機能部数 + ALM情報数+ERR情報数		*/

		/************/
		/* 応答送信 */
		/************/
		ressnd_rslt = m_cm_L3MsgSend_REC(rsp_p);

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return ;
	}

	/**************/
	/* 報告値設定 */
	/**************/
	/* カード名 */
	/*************************************************************************************/
	/* カード名                                                                          */
	/*************************************************************************************/
	for (loopcnt = CMD_NUM0; loopcnt < sizeof(rsp_p->cpridat_cardstsres.cardsts_inf.card_name)/sizeof(USHORT); loopcnt++)
	{
		rsp_p->cpridat_cardstsres.cardsts_inf.card_name[loopcnt*CMD_NUM2] = (UCHAR)((cmw_tra_inv_tbl.cmx_cardname[loopcnt] & RCD_HI_MSK) >> CMD_BIT8);
		rsp_p->cpridat_cardstsres.cardsts_inf.card_name[loopcnt*CMD_NUM2+CMD_NUM1] = (UCHAR)(cmw_tra_inv_tbl.cmx_cardname[loopcnt] & RCD_LW_MSK);
	}
#if 0
//	cm_MemCpy( rsp_p->cpridat_cardstsres.cardsts_inf.card_name,
//					  (const CHAR*)"FHM-3201", (INT)strlen("FHM-3201") );
#endif
	/* RTC読み出しデータ初期化 */
	cm_MemClr(&rtc_data_p, sizeof(T_RRH_SYSTEM_TIME));

	/****************************************/
	/* 「API関数: 時刻取得要求取得」コール  */
	/****************************************/
	cm_RtcGet(&rtc_data_p);

	/**********/
	/* 年設定 */
	/**********/
	rsp_p->cpridat_cardstsres.cardsts_inf.year = rtc_data_p.year;

	/************/
	/* 月日設定 */
	/************/
	rsp_p->cpridat_cardstsres.cardsts_inf.month_day
		= (USHORT)((rtc_data_p.month << CMD_BIT8) | rtc_data_p.day);

	/**********/
	/* 時設定 */
	/**********/
	rsp_p->cpridat_cardstsres.cardsts_inf.hour = rtc_data_p.hour;

	/************/
	/* 分秒設定 */
	/************/
	rsp_p->cpridat_cardstsres.cardsts_inf.minsec
		= (USHORT)((rtc_data_p.min << CMD_BIT8) | rtc_data_p.sec);


	/**************************/
	/* カード基本監視項目設定 */
	/**************************/

	/* 共用メモリFHM状態の呼び出し 取得用 */
	f_cmn_com_fhm_state_get( &ltFhmSta );
	svitem = ltFhmSta.sys[CMD_SYS_S3G].FhmBase;

    /* REカード状態報告(LTE)は、S3GキャリアがONの時USE報告 */
    if (0 != (svitem & CMD_CARDSV_USE_BIT))
    {
        if (0 == (cmw_tra_sv_tbl.carr_onoff & CMD_CARDSV_S3G_USE_BIT))
        {
            svitem &= ~CMD_CARDSV_USE_BIT;
        }
    }

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s FHM svitem:0x%x\n ", __LINE__, __FUNCTION__,svitem);
#endif
	lwTotalCardSvitem = CMD_IVL_VAL;

	/* カード拡張監視項目(AMP部電源断,Warm Up)はAND条件での集約とする */
	/* 初期値はON（該当状態発生）とする */
	/* カード単位　拡張監視項目　初期化 */
	lwTotalCardExtSvitem = (RCD_TRA_WARMUP | RCD_TRA_AMPDWN_S3G);

	cm_MemClr( &ltFuncEvery[CMD_NUM0], sizeof(T_RRH_CARDST_S3G_FUNC_EVERY) * RCD_FB_FHM_NUM );

	/* 機能部単位情報 */
	for( lwFuncCnt = CMD_NUM0; lwFuncCnt < RCD_FB_FHM_NUM; lwFuncCnt++ )
	{
		ltFuncEvery[lwFuncCnt].func_kind = 0xFFFF;
		/* 機能部単位　基本監視項目　初期化 */
		ltFuncEvery[lwFuncCnt].func_svitem = CMD_IVL_VAL;

		/* 機能部単位　拡張監視項目　初期化 */
		ltFuncEvery[lwFuncCnt].func_extsvitem = (RCD_TRA_WARMUP | RCD_TRA_AMPDWN_S3G);

		/* キャリア単位情報 */
		loopcnt = sizeof(ltFuncEvery[lwFuncCnt].car_every) / sizeof(T_RRH_CARDST_S3G_CAR_EVERY);
		for( lwCarrCnt = CMD_NUM0; lwCarrCnt < loopcnt ; lwCarrCnt++ )
		{
			/* キャリア単位　基本監視項目　初期化 */
			ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].func_svitem = CMD_IVL_VAL;
			/* キャリア単位　拡張監視項目　初期化 */
			ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].func_extsvitem = (RCD_TRA_WARMUP | RCD_TRA_AMPDWN_S3G);

			/* ブランチ単位情報 */
			loopcnt2 = sizeof(ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].br_every) / sizeof(T_RRH_CARDST_S3G_BR_EVERY);
			for( lwBrCnt = CMD_NUM0; lwBrCnt < loopcnt2 ; lwBrCnt++ )
			{
				/* ブランチ単位　基本監視項目　初期化 */
				ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].br_every[lwBrCnt].func_svitem = CMD_IVL_VAL;
				/* ブランチ単位　拡張監視項目　初期化 */
				ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].br_every[lwBrCnt].func_extsvitem = (RCD_TRA_WARMUP | RCD_TRA_AMPDWN_S3G);
			}
		}
	}

	/* ALM FMH */
	lwCpyAlmNum = ltFhmSta.sys[CMD_SYS_S3G].alm_num;
	alm_num = ltFhmSta.sys[CMD_SYS_S3G].alm_num;
	/* エラーコード取得 */
	cm_MemCpy(	&gw_lte_alm_code[CMD_NUM0],
				&ltFhmSta.sys[CMD_SYS_S3G].alm_inf[CMD_NUM0],
				sizeof(USHORT) * lwCpyAlmNum);

	/*	FHM装置のALM要因が擬似ALM(0xF0)のみの場合	*/
	if((ltFhmSta.sys[CMD_SYS_S3G].alm_num == 1)&&(ltFhmSta.sys[CMD_SYS_S3G].alm_inf[CMD_NUM0]==0xF0))
	{
		sysAlmOFF = D_RRH_ON;
	}
    /* 送信系切替制御中は0xFFを通知する */
    if (D_RRH_TRUE == m_rc_IsSwap())
    {
        gw_lte_alm_code[alm_num] = 0xFF;
        alm_num++;
    }

	/* ERR FHM */
	lwCpyErrNum = ltFhmSta.sys[CMD_SYS_S3G].err_num;
	err_num = ltFhmSta.sys[CMD_SYS_S3G].err_num;
	/* エラーコード取得 */
	cm_MemCpy(	&gw_lte_err_code[CMD_NUM0],
				&ltFhmSta.sys[CMD_SYS_S3G].err_inf[CMD_NUM0],
				sizeof(USHORT) * lwCpyErrNum);

	/*CPRIレートが9.8G/4.9Gの場合は, ALM情報数とERR情報数の上限を5107とする*/
	if( (CMD_LINE_BITRATE_49G == cmw_linebitrate) || (CMD_LINE_BITRATE_98G == cmw_linebitrate) )
	{
		ldwMaxAlmNum = CMD_MAX_ALM_NUM_HIG_BITRAT;
		ldwMaxErrNum = CMD_MAX_ERR_NUM_HIG_BITRAT;
	}

	/* 共用メモリLayer3と配下REが使用状態の呼び出し 取得用 */
	f_cmn_com_layer3_get( CMD_SYS_S3G, &ltLayer3StaLte );
	f_cmn_com_conn_disk_get(&shmConnDisc);

	/* Check intermediate device card state only 51~56 */
	if (parm_p->cpridat_cardstsreq.slot_no < 51 || parm_p->cpridat_cardstsreq.slot_no > 56)
	{
		/* REカード状態報告管理テーブルにより最新のREカード状態を構築 */
		for( lwReCnt = CMD_NUM0; lwReCnt < D_RRH_CPRINO_RE_MAX ; lwReCnt++ )
		{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf("[%d]%s layer3_rests :%d\n ", __LINE__, __FUNCTION__,ltLayer3StaLte.layer3_re[lwReCnt]);
	#endif
			/*	未使用または使用中が確定していない場合	*/
			if((shmConnDisc.conn_disc[lwReCnt] != D_RRH_ON)||(re_funcFailValid[lwReCnt+1] != D_RRH_ON))
			{
				/*	Layer状態が「③強制停止状態」または「⑤運用中アイドル状態」の場合は、未確定でも確定していると扱う	*/
				if((shmConnDisc.conn_disc[lwReCnt] == D_RRH_ON)&&
					((E_RRH_LAYER3_RE_STOP == ltLayer3StaLte.layer3_re[lwReCnt]) ||
					(E_RRH_LAYER3_RE_OPEIDLE == ltLayer3StaLte.layer3_re[lwReCnt])))
				{
					/*	CPRIリンク異常と扱う	*/
					re_funcFailValid[lwReCnt+1] = D_RRH_ON;
					lwCpriFail++;
				}
				continue;
			}
			/*	ココ以降は、CPRI使用中が確定している状態	*/
			/* Layer3状態が「②起動中状態」、または「④運用中状態」の配下REだけが対象処理 */
			if(( D_L3_STA_2 != ltLayer3StaLte.layer3_re[lwReCnt] ) &&
			( D_L3_STA_4 != ltLayer3StaLte.layer3_re[lwReCnt] ))
			{
				/*	CPRIリンク異常	*/
				lwCpriFail++;
				continue;
			}

			/* 共用メモリ(LTE)REカード状態の呼び出し 取得用 */
			f_cmn_com_cardst_s3g_get( lwReCnt + CMD_NUM1, &ltCardStaLte );
	#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf("[%d]%s [%d]signal_kind :0x%x\n ", __LINE__, __FUNCTION__,lwReCnt,ltCardStaLte.signal_kind);
	#endif
			/* 信号種別が0の場合はDon't Care */
			if ( CMD_NUM0 == ltCardStaLte.signal_kind )
			{
				/*	無応答(TO)は、CPRIリンク異常と扱う	*/
				lwCpriFail++;
				continue;

			}
			lwDataExist = CMD_ON;

			if (CMD_OK != ltCardStaLte.result)
			{
				/*	NG応答は、CPRIリンク異常と扱う	*/
				lwCpriFail++;
				continue;
			}
			lwAllErrRsp = CMD_OFF;

			/* カード基本監視項目 */
			m_rc_GetSvitemfunc( &lwTotalCardSvitem, ltCardStaLte.card_svitem );
			/* カード拡張監視項目                                            */
			m_rc_GetExtSvitem( &lwTotalCardExtSvitem, ltCardStaLte.card_extsvitem );
			/* 機能部数n */

			/* カード制御項目 */
			lwTotalCardCntitem |= ltCardStaLte.card_cntitem;

			lwFuncNum = ltCardStaLte.func_num;
	#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf("[%d]%s func_num:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_num);
			for( lwFuncCnt = CMD_NUM0; lwFuncCnt < lwFuncNum; lwFuncCnt++ )
			{
				printf("[%d]%s func_kind:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].func_kind);
				printf("[%d]%s func_failure:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].func_failure);
				printf("[%d]%s func_svitem:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].func_svitem);
				printf("[%d]%s func_extsvitem:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].func_extsvitem);
				printf("[%d]%s car_num:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].car_num);
				for( lwCarrCnt = CMD_NUM0; lwCarrCnt < ltCardStaLte.func_every[lwFuncCnt].car_num; lwCarrCnt ++ )
				{
					printf("[%d]%s     car_no:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].car_every[lwCarrCnt].car_no);
					printf("[%d]%s     band_width:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].car_every[lwCarrCnt].band_width);
					printf("[%d]%s     func_svitem:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].car_every[lwCarrCnt].func_svitem);
					printf("[%d]%s     func_extsvitem:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].car_every[lwCarrCnt].func_extsvitem);
					printf("[%d]%s     br_num:%d\n ", __LINE__, __FUNCTION__,ltCardStaLte.func_every[lwFuncCnt].car_every[lwCarrCnt].br_num);
				}
			}
		
	#endif
		/* 機能部単位情報 */
		for( lwFuncCnt = CMD_NUM0; lwFuncCnt < lwFuncNum; lwFuncCnt++ )
		{
			/* 機能部種別  */
			lwFuncKind = ltCardStaLte.func_every[lwFuncCnt].func_kind;

			/* Abnormality is detected in the functional part state of the RE */
			if(ltCardStaLte.card_svitem & (CMD_CARDSV_ACT_BIT | CMD_CARDSV_ALM_BIT))
			{
				if ((RCD_FB_DUP == lwFuncKind) || (RCD_FB_LNA == lwFuncKind) || (RCD_FB_T_PA == lwFuncKind))
				{
					/* Pause carrier */
					/* TBD: Call carrier pause hardware / soft interface */
				}
				else if ((RCD_FB_TRX == lwFuncKind) || (RCD_FB_TRX_INF == lwFuncKind)) /* TRX Release */
				{
						CMT_TSKIF_CPRIRCV_TRXRELREQ 	msgReq;
						cm_MemClr(&msgReq, sizeof(msgReq));

						/* Make msg request */
						msgReq.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)
														+	sizeof(CMT_CPRIF_TRXRELREQ);
						msgReq.cprircv_inf.link_num = lwReCnt;                        /* CPRI番号 */
    					msgReq.cprircv_inf.length   = CMD_CPRLN_TRXRELREQ;      /* buffer length */
						msgReq.cpridat_trxrelreq.signal_kind = CMD_CPRID_TRXRELREQ + CMD_NUM1;  /* 信号種別 */
						msgReq.cpridat_trxrelreq.carno = ltCardStaLte.slot_no;           /* キャリア番号 */

						/* SV-MT/eNB-MT接続解放応答 ( l3/rct->re/rec ) */
						(VOID)l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  &msgReq, sizeof( CMT_TSKIF_CPRIRCV_TRXRELREQ ) );
				}
			}

			if (RCD_FB_DUP == lwFuncKind)
			{	/* 0:DUP/BPF/SWITCHは、集約対象外 */
			continue;
			}

			/* Convert func_kind to index of ltFuncEvery table */
			lwFuncId = m_rc_ConvFuncKindToId(lwFuncKind);
			m_rc_GetFuncEvery(&ltFuncEvery[lwFuncId], ltCardStaLte.func_every[lwFuncCnt], &lwSmallCarNum[lwFuncId]);
		}

			/* RE# ALM */
			if( ( alm_num + ltCardStaLte.alm_num ) > ldwMaxAlmNum )
			{
				lwCpyAlmNum = ldwMaxAlmNum - alm_num;
			}
			else
			{
				lwCpyAlmNum = ltCardStaLte.alm_num;
			}
			/* 発生アラーム情報取得 */
			cm_MemCpy(	&gw_lte_alm_code[alm_num],
						&ltCardStaLte.alm_inf[CMD_NUM0],
						sizeof(USHORT) * lwCpyAlmNum);
			alm_num += lwCpyAlmNum;

			/* RE# ERR */
			if( ( err_num + ltCardStaLte.err_num ) > ldwMaxErrNum )
			{
				lwCpyErrNum = ldwMaxErrNum - err_num;
			}
			else
			{
				lwCpyErrNum = ltCardStaLte.err_num;
			}
			/* エラーコード取得 */
			cm_MemCpy(	&gw_lte_err_code[err_num],
						&ltCardStaLte.err_inf[CMD_NUM0],
						sizeof(USHORT) * lwCpyErrNum);
			err_num += lwCpyErrNum;
		}
	}

	/* 応答があり、かつ、配下REからの最新応答が全配下REにてNG応答を受信 */
	if( (CMD_ON == lwDataExist) && (CMD_ON == lwAllErrRsp) )
	{
		/* CPRI信号応答内容設定 */
		/* CPRI信号応答結果 51:処理NG(RE) */
		rsp_p->cpridat_cardstsres.result = CMD_HDL_NG;
		/* 異常ルートは可変長の長さを表すパラメータに0を設定	*/
		cm_MemClr(&(rsp_p->cpridat_cardstsres.cardsts_inf.slot_no) + CMD_NUM1,	/* スロット番号は正しく返す必要がある */
			(sizeof(CARDSTS_INF_S3G) - sizeof(rsp_p->cpridat_cardstsres.cardsts_inf.slot_no)) +	/* カード状態詳細 - スロット番号	*/
			(sizeof(USHORT) * CMD_NUM3));														/* 機能部数+ALM情報数+ERR情報数		*/

		/**********************/
		/* タスク間ヘッダ作成 */
		/**********************/
		rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)	+		/* CPRI信号送信情報部		*/
									sizeof(CMT_CPRIF_CARDSTSRES_S3G) +	/* 信号種別～カード拡張制御項目	*/
									(sizeof(USHORT) * CMD_NUM3);		/* 機能部数 + ALM情報数+ERR情報数		*/

		/************/
		/* 応答送信 */
		/************/
		ressnd_rslt = m_cm_L3MsgSend_REC(rsp_p);

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return ;
	}
	/* 配下REからの応答がない場合はFHMの情報を報告 */
	else if (CMD_OFF == lwDataExist)
	{
        /* CPRI信号応答内容設定 */
        /* CPRI信号応答結果 0:正常 */
        rsp_p->cpridat_cardstsres.result = CMD_NML;

        m_rc_GetInitValue_S3G(&(rsp_p->cpridat_cardstsres.cardsts_inf), &data_length, lwCpriFail, sysAlmOFF );

        /**********************/
        /* タスク間ヘッダ作成 */
        /**********************/
        rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)  +       /* CPRI信号送信情報部      */
                                    sizeof(CMT_CPRIF_CARDSTSRES_S3G) +  /* 信号種別～カード拡張制御項目   */
                                    (sizeof(USHORT) * data_length);     /* 機能部数 + ALM情報数+ERR情報数     */

        /************/
        /* 応答送信 */
        /************/
        ressnd_rslt = m_cm_L3MsgSend_REC(rsp_p);

#ifdef FHM_RCT_DEBUG_FOR_IT1
    printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
        return ;
	}
	else
	{}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s lwTotalCardSvitem: 0x%x\n ", __LINE__, __FUNCTION__,lwTotalCardSvitem);
#endif
	/* カード基本監視項目 */
	/* 使用中(USE) : FHMにおいて、S3GのTRX設定がなされてリソースが使用中の場合に「1:使用中」を報告します。*/
	/* 故障(ALM) : FHMがALM、若しくはすべての配下REがALMの場合に「1:故障発生」を報告します。*/
	/* エラー(ERR) :FHMがERR、若しくは一部の配下REがALMの場合に「1:ERR発生中」を報告します。*/
	lwTotalCardSvitem |= 	(svitem & (CMD_CARDSV_USE_BIT | CMD_CARDSV_ALM_BIT | CMD_CARDSV_ERR_BIT)) |
							CMD_CARDSV_IMP_BIT ;
	/* 運用(ACT) : FHMの状態を報告します。*/
	if( svitem & CMD_CARDSV_ACT_BIT )
	{
		lwTotalCardSvitem |= CMD_CARDSV_ACT_BIT;
	}
	else
	{
		lwTotalCardSvitem &= ~CMD_CARDSV_ACT_BIT;
	}

	rsp_p->cpridat_cardstsres.cardsts_inf.card_svitem = lwTotalCardSvitem;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s card_svitem: 0x%x\n ", __LINE__, __FUNCTION__,lwTotalCardSvitem);
#endif
	/* REカード拡張監視項目 */
	rsp_p->cpridat_cardstsres.cardsts_inf.card_extitem = lwTotalCardExtSvitem;
	/* カード拡張制御項目: 0 固定値 */
	rsp_p->cpridat_cardstsres.cardsts_inf.card_extcntitem = CMD_NUM0;
	/* カード基本制御項目 */
	rsp_p->cpridat_cardstsres.cardsts_inf.card_cntitem = (lwTotalCardCntitem & (CMD_CRDCTBIT_L_RST_LTE | CMD_CRDCTBIT_AUTO_RST_LTE | CMD_CRDCTBIT_R_RST_LTE));

	setdata = malloc(data_size);
	cm_MemClr(setdata, data_size);

	setdata[set_data_cnt] = lwTotalFuncNum;
	set_data_cnt++;

	/* 機能部故障状態 */ /* TBD spec unclear Q&A*/
	for( lwFuncCnt = CMD_NUM0; lwFuncCnt < RCD_FB_FHM_NUM; lwFuncCnt++ )
	{
		if( 0xFFFF == ltFuncEvery[lwFuncCnt].func_kind )
		{
			continue;
		}

		lwTotalFuncNum++;

		/* 使用中の配下REがSYS-ALMかHigh-ALMかCPRIリンク異常の場合 */
		if(( CMD_NUM0 != ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_SYS )) ||
		   ( CMD_NUM0 != ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_HIGH )) ||
		   ( CMD_NUM0 != lwCpriFail))
		{
			/* すべての配下REがSYS-ALMかHigh-ALM(normalと未実装がない)場合 */
			if(( CMD_NUM0 == ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_NOTINS ))&&
				( CMD_NUM0 == ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_NORMAL )))
			{
				/* ＜SYS-ALM＞ */
				ltFuncEvery[lwFuncCnt].func_failure = RCD_FB_ALM_SYS;
			}
			/* 一部の配下REがSYS-ALMかHigh-ALM(normalか未実装がある)場合 */
			else
			{
				/* ＜HIGH-ALM＞ */
				ltFuncEvery[lwFuncCnt].func_failure = RCD_FB_ALM_HIGH;
			}
		}
		/* 使用中の配下REがnormalまたは未実装の場合 */
		else
		{
			/* 1台以上の配下REがNormal */
			if( CMD_NUM0 != ( ltFuncEvery[lwFuncCnt].func_failure & RCD_FB_ALM_FLG_NORMAL ))
			{
				/* ＜normal＞ */
				ltFuncEvery[lwFuncCnt].func_failure = RCD_FB_ALM_NORMAL;
			}
			else
			{
				/* ＜未実装＞ */
				ltFuncEvery[lwFuncCnt].func_failure = RCD_FB_ALM_NOTINS;
			}
		}

		/*	TRX-INF機能部(4)の場合	*/
		if(ltFuncEvery[lwFuncCnt].func_kind == RCD_FB_TRX_INF)
		{
			/*	USE/ALM/ERR/実装bitはFHMの状態を設定する	*/
			ltFuncEvery[lwFuncCnt].func_svitem |= 	(svitem & (CMD_CARDSV_USE_BIT | CMD_CARDSV_ALM_BIT | CMD_CARDSV_ERR_BIT)) | CMD_CARDSV_IMP_BIT ;
			/* 運用(ACT) : FHMの状態を報告する	*/
			if( svitem & CMD_CARDSV_ACT_BIT )
			{
				ltFuncEvery[lwFuncCnt].func_svitem |= CMD_CARDSV_ACT_BIT;
			}
			else
			{
				ltFuncEvery[lwFuncCnt].func_svitem &= ~CMD_CARDSV_ACT_BIT;
			}
			/*	故障状態(SYS-ALM :FHMのカード基本監視項目(LTE)がALM（＊１）)	*/
			/*	[暫定対処]FHMのALM要因が0xF0のみの場合はSYS-ALMとしない	*/
			if(( svitem & CMD_CARDSV_ALM_BIT ) && (sysAlmOFF != D_RRH_ON))
			{
				ltFuncEvery[lwFuncCnt].func_failure = RCD_FB_ALM_SYS;
			}
		}
		/* Copy data: 機能部種別, 機能部故障状態, 機能部基本監視項目, 機能部拡張監視項目, 対象キャリア数n */
		/* 対象キャリア数は配下REの中で最小のものを報告 */
		//cm_MemCpy( &setdata[set_data_cnt], &ltFuncEvery[lwFuncCnt], sizeof(USHORT) * CMD_NUM5 );
		setdata[set_data_cnt+0] = ltFuncEvery[lwFuncCnt].func_kind;
		setdata[set_data_cnt+1] = ltFuncEvery[lwFuncCnt].func_failure;
		setdata[set_data_cnt+2] = ltFuncEvery[lwFuncCnt].func_svitem;
		setdata[set_data_cnt+3] = ltFuncEvery[lwFuncCnt].func_extsvitem;
		setdata[set_data_cnt+4] = lwSmallCarNum[lwFuncCnt];
		set_data_cnt += CMD_NUM5;

		/* Sort data */
		for( lwCarrCnt = CMD_NUM0; lwCarrCnt < ltFuncEvery[lwFuncCnt].car_num; lwCarrCnt++ )
		{
			l3_com_ushortsort_asc(
				ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].br_num,
				sizeof(T_RRH_CARDST_S3G_BR_EVERY)/sizeof(USHORT),
				(USHORT*)ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].br_every
			);
		}
		l3_com_ushortsort_asc(
			ltFuncEvery[lwFuncCnt].car_num,
			sizeof(T_RRH_CARDST_S3G_CAR_EVERY)/sizeof(USHORT),
			(USHORT*)ltFuncEvery[lwFuncCnt].car_every
		);

		/* 対象キャリア数は配下REの中で最小のものを報告 */
		ltFuncEvery[lwFuncCnt].car_num = lwSmallCarNum[lwFuncCnt];

		/* キャリア単位情報 */
		for( lwCarrCnt = CMD_NUM0; lwCarrCnt < ltFuncEvery[lwFuncCnt].car_num; lwCarrCnt++ )
		{
			/* Copy data:キャリア番号, システム帯域幅, 機能部基本監視項目, 機能部拡張監視項目, 対象ブランチ数m */
			cm_MemCpy( &setdata[set_data_cnt], &ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt], sizeof(USHORT) * CMD_NUM5 );
			set_data_cnt += CMD_NUM5;
			/* ブランチ単位情報 */
			/* Copy data:(ブランチ番号, 機能部基本監視項目, 機能部拡張監視項目) * 対象ブランチ数m */
			cm_MemCpy( &setdata[set_data_cnt], &ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].br_every[CMD_NUM0],
						sizeof(USHORT) * CMD_NUM3 * ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].br_num );
			set_data_cnt += ( CMD_NUM3 * ltFuncEvery[lwFuncCnt].car_every[lwCarrCnt].br_num );
		}
	}

	setdata[0] = lwTotalFuncNum;

	/*******************************************/
	/* ALM情報数/ALM情報/ERR情報数/ERR情報設定 */
	/*******************************************/
	setdata[set_data_cnt] = alm_num;
	ldwIndexAlmNum = set_data_cnt;
	set_data_cnt++;

	if( CMD_NUM0 != alm_num )
	{
		/* Sort data */
//		l3_com_heapsort(gw_lte_alm_code, alm_num, sizeof(USHORT), l3_com_cmp_ushort, l3_com_swap_ushort);
		cm_MemCpy(&setdata[set_data_cnt], gw_lte_alm_code, sizeof(USHORT) * alm_num);
		set_data_cnt += alm_num;
	}

	setdata[set_data_cnt] = err_num;
	ldwIndexErrNum = set_data_cnt;
	set_data_cnt++;

	if( CMD_NUM0 != err_num )
	{
		/* Sort data */
//		l3_com_heapsort(gw_lte_err_code, err_num, sizeof(USHORT), l3_com_cmp_ushort, l3_com_swap_ushort);
		cm_MemCpy(&setdata[set_data_cnt], gw_lte_err_code, sizeof(USHORT) * err_num);
		set_data_cnt += err_num;
	}

	lwCurrLength = sizeof(CMT_CPRIF_CARDSTSRES_S3G);

	/*Check length of data and cut data if it is over*/
	if( CMD_MAX_SIZ_STA_REP < (lwCurrLength + set_data_cnt * sizeof(USHORT)) )
	{
		set_data_cnt_old = set_data_cnt;

		/*Update data count based on remained capacity*/
		set_data_cnt = (CMD_MAX_SIZ_STA_REP - lwCurrLength)/sizeof(USHORT);

		/*Update alm/error number information after cut*/
		/*Only cut error code*/
		if( (set_data_cnt_old - set_data_cnt) <= err_num )
		{
			/*Decrease error numbers equal to number of elements removed*/
			setdata[ldwIndexErrNum] -= (set_data_cnt_old - set_data_cnt);
		}
		/*Cut all error codes and begin cut alm code */
		else if( (set_data_cnt_old - set_data_cnt) <= (err_num + alm_num) )
		{
			/*Decrease alm numbers equal to: number of elements removed substract to error elements */
			setdata[ldwIndexAlmNum] -= (set_data_cnt_old - set_data_cnt - err_num);
			/* error number */
			setdata[set_data_cnt - CMD_NUM1] = CMD_NUM0;
		}
	}

	/* 応答データに可変長データを設定 */
	cm_MemCpy(&(rsp_p->cpridat_cardstsres.cardsts_inf.card_extcntitem) + CMD_NUM1,
			  setdata,
			  sizeof(USHORT) * (set_data_cnt));

	/****************/
	/* データ長設定 */
	/****************/
	datalength = (USHORT)(	(sizeof(CMT_CPRISIGSND_INF)
						+	sizeof(CMT_CPRIF_CARDSTSRES_S3G))	/* 信号種別～キャリア情報	*/
						+	(set_data_cnt * CMD_NUM2));		/* ALM情報数+ALM情報	+	*/
															/* ERR情報数+ERR情報		*/

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + datalength;

	/************/
	/* 応答送信 */
	/************/
	ressnd_rslt = m_cm_L3MsgSend_REC(rsp_p);

	/*****************/
	/* バッファ開放 */
	/****************/
	free(setdata);
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	if(cmw_filedatsnd_flag !=0)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "card state req End:flag=%d", cmw_filedatsnd_flag);
	}
	return ;
}

/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    Get カード基本監視項目
 *  @note     すべての配下REがALMの場合に「1:故障発生」を報告します。
			  一部の配下REがALMの場合に「1:ERR発生中」を報告します。
 *  @param    awTotalSvItem_p [out] カード基本監視項目
 *  @param    awSvItem [in] 配下REのカード基本監視項目
 *  @return   VOID
 *  @date  2015/07/31 FPT)Yen create
 *  @date  2015/09/30 FPT)Yen fix bug IT2
 *  @TBD_No
*/
/********************************************************************************************************************/

VOID	m_rc_GetSvitem(USHORT* awTotalSvItem_p, USHORT awSvItem)
{

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start(SvItem:0x%x, awTotalSvItem_p:0x%x)\n ", __LINE__, __FUNCTION__, awSvItem, *awTotalSvItem_p);
#endif
	if( NULL == awTotalSvItem_p )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}
	/*	配下REの基本監視項目のみ設定すること!!!			*/
	/*	FHMのカード状態はこの関数で設定しないこと!!!	*/
	
	/* Init data(first time) */
	if( 0x0000 == *awTotalSvItem_p )
	{
		/* clear bit D15, D14, D13, D12, D11, D8, D6, D4, D3, D2, D1, D0 */
		/*	配下REの場合はERR情報もOFFする	*/
		*awTotalSvItem_p = awSvItem & 0x06A0;
	}
	/*	初回以外の場合	*/
	else
	{
		/*	実装, ACT, USE bitは論理和する	*/
		*awTotalSvItem_p |= awSvItem & (CMD_CARDSV_IMP_BIT | CMD_CARDSV_ACT_BIT | CMD_CARDSV_USE_BIT);
		
		/*	以下は全配下REがALMでない場合	*/
		/*	1)他配下REが全てALM状態においてALMなし状態のREを検出した場合					*/
		/*	2)他配下REにてALM状態でないREが存在する状態において、ALM状態のREを検出した場合	*/
		if(( *awTotalSvItem_p & CMD_CARDSV_ALM_BIT) != (awSvItem & CMD_CARDSV_ALM_BIT))
		{
			/*	ERR bit ON/ALM bit OFF	*/
			*awTotalSvItem_p |= CMD_CARDSV_ERR_BIT;
			*awTotalSvItem_p &= ~CMD_CARDSV_ALM_BIT;
		}
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end(SvItem:0x%x, awTotalSvItem_p:0x%x)\n ", __LINE__, __FUNCTION__, awSvItem, *awTotalSvItem_p);
#endif
	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    Get 機能部基本監視項目
 *  @note     すべての配下REがBLKの場合(論理積)に「1:発生中」を報告します。
			  BLK以外(USE/ALM/ERR/実装/ACT)の場合は一部配下REが1の場合(論理和)に「1:発生中」を報告します。
 *  @param    awTotalSvItem_p [out] カード基本監視項目
 *  @param    awSvItem [in] 配下REのカード基本監視項目
 *  @return   VOID
 *  @date  2015/07/31 FPT)Yen create
 *  @date  2015/09/30 FPT)Yen fix bug IT2
 *  @TBD_No
*/
/********************************************************************************************************************/

VOID	m_rc_GetSvitemfunc(USHORT* awTotalSvItem_p, USHORT awSvItem)
{

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start(SvItem:0x%x, awTotalSvItem_p:0x%x)\n ", __LINE__, __FUNCTION__, awSvItem, *awTotalSvItem_p);
#endif
	if( NULL == awTotalSvItem_p )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}
	/*	配下REの機能部基本監視項目のみ設定すること!!!			*/
	/*	FHMの監視項目は設定しないこと!!!						*/
	/*	配下REのカード基本監視項目では使用しないこと!!!			*/
	
	/* Init data(first time) */
	if( CMD_IVL_VAL == *awTotalSvItem_p )
	{
		/* clear bit D15, D14, D13, D12, D11, D6, D4, D3, D2, D0 */
		/*	配下REの機能部監視項目の場合はBLK情報は有効とする	*/
		*awTotalSvItem_p = awSvItem & 0x07A2;
	}
	/*	初回以外の場合	*/
	else
	{
		/*	ALM, ERR, 実装, ACT, USE bitは論理和する	*/
		*awTotalSvItem_p |= awSvItem & (CMD_CARDSV_ALM_BIT | CMD_CARDSV_ERR_BIT | CMD_CARDSV_IMP_BIT | CMD_CARDSV_ACT_BIT | CMD_CARDSV_USE_BIT);
		
		/*	以下は全配下REがBLKでない場合	*/
		/*	1)他配下REが全てBLK状態においてBLKなし状態のREを検出した場合					*/
		/*	2)他配下REにてBLK状態でないREが存在する状態において、BLK状態のREを検出した場合	*/
		if(( *awTotalSvItem_p & CMD_CARDSV_BLK_BIT) != (awSvItem & CMD_CARDSV_BLK_BIT))
		{
			/*	BLK bit OFF	*/
			*awTotalSvItem_p &= ~CMD_CARDSV_BLK_BIT;
		}
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end(SvItem:0x%x, awTotalSvItem_p:0x%x)\n ", __LINE__, __FUNCTION__, awSvItem, *awTotalSvItem_p);
#endif
	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    Get カード拡張監視項目
 *  @note     すべての配下REがALMの場合に「1:故障発生」を報告します。
 *  @note     一部の配下REがALMの場合に「1:ERR発生中」を報告します。
 *  @param    awTotalExtSvItem_p    [out]   カード拡張監視項目
 *  @param    awExtSvItem           [in ]   配下REのカード拡張監視項目
 *  @return   VOID
 *  @date     2015/07/31 FPT)Yen create
 *  @date     2016/03/24 tdips)enoki 仕様変更 拡張監視項目の集約条件変更(OR条件⇒AND条件)
*/
/********************************************************************************************************************/
VOID	m_rc_GetExtSvitem(USHORT* awTotalExtSvItem_p, USHORT awExtSvItem)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start(awTotalExtSvItem_p: 0x%x, awExtSvItem: 0x%x)\n ", __LINE__, __FUNCTION__, *awTotalExtSvItem_p, awExtSvItem);
#endif
	if( NULL == awTotalExtSvItem_p )
	{

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}

	/* 1つ以上の配下REがWarm Upなしの場合に「0:Warm Upなし状態」とする*/
	/* 1つ以上の配下REがAMP部電源断なしの場合に「0:AMP部電源断なし」とする */
	*awTotalExtSvItem_p &= awExtSvItem;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    Get カード拡張監視項目
 *  @note     すべての配下REがALMの場合に「1:故障発生」を報告します。
 *  @note     一部の配下REがALMの場合に「1:ERR発生中」を報告します。
 *  @param    awTotalExtSvItem_p    [out] カード拡張監視項目
 *  @param    awExtSvItem           [in ] 配下REのカード拡張監視項目
 *  @return   VOID
 *	@date     2015/07/31 FPT)tan create
 *  @date     2016/03/24 tdips)enoki 仕様変更 拡張監視項目の集約条件変更(OR条件⇒AND条件)*/
/********************************************************************************************************************/

VOID	m_rc_GetExtSvitem_3G(USHORT* awTotalExtSvItem_p, USHORT awExtSvItem)
{

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif
	if( NULL == awTotalExtSvItem_p )
	{

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}

	/* 1つ以上の配下REがWarm Upなしの場合に「0:Warm Upなし状態」とする*/
	/* 1つ以上の配下REがAMP部電源断なしの場合に「0:AMP部電源断なし」とする */
	*awTotalExtSvItem_p &= awExtSvItem;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Convert function kind to index
 *  @note
 *  @param  awFuncKind [in] function kind
 *  @return Index of function kind
 *  @date   2015/08/02 FPT)Yen create
*/
/********************************************************************************************************************/

USHORT	m_rc_ConvFuncKindToId( USHORT awFuncKind )
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
 *  @date   2015/08/02 FPT)Yen create
 *  @date   2015/10/22 FPT)Yen fix bug IT3 No24
 *  @date   2016/02/29 TDI)satou 配下REから報告されたキャリア数を返すように
 *  @date   2016/03/01 TDI)satou 配下REから報告された最小のシステム帯域幅を設定するように
*/
/********************************************************************************************************************/

VOID	m_rc_GetFuncEvery( 	T_RRH_CARDST_S3G_FUNC_EVERY* atTotalFuncEvery_p,
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
	m_rc_GetSvitemfunc( &atTotalFuncEvery_p->func_svitem, atFuncEvery.func_svitem );
	
	/* 機能部拡張監視項目 */
	m_rc_GetExtSvitem( &atTotalFuncEvery_p->func_extsvitem, atFuncEvery.func_extsvitem );

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
		m_rc_GetSvitemfunc( &atTotalFuncEvery_p->car_every[lwCarCnt2].func_svitem,
						 atFuncEvery.car_every[lwCarCnt1].func_svitem );
		/* 機能部拡張監視項目 */
		m_rc_GetExtSvitem( &atTotalFuncEvery_p->car_every[lwCarCnt2].func_extsvitem,
							atFuncEvery.car_every[lwCarCnt1].func_extsvitem );

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
			m_rc_GetSvitemfunc( &atTotalFuncEvery_p->car_every[lwCarCnt2].br_every[lwBrCnt2].func_svitem,
							 atFuncEvery.car_every[lwCarCnt1].br_every[lwBrCnt1].func_svitem );
			/* 機能部拡張監視項目 */
			m_rc_GetExtSvitem( &atTotalFuncEvery_p->car_every[lwCarCnt2].br_every[lwBrCnt2].func_extsvitem,
								atFuncEvery.car_every[lwCarCnt1].br_every[lwBrCnt1].func_extsvitem );
		}
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
}

/********************************************************************************************************************/
/**
 *  @brief  Init table value for LTE
 *  @note
 *  @param  atInitTable [out] Init value
 *  @param  data_length [out] Data length
 *  @return VOID
 *  @date   2015/10/20 FPT)Tan create FHM技説-設計変更-024
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 *  @date	2016/02/05 TDI)satou RE0台空転でメモリを食い尽くす問題の対処(free漏れを修正)
 *  @date   2017/04/10 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 *  @date   2017/08/17 FJT)ohashi  17A(3201-16B-TS91)対応 カード基本監視項目の使用中状態報告処理修正
 */
/********************************************************************************************************************/

VOID    m_rc_GetInitValue_S3G( CARDSTS_INF_S3G* atInitTable, USHORT *data_length, USHORT awCpriFail, USHORT sysAlmOFF )
{
    USHORT*             setdata;
    USHORT              svitem;
    T_RRH_FHM_STATE     ltFhmSta;
    USHORT              set_data_cnt = CMD_NUM0;        /* 設定データカウンタ            */
    USHORT              data_size;
	USHORT				loopcnt;

    data_size = sizeof(USHORT) *
                (CMD_NUM1           +           /* 機能部数                     */
                ((((CMD_NUM3 * CMD_NUM8) + CMD_NUM5) * CMD_NUM5 + CMD_NUM5) * CMD_NUM29) +
                                            /* 機能部情報                    */
                CMD_NUM1            +       /* ALM情報数                   */
                CMD_CRDALM_INFNUMMAX+       /* ALM情報                        */
                CMD_NUM1            +       /* ERR情報数                   */
                CMD_CRDERR_INFNUMMAX);      /* ERR情報                        */




    setdata = malloc(data_size);
    if (NULL == setdata)
    {
    	cm_Assert(CMD_ASL_USEHIGH, data_size, "malloc NG");
    	*data_length = 0;
    	return;
    }

    atInitTable->slot_no = CMD_NUM1;
    /*************************************************************************************/
    /* カード名                                                                          */
    /*************************************************************************************/
	for (loopcnt = CMD_NUM0; loopcnt < sizeof(atInitTable->card_name)/sizeof(USHORT); loopcnt++)
	{
		atInitTable->card_name[loopcnt*CMD_NUM2] = (UCHAR)((cmw_tra_inv_tbl.cmx_cardname[loopcnt] & RCD_HI_MSK) >> CMD_BIT8);
		atInitTable->card_name[loopcnt*CMD_NUM2+CMD_NUM1] = (UCHAR)(cmw_tra_inv_tbl.cmx_cardname[loopcnt] & RCD_LW_MSK);
	}

    f_cmn_com_fhm_state_get( &ltFhmSta );
    svitem = ltFhmSta.FhmBase;
    atInitTable->card_svitem = 0x0000;

    /* カード基本監視項目 */
    /* 使用中(USE) : FHMにおいて、S3GのTRX設定がなされてリソースが使用中の場合に「1:使用中」を報告します。*/
    /* 故障(ALM) : FHMがALMの場合に「1:故障発生」を報告します。*/
    /* エラー(ERR) :FHMがERRの場合に「1:ERR発生中」を報告します。*/
    atInitTable->card_svitem |= (svitem & (CMD_CARDSV_USE_BIT | CMD_CARDSV_ALM_BIT | CMD_CARDSV_ERR_BIT)) | CMD_CARDSV_IMP_BIT ;

	/* REカード状態報告(LTE)は、S3GキャリアがONの時USE報告 */
	if (0 != (svitem & CMD_CARDSV_USE_BIT))
	{
	    if (0 == (cmw_tra_sv_tbl.carr_onoff & CMD_CARDSV_S3G_USE_BIT))
	    {
	        atInitTable->card_svitem &= ~CMD_CARDSV_USE_BIT;
	    }
	}

    /* 運用(ACT) : FHMの状態を報告します。*/
    if( svitem & CMD_CARDSV_ACT_BIT )
    {
        atInitTable->card_svitem |= CMD_CARDSV_ACT_BIT;
    }
    else
    {
        atInitTable->card_svitem &= ~CMD_CARDSV_ACT_BIT;
    }

    atInitTable->card_cntitem    = CMD_NUM0;
    atInitTable->card_extcntitem = CMD_NUM0;

    setdata[set_data_cnt] = 1;
    set_data_cnt ++;
    setdata[set_data_cnt] = RCD_FB_TRX_INF;
    set_data_cnt ++;

    /* ＜SYS-ALM＞ FHMのカード基本監視項目(LTE)がALM(ALM要因=0xF0以外) */
    /* または、    使用中の全配下REがCPRIリンク異常 */
    if(((0 != (svitem & CMD_CARDSV_ALM_BIT)) && (sysAlmOFF != D_RRH_ON)) || (awCpriFail != 0))
    {
        setdata[set_data_cnt] = RCD_FB_ALM_SYS;
    }
    /* ＜未実装＞ すべての配下REが未使用の場合 */
    /* かつ、 FHMのALM要因=なしまたは0xF0の場合 */
    else
    {
        setdata[set_data_cnt] = RCD_FB_ALM_NOTINS;
    }

    set_data_cnt ++;
    /* 機能部#1機能部基本監視項目 */
    setdata[set_data_cnt] = atInitTable->card_svitem;
    set_data_cnt ++;
    /* 機能部#1機能部拡張監視項目 */
    setdata[set_data_cnt] = atInitTable->card_extitem;
    set_data_cnt ++;
    /* 機能部#1対象キャリア数n */
    setdata[set_data_cnt] = CMD_NUM0;
    set_data_cnt ++;

    /* 送信系切替制御中は0xFFを通知する */
    if (D_RRH_TRUE == m_rc_IsSwap())
    {
        ltFhmSta.alm_inf[ltFhmSta.alm_num] = 0xFF;
        ltFhmSta.alm_num++;
    }

    setdata[set_data_cnt] = ltFhmSta.alm_num;
    set_data_cnt ++;
    /* エラーコード取得 */
    cm_MemCpy(  &setdata[set_data_cnt],
                &ltFhmSta.alm_inf[CMD_NUM0],
                sizeof(USHORT) * ltFhmSta.alm_num);

    set_data_cnt += ltFhmSta.alm_num;

    setdata[set_data_cnt] = ltFhmSta.err_num;
    set_data_cnt ++;
    /* エラーコード取得 */
    cm_MemCpy(  &setdata[set_data_cnt],
                &ltFhmSta.err_inf[CMD_NUM0],
                sizeof(USHORT) * ltFhmSta.err_num);
    set_data_cnt += ltFhmSta.err_num;

    /* 応答データに可変長データを設定 */
    cm_MemCpy(&(atInitTable->card_extcntitem) + CMD_NUM1,
              setdata,
              sizeof(USHORT) * (set_data_cnt));

    *data_length = set_data_cnt;

    free(setdata);
}
/********************************************************************************************************************/
/**
 *  @brief  送信系切替状態取得
 *  @retval D_RRH_TRUE  : 送信系切替中
 *  @retval D_RRH_FALSE : 非制御中
 *  @date   2015/11/05 TDI)satou create
 */
/********************************************************************************************************************/
INT m_rc_IsSwap(VOID)
{
    UINT regAdr, regVal;

    regAdr = D_RRH_REG_DIS_LSWAP0;
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regAdr, &regVal);
    regVal &= 0xFFFF;
    if (0 != regVal)
    {
        return D_RRH_TRUE;
    }

    regAdr = D_RRH_REG_DIS_LSWAP1;
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regAdr, &regVal);
    regVal &= 0xFFFF;
    if (0 != regVal)
    {
        return D_RRH_TRUE;
    }

    regAdr = D_RRH_REG_DIS_WSWAP;
    (VOID)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regAdr, &regVal);
    regVal &= 0xFFFF;
    if (0 != regVal)
    {
        return D_RRH_TRUE;
    }

    return D_RRH_FALSE;
}
/* @} */
/* @} */
/* @} */


