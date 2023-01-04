/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_SvMt_Connect.c
 *  @brief  SV-MT/eNB-MT接続要求受信
 *  @date   2015/07/16 Son)FPT
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-43)
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
#include "f_sys_inc.h"

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  SV-MT/eNB-MT接続要求処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2015/07/16 Son)FPT
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-43)
 */
/********************************************************************************************************************/

VOID m_rc_SvMt_ConnectReq( CMT_TSKIF_CPRIRCV_SVMTCNTREQ* parm_p )
{
	UINT		sigchk_rslt;						/*  result of checking signal */
	USHORT		lwsigsys_rslt;						/*  system by checking signal */
	UINT		ldwRet;								/*  result of checking start timer */
	USHORT		lwTimerId;							/*  Timer ID 						*/
	int			errcd;
	USHORT		lwTimerValue;
	USHORT		lwLinkNo;
	CMT_TSKIF_CPRIRCV_SVMTCNTREQ* msg_p;
	UINT   		bufget_rslt;							/* バッファ獲得結果				*/
	UINT		keyID;
	USHORT		lwRspResult[CMD_MAX_SYSNUM] = { CMD_MTCNT_DISABLE,		 /* 231 : MT接続不可 (3G) */
												CMD_MTCNT_DISABLE_LTE }; /* 101 : MT接続不可 (LTE) */

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}
	
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRIRCV_SVMTCNTREQ),
								CMD_NUM1,
								(VOID **)&msg_p	);
	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)msg_p)->uiKeyID;

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

	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_svmtcntreq.signal_kind,&lwsigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							lwsigsys_rslt, 
							"validate signal NG");

		if (CMD_SYS_3G == lwsigsys_rslt) {
			/* 応答送信(MT接続不可)(L3->re/rec) - 232 : REC応答NG */
			m_rc_SvMt_ConnectResSnd(CMD_RECRS_NG, lwsigsys_rslt, parm_p->cprircv_inf.link_num );
			return;
		}
	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwsigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_SVeNB_MT_CONN_CONF_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_SVeNB_MT_CONN_CONF_LTE;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s gw_rcw_SvEnbMt_ConnSts[%d] = %d \n",__LINE__,__FUNCTION__, lwsigsys_rslt, gw_rcw_SvEnbMt_ConnSts[lwsigsys_rslt]);
// #endif		
// 	/* SV-MT/eNB-MT接続状態が接続の場合 */
// 	if(gw_rcw_SvEnbMt_ConnSts[lwsigsys_rslt] == D_L3_MT_CONNECT )
// 	{
// 		/* SV-MT/eNB-MT接続先が要求のCPRI番号と違う場合 */
// 		if(gw_rcw_SvEnbMt_ConnDst[lwsigsys_rslt] != parm_p->cprircv_inf.link_num )
// 		{
// 			/* 応答送信(MT接続不可)(L3->re/rec) */
// 			m_rc_SvMt_ConnectResSnd( lwRspResult[lwsigsys_rslt], lwsigsys_rslt, parm_p->cprircv_inf.link_num );
// 			/* 処理終了 */
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 			return;
// 		}
// 	}

	lwLinkNo = parm_p->cprircv_inf.link_num;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s lwLinkNo: %d \n",__LINE__,__FUNCTION__, parm_p->cprircv_inf.link_num);
#endif	
	
	memcpy(msg_p, parm_p, sizeof(CMT_TSKIF_CPRIRCV_SVMTCNTREQ));
	((T_SYS_COMMON_THDIF*)msg_p)->uiKeyID = keyID;

	/*SV-MT/eNB-MT接続要求をl2/lpbに送信する */
	ldwRet = m_cm_L3MsgSend_REC(msg_p);
	/*Sending fail*/
	if( CMD_L3_OK != ldwRet)
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwRet, "Send request NG" );
		return;
	}

// 	/*SV-MT/eNB-MT接続先設定--> 当該配下RE */
// 	gw_rcw_SvEnbMt_ConnDst[lwsigsys_rslt] = lwLinkNo ;

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s gw_rcw_SvEnbMt_ConnDst[%d] = %d \n",__LINE__,__FUNCTION__, lwsigsys_rslt, gw_rcw_SvEnbMt_ConnDst[lwsigsys_rslt]);
// #endif	
// 	/* タイマ起動(SV-MT/eNB-MT接続確認タイマ) */
// 	lwTimerValue = cmw_sys_mng_tbl[lwsigsys_rslt].svmt_cttimer;

