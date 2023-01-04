/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_SVMTDatRcv.c
 *  @brief  SV-MT/eNB-MTデータ受信関数
 *  @date   2015/07/21 FPT)TuanBV
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-45)
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
#include "f_sys_inc.h"
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief 	SV-MT/eNB-MTデータ受信要求関数
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 	2015/07/21 FPT)TuanBV
 *  @date   2015/10/05 FPT)Yen Fix bug IT2(update size of msg SV-MT/eNB-MTデータ受信要求)
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-45)
 */
/********************************************************************************************************************/

VOID m_rc_SndSVMTDatRcvReq( CMT_TSKIF_CPRIRCV_SVMTDATRCVREQ* parm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif

	UINT								lwSigchk_rslt;			/* result of checking signal */
	USHORT								lwSigsys_rslt;			/* system after checking signal */
	USHORT								lwTimerID;				/* Timer Identify */
	INT									errcd;
	UINT								ldwRtc;
	USHORT								lwTimerVal;
	CMT_TSKIF_CPRIRCV_SVMTDATRCVREQ* 	ltSvmtDatRcvReq_p;
 	UINT								luiRet;	/* buffer hunt return code */
	UINT 								keyID;
	UINT 								luiMsgSize;

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_svmtdatrcvreq.signal_kind,&lwSigsys_rslt);
	
	/* validate signal NGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");

		if (CMD_SYS_3G == lwSigsys_rslt) {
			/* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
			m_rc_SVMTDatRcvResSnd( CMD_RECRS_NG, lwSigsys_rslt );
			return;
		}
	}

	/* Save Re which sent data reception request */
	gw_rcw_SvMt_DatRcvReqRe[lwSigsys_rslt] = parm_p->cprircv_inf.link_num;

// 	/* パラメータチェックNGの場合 */
// 	/* signalが３G signalの場合*/
// 	if( lwSigsys_rslt == CMD_SYS_3G )
// 	{
// 		lwTimerID = CMD_TIMID_SVMTDATRCV_3G;
// 		if (( parm_p->cpridat_svmtdatrcvreq.rcv_bytnum < MIN_MSG3G_SIZE ) ||
// 			( parm_p->cpridat_svmtdatrcvreq.rcv_bytnum > MAX_MSG3G_SIZE ) ||
// 			( CMD_NUM0 == gw_rcw_SvEnbMt_ConnDst[lwSigsys_rslt] )
// 		)
// 		{
// 			/* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
// 			m_rc_SVMTDatRcvResSnd( CMD_SVMT_CMDRCV_NG, lwSigsys_rslt );
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 			/* 処理終了 */
// 			return;
// 		}
// 	} 
// 	/* signalがLTE signalの場合*/
// 	else
// 	{
// 		lwTimerID = CMD_TIMID_SVMTDATRCV_LTE;
// 		if (( parm_p->cpridat_svmtdatrcvreq.rcv_bytnum < MIN_MSGLTE_SIZE ) ||
// 			( parm_p->cpridat_svmtdatrcvreq.rcv_bytnum > MAX_MSGLTE_SIZE ) ||
// 			( CMD_NUM0 == gw_rcw_SvEnbMt_ConnDst[lwSigsys_rslt] )
// 		)
// 		{
// 			/* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
// 			m_rc_SVMTDatRcvResSnd( CMD_SVMT_CMDRCV_NG_LTE, lwSigsys_rslt );
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 			/* 処理終了 */
// 			return;
// 		}
// 	}

	/* パラメータチェックNGの場合 */
	if (( parm_p->cpridat_svmtdatrcvreq.rcv_bytnum < MIN_MSGLTE_SIZE ) ||
		( parm_p->cpridat_svmtdatrcvreq.rcv_bytnum > MAX_MSGLTE_SIZE ) ||
		( CMD_NUM0 == gw_rcw_SvEnbMt_ConnDst[lwSigsys_rslt] )
	)
	{
		/* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
		m_rc_SVMTDatRcvResSnd( CMD_SVMT_CMDRCV_NG_LTE, lwSigsys_rslt );
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		/* 処理終了 */
		return;
	}

	luiMsgSize = ((CMT_TSKIF_CPRIRCV_SVMTDATRCVREQ *)parm_p)->head.uiLength;

	luiRet = cm_L2BfGet(CMD_BUFCA_LAPSND, luiMsgSize, CMD_NUM1, (void**)&ltSvmtDatRcvReq_p);
	if (luiRet  != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_SndSVMTDatRcvReq", 
			"buffer hunt fail", luiRet, luiMsgSize, CMD_NUM0);
	}
	
	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)ltSvmtDatRcvReq_p)->uiKeyID;
	
	cm_MemCpy(ltSvmtDatRcvReq_p, parm_p,  luiMsgSize);
	
	((T_SYS_COMMON_THDIF*)ltSvmtDatRcvReq_p)->uiKeyID = keyID;
	
	/* SV-MT/eNB-MTデータ受信要求(l3/rtc ->l2/lpb ) */
	ldwRtc = m_cm_L3MsgSend_REC( ltSvmtDatRcvReq_p );
	
	if( ldwRtc != CMD_L3_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwRtc, "Send SV-MT/eNB-MTデータ受信要求 NG" );
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

