/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_FldMt_FileTrans.c
 *  @brief  FLD-MTデータファイル転送受信処理
 *  @date   2015/07/08
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-40)
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
 *  @brief  FLD-MTデータファイル転送受信要求
 *  @note 
 *  @param  parm_p  [in]  the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/14 Fpt)Dong
 *  @date 2015/09/21 FPT)Quynh Fix bug IT1
 *  @date 2015/09/29 FPT)Yen Fix bug IT2
 *  @date 2015/10/05 FPT)Yen Fix bug IT2(update size of msg  FLD-MTデータファイル転送受信要求)
 *  @date 2016/03/18 TDI)satou 本来とは異なるシスパラ(SV-MT data receiving confirmation timer)からタイマ値を参照していた不具合を修正
 *  @date 2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-40)
 */
/********************************************************************************************************************/
VOID m_rc_FldMt_FileTransReq(void * parm_p)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	UINT		lwSigchk_rslt;						/*  result of checking signal */
	USHORT		lwSigsys_rslt;						/*  system by checking signal */
	INT			errcd;
	USHORT		lwTimerId;
	USHORT		lwRcvByte;
	UINT		ldwRtc;
	USHORT		lwTimerVal = 0;
	CMT_TSKIF_CPRIRCV_FLDFILRCVREQ* ltFldmtFilRcvReq3G_p;
	CMT_TSKIF_CPRIRCV_FLDFILRCVREQ_S3G* ltFldmtFilRcvReqS3G_p;
 	UINT		a_ret;	/* buffer hunt return code */
	UINT 		keyID;
	USHORT		lwResult[CMD_MAX_SYSNUM] = { CMD_FLD_CMDRCV_NG,		  /* 233:受信不可状態 */
											 CMD_FLD_CMDRCV_NG_LTE }; /* 103:受信不可状態 */;
	UINT		luiMsgSize;

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET*) parm_p)->signal_kind, &lwSigsys_rslt);
	
	/* signalがNGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");

		if (CMD_SYS_3G == lwSigsys_rslt) {
			/* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) - 232:REC応答NG */
			m_rc_FldMt_FilTransResSnd( CMD_REC_RES_NG, lwSigsys_rslt );
			return;
		}
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s gw_rcw_FldMt_ConnSts[%d] = %d,link_num = %d, gw_rcw_FldMt_ConnDst[%d] = %d \n",
					__LINE__,__FUNCTION__,lwSigsys_rslt,gw_rcw_FldMt_ConnSts[lwSigsys_rslt],
					((CMT_TSKIF_CPRIRCV_SIGNALGET*) parm_p)->cprircv_inf.link_num,
					lwSigsys_rslt, gw_rcw_FldMt_ConnDst[lwSigsys_rslt]);
#endif
	/* FLD-MT not connect or Re which connect with FLD-MT is diffirent with Re request */
	if(( gw_rcw_FldMt_ConnSts[lwSigsys_rslt] != D_L3_MT_CONNECT) ||
		( ((CMT_TSKIF_CPRIRCV_SIGNALGET*) parm_p)->cprircv_inf.link_num != gw_rcw_FldMt_ConnDst[lwSigsys_rslt] ))
	{
		return ;
	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_FLDMT_FILE_TRANS_3G;
// 		lwRcvByte = ((CMT_TSKIF_CPRIRCV_FLDFILRCVREQ *)parm_p)->cpridat_fldfilrcvreq.rcvbyte_num;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_FLDMT_FILE_TRANS_LTE;
// 		lwRcvByte = ((CMT_TSKIF_CPRIRCV_FLDFILRCVREQ_S3G *)parm_p)->cpridat_fldfilrcvreq.rcvbyte_num;
// 	}

	lwRcvByte = ((CMT_TSKIF_CPRIRCV_FLDFILRCVREQ_S3G *)parm_p)->cpridat_fldfilrcvreq.rcvbyte_num;

	/* Save Re which sent FLD-MTデータファイル転送受信要求 */
	gw_rcw_FldMt_FilTransReqRe[lwSigsys_rslt] = ((CMT_TSKIF_CPRIRCV_SIGNALGET*) parm_p)->cprircv_inf.link_num;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]gw_rcw_FldMt_FilTransReqRe[lwSigsys_rslt]: %d \n",__LINE__,gw_rcw_FldMt_FilTransReqRe[lwSigsys_rslt]);
