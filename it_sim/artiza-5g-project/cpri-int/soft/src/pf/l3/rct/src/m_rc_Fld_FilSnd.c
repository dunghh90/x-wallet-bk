/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_Fld_FilSnd.c
 *  @brief  FLD-MTデータファイル転送送信処理
 *  @date   2015/07/10
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-39)
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
 *  @brief  FLD-MTデータファイル転送送信要求処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/10 FPT)Tunghv
 *  @date	2015/10/05 FPT)Yen Fix bug IT2(update size of msg  FLD-MTデータファイル転送送信要求)
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-39)
 */
/********************************************************************************************************************/

VOID m_rc_Fld_FilSndReq( VOID* parm_p )
{
	UINT		sigchk_rslt;				/*  result of checking signal */
	USHORT		sigsys_rslt;				/*  system by checking signal */
	USHORT		sndbyte_num;				/**< 送信バイト数				 */
	INT			errcd;
	USHORT		lwTimerId;
	INT			ldwRet;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
	/* In case buffer is Null */
	if( NULL == parm_p )
	{
		/* end processing */
		return;
	}

	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *) parm_p)->signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
  		cm_Assert(	CMD_ASL_USELOW, sigsys_rslt, "validate signal NG");
	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == sigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_FLD_FILE_SND_3G;
// 		sndbyte_num = ((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ *)parm_p)->cpridat_fldfilsndreq.sndbyte_num;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_FLD_FILE_SND_LTE;
// 		sndbyte_num = ((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ_S3G *)parm_p)->cpridat_fldfilsndreq.sndbyte_num;
// 	}

	/* 送信バイト数は範囲外の場合 */
	sndbyte_num = ((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ_S3G *)parm_p)->cpridat_fldfilsndreq.sndbyte_num;
	if((sndbyte_num < CMD_NUM1) ||			/* If send byte number is smaller than 1, or	*/
		(sndbyte_num > CMD_FLDFIL_MXLEN))		/* send byte number is bigger than allowed		*/
	{
		/* FLD-MTデータファイル転送送信応答(225:送信不可状態)をRECに送信する */
		m_rc_Fld_FilSndL3SendMsg(sigsys_rslt, CMD_FLD_FILSND_NG);
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end(sndbyte_num:%d)\n",__LINE__,__FUNCTION__,sndbyte_num);
#endif
		return;
	}
	/* FLD-MT接続状態が未接続の場合 */
	if ( D_L3_MT_NOT_CONNECT == gw_rcw_FldMt_ConnSts[sigsys_rslt] )	
	{
		/* FLD-MTデータファイル転送送信応答(225:送信不可状態)をRECに送信する */
		m_rc_Fld_FilSndL3SendMsg(sigsys_rslt, CMD_FLD_FILSND_NG);

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		return;
	}

// 	/* FLD-MTデータファイル転送送信要求のCPRI番号設定-->FLD-MT接続先 */
// 	/* 3G system*/
// 	if( CMD_SYS_3G == sigsys_rslt)
// 	{
// 		((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ *)parm_p)->cprircv_inf.link_num = gw_rcw_FldMt_ConnDst[sigsys_rslt];
		
// 		/* FLD-MTデータファイル転送送信要求をre/recに送信する */
// 		ldwRet = l3_com_sendMsg(CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,
// 			parm_p, ((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ *)parm_p)->head.uiLength);
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ_S3G *)parm_p)->cprircv_inf.link_num = gw_rcw_FldMt_ConnDst[sigsys_rslt];
		
