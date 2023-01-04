/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReStaRep.c
 *  @brief  RE状態報告要求受信
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
#include "rrhApi_Svp.h"

/********************************************************************************************************************/
/**
 *  @brief  RE状態を集約して、RECに送信する。
 *  @note 
 *  @param  aparm_p [in] request pointer
 *  @return None
 *  @date 
 */
/********************************************************************************************************************/

VOID m_rc_ReStaRep(CMT_TSKIF_CPRIRCV_RESTSREQ* aparm_p)
{

	UINT                    ldwSigchk_rslt;								/*  result of checking signal */
	USHORT                  lwSigsys_rslt;								/*  system by checking signal */
	
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
	}
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s System type: %d\n",__LINE__,__FUNCTION__,lwSigsys_rslt);
#endif
	if(CMD_SYS_S3G == lwSigsys_rslt)
	{
		//NR independent mode or LTE + NR mode
		if (cmw_connectoperatemode == CMD_OP_LTE)
		{
			/* Send message between threads (l3/rct->re/rec) */
			(VOID)l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, aparm_p, sizeof(CMT_TSKIF_CPRIRCV_RESTSREQ));	
		}
		else
		{
			m_rc_ReStaLTESub();
		}
	}
	else
	{
		m_rc_ReSta3GSub();
	}
	

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	return;
}

/********************************************************************************************************************/
/**
 *  @brief  RE status report response
 *  @note 
 *  @param  aparm_p [in] request pointer
 *  @return None
 *  @date 2021/02/19 M&C)DuongMX Create
 */
