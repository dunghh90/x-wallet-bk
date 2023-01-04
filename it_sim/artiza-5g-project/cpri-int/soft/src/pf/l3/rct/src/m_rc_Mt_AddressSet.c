/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_Mt_AddressSet.c
 *  @brief  MTアドレス設定処理
 *  @date
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-35)
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
 *  @brief  MTアドレス設定要求処理
 *  @note
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2015/11/09 TDIPS)Takeuchi 技説QA120
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-35)
 */
/********************************************************************************************************************/

VOID m_rc_Mt_AddressSetReq( CMT_TSKIF_CPRIRCV_MTADDSETREQ* parm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif
	UINT		lwSigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	INT			ldwResult;
	INT 		errcd;
	USHORT		lwTimerId;

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_mtaddsetreq.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");
	}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s cpridat_mtaddsetreq.tgtmt:%d \n",__LINE__,__FUNCTION__,parm_p->cpridat_mtaddsetreq.tgtmt);
#endif
	/* 対象MTが0(FLD-MT)/1(SV-MT/eNB-MT)以外場合 */
	/* 対象MTが"FLD-MT"の場合 and 対象MTが"SV-MT"の場合 */
	if((parm_p->cpridat_mtaddsetreq.tgtmt != CMD_FLD_MT)&&( parm_p->cpridat_mtaddsetreq.tgtmt != CMD_SV_MT))
	{
		/* MTアドレス設定応答(NG)送信					*/
		if( CMD_SYS_3G == lwSigsys_rslt )
		{
			m_rc_MtAddrSetRspSnd( lwSigsys_rslt, CMD_SET_NG );
			return;
		}
		else
		{
			m_rc_MtAddrSetRspSnd(lwSigsys_rslt, CMD_HDL_NG);
		}
		return;
	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_MTADDSET_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_MTADDSET_LTE;
// 	}

// 	/* REC主導に対するMTアドレス設定状態が処理中の場合 */
// 	if( D_L3_ON == gw_rcw_Mt_AdressSetReqSts[lwSigsys_rslt] )
// 	{
// 		/* 「MTアドレス設定確認タイマ」停止 */
// 		cm_TStop( lwTimerId, &errcd );
// 	}

	/* MTアドレス設定要求をRe/recに送信する */
	ldwResult = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, sizeof( CMT_TSKIF_CPRIRCV_MTADDSETREQ ) );
	/*Sending fail*/
	if( D_RRH_OK != ldwResult)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							ldwResult,
							"Sending msg NG");
		return;
	}

// 	/* MTアドレス設定確認タイマ開始 */
// 	ldwResult = cm_TStat(lwTimerId, CMD_TIMVAL_MT_ADDR_SETTING, CMD_TIMMD_SINGLE,
// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
// 	/*Start timer NG*/
// 	if(ldwResult != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, lwTimerId, "MTアドレス設定確認タイマ開始 NG" );
// 		return;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]cm_TStat(TimerId: %d) \n",__LINE__,lwTimerId);
// #endif
//	/* Save MT address seting info */
//	memcpy( &gwt_Mt_AdressSetInfo[lwSigsys_rslt],&parm_p->cpridat_mtaddsetreq, sizeof(CMT_CPRIF_MTADDSETREQ) );

//	/* REC主導に対するMTアドレス設定状態設定-->処理中 */
//	gw_rcw_Mt_AdressSetReqSts[lwSigsys_rslt] = D_L3_ON;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif

	return;

}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  MTアドレス設定応答処理
 *  @note
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2015/09/27 TDI)satou Layer3状態の運用中状態設定処理を追加
 *  @date   2015/10/22 TDI)satou その他未実装-007 LED設定処理を追加
 *  @date   2015/11/09 TDIPS)sasaki IT2問処No.163対処
 *  @date   2015/11/10 TDIPS)sasaki IT2問処No.163対処(LTE/3G共用に対応)
 *  @date   2016/02/05 TDI)enoki 3G時のREC起動シーケンス完了判断をMTアドレス設定からREキャリア状態報告に変更
 *  @date   2016/11/16 FJT)ohashi 16B スロット数制限機能追加(強制停止状態でのOPE状態移行の抑止)
 *  @date   2017/01/26 FJT)ohashi 3201-16B-TS96対処 キューイングREのCPRI接続通知条件を追加
 *  @date 	2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-35)
 */
/********************************************************************************************************************/

