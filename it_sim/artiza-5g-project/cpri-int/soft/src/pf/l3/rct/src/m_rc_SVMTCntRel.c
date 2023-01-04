/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_SVMTCntRel.c
 *  @brief  SV-MT/eNB-MT接続解放関数
 *  @date   2015/07/18 FPT)TuanBV
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-46)
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
#include "rrhApi_Svp.h"
#include "m_rc_def.h"
#include "m_rc_ftyp.h"
#include "f_sys_inc.h"

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief	SV-MT/eNB-MT接続解放要求関数
 *  @note 
 *  @param	parm_p [in] the buffer address pointer of received message
 *  @return	None
 *  @date	2015/07/18 FPT)TuanBV
 *	@date	2015/10/20 FPT)HaiLC Fix 対応依頼VN.txt - No. 63.
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-46)
 */
/********************************************************************************************************************/

VOID m_rc_SndSVMTCntRelReq( CMT_TSKIF_CPRIRCV_SVMTCNTRELREQ* parm_p )
{
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	UINT		lwSigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	INT			errcd;
	UINT		lwTimerID;
	UINT		ldwRtc;
	USHORT		lwTimerVal;
//  USHORT      lwReNo;
	CMT_TSKIF_CPRIRCV_SVMTCNTRELREQ* ltSvmtCntRelReq_p;
 	UINT		a_ret;	/* buffer hunt return code */
	UINT 		keyID;
	UINT                ldwL3SndResult;                 /*result of message sending*/

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
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_svmtcntrelreq.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");

		if (lwSigchk_rslt == CMD_SYS_3G) {
			/* SV-MT/eNB-MT接続解放応答 ( l3/rct->re/rec ) */
			m_rc_SVMTCntRelResSnd(CMD_RECRS_NG, lwSigchk_rslt);
			return;
		}
	}

// 	/**get RE number*/
// 	lwReNo = parm_p->cprircv_inf.link_num;

// 	/*内容が初期値で応答先不明(接続要求を受信していないのに解放を受信)の場合は      */
// 	/*or the situation that the connection destination is different from RE request release*/
// 	if((gw_rcw_SvEnbMt_ConnDst[lwSigsys_rslt] != lwReNo) ||
// 		(gw_rcw_SvEnbMt_ConnSts[lwSigsys_rslt] == D_L3_MT_NOT_CONNECT))
// 	{
// 		/*Send message NG to RE*/
// 		CMT_TSKIF_CPRIRCV_SVMTCNTRELRES 	ltMsgRsp;
// 		cm_MemClr(&ltMsgRsp, sizeof(ltMsgRsp));

// 		/* Make msg respone */
// 		ltMsgRsp.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)
// 										+	sizeof(CMT_CPRIF_SVMTCNTRELRES);
// 		/* Signal kind */
// 		ltMsgRsp.head.uiSignalkind = CMD_CPRID_SVMTCNTRELRES + lwSigsys_rslt;
// 		ltMsgRsp.cpridat_svmtcntrelres.signal_kind = CMD_CPRID_SVMTCNTRELRES + lwSigsys_rslt;

// 		/* 応答結果設定 */
// 		ltMsgRsp.cpridat_svmtcntrelres.result = CMD_NML;
		
// 		ltMsgRsp.cprircv_inf.link_num = lwReNo;
		
// 		/* SV-MT/eNB-MT接続解放応答 ( l3/rct->re/rec ) */
// 		ldwL3SndResult = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  &ltMsgRsp, sizeof( CMT_TSKIF_CPRIRCV_SVMTCNTRELRES ) );	
		
// 		if(ldwL3SndResult != D_RRH_OK)
// 		{
// 			cm_Assert(	CMD_ASL_DBGLOW,
// 						lwSigsys_rslt, 
// 						"m_rc_SndSVMTCntRelReq l3 send NG");
// 		}
		
		
// 		cm_Assert(	CMD_ASL_USELOW,
// 					lwSigsys_rslt, 
// 					"m_rc_SndSVMTCntRelReq request release RE NG");
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s gw_rcw_SvEnbMt_ConnSts[%d] = %d \n [%d] \ngw_rcw_SvEnbMt_ConnSts[%d] = %d\n",
// 		__LINE__,__FUNCTION__,lwSigsys_rslt,gw_rcw_SvEnbMt_ConnSts[lwSigsys_rslt],lwReNo,lwSigsys_rslt,gw_rcw_SvEnbMt_ConnSts[lwSigsys_rslt]);
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
		
