/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_BufLnkData.c
 *  @brief  バッファリング送信/受信要求データ処理
 *  @date   2015/07/09 FPT)Yen Create new
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
 *  @brief  バッファリング送信要求データ処理
 *  @note 
 *  @param  awSystemType [in] システムタイプ
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01796
 *  @date   2015/07/09 FPT)Yen Create new   
 *  @date 	2015/10/26 FPT)HaiLC M-RRU-ZSYS-01796 fix BUG_101_IT2_22-09-03_22-10-04_2 ヘッダのメッセージ長を設定, メッセージに外部装置番号を設定
 */
/********************************************************************************************************************/
VOID m_rc_GetBufLnkDataSnd( USHORT awSystemType)
{
	CMT_TSKIF_CPRIRCV_EXTDATSNDREQ ltExDatSndReq;
	USHORT		lwSignalType;
	USHORT		lwReadId;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
	/* 外部装置データ送信要求バッファチェック */
	if(CMD_NUM0 == gt_rcw_ExtDataSndBuf[awSystemType].wBufferNum)
	{
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		/* データなし場合 */
		/* Return */
		return;
	}
	
	cm_MemClr(&ltExDatSndReq, sizeof(ltExDatSndReq));
	ltExDatSndReq.head.uiSignalkind = CMD_CPRID_EXTDATSNDREQ + awSystemType;
	
	ltExDatSndReq.cpridat_extdatsndreq.signal_kind  = CMD_CPRID_EXTDATSNDREQ + awSystemType;

	/* 外部装置データ送信要求バッファから送信データ取り出し */
	/* ※「読み込みIDXにより」、送信データ取り出し */
	lwReadId = gt_rcw_ExtDataSndBuf[awSystemType].wReadId;
	
	/* Set uiLength base on size of data */
	ltExDatSndReq.head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF) + 
								 CMD_NUM3 * sizeof(USHORT) + gt_rcw_ExtDataSndBuf[awSystemType].tExtDataSnd[lwReadId].wSize;
	
	
	lwSignalType	= gt_rcw_ExtDataSndBuf[awSystemType].tExtDataSnd[lwReadId].wSignalType;

	/* 外部装置番号 */
	/* ３Gの場合 */
	if(CMD_SYS_3G == awSystemType)
	{
		ltExDatSndReq.cpridat_extdatsndreq.extdev_no		= EXTDEV_NUM_TILTINF_3G;
	}
	/* LTEの場合 */
	else
	{
		ltExDatSndReq.cpridat_extdatsndreq.extdev_no		= EXTDEV_NUM_TILTINF_LTE;
	}

	/* 送信バイト数 */
	ltExDatSndReq.cpridat_extdatsndreq.snd_byte		= gt_rcw_ExtDataSndBuf[awSystemType].tExtDataSnd[lwReadId].wSize;
	/* 送信メッセージ */
	memcpy( &ltExDatSndReq.cpridat_extdatsndreq.snd_msg[0],
			&gt_rcw_ExtDataSndBuf[awSystemType].tExtDataSnd[lwReadId].cData[0],
			gt_rcw_ExtDataSndBuf[awSystemType].tExtDataSnd[lwReadId].wSize);
	
	/* 読み込みIDX更新 */
	gt_rcw_ExtDataSndBuf[awSystemType].wReadId = ( lwReadId + CMD_NUM1) % CMD_NUM30;
	/* バッファリング数更新 */
	gt_rcw_ExtDataSndBuf[awSystemType].wBufferNum -= CMD_NUM1;
	
	/* 送信バッファから取り出した送信データの信号種別チェック */
	/* AISG信号の場合 */
	if(RCD_EXTSNDREQ_AISG == lwSignalType)
	{
		/* AISG信号受信処理 */
		m_rc_AisgSignal(&ltExDatSndReq);
	}
	else if (RCD_EXTTSNDREQ_NONEAISG == lwSignalType)  /* 非AISG信号の場合 */
	{
		/* 非AISG信号受信処理 */
		m_rc_NoneAisgSignal(&ltExDatSndReq);
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  外部装置データ送信要求信号をバッファリングする。
 *  @note 
 *  @param  awSystemType [in] システムタイプ
 *  @param  awSignalType [in] 信号種別 (AISG/noneAISG)
 *  @param  atBufferData_p [in] buffer pointer
 *  @return UINT
 *  @date   2015/07/09 FPT)Yen Create new   
 *  @date 
 */
