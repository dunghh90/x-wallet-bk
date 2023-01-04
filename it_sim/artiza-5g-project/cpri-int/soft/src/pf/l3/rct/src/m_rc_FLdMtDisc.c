/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_FLdMtDisc.c
 *  @brief  FLD-MT接続解放要求処理
 *  @date   2015/07/09 FPT)Quynh create new
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-41)
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
 *  @brief  3G(0x9330)FLD-MT接続解放要求を受信し、中継する。
 *  @brief  LTE(0x9331)FLD-MT接続解放要求を受信し、中継する。
 *  @note 
 *  @param  aparm_p [in] parameter pointer
 *  @return none
 *  @date 
 *  @date 2015/09/17 FPT)Quynh Fix bug IT1
 *  @date 2015/09/21 FPT)Quynh Fix bug IT1
 *	@date 2015/10/20 FPT)HaiLC Fix 対応依頼VN.txt - No. 63.
 *  @date 2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-41)
*/
/********************************************************************************************************************/

VOID m_rc_ReFldMReDiscReqRcv( CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ* aparm_p)
{
    INT                 ldwTimerStaResult;              /*result of timer start*/
    UINT                ldwL3SndResult;                 /*result of message sending*/
    USHORT              lwReNo;
    INT                 ldwErrcd;
    USHORT              lwTimVal  =  CMD_NUM0;          /* time value*/
    UINT                ldwSigchk_rslt;                 /*  result of checking signal */
    USHORT              lwSigsys_rslt;                  /*  system by checking signal */
    UINT                a_ret;	/* buffer hunt return code */
	CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ* ltFldmtCncRelReq_p;
	UINT				keyID;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
	/*In case buffer is NULL*/
	if( NULL == aparm_p )
	{

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		/*end processing*/
		return;
	}
	
	/* validate signal */
	ldwSigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)aparm_p)->signal_kind,&lwSigsys_rslt);
	
	if(ldwSigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
					lwSigsys_rslt, 
					"m_rc_ReFldMReDiscReqRcv validate signal NG");

		if (ldwSigchk_rslt == CMD_SYS_3G) {
			/* Send FLD-MT接続解放応答 to re/rec */
			m_rc_ReFldDiscRspSnd(lwSigsys_rslt, CMD_RECRS_NG);
			return;
		}
	}
	
	/* Get RE number */
	lwReNo = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)aparm_p)->cprircv_inf.link_num;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s lwReNo: %d,\n gw_rcw_FldMt_ConnDst[lwSigsys_rslt] = %d, system type: %d\ngw_rcw_FldMt_ConnSts[lwSigsys_rslt] = %d\n",
							__LINE__,__FUNCTION__, lwReNo, gw_rcw_FldMt_ConnDst[lwSigsys_rslt], lwSigsys_rslt, gw_rcw_FldMt_ConnSts[lwSigsys_rslt]);
