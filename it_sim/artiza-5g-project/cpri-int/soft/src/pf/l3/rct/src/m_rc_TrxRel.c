/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_TrxRel.c
 *  @brief  RE TRX release process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2009/02/09 FFCS)Linlj (M-S3G-eNBPF-01332) TRX設定解放実施すると、
 *                      ONになっているキャリアがあるにもかかわらず、USE-LEDが消灯する。
 *  @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) 
 *  @date   2010/01/13 QNET)Kabasima MOD CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース追加修正
 *  @date   2010/09/06 COAMS)Kudou Modify for M-S3G-eNBSYS-01874
 *  @date   2012/10/16 FFCS)Zhangfeng modify for M-S3G-eNBSYS-02684
 *  @date	2012/10/22 FFCS)Zhangfeng modify for M-S3G-eNBPF-04288
 *  @date   2013/11/26 FFCS)Xut modify for ZYNQ.
 *  @date   2015/04/27 FFCS)Zhangxq TDD SRE
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
#include "f_rrh_reg_dis.h"
#include "f_rrh_reg_cpri.h"
#include "f_rrh_reg_cnt.h"
#include "f_rrh_reg_mix.h"
#include "f_sys_def.h"
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RECからのTRX解放要求受信
 *  @note RECからのTRX解放要求受信
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 2015/10/19 FPT)Dung Update response result table init
 *  @date 2015/11/08 FPT)Yen update ( normal router add free buffer )
 *  @date 2015/11/11 FPT)Yen Fix bug IT3 No70
 *  @date 2016/03/18 TDI)satou 送信対象REが存在しない場合、直ちにNGをRECに通知するように
 *  @date 2021/02/03 TDI)Huan.dh Requirement No.17 disable send TRX release to RE
 */
/********************************************************************************************************************/