/********************************************************************************************************************/
VOID m_rc_ReStaRes(CMT_TSKIF_CPRIRCV_RESTSRES* aparm_p)
{

	UINT                    ldwSigchk_rslt;								/*  result of checking signal */
	USHORT                  lwSigsys_rslt;								/*  system by checking signal */

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
	ldwSigchk_rslt = m_cm_chksig((aparm_p)->cpridat_restsres.signal_kind, &lwSigsys_rslt);

	if(ldwSigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW, lwSigsys_rslt, "m_rc_ReStaRes validate signal NG");
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s System type: %d\n",__LINE__,__FUNCTION__,lwSigsys_rslt);
#endif
	if(CMD_SYS_S3G == lwSigsys_rslt)
	{
		/************/
		/* 応答送信 */
		/************/
		(VOID)m_cm_L3MsgSend_REC(aparm_p);
	}
	else
	{
		cm_Assert(	CMD_ASL_USELOW, lwSigsys_rslt, "m_rc_ReStaRes 3G signal NG");
		return;
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	return;
}

/********************************************************************************************************************/
/**
 *  @brief  Synthesize RE state for LTE 
 *  @note 
 *  @param -   
 *  @return None
 *  @date	2016/03/10 TDI)satou 正常応答を受信していないREは集約対象外に
 */
/********************************************************************************************************************/

VOID m_rc_ReStaLTESub()
{
	T_RRH_REST_S3G               ltReStaArrLTE[D_RRH_CPRINO_RE_MAX];			/* Array Re state */
	T_RRH_FHM_STATE              ltFhmSta;										/* FHM state*/
	T_RRH_LAYER3                 ltReLayer3Stas3g;								/* Layer 3 state*/
	USHORT                       lwLoop;
	USHORT                       lwLoop1;
	USHORT                       lwFixMakerInfNoPosition;						/* The position of メーカ固有RE情報数n*/
	USHORT                       lwNGFlag = CMD_TRUE;							/* Its value is true so that response NG to REC*/
	USHORT                       lwNGGetShmChkFlag = CMD_FALSE;					/* Its value is true if have a NG shared memory get situation */
	USHORT                       lwDatalength;                  				/* データ長						*/
	CMT_TSKIF_CPRISND_RESTSRES*  rsp_p;											/* タスク間フレームポインタ		*/
	UINT                         bufget_rslt;									/* バッファ獲得結果				*/
	USHORT                       set_data_cnt     = CMD_NUM0;					/* 設定データカウンタ			*/
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
	/* 可変長設定用データ */
	USHORT                  set_data[CMD_NUM1  +	/* FAN情報数				*/
							CMD_NUM16          +	/* FAN情報					*/
							CMD_NUM1           +	/* メーカ固有RE情報数		*/
							CMD_NUM32];				/* メーカ固有RE情報			*/
	/*Get FHM state*/
	
	if( D_RRH_OK != f_cmn_com_fhm_state_get( &ltFhmSta ) )
	{
		cm_Assert(	CMD_ASL_USELOW,
					CMD_SYS_LTE, 
					"m_rc_ReStaLTESub Get FHM state from shared memory NG");
		lwNGGetShmChkFlag = CMD_TRUE;
	}
	
	if( CMD_FALSE == lwNGGetShmChkFlag )
	{
		/*Get all RE state information from shared memory*/
		for(lwLoop=0;lwLoop<D_RRH_CPRINO_RE_MAX;lwLoop++)
		{
			if( D_RRH_OK != f_cmn_com_rest_s3g_get( lwLoop + CMD_NUM1, &ltReStaArrLTE[lwLoop]  ) )
			{
				cm_Assert(	CMD_ASL_USELOW,
					CMD_SYS_LTE, 
					"m_rc_ReStaLTESub Get RE state from shared memory NG");
				lwNGGetShmChkFlag = CMD_TRUE;
				break;
			}
		}
	}
	
	/*Get Re's layer3 state */
	if( CMD_FALSE == lwNGGetShmChkFlag )
	{
		if( D_RRH_OK != f_cmn_com_layer3_get( CMD_SYS_S3G, &ltReLayer3Stas3g ))
		{
			cm_Assert(	CMD_ASL_USELOW,
					CMD_SYS_LTE, 
					"m_rc_ReStaLTESub Get layer3 state from shared memory NG");
			lwNGGetShmChkFlag = CMD_TRUE;
		}
	}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s ltReLayer3Stas3g.layer3_re[CMD_NUM0] = %d\n",__LINE__,__FUNCTION__, ltReLayer3Stas3g.layer3_re[CMD_NUM0]);
#endif

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s ltReLayer3Stas3g.layer3_re[CMD_NUM2] = %d\n",__LINE__,__FUNCTION__, ltReLayer3Stas3g.layer3_re[CMD_NUM2]);
#endif
	/* 管理テーブルでループ */
	/* 全配下REにてNG応答を受信、または 一度も全配下REからOK応答を受信していない。 */
	if( CMD_FALSE == lwNGGetShmChkFlag )
	{
		for( lwLoop=CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++)
		{
			/* Layer3状態が「②RE起動中状態」、または「④運用中状態」の配下REだけが対象処理場合 */
			if( (CMD_NUM2 == ltReLayer3Stas3g.layer3_re[lwLoop]) || (CMD_NUM4 == ltReLayer3Stas3g.layer3_re[lwLoop]) )
			{
				if(ltReStaArrLTE[lwLoop].signal_kind != CMD_NUM0)
				{
					if(CMD_NML == ltReStaArrLTE[lwLoop].result)
					{
						lwNGFlag = CMD_FALSE;
						break;
					}
				}
			}
		}
	}
	
	/*make message*/
	/* ポインタ初期化 */
	rsp_p = NULL;

	/* 可変長設定用データ	*/
	cm_MemClr(set_data, sizeof(set_data));

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 取りうる最大のデータ長でバッファを獲得する */
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_RESTSRES)	+	/* メーカ名まで				*/
								sizeof(USHORT) * sizeof(set_data)	,	/* FAN情報数			+	*/
																		/* FAN情報				+	*/
																		/* メーカ固有RE情報数	+	*/
																		/* メーカ固有RE情報			*/
								CMD_NUM1,
								(VOID **)&rsp_p	);
	/********************/
	/* バッファ獲得失敗 */
	/********************/
	if(bufget_rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					bufget_rslt,
					"m_rc_ReStaLTESub cm_L2BfGet NG");
		

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		return;
	}
	
	/* If have not any OK response or all response is NG*/
	/* Or get shared memory NG*/
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s lwNGFlag = %d, lwNGGetShmChkFlag = %d\n",__LINE__,__FUNCTION__, lwNGFlag, lwNGGetShmChkFlag);
#endif
	if( (CMD_TRUE == lwNGFlag) || ( CMD_TRUE == lwNGGetShmChkFlag) )
	{
		/* (LTE)(0x5321)場合 */
		/* RE状態報告応答を送信する(l3/rct->l2/lpb)(51:処理NG(RE)) */
		/*make message*/
		/* CPRI信号応答結果にNGを設定 */
		/* もし、対RECの立ち上げシーケンス未完了の場合、OKを設定 2015/11/25*/
		if( rec_Startup_First_flg[CMD_SYS_S3G] == CMD_NUM0 )
		{
			rsp_p->cpridat_restsres.result = CMD_NML;
		}
		else
		{ 
			rsp_p->cpridat_restsres.result = CMD_HDL_NG;
		}
	
		/* CPRI信号種別設定 */
		rsp_p->cpridat_restsres.signal_kind = CMD_CPRID_RESTSRES + CMD_SYS_S3G;
	
		/* メーカ名(不定値) */
		rsp_p->cpridat_restsres.rests_inf.maker_name = D_RRH_IND_MAKER_NAME;
		
		/* タスク間ヘッダ作成 */
		
		/**********************************/
		/* 応答データに可変長データを設定 */
		/**********************************/
		/* 異常ルートは可変長の長さを表すパラメータに0を設定	*/
		cm_MemClr(&(rsp_p->cpridat_restsres.rests_inf.maker_name) + CMD_NUM1, 
			  		sizeof(USHORT) * CMD_NUM2);														/* FAN情報数+メーカ固有RE情報数	*/
		
		rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + (USHORT)(	sizeof(CMT_CPRISIGSND_INF)		/* CPRI信号送信情報部			*/
											+	sizeof(CMT_CPRIF_RESTSRES)							/* 信号種別+応答結果+メーカ名	*/
											+	(sizeof(USHORT) * CMD_NUM2));						/* FAN情報数+メーカ固有RE情報数	*/
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end\n result = %d, \nrsp_p->cpridat_restsres.signal_kind = %x\n",__LINE__,__FUNCTION__, rsp_p->cpridat_restsres.result,
																				rsp_p->cpridat_restsres.signal_kind);
