/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_TrxSet.c
 *  @brief  RE TRX set process
 *  @date   2015/04/27 FFCS)Zhangxq TDD SRE
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 20015
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
#include "f_rrh_reg_cnt.h"
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief     RE TRX set parameter check for S3G
 *  @note     RE TRX set parameter check for S3G
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   RETURN CODE
 *  @retval    CMD_OK							0: 	Normal end
 *  @retval    CMD_NG							1: 	input para NG
 *  @retval    CMD_SYSPARA_CHECKNG				2: 	input para overange system para
 *  @retval    CMD_S3G_BW_CHECKNG				4:	system para NG
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2009/03/04 FFCS)linlj (M-S3G-eNBPF-01389) [IT3]LRE1縺?2101)TRX設定マクロ発行でALM
 *  @date     2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) and CR-00054-005(CPRI spec V1.0.8) 
 *  @date     2009/09/08 FFCS)Tangj modify for M-S3G-eNBPF-02442, change the NG code
 *  @date     2010/04/27 FJT)Otani	 CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *  @date     2011/03/09 FJT)Koshida modify for 1.5G-RRE対応
 *  @date     2011/05/25 FJT)Tokunaga modify for
 *							  CeNB-F-073-017(DCM)(F-1210(DCM):1.5G-RREにおけるTRX設定応答について)110421.doc
 *  @date     2012/09/10 FFCS)Xut Modify for 1chip for 1.7G/700M   
 *  @date     2013/02/18 CMS)Andou XXXXXXXX TRX設定応答のコード変更
 *  @date   2015/04/27 FFCS)Zhangxq TDD SRE
 */
/********************************************************************************************************************/

UINT m_rc_TrxSetChk_S3G(CMT_TSKIF_CPRIRCV_TRXSET_S3G_REQ *parm_p)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	CHAR log_data[CMD_NUM64];				/* アサート情報収集用エリア		*/
	USHORT	lwFrqnoMax = CMD_NUM0;
	USHORT	lwFrqnoMin = CMD_NUM0;
	T_RRH_EQP_S3G               ltReEqpInfBufLte;				/*Buffer to get equipment information of each subordinate RE*/
	
	/* 初期化*/
	memset(log_data, 0x0, sizeof(log_data));

	/* キャリア番号範囲チェック */
	/* 1以外の場合*/
	if( parm_p->cpridat_trxsetreq.carno < CMD_MIN_CARNUM || parm_p->cpridat_trxsetreq.carno > CMD_MAX_CARNUM_S3G )
	{
		/* ログデータに要因をコピー */
		sprintf(	log_data, 
					"ParmCHK_S3G NG %d,%d,%d,%d,%d,%d,%d",
					parm_p->cpridat_trxsetreq.carno,
					parm_p->cpridat_trxsetreq.bandwidth,
					parm_p->cpridat_trxsetreq.dl_freq,
					parm_p->cpridat_trxsetreq.snd_br,
					parm_p->cpridat_trxsetreq.tdd_up_dwn_config,
					parm_p->cpridat_trxsetreq.tdd_spe_sf_config,
					parm_p->cpridat_trxsetreq.tdd_dwnlk_cp_len);


		/* アサート情報出力 */
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0, 
					log_data);
#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return CMD_NG;
	}

	/* bandwidth check and get */
	/* 1-4bandwidth以外の場合*/
	if((parm_p->cpridat_trxsetreq.bandwidth >  CMD_NUM4) ||
		(parm_p->cpridat_trxsetreq.bandwidth <  CMD_NUM1))
	{
		/* ログデータに要因をコピー */
		sprintf(	log_data, 
					"ParmCHK_S3G bd NG %d,%d,%d,%d,%d,%d",
					parm_p->cpridat_trxsetreq.bandwidth,
					parm_p->cpridat_trxsetreq.dl_freq,
					parm_p->cpridat_trxsetreq.snd_br,
					parm_p->cpridat_trxsetreq.tdd_up_dwn_config,
					parm_p->cpridat_trxsetreq.tdd_spe_sf_config,
					parm_p->cpridat_trxsetreq.tdd_dwnlk_cp_len);
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0, 
					log_data);

#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return CMD_NG;
	}

	/* 送信系 */
	/* TBD Q&A */
	if( ( parm_p->cpridat_trxsetreq.snd_br < CMD_NUM1 ) ||
		( parm_p->cpridat_trxsetreq.snd_br > CMD_NUM15 ) )
	{
		/* ログデータに要因をコピー */
		sprintf(	log_data, 
					"ParmCHK_S3G bd NG %d,%d,%d,%d,%d,%d",
					parm_p->cpridat_trxsetreq.bandwidth,
					parm_p->cpridat_trxsetreq.dl_freq,
					parm_p->cpridat_trxsetreq.snd_br,
					parm_p->cpridat_trxsetreq.tdd_up_dwn_config,
					parm_p->cpridat_trxsetreq.tdd_spe_sf_config,
					parm_p->cpridat_trxsetreq.tdd_dwnlk_cp_len);
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0, 
					log_data);

#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return CMD_NG;
	}

	/* 共有メモリ(LTE)装置構成情報の呼び出し 取得用 */
	f_cmn_com_booteqp_s3g_get( &ltReEqpInfBufLte  );

	switch (ltReEqpInfBufLte.eqp_inf[16])
	{
		/**************************/
		/* 対応周波数帯が2Gの場合*/
		/**************************/
		case CMD_CRP_FRQ_2G_S3G:
			lwFrqnoMax = CMD_FRQ2G_LTE_MAX;
			lwFrqnoMin = CMD_FRQ2G_LTE_MIN;
			break;
		/**************************/
		/* 対応周波数帯が1.7Gの場合*/
		/**************************/
		case CMD_CRP_FRQ_17G_S3G:
			lwFrqnoMax = CMD_FRQ17G_LTE_MAX;
			lwFrqnoMin = CMD_FRQ17G_LTE_MIN;
			break;
		/**************************/
		/* 対応周波数帯が800Hzの場合*/
		/**************************/
		case CMD_CRP_FRQ_800M_S3G:
			lwFrqnoMax = CMD_FRQ800M_LTE_MAX;
			lwFrqnoMin = CMD_FRQ800M_LTE_MIN;
			break;
		/**************************/
		/* 対応周波数帯が1.5GHz の場合*/
		/**************************/
		case CMD_CRP_FRQ_15G_S3G:
			lwFrqnoMax = CMD_FRQ15G_LTE_MAX;
			lwFrqnoMin = CMD_FRQ15G_LTE_MIN;
			break;
		/**************************/
		/* 対応周波数帯 700(S3G報告値)の場合*/
		/**************************/
		case CMD_CRP_FRQ_700M_S3G:
			lwFrqnoMax = CMD_FRQ700M_LTE_MAX;
			lwFrqnoMin = CMD_FRQ700M_LTE_MIN;
			break;
		/**************************/
		/* 対応周波数帯 3.5G(S3G報告値)の場合*/
		/**************************/
		case CMD_CRP_FRQ_35G_S3G:
			lwFrqnoMax = CMD_FRQ35G_LTE_MAX;
			lwFrqnoMin = CMD_FRQ35G_LTE_MIN;
			break;
	}

	/* 周波数番号チェック */
	if( parm_p->cpridat_trxsetreq.dl_freq < lwFrqnoMin || parm_p->cpridat_trxsetreq.dl_freq > lwFrqnoMax )
	{
		/* ログデータに要因をコピー */
		sprintf(	log_data, 
					"ParmCHK_S3G NG %d,%d",
					parm_p->cpridat_trxsetreq.carno,
					parm_p->cpridat_trxsetreq.dl_freq);


		/* アサート情報出力 */
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0, 
					log_data);

