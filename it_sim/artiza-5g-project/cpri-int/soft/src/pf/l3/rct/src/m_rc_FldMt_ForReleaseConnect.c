/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_FldMt_ForReleaseConnect.c
 *  @brief  FLD-MT接続強制解放処理
 *  @date   2015/07/20
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-42)
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited
 */
/********************************************************************************************************************/

/** @addtogroup L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"
#include "m_rc_header.h"

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief FLD-MT接続強制解放要求受信処理
 *  @note
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/20 FPT)Son
 *  @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-42)
 */
/********************************************************************************************************************/

VOID m_rc_Fld_Mt_ForReleaseConnectReq( CMT_TSKIF_CPRIRCV_FLMTCMPRELREQ* parm_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif

	UINT		lwSigchk_rslt;						/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	INT			errcd;
	INT			ldwResult;
	USHORT		lwTimerId;
	USHORT		lwLoop;
	T_RRH_LAYER3	ltLayer3Sts;

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_flmtcmprelreq.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]m_cm_chksig: %d \n",__LINE__,lwSigchk_rslt);
	#endif
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");

		if (lwSigsys_rslt == CMD_SYS_3G) {
			/* FLD-MT接続強制解放応答送信処理 */
			m_rc_Fld_Mt_ForReleaseRspSnd(CMD_SET_NG, lwSigsys_rslt);
			return;
		}
	}

// 	/* 3G system*/
// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]lwSigsys_rslt: %d \n",__LINE__,lwSigsys_rslt);
// 	#endif
// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_FLD_MT_CONN_REL_FORCE_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_FLD_MT_CONN_REL_FORCE_LTE;
// 	}
// 	/* タイマ停止FLD-MT接続強制解放確認タイマ */
// 	cm_TStop( lwTimerId, &errcd);

// 	/* タイマ起動FLD-MT接続強制解放確認タイマ */
// 	ldwResult = cm_TStat(lwTimerId, CMD_TIMVAL_FLDMT_CONN_REL_FORCE, CMD_TIMMD_SINGLE,
// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
// 	/*Start timer NG*/
// 	if(ldwResult != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "SV-MT/eNB-MT接続強制解放確認タイマ Start NG" );
// 		return;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]cm_TStat(TimerId: %d) \n",__LINE__,lwTimerId);
// #endif

//	/* リトライ実施回数を初期化 */
//	g_FldMt_retry_counter[lwSigsys_rslt] = D_RETRY_COUNTER_INIT;

	/* Get layer3 status*/
	(VOID)f_cmn_com_layer3_get(lwSigsys_rslt, &ltLayer3Sts);
	/* 全てのREをループする (16 RE)*/
	for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		/* 対RE Layer3状態が「②RE起動中状態」または「④運用中状態」の場合 */
		if(( D_L3_STA_2 == ltLayer3Sts.layer3_re[ lwLoop ] ) || ( D_L3_STA_4 == ltLayer3Sts.layer3_re[ lwLoop ] ))
		{
			/*Set link num*/
			parm_p->cprircv_inf.link_num = lwLoop + 1;
			/* FLD-MT接続強制解放要求をre/recに送信する */
			ldwResult = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, parm_p, sizeof( CMT_TSKIF_CPRIRCV_FLMTCMPRELREQ ) );
			/*Sending success*/
			if( D_RRH_OK == ldwResult)
			{
				/* テーブル更新FLD-MT接続強制解放送信先←送信した配下RE */
				#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]lwSigsys_rslt: %d - lwLoop: %d \n",__LINE__,lwSigsys_rslt,lwLoop);
				#endif
				gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[lwSigsys_rslt][lwLoop] = D_L3_ON;
			}
		}
	}

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief 配下REからのFLD-MT接続強制解放応答受信処理
 *  @note
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/20 FPT)Son
 *  @date   2015/11/08 TDIPS)Takeuchi 技説QA120
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-42)
 */
/********************************************************************************************************************/