#endif	
	
	/*内容が初期値で応答先不明(接続要求を受信していないのに解放を受信)の場合は      */
	/*or the situation that the connection destination is different from RE request release*/
	if((gw_rcw_FldMt_ConnDst[lwSigsys_rslt] != lwReNo) ||
		(gw_rcw_FldMt_ConnSts[lwSigsys_rslt] == D_L3_MT_NOT_CONNECT))
	{
		/*Send message NG to RE*/
		CMT_TSKIF_CPRISND_FLMTCNCRELRES  ltFlmtCncRelRes;

		cm_MemClr(&ltFlmtCncRelRes, sizeof(ltFlmtCncRelRes));

		/**********************/
		/* タスク間ヘッダ作成 */
		/**********************/
		/* message size (sizeof(CMT_TSKIF_CPRISND_FLMTCNCRELRES) )*/
		ltFlmtCncRelRes.head.uiLength = sizeof(CMT_TSKIF_HEAD)+ sizeof(CMT_CPRISIGRCV_INF) + sizeof(CMT_CPRIF_FLMTCNCRELRES);
		
		/*	Fill cpri signal head structure	*/
		
		/*RE number*/
		ltFlmtCncRelRes.cprisnd_inf.link_num = lwReNo; 
		ltFlmtCncRelRes.cprisnd_inf.length   = CMD_NUM4;
		/*	Fill the data structure: signal kind	*/
		ltFlmtCncRelRes.head.uiSignalkind = CMD_CPRID_FLMTCNCRELRES + lwSigsys_rslt;
		ltFlmtCncRelRes.cpridat_flmtcncrelres.signal_kind = CMD_CPRID_FLMTCNCRELRES + lwSigsys_rslt;
		/*	Result		*/
		ltFlmtCncRelRes.cpridat_flmtcncrelres.result = CMD_NML;
		/********************/
		/* Send message		*/
		/********************/
		ldwL3SndResult = l3_com_sendMsg(CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, &ltFlmtCncRelRes, sizeof(CMT_TSKIF_CPRISND_FLMTCNCRELRES));
		if(ldwL3SndResult != D_RRH_OK)
		{
			cm_Assert(	CMD_ASL_DBGLOW,
						lwSigsys_rslt, 
						"m_rc_ReFldMReDiscReqRcv l3 send NG");
		}
		
		cm_Assert(	CMD_ASL_USELOW,
					lwSigsys_rslt, 
					"m_rc_ReFldMReDiscReqRcv request release RE NG");
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		/*応答を破棄   */
		return;
	}
	/*  FLD-MT接続解放要求を中継する(l3/rct->l2/lpb) */
	
	a_ret = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ), CMD_NUM1, (void**)&ltFldmtCncRelReq_p);
	if (a_ret  != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_ReFldMReDiscReqRcv", 
			"buffer hunt fail", a_ret, sizeof(CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ), CMD_NUM0);
	}
	
	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)ltFldmtCncRelReq_p)->uiKeyID;
	
	cm_MemCpy(ltFldmtCncRelReq_p, aparm_p,  sizeof(CMT_TSKIF_CPRIRCV_FLMTCNCRELREQ));
	
	((T_SYS_COMMON_THDIF*)ltFldmtCncRelReq_p)->uiKeyID = keyID; 
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s, signal kind: 0x%x\n",__LINE__,__FUNCTION__, ltFldmtCncRelReq_p->cpridat_flmtcncrelreq.signal_kind);
#endif	
	
	ldwL3SndResult = m_cm_L3MsgSend_REC(ltFldmtCncRelReq_p);
	
	if(ldwL3SndResult != D_RRH_OK)
	{
		cm_Assert(	CMD_ASL_DBGLOW,
					lwSigsys_rslt, 
					"m_rc_ReFldMReDiscReqRcv forward NG");
	}
	
// 	/* タイマ起動(FLD-MT接続解放要求) */
// 	/* システムパラメータ情報管理テーブルよりタイマ値を取得 */
// 	lwTimVal = cmw_sys_mng_tbl[lwSigsys_rslt].fldmt_rltimer;

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s timer value: %d, timer Id: %d\n",__LINE__,__FUNCTION__,lwTimVal, (CMD_TIMID_FMRELCF*(CMD_NUM1-lwSigsys_rslt)
// 															+ CMD_TIMID_FMRELCF_S3G*lwSigsys_rslt));
// #endif
// 	ldwTimerStaResult= cm_TStat((CMD_TIMID_FMRELCF*(CMD_NUM1-lwSigsys_rslt) + CMD_TIMID_FMRELCF_S3G*lwSigsys_rslt), TIMVAL_GET( lwTimVal ), 
// 									CMD_TIMMD_SINGLE, CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &ldwErrcd ); 
// 	/*check result start timer*/
// 	if(ldwTimerStaResult != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, 
// 				 (CMD_TIMID_FMRELCF*(CMD_NUM1-lwSigsys_rslt) + CMD_TIMID_FMRELCF_S3G*lwSigsys_rslt), 
// 				 "Request Timer Start NG" );
// 	}
// 	/* 待ちフラグ(FLD-MT接続解放要求)がONに設定します */
// 	gw_rcw_FldMt_ConnRel_WaitFlg[lwSigsys_rslt] = D_L3_ON;

	/**save FLD-MT接続解放要求を送信したRE*/
	gw_rcw_FldMt_RelReqRe[lwSigsys_rslt] = lwReNo;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	/* 終了 */
	return;

}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  receive FLD-MT接続解放応答 that is sent from REC
 *  @note 
 *  @param  aparm_p [in] parameter pointer
 *  @return none
 *  @date 	2015/09/29 FPT)Yen fix bug IT2
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-41)
 */