#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s eqp_inf[16]:%d \n",__LINE__,__FUNCTION__,ltReEqpInfBufLte.eqp_inf[16]);
		printf( "[%d]%s dl_freq:%d, Min:%d, Max:%d \n",__LINE__,__FUNCTION__, parm_p->cpridat_trxsetreq.dl_freq, lwFrqnoMin, lwFrqnoMax);
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return CMD_NG;
	}

	/* 下り周波数番号が3.5GHz帯の場合 */
	if(CMD_CRP_FRQ_35G_S3G == ltReEqpInfBufLte.eqp_inf[16])
	{
		/* TDD uplink/downlink configuration  0～6 */
		if( parm_p->cpridat_trxsetreq.tdd_up_dwn_config < CMD_UPDWN_LNK_MIN || parm_p->cpridat_trxsetreq.tdd_up_dwn_config > CMD_UPDWN_LNK_MAX )
		{
			/* ログデータに要因をコピー */
			sprintf(	log_data, 
						"ParmCHK_S3G NG %d,%d",
						parm_p->cpridat_trxsetreq.carno,
						parm_p->cpridat_trxsetreq.tdd_up_dwn_config);


			/* アサート情報出力 */
			cm_Assert(	CMD_ASL_USELOW,
						CMD_NUM0, 
						log_data);

#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
			return CMD_NG;
		}
		/* TDD special subframe configuration 0～9 */
		if( parm_p->cpridat_trxsetreq.tdd_spe_sf_config < CMD_SPE_SF_MIN || parm_p->cpridat_trxsetreq.tdd_spe_sf_config > CMD_SPE_SF_MAX )
		{
			/* ログデータに要因をコピー */
			sprintf(	log_data, 
						"ParmCHK_S3G NG %d,%d",
						parm_p->cpridat_trxsetreq.carno,
						parm_p->cpridat_trxsetreq.tdd_spe_sf_config);


			/* アサート情報出力 */
			cm_Assert(	CMD_ASL_USELOW,
						CMD_NUM0, 
						log_data);

#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
			return CMD_NG;
		}
		/* TDD downlink cyclic prefix length 0～1 */
		if( parm_p->cpridat_trxsetreq.tdd_dwnlk_cp_len < CMD_DWNLK_CP_MIN || parm_p->cpridat_trxsetreq.tdd_dwnlk_cp_len > CMD_DWNLK_CP_MAX )
		{
			/* ログデータに要因をコピー */
			sprintf(	log_data, 
						"ParmCHK_S3G NG %d,%d",
						parm_p->cpridat_trxsetreq.carno,
						parm_p->cpridat_trxsetreq.tdd_dwnlk_cp_len);


			/* アサート情報出力 */
			cm_Assert(	CMD_ASL_USELOW,
						CMD_NUM0, 
						log_data);

#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
			return CMD_NG;
		}
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return CMD_OK;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Check TRX設定要求_3G parameter
 *  @note 	Check 3G parameter
 *  @param  parm_3g [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

USHORT m_rc_TrxSetChk_3G( CMT_TSKIF_CPRIRCV_TRXSETREQ *parm_3g )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	CHAR 			log_data[CMD_NUM64];				/* アサート情報収集用エリア		*/
	USHORT			lwFrqnoMax = 0;
	USHORT			lwFrqnoMin = 0;
	T_RRH_EQP_3G	ltReEqpInfBuf3g;					/* Buffer to get equipment information of each subordinate RE*/

	memset(log_data, 0x0, sizeof(log_data));
	/* キャリア番号範囲チェック (1～4) */
	if( parm_3g->cpridat_trxsetreq.carno < CMD_MIN_CARNUM || parm_3g->cpridat_trxsetreq.carno > CMD_MAX_CARNUM_3G )
	{
		/* ログデータに要因をコピー */
		sprintf(	log_data, 
					"ParmCHK_3G NG %d,%d",
					parm_3g->cpridat_trxsetreq.carno,
					parm_3g->cpridat_trxsetreq.frqno);


		/* アサート情報出力 */
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0, 
					log_data);

#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return CMD_NG;
	}

	/* 共有メモリ(3G)装置構成情報の呼び出し 取得用 */
	f_cmn_com_booteqp_3g_get( &ltReEqpInfBuf3g );
	switch (ltReEqpInfBuf3g.eqp_inf[14])
	{
		/**************************/
		/* 対応周波数帯が2Gの場合*/
		/**************************/
		case CMD_CRP_FRQ_2G_3G:
			lwFrqnoMax = CMD_FRQ2G_MAX;
			lwFrqnoMin = CMD_FRQ2G_MIN;
			break;
		/**************************/
		/* 対応周波数帯が1.7Gの場合*/
		/**************************/
		case CMD_CRP_FRQ_17G_3G:
			lwFrqnoMax = CMD_FRQ17G_MAX;
			lwFrqnoMin = CMD_FRQ17G_MIN;
			break;
		/**************************/
		/* 対応周波数帯が800Hzの場合*/
		/**************************/
		case CMD_CRP_FRQ_800M_3G:
			lwFrqnoMax = CMD_FRQ800M_1_MAX;
			lwFrqnoMin = CMD_FRQ800M_1_MIN;
			break;
		/**************************/
		/* 対応周波数帯が1.5GHz の場合*/
		/**************************/
		case CMD_CRP_FRQ_15G_3G:
			lwFrqnoMax = CMD_FRQ15G_MAX;
			lwFrqnoMin = CMD_FRQ15G_MIN;
			break;
	}
	
	/* 周波数番号チェック */
	switch (ltReEqpInfBuf3g.eqp_inf[14])
	{
		case CMD_CRP_FRQ_2G_3G:
		case CMD_CRP_FRQ_17G_3G:
		case CMD_CRP_FRQ_15G_3G:
			if( parm_3g->cpridat_trxsetreq.frqno < lwFrqnoMin || parm_3g->cpridat_trxsetreq.frqno > lwFrqnoMax )
			{
				/* ログデータに要因をコピー */
				sprintf(	log_data, 
							"ParmCHK_3G NG %d,%d",
							parm_3g->cpridat_trxsetreq.carno,
							parm_3g->cpridat_trxsetreq.frqno);


				/* アサート情報出力 */
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0, 
							log_data);

#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
				return CMD_NG;
			}
			break;
		/* RE が800Hz 帯の場合 */
		case CMD_CRP_FRQ_800M_3G:
			/* 値の範囲：4162～4188、812、837 */
			if(( parm_3g->cpridat_trxsetreq.frqno < lwFrqnoMin || parm_3g->cpridat_trxsetreq.frqno > lwFrqnoMax ) &&
				( parm_3g->cpridat_trxsetreq.frqno != CMD_FRQ800M_3 ) &&
				( parm_3g->cpridat_trxsetreq.frqno != CMD_FRQ800M_4 ))
			{
				/* ログデータに要因をコピー */
				sprintf(	log_data, 
							"ParmCHK_3G NG %d,%d",
							parm_3g->cpridat_trxsetreq.carno,
							parm_3g->cpridat_trxsetreq.frqno);


				/* アサート情報出力 */
				cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM0, 
							log_data);

#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
				return CMD_NG;
			}
			break;
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return CMD_OK;

}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RE TRX set process
 *  @note RE TRX set process
 *  @param  atParm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 *  @date 2015/10/08 FPT)Dung Update response result (BD update)
 *  @date 2015/11/08 FPT)Yen update (normal router doesn't make response message)
 *  @date 2015/11/11 FPT)Yen Fix bug IT3 No70
 *  @date 2016/03/18 TDI)satou 送信対象REが存在しない場合、直ちに42:その他エラー(RE)をRECに通知するように
 *  @date 2021/02/02 M&C)Huan.dh Intermediate device 3G reponse NG
 */