VOID m_rc_Fld_Mt_ForReleaseConnectRes( CMT_TSKIF_CPRIRCV_FLMTCMPRELRES* parm_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif

	UINT		lwSigchk_rslt;						/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	USHORT		lwReNo;
	USHORT		lwTimerId;
	USHORT		lwRcvAllResFlag;
	INT			errcd;

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_flmtcmprelres.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]m_cm_chksig: %d \n",__LINE__,lwSigchk_rslt);
	#endif
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");
		
		if (lwSigsys_rslt == CMD_SYS_3G) {
			/*FLD-MT接続強制解放応答送信処理*/
			m_rc_Fld_Mt_ForReleaseRspSnd( CMD_SET_NG, lwSigsys_rslt );
			return;
		}
	}

	// /* 3G system*/
	// #ifdef FHM_RCT_DEBUG_FOR_IT1
	// printf( "[%d]lwSigsys_rslt: %d \n",__LINE__,lwSigsys_rslt);
	// #endif
	// if( CMD_SYS_3G == lwSigsys_rslt)
	// {
	// 	lwTimerId = CMD_TIMID_FLD_MT_CONN_REL_FORCE_3G;
	// }
	// /* LTE system*/
	// else
	// {
	// 	lwTimerId = CMD_TIMID_FLD_MT_CONN_REL_FORCE_LTE;
	// }

	/* Check CPRI番号 */
	if(( parm_p->cprircv_inf.link_num > D_RRH_CPRINO_RE_MAX ) ||
	   ( parm_p->cprircv_inf.link_num < D_RRH_CPRINO_RE_MIN ))
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							CMD_NUM1,
							"CPRI番号 is invalid");
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	lwReNo = parm_p->cprircv_inf.link_num - 1;

	/* 配下REからの応答待ち中の場合 */
	if( gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[lwSigsys_rslt][lwReNo] == D_L3_ON )
	{
		/* 応答結果が「0:正常」の場合 */
		if( CMD_RES_OK == parm_p->cpridat_flmtcmprelres.result )
		{
			/* テーブル更新FLD-MT接続強制解放応答元←受信した配下RE */
			gw_rcw_Fld_Mt_ForRelConn_ResSrcRcvSts[lwSigsys_rslt][lwReNo] = D_L3_ON;
			#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d]lwSigsys_rslt: %d - lwReNo:%d \n",__LINE__,lwSigsys_rslt,lwReNo);
			#endif
			/* 要求を送信したすべての配下REから正常応答を受信チェック*/
			lwRcvAllResFlag = m_rc_Fld_Mt_ForReleaseConnectResRcvAllChk( lwSigsys_rslt );
			/* 要求を送信したすべての配下REから正常応答を受信場合*/
			if( CMD_TRUE == lwRcvAllResFlag )
			{
				// /* タイマ停止FLD-MT接続強制解放確認タイマ */
				// cm_TStop( lwTimerId, &errcd );
				/*FLD-MT接続強制解放応答送信処理*/
				m_rc_Fld_Mt_ForReleaseRspSnd( CMD_NML, lwSigsys_rslt );
				// /*FLD-MT接続状態←未接続*/
				// gw_rcw_FldMt_ConnSts[lwSigsys_rslt] = D_L3_MT_NOT_CONNECT;
				// /* 再送回数を初期化 */
				// g_FldMt_retry_counter[lwSigsys_rslt] = D_RETRY_COUNTER_INIT;
				// #ifdef FHM_RCT_DEBUG_FOR_IT1
				// printf( "[%d]lwSigsys_rslt: %d - gw_rcw_FldMt_ConnSts[lwSigsys_rslt]:%d \n",__LINE__,lwSigsys_rslt,gw_rcw_FldMt_ConnSts[lwSigsys_rslt]);
				// #endif
			}
		}
	}

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \nSignal kind : %d\nLink num : %d\nResult : %d\n",__LINE__,__FUNCTION__,parm_p->cpridat_flmtcmprelres.signal_kind,parm_p->cprircv_inf.link_num,parm_p->cpridat_flmtcmprelres.result);
	#endif
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief 要求を送信したすべての配下REから正常応答を受信チェック
 *  @note
 *  @param  adw_Sys_type [in] system type
 *  @return None
 *  @date	2015/07/20 FPT)Son
 */
/********************************************************************************************************************/

USHORT m_rc_Fld_Mt_ForReleaseConnectResRcvAllChk( UINT adw_Sys_type)
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif

	USHORT	lwLoop;
	/* 全てのREをループする (16 RE)*/
	for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		/* FLD-MT接続強制解放送信先が送信した場合*/
		if( D_L3_ON == gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[adw_Sys_type][lwLoop] )
		{
			/* 更新FLD-MT接続強制解放応答元が受信しない場合*/
			if( D_L3_OFF == gw_rcw_Fld_Mt_ForRelConn_ResSrcRcvSts[adw_Sys_type][lwLoop] )
			{
				#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]adw_Sys_type: %d - lwLoop:%d \n",__LINE__,adw_Sys_type,lwLoop);
				#endif
				#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
				#endif
				return CMD_FALSE;
			}
		}

	}
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return CMD_TRUE;
}
/* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief  FLD-MT接続強制解放確認タイマ タイムアウト処理
//  *  @note
//  *  @param  awTimerId [in] タイマ ID
//  *  @return None
//  *  @date	2015/07/20 FPT)Son
//  *  @date	2015/09/30 FPT)Tuan fix bug IT2
//  *  @date   2015/11/08 TDIPS)Takeuchi 技説QA120
//  *  @date   2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-42)
//  */
// /********************************************************************************************************************/