#endif
		m_cm_L3MsgSend_REC(rsp_p);

		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		return ;
	}

	/* 応答結果: 0 */
	/* Situation of normal response :  */

	/* RECへの状態報告メッセージを集約する。 */
	rsp_p->cpridat_restsres.result = CMD_NML;
	/* FAN情報を集約する */
	
	/* 管理テーブルでループ */
	/* FHM状態の場合 */
	/* 全てのFHMFAN情報?をRECへの送信済みメッセージにコーピーする */

	for(lwLoop1=CMD_NUM0; lwLoop1<ltFhmSta.fan_num; lwLoop1++)
	{
		set_data_cnt++;

		set_data[set_data_cnt]  = ltFhmSta.fan_inf[lwLoop1];
		/* nRECへのメッセージのREC_FAN情報数#nを管理テーブルのFHM状態でFAN情報数nに増やす。*/
		/*Increment*/
		set_data[CMD_NUM0] += CMD_NUM1;//ltFhmSta.fan_num;
	}

	/*With state information of RE*/	
	/* 状態管理テーブルの残っているRE状態要素でループ */
	for( lwLoop=CMD_NUM0; lwLoop<D_RRH_CPRINO_RE_MAX;lwLoop++)
	{
		/* Layer3状態が「②RE起動中状態」、または「④運用中状態」の配下REだけが対象処理場合 */
		if( (CMD_NUM2 == ltReLayer3Stas3g.layer3_re[lwLoop]) || (CMD_NUM4 == ltReLayer3Stas3g.layer3_re[lwLoop]) )
		{	/* 正常応答受信していないREは集約対象外 */
			if ((0 == ltReStaArrLTE[lwLoop].signal_kind) || (CMD_NML != ltReStaArrLTE[lwLoop].result))
			{
				continue;
			}

			/* REC_FAN情報数#n =16場合 */
			if(CMD_NUM16 == set_data[CMD_NUM0])
			{
				/* FAN更新停止 */
				break;
			}

			/* REC_FAN情報数#n <=16、かつコーピーするREFAN情報がまだ残っている間 */
			for( lwLoop1=CMD_NUM0; lwLoop1<ltReStaArrLTE[lwLoop].fan_num ; lwLoop1++)
			{
				/* REC_FAN情報数#n =16場合 */
				if( CMD_NUM16 == set_data[CMD_NUM0] )
				{
					break;
				}
				/* 一つのREFAN情報をREへの送信済みメッセージをコーピーする */
				set_data_cnt++;
				set_data[set_data_cnt] = ltReStaArrLTE[lwLoop].fan_inf[lwLoop1];
				/* REC_FAN情報数#n  をインクリメント(of message which is sent to REC)  */
				set_data[CMD_NUM0] += CMD_NUM1;
			}
		}
	}
	/*get the position of メーカ固有RE情報数n*/
	set_data_cnt++;
	lwFixMakerInfNoPosition = set_data_cnt;
	
	/*  メーカ固有RE情報数情報を集約する: */

	/* FHM_メーカ固有をRECへの送信メッセージにコーピーする */
	
	for(lwLoop1= CMD_NUM0;lwLoop1<ltFhmSta.maker_inf_num;lwLoop1++)
	{
		//*(&(rsp_p->cpridat_restsres.rests_inf.maker_name + lwLoop1 + CMD_NUM1)) += 
		set_data_cnt++;
		set_data[set_data_cnt]  = ltFhmSta.maker_inf[lwLoop1];
		
		/* RECへのメッセージのREC_メーカ固有RE情報数nを管理テーブルのFHM状態でメーカ固有RE情報数nに増やす。 */
		/*Increment*/
	    set_data[lwFixMakerInfNoPosition] += CMD_NUM1;
	}
		
	/* With state information of RE*/	
	/* 状態管理テーブルの残っているRE状態要素でループ */
	for( lwLoop=CMD_NUM0; lwLoop<D_RRH_CPRINO_RE_MAX; lwLoop++)
	{
		/* Layer3状態が「②RE起動中状態」、または「④運用中状態」の配下REだけが対象処理場合 */
		if( (CMD_NUM2 == ltReLayer3Stas3g.layer3_re[lwLoop]) || (CMD_NUM4 == ltReLayer3Stas3g.layer3_re[lwLoop]) )
		{
			/* REC_メーカ固有RE情報数n =32場合 */
			if(CMD_NUM32 == set_data[lwFixMakerInfNoPosition])
			{
				/* メーカ固有RE情報更新停止 */
				break;
			}
			/* REC_メーカ固有RE情報数n <32場合 */
			/* REC_メーカ固有RE情報数n <=32、かつコーピーするメーカ固有RE情報がまだ残っている間 */
			for( lwLoop1=CMD_NUM0; lwLoop1<ltReStaArrLTE[lwLoop].maker_inf_num ; lwLoop1++ )
			{
				if( CMD_NUM32 == set_data[lwFixMakerInfNoPosition] )
				{
					break;
				}
			    /* 一つのメーカ固有RE情報をREへの送信済みメッセージをコーピーする */
				set_data_cnt++;
				set_data[set_data_cnt] = ltReStaArrLTE[lwLoop].maker_inf[lwLoop1];
				/* REC_メーカ固有RE情報数n  をインクリメント(of message which is sent to REC)  */
				/*Increment*/
				set_data[lwFixMakerInfNoPosition] += CMD_NUM1;
			}
		}
	}

	/* CPRI信号応答結果にOKを設定 */
	rsp_p->cpridat_restsres.result = CMD_NML;

	/* CPRI信号種別設定 */
	rsp_p->cpridat_restsres.signal_kind = CMD_CPRID_RESTSRES + CMD_SYS_S3G;
			/* メーカ名(不定値) */
	rsp_p->cpridat_restsres.rests_inf.maker_name = D_RRH_IND_MAKER_NAME;
		
	/************************************************/
	/* 可変長設定用データに保持したデータを詰める	*/
	/************************************************/
	cm_MemCpy(&(rsp_p->cpridat_restsres.rests_inf.maker_name) + CMD_NUM1, 
		  set_data, 
		  sizeof(USHORT) * (set_data_cnt + CMD_NUM1));
	/****************/
	/* データ長設定 */
	/****************/
	lwDatalength = (USHORT)(	sizeof(CMT_CPRISIGSND_INF)
					+	sizeof(CMT_CPRIF_RESTSRES)	/* 信号種別+応答結果+メーカ名	*/
					+	((set_data_cnt + CMD_NUM1)* CMD_NUM2));	/* FAN情報数			+		*/
													/* FAN情報				+		*/
													/* メーカ固有RE情報数	+		*/
													/* メーカ固有RE情報				*/
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s Signal kind: %x, result: %d\n",__LINE__,__FUNCTION__, rsp_p->cpridat_restsres.signal_kind, rsp_p->cpridat_restsres.result);
#endif
	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + lwDatalength;
	/*  RE状態報告応答 to RECを送信する(l3/rct->l2/lpb) */
	m_cm_L3MsgSend_REC(rsp_p);