// 	ldwRet = cm_TStat(lwTimerId, TIMVAL_GET(lwTimerValue), CMD_TIMMD_SINGLE,
// 						CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT,&errcd);
// 	if(ldwRet != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "SV-MT/eNB-MT接続強制解放確認タイマ Start NG" );
// 		return;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s start timer (timerId:%d, timerValue:%d) \n",__LINE__,__FUNCTION__,lwTimerId, TIMVAL_GET(lwTimerValue));
// #endif
// 	/* SV-MT/eNB-MT接続応答待ちフラグ設定-->ON */
// 	gw_rcw_SvMt_Conn_WaitFlg[lwsigsys_rslt] =  D_L3_ON ;

#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s gw_rcw_SvMt_Conn_WaitFlg[%d] = %d \n",__LINE__,__FUNCTION__,lwsigsys_rslt, gw_rcw_SvMt_Conn_WaitFlg[lwsigsys_rslt]);
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__ );
#endif	

}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  SV-MT/eNB-MT接続応答処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/16 FPT) Son create
 *  @date	2015/09/29 FPT) Yen fix bug IT2
 *  @date   2015/10/07 TDI)satou [FHM技説-QA No.50] 応答結果はRECから受信したものを透過するように
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-43)
 */
/********************************************************************************************************************/

VOID m_rc_SvMt_ConnectResp( CMT_TSKIF_CPRIRCV_SVMTCNTRES* parm_p )
{
	UINT		sigchk_rslt;						/*  result of checking signal */
	USHORT		lwsigsys_rslt;						/*  system by checking signal */
	USHORT		lwTimerId;							/* 	Timer ID 				  */
	UINT		ldwResult; 
	int 		errcd;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

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
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_svmtcntres.signal_kind,&lwsigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							lwsigsys_rslt, 
							"validate signal NG");
	}

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s gw_rcw_SvMt_Conn_WaitFlg[%d]= %d \n",__LINE__,__FUNCTION__, lwsigsys_rslt, gw_rcw_SvMt_Conn_WaitFlg[lwsigsys_rslt]);
// #endif
// 	/* SV-MT/eNB-MT接続応答待ちフラグがOFFの場合 */
// 	if(gw_rcw_SvMt_Conn_WaitFlg[lwsigsys_rslt] ==  D_L3_OFF )
// 	{
// 		cm_Assert(	CMD_ASL_USELOW,
// 							lwsigsys_rslt, 
// 							"SV-MT/eNB-MT接続応答待ちフラグがOFFの場合 NG");
// 		return;
// 	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwsigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_SVeNB_MT_CONN_CONF_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_SVeNB_MT_CONN_CONF_LTE;
// 	}
	
// 	/* タイマ停止(SV-MT/eNB-MT接続確認タイマ) */
// 	ldwResult = cm_TStop( lwTimerId, &errcd);
// 	/* Stop timer NG */
// 	if( ldwResult != CMD_RES_OK )
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "SV-MT/eNB-MT接続強制解放確認タイマ Stop NG" );
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s Stop timer (timerId:%d)\n",__LINE__,__FUNCTION__,lwTimerId);
// 	printf( "[%d]%s Response result= %d\n",__LINE__,__FUNCTION__,parm_p->cpridat_svmtcntres.result);
// #endif

	/*  RECからの応答結果が「0:正常」の場合 */
	if(parm_p->cpridat_svmtcntres.result == CMD_RES_OK)	
	{
		/* SV-MT/eNB-MT接続状態設定-->接続中 */
		gw_rcw_SvEnbMt_ConnSts[lwsigsys_rslt] = D_L3_MT_CONNECT ;
		/*SV-MT/eNB-MT接続応答(0：正常)をrec/reに送信する */
		m_rc_SvMt_ConnectResSnd(parm_p->cpridat_svmtcntres.result,lwsigsys_rslt, gw_rcw_SvEnbMt_ConnDst[lwsigsys_rslt] );
		
	}
	/* RECからの応答結果が「0:正常」以外場合 */
	else
	{
		/*SV-MT/eNB-MT接続応答(REC応答NG)をrec/reに送信する */
		m_rc_SvMt_ConnectResSnd(parm_p->cpridat_svmtcntres.result, lwsigsys_rslt, gw_rcw_SvEnbMt_ConnDst[lwsigsys_rslt]);
	}
	
