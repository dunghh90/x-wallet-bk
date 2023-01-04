/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_FldMt_Connect.c
 *  @brief  FLD-MT接続要求処理
 *  @date   2015/07/16 Fpt)Son
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-36)
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
 *  @brief  FLD-MT接続要求処理
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01869 
 *  @date   2015/07/16 Fpt)Son
 *  @date   2015/09/22 TDIPS)Lay M-RRU-ZSYS-01869 LAPB用バッファを使用するように修正
 *  @date   2015/09/29 Fpt)Yen fix bug IT2
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-36)
 */
/********************************************************************************************************************/
VOID m_rc_FldMt_ConnectReq( CMT_TSKIF_CPRIRCV_FLDMTCNCREQ* parm_p )
{
	UINT		sigchk_rslt;						/*  result of checking signal */
	USHORT		lwsigsys_rslt;						/*  system by checking signal */
	UINT		ldwRet;								/*  result of checking start timer */
	USHORT		lwTimerId;							/* 	Timer ID 						*/
	int 		errcd;
	USHORT		lwTimerValue;
	USHORT		lwReNo;
    UINT		a_ret;								/* buffer hunt return code */
	CMT_TSKIF_CPRIRCV_FLDMTCNCREQ* ltFldmtCncReq_p;
	UINT 		keyID;
	USHORT		lwRspResult[CMD_MAX_SYSNUM] = { CMD_MTCNT_DISABLE,		 /* 231 : MT接続不可 (3G) */
												CMD_MTCNT_DISABLE_LTE }; /* 101 : MT接続不可 (LTE) */
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* get Re number */
	lwReNo = parm_p->cprircv_inf.link_num;
	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_fldmtcncreq.signal_kind,&lwsigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_USELOW, lwsigsys_rslt, "validate signal NG");

		if (CMD_SYS_3G == lwsigsys_rslt) {
			/* 応答送信(MT接続不可)(L3->re/rec) - 232 : REC応答NG*/
			m_rc_FldMt_ConnectResSnd(CMD_RECRS_NG, lwsigsys_rslt, lwReNo );
			return;
		}
	}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s system type: %d\n",__LINE__,__FUNCTION__, lwsigsys_rslt);
#endif

// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwsigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_FLD_MT_CONN_CONF_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_FLD_MT_CONN_CONF_LTE;
// 	}
	
	/* FLD-MT接続状態が接続の場合 */
	if( D_L3_MT_CONNECT == gw_rcw_FldMt_ConnSts[lwsigsys_rslt] )
	{
		/* FLD-MT接続先が要求のCPRI番号と違う場合 */
		if ( gw_rcw_FldMt_ConnDst[lwsigsys_rslt] != lwReNo)
		{
			/* 応答送信(MT接続不可)(L3->re/rec) */
			m_rc_FldMt_ConnectResSnd( lwRspResult[lwsigsys_rslt], lwsigsys_rslt, lwReNo );
			/* 処理終了 */
			return;
		}
	}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s, parm_p->cprircv_inf.link_num = %d\n",__LINE__,__FUNCTION__,lwReNo);
#endif

	/***************************/
	/* Forward request to REC  */
	/***************************/
	a_ret = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRIRCV_FLDMTCNCREQ), CMD_NUM1, (void**)&ltFldmtCncReq_p);
	if (a_ret  != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_FldMt_ConnectReq", 
			"buffer hunt fail", a_ret, sizeof(CMT_TSKIF_CPRIRCV_FLDMTCNCREQ), CMD_NUM0);
	}
	
	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)ltFldmtCncReq_p)->uiKeyID;
	
	cm_MemCpy(ltFldmtCncReq_p, parm_p,  sizeof(CMT_TSKIF_CPRIRCV_FLDMTCNCREQ));
	
	((T_SYS_COMMON_THDIF*)ltFldmtCncReq_p)->uiKeyID = keyID; 
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s Signal kind: %x \n",__LINE__,__FUNCTION__, ltFldmtCncReq_p->cpridat_fldmtcncreq.signal_kind );
#endif
	
	/*FLD-MT接続要求をl2/lpbに送信する */
	ldwRet = m_cm_L3MsgSend_REC( ltFldmtCncReq_p );
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s ldwRet =  %d, parm_p->cprircv_inf.link_num = %d\n",__LINE__,__FUNCTION__, ldwRet,lwReNo);
#endif
	/* Sending fail */
	if( ldwRet!= CMD_L3_OK )
	{
		return;
	}

	/*FLD-MT接続先設定--> 当該配下RE */
	gw_rcw_FldMt_ConnDst[lwsigsys_rslt] = lwReNo ;