VOID m_rc_TrxRel_Transfer_Req( CMT_TSKIF_CPRIRCV_TRXRELREQ* parm_p)
{
	UINT	sigchk_rslt;						/*  result of checking signal */
	USHORT	sigsys_rslt;						/*  system by checking signal */
	CMT_TSKIF_CPRISND_TRXRELRES	*rsp_p;			/* タスク間フレームポインタ	  */
	UINT	lwRslt;								/* バッファ獲得結果			  */
	USHORT	lwCarrNo;
	USHORT	lwCnt;
	INT	errcd;
	USHORT						lwTimerId;
	//T_TRXSET_PARA_3G			ltTrxSetPara_3G;
	T_TRXSET_PARA_LTE			ltTrxSetPara_LTE;
	T_RRH_LAYER3				ltLayer3Sta;	/* Layer3状態 */
	USHORT	lwCarrierMax[CMD_MAX_SYSNUM] = {
				CMD_MAX_CAR_3G,		/* 3G */
				CMD_MAX_CAR_LTE		/* S3G */
			};

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* 信号を有効にしてください。 */
	sigchk_rslt = m_cm_chksig( ((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind, &sigsys_rslt );
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt, 
							"m_rc_TrxRel_Transfer_Req validate signal NG");
		return;
	}
	/* ポインタ初期化 */
	rsp_p = NULL;
	/* バッファ獲得 */
	lwRslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_TRXRELRES),
								CMD_NUM1,
								(VOID **)&rsp_p	);
	/* バッファ獲得失敗 */
	if(lwRslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					lwRslt,
					"m_rc_TrxSet_Transfer_Req cm_L2BfGet NG");
		return;
	}

	/* CPRI信号応答結果にOKを設定*/
	rsp_p->cpridat_trxrelres.result      = CMD_NML;

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_TRXRELRES);

	/* キャリア番号設定 */
	rsp_p->cpridat_trxrelres.carno = parm_p->cpridat_trxrelreq.carno;
	
	/* 信号種別設定 */
	rsp_p->cpridat_trxrelres.signal_kind = CMD_CPRID_TRXRELRES + sigsys_rslt;

	/* 3Gの場合 */
	if( CMD_SYS_3G == sigsys_rslt )
	{
		/* その他エラー(RE)(42) */
		lwRslt = CMD_OTHR_ERR_RE;
	}
	/* LTEの場合 */
	else
	{
		/* 処理NG(RE)(51) */
		lwRslt = CMD_HDL_NG;
	}

	lwCarrNo = parm_p->cpridat_trxrelreq.carno;

	/*  パラメータチェック */
	if ((lwCarrNo < CMD_MIN_CARNUM)||
		(lwCarrNo > lwCarrierMax[sigsys_rslt]))
	{
		/* CPRI信号応答結果設定 */
		rsp_p->cpridat_trxrelres.result = lwRslt;

		/* l3/rctからのメッセージをRECに送信する(TRX設定応答) */
		m_cm_L3MsgSend_REC(rsp_p);
		/* 終了 */
		return;
	}

	/* 共用メモリLayer3の呼び出し 取得用 */
	f_cmn_com_layer3_get(sigsys_rslt, &ltLayer3Sta);

	/* 送信対象のREがいない場合はNG応答する */
	for(lwCnt = CMD_NUM0; lwCnt < D_RRH_CPRINO_RE_MAX; lwCnt++)
	{
		if(D_L3_STA_4 == ltLayer3Sta.layer3_re[lwCnt])
		{
			break;
		}
	}
	if (D_RRH_CPRINO_RE_MAX == lwCnt)
	{
		rsp_p->cpridat_trxrelres.result = lwRslt;
		m_cm_L3MsgSend_REC(rsp_p);
		return;
	}

	if( CMD_SYS_3G == sigsys_rslt )
	{
		// /* 共有メモリ(3G)TRX設定パラメータテーブルの呼び出し 取得用 */
		// f_cmn_com_trxset_para_3g_get(parm_p->cpridat_trxrelreq.carno, &ltTrxSetPara_3G);

		// /* 指定キャリアについてTRX解放済みの場合 */
		// if( CMD_NUM0 == ltTrxSetPara_3G.wSigKind )
		// {
		// 	/* l3/rctからの正常メッセージをRECに送信する(TRX解放応答(0:正常)) */
		// 	m_cm_L3MsgSend_REC(rsp_p);
		// 	return;
		// }
		// /* TrxRelease is running */
		// if( E_RRH_TRXSTA_IDLE != ltTrxSetPara_3G.wStatus )
		// {
		// 	/* ASSERT */
		// 	cm_Assert(	CMD_ASL_USELOW,
		// 				parm_p->cpridat_trxrelreq.carno,
		// 				"m_rc_TrxRel_Transfer_Req: TrxRelease is running");

		// 	cm_BFree(rsp_p);
		// 	return;
		// }

		// ltTrxSetPara_3G.wStatus = E_RRH_TRXSTA_RELING;			/* x */
		// /* TRX設定パラメータ(3G)設定 (TrxRelease is running) */
		// f_cmn_com_trxset_para_3g_set(parm_p->cpridat_trxrelreq.carno, &ltTrxSetPara_3G);

		// lwTimerId = CMD_TIMID_TRXREL_3GMIN + lwCarrNo - CMD_NUM1;
		// /* 待ちフラグ(TRX解放応答)がONに設定します */
		// gw_rcw_WaitResFlag_trxrel_3g[lwCarrNo - CMD_NUM1] = D_L3_ON;
		
		/* Set NG for 3G CPRI signal response result */
		rsp_p->cpridat_trxrelres.result      = CMD_RPT_NG;
		/* Send NG message from l3/rct to REC */
		m_cm_L3MsgSend_REC(rsp_p);

	}
	else
	{
		/* 共用メモリシスパラ格納領域(LTE)の呼び出し 取得用 */
		f_cmn_com_trxset_para_lte_get(parm_p->cpridat_trxrelreq.carno, &ltTrxSetPara_LTE );

		/* 指定キャリアについてTRX解放済みの場合 */
		if( CMD_NUM0 == ltTrxSetPara_LTE.wSigKind )
		{
			/* l3/rctからの正常メッセージをRECに送信する(TRX解放応答(0:正常)) */
			m_cm_L3MsgSend_REC(rsp_p);
			return;
		}
		/* TrxRelease is running */
		if( E_RRH_TRXSTA_IDLE != ltTrxSetPara_LTE.wStatus )
		{
			/* ASSERT */
			cm_Assert(	CMD_ASL_USELOW,
						parm_p->cpridat_trxrelreq.carno,
						"m_rc_TrxRel_Transfer_Req: Release is running");

			cm_BFree(rsp_p);
			return;
		}
		ltTrxSetPara_LTE.wStatus = E_RRH_TRXSTA_RELING;			/* TRX解放中状態 */
		/* TRX設定パラメータ(3G)設定 (TrxRelease is running) */
		f_cmn_com_trxset_para_lte_set(parm_p->cpridat_trxrelreq.carno, &ltTrxSetPara_LTE);

		lwTimerId = CMD_TIMID_TRXREL_LTEMIN + lwCarrNo - CMD_NUM1;
		/* 待ちフラグ(TRX解放応答)がONに設定します */
		gw_rcw_WaitResFlag_trxrel_lte[lwCarrNo - CMD_NUM1] = D_L3_ON;
	}

	/* 正常の場合、バッファ解放 */
	cm_BFree(rsp_p);

	/* タイマ起動TRX解放確認タイマ */
	cm_TStat(lwTimerId, CMD_TIMVAL_TRX_REL, CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);

	/* Requirement No.17 disable send TRX release to RE */
	/*  CPRIリンク数だけループループ */
	// for(lwCnt = CMD_NUM0; lwCnt < D_RRH_CPRINO_RE_MAX; lwCnt++)
	// {
	// 	/* Layer3状態が運用中状態みの場合 */
	// 	if(D_L3_STA_4 == ltLayer3Sta.layer3_re[lwCnt])
	// 	{
	// 		parm_p->cprircv_inf.link_num = lwCnt + CMD_NUM1;
	// 		/* l3/rctからの正常メッセージをre/recに送信する(TRX設定要求) */
	// 		l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, sizeof( CMT_TSKIF_CPRIRCV_TRXRELREQ ) );
			
	// 		if( CMD_SYS_3G == sigsys_rslt )
	// 		{
	// 			/* テーブル更新TRX解放送信先←送信した配下RE */
	// 			gw_rcw_trxrel_req_flg_3g_tbl[lwCnt][lwCarrNo - CMD_NUM1] = D_L3_ON;
	// 			/* テーブル初期化TRX解放応答元 */
	// 			gw_rcw_trxrel_res_flg_3g_tbl[lwCnt][lwCarrNo - CMD_NUM1] = D_L3_OFF;
	// 			/* テーブル初期化TRX解放応答結果 */
	// 			gw_rcw_trxrel_res_3g_tbl[lwCnt][lwCarrNo - CMD_NUM1] = 0xFF;
	// 		}
	// 		else
	// 		{
	// 			/* テーブル更新TRX解放送信先←送信した配下RE */
	// 			gw_rcw_trxrel_req_flg_lte_tbl[lwCnt][lwCarrNo - CMD_NUM1] = D_L3_ON;
	// 			/* テーブル初期化TRX解放応答元 */
	// 			gw_rcw_trxrel_res_flg_lte_tbl[lwCnt][lwCarrNo - CMD_NUM1] = D_L3_OFF;
	// 			/* テーブル初期化TRX解放応答結果 */
	// 			gw_rcw_trxrel_res_lte_tbl[lwCnt][lwCarrNo - CMD_NUM1] = 0xFF;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		if( CMD_SYS_3G == sigsys_rslt )
	// 		{
	// 			gw_rcw_trxrel_req_flg_3g_tbl[lwCnt][lwCarrNo - CMD_NUM1] = D_L3_OFF;
	// 			gw_rcw_trxrel_res_flg_3g_tbl[lwCnt][lwCarrNo - CMD_NUM1] = D_L3_OFF;
	// 		}
	// 		else
	// 		{
	// 			gw_rcw_trxrel_req_flg_lte_tbl[lwCnt][lwCarrNo - CMD_NUM1] = D_L3_OFF;
	// 			gw_rcw_trxrel_res_flg_lte_tbl[lwCnt][lwCarrNo - CMD_NUM1] = D_L3_OFF;
	// 		}
	// 	}
	// }

	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  rctスレッドのTRX解放確認タイマ タイムアウト
 *  @note 
 *  @param  awTimerId [in] TimerId
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_TrxRel_Trans_TO( USHORT awTimerId)
{
	UINT	lwSysType = 0xFF;						/*  Systerm typel */
	USHORT	lwCarNo;								/* キャリア番号 */

	/* 3G の場合 */
	if( (CMD_TIMID_TRXREL_3GMIN <= awTimerId) && (awTimerId <= CMD_TIMID_TRXREL_3GMAX ) )
	{
		lwSysType = CMD_SYS_3G;
		/* キャリア番号設定 */
		lwCarNo = awTimerId - CMD_TIMID_TRXREL_3GMIN + CMD_NUM1;
		/* 配下REからのTRX解放応答待ち(rctスレッドのTRX解放確認タイマ動作中)以外の場合 */
		if( D_L3_ON != gw_rcw_WaitResFlag_trxrel_3g[lwCarNo - 1] )
		{
			/* ASSERT */
			cm_Assert(	CMD_ASL_USELOW,
					0,
					"m_rc_TrxRel_Trans_TO Not wait response");
			return;
		}
	}
	else
	{
		lwSysType = CMD_SYS_LTE;
		/* キャリア番号設定 */
		lwCarNo = awTimerId - CMD_TIMID_TRXREL_LTEMIN + CMD_NUM1;
		/* 配下REからのTRX解放応答待ち(rctスレッドのTRX解放確認タイマ動作中)以外の場合 */
		if( D_L3_ON != gw_rcw_WaitResFlag_trxrel_lte[lwCarNo - 1] )
		{
			/* ASSERT */
			cm_Assert(	CMD_ASL_USELOW,
					0,
					"m_rc_TrxRel_Trans_TO Not wait response");
			return;
		}
	}

	/* l3/rctからの正常メッセージをRECに送信する(TRX解放応答) */
	m_rc_TrxRel_Snd_Rsp( lwCarNo, lwSysType );

	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Send TRX解放応答 to REC
 *  @param  awCarrNo    [in ] キャリア番号
 *  @param  awSysKind   [in ] 3G/LTE
 *  @return None
 *  @date 
 *  @date   2015/11/11 FPT)Yen Fix bug IT3 No70
 *  @date   2015/12/16 TDIPS) enoki 複数キャリアでのTRX解放にてUSELEDが消灯しない
 */