/********************************************************************************************************************/

UINT m_rc_SetBufLnkDataSnd( USHORT awSystemType, USHORT awSignalType, CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* atBufferData_p )
{
	USHORT					 lwWriteId;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	/* パラメータチェック */
	/* NGの場合 */
	/* ※システムタイプが３G/LTE以外の場合 */
	if(awSystemType >= CMD_MAX_SYSNUM )
	{
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		return CMD_RES_NG;
	}
	
	/* ※atBufferData_pがNULLの場合 */
	if(NULL == atBufferData_p)
	{
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		/* Return（異常） */
		return CMD_RES_NG;
	}
	
	/* 外部装置データ送信要求バッファチェック */
	/* バッファフル発生 */
	if(CMD_NUM30 == gt_rcw_ExtDataSndBuf[awSystemType].wBufferNum)
	{
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		/* Return（異常） */
		return CMD_RES_NG;
	}

	lwWriteId = gt_rcw_ExtDataSndBuf[awSystemType].wWriteId;
	/* atBufferData_pを外部装置データ送信要求バッファに追加 */
	/* 信号種別 */
	gt_rcw_ExtDataSndBuf[awSystemType].tExtDataSnd[lwWriteId].wSignalType	= awSignalType;
	/* 送信バイト数 */
	gt_rcw_ExtDataSndBuf[awSystemType].tExtDataSnd[lwWriteId].wSize		= atBufferData_p->cpridat_extdatsndreq.snd_byte;
	/* 送信メッセージ */
	memcpy( &gt_rcw_ExtDataSndBuf[awSystemType].tExtDataSnd[lwWriteId].cData[0],
			&atBufferData_p->cpridat_extdatsndreq.snd_msg[0],
			atBufferData_p->cpridat_extdatsndreq.snd_byte ); 

	gt_rcw_ExtDataSndBuf[awSystemType].wWriteId		= (lwWriteId +1) % CMD_NUM30;
	gt_rcw_ExtDataSndBuf[awSystemType].wBufferNum ++;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	/* Return（正常） */
	return CMD_RES_OK;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  バッファリング受信要求データ処理
 *  @note 
 *  @param  awSystemType [in] システムタイプ
 *  @return none
 *  @date   2015/07/09 FPT)Yen Create new   
 *  @date   2015/10/21 FPT)Lay IT2問処番号117対応
 *  @date   2016/04/15 TDI)satou 3001-15B-TS120 Length部の設定値が、常に2048byteのL3メッセージのものになっていた不具合を修正
 */
/********************************************************************************************************************/