//	/* タイマ起動(FLD-MT接続確認タイマ) */
// 	lwTimerValue = cmw_sys_mng_tbl[lwsigsys_rslt].fldmt_cttimer;

// 	ldwRet = cm_TStat(lwTimerId, TIMVAL_GET(lwTimerValue), CMD_TIMMD_SINGLE,
// 						CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
	// if(ldwRet != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "FLD-MT接続確認タイマ Start NG" );
// 		return;
// 	}
// 	/* FLD-MT接続応答待ちフラグ設定-->ON */
// 	gw_rcw_FldMt_ConnRes_WaitFlg[lwsigsys_rslt] =  D_L3_ON ;

	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  FLD-MT接続応答処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2015/07/16 Fpt)Son
 *  @date   2015/09/29 Fpt)Yen fix bug IT2
 *  @date   2015/10/07 TDI)satou [FHM技説-QA No.50] 応答結果はRECから受信したものを透過するように
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-36)
 */
/********************************************************************************************************************/

VOID m_rc_FldMt_ConnectResp( CMT_TSKIF_CPRIRCV_FLDMTCNCRES* parm_p )
{

	UINT		sigchk_rslt;						/*  result of checking signal */
	USHORT		lwsigsys_rslt;						/*  system by checking signal */
	USHORT		lwTimerId;							/* 	Timer ID 				  */
	UINT		ldwResult; 
	int			errcd;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_fldmtcncres.signal_kind,&lwsigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							lwsigsys_rslt, 
							"validate signal NG");
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s lwsigsys_rslt:%d, gw_rcw_FldMt_ConnRes_WaitFlg:%d\n",
		__LINE__,__FUNCTION__,lwsigsys_rslt, gw_rcw_FldMt_ConnRes_WaitFlg[lwsigsys_rslt]);
#endif
// 	/* FLD-MT接続応答待ちフラグがOFFの場合 */
// 	if(gw_rcw_FldMt_ConnRes_WaitFlg[lwsigsys_rslt] ==  D_L3_OFF )
// 	{
// 		cm_Assert(	CMD_ASL_USELOW,
// 							lwsigsys_rslt, 
// 							"FLD-MT接続応答待ちフラグがOFF");
// 		return;
// 	}
	
// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwsigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_FLD_MT_CONN_CONF_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_FLD_MT_CONN_CONF_LTE;
// 	}
// 	lwTimerId = CMD_TIMID_FLD_MT_CONN_CONF_LTE;
	
// 	/* タイマ停止(FLD-MT接続確認タイマ) */
// 	ldwResult = cm_TStop( lwTimerId, &errcd);
// 	/* Stop timer NG */
// 	if( ldwResult != CMD_RES_OK )
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "SV-MT/eNB-MT接続強制解放確認タイマ Stop NG" );
// 	}

	/*  RECからの応答結果が「0:正常」の場合 */
	if( parm_p->cpridat_fldmtcncres.result == CMD_RES_OK )
	{
		/* FLD-MT接続状態設定-->接続中 */
		gw_rcw_FldMt_ConnSts[lwsigsys_rslt] = D_L3_MT_CONNECT ;
		/*FLD-MT接続応答(0：正常)をrec/reに送信する */
		m_rc_FldMt_ConnectResSnd(parm_p->cpridat_fldmtcncres.result, lwsigsys_rslt, gw_rcw_FldMt_ConnDst[lwsigsys_rslt] );
	}
	/* RECからの応答結果が「0:正常」以外場合 */
	else
	{
		/*FLD-MT接続応答(REC応答NG)をrec/reに送信する */
		 m_rc_FldMt_ConnectResSnd(parm_p->cpridat_fldmtcncres.result, lwsigsys_rslt, gw_rcw_FldMt_ConnDst[lwsigsys_rslt] );
	}
	
