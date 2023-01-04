/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_FldMt_DataCmd.c
 *  @brief  FLD-MTデータコマンド転送送信処理
 *  @date   
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-37)
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
 *  @brief  FLD-MTデータコマンド転送送信要求処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 *  @date 2015/10/05 FPT)Yen Fix bug IT2(update size of msg FLD-MTデータコマンド転送送信要求)
 *  @date 2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-37)
 */
/********************************************************************************************************************/

VOID m_rc_FldMt_DataCmdTransferSndReq( CMT_TSKIF_CPRIRCV_FLDCOMSNDREQ* parm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	
	UINT		lwSigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	INT			lwErrcd;
	INT			ldwResult;
	USHORT		lwTimerId;

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
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_fldcomsndreq.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");

		if (CMD_SYS_3G == lwSigsys_rslt) {
			/* Send SV-MT/eNB-MTデータ受信応答 ( l3/rct->re/rec  ) */
			m_rc_FldMt_DataCmdTransferSndRspSend( CMD_FLD_FILSND_RE_NG, lwSigsys_rslt );
			return;
		}
	}
	
	// /*Save data command information*/
	// gt_rcw_DataTransInfo[lwSigsys_rslt].sndalldat_num 	= parm_p->cpridat_fldcomsndreq.sndalldat_num;
	// gt_rcw_DataTransInfo[lwSigsys_rslt].snddatseq_no	= parm_p->cpridat_fldcomsndreq.snddatseq_no;

	// /*Parameter check*/
	// ldwResult = m_rc_Fld_CmdTransSndReqParamChk( parm_p );
	// /* Parameter NG */
	// if( CMD_RES_OK !=ldwResult)
	// {
	// 	/*Send FLD-MTデータコマンド転送送信応答(225:送信不可状態)*/
	// 	m_rc_FldMt_DataCmdTransferSndRspSend( CMD_FLD_FILSND_NG, lwSigsys_rslt);
	// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
	// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	// 	#endif
	// 	return;
	// }

	// /* check system type */
	// if ( CMD_SYS_3G == lwSigsys_rslt )
	// {
	// 	/* Get 3G timer*/
	// 	lwTimerId = CMD_TIMID_MT_DATA_TRANS_SND_3G;
	// }
	// else
	// {
	// 	/* Get LTE timer*/
	// 	lwTimerId = CMD_TIMID_MT_DATA_TRANS_SND_LTE;
	// }

	// /* FLD-MT接続状態が未接続 */
	// if( D_L3_FLDMT_NOT_CONN == gw_rcw_FldMt_ConnSts[lwSigsys_rslt] )
	// {
	// 	/*Send FLD-MTデータコマンド転送送信応答(225:送信不可状態)*/
	// 	m_rc_FldMt_DataCmdTransferSndRspSend( CMD_FLD_FILSND_NG, lwSigsys_rslt);
	// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
	// 	printf( "[%d]%s  gw_rcw_FldMt_ConnSts[%d]: %d \n",__LINE__,__FUNCTION__, lwSigsys_rslt, gw_rcw_FldMt_ConnSts[lwSigsys_rslt]);
	// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	// 	#endif
	// 	return;
	// }

	/* テーブル参照FLD-MT接続先 */
	parm_p->cprircv_inf.link_num = gw_rcw_FldMt_ConnDst[lwSigsys_rslt];
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s : Forward message: Signal kind: %x\nlink_num = %d, gw_rcw_FldMt_ConnDst[%d] = %d\n",
			__LINE__,__FUNCTION__, parm_p->cpridat_fldcomsndreq.signal_kind, 
			parm_p->cprircv_inf.link_num, lwSigsys_rslt, gw_rcw_FldMt_ConnDst[lwSigsys_rslt]);
#endif
	
	/* Send FLD-MTデータコマンド転送送信要求 to re/rec*/
	ldwResult = l3_com_sendMsg(CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, parm_p, parm_p->head.uiLength);

	/*Sending fail*/
	if( D_RRH_OK != ldwResult)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							ldwResult, 
							"Send msg NG");
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	// /* FLD-MTデータコマンド転送送信確認タイマ */
	// ldwResult = cm_TStat(lwTimerId, CMD_TIMVAL_MT_DATA_TRANS_SND, CMD_TIMMD_SINGLE,
	// 				CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &lwErrcd);
	// /*Start timer NG*/
	// #ifdef FHM_RCT_DEBUG_FOR_IT1
	// printf( "[%d]cm_TStat(timerId:%d), Result: %d \n",__LINE__,lwTimerId, ldwResult);
	// #endif
	// if(ldwResult != CMD_RES_OK)
	// {
	// 	cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "FLD-MTデータコマンド転送送信確認タイマ Start NG" );
	// 	return;
	// }

	// /* FLD-MTデータコマンド転送送信応答待ちフラグ設定-->ON */
	// gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt] = D_L3_ON;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%sgw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt]: %d \n",__LINE__,__FUNCTION__,gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt]);
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  FLD-MTデータコマンド転送送信応答処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-37)
 */
