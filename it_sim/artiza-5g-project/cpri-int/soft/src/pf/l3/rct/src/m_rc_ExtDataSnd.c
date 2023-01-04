/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ExtDataSnd.c
 *  @brief  外部装置データ送信要求
 *  @date   2015/07/17 FPT) DuongCD create.
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

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Send [外部装置データ送信応答] to REC
 *  @note 
 *  @param  awSystemType [in] the system type LTE/3G
 *  @param  awBufResult	 [in] result (NG: don't check 配下REからの応答, OK: check 配下REからの応答)
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01796
 *  @date 	2015/09/30 FPT)HaiLC M-RRU-ZSYS-01796 fix bug takeuchi_BUG_101_1019 バッファフル検出したメッセージ受信時には状態を更新しないよう修正
 */
/********************************************************************************************************************/
VOID m_rc_ExtDataSndRspSubProc( USHORT awSystemType, USHORT awBufResult )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	CMT_TSKIF_CPRIRCV_EXTDATSNDRES 	*extsndrsp_p;	/* EXT data send response			*/
	UINT							ldwRsltchk;
	UINT 							ldwRet_breq;	/* Return value of buffer request	*/
	UINT							ldwSnd_ret;		/* Return value of sending			*/
	USHORT							lwLoop;

	extsndrsp_p = NULL;
	ldwRet_breq = CMD_NUM0;
	ldwSnd_ret = CMD_NUM0;
	
	/* Buffer request	 */
	ldwRet_breq = cm_L2BfGet(CMD_BUFCA_LAPSND,
						  sizeof(CMT_TSKIF_CPRIRCV_EXTDATSNDRES),
						  CMD_NUM1, (VOID **)&extsndrsp_p);
	
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]cm_L2BfGet : %d \n",__LINE__,ldwRet_breq );
	#endif
	
	if(ldwRet_breq != CMD_RES_OK)		/* If buffer request NG		*/
	{
		cm_MAbort(	CMD_ALMCD_BUFGET, 
					"m_ex_ExtDataTx", 
					"Buffer request error", 
					ldwRet_breq, 
					sizeof(CMT_TSKIF_CPRIRCV_EXTDATSNDRES), 
					CMD_NUM0);
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	extsndrsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRIF_EXTDATSNDRES) + sizeof(CMT_CPRISIGRCV_INF);
	/* 信号種別：3G: 0x9120 / LTE: 0x9121 */
	extsndrsp_p->cpridat_extdatsndres.signal_kind = CMD_CPRID_EXTDATSNDRES + awSystemType;

	/* 3G */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]awSystemType : %d \n",__LINE__,awSystemType );
	#endif
	if(CMD_SYS_3G == awSystemType)
	{
		/* 外部装置番号：4：TILT-INF */
		extsndrsp_p->cpridat_extdatsndres.extdev_no = EXTDEV_NUM_TILTINF_3G;
		/* 応答結果：235：その他エラー */
		ldwRsltchk = CMD_OTH_ERR;
	}
	else
	{
		/* 外部装置番号：2：TILT-INF */
		extsndrsp_p->cpridat_extdatsndres.extdev_no = EXTDEV_NUM_TILTINF_LTE;
		/* 応答結果：51:処理NG(RE) */
		ldwRsltchk = CMD_HDL_NG;
	}
	/* 信号送信対象REから応答結果を決定する */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]awBufResult : %d \n",__LINE__,awBufResult );
	#endif
	if(CMD_RES_OK != awBufResult)
	{
		extsndrsp_p->cpridat_extdatsndres.result = awBufResult;
	}
	else
	{
		/* ※いずれかの配下REからの応答がOKならば、OK応答とする。 */
		/* 　上記以外は、NG応答（LTE： 51=処理NG(RE)／3G： 235:その他エラー）とする。 */
		/* Loop(RE#1～16) */
		for( lwLoop = CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
		{
			if(RCD_SIGSNDRE_OK == gw_rcw_SigSndReNo[ awSystemType][ lwLoop])
			{
				#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]lwLoop : %d \n",__LINE__,lwLoop );
				#endif
				ldwRsltchk = CMD_RES_OK;
				break;
			}
		}
		extsndrsp_p->cpridat_extdatsndres.result = ldwRsltchk;
	}

//TODO compile error
//	/* External device data sending and receiving running history */
//	m_ex_cm_RunHisSet(EXD_RUNHIS_FLG_SND, CMD_TSKID_LPB, CMD_TSKIF_CPRISNDNTC, (UCHAR *)extsndrsp_p);

	/* 外部装置データ送信応答 */
	/* Send EXT data transmission response to LAPB task 				 */
	ldwSnd_ret = m_cm_L3MsgSend_REC(extsndrsp_p);
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]m_cm_L3MsgSend_REC : %d \n",__LINE__,ldwSnd_ret );
	#endif
	if(ldwSnd_ret != CMD_L3_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, CMD_NUM1, 
								"m_ex_ExtDataTx MSGSndNG");
	}

	/* Not reset state when buffer full*/
	if( (CMD_BUFF_FUL_RE == awBufResult) || (CMD_BUFF_FUL_RE_S3G == awBufResult) )
	{
		return;
	}

	/* 信号送信対象RE：全てを未処理に設定 */
	for( lwLoop = CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++ )
	{
		gw_rcw_SigSndReNo[awSystemType][lwLoop] = RCD_SIGSNDRE_IDLE;
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]lwLoop : %d \n",__LINE__,lwLoop );
		#endif
	}

	/* 外部装置データ送信中状態：未処理 */
	gw_rcw_ExtSndReqSta[awSystemType] = RCD_EXTSNDREQ_IDLE;
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]gw_rcw_ExtSndReqSta[awSystemType] : %d \n",__LINE__,gw_rcw_ExtSndReqSta[awSystemType] );
	#endif
	
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;
	
}
/* @} */
/********************************************************************************************************************/
/**
 *  @brief  Send [外部装置データ送信応答] to REC
 *  @note   配下REからの応答を考慮する必要がないケース用
 *  @param  parm_p       [in] 外部装置データ送信要求
 *  @param  awSystemType [in] the system type LTE/3G
 *  @param  awBufResult  [in] result (NG: don't check 配下REからの応答, OK: check 配下REからの応答)
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01738
 *  @date   2015/09/30 TDI)satou M-RRU-ZSYS-01738 create
 */