// 	/*FLD-MT接続応答待ちフラグ設定-->OFF */
// 	gw_rcw_FldMt_ConnRes_WaitFlg[lwsigsys_rslt] =  D_L3_OFF;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s gw_rcw_FldMt_ConnSts: %d\n",__LINE__,__FUNCTION__, gw_rcw_FldMt_ConnSts[lwsigsys_rslt]);
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
}
/* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief  FLD-MT接続確認タイマタイムアウト処理
//  *  @note 
//  *  @param  awTimerId [in] timer id
//  *  @return None
//  *  @date	 2015/07/16 Fpt)Son
//  *  @date     2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-36)
//  */
// /********************************************************************************************************************/

// VOID m_rc_FldMt_ConnectTo( USHORT awTimerId  )
// {
// 	USHORT		lwSystemType;
// 	USHORT		lwRspResult[CMD_MAX_SYSNUM] = { CMD_RECRS_NG,		  /* 232 : REC応答NG (3G) */
// 												CMD_REC_RSP_NG_LTE }; /* 102 : REC応答NG (LTE) */

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
// #endif
// 	/*タイマIdが３G タイマの場合*/
// 	if( CMD_TIMID_FLD_MT_CONN_CONF_3G == awTimerId )
// 	{
// 		lwSystemType = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		lwSystemType = CMD_SYS_LTE;
// 	}

// 	/* FLD-MT接続応答待ちフラグがOFFの場合 */
// 	if( D_L3_OFF == gw_rcw_FldMt_ConnRes_WaitFlg[lwSystemType] )
// 	{
// 		cm_Assert(	CMD_ASL_USELOW,
// 					lwSystemType,
// 					"FLD-MT接続応答待ちフラグがOFF");
// 		return;
// 	}

// 	/*FLD-MT接続応答(232:REC応答NG)をrec/reに送信する */
// 	m_rc_FldMt_ConnectResSnd(lwRspResult[lwSystemType], lwSystemType, gw_rcw_FldMt_ConnDst[lwSystemType] );
// 	/*FLD-MT接続応答待ちフラグ設定-->OFF */
// 	gw_rcw_FldMt_ConnRes_WaitFlg[lwSystemType] =  D_L3_OFF;
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 	return;
// }


/********************************************************************************************************************/
/**
 *  @brief FLD-MT接続応答送信処理
 *  @note 
 *  @param  awResult [in] Response result
 *  @param  awSystem [in] System type
 *  @param  awCpriLink [in] CpriLink
 *  @return None
 *  @date	2015/07/16 Fpt)Son
 *  @date	2015/09/29 Fpt)Yen fix bug IT2
 */
/********************************************************************************************************************/

VOID m_rc_FldMt_ConnectResSnd( USHORT awResult, USHORT awSystem, USHORT awCpriLink )
{
	CMT_TSKIF_CPRIRCV_FLDMTCNCRES	ltRes;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start (Result:%d, Systerm:%d, CpriNo:%d)\n",__LINE__,__FUNCTION__,awResult, awSystem, awCpriLink);
#endif
	cm_MemClr(&ltRes, sizeof(ltRes));
	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	ltRes.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)
									+	sizeof(CMT_CPRIF_FLDMTCNCRES);

	/* 応答結果設定 */
	ltRes.cpridat_fldmtcncres.result = awResult;
	/* SignalKind */
	ltRes.head.uiSignalkind = CMD_CPRID_FLDMTCNCRES + awSystem;
	/* 信号種別設定 */
	ltRes.cpridat_fldmtcncres.signal_kind = CMD_CPRID_FLDMTCNCRES + awSystem;
	
	/* CPRI番号*/
	ltRes.cprircv_inf.link_num = awCpriLink;
	
	/*Send FLD-MT接続応答 to re/rec*/
	(VOID)l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  &ltRes, sizeof( CMT_TSKIF_CPRIRCV_FLDMTCNCRES ) );
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	return;
}
/* @} */


/* @} */