// 	/* タイマ起動SV-MT/eNB-MTデータ受信確認タイマ */
// 	lwTimerVal = cmw_sys_mng_tbl[lwSigsys_rslt].svmt_rcvtimer; 
	
// 	/* タイマ起動SV-MT/eNB-MTデータ受信応答確認タイマ */
// 	ldwRtc = cm_TStat(lwTimerID, TIMVAL_GET(lwTimerVal), CMD_TIMMD_SINGLE,
// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
	
// 	if(ldwRtc != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerID, "SV-MT/eNB-MTデータ受信確認タイマ Start NG" );
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 		return;
// 	}

// 	/* 待ちフラグ(SV-MT/eNB-MTデータ受信応答):ON */
// 	gw_recw_WaitFlag_MT_DatRcv[lwSigsys_rslt] = D_L3_ON;
	
	/* MT is connected */
	gw_rcw_SvEnbMt_ConnSts[lwSigsys_rslt] = D_L3_MT_CONNECT;

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
 *  @brief  SV-MT/eNB-MTデータ受信応答関数
 *  @note 
 *  @param  rsp_p  [in] the buffer address pointer of received message
 *  @return None
 *  @date 	2015/07/21 FPT)TuanBV
 *  @date   2015/10/07 TDI)satou [FHM技説-QA No.50] 応答結果はRECから受信したものを透過するように
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-45)
 */
/********************************************************************************************************************/

VOID m_rc_RcvSVMTDatRcvRes( CMT_TSKIF_CPRIRCV_SVMTDATRCVRES*  rsp_p )
{
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
		
	UINT		lwSigchk_rslt;						/*  result of checking signal */
	USHORT		lwSigsys_rslt;						/* system after checking signal */	
	USHORT		lwTimerID;							/* Timer Identify */	
	INT			errcd;
	
	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(rsp_p->cpridat_svmtdatrcvres.signal_kind,&lwSigsys_rslt);
	
	/* validate signal NGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}

// 	/* 待ちフラグ(SV-MT/eNB-MTデータ受信応答関数) がOFFの場合 */
// 	if( gw_recw_WaitFlag_MT_DatRcv[lwSigsys_rslt] == D_L3_OFF )
// 	{
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
		
// 		/* 処理終了 */
// 		return;
// 	}
	
// 	/* signalが3Gの場合 */
// 	if( lwSigsys_rslt == CMD_SYS_3G )
// 	{
// 		lwTimerID = CMD_TIMID_SVMTDATRCV_3G;
// 	}
// 	/* signalがLTEの場合 */
// 	else
// 	{
// 		lwTimerID = CMD_TIMID_SVMTDATRCV_LTE;
// 	}

// 	/* タイマ停止SV-MT/eNB-MTデータ受信確認タイマ */
// 	cm_TStop(lwTimerID,&errcd);
	
    /* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
    m_rc_SVMTDatRcvResSnd( rsp_p->cpridat_svmtdatrcvres.result, lwSigsys_rslt );
	
// 	/* 待ちフラグ(SV-MT/eNB-MTデータ受信応答):OFF */
// 	gw_recw_WaitFlag_MT_DatRcv[lwSigsys_rslt] = D_L3_OFF;
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		
	/* 処理終了 */
	return;
}
/* @} */	

