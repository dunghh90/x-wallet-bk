/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_Fld_CmdRcv.c
 *  @brief  FLD-MTデータコマンド転送受信処理
 *  @date   2015/07/09 FPT)Tunghv
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-38)
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
 *  @brief  FLD-MTデータコマンド転送受信要求処理
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01870 
 *  @date	2015/07/09 FPT)Tunghv
 *  @date 2015/09/21 FPT)Quynh M-RRU-ZSYS-01870 Fix bug IT1 LAPB用バッファを使用するように修正
 *  @date 2015/09/29 FPT)Yen Fix bug IT2
 *  @date 2015/10/05 FPT)Yen Fix bug IT2(update size of msg FLD-MTデータコマンド転送受信要求)
 *  @date 2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-38)
 */
/********************************************************************************************************************/

VOID m_rc_Fld_CmdRcvReq( CMT_TSKIF_CPRIRCV_FLDCOMRCVREQ* parm_p )
{
	UINT        parmchk_rslt;				/*  result of checking parameter */
	UINT		sigchk_rslt;				/*  result of checking signal */
	USHORT		sigsys_rslt;				/*  system by checking signal */
	INT			ldwErrcd;
	UINT		ldwResult;
	USHORT      lwTimVal  =  CMD_NUM0;
	INT		    ldwTimerStaResult; 				/*result of timer start*/
	USHORT		lwTimerId;
	CMT_TSKIF_CPRIRCV_FLDCOMRCVREQ* ltFldCmdRcvReq_p;
 	UINT		a_ret;	/* buffer hunt return code */
	UINT		keyID;
	USHORT				lwResult[CMD_MAX_SYSNUM] = { CMD_FLD_CMDRCV_NG, 		/* 233:受信不可状態(3G) */
													 CMD_FLD_CMDRCV_NG_LTE };	/* 103:受信不可状態(LTE) */
	USHORT		lwSize;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	/* In case buffer is Null */
	if( NULL == parm_p )
	{
		/* end processing */
		return;
	}

	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_fldcomrcvreq.signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
  		cm_Assert(	CMD_ASL_USELOW, sigsys_rslt, "validate signal NG");

		if (CMD_SYS_3G == sigsys_rslt) {
			/* FLD-MTデータコマンド転送受信応答(受信不可状態)をre/recに送信する - 232:REC応答NG) */
			m_rc_Fld_CmdRcvl3SendMsg( sigsys_rslt, CMD_REC_RES_NG );
			return;
		}
	}

	/* FLD-MT not connect or Re which connect with FLD-MT is diffirent with Re request */
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s gw_rcw_FldMt_ConnSts[%d] = %d, parm_p->cprircv_inf.link_num = %d \n",
							__LINE__,__FUNCTION__,sigsys_rslt, gw_rcw_FldMt_ConnSts[sigsys_rslt],parm_p->cprircv_inf.link_num);
	printf( "[%d]%s gw_rcw_FldMt_ConnDst[%d] = %d\n",__LINE__,__FUNCTION__,sigsys_rslt, gw_rcw_FldMt_ConnDst[sigsys_rslt]);
#endif
	
	
	if(( gw_rcw_FldMt_ConnSts[sigsys_rslt] != D_L3_MT_CONNECT) ||
		( parm_p->cprircv_inf.link_num != gw_rcw_FldMt_ConnDst[sigsys_rslt] ))
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		return ;
	}

// 	/* FLD-MTデータコマンド転送受信要求データを保持する */
// 	gw_rcw_FldMt_CmdRcvReqData[sigsys_rslt].link_num 			= parm_p->cprircv_inf.link_num;
// 	gw_rcw_FldMt_CmdRcvReqData[sigsys_rslt].rcvalldat_num 		= parm_p->cpridat_fldcomrcvreq.rcvalldat_num;
// 	gw_rcw_FldMt_CmdRcvReqData[sigsys_rslt].rcvdatseq_no		= parm_p->cpridat_fldcomrcvreq.rcvdatseq_no;