#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	/* 終了 */
	return;
}

/********************************************************************************************************************/
/**
 *  @brief  Synthesize RE state for 3G 
 *  @note 
 *  @param -   
 *  @return None
 *  @date	2016/03/10 TDI)satou 正常応答を受信していないREは集約対象外に
 */
/********************************************************************************************************************/

VOID m_rc_ReSta3GSub()
{
	// T_RRH_REST_3G                 ltReStaArr3G[D_RRH_CPRINO_RE_MAX];			/* Array Re state */
	// T_RRH_FHM_STATE               ltFhmSta;										/* FHM state*/
	// // T_RRH_LAYER3                  ltReLayer3Sta3g;								/* Layer 3 state*/
	// USHORT                        lwLoop;
	// USHORT                        lwLoop1;
	// USHORT                        lwFixMakerInfNoPosition;						/* The position of メーカ固有RE情報数n*/
	// USHORT                        lwNGFlag = CMD_TRUE;							/* Its value is true so that response NG to REC*/
	// USHORT                        lwNGGetShmChkFlag = CMD_FALSE;				/* Its value is true if have a NG shared memory get situation */
	USHORT                        lwDatalength;									/* データ長						*/
	CMT_TSKIF_CPRISND_RESTSRES*   rsp_p;										/* タスク間フレームポインタ		*/
	UINT                          bufget_rslt;									/* バッファ獲得結果				*/
	USHORT                        set_data_cnt     = CMD_NUM0;					/* 設定データカウンタ			*/
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s start\n",__LINE__,__FUNCTION__);
#endif
	
	/* 可変長設定用データ */
	USHORT                  set_data[CMD_NUM1  +	/* FAN情報数					*/
							CMD_NUM16          +	/* FAN情報						*/
							CMD_NUM1           +	/* メーカ固有RE情報数			*/
							CMD_NUM32];				/* メーカ固有RE情報				*/
// 	/*Get FHM state*/
// 	if( D_RRH_OK != f_cmn_com_fhm_state_get( &ltFhmSta ) )
// 	{
// 		/* Assert */
// 		cm_Assert(	CMD_ASL_USELOW,
// 					CMD_SYS_3G, 
// 					"m_rc_ReSta3GSub Get FHM state from shared memory NG");
// 		lwNGGetShmChkFlag = CMD_TRUE;
// 	}
	
// 	/*Get all RE state information from shared memory*/
// 	if( CMD_FALSE == lwNGGetShmChkFlag )
// 	{
// 		for( lwLoop = CMD_NUM0; lwLoop<D_RRH_CPRINO_RE_MAX; lwLoop++ )
// 		{
// 			if( D_RRH_OK != f_cmn_com_rest_3g_get( lwLoop + CMD_NUM1, &ltReStaArr3G[lwLoop] ))
// 			{
// 				/* Assert */
// 				cm_Assert(	CMD_ASL_USELOW,
// 							CMD_SYS_3G, 
// 							"m_rc_ReSta3GSub Get RE state from shared memory NG");
// 				lwNGGetShmChkFlag = CMD_TRUE;
// 				break;
// 			}
// 		}
// 	}

// 	/*Get Re's layer3 state */
// 	if( CMD_FALSE == lwNGGetShmChkFlag )
// 	{
// 		if( D_RRH_OK != f_cmn_com_layer3_get( CMD_SYS_3G, &ltReLayer3Sta3g ) )
// 		{
// 			/* Assert */
// 			cm_Assert(	CMD_ASL_USELOW,
// 						CMD_SYS_3G, 
// 						"m_rc_ReSta3GSub Get layer3 state from shared memory NG");
// 			lwNGGetShmChkFlag = CMD_TRUE;
// 		}
// 	}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s ltReLayer3Sta3g.layer3_re[CMD_NUM0] = %d\n",__LINE__,__FUNCTION__, ltReLayer3Sta3g.layer3_re[CMD_NUM0]);
// #endif

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s ltReLayer3Sta3g.layer3_re[CMD_NUM2] = %d\n",__LINE__,__FUNCTION__, ltReLayer3Sta3g.layer3_re[CMD_NUM2]);
// #endif
	
// 	/* 管理テーブルでループ */
// 	/* 全配下REにてNG応答を受信、または 一度も全配下REからOK応答を受信していない。 */
// 	if( CMD_FALSE == lwNGGetShmChkFlag )
// 	{
// 		for( lwLoop=CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX; lwLoop++)
// 		{
// 			/* Layer3状態が「②RE起動中状態」、または「④運用中状態」の配下REだけが対象処理場合 */
// 			if( (CMD_NUM2 == ltReLayer3Sta3g.layer3_re[lwLoop]) || (CMD_NUM4 == ltReLayer3Sta3g.layer3_re[lwLoop]) )
// 			{
// 				if(ltReStaArr3G[lwLoop].signal_kind != CMD_NUM0)
// 				{
// 					if(CMD_NML == ltReStaArr3G[lwLoop].result)
// 					{
// 						lwNGFlag = CMD_FALSE;
// 						break;
// 					}
// 				}
// 			}
// 		}
// 	}
	
	/*make message*/
	/* ポインタ初期化 */
	rsp_p = NULL;
	
	/* 可変長設定用データ	*/
	cm_MemClr(set_data, sizeof(set_data));
	
	/****************/
	/* バッファ獲得 */
	/****************/
	/* 取りうる最大のデータ長でバッファを獲得する */
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
							sizeof(CMT_TSKIF_CPRISND_RESTSRES)	+	/* メーカ名まで				*/
							sizeof(USHORT) * sizeof(set_data)	,	/* FAN情報数			+	*/
																	/* FAN情報				+	*/
																	/* メーカ固有RE情報数	+	*/
																	/* メーカ固有RE情報			*/
							CMD_NUM1,
							(VOID **)&rsp_p	);
	/********************/
	/* バッファ獲得失敗 */
	/********************/
	if(bufget_rslt != CMD_RES_OK)
	{
			/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
				bufget_rslt,
				"m_rc_ReSta3GSub cm_L2BfGet NG");
		

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
		
		return;
	}
	