/********************************************************************************************************************/

VOID m_rc_TrxRel_Snd_Rsp( USHORT awCarrNo, USHORT awSysKind )
{
	CMT_TSKIF_CPRISND_TRXRELRES	*rsp_p;				/* タスク間フレームポインタ		*/
	UINT						lwRslt;							/* バッファ獲得結果	*/
	USHORT						lwReCnt;
	T_TRXSET_PARA_3G			ltTrxSetPara_3G;
	T_TRXSET_PARA_LTE			ltTrxSetPara_LTE;

	/* ポインタ初期化 */
	rsp_p = NULL;
	/* バッファ獲得 */
	lwRslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_TRXRELRES),
								CMD_NUM1,
								(VOID **)&rsp_p	);
	/* バッファ獲得失敗 */
	if(lwRslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					lwRslt,
					"m_rc_TrxRel_Trans_TO cm_L2BfGet NG");
		return;
	}

	/* 3G の場合 */
	if( CMD_SYS_3G == awSysKind )
	{
		for(lwReCnt = 0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++)
		{
			/* 1台以上の配下REから応答結果「0:正常」を受信の場合 */
			if(CMD_NML == gw_rcw_trxrel_res_3g_tbl[lwReCnt][awCarrNo - CMD_NUM1])
			{
				/* l3/rctからの正常メッセージをRECに送信する(TRX解放応答(0:正常)) */
				lwRslt = CMD_NML;
				break;
			}
		}
		if( D_RRH_CPRINO_RE_MAX == lwReCnt )
		{
			/* その他エラー(RE)(42) */
			lwRslt = CMD_OTHR_ERR_RE;
		}
		/* TRX設定パラメータ(3G)取得 */
		f_cmn_com_trxset_para_3g_get(awCarrNo, &ltTrxSetPara_3G);
		/* TRX設定/TRX解放状態 -> アイドル状態 */
		ltTrxSetPara_3G.wStatus = E_RRH_TRXSTA_IDLE;
		if( CMD_NML == lwRslt )
		{
			/* normal case, clear share memory data */
			memset(&ltTrxSetPara_3G, 0x0, sizeof(ltTrxSetPara_3G));

			/* Turn off USE bit of FhmBase */
			rrhApi_Svp_Mnt_SvRegSet(D_RRH_PROCQUE_L3, D_RRH_REG_CNT_SV1INF, D_RRH_REG_CNT_SV1INF_3G_USE_01 << (awCarrNo - 1), 0);
		}
		/* TRX設定パラメータ(3G)設定 */
		f_cmn_com_trxset_para_3g_set(awCarrNo, &ltTrxSetPara_3G );
	}
	else
	{
		for(lwReCnt = 0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++)
		{
			/* 1台以上の配下REから応答結果「0:正常」を受信の場合 */
			if(CMD_NML == gw_rcw_trxrel_res_lte_tbl[lwReCnt][awCarrNo - 1])
			{
				/* l3/rctからの正常メッセージをRECに送信する(TRX解放応答(0:正常)) */
				lwRslt = CMD_NML;
				break;
			}
		}
		if( D_RRH_CPRINO_RE_MAX == lwReCnt )
		{
			/* 処理NG(RE)(51) */
			lwRslt = CMD_HDL_NG;
		}
		/* TRX設定パラメータ(S3G)取得 */
		f_cmn_com_trxset_para_lte_get(awCarrNo, &ltTrxSetPara_LTE);
		/* TRX設定/TRX解放状態 -> アイドル状態 */
		ltTrxSetPara_LTE.wStatus = E_RRH_TRXSTA_IDLE;
		if( CMD_NML == lwRslt )
		{
			/* normal case, clear share memory data */
			memset(&ltTrxSetPara_LTE, 0x0, sizeof(ltTrxSetPara_LTE));

			/* Turn off USE bit of FhmBase */
		    rrhApi_Svp_Mnt_SvRegSet(D_RRH_PROCQUE_L3, D_RRH_REG_CNT_SV1INF, D_RRH_REG_CNT_SV1INF_S3G_USE_01 << (awCarrNo - 1), 0);
		}
		/* TRX設定パラメータ(S3G)設定 */
		f_cmn_com_trxset_para_lte_set(awCarrNo, &ltTrxSetPara_LTE );
	}
	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_TRXRELRES);

	/* キャリア番号設定 */
	rsp_p->cpridat_trxrelres.carno = awCarrNo;
	
	/* 信号種別設定 */
	rsp_p->cpridat_trxrelres.signal_kind = CMD_CPRID_TRXRELRES + awSysKind;

	/* CPRI信号応答結果設定*/
	rsp_p->cpridat_trxrelres.result = lwRslt;

	/* l3/rctからの正常メッセージをRECに送信する(TRX解放応答) */
	m_cm_L3MsgSend_REC( rsp_p );

	/* Clear all data */
	if(awSysKind == CMD_SYS_3G)
	{
		/* 待ちフラグ(TRX解放応答)がOFFに設定します */
		gw_rcw_WaitResFlag_trxrel_3g[awCarrNo - 1] = D_L3_OFF;
		for( lwReCnt = CMD_NUM0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++ )
		{
			/* テーブル更新TRX解放送信先←送信した配下RE */
			gw_rcw_trxrel_req_flg_3g_tbl[lwReCnt][awCarrNo - CMD_NUM1] = D_L3_OFF;
			/* テーブル初期化TRX解放応答元 */
			gw_rcw_trxrel_res_flg_3g_tbl[lwReCnt][awCarrNo - CMD_NUM1] = D_L3_OFF;
			/* テーブル初期化TRX解放応答結果 */
			gw_rcw_trxrel_res_3g_tbl[lwReCnt][awCarrNo - CMD_NUM1] = 0xFF;
		}
	}
	else
	{
		/* 待ちフラグ(TRX解放応答)がOFFに設定します */
		gw_rcw_WaitResFlag_trxrel_lte[awCarrNo - 1] = D_L3_OFF;
		for( lwReCnt = CMD_NUM0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++ )
		{
			/* テーブル更新TRX解放送信先←送信した配下RE */
			gw_rcw_trxrel_req_flg_lte_tbl[lwReCnt][awCarrNo - CMD_NUM1] = D_L3_OFF;
			/* テーブル初期化TRX解放応答元 */
			gw_rcw_trxrel_res_flg_lte_tbl[lwReCnt][awCarrNo - CMD_NUM1] = D_L3_OFF;
			/* テーブル初期化TRX解放応答結果 */
			gw_rcw_trxrel_res_lte_tbl[lwReCnt][awCarrNo - CMD_NUM1] = 0xFF;
		}
	}

	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Receive TRX解放応答 from re/rec in case TRX制御状態 = アイドル
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_TrxRel_Rcv_Rsp( CMT_TSKIF_CPRISND_TRXRELRES* parm_p  )
{
	UINT	sigchk_rslt;						/*  result of checking signal */
	USHORT	sigsys_rslt;					/*  system by checking signal */

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* 信号を有効にしてください。 */
	sigchk_rslt = m_cm_chksig( ((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind, &sigsys_rslt );
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt, 
							"m_rc_TrxRel_Rcv_Rsp validate signal NG");
		return;
	}

	/* 3G信号の場合 */
	if( CMD_SYS_3G == sigsys_rslt )
	{
		m_rc_TrxRel_Rcv_Rsp_3G(parm_p);
	}
	/* LTE信号の場合 */
	else
	{
		m_rc_TrxRel_Rcv_Rsp_LTE(parm_p);
	}
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Receive TRX解放応答 from re/rec in case TRX制御状態 = アイドル
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_TrxRel_Rcv_Rsp_3G( CMT_TSKIF_CPRISND_TRXRELRES* parm_p  )
{
	USHORT		lwCarNo;
	USHORT		lwReNo;
	USHORT		lwReCnt;
	INT			errcd;

	lwReNo = parm_p->cprisnd_inf.link_num;
	lwCarNo = parm_p->cpridat_trxrelres.carno;

	/* 配下REからのTRX解放応答待ち(rctスレッドのTRX解放確認タイマ動作中)以外の場合 */
	if( D_L3_ON != gw_rcw_WaitResFlag_trxrel_3g[lwCarNo - 1] )
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
				0,
				"m_rc_TrxRel_Rcv_Rsp_3G Not wait response");
		return;
	}

	/* テーブル更新TRX解放応答元←受信した配下RE */
	gw_rcw_trxrel_res_flg_3g_tbl[lwReNo - 1][lwCarNo - 1] = D_L3_ON;
	/* テーブル更新TRX解放応答結果←配下REの応答結果 */
	gw_rcw_trxrel_res_3g_tbl[lwReNo - 1][lwCarNo - 1] = parm_p->cpridat_trxrelres.result;

	/* 要求を送信したすべての配下REから応答を受信 の場合 */
	for(lwReCnt = 0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++)
	{
		if(gw_rcw_trxrel_res_flg_3g_tbl[lwReCnt][lwCarNo-1] != gw_rcw_trxrel_req_flg_3g_tbl[lwReCnt][lwCarNo - 1])
		{
			return;
		}
	}
	gw_rcw_WaitResFlag_trxrel_3g[lwCarNo - 1] = D_L3_OFF;

	/* タイマ停止TRX設定確認タイマ */
	cm_TStop(CMD_TIMID_TRXREL_3GMIN + lwCarNo - 1,&errcd);

	/* l3/rctからの正常メッセージをRECに送信する(TRX解放応答) */
	m_rc_TrxRel_Snd_Rsp( lwCarNo, CMD_SYS_3G );

    /* AxC設定 */
    m_cmR_AxC();

    /* 送波・停波設定 */
    m_rc_SetWave();
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Receive TRX解放応答 from re/rec in case TRX制御状態 = アイドル
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_TrxRel_Rcv_Rsp_LTE( CMT_TSKIF_CPRISND_TRXRELRES* parm_p  )
{
	USHORT		lwCarNo;
	USHORT		lwReNo;
	USHORT		lwReCnt;
	INT			errcd;

	lwReNo = parm_p->cprisnd_inf.link_num;
	lwCarNo = parm_p->cpridat_trxrelres.carno;

	/* 配下REからのTRX解放応答待ち(rctスレッドのTRX解放確認タイマ動作中)以外の場合 */
	if( D_L3_ON != gw_rcw_WaitResFlag_trxrel_lte[lwCarNo - 1] )
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
				0,
				"m_rc_TrxRel_Rcv_Rsp_LTE Not wait response");
		return;
	}

	/* テーブル更新TRX解放応答元←受信した配下RE */
	gw_rcw_trxrel_res_flg_lte_tbl[lwReNo - 1][lwCarNo - 1] = D_L3_ON;
	/* テーブル更新TRX解放応答結果←配下REの応答結果 */
	gw_rcw_trxrel_res_lte_tbl[lwReNo - 1][lwCarNo - 1] = parm_p->cpridat_trxrelres.result;

	/* 要求を送信したすべての配下REから応答を受信 の場合 */
	for(lwReCnt = 0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++)
	{
		if(gw_rcw_trxrel_res_flg_lte_tbl[lwReCnt][lwCarNo-1] != gw_rcw_trxrel_req_flg_lte_tbl[lwReCnt][lwCarNo - 1])
		{
			return;
		}
	}

	/* タイマ停止TRX設定確認タイマ */
	cm_TStop(CMD_TIMID_TRXREL_LTEMIN + lwCarNo - 1,&errcd);

	/* 待ちフラグ(TRX解放応答)がOFFに設定します */
	gw_rcw_WaitResFlag_trxrel_lte[lwCarNo - 1] = D_L3_OFF;

	/* l3/rctからの正常メッセージをRECに送信する(TRX解放応答) */
	m_rc_TrxRel_Snd_Rsp( lwCarNo, CMD_SYS_S3G );

    /* AxC設定 */
    m_cmR_AxC();

    /* 送波・停波設定 */
    m_rc_SetWave();
}