// 		/* FLD-MTデータファイル転送送信要求をre/recに送信する */
// 		ldwRet = l3_com_sendMsg(CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,
// 			parm_p, ((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ_S3G *)parm_p)->head.uiLength);
// 	}

	/* FLD-MTデータファイル転送送信要求のCPRI番号設定-->FLD-MT接続先 */
	((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ_S3G *)parm_p)->cprircv_inf.link_num = gw_rcw_FldMt_ConnDst[sigsys_rslt];
	
	/* FLD-MTデータファイル転送送信要求をre/recに送信する */
	ldwRet = l3_com_sendMsg(CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,
		parm_p, ((CMT_TSKIF_CPRIRCV_FLDFILSNDREQ_S3G *)parm_p)->head.uiLength);
	
	/*Send msg fail*/
	if( D_RRH_OK != ldwRet )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwRet, "Send FLD-MTデータファイル転送送信要求 NG" );
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		return;
	}

// 	/* FLD-MTデータファイル転送送信確認タイマ起動 */
// 	ldwRet = cm_TStat(lwTimerId, CMD_TIMVAL_FMFILSNDCF, CMD_TIMMD_SINGLE,
// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]cm_TStat(timerId:%d), Result: %d \n",__LINE__,lwTimerId, ldwRet);
// #endif
// 	/* Start timer NG */
// 	if(ldwRet != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "FLD-MTデータファイル転送送信確認タイマ起動 NG" );
// 		return;
// 	}
// 	/* FLD-MTデータファイル転送送信応答待ちフラグ設定-->ON */
// 	gw_rcw_FldMt_FilRes_WaitFlg[sigsys_rslt] = D_L3_ON;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  FLD-MTデータファイル転送送信応答処理
 *  @note   None
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/10 FPT)Tunghv
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-39)
 */
/********************************************************************************************************************/

VOID m_rc_Fld_FilSndRes( CMT_TSKIF_CPRIRCV_FLDFILSNDRES* parm_p )
{
	UINT		sigchk_rslt;				/*  result of checking signal */
	USHORT		sigsys_rslt;				/*  system by checking signal */
	INT			errcd;
	USHORT		lwTimerId;
	UINT		ldwResult; 

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
	/* In case buffer is Null */
	if( NULL == parm_p )
	{
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		/* end processing */
		return;
	}

	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_fldfilsndres.signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
  		cm_Assert(	CMD_ASL_USELOW, sigsys_rslt, "validate signal NG");
	}
	
// 	/* FLD-MTデータファイル転送送信応答待ちフラグ設定はOFFの場合 */
// 	if(gw_rcw_FldMt_FilRes_WaitFlg[sigsys_rslt] == D_L3_OFF)
// 	{
// 		cm_Assert(	CMD_ASL_USELOW, gw_rcw_FldMt_FilRes_WaitFlg[sigsys_rslt], "Not wait reponse");
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 		return;	
// 	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == sigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_FLD_FILE_SND_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_FLD_FILE_SND_LTE;
// 	}

// 	/* FLD-MTデータファイル転送送信確認タイマ停止 */
// 	ldwResult = cm_TStop( lwTimerId, &errcd);

// 	/* Stop timer NG */
// 	if( ldwResult != CMD_RES_OK )
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, sigsys_rslt, "FLD-MTデータファイル転送送信確認タイマ停止 NG" );
// 	}

	/* 配下REの応答結果が「0:正常」の場合 */
	if(parm_p->cpridat_fldfilsndres.result == CMD_OK)
	{
		/* FLD-MTデータファイル転送送信応答(0:正常)をRECに送信する */
		m_rc_Fld_FilSndL3SendMsg(sigsys_rslt, CMD_NML);
	}
	/*  配下REの応答結果が「0:正常」以外の場合 */
	else 
	{
		/* FLD-MTデータファイル転送送信応答(224:配下RE応答NG)をRECに送信する */
		m_rc_Fld_FilSndL3SendMsg(sigsys_rslt, CMD_MAKER_CODE);
	}

//	/* FLD-MTデータファイル転送送信応答待ちフラグ設定-->OFF */
//	gw_rcw_FldMt_FilRes_WaitFlg[sigsys_rslt] = D_L3_OFF;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	return;
}

/* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief  FLD-MTデータファイル転送送信確認タイムアウト処理(LTE/3G)
//  *  @note 
//  *  @param  awTimerId [in] タイマ ID
//  *  @return None
//  *  @date	2015/07/08 FPT)Tunghv
 // *  @date 	2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-39)
//  */
// /********************************************************************************************************************/

// VOID m_rc_Fld_FilSndResConfirmTo( USHORT awTimerId )
// {
// 	USHORT		sigsys_rslt;				/*  system by checking signal */

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s start (timerId:%d)\n",__LINE__,__FUNCTION__,awTimerId);
// #endif
	
// 	/*タイマIdが３G タイマの場合*/
// 	if( CMD_TIMID_FLD_FILE_SND_3G == awTimerId )
// 	{
// 		sigsys_rslt = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		sigsys_rslt = CMD_SYS_LTE;
// 	}

// 	/* FLD-MTデータファイル転送送信応答待ちフラグ設定はOFFの場合 */
// 	if( D_L3_OFF == gw_rcw_FldMt_FilRes_WaitFlg[sigsys_rslt] )
// 	{
// 		cm_Assert(	CMD_ASL_USELOW, gw_rcw_FldMt_FilRes_WaitFlg[sigsys_rslt], "Not wait reponse");
		
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
		
// 		return;	
// 	}

// 	/* FLD-MTデータファイル転送送信応答(224:配下RE応答NG)をRECに送信する*/
// 	m_rc_Fld_FilSndL3SendMsg(sigsys_rslt, CMD_MAKER_CODE);

// 	/* FLD-MTデータファイル転送送信応答待ちフラグ設定-->OFF */
// 	gw_rcw_FldMt_FilRes_WaitFlg[sigsys_rslt] = D_L3_OFF;

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
	
// 	return;
// }
// /* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  The function of sending respone message from l3/rct to REC for FLD-MTデータファイル転送送信処理
 *  @note 
 *  @param  sigsys        [in] the system signal type
 *  @param  result        [in] the result of respone
 *  @return RETURN CODE
 *  @date	2015/07/09 FPT)Tunghv
 */
/********************************************************************************************************************/

INT m_rc_Fld_FilSndL3SendMsg( USHORT sigsys, USHORT result )
{
	CMT_TSKIF_CPRISND_FLDFILSNDRES *rsp_p;	/* タスク間フレームポインタ		*/
	UINT		rslt;						/* バッファ獲得結果				*/
	UINT		ldwResult;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
	/**********/
	/* 初期化 */
	/**********/

	/* ポインタ初期化 */
	rsp_p = NULL;

	/****************/
	/* バッファ獲得 */
	/****************/
	rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_FLDFILSNDRES),
								CMD_NUM1,
								(VOID **)&rsp_p	);
	/********************/
	/* バッファ獲得失敗 */
	/********************/
	if(rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					rslt,
					"cm_L2BfGet NG");
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		return D_RRH_NG;
	}

	/********************/
	/* CPRI信号種別設定 */
	/********************/
	rsp_p->cpridat_fldfilsndres.signal_kind = CMD_CPRID_FLDFILSNDRES + sigsys;
	
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)
									+	sizeof(CMT_CPRIF_FLDFILSNDRES);

	/*******************************/
	/* CPRI信号応答結果設定 */
	/*******************************/
	rsp_p->cpridat_fldfilsndres.result = result;

	/* FLD-MTデータファイル転送送信応答(224:配下RE応答NG)をL2/lpbに送信する*/
	ldwResult = m_cm_L3MsgSend_REC( rsp_p );
	/*Sending fail*/
	if( CMD_L3_OK != ldwResult )
	{
		cm_Assert(	CMD_ASL_USELOW, ldwResult, "Send msg to L2/lpb fail");
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		return D_RRH_NG;
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	return D_RRH_OK;
}

/* @} */

/* @} */