// 	/*If have not any OK response or all response is NG*/
// 	if( (CMD_TRUE == lwNGFlag) || CMD_TRUE == lwNGGetShmChkFlag )
// 	{
// 		/* (3G)(0x5320)場合 */
// 		/* RE状態報告応答を送信する(l3/rct->l2/lpb)(2:処理NG(RE)) */
// 		/*make message*/
// 		/* CPRI信号応答結果にNGを設定 */
// 		/* もし、対RECの立ち上げシーケンス未完了の場合、OKを設定 2015/11/25*/
// 		if( rec_Startup_First_flg[CMD_SYS_3G] == CMD_NUM0 )
// 		{
// 			rsp_p->cpridat_restsres.result = CMD_NML;
// 		}
// 		else
// 		{
// 			rsp_p->cpridat_restsres.result = CMD_RPT_NG;
// 		}

// 		/* CPRI信号種別設定 */
// 		rsp_p->cpridat_restsres.signal_kind = CMD_CPRID_RESTSRES;

// 		/* メーカ名(不定値) */
// 		rsp_p->cpridat_restsres.rests_inf.maker_name = D_RRH_IND_MAKER_NAME;

// 		/* タスク間ヘッダ作成 */

// 		/**********************************/
// 		/* 応答データに可変長データを設定 */
// 		/**********************************/
// 		/* 異常ルートは可変長の長さを表すパラメータに0を設定	*/
// 		cm_MemClr(&(rsp_p->cpridat_restsres.rests_inf.maker_name) + CMD_NUM1, 
// 		  	sizeof(USHORT) * CMD_NUM2);										/* FAN情報数+メーカ固有RE情報数	*/