// VOID m_rc_Fld_Mt_ForReleaseConnectTO( USHORT awTimerId )
// {
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s start(awTimerId:%d) \n",__LINE__,__FUNCTION__,awTimerId);
// #endif

// 	USHORT								lwSystemType;
// 	USHORT								lwLoop;
// 	USHORT								lwRcvAllResFlag;
// 	INT									errcd;
// 	CMT_TSKIF_CPRIRCV_FLMTCMPRELREQ		ltReq;
// 	INT									ldwRet;
// 	T_RRH_LAYER3						ltLayer3Sts;

// 	/*タイマIdが3G タイマの場合*/
// 	if( CMD_TIMID_FLD_MT_CONN_REL_FORCE_3G == awTimerId )
// 	{
// 		lwSystemType = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		lwSystemType = CMD_SYS_LTE;
// 	}

// 	lwRcvAllResFlag = m_rc_Fld_Mt_ForReleaseConnectResRcvAllChk( lwSystemType );
// 	/* 要求を送信したすべての配下REから正常応答を受信した場合*/
// 	if( CMD_TRUE == lwRcvAllResFlag )
// 	{
// 		/*FLD-MT接続強制解放応答送信処理*/
// 		m_rc_Fld_Mt_ForReleaseRspSnd( CMD_NML, lwSystemType );
// 		/*FLD-MT接続状態←未接続*/
// 		gw_rcw_FldMt_ConnSts[lwSystemType] = D_L3_MT_NOT_CONNECT;
// 		/* リトライ実施回数を初期化 */
// 		g_FldMt_retry_counter[lwSystemType] = D_RETRY_COUNTER_INIT;
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s lwRcvAllResFlag: %d \n",__LINE__,__FUNCTION__,lwRcvAllResFlag);
// 	printf( "[%d]%s gw_rcw_FldMt_ConnSts[%d]: %d \n",__LINE__,__FUNCTION__,lwSystemType,gw_rcw_FldMt_ConnSts[lwSystemType]);
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 		return;
// 	}

// 	/* リトライ実施回数が3回以上の場合 */
// 	if( D_RETRY_COUNTER_MAX <= g_FldMt_retry_counter[lwSystemType] )
// 	{
// 		/*FLD-MT接続強制解放応答送信処理*/
// 		m_rc_Fld_Mt_ForReleaseRspSnd( CMD_ALLRE_NG, lwSystemType );
// 		/*FLD-MT接続状態←未接続*/
// 		gw_rcw_FldMt_ConnSts[lwSystemType] = D_L3_MT_NOT_CONNECT;
// 		/* リトライ実施回数を初期化 */
// 		g_FldMt_retry_counter[lwSystemType] = D_RETRY_COUNTER_INIT;
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s lwRcvAllResFlag: %d \n",__LINE__,__FUNCTION__,lwRcvAllResFlag);
// 	printf( "[%d]%s gw_rcw_FldMt_ConnSts[%d]: %d \n",__LINE__,__FUNCTION__,lwSystemType,gw_rcw_FldMt_ConnSts[lwSystemType]);
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 		return;
// 	}

// 	/* Get layer3 status*/
// 	(VOID)f_cmn_com_layer3_get(lwSystemType, &ltLayer3Sts);

// 	cm_MemClr(&ltReq, sizeof(ltReq));
// 	/* Create request msg */
// 	ltReq.head.uiSignalkind = CMD_CPRID_FLMTCMPRELREQ + lwSystemType;
// 	ltReq.cpridat_flmtcmprelreq.signal_kind = CMD_CPRID_FLMTCMPRELREQ + lwSystemType;
// 	/* 全てのREをループする (16 RE)*/
// 	for( lwLoop = 0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
// 	{
// 		 /* 要求を送信したが正常応答未受信 かつ 要求を送信できる配下RE */
// 		if(( D_L3_ON == gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[lwSystemType][lwLoop] ) && ( D_L3_OFF == gw_rcw_Fld_Mt_ForRelConn_ResSrcRcvSts[lwSystemType][lwLoop] ))
// 		{
// 			/* 対RE Layer3状態が「②RE起動中状態」または「④運用中状態」の場合 */
// 			if(( D_L3_STA_2 == ltLayer3Sts.layer3_re[ lwLoop ] ) || ( D_L3_STA_4 == ltLayer3Sts.layer3_re[ lwLoop ] ))
// 			{
// 				ltReq.cprircv_inf.link_num = lwLoop + 1;
// 				//FLD-MT接続強制解放要求をre/recに送信する
// 				(VOID)l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  &ltReq, sizeof( CMT_TSKIF_CPRIRCV_FLMTCMPRELREQ ) );
// 			}
// 			else
// 			{
// 				/*テーブル更新FLD-MT接続強制解放送信先←該当REを削除*/
// 				gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[lwSystemType][lwLoop] = D_L3_OFF;
// 			}
// 		}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[%d]:%d, gw_rcw_Fld_Mt_ForRelConn_ResSrcRcvSts[%d]:%d, layer3_re:%d\n",__LINE__,__FUNCTION__,
// 		lwLoop, gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[lwSystemType][lwLoop],
// 		lwLoop, gw_rcw_Fld_Mt_ForRelConn_ResSrcRcvSts[lwSystemType][lwLoop],
// 		ltLayer3Sts.layer3_re[ lwLoop ]);
// #endif
// 	}

