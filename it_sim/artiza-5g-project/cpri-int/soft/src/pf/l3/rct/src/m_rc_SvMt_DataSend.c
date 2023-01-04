/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_SvMt_Connect.c
 *  @brief  SV-MT/eNB-MTデータ送信
 *  @date   2015/07/21 Son)FPT
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-44)
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
 *  @brief SV-MT/eNBデータ送信要求処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/21 Son)FPT
 *  @date   2015/10/05 FPT)Yen Fix bug IT2(update size of msg SV-MT/eNBデータ送信要求)
 *	@date	2015/10/20 FPT)HaiLC Fix BUG_71_IT2_13-03-02
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-44)
 */
/********************************************************************************************************************/

VOID  m_rc_SvMt_DatSndReq( CMT_TSKIF_CPRIRCV_SVMTDATSNDREQ* parm_p )
{
	UINT		sigchk_rslt;						/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	INT			errcd;
	INT			ldwResult; 
	USHORT		lwTimerId;
	USHORT		lwRtc;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s satrt\n ", __LINE__, __FUNCTION__);
#endif
	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}
	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_svmtdatsndreq.signal_kind,&lwSigsys_rslt);
	
	/* signalがNGの場合 */
	if(sigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");

		if (CMD_SYS_3G == lwSigsys_rslt) {
			/* SV-MTデータ送信応答(235:その他エラー(FHM)) */
			m_rc_SvMt_DataSndResSend( CMD_OTH_ERR, lwSigsys_rslt );
			return;
		}
	}

	/* Check parameter of SV-MT/eNBデータ送信要求 */
	lwRtc = m_rc_SvMt_DatSndReqParaCheck( parm_p, lwSigsys_rslt );
	if( lwRtc != CMD_RES_OK)
	{	/* send message response to REC */
		if( CMD_SYS_LTE == lwSigsys_rslt)
		{
			/* eNB-MTデータ送信応答(51:処理NG(RE)) */
			m_rc_SvMt_DataSndResSend( CMD_HDL_NG, lwSigsys_rslt );
		}
		/* 3G system*/
		else
		{
			/* SV-MTデータ送信応答(235:その他エラー(FHM)) */
			m_rc_SvMt_DataSndResSend( CMD_OTH_ERR, lwSigsys_rslt );
		}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s gw_rcw_SvEnbMt_ConnSts[%d]= %d\n ", __LINE__, __FUNCTION__, lwSigsys_rslt, gw_rcw_SvEnbMt_ConnSts[lwSigsys_rslt]);
#endif
	/* テーブル参照 SV-MT/eNB-MT接続状態 */
	/* if SV-MT/eNB-MT接続状態が未接続 */
	if(gw_rcw_SvEnbMt_ConnSts[lwSigsys_rslt] == D_L3_MT_NOT_CONNECT)
	{ 
		/* send message response to REC */
		/* message content [SV-MTデータ送信応答(225:送信不可状態)
					   eNB-MTデータ送信応答(225:送信不可状態)] */
		m_rc_SvMt_DataSndResSend( CMD_FLD_FILSND_NG, lwSigsys_rslt );
		return ;
	}
	
	/*Set link num*/
	parm_p->cprircv_inf.link_num = gw_rcw_SvEnbMt_ConnDst[lwSigsys_rslt];
	/* send SV-MTデータ送信要求 to rec */
	ldwResult = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, ((CMT_TSKIF_CPRIRCV_SVMTDATSNDREQ *)parm_p)->head.uiLength );
	/*Sending fail*/
	if( D_RRH_OK != ldwResult)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							ldwResult, 
							"Sending msg NG");
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_SVMTDATSND_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_SVMTDATSND_LTE;
// 	}

// 	/* タイマ起動 SV-MT/eNB-MTデータ送信確認タイマ */
// 	ldwResult = cm_TStat(lwTimerId, CMD_TIMVAL_SVMT_DATA_SND, CMD_TIMMD_SINGLE,
// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s StartTimer(lwTimerId = %d, Result:%d)\n ", __LINE__, __FUNCTION__, lwTimerId,ldwResult);
// #endif
// 	/*Start timer NG*/
// 	if(ldwResult != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "SV-MT/eNB-MT接続強制解放確認タイマ Start NG" );
// 		return;
// 	}
	