// /* @{ */	
// /********************************************************************************************************************/
// /**
//  *  @brief  SV-MT/eNB-MTデータ受信確認タイマタイムアウト
//  *  @note 
//  *  @param  awTimerID [in] Timer ID
//  *  @return None
//  *  @date 	2015/07/21 FPT)TuanBV
//  *  @date 	2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-45)
//  */
// /********************************************************************************************************************/

// VOID m_rc_SVMTDatRcvTO( UINT  awTimerID )
// {
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s Start(timerId: %d) \n",__LINE__,__FUNCTION__, awTimerID);
// #endif
// 	USHORT								lwSystemType;
// 	USHORT								lwRslt;
	
// 	/*タイマIdが３G タイマの場合*/
// 	if( awTimerID == CMD_TIMID_SVMTDATRCV_3G)
// 	{
// 		lwSystemType = CMD_SYS_3G;
// 		lwRslt = CMD_RECRS_NG;		
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else 
// 	{
// 		lwSystemType = CMD_SYS_LTE;
// 		lwRslt = CMD_REC_RSP_NG_LTE;
// 	}
	
// 	/* 待ちフラグ(SV-MT/eNB-MTデータ受信応答関数) がOFFの場合 */
// 	if( gw_recw_WaitFlag_MT_DatRcv[lwSystemType] == D_L3_OFF )
// 	{
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
		
// 		/* 処理終了 */
// 		return;
// 	}
	
// 	/* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
// 	m_rc_SVMTDatRcvResSnd( lwRslt, lwSystemType );

// 	/* 待ちフラグ(SV-MT/eNB-MTデータ受信応答):OFF */
// 	gw_recw_WaitFlag_MT_DatRcv[lwSystemType] = D_L3_OFF;
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
		
// 	/* 処理終了 */
// 	return;
// }
// /* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief	Send SV-MT/eNB-MTデータ受信応答
 *  @note 
 *  @param	awRslt [in] Resonse result
 *  @param	awSystem [in] System type
 *  @return	None
 *  @date	2015/07/21 FPT)TuanBV
 */
/********************************************************************************************************************/

VOID m_rc_SVMTDatRcvResSnd( USHORT  awRslt, USHORT awSystem )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif

	CMT_TSKIF_CPRIRCV_SVMTDATRCVRES		ltMsgRsp;
	
	/*SvMT is not connected*/
	if( gw_rcw_SvEnbMt_ConnSts[awSystem] != D_L3_MT_CONNECT )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/*Re which send request is not connect with MT*/
	if( gw_rcw_SvMt_DatRcvReqRe[awSystem] != gw_rcw_SvEnbMt_ConnDst[awSystem] )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	cm_MemClr(&ltMsgRsp, sizeof(ltMsgRsp));
	/* Make msg respone */
	ltMsgRsp.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)
									+	sizeof(CMT_CPRIF_SVMTDATRCVRES);
	
	ltMsgRsp.head.uiSignalkind = CMD_CPRID_SVMTDATRCVRES + awSystem;
	ltMsgRsp.cpridat_svmtdatrcvres.signal_kind = CMD_CPRID_SVMTDATRCVRES + awSystem;

	/* 応答結果設定 */
	ltMsgRsp.cpridat_svmtdatrcvres.result = awRslt;
	
	ltMsgRsp.cprircv_inf.link_num = gw_rcw_SvMt_DatRcvReqRe[awSystem];
	
	/* SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
	(VOID)l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  &ltMsgRsp, sizeof( CMT_TSKIF_CPRIRCV_SVMTDATRCVRES ) );	
	
	/* Clear infor about Re which sent SV-MT/eNB-MTデータ受信要求  */
	gw_rcw_SvMt_DatRcvReqRe[awSystem] = 0;
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \nSystemtype : [%d]\nSignalkind: [%d]\nLink num: [%d]\nResult: [%d]\n",__LINE__,
		__FUNCTION__,awSystem,ltMsgRsp.cpridat_svmtdatrcvres.signal_kind,ltMsgRsp.cprircv_inf.link_num,awRslt);
#endif
		
	/* 処理終了 */
	return;
}

/* @} */
/* @} */