// 	/*タイマ起動FLD-MT接続強制解放確認タイマ*/
// 	ldwRet = cm_TStat(awTimerId, CMD_TIMVAL_FLDMT_CONN_REL_FORCE, CMD_TIMMD_SINGLE,
// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
// 	/*Start timer NG*/
// 	if(ldwRet != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, awTimerId, "SV-MT/eNB-MT接続強制解放確認タイマ Start NG" );
// 	}
// 	/* リトライ実施回数をインクリメント */
// 	g_FldMt_retry_counter[lwSystemType]++;

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s lwRcvAllResFlag: %d \n",__LINE__,__FUNCTION__,lwRcvAllResFlag);
// 	printf( "[%d]%s gw_rcw_FldMt_ConnSts[%d]: %d \n",__LINE__,__FUNCTION__,lwSystemType,gw_rcw_FldMt_ConnSts[lwSystemType]);
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 	return;
// }

// /* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief FLD-MT接続強制解放処理のグローバル変数を初期化する。
 *  @param awSystem		system type
 *  @note
 *  @return None
 *  @date	2015/07/20 FPT)Son
 *  @date	2016/03/30 TDI)satou 引数にsystem typeを追加し、指定したsystemのみ初期化するように
 */
/********************************************************************************************************************/

VOID m_rc_Fld_Mt_ForReleaseConnectInit( USHORT awSystem )
{
	USHORT re_idx;

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif

	for (re_idx = 0; re_idx < D_RRH_CPRINO_RE_MAX; re_idx++)
	{
		/* テーブル更新FLD-MT接続強制解放送信先変数を初期化する。*/
		gw_rcw_Fld_Mt_ForRelConn_ReqDstSndSts[awSystem][re_idx] = D_L3_OFF;
		/* 更新FLD-MT接続強制解放応答元 */
		gw_rcw_Fld_Mt_ForRelConn_ResSrcRcvSts[awSystem][re_idx] = D_L3_OFF;
	}

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief FLD-MT接続強制解放応答送信処理
 *  @note
 *  @param  awResult [in] Response result
*   @param  awSystem [in] System type
 *  @return None
 *  @date	2015/07/20 FPT)Son
 *  @date	2016/03/30 TDI)satou m_rc_Fld_Mt_ForReleaseConnectInit()にsystem typeを渡すように
 */
/********************************************************************************************************************/

VOID m_rc_Fld_Mt_ForReleaseRspSnd( USHORT awResult, USHORT awSystem )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start(result:%d, Systerm:%d) \n",__LINE__,__FUNCTION__,awResult, awSystem);
#endif

	CMT_TSKIF_CPRIRCV_FLMTCMPRELRES*	ltRes_p;
	UINT   bufget_rslt;						/* バッファ獲得結果				*/
	/****************/
	/* バッファ獲得 */
	/****************/
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRIRCV_FLMTCMPRELRES),
								CMD_NUM1,
								(VOID **)&ltRes_p );
	/********************/
	/* バッファ獲得失敗 */
	/********************/
	if(bufget_rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					bufget_rslt,
					"m_rc_RePortCnt cm_L2BfGet NG");
		return;
	}

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	ltRes_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)
									+	sizeof(CMT_CPRIF_FLMTCMPRELRES);

	/* CPRI信号応答結果にOKを設定 */
	ltRes_p->cpridat_flmtcmprelres.result = awResult;

	/* CPRI信号種別設定 */
	ltRes_p->cpridat_flmtcmprelres.signal_kind = CMD_CPRID_FLMTCMPRELRES + awSystem;

	/*FLD-MT接続強制解放応答(0:正常)をl2/lpbに送信する*/
	m_cm_L3MsgSend_REC( ltRes_p );

	/* FLD-MT接続強制解放処理のグローバル変数を初期化する。*/
	m_rc_Fld_Mt_ForReleaseConnectInit( awSystem );

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
}
/* @} */

/* @} */