/********************************************************************************************************************/
VOID m_rc_ExtDataSndRspSubProc_OnlyMyself( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p, USHORT awSystemType, USHORT awBufResult ) {
    CMT_TSKIF_CPRIRCV_EXTDATSNDRES  *extsndrsp_p;   /* EXT data send response */
    UINT ret;

    ret = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRIRCV_EXTDATSNDRES), CMD_NUM1, (VOID**)&extsndrsp_p);
    if (CMD_RES_OK != ret) {
        cm_MAbort(CMD_ALMCD_BUFGET,
                  "m_rc_ExtDataSndRspSubProc_OnlyMyself",
                  "Buffer request error",
                  ret,
                  sizeof(CMT_TSKIF_CPRIRCV_EXTDATSNDRES),
                  0);
        return;
    }

    extsndrsp_p->head.uiDstPQueueID               = D_RRH_PROCQUE_L2;
    extsndrsp_p->head.uiDstTQueueID               = 0;
    extsndrsp_p->head.uiSrcPQueueID               = D_RRH_PROCQUE_L3;
    extsndrsp_p->head.uiSrcTQueueID               = CMD_TSKID_RCT;
    extsndrsp_p->head.uiLength                    = sizeof(CMT_TSKIF_CPRIRCV_EXTDATSNDRES);
    extsndrsp_p->cpridat_extdatsndres.signal_kind = CMD_CPRID_EXTDATSNDRES + awSystemType;
    extsndrsp_p->cpridat_extdatsndres.result      = awBufResult;
    extsndrsp_p->cpridat_extdatsndres.extdev_no   = parm_p->cpridat_extdatsndreq.extdev_no;

    ret = m_cm_L3MsgSend_REC(extsndrsp_p);
    if (CMD_L3_OK != ret) {
        cm_Assert(CMD_ASL_DBGLOW, 1, "m_rc_ExtDataSndRspSubProc_OnlyMyself m_cm_L3MsgSend_REC NG");
    }
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  外部装置データ送信応答受信処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_ExtDataSndRspProc( CMT_TSKIF_CPRIRCV_EXTDATSNDRES* parm_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	UINT		ldwSigchk_rslt;				/*  result of checking signal */
	USHORT		lwSigsys_rslt;				/*  system by checking signal */
	INT			ldwerrcd;
	INT			ldwResult; 
	USHORT		lwTimerId;
	USHORT		lwReNo;
	USHORT		lwReCnt;

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
	ldwSigchk_rslt = m_cm_chksig(parm_p->cpridat_extdatsndres.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	if(ldwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}

	lwReNo = parm_p->cprircv_inf.link_num;
	/* 信号送信対象REチェック */
	/* 対象REからの応答ではないの場合(信号送信対象REが「送信処理中」以外の場合) */
	if( RCD_SIGSNDRE_RUNNING != gw_rcw_SigSndReNo[lwSigsys_rslt][lwReNo - CMD_NUM1] )
	{
		/* Return */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] parm_p->cpridat_extdatsndres.result : %d \n",__LINE__, parm_p->cpridat_extdatsndres.result );
	#endif
	if( CMD_RES_OK == parm_p->cpridat_extdatsndres.result )
	{
		/* 信号送信対象RE：応答結果 OK*/
		gw_rcw_SigSndReNo[lwSigsys_rslt][lwReNo - CMD_NUM1] = RCD_SIGSNDRE_OK;
	}
	else
	{
		/* 信号送信対象RE：応答結果 NG*/
		gw_rcw_SigSndReNo[lwSigsys_rslt][lwReNo - CMD_NUM1] = RCD_SIGSNDRE_NG;
	}

	/* 信号送信対象REチェック */
	/* 未応答の対象REあり(1: 送信処理中) */
	for( lwReCnt = CMD_NUM0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++ )
	{
		if(RCD_SIGSNDRE_RUNNING == gw_rcw_SigSndReNo[lwSigsys_rslt][lwReCnt])
		{
			/* Return */
			#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d] lwReCnt : %d \n",__LINE__, lwReCnt );
			#endif
			#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
			#endif
			return;
		}
	}

	/* タイマ番号 */
	/* 3G system*/
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] lwSigsys_rslt : %d \n",__LINE__, lwSigsys_rslt );
	#endif
	if( CMD_SYS_3G == lwSigsys_rslt)
	{
		lwTimerId = CMD_TIMID_EXT_DAT_SND_3G;
	}
	/* LTE system*/
	else
	{
		lwTimerId = CMD_TIMID_EXT_DAT_SND_LTE;
	}

	/* 「外部装置データ送信確認タイマ」停止 */
	ldwResult = cm_TStop( lwTimerId, &ldwerrcd);
	/* Stop timer NG */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] cm_TStop : %d \n",__LINE__, ldwResult );
	#endif
	if( ldwResult != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, ldwResult, "外部装置データ送信確認タイマ Stop NG" );
	}

	/* 外部装置データ送信応答 */
	/* ※いずれかの配下REからの応答がOKならば、OK応答とする。 */
	/* 　上記以外は、NG応答（LTE： 51=処理NG(RE)／3G： 235:その他エラー）とする。*/
	m_rc_ExtDataSndRspSubProc(lwSigsys_rslt, CMD_RES_OK);

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief 外部装置データ送信要求処理
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01721
 *  @Bug_No M-RRU-ZSYS-01738
 *  @date	2015/07/17 Fpt)DuongCD
 *  @date   2015/09/24 TDIPS)Lay M-RRU-ZSYS-01721 信号長のチェック誤りを修正
 *  @date   2015/09/30 TDI)satou M-RRU-ZSYS-01738 外部装置データ送信応答が必ずNG応答になる不具合の修正
 *  @date   2015/10/23 TDI)satou FHM技説-設計変更-020
 *  @date   2016/06/20 FJT)koshida Update 16A(CeNB-F-128-034(F):外部装置データのポート番号のBCD変換削除対応)
 */