/********************************************************************************************************************/

VOID m_rc_TrxSet_Transfer_Req_3G( CMT_TSKIF_CPRIRCV_TRXSETREQ *atParm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	printf( "[%d]%s signal_kind:0x%x, Carrier no:%d, frqno:%d \n",__LINE__,__FUNCTION__,
		atParm_p->cpridat_trxsetreq.signal_kind, atParm_p->cpridat_trxsetreq.carno, atParm_p->cpridat_trxsetreq.frqno);
#endif

	CMT_TSKIF_CPRISND_TRXSETRES	*ltRsp_p;		/* タスク間フレームポインタ		*/
	UINT						lwRslt;			/* バッファ獲得結果				*/
	// INT							errcd;
	// UINT						lwCnt;
	// T_RRH_LAYER3				ltLayer3Sta;	/* Layer3状態 */
	// T_TRXSET_PARA_3G			ltTrxSetPara;
	// USHORT						lwTimerId;
	// USHORT						lwResult = CMD_NML;
	// CHAR 			log_data[CMD_NUM64];				/* アサート情報収集用エリア		*/

	/* ポインタ初期化 */
	ltRsp_p = NULL;

	/* キャリア番号範囲チェック (1～4) */
	// if( atParm_p->cpridat_trxsetreq.carno < CMD_MIN_CARNUM || atParm_p->cpridat_trxsetreq.carno > CMD_MAX_CARNUM_3G )
	// {
	// 	/* ログデータに要因をコピー */
	// 	sprintf(	log_data, "ParmCHK_3G NG CARNO=%d", atParm_p->cpridat_trxsetreq.carno);

	// 	/* アサート情報出力 */
	// 	cm_Assert(	CMD_ASL_USELOW, atParm_p->cpridat_trxsetreq.carno, log_data);
		
	 	/* バッファ獲得 */
	// 	lwRslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
	// 							sizeof(CMT_TSKIF_CPRISND_TRXSETRES),
	// 							CMD_NUM1,
	// 							(VOID **)&ltRsp_p	);
	// 	/* バッファ獲得失敗 */
	// 	if(lwRslt != CMD_RES_OK)
	// 	{
	// 		/* ASSERT */
	// 		cm_Assert(	CMD_ASL_USELOW,
	// 					lwRslt,
	// 					"m_rc_TrxSet_Transfer_Req_3G cm_L2BfGet NG");
	// 		return;
	// 	}

		/* CMT_TSKIF_CPRISND_TRXSETRES */
		
		/**********************/
		/* タスク間ヘッダ作成 */
		/**********************/
		ltRsp_p->head.uiLength = 	sizeof(CMT_TSKIF_HEAD) + 
								sizeof(CMT_CPRISIGSND_INF) + 
								sizeof(CMT_CPRIF_TRXSETRES);
		/* CPRI信号応答結果にOK/42:その他エラー(RE)を設定*/
		ltRsp_p->cpridat_trxsetres.result      = CMD_OTHR_ERR_RE;

		/* CPRI信号応答内容(キャリア番号)設定 */
		ltRsp_p->cpridat_trxsetres.carno       = atParm_p->cpridat_trxsetreq.carno;
		
		/* CPRI信号応答内容(CPRI信号種別)設定 */
		ltRsp_p->cpridat_trxsetres.signal_kind = CMD_CPRID_TRXSETRES;

		/* l3/rctからのメッセージをRECに送信する(TRX設定応答) */
		m_cm_L3MsgSend_REC(ltRsp_p);
		
		/* 終了 */
		return;
	//}
	// /* テーブル参照:TRX設定 */
	// /* 共有メモリ(3G)TRX設定パラメータテーブルの呼び出し 取得用 */
	// f_cmn_com_trxset_para_3g_get(atParm_p->cpridat_trxsetreq.carno, &ltTrxSetPara);

	// /* 共用メモリLayer3の呼び出し 取得用 */
	// f_cmn_com_layer3_get(CMD_SYS_3G, &ltLayer3Sta);

	// /* 送信するREが存在しない場合はNG応答(42:その他エラー(RE)) */
	// for(lwCnt = CMD_NUM0; lwCnt < D_RRH_CPRINO_RE_MAX; lwCnt++)
	// {
	// 	if( E_RRH_LAYER3_RE_OPE == ltLayer3Sta.layer3_re[lwCnt] )
	// 	{
	// 		break;
	// 	}
	// }
	// if (D_RRH_CPRINO_RE_MAX == lwCnt)
	// {
	// 	lwResult = CMD_OTHR_ERR_RE;
	// }

	// /* 指定キャリアについてTRX設定済み、または送信対象のREが存在しない場合 */
	// if(( CMD_NUM0 != ltTrxSetPara.wSigKind ) || ( CMD_NML != lwResult ))
	// {
	// 	/* バッファ獲得 */
	// 	lwRslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
	// 							sizeof(CMT_TSKIF_CPRISND_TRXSETRES),
	// 							CMD_NUM1,
	// 							(VOID **)&ltRsp_p	);
	// 	/* バッファ獲得失敗 */
	// 	if(lwRslt != CMD_RES_OK)
	// 	{
	// 		/* ASSERT */
	// 		cm_Assert(	CMD_ASL_USELOW,
	// 					lwRslt,
	// 					"m_rc_TrxSet_Transfer_Req_3G cm_L2BfGet NG");
	// 		return;
	// 	}

	// 	/* CMT_TSKIF_CPRISND_TRXSETRES */
		
	// 	/**********************/
	// 	/* タスク間ヘッダ作成 */
	// 	/**********************/
	// 	ltRsp_p->head.uiLength = 	sizeof(CMT_TSKIF_HEAD) + 
	// 							sizeof(CMT_CPRISIGSND_INF) + 
	// 							sizeof(CMT_CPRIF_TRXSETRES);
	// 	/* CPRI信号応答結果にOK/42:その他エラー(RE)を設定*/
	// 	ltRsp_p->cpridat_trxsetres.result      = lwResult;

	// 	/* CPRI信号応答内容(キャリア番号)設定 */
	// 	ltRsp_p->cpridat_trxsetres.carno       = atParm_p->cpridat_trxsetreq.carno;
		
	// 	/* CPRI信号応答内容(CPRI信号種別)設定 */
	// 	ltRsp_p->cpridat_trxsetres.signal_kind = CMD_CPRID_TRXSETRES;

	// 	/* l3/rctからのメッセージをRECに送信する(TRX設定応答) */
	// 	m_cm_L3MsgSend_REC(ltRsp_p);
	//#ifdef FHM_RCT_DEBUG_FOR_IT1
	//		printf( "[%d]%s end (ltTrxSetPara.wSigKind : %d)\n",__LINE__,__FUNCTION__, ltTrxSetPara.wSigKind);
	//#endif
	// /* 終了 */
	// return;
	// 	}

	// 	/* TrxSet is running */
	// 	if( E_RRH_TRXSTA_IDLE != ltTrxSetPara.wStatus )
	// 	{
	// 		/* ASSERT */
	// 		cm_Assert(	CMD_ASL_USELOW,
	// 					atParm_p->cpridat_trxsetreq.carno,
	// 					"m_rc_TrxSet_Transfer_Req_3G: TrxSet is running");

	// 		return;
	// 	}

	// 	memset( &ltTrxSetPara, 0, sizeof(ltTrxSetPara) );
	// 	ltTrxSetPara.wStatus = E_RRH_TRXSTA_SETING;				 /* TRX設定中 */
	// 	/* TRX設定パラメータ(3G)設定 (TrxSet is running) */
	// 	f_cmn_com_trxset_para_3g_set(atParm_p->cpridat_trxsetreq.carno, &ltTrxSetPara);

	// 	/* テーブル更新TRX設定(一時保存用) */
	// 	memcpy(&gt_rcw_trxset_save_3g_tmp[atParm_p->cpridat_trxsetreq.carno - 1], &atParm_p->cpridat_trxsetreq, sizeof(CMT_CPRIF_TRXSETREQ));

	// 	lwTimerId = CMD_TIMID_TRXSET_3GMIN + atParm_p->cpridat_trxsetreq.carno - 1;
	// 	/* タイマ起動TRX設定確認タイマ */
	// 	cm_TStat(lwTimerId, CMD_TIMVAL_TRX_SET, CMD_TIMMD_SINGLE,
	// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);

	// 	/*  CPRIリンク数だけループループ */
	// 	for(lwCnt = CMD_NUM0; lwCnt < D_RRH_CPRINO_RE_MAX; lwCnt++)
	// 	{
	// 		/* Layer3状態が運用中状態みの場合 */
	// 		if( E_RRH_LAYER3_RE_OPE == ltLayer3Sta.layer3_re[lwCnt] )
	// 		{
	// 			atParm_p->cprircv_inf.link_num = lwCnt + 1;
	// 			/* l3/rctからTRX設定要求をre/recに送信する */
	// 			l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  atParm_p, sizeof( CMT_TSKIF_CPRIRCV_TRXSETREQ ) );
	// 			/* テーブル更新TRX設定送信先←送信した配下RE */
	// 			gw_rcw_trxset_req_flg_3g_tbl[lwCnt][atParm_p->cpridat_trxsetreq.carno - 1] = D_L3_ON;
	// 			/* テーブル更新TRX設定応答元 */
	// 			gw_rcw_trxset_res_flg_3g_tbl[lwCnt][atParm_p->cpridat_trxsetreq.carno - 1] = D_L3_OFF;
	// 		}
	// 		else
	// 		{
	// 			gw_rcw_trxset_req_flg_3g_tbl[lwCnt][atParm_p->cpridat_trxsetreq.carno - 1] = D_L3_OFF;
	// 			gw_rcw_trxset_res_flg_3g_tbl[lwCnt][atParm_p->cpridat_trxsetreq.carno - 1] = D_L3_OFF;
	// 		}
	// 		/* テーブル初期化TRX設定応答結果 */
	// 		gw_rcw_trxset_res_3g_tbl[lwCnt][atParm_p->cpridat_trxsetreq.carno - 1] = 0xFF;
	// 	}

	// #ifdef FHM_RCT_DEBUG_FOR_IT1
	// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	// #endif
	// 	return;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RE TRX set process
 *  @note 	RE TRX set process
 *  @param  atParm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2015/10/08 FPT)Dung Update response result (BD update)
 *  @date   2015/10/29 FPT)Duongcd Fix bug IT3 No27
 *  @date   2015/11/08 FPT)Yen update (normal router doesn't make response message)
 *  @date   2015/11/11 FPT)Yen Fix bug IT3 No70
 *  @date   2016/03/18 TDI)satou 送信対象REが存在しない場合、直ちに51:処理NG(RE)をRECに通知するように
 */