// 	/* 客パラメータFLD-MTデータコマンド転送受信要求, 全受信データ数, 受信データ順序番号, 受信バイト数をチェック */
// 	/*「パラメータチェックNG 」の場合 */
// 	parmchk_rslt = m_rc_Fld_CmdRcvReqParamChk(parm_p);
// 	if(parmchk_rslt != CMD_RES_OK)
// 	{
// 		/* FLD-MTデータコマンド転送受信応答(受信不可状態)をre/recに送信する*/
// 		m_rc_Fld_CmdRcvl3SendMsg( sigsys_rslt, lwResult[sigsys_rslt] );
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 		return ;
// 	}

	/* Msg Length(Head部 + Data部) */
	lwSize = parm_p->head.uiLength;

	a_ret = cm_L2BfGet(CMD_BUFCA_LAPSND, lwSize, CMD_NUM1, (void**)&ltFldCmdRcvReq_p);
	if (a_ret  != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, "m_rc_Fld_CmdRcvReq", 
			"buffer hunt fail", a_ret, sizeof(CMT_TSKIF_CPRIRCV_FLDCOMRCVREQ), CMD_NUM0);
	}
	/*temp save key*/
	keyID = ((T_SYS_COMMON_THDIF*)ltFldCmdRcvReq_p)->uiKeyID;
	
	cm_MemCpy(ltFldCmdRcvReq_p, parm_p, lwSize);
	
	((T_SYS_COMMON_THDIF*)ltFldCmdRcvReq_p)->uiKeyID = keyID;

	/* FLD-MTデータコマンド転送受信要求をL2/lpbに送信する*/
	ldwResult = m_cm_L3MsgSend_REC( ltFldCmdRcvReq_p );
	/*Sending fail*/
	if( CMD_L3_OK != ldwResult )
	{
		cm_Assert(	CMD_ASL_USELOW, ldwResult, "Send msg to L2/lpb fail");
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		return ;
	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == sigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_FMCOMRXCF_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_FMCOMRXCF_LTE;
// 	}

// 	/* FLD-MTデータコマンド転送受信応答確認タイマ起動 */
// 	/* システムパラメータ情報管理テーブルよりタイマ値を取得 */
// 	lwTimVal = cmw_sys_mng_tbl[sigsys_rslt].fldmt_dtcd_timer;

// 	ldwTimerStaResult= cm_TStat(lwTimerId,
// 								TIMVAL_GET( lwTimVal ), 
// 								CMD_TIMMD_SINGLE, CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &ldwErrcd ); 
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s Start timer (timerId:%d)\n",__LINE__,__FUNCTION__,lwTimerId);
// #endif
// 	/*check result start timer*/
// 	if(ldwTimerStaResult != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "Request Timer Start NG" );
// 	}

// 	/* FLD-MTデータコマンド転送受信応答待ちフラグ設定にONを設定 */
// 	gw_rcw_FldMt_CmdRes_WaitFlg[sigsys_rslt] = D_L3_ON;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	return ;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  FLD-MTデータコマンド転送受信応答処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/08 FPT)Tunghv
 *  @date   2015/10/07 TDI)satou [FHM技説-QA No.50] 応答結果はRECから受信したものを透過するように
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-38)
 */
/********************************************************************************************************************/

VOID m_rc_Fld_CmdRcvRes( CMT_TSKIF_CPRIRCV_FLDCOMRCVRES* parm_p )
{
	UINT		sigchk_rslt;				/*  result of checking signal */
	USHORT		sigsys_rslt;				/*  system by checking signal */
	INT			errcd;
	UINT		ldwResult; 
	USHORT		lwTimerId;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	/* In case buffer is Null */
	if( NULL == parm_p )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		/* end processing */
		return;
	}

	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_fldcomrcvres.signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
  		cm_Assert(	CMD_ASL_USELOW, sigsys_rslt, "validate signal NG");

		/* FLD-MTデータコマンド転送受信応答をre/recに送信する */
		m_rc_Fld_CmdRcvl3SendMsg( sigsys_rslt, parm_p->cpridat_fldcomrcvres.result );
		return;
	}
	
// 	/*If not wait response*/
// 	if( D_L3_OFF == gw_rcw_FldMt_CmdRes_WaitFlg[sigsys_rslt] )
// 	{
// 		cm_Assert(	CMD_ASL_USELOW, gw_rcw_FldMt_CmdRes_WaitFlg[sigsys_rslt], "Not wait response");
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
		
// 		return ;
// 	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == sigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_FMCOMRXCF_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_FMCOMRXCF_LTE;
// 	}

// 	/* FLD-MTデータコマンド転送受信要求データを保持する */
// 	gw_rcw_FldMt_CmdRcvReqData[sigsys_rslt].rcvalldat_num 	= parm_p->cpridat_fldcomrcvres.rcvalldat_num;
// 	gw_rcw_FldMt_CmdRcvReqData[sigsys_rslt].rcvdatseq_no	= parm_p->cpridat_fldcomrcvres.rcvdatseq_no;

// 	/* FLD-MTデータコマンド転送受信応答確認タイマ停止 */
// 	ldwResult = cm_TStop( lwTimerId, &errcd);