VOID m_rc_Mt_AddressSetRsp( CMT_TSKIF_CPRIRCV_MTADDSETRES* parm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Start \n",__LINE__,__FUNCTION__);
#endif

	UINT		sigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	USHORT		lwTimerId;
	UINT		rec_startup_cmp = D_RRH_OFF;
	INT 		errcd;
	INT         linkno;

	USHORT		cpr_idx;
	T_RRH_LAYER3 	layer3;
	
//16B スロット数制限機能追加 start
	T_RRH_LAYER3 shmLayer3[CMD_MAX_SYSNUM];
//16B スロット数制限機能追加 end
	UINT	cnt1;
	UINT	cnt2;


	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}
	/* validate signal */
	sigchk_rslt = m_cm_chksig(parm_p->cpridat_mtaddsetres.signal_kind,&lwSigsys_rslt);

	/* signalがNGの場合 */
	if(sigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");
	}

// 	/* REC主導に対するMTアドレス設定状態が未処理の場合 */
// 	if(gw_rcw_Mt_AdressSetReqSts[lwSigsys_rslt] == D_L3_OFF)
// 	{
// 		/* アサート出力処理 */
// 		cm_Assert(	CMD_ASL_USELOW,
// 							lwSigsys_rslt,
// 							"REC主導に対するMTアドレス設定状態←未処理");
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 		return;
// 	}

// 	/* 3G system*/
// 	if( CMD_SYS_3G == lwSigsys_rslt)
// 	{
// 		lwTimerId = CMD_TIMID_MTADDSET_3G;
// 	}
// 	/* LTE system*/
// 	else
// 	{
// 		lwTimerId = CMD_TIMID_MTADDSET_LTE;
// 	}

// 	/* 「MTアドレス設定確認タイマ」停止 */
// 	cm_TStop( lwTimerId, &errcd );
// 	/* REC主導に対するMTアドレス設定状態設定-->未処理 */
// 	gw_rcw_Mt_AdressSetReqSts[lwSigsys_rslt] = D_L3_OFF;

	/* 配下REのLayer３状態が②起動中/④運用中以外によるNGの場合、正常扱いとする */
	if( CMD_OTH_ERR == parm_p->cpridat_mtaddsetres.result )
	{
		parm_p->cpridat_mtaddsetres.result = CMD_RES_OK;
	}

// 	/* 応答(0：正常)の場合 */
// 	if( CMD_RES_OK == parm_p->cpridat_mtaddsetres.result )
// 	{
// 		/* Copy MT address seting info into the shared memory */
// 		f_cmn_com_mtaddset_set( lwSigsys_rslt, gwt_Mt_AdressSetInfo[lwSigsys_rslt].tgtmt, (T_RRH_MTADDSET*)(&gwt_Mt_AdressSetInfo[lwSigsys_rslt]) );

// 		/*
// 		 * 「対象MT」が「SV-MT」の正常応答を初回送信時、RECの起動シーケンスが完了したと判断し、
// 		 * この時点で立ち上げ応答(正常)を受信している配下REについて、Layer3状態を「運用中状態」に設定する
// 		 */
// 		if ((CMD_SV_MT == gwt_Mt_AdressSetInfo[lwSigsys_rslt].tgtmt)
// 		&&  (D_RRH_OFF == rec_Startup_First_flg[lwSigsys_rslt]     )) {
// 			rec_Startup_First_flg[lwSigsys_rslt] = D_RRH_ON;

// //16B スロット数制限機能追加 start
// //強制停止状態時はE_RRH_LAYER3_RE_OPEへの状態移行を実施しない
// 			f_cmn_com_layer3_get(lwSigsys_rslt, &shmLayer3[lwSigsys_rslt]);

// 			for (linkno = D_RRH_CPRINO_RE1; linkno <= D_RRH_CPRINO_RE_MAX; linkno++) {
// 				if (re_startupRSP[linkno][lwSigsys_rslt] == D_RRH_ON)
// 				{
// 			        if (shmLayer3[lwSigsys_rslt].layer3_re[linkno -1] != E_RRH_LAYER3_RE_STOP){
// 						(VOID)f_cmn_com_layer3_set_each(linkno, lwSigsys_rslt, (USHORT)E_RRH_LAYER3_RE_OPE);
// 					}
// //16B スロット数制限機能追加 end
// 				}
// 			}
// 			/* REC起動完了までに変更されたRE接続有無状態の内容を反映する */
// 			m_rc_ReUseApplySettingsInBoot();

// 			/* LED設定処理 */
// 			m_cm_WriteLEDReg();
// 		}
// 	}

	/* MTアドレス設定応答をl2/lpbに送信する */
	m_rc_MtAddrSetRspSnd(lwSigsys_rslt, parm_p->cpridat_mtaddsetres.result);