/********************************************************************************************************************/

VOID m_rc_TrxSet_Transfer_Req_LTE( VOID *atParm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	CMT_TSKIF_CPRISND_TRXSETRES	*rsp_p;		/* タスク間フレームポインタ		*/
	UINT						lwRslt;		/* バッファ獲得結果				*/
	INT							errcd;
	UINT						lwCnt;
	T_RRH_LAYER3				ltLayer3Sta;	/* Layer3状態 */
	T_TRXSET_PARA_LTE			ltTrxSetPara;
	USHORT						lwTimerId;
	CMT_TSKIF_CPRIRCV_TRXSET_S3G_FDD_REQ *l_cpritrxset_p = NULL;
	USHORT 						lwCarrNo;
	USHORT 						lwMsgSize;
	USHORT						lwResult = CMD_NML;
	CHAR 			log_data[CMD_NUM64];				/* アサート情報収集用エリア		*/

	/* ポインタ初期化 */
	rsp_p = NULL;

	l_cpritrxset_p = (CMT_TSKIF_CPRIRCV_TRXSET_S3G_FDD_REQ*)atParm_p;
	lwCarrNo = l_cpritrxset_p->cpridat_trxsetreq.carno;
	/* 1～2以外の場合*/
	if( l_cpritrxset_p->cpridat_trxsetreq.carno < CMD_MIN_CARNUM || l_cpritrxset_p->cpridat_trxsetreq.carno > CMD_MAX_CARNUM_S3G )
	{
		/* ログデータに要因をコピー */
		sprintf(	log_data, "ParmCHK_S3G NG CARNO=%d", l_cpritrxset_p->cpridat_trxsetreq.carno);

		/* アサート情報出力 */
		cm_Assert(	CMD_ASL_USELOW, l_cpritrxset_p->cpridat_trxsetreq.carno, log_data);
		
		/* バッファ獲得 */
		lwRslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
									sizeof(CMT_TSKIF_CPRISND_TRXSETRES),
									CMD_NUM1,
									(VOID **)&rsp_p	);
		/* バッファ獲得失敗 */
		if(lwRslt != CMD_RES_OK)
		{
			/* ASSERT */
			cm_Assert(	CMD_ASL_USELOW,
						lwRslt,
						"m_rc_TrxSet_Transfer_Req_LTE cm_L2BfGet NG");

			return;
		}
		/**********************/
		/* タスク間ヘッダ作成 */
		/**********************/
		rsp_p->head.uiLength			 = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_TRXSETRES);
		/* CPRI信号応答結果にOK/51:処理NG(RE)を設定*/
		rsp_p->cpridat_trxsetres.result      = CMD_HDL_NG;

		/* CPRI信号応答内容(キャリア番号)設定 */
		rsp_p->cpridat_trxsetres.carno       = lwCarrNo;
		
		/* CPRI信号応答内容(CPRI信号種別)設定 */
		rsp_p->cpridat_trxsetres.signal_kind = CMD_CPRID_TRXSETRES + CMD_SYS_S3G;

		/* l3/rctからのメッセージをRECに送信する(TRX設定応答) */
		m_cm_L3MsgSend_REC(rsp_p);

		/* 終了 */
		return;
	}
	/*	受信データサイズを格納	*/
	lwMsgSize = l_cpritrxset_p->head.uiLength;
	/*	受信データサイズからヘッダサイズを減算し、TRX設定要求メッセージサイズを算出	*/
	gt_rcw_trxset_datSize[lwCarrNo-1] = lwMsgSize - sizeof(CMT_TSKIF_HEAD) - sizeof(CMT_CPRISIGRCV_INF);

	/* テーブル参照:TRX設定 */
	/* 共用メモリシスパラ格納領域(LTE)の呼び出し 取得用 */
	f_cmn_com_trxset_para_lte_get(lwCarrNo, &ltTrxSetPara);

	/* 共用メモリLayer3の呼び出し 取得用 */
	f_cmn_com_layer3_get(CMD_SYS_S3G, &ltLayer3Sta);

	for(lwCnt = 0; lwCnt < D_RRH_CPRINO_RE_MAX; lwCnt++)
	{
		if( E_RRH_LAYER3_RE_OPE == ltLayer3Sta.layer3_re[lwCnt] )
		{
			break;
		}
	}
	if (D_RRH_CPRINO_RE_MAX == lwCnt)
	{
		lwResult = CMD_HDL_NG;
	}

	/* 指定キャリアについてTRX設定済み、または送信対象のREが存在しない場合 */
	if(( CMD_NUM0 != ltTrxSetPara.wSigKind ) || ( CMD_NML != lwResult ))
	{
		/* バッファ獲得 */
		lwRslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
									sizeof(CMT_TSKIF_CPRISND_TRXSETRES),
									CMD_NUM1,
									(VOID **)&rsp_p	);
		/* バッファ獲得失敗 */
		if(lwRslt != CMD_RES_OK)
		{
			/* ASSERT */
			cm_Assert(	CMD_ASL_USELOW,
						lwRslt,
						"m_rc_TrxSet_Transfer_Req_LTE cm_L2BfGet NG");

			return;
		}

		/**********************/
		/* タスク間ヘッダ作成 */
		/**********************/
		rsp_p->head.uiLength			 = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_TRXSETRES);
		/* CPRI信号応答結果にOK/51:処理NG(RE)を設定*/
		rsp_p->cpridat_trxsetres.result      = lwResult;

		/* CPRI信号応答内容(キャリア番号)設定 */
		rsp_p->cpridat_trxsetres.carno       = lwCarrNo;
		
		/* CPRI信号応答内容(CPRI信号種別)設定 */
		rsp_p->cpridat_trxsetres.signal_kind = CMD_CPRID_TRXSETRES + CMD_SYS_S3G;

		/* l3/rctからのメッセージをRECに送信する(TRX設定応答) */
		m_cm_L3MsgSend_REC(rsp_p);