// 	/* Stop timer NG */
// 	if( ldwResult != CMD_RES_OK )
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, sigsys_rslt, "FLD-MTデータコマンド転送受信応答確認タイマ停止 NG" );
// 	}

    /* FLD-MTデータコマンド転送受信応答をre/recに送信する */
    m_rc_Fld_CmdRcvl3SendMsg( sigsys_rslt, CMD_REC_RES_NG );

	/* FLD-MTデータコマンド転送受信応答待ちフラグ設定にOFFを設定 */
//	gw_rcw_FldMt_CmdRes_WaitFlg[sigsys_rslt] = D_L3_OFF;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	return ;
}

/* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief  FLD-MTデータコマンド転送受信応答確認タイムアウト処理(LTE/3G)
//  *  @note 
//  *  @param  adwTimerId [in] タイマ ID
//  *  @return None
//  *  @date	2015/07/08 FPT)Tunghv
//  *  @date 	2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-38)
//  */
// /********************************************************************************************************************/

// VOID m_rc_Fld_CmdRcvResConfirmTo( UINT adwTimerId )
// {
// 	USHORT		sigsys_rslt;				/*  system by checking signal */
// 	USHORT		lwResult[CMD_MAX_SYSNUM] = { CMD_RECRS_NG, 			/* 232: REC応答NG(3G) */
// 											CMD_REC_RSP_NG_LTE };	/* 102: REC応答NG(LTE) */
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
// #endif
// 	/*タイマIdが３G タイマの場合*/
// 	if( CMD_TIMID_FMCOMRXCF_3G == adwTimerId )
// 	{
// 		sigsys_rslt = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		sigsys_rslt = CMD_SYS_LTE;
// 	}

// 	/*If not wait response*/
// 	if( D_L3_OFF == gw_rcw_FldMt_CmdRes_WaitFlg[sigsys_rslt] )
// 	{
// 		cm_Assert(	CMD_ASL_USELOW, gw_rcw_FldMt_CmdRes_WaitFlg[sigsys_rslt], "Not wait response");
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 		return ;
// 	}

// 	/* FLD-MTデータコマンド転送受信応答(REC応答NG)をre/recに送信する*/
// 	m_rc_Fld_CmdRcvl3SendMsg( sigsys_rslt, lwResult[sigsys_rslt] );

// 	/* FLD-MTデータコマンド転送受信応答待ちフラグ設定にOFFを設定 */
// 	gw_rcw_FldMt_CmdRes_WaitFlg[sigsys_rslt] = D_L3_OFF;
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 	return ;
// }
// /* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief  FLD-MTデータコマンド転送受信要求信号をチェック
//  *  @note 
//  *  @param  parm_p [in] the buffer address pointer of received message
//  *  @return RETURN CODE
//  *  @date	2015/07/09 FPT)Tunghv
//  *  @date 	2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-38)
//  */
// /********************************************************************************************************************/

// UINT m_rc_Fld_CmdRcvReqParamChk( CMT_TSKIF_CPRIRCV_FLDCOMRCVREQ* parm_p )
// {
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s start\n",__LINE__,__FUNCTION__);
// #endif
// 	/* In case buffer is Null */
// 	if( NULL == parm_p )
// 	{
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 		/* end processing */
// 		return CMD_RES_PARAMERR;
// 	}

// 	/* FLD-MTデータコマンド転送受信要求は範囲外の場合 */
// 	if((parm_p->cpridat_fldcomrcvreq.signal_kind & CMD_SYS_MSK_CPRID) != CMD_CPRID_FLDCOMRCVREQ)
// 	{
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 		return CMD_RES_PARAMERR;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s parm_p->cpridat_fldcomrcvreq.rcvalldat_num = %d\n",__LINE__,__FUNCTION__,parm_p->cpridat_fldcomrcvreq.rcvalldat_num);
// #endif
// 	/* 全受信データ数は範囲外の場合 */
// 	if(parm_p->cpridat_fldcomrcvreq.rcvalldat_num < CMD_NUM1 ||			/* If all receive data number is smaller than 1, or	*/
// 	   parm_p->cpridat_fldcomrcvreq.rcvalldat_num > CMD_FLDCMD_MAXBLK)	/* all receive data number is bigger than allowed	*/
// 	{
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 		return CMD_RES_PARAMERR;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s parm_p->cpridat_fldcomrcvreq.rcvdatseq_no = %d\n",__LINE__,__FUNCTION__, parm_p->cpridat_fldcomrcvreq.rcvdatseq_no);
// #endif
// 	/* 受信データ順序番号は範囲外の場合 */
// 	if(parm_p->cpridat_fldcomrcvreq.rcvdatseq_no < CMD_NUM1 ||			/* If receive data numbering is smaller than 1, or	*/
// 	   parm_p->cpridat_fldcomrcvreq.rcvdatseq_no > CMD_FLDCMD_MAXBLK)	/* receive data numbering is bigger than allowed	*/
// 	{
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 		return CMD_RES_PARAMERR;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s parm_p->cpridat_fldcomrcvreq.rcvbyte_num = %d\n",__LINE__,__FUNCTION__, parm_p->cpridat_fldcomrcvreq.rcvbyte_num);
// #endif
// 	/* 受信バイト数は範囲外の場合 */
// 	if(parm_p->cpridat_fldcomrcvreq.rcvbyte_num < CMD_NUM1 ||			/* If receive byte number is smaller than 1, or	*/
// 	   parm_p->cpridat_fldcomrcvreq.rcvbyte_num > CMD_FLDCMD_BLKLEN)	/* receive byte number is bigger than allowed	*/
// 	{
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 		return CMD_RES_PARAMERR;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
// 	return CMD_RES_OK;
// }