// 	/*SV-MT/eNB-MT接続応答待ちフラグ設定-->OFF */
// 	gw_rcw_SvMt_Conn_WaitFlg[lwsigsys_rslt] =  D_L3_OFF;
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end gw_rcw_SvMt_Conn_WaitFlg[%d]= %d\n",__LINE__,__FUNCTION__,lwsigsys_rslt, gw_rcw_SvMt_Conn_WaitFlg[lwsigsys_rslt]);
// #endif
}

/* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief SV-MT/eNB-MT接続確認タイマタイムアウト処理
//  *  @note 
//  *  @param  awTimerId [in] timer id
//  *  @return None
//  *  @date   2015/07/16 Son)FPT
//  *  @date   2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-43)
//  */
// /********************************************************************************************************************/

// VOID m_rc_SvMt_ConnectTo( USHORT awTimerId )
// {
// 	USHORT		lwSystemType;
// 	USHORT		lwRspResult[CMD_MAX_SYSNUM] = { CMD_RECRS_NG,		  /* 232 : REC応答NG (3G) */
// 												CMD_REC_RSP_NG_LTE }; /* 102 : REC応答NG (LTE) */
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
// #endif
// 	/*タイマIdが３G タイマの場合*/
// 	if( CMD_TIMID_SVeNB_MT_CONN_CONF_3G == awTimerId )
// 	{
// 		lwSystemType = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		lwSystemType = CMD_SYS_LTE;
// 	}

// 	/* SV-MT/eNB-MT接続応答待ちフラグがOFFの場合 */
// 	if(gw_rcw_SvMt_Conn_WaitFlg[lwSystemType] ==  D_L3_OFF )
// 	{
// 		cm_Assert(	CMD_ASL_USELOW,
// 					lwSystemType,
// 					"SV-MT/eNB-MT接続応答待ちフラグがOFFの場合 NG");
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end  gw_rcw_SvMt_Conn_WaitFlg[%d]= %d \n",__LINE__,__FUNCTION__, lwSystemType, gw_rcw_SvMt_Conn_WaitFlg[lwSystemType]);
// #endif
// 		return;
// 	}

// 	/*SV-MT/eNB-MT接続応答(REC応答NG)をrec/reに送信する */
// 	m_rc_SvMt_ConnectResSnd(lwRspResult[lwSystemType], lwSystemType, gw_rcw_SvEnbMt_ConnDst[lwSystemType]);

// 	/*SV-MT/eNB-MT接続応答待ちフラグ設定-->OFF */
// 	gw_rcw_SvMt_Conn_WaitFlg[lwSystemType] =  D_L3_OFF;

// 	return;
// }


/********************************************************************************************************************/
/**
 *  @brief  SV-MT/eNB-MT接続応答送信処理.
 *  @note 
 *  @param  awResult [in] Response result
 *  @param  awSystem [in] System type
 *  @param  awCpriLink [in] Re number
 *  @return None
 *  @date	2015/07/16 FPT) Son create
 *  @date	2015/09/29 FPT) Yen fix bug IT2
 */
/********************************************************************************************************************/

VOID m_rc_SvMt_ConnectResSnd( USHORT awResult, USHORT awSystem, USHORT awCpriLink )
{
	CMT_TSKIF_CPRIRCV_SVMTCNTRES ltRes;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start(Result:%d, Systerm:%d, CpriNo:%d) \n",__LINE__,__FUNCTION__, awResult, awSystem, awCpriLink);
#endif

	cm_MemClr(&ltRes, sizeof(ltRes));
	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	ltRes.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)
									+	sizeof(CMT_CPRIF_SVMTCNTRES);

	/* CPRI信号応答結果にOKを設定 */
	ltRes.cpridat_svmtcntres.result = awResult;

	/* CPRI信号種別設定 */
	ltRes.head.uiSignalkind = CMD_CPRID_SVMTCNTRES + awSystem;
	ltRes.cpridat_svmtcntres.signal_kind = CMD_CPRID_SVMTCNTRES + awSystem;

	/* Set cpri link*/
	ltRes.cprircv_inf.link_num = awCpriLink;

	/*ファイルデータ送信完了報告応答を送信*/
	l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, &ltRes, sizeof(CMT_TSKIF_CPRIRCV_SVMTCNTRES) );
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;
}
/* @} */


/* @} */