/********************************************************************************************************************/

VOID m_rc_FldMt_DataCmdTransferSndResRcv( CMT_TSKIF_CPRISND_FLDCOMSNDRES* parm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	
	UINT		lwSigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	USHORT		lwTimerId;
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
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_fldcomsndres.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s System type: %d \n",__LINE__,__FUNCTION__, lwSigsys_rslt);
#endif
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}

	// /*If not wait response*/
	// #ifdef FHM_RCT_DEBUG_FOR_IT1
	// printf( "[%d]gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt]: %d \n",__LINE__,gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt]);
	// #endif
	// if( D_L3_OFF == gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt] )
	// {
	// 	/* アサート出力処理 */
	// 	cm_Assert(	CMD_ASL_USELOW,
	// 						gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt], 
	// 						"Not wait response");
	// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
	// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	// 	#endif
	// 	return;
	// }
	
	// /*Save data command information*/
	// gt_rcw_DataTransInfo[lwSigsys_rslt].sndalldat_num 	= parm_p->cpridat_fldcomsndres.sndalldat_num;
	// gt_rcw_DataTransInfo[lwSigsys_rslt].snddatseq_no	= parm_p->cpridat_fldcomsndres.snddatseq_no;
	// #ifdef FHM_RCT_DEBUG_FOR_IT1
	// printf( "[%d]gt_rcw_DataTransInfo[lwSigsys_rslt].sndalldat_num: %d \n",__LINE__,gt_rcw_DataTransInfo[lwSigsys_rslt].sndalldat_num);
	// printf( "[%d]gt_rcw_DataTransInfo[lwSigsys_rslt].snddatseq_no: %d \n",__LINE__,gt_rcw_DataTransInfo[lwSigsys_rslt].snddatseq_no);
	// #endif

	// /* 3G system*/
	// if( CMD_SYS_3G == lwSigsys_rslt)
	// {
	// 	lwTimerId = CMD_TIMID_MT_DATA_TRANS_SND_3G;
	// }
	// /* LTE system*/
	// else
	// {
	// 	lwTimerId = CMD_TIMID_MT_DATA_TRANS_SND_LTE;
	// }
	
	// /* FLD-MTデータコマンド転送送信確認タイマ */
	// cm_TStop( lwTimerId, &errcd );
	
	 /* 配下REの応答結果が「0:正常」の場合  */
	if( CMD_RES_OK == parm_p->cpridat_fldcomsndres.result )
	{
		/*Send FLD-MTデータコマンド転送送信応答(0:正常)*/
		m_rc_FldMt_DataCmdTransferSndRspSend( CMD_RES_OK, lwSigsys_rslt);
	}
	else
	{
		/*Send FLD-MTデータコマンド転送送信応答(224:全配下RE応答NG)*/
		m_rc_FldMt_DataCmdTransferSndRspSend( CMD_FLD_FILSND_RE_NG, lwSigsys_rslt);
	}

	// /* FLD-MTデータコマンド転送送信応答待ちフラグ設定-->OFF */
	// gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt] = D_L3_OFF;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	// printf( "[%d]gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt]: %d \n",__LINE__,gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSigsys_rslt]);
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif

	return;
}

/* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief  FLD-MTデータコマンド転送送信確認タイマタイムアウト処理
//  *  @note 
//  *  @param  awTimerId [in] timer id
//  *  @return None
//  *  @date 
//  *  @date 	2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-37)
//  */
// /********************************************************************************************************************/

// VOID m_rc_FldMt_DataCmdTransferSndTO( USHORT awTimerId )
// {
// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s start(awTimerId :%d) \n",__LINE__,__FUNCTION__,awTimerId);
// 	#endif
	
// 	USHORT		lwSystemType;

// 	/*タイマIdが３G タイマの場合*/
// 	if( CMD_TIMID_MT_DATA_TRANS_SND_3G == awTimerId )
// 	{
// 		lwSystemType = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		lwSystemType = CMD_SYS_LTE;
// 	}
// 	/* FLD-MTデータコマンド転送送信応答待ちフラグ がOFFの場合 */
// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSystemType]: %d \n",__LINE__,gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSystemType]);
// 	#endif
// 	if( gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSystemType] == D_L3_OFF )
// 	{
// 		/* アサート出力処理 */
// 		cm_Assert(	CMD_ASL_USELOW,
// 							gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSystemType], 
// 							"Not wait response");
// 		/* 処理終了 */
// 		#ifdef FHM_RCT_DEBUG_FOR_IT1
// 		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// 		#endif
// 		return;
// 	}
	
// 	/* FLD-MTデータコマンド転送送信応答待ちフラグ設定-->OFF */
// 	gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSystemType] = D_L3_OFF;
// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSystemType]: %d \n",__LINE__,gw_rcw_FldMt_DataTranferRes_WaitFlg[lwSystemType]);
// 	#endif
	
// 	/*Send FLD-MTデータコマンド転送送信応答(224:全配下RE応答NG)*/
// 	m_rc_FldMt_DataCmdTransferSndRspSend( CMD_FLD_FILSND_RE_NG, lwSystemType);