// 		/*応答を破棄   */
// 		return;
// 	}

	a_ret = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRIRCV_SVMTCNTRELREQ), CMD_NUM1, (void**)&ltSvmtCntRelReq_p);
	if (a_ret  != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_SndSVMTCntRelReq", 
			"buffer hunt fail", a_ret, sizeof(CMT_TSKIF_CPRIRCV_SVMTCNTRELREQ), CMD_NUM0);
	}
	
	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)ltSvmtCntRelReq_p)->uiKeyID;
	
	cm_MemCpy(ltSvmtCntRelReq_p, parm_p,  sizeof(CMT_TSKIF_CPRIRCV_SVMTCNTRELREQ));
	
	((T_SYS_COMMON_THDIF*)ltSvmtCntRelReq_p)->uiKeyID = keyID;
	
	/* SV-MT/eNB-MT接続解放要求(l3/rtc ->l2/lpb ) */
	ldwRtc = m_cm_L3MsgSend_REC( ltSvmtCntRelReq_p );
	if( ldwRtc != CMD_L3_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1, "Send SV-MT/eNB-MT接続解放要求 NG" );
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		
		return;
	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		lwTimerID = CMD_TIMID_SVMTCNTREL_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerID = CMD_TIMID_SVMTCNTREL_LTE;
// 	}
// 	/* タイマ起動SV-MT/eNB-MT接続解放確認タイマ */
// 	lwTimerVal = cmw_sys_mng_tbl[lwSigsys_rslt].svmt_rltimer; 
	
// 	/* タイマ起動SV-MT/eNB-MT接続解放応答確認タイマ */
// 	ldwRtc = cm_TStat(lwTimerID, TIMVAL_GET(lwTimerVal), CMD_TIMMD_SINGLE,
// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
	
// 	if(ldwRtc != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerID, "SV-MT/eNB-MT接続解放確認タイマ Start NG" );
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end [%d]\n",__LINE__,__FUNCTION__,lwTimerID);
// #endif
		
// 		return;
// 	}
	
// 	/* 待ちフラグ(SV-MT/eNB-MT接続解放応答):ON */
// 	gw_rcw_SvMt_CntRel_WaitFlg[lwSigsys_rslt] = D_L3_ON;

//	/* Save Re which sent release connect request */
//	gw_rcw_SvMt_ConnRelReqRe[lwSigsys_rslt] = lwReNo;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		
	/* 処理終了 */
	return;
}
/* @} */
	
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief	SV-MT/eNB-MT接続解放応答関数
 *  @note 
 *  @param	rsp_p [in] the buffer address pointer of received message
 *  @return	None
 *  @date	2015/07/18 FPT)TuanBV
 *  @date   2015/10/07 TDI)satou [FHM技説-QA No.50] 応答結果はRECから受信したものを透過するように
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-46)
 */
/********************************************************************************************************************/