/********************************************************************************************************************/

VOID m_rc_ReFldDiscRspRcv( CMT_TSKIF_CPRISND_FLMTCNCRELRES* aparm_p)
{
	UINT         sigchk_rslt;							/*  result of checking signal */
	USHORT       lwSigsys_rslt;							/*  system by checking signal */
	INT          ldwErrcd;
	INT          ldwTimerStpResult;						/*result of timer stop*/
	UINT		 ldwTimerId[CMD_MAX_SYSNUM] = { CMD_TIMID_FMRELCF,
												CMD_TIMID_FMRELCF_S3G };

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
	printf( "\n[%d]%s param: uiSignalKind:0x%x, link_num:%d, result:%d\n",
		__LINE__,__FUNCTION__,aparm_p->head.uiSignalkind, aparm_p->cprisnd_inf.link_num, aparm_p->cpridat_flmtcncrelres.result);
#endif
	
	/*In case buffer is NULL*/
	if( NULL == aparm_p )
	{
		/*end processing*/
		return;
	}
	
	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)aparm_p)->signal_kind,&lwSigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
					lwSigsys_rslt, 
					"m_rc_ReFldDiscRspRcv validate signal NG");

		if (sigchk_rslt == CMD_SYS_3G) {
			/* Send FLD-MT接続解放応答 to re/rec */
			m_rc_ReFldDiscRspSnd(lwSigsys_rslt, CMD_RECRS_NG);
			return;
		}
	}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s lwSigsys_rslt: %d, gw_rcw_FldMt_ConnRel_WaitFlg:%d\n",__LINE__,__FUNCTION__, lwSigsys_rslt, gw_rcw_FldMt_ConnRel_WaitFlg[lwSigsys_rslt]);
#endif

// 	/* 待ちフラグ(FLD-MT接続解放応答)チェック */
// 	/* 待ちフラグ(FLD-MT接続解放応答)がOFFの場合 */
// 	if(gw_rcw_FldMt_ConnRel_WaitFlg[lwSigsys_rslt] == D_L3_OFF)
// 	{
// 		cm_Assert(	CMD_ASL_USELOW,
// 					lwSigsys_rslt, 
// 					"[m_rc_ReFldDiscRspRcv]gw_rcw_FldMt_ConnRel_WaitFlg OFF");
// 		/* 終了 */
// 		return;
// 	}

// 	/* the situation that above flag is still ON	*/

// 	/* タイマ停止(FLD-MT接続解放応答) */
// 	ldwTimerStpResult = cm_TStop(ldwTimerId[lwSigsys_rslt], &ldwErrcd); 
// 	/*check return code*/
// 	if(ldwTimerStpResult != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, ldwTimerStpResult, 
// 					"Response forward Timer Stop NG" );
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s timer stop(timer Id: %d)\n",__LINE__,__FUNCTION__, ldwTimerId[lwSigsys_rslt]);
// #endif

// 	/* 待ちフラグ(FLD-MT接続解放応答)がOFFに設定します */
// 	gw_rcw_FldMt_ConnRel_WaitFlg[lwSigsys_rslt] = D_L3_OFF;

	/* Send FLD-MT接続解放応答 to re/rec */
	m_rc_ReFldDiscRspSnd(lwSigsys_rslt, aparm_p->cpridat_flmtcncrelres.result);

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s gw_rcw_FldMt_ConnSts: %d\n",__LINE__,__FUNCTION__, gw_rcw_FldMt_ConnSts[lwSigsys_rslt]);
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	/* 終了 */
	return;

}

/* @} */
 	
// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief FLD-MT接続解放確認タイマタイムアウト。
//  *  @note 
//  *  @param adwTimerId [in] Timer Identification 
//  *  @return none
//  *  @date 	2015/09/29 FPT)Yen fix bug IT2
//  *  @date   2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-41)
//  */
// /********************************************************************************************************************/

// VOID m_rc_ReFldReDiscReqTO(UINT adwTimerId)
// {
// 	USHORT             lwSysType;											/* system type */
// 	USHORT             lwResult[CMD_MAX_SYSNUM] = { CMD_RECRS_NG, 			/* 232: REC応答NG(3G) */
// 													CMD_REC_RSP_NG_LTE };	/* 102: REC応答NG(LTE) */
	
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s start(timerId:%d)\n",__LINE__,__FUNCTION__,adwTimerId);
// #endif
	
