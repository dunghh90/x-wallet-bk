/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_AisgSignal.c
 *  @brief  AISG信号受信処理
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
 *  @brief  AISG信号受信処理
 *  @note 
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01797 
 *  @date   2015/07/09 FPT)Yen Create new
 *  @date   2015/10/19 FPT)Hai M-RRU-ZSYS-01797 Update (プロセススレッド間シーケンス(4 TILT-INF信号) v0.4) 
 *  @date   2015/11/ FPT)Hai Update (プロセススレッド間シーケンス(4 TILT-INF信号) v0.4)
 *  @date   2016/06/20 FJT)koshida Update 16A(CeNB-F-128-034(F):外部装置データのポート番号のBCD変換削除対応)
 */
/********************************************************************************************************************/

VOID m_rc_AisgSignal( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	UINT		lwSigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	USHORT		lwSetbuf_rslt;
	USHORT		lwPortnum;
	USHORT		lwReNo;
	INT			ldwResult;
	INT			errcd;
	USHORT		lwTimerId;
	T_RRH_LAYER3	ltLayer3;
	USHORT			lwNgResult[CMD_MAX_SYSNUM] = { CMD_OTH_ERR,		/* 3G 235:その他エラー */
												   CMD_HDL_NG };	/* LTE 51:処理NG（RE） */

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
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_extdatsndreq.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]m_cm_chksig: %d \n",__LINE__,lwSigchk_rslt);
	#endif
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}
	
	/* 外部装置データ送信中状態チェック */
	/* 外部装置データ送信処理中の場合（0以外） */
	if (RCD_EXTSNDREQ_IDLE != gw_rcw_ExtSndReqSta[lwSigsys_rslt])
	{
		/* 外部装置データ送信要求信号をバッファリングする。 */
		lwSetbuf_rslt = m_rc_SetBufLnkDataSnd(lwSigsys_rslt, RCD_EXTSNDREQ_AISG, parm_p);
		/* NGの場合(バッファフル発生) */
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]m_rc_SetBufLnkDataSnd: %d \n",__LINE__,lwSetbuf_rslt);
		#endif
		if (CMD_RES_OK != lwSetbuf_rslt)
		{
			/* 3G */
			if(CMD_SYS_3G == lwSigsys_rslt)
			{
				/* 46: バッファフル(RE) */
				lwSetbuf_rslt = CMD_BUFF_FUL_RE;
			}
			/* LTE */
			else
			{
				/* 54: バッファフル(RE)(for LTE) */
				lwSetbuf_rslt = CMD_BUFF_FUL_RE_S3G;
			}
			/* 外部装置データ送信応答（バッファフル(RE)) */
			m_rc_ExtDataSndRspSubProc(lwSigsys_rslt, lwSetbuf_rslt );
		}
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* 配下CPRI番号 = P(ポート番号) - 01(D) */
	lwPortnum = parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM0];
	if(( CMD_NUM2 > lwPortnum )||( CMD_NUM15 < lwPortnum ))
	{
	    /* 処理終了(FHMが動作不可となるTILT-INF信号の場合は、FHM内で破棄し、OK応答する) */
		cm_Assert( CMD_ASL_USELOW, lwPortnum, "AISG Send PortNumber NG" );
		m_rc_ExtDataSndRspSubProc_OnlyMyself(parm_p, lwSigsys_rslt, CMD_NML);
		return;
	}
	lwReNo = lwPortnum - CMD_NUM1;
	
	/* Layer3状態取得 */
	f_cmn_com_layer3_get(lwSigsys_rslt, &ltLayer3);
	/* Layer3状態が「②RE起動中状態または④運用中状態」以外 */
	if(( E_RRH_LAYER3_RE_START != ltLayer3.layer3_re[lwReNo - CMD_NUM1] ) &&
	   ( E_RRH_LAYER3_RE_OPE   != ltLayer3.layer3_re[lwReNo - CMD_NUM1] ))
	{
		/* 外部装置データ送信応答（3G)235:その他エラー, (LTE)51:処理NG(RE) */
		m_rc_ExtDataSndRspSubProc(lwSigsys_rslt, lwNgResult[lwSigsys_rslt] );
 		return;
	}
	/* 外部装置データ送信中状態：AISG信号処理中（１） */
	gw_rcw_ExtSndReqSta[lwSigsys_rslt] = RCD_EXTSNDREQ_AISG;

	parm_p->cprircv_inf.link_num = lwPortnum - CMD_NUM1;

	/* 信号送信対象REを決定(1: 送信処理中) */
	gw_rcw_SigSndReNo[lwSigsys_rslt ][lwPortnum - CMD_NUM2] = RCD_SIGSNDRE_RUNNING;
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]gw_rcw_SigSndReNo[lwSigsys_rslt ][lwPortnum - CMD_NUM2]: %d \n",__LINE__,gw_rcw_SigSndReNo[lwSigsys_rslt ][lwPortnum - CMD_NUM2]);
	#endif
	
	/* Send l3 message */
	/* event　：　(0x4001)RE Forwarding Notice */
	/* msg　　：　外部装置データ送信要求 */
	l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,
					parm_p, (( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ *)parm_p)->head.uiLength );

	/* 3G system*/
 	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]lwSigsys_rslt: %d \n",__LINE__,lwSigsys_rslt);
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

	/* 「外部装置データ送信確認タイマ」開始 */
		/* タイマ番号：外部装置(TILT)データ送信確認 for 3G/LTE */
		/* タイマ値：(変数名A)外部装置データ送信確認タイマ(1000ms) */
		/* タイマモード：シングルモード */
		/* eventno ：CMD_TSKIF_TIMOUTNTC*/
		/* queueid ：CMD_TSKID_RCT*/
	ldwResult = cm_TStat(lwTimerId, CMD_TIMVAL_EXTDATASND, CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
	/*Start timer NG*/
 	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]cm_TStat: %d \n",__LINE__,ldwResult);
	#endif
	if(ldwResult != CMD_RES_OK)
	{
		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "外部装置データ送信確認タイマ Start NG" );
	}

	/* Return */
 	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;
}

/* @} */


/* @} */