#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end (ltTrxSetPara.wSigKind : %d)\n",__LINE__,__FUNCTION__, ltTrxSetPara.wSigKind);
#endif
		/* 終了 */
		return;
	}

	/* TrxSet is running */
	if( E_RRH_TRXSTA_IDLE != ltTrxSetPara.wStatus )
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					lwCarrNo,
					"m_rc_TrxSet_Transfer_Req_LTE: TrxSet is running");

		return;
	}

	memset( &ltTrxSetPara, 0, sizeof(ltTrxSetPara) );
	ltTrxSetPara.wStatus = E_RRH_TRXSTA_SETING;				  /* TRX設定中 */
	ltTrxSetPara.wdatSize = gt_rcw_trxset_datSize[lwCarrNo-1];	  /* TRX設定要求データサイズ */
	/* TRX設定パラメータ(S3G)設定 (TrxSet is running) */
	f_cmn_com_trxset_para_lte_set(lwCarrNo, &ltTrxSetPara);

	memset(&gt_rcw_trxset_save_lte_tmp[lwCarrNo - 1], 0x0,
		sizeof(gt_rcw_trxset_save_lte_tmp[lwCarrNo - 1]));
	/* テーブル更新TRX設定(一時保存用) */
	memcpy(&gt_rcw_trxset_save_lte_tmp[lwCarrNo - 1],
							&l_cpritrxset_p->cpridat_trxsetreq, gt_rcw_trxset_datSize[lwCarrNo-1]);

	lwTimerId = CMD_TIMID_TRXSET_LTEMIN + lwCarrNo - 1;

	/* タイマ起動TRX設定確認タイマ */
	cm_TStat(lwTimerId, CMD_TIMVAL_TRX_SET, CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s Timer start (lwTimerId : %d)\n",__LINE__,__FUNCTION__, lwTimerId);
#endif

	/*  CPRIリンク数だけループループ */
	for(lwCnt = 0; lwCnt < D_RRH_CPRINO_RE_MAX; lwCnt++)
	{
		/* Layer3状態が運用中状態みの場合 */
		if( E_RRH_LAYER3_RE_OPE == ltLayer3Sta.layer3_re[lwCnt] )
		{
			((CMT_TSKIF_CPRIRCV_TRXSET_S3G_FDD_REQ*)atParm_p)->cprircv_inf.link_num = lwCnt + 1;
			/* l3/rctからの正常メッセージをre/recに送信する(TRX設定要求) */
			l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, atParm_p, lwMsgSize );
			/* テーブル更新TRX設定送信先←送信した配下RE */
			gw_rcw_trxset_req_flg_lte_tbl[lwCnt][lwCarrNo - 1] = D_L3_ON;
			/* テーブル更新TRX設定応答元 */
			gw_rcw_trxset_res_flg_lte_tbl[lwCnt][lwCarrNo - 1] = D_L3_OFF;
		}
		else
		{
			gw_rcw_trxset_req_flg_lte_tbl[lwCnt][lwCarrNo - 1] = D_L3_OFF;
			gw_rcw_trxset_res_flg_lte_tbl[lwCnt][lwCarrNo - 1] = D_L3_OFF;
		}
		/* テーブル初期化TRX設定応答結果 */
		gw_rcw_trxset_res_lte_tbl[lwCnt][lwCarrNo - 1] = 0xFF;
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RE TRX set process
 *  @note RE TRX set process
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_TrxSet_Transfer_Req( VOID *parm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

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
							"m_rc_TrxSet_Transfer_Req validate signal NG");
	}

	/* 3G の場合 */
	if( CMD_SYS_3G == sigsys_rslt )
	{
		m_rc_TrxSet_Transfer_Req_3G((CMT_TSKIF_CPRIRCV_TRXSETREQ *) parm_p);
	}
	/* LTE の場合 */
	else
	{
		m_rc_TrxSet_Transfer_Req_LTE(parm_p);
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif

}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  rctスレッドのTRX設定確認タイマ タイムアウト
 *  @note 
 *  @param  awTimerId [in] TimerId
 *  @return None
 *  @date   2015/07/24 FPT)Dung create new
 *  @date   2015/10/02 FPT)Yen fix bug IT2
 */