// 	/****************************************************************************/
// 	/* キューイングされたCPRI接続通知があれば、キュー先頭から処理開始			*/
// 	/****************************************************************************/
// 	rec_startup_cmp = (cmw_validsysinfo == CMD_SYS_S3G) ? (rec_Startup_First_flg[CMD_SYS_S3G]) : (rec_Startup_First_flg[CMD_SYS_S3G] & rec_Startup_First_flg[CMD_SYS_3G]);
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s rcw_buffcnt[%d] rec_startup_cmp[%d]\n",__LINE__,__FUNCTION__,rcw_buffcnt,rec_startup_cmp);
// #endif

// 	/* REC-FHM間起動完了してれば、キューイングの先頭から処理開始 */
// 	if(rcw_buffcnt > CMD_NUM0 && rec_startup_cmp != D_RRH_OFF)
// 	{
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 		printf("[%d]%s rcw_cpri[0]=%d rcw_system_type[0]=%d\n",__LINE__,__FUNCTION__,rcw_cpri[0],rcw_system_type[0]);
// #endif

// 		/* キューイング数が0となったら抜ける */
// 		while(rcw_buffcnt > CMD_NUM0)
// 		{
// 			/* 該当systemのL3状態取得 */
// 			f_cmn_com_layer3_get(rcw_system_type[0], &layer3);

// 			/* CPRI index算出 */
// 			cpr_idx = rcw_cpri[0] -1;

// 			/* 起動待ちアイドル状態のRE */
// 			if(layer3.layer3_re[cpr_idx] == E_RRH_LAYER3_RE_IDLE)
// 			{
// 				/* REの状態を全て確認 */
// 				for ( cnt1=CMD_NUM0; cnt1 < CMD_SYS_BOTH_3G_S3G	; cnt1++ )
// 				{
// 					f_cmn_com_layer3_get(cnt1, &layer3);

// 					for ( cnt2=CMD_NUM0; cnt2 < D_RRH_CPRINO_RE_MAX; cnt2++ )
// 					{
// 						/* RE起動中状態のREがあれば、LOOPを抜け、起動完了後に実施する */
// 						if(layer3.layer3_re[cnt2] == E_RRH_LAYER3_RE_START)
// 						{
// 							cm_Assert( CMD_ASL_USELOW, 0, "Skip RE Start [other RE start]" );

// 							/* 起動中REが存在していたら処理終了 */
// 							return;
// 						}
// 					}
// 				}

// 				/* キュー先頭のCPRI接続通知を処理 */
// 				m_rc_ReStr_Req(rcw_cpri[0], rcw_system_type[0]);
// 				cm_Assert( CMD_ASL_USELOW, 
// 					(((rcw_system_type[0] << 24 ) & 0xFF000000 ) | ((rcw_cpri[0] << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
// 					"RE Start" );

// 				/* 処理したCPRI接続通知を削除 */
// 				memcpy(&rcw_cpri[0], &rcw_cpri[1], sizeof(rcw_cpri[0]) * (rcw_buffcnt - 1));
// 				memcpy(&rcw_system_type[0], &rcw_system_type[1], sizeof(rcw_system_type[0]) * (rcw_buffcnt - 1));
// 				rcw_buffcnt--;

// 				/* 実施したらLOOPを抜ける */
// 				break;
// 			}
// 			else
// 			{
// 				/* 起動不要であるため、CPRI接続通知を削除し、次のキューを検索 */
// 				memcpy(&rcw_cpri[0], &rcw_cpri[1], sizeof(rcw_cpri[0]) * (rcw_buffcnt - 1));
// 				memcpy(&rcw_system_type[0], &rcw_system_type[1], sizeof(rcw_system_type[0]) * (rcw_buffcnt - 1));
// 				rcw_buffcnt--;

// 				cm_Assert( CMD_ASL_USELOW, 
// 					(((rcw_system_type[0] << 24 ) & 0xFF000000 ) | ((rcw_cpri[0] << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
// 					"Skip RE Start [NOT IDLE] " );
// 			}
// 		}
// 	}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
	return;
}

/* @} */

// /* @{ */
// /********************************************************************************************************************/
// /**
//  *  @brief  「MTアドレス設定確認タイマ」タイムアウト(REC)処理
//  *  @note
//  *  @param  adwTimerId [in] timer id
//  *  @return None
//  *  @date
//  *  @date  2015/10/26 FPT)Yen fix bug IT3 No 140
//  *  @date  2015/11/08 TDIPS)Takeuchi 技説QA120
 // *  @date  2021/02/04 M&C)Tri.hn Delete function based on spec no.68 (sequence 3-35)
//  */
// /********************************************************************************************************************/

// VOID m_rc_Mt_AddressSetTo( UINT adwTimerId )
// {
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s Start(TimerId:%d) \n",__LINE__,__FUNCTION__,adwTimerId);
// #endif
// 	USHORT		lwSystemType;
// 	CMT_TSKIF_CPRISND_ADDSETTOREQ		ltReq;
// 	INT			ldwResult;
// 	INT 		errcd;

