/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ExtDataRcv.c
 *  @brief  外部装置データ受信処理
 *  @date   2015/07/09 FPT)Yen Create new
 *  @date   
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
 *  @brief  外部装置データ受信応答
 *  @note 
 *  @param  awSystemType [in] the system type LTE/3G
 *  @param  awResult	 [in] the the response result
 *  @param  link_num     [in] 送信先CPRIリンク番号
 *  @return None
 *  @date 2015/07/30
 *  @date 2015/10/02 TDI)satou 送信メッセージのヘッダ設定を追加
 *  @date 2015/10/02 TDI)satou 送信先のCPRIリンク番号の引数を追加
 */
/********************************************************************************************************************/

VOID m_rc_ExtDataRcvRspSubProc( USHORT awSystemType, USHORT awResult, USHORT link_num )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	CMT_TSKIF_CPRIRCV_EXTDATRCVRES	ltextrcvrsp;			/* EXT data reception response	*/
	
	/* ヘッダ部設定 */
	memset(&ltextrcvrsp, 0, sizeof ltextrcvrsp);
	ltextrcvrsp.head.uiSignalkind    = CMD_CPRID_EXTDATRCVRES + awSystemType;
	ltextrcvrsp.cprircv_inf.link_num = link_num;
	
	/* 信号種別 */
	ltextrcvrsp.cpridat_extdatrcvres.signal_kind = CMD_CPRID_EXTDATRCVRES + awSystemType;
	
	/* ３Gの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]awSystemType : %d \n",__LINE__,awSystemType );
	#endif
	if(CMD_SYS_3G == awSystemType)
	{
		ltextrcvrsp.cpridat_extdatrcvres.extdev_no = EXTDEV_NUM_TILTINF_3G;
	}
	/* LTEの場合 */
	else
	{
		ltextrcvrsp.cpridat_extdatrcvres.extdev_no = EXTDEV_NUM_TILTINF_LTE;
	}
	
	ltextrcvrsp.cpridat_extdatrcvres.result = awResult;
	
	/* Send l3 message（l3_rct -> re_ec) */
	/* Event ：　(0x4001)RE Forwarding Notice  */
	l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,
					&ltextrcvrsp, sizeof( CMT_TSKIF_CPRIRCV_EXTDATRCVRES ) );
	
	/* 処理終了 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;

}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  外部装置データ受信要求処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_ExtDataRcvReq( CMT_TSKIF_CPRIRCV_EXTDATRCVREQ* parm_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	UINT							lwSigchk_rslt;						/* result of checking signal */
	USHORT							lwSigsys_rslt;						/* system after checking signal */
	USHORT							lwParaChkNgRtc;
	UINT							ldwSetbuf_rslt;
	
	
	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_extdatrcvreq.signal_kind, &lwSigsys_rslt);

	/* validate signal NGの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]lwSigchk_rslt : %d \n",__LINE__,lwSigchk_rslt );
	#endif
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}
	
	/* ３Gの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]lwSigsys_rslt : %d \n",__LINE__,lwSigsys_rslt );
	#endif
	if( CMD_SYS_3G == lwSigsys_rslt )
	{
		/* 235その他エラー */
		lwParaChkNgRtc = CMD_OTH_ERR;
	}
	else
	{
		/* 51処理NG(RE) */
		lwParaChkNgRtc = CMD_HDL_NG;
	}
	
	/* メッセージパラメータチェック */
	/* メッセージパラメータNGの場合 */
	/* ※外部装置番号がTILT-INF(3G=４)以外 */
	if (( CMD_SYS_3G == lwSigsys_rslt ) &&
		( EXTDEV_NUM_TILTINF_3G != parm_p->cpridat_extdatrcvreq.extdev_no ))
	{
		/* Msg　：　外部装置データ受信応答（３G：235その他エラー/LTE：51処理NG(RE)) */
		/* Send l3 message（l3_rct -> re_ec) */
		/* Event ：　(0x4001)RE Forwarding Notice  */
		m_rc_ExtDataRcvRspSubProc(lwSigsys_rslt, lwParaChkNgRtc, parm_p->cprircv_inf.link_num);
		
		/* 処理終了 */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
 	
	/* ※外部装置番号がTILT-INF(LTE=２)以外 */
 	if (( CMD_SYS_S3G == lwSigsys_rslt ) &&
		( EXTDEV_NUM_TILTINF_LTE != parm_p->cpridat_extdatrcvreq.extdev_no ))
	{
		/* Msg　：　外部装置データ受信応答（LTE：51処理NG(RE)) */
		/* Send l3 message（l3_rct -> re_ec) */
		/* Event ：　(0x4001)RE Forwarding Notice  */
		m_rc_ExtDataRcvRspSubProc(lwSigsys_rslt, lwParaChkNgRtc, parm_p->cprircv_inf.link_num);
		
		/* 処理終了 */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* ※送信バイト数が1～2048でない */
	if (( parm_p->cpridat_extdatrcvreq.sndbyte_num < MIN_EXTDAT_MSGSND_SIZE ) ||
		( parm_p->cpridat_extdatrcvreq.sndbyte_num > MAX_EXTDAT_MSGSND_SIZE ))
	{
		/* Msg　：　外部装置データ受信応答（３G：235その他エラー) */
		/* Send l3 message（l3_rct -> re_ec) */
		/* Event ：　(0x4001)RE Forwarding Notice  */
		m_rc_ExtDataRcvRspSubProc(lwSigsys_rslt, lwParaChkNgRtc, parm_p->cprircv_inf.link_num);
		
		/* 処理終了 */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* 外部装置データ受信要求送信 */
	ldwSetbuf_rslt = m_rc_ExtDataRcvReqExe(parm_p, RCD_EXTRCVREQ_RESND, lwSigsys_rslt);
	
	/* NGの場合(バッファフル発生) */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]m_rc_ExtDataRcvReqExe : %d \n",__LINE__,ldwSetbuf_rslt );
	#endif
	if(CMD_RES_OK != ldwSetbuf_rslt)
	{
		/* ３Gの場合 */
		if( CMD_SYS_3G == lwSigsys_rslt )
		{
			/* ３G：19（バッファフル) */
			/* Send l3 message（l3_rct -> re_ec) */
			/* event　：　(0x4001)RE Forwarding Notice */
			m_rc_ExtDataRcvRspSubProc(lwSigsys_rslt, CMD_BUFF_FUL, parm_p->cprircv_inf.link_num);
		}
		else
		{
			/* LTE: 60 バッファフル(REC)(for LTE) */
			/* Send l3 message（l3_rct -> re_ec) */
			/* event　：　(0x4001)RE Forwarding Notice */
			m_rc_ExtDataRcvRspSubProc(lwSigsys_rslt, CMD_BUFF_FUL_S3G, parm_p->cprircv_inf.link_num);
		}
	
	}
	
	/* 処理終了 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;

}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  外部装置データ受信要求送信処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @param  awProcessSta [in] process status
 *  @param  awSystemType [in] システムタイプ
 *  @return UINT
 *  @Bug_No M-RRU-ZSYS-01927
 *  @date 2015/10/21 FPT)Quynh Fix bug IT2 (BUG_110_IT2_12-11-04)
 *  @date 2015/10/28 FPT)Yen M-RRU-ZSYS-01927 Fix bug IT2 (No 150 ) 局リセットの場合、バッファにいれないですぐにRECへ流す
 */
/********************************************************************************************************************/
UINT m_rc_ExtDataRcvReqExe( CMT_TSKIF_CPRIRCV_EXTDATRCVREQ* parm_p, USHORT awProcessSta, USHORT awSystemType )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	CMT_TSKIF_CPRIRCV_EXTDATRCVREQ 	*extrcvreq_p;	/* EXT data reception request			*/
	UINT 							ldwRet_breq;	/* Return value of buffer request	*/
	UINT							ldwSetbuf_rslt;
	UINT							ldwSnd_ret;		/* Return value of sending			*/
	INT								ldwerrcd;
	USHORT							lwTimerId;
	INT								ldwResult;
	USHORT							lwTim_val	= CMD_NUM0;		/* external equipment receiving confirmation timer */
	UINT                            keyID;
	UINT							luiMsgLength;
	T_RRH_LAYER3					layer3;

	/* 外部装置データ受信中状態チェック */
	/* 外部装置データ受信処理中の場合 */
	if( (RCD_EXTRCVREQ_IDLE != gw_rcw_ExtRcvReqSta[awSystemType]) && ( RCD_EXTRCVREQ_SYSRST != awProcessSta ) )
	{
		/* 外部装置データ受信要求信号をバッファリングする。 */
		ldwSetbuf_rslt = m_rc_SetBufLnkDataRcv(awSystemType, awProcessSta, parm_p);
		
		/* return (バッファリング結果を返す) */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return ldwSetbuf_rslt;
	}
	
	//Get state of layer 3
	f_cmn_com_layer3_get(awSystemType, &layer3);
	
	//FHMとREC間の状態が⑦RE運用中状態以外の場合
	//Send response NG to RE
	if( E_RRH_LAYER3_REC_OPE != layer3.layer3_rec )
	{
		if( CMD_SYS_LTE == awSystemType )
		{
			/* LTE：51 */
			/* Send l3 message（l3_rct -> re_rec) */
			/* event　：　(0x4001)RE Forwarding Notice */
			m_rc_ExtDataRcvRspSubProc(awSystemType, CMD_HDL_NG, parm_p->cprircv_inf.link_num);
		}
		else
		{
			/* 3G：235 */
			/* Send l3 message（l3_rct -> re_rec) */
			/* event　：　(0x4001)RE Forwarding Notice */
			m_rc_ExtDataRcvRspSubProc(awSystemType, CMD_OTH_ERR, parm_p->cprircv_inf.link_num);
		}
		//End processing
		return CMD_RES_OK;
	}
	
	/* 外部装置データ受信中状態 ：　awProcessSta */
	/* ※ */
		/* 1 RE主導外部装置データ受信要求処理中 */
		/* 11 FHM主導の状態報告信号処理中 */
		/* 12 FHM主導の局リセット信号処理中 */
		/* 13 FHM主導の配下REリセット報告信号処理中 */
		/* 14 FHM主導の局ON/OFF報告信号処理中 */
		/* 15 FHM主導の配下REON/OFF報告信号処理中 */
		/* 16 FHM主導の配下ヘルスチェック応答信号処理中 */
		/* 17 FHM主導の状態変化信号処理中 */
	gw_rcw_ExtRcvReqSta[awSystemType]         = awProcessSta;
	gw_rcw_ExtRcvRspDestLinkNum[awSystemType] = parm_p->cprircv_inf.link_num;
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]gw_rcw_ExtRcvReqSta[awSystemType] : %d \n",__LINE__,gw_rcw_ExtRcvReqSta[awSystemType] );
	#endif
	
	ldwRet_breq = CMD_NUM0;

	luiMsgLength = ((CMT_TSKIF_CPRIRCV_EXTDATRCVREQ *)parm_p)->head.uiLength;
	/* Buffer request	 */
	ldwRet_breq = cm_L2BfGet(CMD_BUFCA_LAPSND,
						  luiMsgLength,
						  CMD_NUM1, (VOID **)&extrcvreq_p);
	
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]cm_L2BfGet : %d \n",__LINE__,ldwRet_breq );
	#endif
	
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
		return CMD_RES_NG;
	}
	
	keyID = extrcvreq_p->head.uiKeyID;
	memcpy(extrcvreq_p, parm_p, luiMsgLength);
	extrcvreq_p->head.uiKeyID = keyID;
	
	extrcvreq_p->cpridat_extdatrcvreq.signal_kind = CMD_CPRID_EXTDATRCVREQ + awSystemType;
	
	/* ３Gの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]awSystemType : %d \n",__LINE__,awSystemType );
	#endif
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
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]m_cm_L3MsgSend_REC : %d \n",__LINE__,ldwSnd_ret );
	#endif
	if(ldwSnd_ret != CMD_L3_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM1, 
								"m_ex_ExtDataRcv MSGSndNG");
	}	
	
	/* 3G system*/
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]awSystemType : %d \n",__LINE__,awSystemType );
	#endif
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
		/* タイマ番号：外部装置データ受信確認  */
		/* タイマ値 ：(変数名D)外部装置データ受信確認タイマ（単位：10ms）*/
		/* タイマモード ：シングルモード*/
		/* eventno ：CMD_TSKIF_TIMOUTNTC*/
		/* queueid ：CMD_TSKID_RCT*/
	ldwResult = cm_TStat(lwTimerId, TIMVAL_GET( lwTim_val ), CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &ldwerrcd);
	/*Start timer NG*/
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]cm_TStat : %d \n",__LINE__,ldwResult );
	#endif
	if(CMD_RES_OK != ldwResult)
	{
		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "外部装置データ受信確認タイマ Start NG" );
	}

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return CMD_RES_OK;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  外部装置データ受信応答受信
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_ExtDataRcvRsp( CMT_TSKIF_CPRIRCV_EXTDATRCVRES* parm_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	UINT						lwSigchk_rslt;					/* result of checking signal */
	USHORT						lwSigsys_rslt;					/* system after checking signal */
	INT							ldwerrcd;
	USHORT						lwTimerId;
	INT							ldwResult;
	
	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_extdatrcvres.signal_kind,&lwSigsys_rslt);
	
	/* validate signal NGの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]m_cm_chksig : %d \n",__LINE__,lwSigchk_rslt );
	#endif
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}
	
	/* 外部装置データ受信中状態チェック */
	/* 外部装置データ受信未処理の場合 */
	if(RCD_EXTRCVREQ_IDLE == gw_rcw_ExtRcvReqSta[lwSigsys_rslt] )
	{
		/* 処理終了 */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* タイマ番号 */
	/* 3G system*/
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]lwSigsys_rslt : %d \n",__LINE__,lwSigsys_rslt );
	#endif
	if( CMD_SYS_3G == lwSigsys_rslt)
	{
		lwTimerId = CMD_TIMID_EXT_DAT_RCV_3G;
	}
	/* LTE system*/
	else
	{
		lwTimerId = CMD_TIMID_EXT_DAT_RCV_LTE;
	}
	
	/* 「外部装置データ受信確認タイマ」停止 */
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	
	/* Stop timer NG */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]cm_TStop : %d \n",__LINE__,ldwResult );
	#endif
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "外部装置データ受信確認タイマ Stop NG" );
	}
	
	/* 外部装置データ受信中状態がRE主導外部装置データ受信要求処理中（１）の場合 */
	if(RCD_EXTRCVREQ_RESND == gw_rcw_ExtRcvReqSta[lwSigsys_rslt])
	{
		/* Send l3 message（l3_rct -> re_ec) */
		/* event　：　(0x4001)RE Forwarding Notice */
		/* msg　　：　外部装置データ受信応答 */
		m_rc_ExtDataRcvRspSubProc(lwSigsys_rslt, parm_p->cpridat_extdatrcvres.result, gw_rcw_ExtRcvRspDestLinkNum[lwSigsys_rslt]);
	
	}
	
	/* 外部装置データ受信中状態：未処理 */
	gw_rcw_ExtRcvReqSta[lwSigsys_rslt] = RCD_EXTRCVREQ_IDLE;
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]gw_rcw_ExtRcvReqSta[lwSigsys_rslt] : %d \n",__LINE__,gw_rcw_ExtRcvReqSta[lwSigsys_rslt] );
	#endif
	
	/* バッファリング受信要求データ処理(システムタイプ(3G/LTE)) */
	m_rc_GetBufLnkDataRcv(lwSigsys_rslt);
	
	/* 処理終了 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  外部装置データ受信応答待ちタイムアウト
 *  @note 
 *  @param  adwTimerId [in] the timer ID
 *  @return None
 *  @date 
 *  @date 2015/10/06 FPT)Quynh Fix bug IT1
 */