/********************************************************************************************************************/
VOID m_rc_ExtDataSndReq( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	UINT							lwSigchk_rslt;						/*  result of checking signal */
	USHORT							lwSigsys_rslt;						/* system after checking signal */	
	UCHAR							lwPortnum;
	UCHAR							lwCont;
	USHORT							lwSignalLengh;
	UCHAR							lwEquipType;
	UCHAR							lwDevnum;
	USHORT							lwParaChkNgRtc;

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_extdatsndreq.signal_kind,&lwSigsys_rslt);

	/* validate signal NGの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] m_cm_chksig : %d \n",__LINE__, lwSigchk_rslt );
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
	printf( "[%d] lwSigsys_rslt : %d \n",__LINE__, lwSigsys_rslt );
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
		( EXTDEV_NUM_TILTINF_3G != parm_p->cpridat_extdatsndreq.extdev_no ))
	{
		/* 外部装置データ送信応答（３G：235その他エラー/LTE：51処理NG(RE)) */
	    // m_rc_ExtDataSndRspSubProc_OnlyMyself( parm_p, lwSigsys_rslt, lwParaChkNgRtc );
		/* 処理終了 */
		cm_Assert( CMD_ASL_USELOW, parm_p->cpridat_extdatsndreq.extdev_no, "Ext eqp kind NG for 3G:RSP NG" );
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
	    return;
	}
 	
	/* ※外部装置番号がTILT-INF(LTE=２)以外 */
	if (( CMD_SYS_S3G == lwSigsys_rslt ) &&
		( EXTDEV_NUM_TILTINF_LTE != parm_p->cpridat_extdatsndreq.extdev_no ))
	{
		/* 外部装置データ送信応答（３G：235その他エラー/LTE：51処理NG(RE)) */
	    m_rc_ExtDataSndRspSubProc_OnlyMyself( parm_p, lwSigsys_rslt, lwParaChkNgRtc );
		/* 処理終了 */
		cm_Assert( CMD_ASL_USELOW, parm_p->cpridat_extdatsndreq.extdev_no, "Ext eqp kind NG for LTE:RSP NG" );
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
	    return;
	}
	
 	/* ※送信バイト数が1～2048でない */
	if (( parm_p->cpridat_extdatsndreq.snd_byte < MIN_EXTDAT_MSGSND_SIZE ) ||
		( parm_p->cpridat_extdatsndreq.snd_byte > MAX_EXTDAT_MSGSND_SIZE ))
	{
		/* 外部装置データ送信応答（３G：235その他エラー/LTE：51処理NG(RE)) */
	    m_rc_ExtDataSndRspSubProc_OnlyMyself( parm_p, lwSigsys_rslt, lwParaChkNgRtc );
		/* 処理終了 */
		cm_Assert( CMD_ASL_USELOW, parm_p->cpridat_extdatsndreq.snd_byte, "Send byte length NG:RSP NG" );
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* ﾎﾟｰﾄ番号 */
	lwPortnum = parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM0];
	/* 装置種別 */
	lwEquipType = D_L3_CNVBCD_DEC(parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM3]);

	/* FHMの自ポート宛て ※P(ポート番号)=01(D) */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] lwPortnum : %d \n",__LINE__, lwPortnum );
	#endif
	if( CMD_NUM1 == lwPortnum )
	{
        /*
         * TILT-INF信号が異常の場合でもCPRI信号としては正常応答とし、
         * FHMが動作不可となるTILT-INF信号の場合は、FHM内で破棄する
         */
        m_rc_ExtDataSndRspSubProc_OnlyMyself(parm_p, lwSigsys_rslt, CMD_NML);

		/* 信号長 */
		lwSignalLengh = parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM1];
		lwSignalLengh = (( lwSignalLengh << 8 ) |
						 ( parm_p->cpridat_extdatsndreq.snd_msg[EXTDEV_NUM_TILTINF_LTE] ));
		/* 信号種別 */
		lwCont = parm_p->cpridat_extdatsndreq.snd_msg[EXTDEV_NUM_TILTINF_3G];

		/* メッセージパラメータチェック */

		/* ※信号長が244byte超える場合 */
		/* 信号長の先頭0xD0の判定追加 */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d] lwSignalLengh : %d \n",__LINE__, lwSignalLengh );
		#endif
		if ( ((lwSignalLengh & 0xFF00) != 0xD000) ||
			((lwSignalLengh & 0x00FF) > MAX_SIGNAL_LENGHT) )
		{
			/* 処理終了(FHMが動作不可となるTILT-INF信号の場合は、FHM内で破棄する) */
			cm_Assert( CMD_ASL_USELOW, lwSignalLengh, "TILT-INF Signal Length NG for FHM:nop" );
			#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
			#endif
		    return;
		}

		/* ※装置種別(A)が自ベンダでは無い場合 */
 		/* TBD 自ベンダ = ??? */
 
		/* ※信号種別(CONT)が配下REリセット要求信号(05)または、配下REON/OFF要求信号(04) */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d] lwCont : %d \n",__LINE__, lwCont );
		#endif
		if(( SUBRE_RESET_REQ_SIG == lwCont ) || ( SUBRE_ONOF_SIG == lwCont ))
		{
			/* 装置番号 */
			lwDevnum = D_L3_CNVBCD_DEC(parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM5]);

			/* 装置番号(E)が1～16以外の場合 */
			#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d] lwDevnum : %d \n",__LINE__, lwDevnum );
			#endif
			if( (lwDevnum < DEV_NUM_MIN ) || ( lwDevnum > DEV_NUM_MAX ) ||
			    (D_RRH_OK != D_L3_CHKBCD(parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM5])) )
			{
			    /* 処理終了(FHMが動作不可となるTILT-INF信号の場合は、FHM内で破棄する) */
				cm_Assert( CMD_ASL_USELOW, lwDevnum, "TILT-INF Signal Device Number NG for FHM:nop" );
				#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
				#endif
				return;
			}
		}

    	/* 信号種別チェック */
    	#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d] lwCont : %d \n",__LINE__, lwCont );
		#endif
 		switch (lwCont)
 		{
    		/* 状態要求 ※CONT(信号種別)=01(H) */
 			case STATE_REQ_SIG:
    			m_rc_StatusReportCont( parm_p );
 				break;

    		/* 局リセット要求 ※CONT(信号種別)=C9(H) */
	 		case RESET_REQ_SIG:
    			m_rc_SystermResetCont( parm_p );
				break;

    		/* 配下REリセット要求 ※CONT(信号種別)=05(H) */
	 		case SUBRE_RESET_REQ_SIG:
 				m_rc_UnderReResetCont( parm_p );
				break;

    		/* 局ON/OFF要求※CONT(信号種別)=88(H) */
	 		case ONOFF_REQ_SIG:
 				m_rc_SystermOnOffCont( parm_p );
				break;

    		/* 配下REON/OFF要求※CONT(信号種別)=04(H) */
	 		case SUBRE_ONOF_SIG:
 				m_rc_UnderReOnOffCont( parm_p );
				break;

    		/* ヘルスチェック要求 ※CONT(信号種別)=00(H) */
	 		case HEAL_CHK_REQ_SIG:
 				m_rc_HealthChkCont( parm_p );
				break;

    		/* その他 */
			default :
			    /* 処理終了(FHMが動作不可となるTILT-INF信号の場合は、FHM内で破棄する) */
				cm_Assert( CMD_ASL_USELOW, lwCont, "TILT-INF Signal kind NG for FHM:nop" );
				#ifdef FHM_RCT_DEBUG_FOR_IT1
				printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
				#endif
				return;
 		}
	}
    /* その他 */
	else
	{
		/* AISG信号 ※A(装置種別)=05(D)or06(D)=AISG信号 */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d] lwEquipType : %d \n",__LINE__, lwEquipType );
		#endif
		if(( CMD_NUM5 == lwEquipType ) || ( CMD_NUM6 == lwEquipType ))
		{
			/* AISG信号受信処理 */
			m_rc_AisgSignal(parm_p );
		}
    	/* その他（非AISG信号） */
		else
		{
			/* 非AISG信号受信処理 */
			m_rc_NoneAisgSignal(parm_p );
		}
	}
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief 外部装置データ送信応答処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date	2015/07/08 FPT)DuongCD create
 */