// 	/*タイマIdが3G タイマの場合*/
// 	if( CMD_TIMID_MTADDSET_3G == adwTimerId )
// 	{
// 		lwSystemType = CMD_SYS_3G;
// 	}
// 	/*タイマIdがLTE タイマの場合*/
// 	else
// 	{
// 		lwSystemType = CMD_SYS_LTE;
// 	}

// 	/* MTアドレス設定確認タイマ開始 */
// 	ldwResult = cm_TStat(adwTimerId, CMD_TIMVAL_MT_ADDR_SETTING, CMD_TIMMD_SINGLE,
// 					CMD_TSKIF_TIMOUTNTC, CMD_TSKID_RCT, &errcd);
// 	/*Start timer NG*/
// 	if(ldwResult != CMD_RES_OK)
// 	{
// 		cm_Assert( CMD_ASL_DBGLOW, adwTimerId, "MTアドレス設定確認タイマ開始 NG" );
// 		return;
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]cm_TStat(TimerId: %d) \n",__LINE__,adwTimerId);
// #endif

// 	cm_MemClr(&ltReq, sizeof(ltReq));
// 	/* Create request msg */
// 	ltReq.head.uiLength = sizeof( CMT_TSKIF_HEAD ) + sizeof( CMT_CPRISIGSND_INF ) + sizeof( CMT_CPRIF_FTPADDSETTOREQ );

// 	ltReq.head.uiSignalkind  = CMD_CPRID_MTADDSETTOREQ + lwSystemType;
// 	ltReq.cpridat_addsettoreq.signal_kind = CMD_CPRID_MTADDSETTOREQ + lwSystemType;
// 	/* タイムアウトeventをRe/recに送信する */
// 	l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  &ltReq, sizeof( CMT_TSKIF_CPRISND_ADDSETTOREQ ) );

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
// #endif
// 	return;
// }


/********************************************************************************************************************/
/**
 *  @brief    Send MT address setting response function
 *  @note     send MT address setting response,according to the input response result
 *  @param    systype   [in]   System type
 *  @param    result    [in]   Response result
 *  @return   None
 *  @retval   -
 *  @date     2008/07/28 FFCS)zhengmh create
 *  @date     2013/11/22 FFCS)zhaodx modify for zynq
 */
/********************************************************************************************************************/

VOID m_rc_MtAddrSetRspSnd(UINT systype,USHORT result)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s Start(systype:%d, result:%d) \n",__LINE__,__FUNCTION__,systype,result);
#endif
	UINT ret_breq;			/* Retrun value of buffer request			*/
	UINT l3snd_ret;			/* Return value of sending layer 3 message	*/
	/* Pointer of CPRI信号受信通知(MTアドレス設定応答)	*/
	CMT_TSKIF_CPRIRCV_MTADDSETRES *mtaddrrsp_p = NULL;

	/* Buffer request	 */
	ret_breq = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRIRCV_MTADDSETRES),
						CMD_NUM1, (VOID **)&mtaddrrsp_p);

	if(ret_breq != CMD_RES_OK)		/* If buffer request NG		*/
	{
		cm_MAbort(CMD_ALMCD_BUFGET,
		"m_mt_cm_MtAddrSetRsp",
		"Buffer request error",
		ret_breq,
		sizeof(CMT_TSKIF_CPRIRCV_MTADDSETRES),
		CMD_NUM0);
		return;
	}

	mtaddrrsp_p->cpridat_mtaddsetres.result = result;

	/* タスク間ヘッダ部編集	*/
	mtaddrrsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRIF_MTADDSETRES) + sizeof(CMT_CPRISIGRCV_INF);

	/* MTアドレス設定応答				*/
	mtaddrrsp_p->cpridat_mtaddsetres.signal_kind = CMD_CPRID_MTADDSETRES + systype;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	/* 共有メモリの内容をプロセス間通信の後にアクセスしてはならない */
	printf( "[%d]%s end \nSignalkind: [%d] \nResult: [%d] \nSystem type: [%d] \n",
		__LINE__,__FUNCTION__,mtaddrrsp_p->cpridat_mtaddsetres.signal_kind,mtaddrrsp_p->cpridat_mtaddsetres.result,systype);
#endif

	l3snd_ret = m_cm_L3MsgSend_REC(mtaddrrsp_p);
	if(l3snd_ret != CMD_L3_OK)
	{
		cm_Assert(CMD_ASL_DBGLOW, l3snd_ret, "m_cm_L3MsgSend NG" );
	}
	return;
}

/* @} */


/* @} */