/* @} */

/********************************************************************************************************************/
/**
 *  @brief  CPRIリンク断発生時の自律TRX解放
 *  @note
 *  @param  buff_adr [in] pointer of received message
 *  @return None
 *  @date
 */
/********************************************************************************************************************/

VOID m_rc_CPRIdis_TrxRel( VOID* buff_adr )
{
	CMT_TSKIF_CPRIDISCONN_NTC *parm_p;
	parm_p = (CMT_TSKIF_CPRIDISCONN_NTC*)buff_adr;
	T_TRXSET_PARA_3G			ltTrxSetPara_3G;
	T_TRXSET_PARA_LTE			ltTrxSetPara_LTE;
	USHORT lwCarrCnt;

	memset(&ltTrxSetPara_3G, 0x0, sizeof(ltTrxSetPara_3G));
	memset(&ltTrxSetPara_LTE, 0x0, sizeof(ltTrxSetPara_LTE));
	/* REC の場合 */
	if (parm_p->link_num < D_RRH_CPRINO_RE_MIN)
	{
		if(E_RRH_RAYER_KIND_1_2 == parm_p->layer_kind)
		{
			for(lwCarrCnt = 0; lwCarrCnt < CMD_MAX_CAR_3G; lwCarrCnt++)
			{
				/* 設定用 */
				f_cmn_com_trxset_para_3g_set(lwCarrCnt + 1, &ltTrxSetPara_3G);
				rrhApi_Svp_Mnt_SvRegSet(D_RRH_PROCQUE_L3, D_RRH_REG_CNT_SV1INF, D_RRH_REG_CNT_SV1INF_3G_USE_01 << lwCarrCnt, 0);
			}
			for(lwCarrCnt = 0; lwCarrCnt < CMD_MAX_CAR_LTE; lwCarrCnt++)
			{
				/* 設定用 */
				f_cmn_com_trxset_para_lte_set(lwCarrCnt + 1, &ltTrxSetPara_LTE);
				rrhApi_Svp_Mnt_SvRegSet(D_RRH_PROCQUE_L3, D_RRH_REG_CNT_SV1INF, D_RRH_REG_CNT_SV1INF_S3G_USE_01 << lwCarrCnt, 0);
			}
		}
		else
		if(E_RRH_RAYER_KIND_3 == parm_p->layer_kind)
		{
			/* 3G の場合 */
			if(CMD_SYS_3G == parm_p->system_type)
			{
				for(lwCarrCnt = 0; lwCarrCnt < CMD_MAX_CAR_3G; lwCarrCnt++)
				{
					/* 設定用 */
					f_cmn_com_trxset_para_3g_set(lwCarrCnt + 1, &ltTrxSetPara_3G);
					rrhApi_Svp_Mnt_SvRegSet(D_RRH_PROCQUE_L3, D_RRH_REG_CNT_SV1INF, D_RRH_REG_CNT_SV1INF_3G_USE_01 << lwCarrCnt, 0);
				}
			}
			/* LTE信号の場合 */
			else
			{
				for(lwCarrCnt = 0; lwCarrCnt < CMD_MAX_CAR_LTE; lwCarrCnt++)
				{
					/* 設定用 */
					f_cmn_com_trxset_para_lte_set(lwCarrCnt + 1, &ltTrxSetPara_LTE);
					rrhApi_Svp_Mnt_SvRegSet(D_RRH_PROCQUE_L3, D_RRH_REG_CNT_SV1INF, D_RRH_REG_CNT_SV1INF_S3G_USE_01 << lwCarrCnt, 0);
				}
			}
		}
	}

    /* AxC設定 */
    m_cmR_AxC();

    /* 送波・停波設定 */
    m_rc_SetWave();
}

/* @} */