// 		rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + (USHORT)(	sizeof(CMT_CPRISIGSND_INF)		/* CPRI信号送信情報部			*/
// 										+	sizeof(CMT_CPRIF_RESTSRES)		/* 信号種別+応答結果+メーカ名	*/
// 										+	(sizeof(USHORT) * CMD_NUM2));	/* FAN情報数+メーカ固有RE情報数	*/
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "[%d]%s end\n result = %d, \nrsp_p->cpridat_restsres.signal_kind = %x\n",__LINE__,__FUNCTION__, rsp_p->cpridat_restsres.result,
// 																				rsp_p->cpridat_restsres.signal_kind);
// #endif
// 		m_cm_L3MsgSend_REC(rsp_p);
		

// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
// #endif
		
// 		return ;
// 	}

	/* 応答結果: 0 */
	/* Situation of normal response :  */

	// /* RECへの状態報告メッセージを集約する。 */
	// rsp_p->cpridat_restsres.result = CMD_NML;
	
	// /* FAN情報を集約する */
	
	// /* 管理テーブルでループ */
	// /* FHM状態の場合 */
	// /* 全てのFHMFAN情報?をRECへの送信済みメッセージにコーピーする */
	// for(lwLoop1=CMD_NUM0; lwLoop1<ltFhmSta.fan_num; lwLoop1++)
	// {
	// 	set_data_cnt++;
	// 	set_data[set_data_cnt]  = ltFhmSta.fan_inf[lwLoop1];
	// 	/* nRECへのメッセージのREC_FAN情報数#nを管理テーブルのFHM状態でFAN情報数nに増やす。*/
	// 	/*Increment*/
	// 	set_data[CMD_NUM0] += CMD_NUM1;
	// }

	// /*With state information of RE*/	
	// /* 状態管理テーブルの残っているRE状態要素でループ */
	// for( lwLoop=CMD_NUM0; lwLoop<D_RRH_CPRINO_RE_MAX;lwLoop++)
	// {	/* 正常応答を受信していないREは集約対象外 */
	// 	if ((0 == ltReStaArr3G[lwLoop].signal_kind) || (CMD_NML != ltReStaArr3G[lwLoop].result))
	// 	{
	// 		continue;
	// 	}

	// 	/* Layer3状態が「②RE起動中状態」、または「④運用中状態」の配下REだけが対象処理場合 */
	// 	if( (CMD_NUM2 == ltReLayer3Sta3g.layer3_re[lwLoop]) || (CMD_NUM4 == ltReLayer3Sta3g.layer3_re[lwLoop]) )
	// 	{
	// 		/* REC_FAN情報数#n =16場合 */
	// 		if(CMD_NUM16 == set_data[CMD_NUM0])
	// 		{
	// 			/* FAN更新停止 */
	// 			break;
	// 		}

	// 		/* REC_FAN情報数#n <16場合 */
	// 		/* REC_FAN情報数#n <=16、かつコーピーするREFAN情報がまだ残っている間 */
	// 		for( lwLoop1=CMD_NUM0; lwLoop1<ltReStaArr3G[lwLoop].fan_num ; lwLoop1++)
	// 		{
	// 			if(CMD_NUM16 == set_data[CMD_NUM0])
	// 			{
	// 				break;
	// 			}
	// 			/* 一つのREFAN情報をREへの送信済みメッセージをコーピーする */
	// 			set_data_cnt++;

	// 			set_data[set_data_cnt] = ltReStaArr3G[lwLoop].fan_inf[lwLoop1];
	// 			/* REC_FAN情報数#n  をインクリメント(of message which is sent to REC)  */
	// 			set_data[CMD_NUM0] += CMD_NUM1;
	// 		}
	// 	}
	// }

	// /*get the position of メーカ固有RE情報数n*/
	// set_data_cnt++;
	// lwFixMakerInfNoPosition = set_data_cnt;
	// /*  メーカ固有RE情報数情報を集約する: */

	// /* FHM_メーカ固有をRECへの送信メッセージにコーピーする */
	// for(lwLoop1= CMD_NUM0;lwLoop1<ltFhmSta.maker_inf_num;lwLoop1++)
	// {
	// 	set_data_cnt++;
	// 	set_data[set_data_cnt]  = ltFhmSta.maker_inf[lwLoop1];
	// 	/* RECへのメッセージのREC_メーカ固有RE情報数nを管理テーブルのFHM状態でメーカ固有RE情報数nに増やす。 */
	// 	/*Increment*/
	//     set_data[lwFixMakerInfNoPosition] += CMD_NUM1;
	// }

	// /*With state information of RE*/	
	// /* 状態管理テーブルの残っているRE状態要素でループ */
	// for( lwLoop=CMD_NUM0; lwLoop<D_RRH_CPRINO_RE_MAX; lwLoop++)
	// {
	// 	/* Layer3状態が「②RE起動中状態」、または「④運用中状態」の配下REだけが対象処理場合 */
	// 	if( (CMD_NUM2 == ltReLayer3Sta3g.layer3_re[lwLoop]) || (CMD_NUM4 == ltReLayer3Sta3g.layer3_re[lwLoop]) )
	// 	{
	// 		/* REC_メーカ固有RE情報数n =32場合 */
	// 		if(CMD_NUM32 == set_data[lwFixMakerInfNoPosition])
	// 		{
	// 			/* メーカ固有RE情報更新停止 */
	// 			break;
	// 		}

	// 		/* REC_メーカ固有RE情報数n <32場合 */
	// 		/* REC_メーカ固有RE情報数n <=32、かつコーピーするメーカ固有RE情報がまだ残っている間 */
	// 		for( lwLoop1=CMD_NUM0; lwLoop1<ltReStaArr3G[lwLoop].maker_inf_num ; lwLoop1++ )
	// 		{
	// 			if(CMD_NUM32 == set_data[lwFixMakerInfNoPosition])
	// 			{
	// 				break;
	// 			}
	// 		    /* 一つのメーカ固有RE情報をREへの送信済みメッセージをコーピーする */
	// 			set_data_cnt++;
	// 			set_data[set_data_cnt] = ltReStaArr3G[lwLoop].maker_inf[lwLoop1];
	// 			/* REC_メーカ固有RE情報数n  をインクリメント(of message which is sent to REC)  */
	// 			/*Increment*/
	// 			set_data[lwFixMakerInfNoPosition] += CMD_NUM1;
	// 		}
	// 	}
	// }

	/* CPRI信号応答結果にOKを設定 */
	rsp_p->cpridat_restsres.result = CMD_RPT_NG;

	/* CPRI信号種別設定 */
	rsp_p->cpridat_restsres.signal_kind = CMD_CPRID_RESTSRES;
	/* メーカ名(不定値) */
	rsp_p->cpridat_restsres.rests_inf.maker_name = D_RRH_IND_MAKER_NAME;

	/************************************************/
	/* 可変長設定用データに保持したデータを詰める	*/
	/************************************************/
	cm_MemCpy(&(rsp_p->cpridat_restsres.rests_inf.maker_name) + CMD_NUM1, 
		  set_data, 
		  sizeof(USHORT) * (set_data_cnt + CMD_NUM1));

	/****************/
	/* データ長設定 */
	/****************/
	lwDatalength = (USHORT)(	sizeof(CMT_CPRISIGSND_INF)
					+	sizeof(CMT_CPRIF_RESTSRES)				/* 信号種別+応答結果+メーカ名	*/
					+	((set_data_cnt + CMD_NUM1)* CMD_NUM2));	/* FAN情報数			+		*/
																/* FAN情報				+		*/
																/* メーカ固有RE情報数	+		*/
																/* メーカ固有RE情報				*/
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s Signal kind: %x, result: %d\n",__LINE__,__FUNCTION__, rsp_p->cpridat_restsres.signal_kind, rsp_p->cpridat_restsres.result);
#endif
	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + lwDatalength;
	/*  RE状態報告応答 to RECを送信する(l3/rct->l2/lpb) */
	m_cm_L3MsgSend_REC(rsp_p);


#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "\n[%d]%s end\n",__LINE__,__FUNCTION__);
#endif
	
	/* 終了 */
	return;
}
/** @} */