VOID m_rc_GetBufLnkDataRcv( USHORT awSystemType )
{
	CMT_TSKIF_CPRIRCV_EXTDATRCVREQ 	*extrcvreq_p;	/* EXT data reception request 		*/
	UINT 							ldwRet_breq;	/* Return value of buffer request	*/
	USHORT							lwReadId;
	USHORT							lwTimerId;
	USHORT							lwTim_val	= CMD_NUM0;		/* external equipment receiving confirmation timer */
	UINT							ldwSnd_ret;		/* Return value of sending			*/
	INT								ldwResult;
	INT								ldwerrcd;
	UINT							ldwFixedLen;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	/* 外部装置データ受信要求バッファチェック */
	if(CMD_NUM0 == gt_rcw_ExtDataRcvBuf[awSystemType].wBufferNum)
	{
		/* データなし場合 */
		/* Return */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* 外部装置データ受信要求バッファから受信データ取り出し */
	/* ※「読み込みIDX」により、送信データ取り出し */
	lwReadId = gt_rcw_ExtDataRcvBuf[awSystemType].wReadId;
	
	/* 外部装置データ受信中状態 ：　送信データの信号種別 */
	gw_rcw_ExtRcvReqSta[awSystemType]         = gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwReadId].wSignalType;
	gw_rcw_ExtRcvRspDestLinkNum[awSystemType] = gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwReadId].wCpriNum;
	
	/* ※ */
	/* 1  RE主導外部装置データ受信要求処理中 */
	/* 11 FHM主導の状態報告信号処理中 */
	/* 12 FHM主導の局リセット信号処理中 */
	/* 13 FHM主導の配下REリセット報告信号処理中 */
	/* 14 FHM主導の局ON/OFF報告信号処理中 */
	/* 15 FHM主導の配下REON/OFF報告信号処理中 */
	/* 16 FHM主導の配下ヘルスチェック応答信号処理中 */
	/* 17 FHM主導の状態変化信号処理中 */
	ldwRet_breq = CMD_NUM0;
	
	/* Buffer request	 */
	ldwRet_breq = cm_L2BfGet(CMD_BUFCA_LAPSND,
						  sizeof(CMT_TSKIF_CPRIRCV_EXTDATRCVREQ),
						  CMD_NUM1, (VOID **)&extrcvreq_p);
	
	if(ldwRet_breq != CMD_RES_OK)		/* If buffer request NG		*/
	{
		cm_MAbort(	CMD_ALMCD_BUFGET, 
					"m_ex_ExtDataRcv", 
					"Buffer request error", 
					ldwRet_breq, 
					sizeof(CMT_TSKIF_CPRIRCV_EXTDATRCVREQ), 
					CMD_NUM0);
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* 送信メッセージ */
	memcpy( &extrcvreq_p->cpridat_extdatrcvreq.sndmsg[0],
		&gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwReadId].cData[0],
		gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwReadId].wSize);
	
	ldwFixedLen  = 0;
	ldwFixedLen += sizeof(CMT_TSKIF_HEAD);
	ldwFixedLen += sizeof(CMT_CPRISIGRCV_INF);
	ldwFixedLen += sizeof extrcvreq_p->cpridat_extdatrcvreq.signal_kind;
	ldwFixedLen += sizeof extrcvreq_p->cpridat_extdatrcvreq.extdev_no;
	ldwFixedLen += sizeof extrcvreq_p->cpridat_extdatrcvreq.sndbyte_num;

	extrcvreq_p->head.uiLength = ldwFixedLen + gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwReadId].wSize;
	extrcvreq_p->cpridat_extdatrcvreq.signal_kind = CMD_CPRID_EXTDATRCVREQ + awSystemType;
	extrcvreq_p->cpridat_extdatrcvreq.sndbyte_num = gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwReadId].wSize;
	
	/* ３Gの場合 */
	if(CMD_SYS_3G == awSystemType)
	{
		extrcvreq_p->cpridat_extdatrcvreq.extdev_no		= EXTDEV_NUM_TILTINF_3G;
	}
	/* LTEの場合 */
	else
	{
		extrcvreq_p->cpridat_extdatrcvreq.extdev_no		= EXTDEV_NUM_TILTINF_LTE;
	}
	
	/* 外部装置データ受信要求 */
	ldwSnd_ret = m_cm_L3MsgSend_REC(extrcvreq_p);
	if(ldwSnd_ret != CMD_L3_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM1, 
								"m_ex_ExtDataRcv MSGSndNG");
	}	
	
	/* 読み込みIDX更新 */
	gt_rcw_ExtDataRcvBuf[awSystemType].wReadId = ( lwReadId + CMD_NUM1) % CMD_NUM30;
	/* バッファリング数更新 */
	gt_rcw_ExtDataRcvBuf[awSystemType].wBufferNum -= CMD_NUM1;
	
	/* 3G system*/
	if( CMD_SYS_3G == awSystemType)
	{
		lwTimerId = CMD_TIMID_EXT_DAT_RCV_3G;
	}
	/* LTE system*/
	else
	{
		lwTimerId = CMD_TIMID_EXT_DAT_RCV_LTE;
	}
	
	lwTim_val = cmw_sys_mng_tbl[awSystemType].oudevt_rcvtimer;
	
	/* 「外部装置データ受信確認タイマ」開始 */
	/* タイマ番号 ：外部装置データ受信確認 */
	/* タイマ値 ：(変数名D)外部装置データ受信確認タイマ（単位：10ms）*/
	/* タイマモード ：シングルモード*/
	/* eventno ：CMD_TSKIF_TIMOUTNTC*/
	/* queueid ：CMD_TSKID_RCT*/
	ldwResult = cm_TStat(lwTimerId, TIMVAL_GET( lwTim_val ), CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &ldwerrcd);
	/*Start timer NG*/
	if(CMD_RES_OK != ldwResult)
	{
		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "外部装置データ受信確認タイマ Start NG" );
	}

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;

}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  外部装置データ受信要求信号をバッファリングする。
 *  @note 
 *  @param  awSystemType    [in] システムタイプ
 *	@param	awProcessSta    [in] process status
 *  @param  atBufferData_p  [in] 外部装置データ受信要求信号
 *  @return UINT
 *  @date   2015/07/09 FPT)Yen Create new   
 *  @date   2015/10/02 TDI)satou 「受信元CPRIリンク番号」のバッファリングを追加
 */