VOID m_rc_RcvSVMTCntRelRes( CMT_TSKIF_CPRIRCV_SVMTCNTRELRES*  rsp_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
	UINT		lwSigchk_rslt;						/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	INT			errcd;
	UINT		lwTimerID;

	/*In case buffer is Null*/
	if( NULL == rsp_p )
	{
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		
		/*end processing*/
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(rsp_p->cpridat_svmtcntrelres.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}

// 	/* 待ちフラグ(SV-MT/eNB-MT接続解放応答) がOFFの場合 */
// 	if( gw_rcw_SvMt_CntRel_WaitFlg[lwSigsys_rslt] == D_L3_OFF )
// 	{
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 		/* 処理終了 */
// 		return;
// 	}
	
// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		lwTimerID = CMD_TIMID_SVMTCNTREL_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerID = CMD_TIMID_SVMTCNTREL_LTE;
// 	}

// 	/* タイマ停止SV-MT/eNB-MT接続解放確認タイマ */
// 	cm_TStop(lwTimerID,&errcd);
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s Stop timer (TimerId:%d) \n",__LINE__,__FUNCTION__,lwTimerID);
// #endif

	/* RECからの応答結果が「0:正常」 ]の場合 */
	if( rsp_p->cpridat_svmtcntrelres.result == CMD_RES_OK )
	{
		/* Send SV-MT/eNB-MT接続解放応答 ( l3/rct->re/rec  ) */
		m_rc_SVMTCntRelResSnd( rsp_p->cpridat_svmtcntrelres.result, lwSigsys_rslt );
		/* テーブル更新SV-MT/eNB-MT接続状態←未接続 */
		gw_rcw_SvEnbMt_ConnSts[lwSigsys_rslt] = D_L3_MT_NOT_CONNECT;
	}
	/* 応答結果が「0:正常」以外の場合 */
	else
	{
		/* Send SV-MT/eNB-MT接続解放応答 ( l3/rct->re/rec  ) */
		m_rc_SVMTCntRelResSnd( rsp_p->cpridat_svmtcntrelres.result, lwSigsys_rslt );
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s gw_rcw_SvEnbMt_ConnSts[%d] = %d \n",__LINE__,__FUNCTION__,lwSigsys_rslt,gw_rcw_SvEnbMt_ConnSts[lwSigsys_rslt]);
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif

	/* 処理終了 */
	return;
}

// /* @} */
// /********************************************************************************************************************/
// /**
//  *  @brief  SV-MT/eNB-MT接続解放確認タイマタイムアウト
//  *  @note 
//  *  @param   awTimerID [in] Timer ID
//  *  @return None
//  *  @date 2015/07/18 FPT)TuanBV
//  *  @date   2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-46)
//  */
// /********************************************************************************************************************/

// VOID m_rc_SVMTCntRelTO( UINT  awTimerID )
// {
// 	USHORT				lwSystemType;
// 	USHORT				lwResult[CMD_MAX_SYSNUM] = { CMD_RECRS_NG, 			/* 232: REC応答NG(3G) */
// 													CMD_REC_RSP_NG_LTE };	/* 102: REC応答NG(LTE) */
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s Start(awTimerID: %d) \n",__LINE__,__FUNCTION__,awTimerID);
// #endif
	
// 	/*タイマIdが３G タイマの場合*/
// 	if( awTimerID == CMD_TIMID_SVMTCNTREL_3G)
// 	{
// 		lwSystemType = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		lwSystemType = CMD_SYS_LTE;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s systerm:%d, gw_rcw_SvMt_CntRel_WaitFlg:%d \n",
// 		__LINE__,__FUNCTION__,lwSystemType,gw_rcw_SvMt_CntRel_WaitFlg[lwSystemType]);
// #endif
// 	if( gw_rcw_SvMt_CntRel_WaitFlg[lwSystemType] == D_L3_OFF )
// 	{
// 		/* 処理終了 */
// 		return;
// 	}

// 	/* Send SV-MT/eNB-MT接続解放応答 ( l3/rct->re/rec  ) */
// 	m_rc_SVMTCntRelResSnd( lwResult[lwSystemType], lwSystemType );

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 	/* 処理終了 */
// 	return;
// }

// /* @} */

/********************************************************************************************************************/
/**
 *  @brief	Send SV-MT/eNB-MT接続解放応答
 *  @note 
 *  @param	awRslt [in] Resonse result
 *  @param	awSystem [in] System type
 *  @return	None
 *  @date	2015/07/18 FPT)TuanBV
 *  @date 	2015/09/29 FPT)Yen fix bug IT2
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-46)
 */
/********************************************************************************************************************/

VOID m_rc_SVMTCntRelResSnd( USHORT  awRslt, USHORT awSystem )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Start (Result:%d, Systerm:%d)\n",__LINE__,__FUNCTION__,awRslt,awSystem);
#endif
	CMT_TSKIF_CPRIRCV_SVMTCNTRELRES 	ltMsgRsp;

	/* MT is not connected*/
	if( gw_rcw_SvEnbMt_ConnSts[awSystem] != D_L3_MT_CONNECT )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}
	/*Re which send request is not connect with MT*/
	if( gw_rcw_SvMt_ConnRelReqRe[awSystem] != gw_rcw_SvEnbMt_ConnDst[awSystem] )
	{
		return;
	}

	cm_MemClr(&ltMsgRsp, sizeof(ltMsgRsp));

	/* Make msg respone */
	ltMsgRsp.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)
									+	sizeof(CMT_CPRIF_SVMTCNTRELRES);
	/* Signal kind */
	ltMsgRsp.head.uiSignalkind = CMD_CPRID_SVMTCNTRELRES + awSystem;
	ltMsgRsp.cpridat_svmtcntrelres.signal_kind = CMD_CPRID_SVMTCNTRELRES + awSystem;

	/* 応答結果設定 */
	ltMsgRsp.cpridat_svmtcntrelres.result = awRslt;
	
	ltMsgRsp.cprircv_inf.link_num = gw_rcw_SvMt_ConnRelReqRe[awSystem];
	
	/* SV-MT/eNB-MT接続解放応答 ( l3/rct->re/rec ) */
	(VOID)l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  &ltMsgRsp, sizeof( CMT_TSKIF_CPRIRCV_SVMTCNTRELRES ) );	
	
	/* Init Re which sent release connect request */
	gw_rcw_SvMt_ConnRelReqRe[awSystem] = 0;

//	/* 待ちフラグ(SV-MT/eNB-MT接続解放応答):OFF */
//	gw_rcw_SvMt_CntRel_WaitFlg[awSystem] = D_L3_OFF;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Signalkind : [%d] \n Link num: [%d] \n Result: [%d] \n System type : [%d]\n "
		,__LINE__,__FUNCTION__,ltMsgRsp.cpridat_svmtcntrelres.signal_kind,ltMsgRsp.cprircv_inf.link_num,ltMsgRsp.cpridat_svmtcntrelres.result,awSystem);
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif

	/* 処理終了 */
	return;
}


/* @} */