/********************************************************************************************************************/

VOID m_rc_TrxSet_Trans_TO( USHORT awTimerId )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	UINT	lwSysType;						/*  Systerm typel */
	USHORT	lwCarrNo;
	USHORT	lwSignalKind;

	if( ( awTimerId < CMD_TIMID_TRXSET_3GMIN ) || ( awTimerId > CMD_TIMID_TRXSET_LTEMAX ) )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/* 3G の場合 */
	if( awTimerId >= CMD_TIMID_TRXSET_3GMIN && awTimerId <= CMD_TIMID_TRXSET_3GMAX )
	{
		lwSysType 		= CMD_SYS_3G;
		lwCarrNo 		= awTimerId - CMD_TIMID_TRXSET_3GMIN + 1;
		lwSignalKind 	= gt_rcw_trxset_save_3g_tmp[lwCarrNo - 1].signal_kind ;
	}
	/* LTE の場合 */
	else
	{
		lwSysType 		= CMD_SYS_S3G;
		lwCarrNo 		= awTimerId - CMD_TIMID_TRXSET_LTEMIN + 1;
		lwSignalKind 	= gt_rcw_trxset_save_lte_tmp[lwCarrNo - 1].signal_kind ;
	}

	/* 配下REからのTRX設定応答待ち(rctスレッドのTRX設定確認タイマ動作中)以外の場合 */
	if( CMD_NUM0 == lwSignalKind )
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					0,
					"m_rc_TrxSet_Trans_TO Not wait response");

#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/* Send TRX設定応答 to REC */
	m_rc_TrxSet_Snd_Rsp( lwCarrNo, lwSysType );

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Receive TRX設定応答(3G) from re/rec
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_TrxSet_Rcv_Rsp_3G( CMT_TSKIF_CPRISND_TRXSETRES *parm_p)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	INT							errcd;
	USHORT						lwReCnt;
	USHORT						lwCarrNo;
	USHORT						lwTimerId;

	/* check Null */
	if( NULL == parm_p )
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					0,
					"m_rc_TrxSet_Rcv_Rsp_3G Buffer Null");
		return;
	}

	/* キャリア番号 */
	lwCarrNo = parm_p->cpridat_trxsetres.carno;

	/* 配下REからのTRX設定応答待ち(rctスレッドのTRX設定確認タイマ動作中)以外の場合 */
	if( CMD_NUM0 == gt_rcw_trxset_save_3g_tmp[lwCarrNo - 1].signal_kind )
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					0,
					"m_rc_TrxSet_Rcv_Rsp_3G Not wait response");