/********************************************************************************************************************/

UINT m_rc_SetBufLnkDataRcv( USHORT awSystemType, USHORT awProcessSta, CMT_TSKIF_CPRIRCV_EXTDATRCVREQ* atBufferData_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	USHORT					 lwWriteId;

	/* パラメータチェック */
	/* NGの場合 */
	/* ※システムタイプが３G/LTE以外の場合 */
	if(awSystemType >= CMD_MAX_SYSNUM )
	{		
		return CMD_RES_NG;
	}
	/* ※atBufferData_pがNULLの場合 */
	if(NULL == atBufferData_p)
	{
		/* Return（異常） */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return CMD_RES_NG;
	}
	
	/* 外部装置データ受信要求バッファチェック */
		/* バッファフル発生 */
	if(CMD_NUM30 == gt_rcw_ExtDataRcvBuf[awSystemType].wBufferNum)
	{
		/* Return（異常） */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return CMD_RES_NG;
	}
	
	/* atBufferData_pを外部装置データ受信要求バッファに追加 */
	lwWriteId = gt_rcw_ExtDataRcvBuf[awSystemType].wWriteId;
	
	/** 信号種別 */
	gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwWriteId].wSignalType	= awProcessSta;
	
	/** 送信バイト数 */
	gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwWriteId].wSize			= atBufferData_p->cpridat_extdatrcvreq.sndbyte_num;
	
	/* 受信元CPRIリンク番号 */
	gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwWriteId].wCpriNum      = atBufferData_p->cprircv_inf.link_num;

	/* 送信メッセージ */
	memcpy( &gt_rcw_ExtDataRcvBuf[awSystemType].tExtDataRcv[lwWriteId].cData[0],
			&atBufferData_p->cpridat_extdatrcvreq.sndmsg[0],
			atBufferData_p->cpridat_extdatrcvreq.sndbyte_num );
	
	gt_rcw_ExtDataRcvBuf[awSystemType].wWriteId		= (lwWriteId + CMD_NUM1) % CMD_NUM30;
	gt_rcw_ExtDataRcvBuf[awSystemType].wBufferNum ++;
	
	/* Return（正常） */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return CMD_RES_OK;

}

/* @} */

/* @} */