#endif
	
	lwRcvByte = ((CMT_TSKIF_CPRIRCV_FLDFILRCVREQ_S3G *)parm_p)->cpridat_fldfilrcvreq.rcvbyte_num;

	/* Receive byte is out of range */	
	if((lwRcvByte < FLDMT_FIL_TRANS_DAT_MIN ) || ( lwRcvByte > FLDMT_FIL_TRANS_DAT_MAX ))
	{
		/* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
		m_rc_FldMt_FilTransResSnd( lwResult[lwSigsys_rslt], lwSigsys_rslt );
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s lwRcvByte: %d \n",__LINE__,__FUNCTION__, lwRcvByte);
#endif
		/* 処理終了 */
		return;
	}

// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		luiMsgSize = ((CMT_TSKIF_CPRIRCV_FLDFILRCVREQ *)parm_p)->head.uiLength;

// 		a_ret = cm_L2BfGet(CMD_BUFCA_LAPSND, luiMsgSize, CMD_NUM1, (void**)&ltFldmtFilRcvReq3G_p);
// 		if (a_ret  != CMD_RES_OK)
// 		{
// 			cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_FldMt_FileTransReq", 
// 				"buffer hunt fail", a_ret, luiMsgSize, CMD_NUM0);
// 		}
		
// 		/*temp save key*/
// 		keyID = ((T_SYS_COMMON_THDIF*)ltFldmtFilRcvReq3G_p)->uiKeyID;
		
// 		cm_MemCpy(ltFldmtFilRcvReq3G_p, parm_p, luiMsgSize);
		
// 		((T_SYS_COMMON_THDIF*)ltFldmtFilRcvReq3G_p)->uiKeyID = keyID; 
		
// 		/* SV-M/eNB-MTデータ受信要求(l3/rtc ->l2/lpb ) */
// 		ldwRtc = m_cm_L3MsgSend_REC( ltFldmtFilRcvReq3G_p );
// 	}
// 	else
// 	{
// 		luiMsgSize = ((CMT_TSKIF_CPRIRCV_FLDFILRCVREQ_S3G *)parm_p)->head.uiLength;
// 		a_ret = cm_L2BfGet(CMD_BUFCA_LAPSND, luiMsgSize, CMD_NUM1, (void**)&ltFldmtFilRcvReqS3G_p);
// 		if (a_ret  != CMD_RES_OK)
// 		{
// 			cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_FldMt_FileTransReq", 
// 				"buffer hunt fail", a_ret, luiMsgSize, CMD_NUM0);
// 		}
		
// 		/*temp save key*/
// 		keyID = ((T_SYS_COMMON_THDIF*)ltFldmtFilRcvReqS3G_p)->uiKeyID;
		
// 		cm_MemCpy(ltFldmtFilRcvReqS3G_p, parm_p, luiMsgSize);
		
// 		((T_SYS_COMMON_THDIF*)ltFldmtFilRcvReqS3G_p)->uiKeyID = keyID;
		
// 		/* SV-M/eNB-MTデータ受信要求(l3/rtc ->l2/lpb ) */
// 		ldwRtc = m_cm_L3MsgSend_REC( ltFldmtFilRcvReqS3G_p );
// 	}

	luiMsgSize = ((CMT_TSKIF_CPRIRCV_FLDFILRCVREQ_S3G *)parm_p)->head.uiLength;
	a_ret = cm_L2BfGet(CMD_BUFCA_LAPSND, luiMsgSize, CMD_NUM1, (void**)&ltFldmtFilRcvReqS3G_p);
	if (a_ret  != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_FldMt_FileTransReq", 
			"buffer hunt fail", a_ret, luiMsgSize, CMD_NUM0);
	}
	
	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)ltFldmtFilRcvReqS3G_p)->uiKeyID;
	
	cm_MemCpy(ltFldmtFilRcvReqS3G_p, parm_p, luiMsgSize);
	
	((T_SYS_COMMON_THDIF*)ltFldmtFilRcvReqS3G_p)->uiKeyID = keyID;
	
	/* SV-M/eNB-MTデータ受信要求(l3/rtc ->l2/lpb ) */
	ldwRtc = m_cm_L3MsgSend_REC( ltFldmtFilRcvReqS3G_p );
	if( ldwRtc != CMD_L3_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwRtc, "Send SV-M/eNB-MTデータ受信要求 NG" );
#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}	
	