#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/* テーブル更新TRX設定応答元←受信した配下RE */
	gw_rcw_trxset_res_flg_3g_tbl[parm_p->cprisnd_inf.link_num-1][lwCarrNo - 1] = D_L3_ON;

	/* テーブル更新TRX設定応答結果←配下REの応答結果 */
	gw_rcw_trxset_res_3g_tbl[parm_p->cprisnd_inf.link_num-1][lwCarrNo - 1] = parm_p->cpridat_trxsetres.result;


	/* 要求を送信したすべての配下REから応答を受信の場合 */
	for(lwReCnt = 0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++)
	{
		if(gw_rcw_trxset_res_flg_3g_tbl[lwReCnt][lwCarrNo - 1] != gw_rcw_trxset_req_flg_3g_tbl[lwReCnt][lwCarrNo - 1])
		{
#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
			return;
		}
	}

	lwTimerId = CMD_TIMID_TRXSET_3GMIN + lwCarrNo - 1;
	/* タイマ停止TRX設定確認タイマ */
	cm_TStop(lwTimerId, &errcd);

	/* Send TRX設定応答 to REC */
	m_rc_TrxSet_Snd_Rsp( lwCarrNo, CMD_SYS_3G );

    /* AxC設定 */
    m_cmR_AxC();

	/* 送波・停波設定 */
	m_rc_SetWave();

	memset(gw_rcw_trxset_req_flg_3g_tbl, 0x0, sizeof(gw_rcw_trxset_req_flg_3g_tbl));
	memset(gw_rcw_trxset_res_flg_3g_tbl, 0x0, sizeof(gw_rcw_trxset_res_flg_3g_tbl));
	memset(gw_rcw_trxset_res_3g_tbl, 0xFF, sizeof(gw_rcw_trxset_res_3g_tbl));

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Receive TRX設定応答(LTE) from re/rec
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_TrxSet_Rcv_Rsp_LTE( CMT_TSKIF_CPRISND_TRXSETRES *parm_p)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	USHORT						lwReCnt;
	USHORT						lwCarrNo;
	INT							errcd;
	USHORT						lwTimerId;

	/* check Null */
	if( NULL == parm_p )
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					0,
					"m_rc_TrxSet_Rcv_Rsp_LTE Buffer Null");
		return;
	}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s carno:%d \n",__LINE__,__FUNCTION__, parm_p->cpridat_trxsetres.carno);
#endif
	/* キャリア番号 */
	lwCarrNo = parm_p->cpridat_trxsetres.carno;

	/* 配下REからのTRX設定応答待ち(rctスレッドのTRX設定確認タイマ動作中)以外の場合 */
	if( CMD_NUM0 == gt_rcw_trxset_save_lte_tmp[lwCarrNo - 1].signal_kind )
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					0,
					"m_rc_TrxSet_Rcv_Rsp_LTE Not wait response");

#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/* テーブル更新TRX設定応答元←受信した配下RE */
	gw_rcw_trxset_res_flg_lte_tbl[parm_p->cprisnd_inf.link_num-1][lwCarrNo - 1] = D_L3_ON;

	/* テーブル更新TRX設定応答結果←配下REの応答結果 */
	gw_rcw_trxset_res_lte_tbl[parm_p->cprisnd_inf.link_num-1][lwCarrNo - 1] = parm_p->cpridat_trxsetres.result;

	/* 要求を送信したすべての配下REから応答を受信の場合 */
	for(lwReCnt = 0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++)
	{
		if(gw_rcw_trxset_res_flg_lte_tbl[lwReCnt][lwCarrNo - 1] != gw_rcw_trxset_req_flg_lte_tbl[lwReCnt][lwCarrNo - 1])
		{
#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d]%s end (gw_rcw_trxset_res_flg_lte_tbl[%d][%d]:%d, gw_rcw_trxset_req_flg_lte_tbl[%d][%d]:%d) \n",__LINE__,__FUNCTION__,
					lwReCnt, lwCarrNo - 1, gw_rcw_trxset_res_flg_lte_tbl[lwReCnt][lwCarrNo - 1],
					lwReCnt, lwCarrNo - 1, gw_rcw_trxset_req_flg_lte_tbl[lwReCnt][lwCarrNo - 1]);
#endif
			return;
		}
	}

	lwTimerId = CMD_TIMID_TRXSET_LTEMIN + lwCarrNo - 1;
	/* タイマ停止TRX設定確認タイマ */
	cm_TStop(lwTimerId, &errcd);
#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s Timer stop (timerId:%d) \n",__LINE__,__FUNCTION__,lwTimerId);
#endif
	/* Send TRX設定応答 to REC */
	m_rc_TrxSet_Snd_Rsp( lwCarrNo, CMD_SYS_S3G );

    /* AxC設定 */
    m_cmR_AxC();

    /* 送波・停波設定 */
    m_rc_SetWave();

	memset(gw_rcw_trxset_req_flg_lte_tbl, 0x0, sizeof(gw_rcw_trxset_req_flg_lte_tbl));
	memset(gw_rcw_trxset_res_flg_lte_tbl, 0x0, sizeof(gw_rcw_trxset_res_flg_lte_tbl));
	memset(gw_rcw_trxset_res_lte_tbl, 0xFF, sizeof(gw_rcw_trxset_res_lte_tbl));

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Send TRX設定応答 to REC
 *  @param  awCarrNo  [in] carrier number
 *  @param  awSysType [in] system type
 *  @return None
 *  @date   2015/07/24 FPT)Yen create new
 *  @date   2015/11/11 FPT)Yen Fix bug IT3 No70
 */
/********************************************************************************************************************/
VOID	m_rc_TrxSet_Snd_Rsp(USHORT awCarrNo, USHORT awSysType)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start(carrNo:%d, Systype:%d) \n",__LINE__,__FUNCTION__,awCarrNo,awSysType);
#endif

	CMT_TSKIF_CPRISND_TRXSETRES	*ltRsp_p;		/* タスク間フレームポインタ		*/
	UINT						rslt;			/* バッファ獲得結果				*/
	T_TRXSET_PARA_LTE			ltTrxSetLte;
	T_TRXSET_PARA_3G			ltTrxSet3G;
	USHORT						lwReCnt;


	/* ポインタ初期化 */
	ltRsp_p = NULL;
	/* バッファ獲得 */
	rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_TRXSETRES),
								CMD_NUM1,
								(VOID **)&ltRsp_p	);
	/* バッファ獲得失敗 */
	if(rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					rslt,
					"m_rc_TrxSet_Rcv_Rsp_LTE cm_L2BfGet NG");