/********************************************************************************************************************/

VOID m_rc_ExtDataSndRsp( CMT_TSKIF_CPRIRCV_EXTDATSNDRES* parm_p)
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	UINT		ldwSigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */

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
	ldwSigchk_rslt = m_cm_chksig(parm_p->cpridat_extdatsndres.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] m_cm_chksig : %d \n",__LINE__, ldwSigchk_rslt );
	#endif
	if(ldwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}
	
	/* 外部装置データ送信中状態チェック */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] gw_rcw_ExtSndReqSta[lwSigsys_rslt] : %d \n",__LINE__, gw_rcw_ExtSndReqSta[lwSigsys_rslt] );
	#endif
	if (RCD_EXTSNDREQ_IDLE == gw_rcw_ExtSndReqSta[lwSigsys_rslt])
	{
		/* 未処理の場合（0） */
		/* Return */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* 外部装置データ送信応答受信 */
	m_rc_ExtDataSndRspProc(parm_p);
	
	/* 外部装置データ送信中状態チェック */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] gw_rcw_ExtSndReqSta[lwSigsys_rslt] : %d \n",__LINE__, gw_rcw_ExtSndReqSta[lwSigsys_rslt] );
	#endif
	if (RCD_EXTSNDREQ_IDLE != gw_rcw_ExtSndReqSta[lwSigsys_rslt])
	{
		/* 処理中の場合（0以外） */
		/* Return */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* バッファリング送信要求データ処理(システムタイプ(3G/LTE)) */
	m_rc_GetBufLnkDataSnd(lwSigsys_rslt);

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;
}
/* @} */



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  外部装置データ送信応答待ちタイムアウト
 *  @note 
 *  @param  adwTimerId [in] the timer ID
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_ExtDataSndRspTO( UINT adwTimerId)
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	USHORT		lwExtSysType;
	USHORT		lwExtSndReqSta;

	/* 3G system*/
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] adwTimerId : %d \n",__LINE__, adwTimerId );
	#endif
	if(CMD_TIMID_EXT_DAT_SND_3G == adwTimerId)
	{
			lwExtSysType = CMD_SYS_3G;
	}
	/* LTE system*/
	else
	{
		lwExtSysType = CMD_SYS_LTE;
	}
	lwExtSndReqSta = gw_rcw_ExtSndReqSta[lwExtSysType];
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d] lwExtSndReqSta : %d \n",__LINE__, lwExtSndReqSta );
	#endif

	/* 外部装置データ送信中状態チェック */
		/* 未処理の場合（0） */
		if(RCD_EXTSNDREQ_IDLE == lwExtSndReqSta)
		{
			/* Return */
			#ifdef FHM_RCT_DEBUG_FOR_IT1
			printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
			#endif
			return;
		}

	/* 外部装置データ送信応答 */
	/* ※いずれかの配下REからの応答がOKならば、OK応答とする。 */
	/* 　上記以外は、NG応答（LTE： 51=処理NG(RE)／3G： 235:その他エラー）とする。 */
	m_rc_ExtDataSndRspSubProc(lwExtSysType, CMD_RES_OK);

	/* バッファリング送信要求データ処理(システムタイプ(3G/LTE)) */
	m_rc_GetBufLnkDataSnd(lwExtSysType);

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;
}
/* @} */
/* @} */