// 	/* タイマ起動: FLD-MT data file receiving confirmation timer */
// 	lwTimerVal = cmw_sys_mng_tbl[lwSigsys_rslt].fldmt_dtfl_timer;
// 	ldwRtc = cm_TStat(lwTimerId, TIMVAL_GET(lwTimerVal), CMD_TIMMD_SINGLE,
// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);	
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]cm_TStat(timerId:%d), Result: %d \n",__LINE__,lwTimerId, ldwRtc);
// #endif
// 	if(ldwRtc != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "SV-MT/eNB-MTデータ受信確認タイマ Start NG" );
// 		return;
// 	}
	
// 	/* FLD-MTデータファイル転送受信応答待ちフラグ設定-->ON */
// 	gw_rcw_FldMt_FilTrans_WaitFlg[lwSigsys_rslt] = D_L3_ON;
#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s gw_rcw_FldMt_FilTrans_WaitFlg[lwSigsys_rslt] = %d\n",__LINE__,__FUNCTION__, gw_rcw_FldMt_FilTrans_WaitFlg[lwSigsys_rslt] );
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif

	return;
}
/* @} */
	
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  FLD-MTデータファイル転送受信応答
 *  @note 
 *  @param  parm_p  [in]  the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/14 Fpt)Dong
 *  @date   2015/10/07 TDI)satou [FHM技説-QA No.50] 応答結果はRECから受信したものを透過するように
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-40)
 */
/********************************************************************************************************************/
VOID m_rc_FldMt_FileTransRes(CMT_TSKIF_CPRIRCV_FLDFILRCVRES* parm_p)
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	UINT		lwSigchk_rslt;						/*  result of checking signal */
	USHORT		lwSigsys_rslt;						/* system after checking signal */	
	USHORT		lwTimerID;							/* Timer Identify */	
	INT			errcd;

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}
	
	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_fldfilrcvres.signal_kind, &lwSigsys_rslt);
	
	/* validate signal NGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}

// 	/* FLD-MTデータファイル転送受信応答待ちフラグ がOFFの場合 */
// 	if( gw_rcw_FldMt_FilTrans_WaitFlg[lwSigsys_rslt] == D_L3_OFF )
// 	{
// 		/* 処理終了 */
// 		#ifdef FHM_RCT_DEBUG_FOR_IT1
// 		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// 		#endif
// 		return;
// 	}

// 	/* signalが3Gの場合 */
// 	if( lwSigsys_rslt == CMD_SYS_3G )
// 	{
// 		lwTimerID = CMD_TIMID_FLDMT_FILE_TRANS_3G;
// 	}
// 	/* signalがLTEの場合 */
// 	else
// 	{
// 		lwTimerID = CMD_TIMID_FLDMT_FILE_TRANS_LTE;
// 	}

// 	/* タイマ停止SV-MT/eNB-MTデータ受信確認タイマ */
// 	cm_TStop(lwTimerID, &errcd);
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s TimerStop(TimerId:%d) \n",__LINE__,__FUNCTION__,lwTimerID);
// #endif	
	
    /* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
    m_rc_FldMt_FilTransResSnd( parm_p->cpridat_fldfilrcvres.result, lwSigsys_rslt );

// 	/* FLD-MTデータファイル転送受信応答待ちフラグ: OFF */
// 	gw_rcw_FldMt_FilTrans_WaitFlg[lwSigsys_rslt] = D_L3_OFF;
#ifdef FHM_RCT_DEBUG_FOR_IT1
//	printf( "[%d]gw_rcw_FldMt_FilTrans_WaitFlg[lwSigsys_rslt]: %d \n",__LINE__,gw_rcw_FldMt_FilTrans_WaitFlg[lwSigsys_rslt]);
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	
	/* 処理終了 */
	return;
}
/* @} */


// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief  FLD-MTデータファイル転送受信確認タイマタイムアウト
//  *  @note 
//  *  @param  awTimerId [in] Timer ID
//  *  @return None
//  *  @date	2015/07/14 Fpt)Dong
//  *  @date 	2015/09/29 FPT)Yen fix bug IT2
//  *  @date 	2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-40)
//  */
// /********************************************************************************************************************/
// VOID m_rc_FldMt_FilTransTO( USHORT awTimerId )
// {	
// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s start(timerId:%d) \n",__LINE__,__FUNCTION__,awTimerId );
// 	#endif
// 	USHORT		lwSystemType;
// 	USHORT		lwRspResult[CMD_MAX_SYSNUM] = { CMD_RECRS_NG,		  /* 232 : REC応答NG (3G) */
// 												CMD_REC_RSP_NG_LTE }; /* 102 : REC応答NG (LTE) */