#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	ltRsp_p->head.uiLength				   = sizeof(CMT_TSKIF_HEAD) + 
											 sizeof(CMT_CPRISIGSND_INF) + 
											 sizeof(CMT_CPRIF_TRXSETRES);
	/* CPRI信号応答結果にOKを設定*/
	ltRsp_p->cpridat_trxsetres.result      = CMD_NML;

	/* CPRI信号応答内容(キャリア番号)設定 */
	ltRsp_p->cpridat_trxsetres.carno       = awCarrNo;
	
	/* CPRI信号応答内容(CPRI信号種別)設定 */
	ltRsp_p->cpridat_trxsetres.signal_kind = CMD_CPRID_TRXSETRES + awSysType;

	/* 3Gの場合 */
	if( CMD_SYS_3G == awSysType )
	{
		memset( &ltTrxSet3G, 0, sizeof(ltTrxSet3G) );
		/* 1台以上の配下REから応答結果「0:正常」を受信の場合  */
		for(lwReCnt = 0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++)
		{
			if(gw_rcw_trxset_res_3g_tbl[lwReCnt][awCarrNo - 1] == CMD_NML)
			{
				/* テーブル更新TRX設定 ← TRX設定(一時保存用) */
				memcpy(&ltTrxSet3G, &gt_rcw_trxset_save_3g_tmp[awCarrNo - 1], sizeof(CMT_CPRIF_TRXSETREQ));
				/* 設定用 */
				f_cmn_com_trxset_para_3g_set(awCarrNo, &ltTrxSet3G);
				/* Turn on USE bit of FhmBase */
				rrhApi_Svp_Mnt_SvRegSet(D_RRH_PROCQUE_L3, D_RRH_REG_CNT_SV1INF, 0, D_RRH_REG_CNT_SV1INF_3G_USE_01 << (awCarrNo - 1));
				/* l3/rctからの正常メッセージをRECに送信する(TRX設定応答(0:正常)) */
				m_cm_L3MsgSend_REC(ltRsp_p);
				/* clear TRX設定(一時保存用) */
				memset(&gt_rcw_trxset_save_3g_tmp[awCarrNo - 1], 0x0, sizeof(gt_rcw_trxset_save_3g_tmp[awCarrNo - 1]));

#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]%s end(lwReCnt:%d) \n",__LINE__,__FUNCTION__,lwReCnt);
#endif
				return;
			}
		}

		f_cmn_com_trxset_para_3g_set(awCarrNo, &ltTrxSet3G);
		/* CPRI信号応答結果にその他エラー(RE)(42)を設定 */
		ltRsp_p->cpridat_trxsetres.result = CMD_OTHR_ERR_RE;
		/* l3/rctからのメッセージをRECに送信する(TRX設定応答) */
		m_cm_L3MsgSend_REC(ltRsp_p);
		/* clear TRX設定(一時保存用) */
		memset(&gt_rcw_trxset_save_3g_tmp[awCarrNo - 1], 0x0, sizeof(gt_rcw_trxset_save_3g_tmp[awCarrNo - 1]));
	}
	/* LTEの場合 */
	else
	{
		memset( &ltTrxSetLte, 0, sizeof(ltTrxSetLte) );
		for(lwReCnt = 0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++)
		{
			if(gw_rcw_trxset_res_lte_tbl[lwReCnt][awCarrNo - 1] == CMD_NML)
			{
				/* テーブル更新TRX設定 ← TRX設定(一時保存用) */
				memcpy(&ltTrxSetLte, &gt_rcw_trxset_save_lte_tmp[awCarrNo - 1], sizeof(CMT_CPRIF_TRXSETREQ_S3G));
				ltTrxSetLte.wdatSize = gt_rcw_trxset_datSize[awCarrNo-1];
				/* 設定用 */
				f_cmn_com_trxset_para_lte_set(awCarrNo, &ltTrxSetLte );
				/* Turn on USE bit of FhmBase */
				rrhApi_Svp_Mnt_SvRegSet(D_RRH_PROCQUE_L3, D_RRH_REG_CNT_SV1INF, 0, D_RRH_REG_CNT_SV1INF_S3G_USE_01 << (awCarrNo - 1));
				/* l3/rctからの正常メッセージをRECに送信する(TRX設定応答(0:正常)) */
				m_cm_L3MsgSend_REC(ltRsp_p);
				/* clear TRX設定(一時保存用) */
				memset(&gt_rcw_trxset_save_lte_tmp[awCarrNo - 1], 0x0, sizeof(gt_rcw_trxset_save_lte_tmp[awCarrNo - 1]));

#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]%s end(lwReCnt:%d) \n",__LINE__,__FUNCTION__,lwReCnt);
#endif
				return;
			}
		}

		f_cmn_com_trxset_para_lte_set(awCarrNo, &ltTrxSetLte );
		/* CPRI信号応答結果に処理NG(RE)(51)を設定 */
		ltRsp_p->cpridat_trxsetres.result = CMD_HDL_NG;

		/* l3/rctからのメッセージをRECに送信する(TRX設定応答) */
		m_cm_L3MsgSend_REC(ltRsp_p);
		/* clear TRX設定(一時保存用) */
		memset(&gt_rcw_trxset_save_lte_tmp[awCarrNo - 1], 0x0, sizeof(gt_rcw_trxset_save_lte_tmp[awCarrNo - 1]));
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Receive TRX設定応答 from re/rec
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01865
 *  @date   2015/09/17 TDIPS)Lay M-RRU-ZSYS-01865 return誤りを修正
 */
/********************************************************************************************************************/

VOID m_rc_TrxSet_Rcv_Rsp( CMT_TSKIF_CPRISND_TRXSETRES *parm_p)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	UINT	sigchk_rslt;					/*  result of checking signal */
	USHORT	sigsys_rslt;					/*  system by checking signal */

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* 信号を有効にしてください。 */
	sigchk_rslt = m_cm_chksig( parm_p->cpridat_trxsetres.signal_kind, &sigsys_rslt );
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
					sigsys_rslt, 
					"m_rc_TrxSet_Rcv_Rsp validate signal NG");
	}
	
	if( CMD_SYS_3G == sigsys_rslt )
	{
		m_rc_TrxSet_Rcv_Rsp_3G( parm_p);
	}
	else
	{
		m_rc_TrxSet_Rcv_Rsp_LTE(parm_p);
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  強制停止
 *  @note 
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

/*VOID m_rc_Force_Stop( )*/
/*{*/


/* 状態テーブル初期化Toffset値報告 */

/* 状態テーブル初期化RE装置構成情報報告 */

/* 状態テーブル初期化REスロット情報報告 */

/* 状態テーブル初期化REカード状態報告 */

/* 状態テーブル初期化REキャリア状態報告 */

/* 状態テーブル初期化RE状態報告 */

/* 状態テーブル初期化RE PORT状態報告 */

/* 状態テーブル初期化TRX制御状態 */

/* カウンタ初期化強制リセット送信再送カウンタ */

/* カウンタ初期化強制リセット再送カウンタ */

/* カウンタ初期化REリセットリトライカウンタ T.B.D */

/* カウンタ初期化Toffset値報告要求再送カウンタ */

/* カウンタ初期化RE装置構成情報報告要求再送カウンタ */

/* カウンタ初期化REスロット情報報告要求再送カウンタ */

/* カウンタ初期化RE状態報告要求再送カウンタ */

/* カウンタ初期化RE PORT状態報告要求再送カウンタ */

/* カウンタ初期化REカード状態報告要求再送カウンタ */

/* カウンタ初期化REキャリア状態報告要求再送カウンタ */

/* 状態初期化遅延補正未実施 */

/* ERR設定強制停止 */

/* RE/RECからのメッセージをL3/CPRに送信する(強制停止通知) */
/* re_com_sendMsg(); */
/* ヘルスチェック停止 */






/*}*/

/* @} */