// 	/* SV-MT/eNBデータ送信応答待ちフラグ -> ON */
// 	gw_rcw_SvMt_DataSnd_WaitFlg[lwSigsys_rslt] =  D_L3_ON ;

#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s gw_rcw_SvMt_DataSnd_WaitFlg[%d] = %d\n ", __LINE__, __FUNCTION__, lwSigsys_rslt, gw_rcw_SvMt_DataSnd_WaitFlg[lwSigsys_rslt]);
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief SV-MT/eNBデータ送信応答処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/21 Son)FPT
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-44)
 */
/********************************************************************************************************************/

VOID m_rc_SvMt_DatSndRes( CMT_TSKIF_CPRIRCV_SVMTDATSNDRES* parm_p )
{
	UINT		sigchk_rslt;						/*  result of checking signal */
	USHORT		lwSigsys_rslt;						/*  system by checking signal */
	USHORT		lwTimerId;							/* 	Timer ID 				  */
	UINT		ldwResult; 
	int 		errcd;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif
	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		/*end processing*/
		return;
	}

	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_svmtdatsndres.signal_kind,&lwSigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}

// 	/* SV-MT/eNBデータ送信応答待ちフラグがOFFの場合 */
// 	if(gw_rcw_SvMt_DataSnd_WaitFlg[lwSigsys_rslt] ==  D_L3_OFF )
// 	{
// 		cm_Assert(	CMD_ASL_USELOW,
// 							lwSigsys_rslt, 
// 							"SV-MT/eNB-MT接続応答待ちフラグがOFFの場合 NG");
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
// #endif
// 		return;
// 	}
// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_SVMTDATSND_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_SVMTDATSND_LTE;
// 	}
// 	/* タイマ停止 SV-MT/eNB-MTデータ送信確認タイマ */
// 	ldwResult = cm_TStop( lwTimerId, &errcd);
// 	/* Stop timer NG */
// 	if( ldwResult != CMD_RES_OK )
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "SV-MT/eNB-MT接続強制解放確認タイマ Stop NG" );
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s StopTimer(TimerId:%d)\n ", __LINE__, __FUNCTION__,lwTimerId);
// #endif	

	/* 配下REの応答結果が「0:正常」*/
	if(parm_p->cpridat_svmtdatsndres.result == CMD_RES_OK)
	{
		/* SV-MT/eNB接続応答(0：正常)をRECに送信する */
		m_rc_SvMt_DataSndResSend(CMD_NML, lwSigsys_rslt );
	}
	/* 配下REの応答結果が「0:正常」以外 */
	else
	{
		/*SV-MT/eNB接続応答(224:配下RE応答NG)をRECに送信する */
		m_rc_SvMt_DataSndResSend(CMD_FLD_FILSND_RE_NG, lwSigsys_rslt );
	}

	/* SV-MT/eNB接続応答待ちフラグ -> OFF */
//	gw_rcw_SvMt_DataSnd_WaitFlg[lwSigsys_rslt] =  D_L3_OFF;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
//	printf("[%d]%s gw_rcw_SvMt_DataSnd_WaitFlg[%d] = %d\n ", __LINE__, __FUNCTION__, lwSigsys_rslt, gw_rcw_SvMt_DataSnd_WaitFlg[lwSigsys_rslt]);
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
}

/* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief SV-MT/eNB-MTデータ送信確認タイマタイムアウト処理
//  *  @note 
//  *  @param  awTimerId [in] TimerId
//  *  @return None
//  *  @date 	2015/07/21 Son)FPT
//  *  @date 	2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-44)
//  */
// /********************************************************************************************************************/