// 	/*タイマIdが３G タイマの場合*/
// 	if( awTimerId == CMD_TIMID_FLDMT_FILE_TRANS_3G)
// 	{
// 		lwSystemType = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		lwSystemType = CMD_SYS_LTE;
// 	}
	
// 	/* FLD-MTデータファイル転送受信応答待ちフラグ がOFFの場合 */
// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]lwSystemType:%d, gw_rcw_FldMt_FilTrans_WaitFlg[lwSystemType]: %d \n",
// 		__LINE__,lwSystemType,gw_rcw_FldMt_FilTrans_WaitFlg[lwSystemType]);
// 	#endif
// 	if( gw_rcw_FldMt_FilTrans_WaitFlg[lwSystemType] == D_L3_OFF )
// 	{
// 		/* 処理終了 */
// 		#ifdef FHM_RCT_DEBUG_FOR_IT1
// 		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// 		#endif
// 		return;
// 	}

// 	/* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
// 	m_rc_FldMt_FilTransResSnd( lwRspResult[lwSystemType], lwSystemType );

// 	/* FLD-MTデータファイル転送受信応答待ちフラグ: OFFF */
// 	gw_rcw_FldMt_FilTrans_WaitFlg[lwSystemType] = D_L3_OFF;
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]gw_rcw_FldMt_FilTrans_WaitFlg[lwSystemType]: %d \n",__LINE__,gw_rcw_FldMt_FilTrans_WaitFlg[lwSystemType]);
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif

// 	/* 処理終了 */
// 	return;
	
// }
// /* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief	Send FLD-MTデータファイル転送受信応答
 *  @note 
 *  @param	awRslt [in] Resonse result
 *  @param	awSystem [in] System type
 *  @return	None
 *  @date	2015/07/21 FPT)TuanBV
 */
/********************************************************************************************************************/

VOID m_rc_FldMt_FilTransResSnd( USHORT  awRslt, USHORT awSystem )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start(Result:%d, Systerm:%d) \n",__LINE__,__FUNCTION__,awRslt,awSystem );
	#endif
	
	CMT_TSKIF_CPRIRCV_FLDFILRCVRES 	ltMsgRsp;
	
	/*FldMT is not connected*/
	if( gw_rcw_FldMt_ConnSts[awSystem] != D_L3_MT_CONNECT )
	{
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/*Re which send request is not connect with MT*/
	if( gw_rcw_FldMt_FilTransReqRe[awSystem] != gw_rcw_FldMt_ConnDst[awSystem] )
	{
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	cm_MemClr(&ltMsgRsp, sizeof(ltMsgRsp));
	
	/* Make msg respone */
	ltMsgRsp.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)
									+	sizeof(CMT_CPRIF_FLDFILRCVRES);
	
	ltMsgRsp.head.uiSignalkind = CMD_CPRID_FLDFILRCVRES + awSystem;
	ltMsgRsp.cpridat_fldfilrcvres.signal_kind = CMD_CPRID_FLDFILRCVRES + awSystem;

	/* 応答結果設定 */
	ltMsgRsp.cpridat_fldfilrcvres.result = awRslt;
	
	ltMsgRsp.cprircv_inf.link_num = gw_rcw_FldMt_FilTransReqRe[awSystem];
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Signal kind: %x, Result: %d, link_num:0x%x\n",__LINE__,__FUNCTION__,
		ltMsgRsp.cpridat_fldfilrcvres.signal_kind, ltMsgRsp.cpridat_fldfilrcvres.result,ltMsgRsp.cprircv_inf.link_num );
#endif
	/* SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
	(VOID)l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  &ltMsgRsp, sizeof( CMT_TSKIF_CPRIRCV_FLDFILRCVRES ) );	
	
	/* Clear infor about Re which sent FLD-MTデータファイル転送受信要求  */
	gw_rcw_FldMt_FilTransReqRe[awSystem] = 0;

	/* 処理終了 */
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]gw_rcw_FldMt_FilTransReqRe[awSystem]: %d \n",__LINE__,gw_rcw_FldMt_FilTransReqRe[awSystem]);
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;
}

/* @} */
/* @} */