// /* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  The function of sending respone message from l3/rct to re/rec for FLD-MTデータコマンド転送受信処理
 *  @note 
 *  @param  awSigsys    [in] the system signal type
 *  @param  awResult    [in] the result of respone
 *  @return RETURN CODE
 *  @date	2015/07/09 FPT)Tunghv
 *  @date	2015/09/29 FPT) Yen fix bug IT2
 */
/********************************************************************************************************************/

INT m_rc_Fld_CmdRcvl3SendMsg( USHORT awSigsys, USHORT awResult )
{
	CMT_TSKIF_CPRIRCV_FLDCOMRCVRES ltMsgRsp;	/* タスク間フレームポインタ		*/
	INT					liRet;					//

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start(Systype: %d, Result:%d)\n",__LINE__,__FUNCTION__,awSigsys,awResult );
#endif
	/*FLD-MT not connect*/
	if(gw_rcw_FldMt_ConnSts[awSigsys] != D_L3_MT_CONNECT)
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		return D_RRH_NG;
	}
	/*Re which send request is not connect with FLD-MT*/
	if(gw_rcw_FldMt_CmdRcvReqData[awSigsys].link_num != gw_rcw_FldMt_ConnDst[awSigsys])
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		return D_RRH_NG;
	}

	cm_MemClr(&ltMsgRsp, sizeof(ltMsgRsp));
	/********************/
	/* CPRI番号設定 */
	/********************/
	ltMsgRsp.cprircv_inf.link_num = gw_rcw_FldMt_CmdRcvReqData[awSigsys].link_num;

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	ltMsgRsp.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF)	+	/* CPRI信号送信情報部 */
							sizeof(CMT_CPRIF_FLDCOMRCVRES);

	/********************/
	/* CPRI信号種別設定 */
	/********************/
	ltMsgRsp.head.uiSignalkind = CMD_CPRID_FLDCOMRCVRES + awSigsys;
	ltMsgRsp.cpridat_fldcomrcvres.signal_kind = CMD_CPRID_FLDCOMRCVRES + awSigsys;

	/*******************************/
	/* CPRI信号応答結果設定 */
	/*******************************/
	ltMsgRsp.cpridat_fldcomrcvres.result = awResult;

	/******************************/
	/* CPRI信号全受信データ数設定 */
	/******************************/
	ltMsgRsp.cpridat_fldcomrcvres.rcvalldat_num = gw_rcw_FldMt_CmdRcvReqData[awSigsys].rcvalldat_num;

	/**********************************/
	/* CPRI信号受信データ順序番号設定 */
	/**********************************/
	ltMsgRsp.cpridat_fldcomrcvres.rcvdatseq_no = gw_rcw_FldMt_CmdRcvReqData[awSigsys].rcvdatseq_no;

	/* FLD-MTデータコマンド転送受信要求データを削除する */
	memset(&gw_rcw_FldMt_CmdRcvReqData[awSigsys], 0, sizeof(gw_rcw_FldMt_CmdRcvReqData[awSigsys]));
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s System type: %d, Signal kind: %x, Result: %d \n",__LINE__,__FUNCTION__,awSigsys, ltMsgRsp.cpridat_fldcomrcvres.signal_kind,
																		ltMsgRsp.cpridat_fldcomrcvres.result);
#endif
	/* FLD-MTデータコマンド転送受信応答(233:受信不可状態)をre/recに送信する*/
	liRet = l3_com_sendMsg(CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, &ltMsgRsp, sizeof(CMT_TSKIF_CPRISND_FLDCOMRCVRES));
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	return liRet;
}

/* @} */

/* @} */