// 	/*get the system type */
// 	if(CMD_TIMID_FMRELCF == adwTimerId)
// 	{
// 		lwSysType = CMD_SYS_3G;
// 	}
// 	else
// 	{
// 		lwSysType = CMD_SYS_LTE;
// 	}

// 	/* Send FLD-MT接続解放応答 to re/rec */
// 	m_rc_ReFldDiscRspSnd(lwSysType, lwResult[lwSysType]);
// }

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Send FLD-MT接続解放応答 to re/rec
 *  @note 
 *  @param  awSystype [in] Systerm type
 *  @param  awResult     [in] Response result
 *  @return none
 *  @date 	2015/09/29 FPT)Yen fix bug IT2
 */
/********************************************************************************************************************/

VOID m_rc_ReFldDiscRspSnd( USHORT awSystype, USHORT awResult)
{
	CMT_TSKIF_CPRISND_FLMTCNCRELRES  ltFlmtCncRelRes;
	UINT                             ldwMsgSize;
	INT                              ldwL3SndResult;				/*result of message sending*/

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start (SysType:%d, Result:%d)\n",__LINE__,__FUNCTION__,awSystype,awResult);
#endif
	/* テーブル参照FLD-MT接続先 */
	
	/* 内容が初期値で応答先不明(接続要求を受信していないのに解放を受信)の場合は      */
	/* or the situation that the connection destination is different from RE request release*/
	if((gw_rcw_FldMt_ConnDst[awSystype] != gw_rcw_FldMt_RelReqRe[awSystype])||
	   (gw_rcw_FldMt_ConnSts[awSystype] == D_L3_MT_NOT_CONNECT))
	{
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s End (ConnDst:%d, ConnSts:%d, RelReqRe:%d) \n",__LINE__,__FUNCTION__,
			gw_rcw_FldMt_ConnDst[awSystype], gw_rcw_FldMt_ConnSts[awSystype], gw_rcw_FldMt_RelReqRe[awSystype] );
#endif
		/*応答を破棄   */
		return;
	}

	/*応答結果が「0:正常」場合*/
	if( CMD_NML == awResult )
	{
		/*テーブル更新FLD-MT接続状態←未接続*/
		gw_rcw_FldMt_ConnSts[awSystype] = D_L3_MT_NOT_CONNECT;
		gw_rcw_FldMt_ConnDst[awSystype] = CMD_NUM0;
	}

	cm_MemClr(&ltFlmtCncRelRes, sizeof(ltFlmtCncRelRes));
	
	/* message size (sizeof(CMT_TSKIF_CPRISND_FLMTCNCRELRES) )*/
	ldwMsgSize = sizeof(CMT_TSKIF_HEAD)+ sizeof(CMT_CPRISIGRCV_INF) + sizeof(CMT_CPRIF_FLMTCNCRELRES);
	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	ltFlmtCncRelRes.head.uiLength = ldwMsgSize;
	
	/*	Fill cpri signal head structure	*/
	
	/*RE number*/
	ltFlmtCncRelRes.cprisnd_inf.link_num = gw_rcw_FldMt_RelReqRe[awSystype]; 
	ltFlmtCncRelRes.cprisnd_inf.length   = CMD_NUM4;
	/*	Fill the data structure: signal kind	*/
	ltFlmtCncRelRes.head.uiSignalkind = CMD_CPRID_FLMTCNCRELRES + awSystype;
	ltFlmtCncRelRes.cpridat_flmtcncrelres.signal_kind = CMD_CPRID_FLMTCNCRELRES + awSystype;
	/*	Result		*/
	ltFlmtCncRelRes.cpridat_flmtcncrelres.result = awResult;
	/********************/
	/* Send message		*/
	/********************/
	ldwL3SndResult = l3_com_sendMsg(CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, &ltFlmtCncRelRes, ldwMsgSize );
	if(ldwL3SndResult != D_RRH_OK)
	{
		cm_Assert(	CMD_ASL_DBGLOW,
					awSystype, 
					"m_rc_ReFldDiscRspSnd l3 send NG");
	}
	gw_rcw_FldMt_RelReqRe[awSystype] = CMD_NUM0;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif

	/*終了*/
	return;
	
}

/* @} */

/* @} */