// 	#ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// 	#endif
// 	return;
// }
// /* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief  FLD-MTデータコマンド転送送信要求をチェック
//  *  @note 
//  *  @param  parm_p [in] the buffer address pointer of received message
//  *  @return check result
//  *  @date	2015/07/09 FPT)Hieu
//  *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-37)
//  */
// /********************************************************************************************************************/

// UINT m_rc_Fld_CmdTransSndReqParamChk( CMT_TSKIF_CPRIRCV_FLDCOMSNDREQ* parm_p )
// {
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
// 	printf( "[%d]%s parm_p->cpridat_fldcomsndreq.sndalldat_num = %d \n",__LINE__,__FUNCTION__, parm_p->cpridat_fldcomsndreq.sndalldat_num);
// 	printf( "[%d]%s parm_p->cpridat_fldcomsndreq.snddatseq_no = %d \n",__LINE__,__FUNCTION__, parm_p->cpridat_fldcomsndreq.snddatseq_no);
// 	printf( "[%d]%s parm_p->cpridat_fldcomsndreq.sndbyte_num = %d \n",__LINE__,__FUNCTION__, parm_p->cpridat_fldcomsndreq.sndbyte_num);
// #endif

// 	/* 全送信データ数は範囲外の場合 */
// 	if(parm_p->cpridat_fldcomsndreq.sndalldat_num < CMD_NUM1 ||			/* If all receive data number is smaller than 1, or	*/
// 	   parm_p->cpridat_fldcomsndreq.sndalldat_num > CMD_FLDCMD_MAXBLK)	/* all receive data number is bigger than allowed	*/
// 	{
// 		#ifdef FHM_RCT_DEBUG_FOR_IT1
// 		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// 		#endif
// 		return CMD_RES_PARAMERR;
// 	}

// 	/* 送信データ順序番号は範囲外の場合 */
// 	if(parm_p->cpridat_fldcomsndreq.snddatseq_no < CMD_NUM1 ||			/* If receive data numbering is smaller than 1, or	*/
// 	   parm_p->cpridat_fldcomsndreq.snddatseq_no > CMD_FLDCMD_MAXBLK)	/* receive data numbering is bigger than allowed	*/
// 	{
// 		#ifdef FHM_RCT_DEBUG_FOR_IT1
// 		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// 		#endif
// 		return CMD_RES_PARAMERR;
// 	}

// 	/* 送信バイト数は範囲外の場合 */
// 	if(parm_p->cpridat_fldcomsndreq.sndbyte_num < CMD_NUM1 ||			/* If receive byte number is smaller than 1, or	*/
// 	   parm_p->cpridat_fldcomsndreq.sndbyte_num > CMD_FLDCMD_BLKLEN)	/* receive byte number is bigger than allowed	*/
// 	{
// 		#ifdef FHM_RCT_DEBUG_FOR_IT1
// 		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// 		#endif
// 		return CMD_RES_PARAMERR;
// 	}

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 	return CMD_RES_OK;
// }

// /* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief Send FLD-MTデータコマンド転送送信応答
 *  @note 
 *  @param  awResult [in] Response result
 *  @param  awSystem [in] System type
 *  @return None
 *  @date	2015/07/09 FPT)Hieu
 */
/********************************************************************************************************************/

VOID	m_rc_FldMt_DataCmdTransferSndRspSend( USHORT awResult, USHORT awSystem)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif
	
	CMT_TSKIF_CPRISND_FLDCOMSNDRES*	ltRes_p;
	UINT   bufget_rslt;						/* バッファ獲得結果				*/
	/****************/
	/* バッファ獲得 */
	/****************/
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_FLDCOMSNDRES),
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
					"m_rc_FldMt_DataCmdTransferSndRspSend cm_L2BfGet NG");
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	ltRes_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)
									+	sizeof(CMT_CPRIF_FLDCOMSNDRES);

	/* CPRI信号応答結果にOKを設定 */
	ltRes_p->cpridat_fldcomsndres.result = awResult;
	
	/* CPRI信号種別設定 */
	ltRes_p->cpridat_fldcomsndres.signal_kind = CMD_CPRID_FLDCOMSNDRES + awSystem;
	
	/* 全送信データ数設定*/
	ltRes_p->cpridat_fldcomsndres.sndalldat_num	= gt_rcw_DataTransInfo[awSystem].sndalldat_num;
	/* 送信データ順序番号*/
	ltRes_p->cpridat_fldcomsndres.snddatseq_no	= gt_rcw_DataTransInfo[awSystem].snddatseq_no;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Signal kind: %x , Result: ltRes_p->cpridat_fldcomsndres.result = %d\n",__LINE__,__FUNCTION__, 
							ltRes_p->cpridat_fldcomsndres.signal_kind, ltRes_p->cpridat_fldcomsndres.result );
#endif
	/*eNB-MT接続強制解放応答をl2/lpbに送信する*/
	m_cm_L3MsgSend_REC( ltRes_p );
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
}

/* @} */
/* @} */