// VOID m_rc_SvMt_DataSndTo( USHORT awTimerId )
// {
// 	USHORT		lwSystemType;
	
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s start(TimerId:%d)\n ", __LINE__, __FUNCTION__,awTimerId);
// #endif
// 	/*タイマIdが３G タイマの場合*/
// 	if( CMD_TIMID_SVMTDATSND_3G == awTimerId )
// 	{
// 		lwSystemType = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		lwSystemType = CMD_SYS_LTE;
// 	}
// 	/* SV-MT/eNBデータ送信応答待ちフラグがOFFの場合 */
// 	if(gw_rcw_SvMt_DataSnd_WaitFlg[lwSystemType] == D_L3_OFF )
// 	{
// 		cm_Assert(	CMD_ASL_USELOW,
// 					lwSystemType,
// 					"SV-MT/eNB-MT接続応答待ちフラグがOFFの場合 NG");
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
// #endif
// 		return;
// 	}
// 	/* SV-MT/eNB接続応答(224:配下RE応答NG)をRECに送信する */
// 	m_rc_SvMt_DataSndResSend(CMD_FLD_FILSND_RE_NG,lwSystemType );
// 	/* SV-MT/eNB接続応答待ちフラグ -> OFF */
// 	gw_rcw_SvMt_DataSnd_WaitFlg[lwSystemType] =  D_L3_OFF;
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s end gw_rcw_SvMt_DataSnd_WaitFlg[%d] = %d\n ", __LINE__, __FUNCTION__, lwSystemType, gw_rcw_SvMt_DataSnd_WaitFlg[lwSystemType]);
// #endif
// }
// /* @} */

/* @{ */

/********************************************************************************************************************/
/**
 *  @brief   SV-MT/eNBデータ送信応答送信処理
 *  @note 
 *  @param  awResult [in] Response result
*   @param  awSystem [in] System type
 *  @return None
 *  @date	2015/07/21 Son)FPT
 */
/********************************************************************************************************************/

VOID m_rc_SvMt_DataSndResSend( USHORT awResult, USHORT awSystem )
{
	CMT_TSKIF_CPRIRCV_SVMTDATSNDRES*	ltRes_p;
	UINT   bufget_rslt;						/* バッファ獲得結果				*/
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif
	/****************/
	/* バッファ獲得 */
	/****************/
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRIRCV_SVMTDATSNDRES),
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
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}
	
	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	ltRes_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)
									+	sizeof(CMT_CPRIF_SVMTDATSNDRES);

	/* CPRI信号応答結果にOKを設定 */
	ltRes_p->cpridat_svmtdatsndres.result = awResult;
	
	/* CPRI信号種別設定 */
	ltRes_p->cpridat_svmtdatsndres.signal_kind = CMD_CPRID_SVMTDATSNDRES + awSystem;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s Signal_kind= %x  Resutl= %d\n ", __LINE__, __FUNCTION__, ltRes_p->cpridat_svmtdatsndres.signal_kind, awResult);
#endif	
	/*ファイルデータ送信完了報告応答を送信*/
	m_cm_L3MsgSend_REC( ltRes_p );
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return;
}
/* @} */

/* @{ */
	
/********************************************************************************************************************/
/**
 *  @brief  Check parameter of SV-MT/eNBデータ送信要求
 *  @note 
 *  @param  parm_p [in] Req msg buffer
*   @param  awSystemType [in] System type
 *  @return None
 *  @date	2015/07/21 Son)FPT
 */
/********************************************************************************************************************/

USHORT m_rc_SvMt_DatSndReqParaCheck( CMT_TSKIF_CPRIRCV_SVMTDATSNDREQ* parm_p, USHORT awSystemType )
{
	UINT ldwRtc = CMD_RES_OK;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ",__LINE__, __FUNCTION__);
	printf("[%d]%s snd_bytnum= %d\n SystemType= %d",__LINE__, __FUNCTION__, parm_p->cpridat_svmtdatsndreq.snd_bytnum, awSystemType);
#endif
	
	/*3G system*/
	if( CMD_SYS_3G == awSystemType )
	{
		/* 送信バイト数 > 2048*/
		if( (parm_p->cpridat_svmtdatsndreq.snd_bytnum > MAX_MSG3G_SIZE) ||
			(parm_p->cpridat_svmtdatsndreq.snd_bytnum < MIN_MSG3G_SIZE))
		{
			ldwRtc = CMD_RES_NG;
		}
	}
	/* LTE system*/
	else
	{
		/* 送信バイト数 > 10240*/
		if(( parm_p->cpridat_svmtdatsndreq.snd_bytnum > MAX_MSGLTE_SIZE )||
			( parm_p->cpridat_svmtdatsndreq.snd_bytnum < MIN_MSGLTE_SIZE ))
		{
			ldwRtc = CMD_RES_NG;
		}
	}
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return ldwRtc;
}
/* @} */


/* @} */