/********************************************************************************************************************/

VOID m_rc_ExtDataRcvRspTo( UINT adwTimerId )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	USHORT								lwExtSysType;
	USHORT								lwParaChkNgRtc;
	
	
	/* 3G system*/
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]adwTimerId : %d \n",__LINE__,adwTimerId );
	#endif
	if(CMD_TIMID_EXT_DAT_RCV_3G == adwTimerId)
	{
		lwExtSysType = CMD_SYS_3G;
	}
	/* LTE system*/
	else
	{
		lwExtSysType = CMD_SYS_LTE;
	}

	/* 外部装置データ受信中状態チェック */
	/* 外部装置データ受信未処理の場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]gw_rcw_ExtRcvReqSta[lwExtSysType] : %d \n",__LINE__,gw_rcw_ExtRcvReqSta[lwExtSysType] );
	#endif
	if(RCD_EXTRCVREQ_IDLE == gw_rcw_ExtRcvReqSta[lwExtSysType] )
	{
		/* 処理終了 */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* 外部装置データ受信中状態がRE主導外部装置データ受信要求処理中（１）の場合 */
	if(RCD_EXTRCVREQ_RESND == gw_rcw_ExtRcvReqSta[lwExtSysType])
	{
		/* 3G */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]lwExtSysType : %d \n",__LINE__,lwExtSysType );
		#endif
		if(CMD_SYS_3G == lwExtSysType)
		{
			/* msg　　：　外部装置データ受信応答(3G：235（その他エラー） */
			lwParaChkNgRtc = CMD_OTH_ERR;
			
		}
		else
		{
			/* msg　　：　外部装置データ受信応答(LTE：51（処理NG(RE)）） */
			lwParaChkNgRtc = CMD_HDL_NG;
			
		}
		
		/* Send l3 message（l3_rct -> re_ec) */
        /* event　：　(0x4001)RE Forwarding Notice */
        /* msg　　：　外部装置データ受信応答(3G：235（その他エラー）、LTE：51（処理NG(RE)）） */
        m_rc_ExtDataRcvRspSubProc(lwExtSysType, lwParaChkNgRtc, gw_rcw_ExtRcvRspDestLinkNum[lwExtSysType]);
			
	}
	
	/* 障害情報を残す（外部装置データ受信要求内容） */
	/* FPT TBD*/
	
	/* 外部装置データ受信中状態：未処理 */
	gw_rcw_ExtRcvReqSta[ lwExtSysType] = RCD_EXTRCVREQ_IDLE;
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]lwExtSysType : %d \n",__LINE__,lwExtSysType );
	#endif
	
	/* バッファリング受信要求データ処理(システムタイプ(3G/LTE)) */
	m_rc_GetBufLnkDataRcv(lwExtSysType);

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
}

/* @} */


/* @} */

