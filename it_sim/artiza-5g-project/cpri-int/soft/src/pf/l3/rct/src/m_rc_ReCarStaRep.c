/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReCarStaRep.c
 *  @brief  response to RE carrier status report request, carry out RE carrier status report process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2009/08/05 FFCS)wangjuan M-S3G-eNBPF-02334 TRA no need stop wave and report carrier alarm when 3G ERROR control happen
 *  @date   2009/08/08 FFCS)Wangjuan CR-00058-005(CPRI Spec V1.0.9) 
 *  @date   2009/12/27 QNET)Kabasima CR-00081-001:[DCM向け]REハードソフトインタフェース仕様書v2.13版リリース
 *  @date   2010/01/12 QNET)Kabasima M-S3G-eNBPF-03094:[IT3](3017)キャリア状態監視開始マクロにて期待外動作
 *  @date   2010/02/19 QNET)Kabasima CR-xxxxx-xxx [DCM向け]TS-145-ALM制御時のRE動作の件
 *  @date   2010/03/11 QNET)Kabasima M-S3G-eNBPF-03249:両系ブランチALM時にREキャリア状態報告(3G)のキャリアALMが設定されない場合がある
 *  @date   2012/07/02 FFCS)Xut CeNB-F-086-011
 *  @date   2012/10/19 FFCS)Xut Modify for CR-00070  
 *  @date   2021/01/18 M&C)Tri.hn Update based on No.18 (sequence 3-27)
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"

/********************************************************************************************************************/
/* prototype																										*/
/********************************************************************************************************************/
static VOID m_rc_MakeCarDat_OnlyFHM_S3G(USHORT carIdx, T_RRH_CARST_S3G_CAR_EVERY *carEvery);
static VOID m_rc_MakeCarDat_OnlyFHM_S3G_SetDisable(T_RRH_CARST_S3G_CAR_EVERY *carEvery);
static VOID m_rc_MakeCarDat_OnlyFHM_S3G_SetEnable(USHORT carNo, USHORT wSend, T_RRH_CARST_S3G_CAR_EVERY *carEvery);
static INT m_rc_MakeCarDat_OnlyFHM_3G(USHORT carIdx, T_RRH_CARST_3G_CAR_EVERY *carEvery);
static VOID m_rc_MakeCarDat_OnlyFHM_3G_SetDisable(T_RRH_CARST_3G_CAR_EVERY *carEvery);
static VOID m_rc_MakeCarDat_OnlyFHM_3G_SetEnable(USHORT carNo, T_RRH_CARST_3G_CAR_EVERY *carEvery);

/********************************************************************************************************************/
/**
 *  @brief    配下REリンク断チェック処理
 *  @note     配下REが使用中かつL3_layer状態が運用中以外のリンクが存在するかを判断する
 *  @param    system_type	[in]	system type
 *  @param    lnkErrlfg		[out]	link error flag (1=have link of error, 0 not have link of error)
 *  @return   None
 *  @retval   -
 *  @date     2016/01/07 FJT)koshida create
 *  @date     2016/04/05 TDI)satou Layer3状態が起動中状態の場合、リンク断と判定しないように
 */
/********************************************************************************************************************/
VOID m_rc_chk_l3layer(USHORT system_type, USHORT* lnkErrlfg)
{
    T_RRH_CONN_DISC shmConnDisc;
	T_RRH_LAYER3	ltLayer3;
	USHORT			lwReCnt;

	/*	OUTPUT引数がNULLポインタの場合	*/
	if(lnkErrlfg == NULL)
	{
		cm_Assert(	CMD_ASL_USELOW,	(UINT)lnkErrlfg, "lnkErrlfg is NULL pointer NG");
		return;
	}
	/*	INPUT引数が異常値の場合	*/
	if(system_type > CMD_SYS_S3G)
	{
		cm_Assert(	CMD_ASL_USELOW,	system_type, "system_type is NG");
		return;
	}
	*lnkErrlfg = CMD_OFF;

    f_cmn_com_conn_disk_get(&shmConnDisc);
	f_cmn_com_layer3_get(system_type, &ltLayer3);
	/*	配下RE数分LOOPする	*/
	for(lwReCnt=0; lwReCnt<D_RRH_CPRINO_RE_MAX; lwReCnt++)
	{
		/* 配下REが使用中 かつ Layer3状態が運用中若しくは起動中の場合 */
		if (D_RRH_OFF == shmConnDisc.conn_disc[lwReCnt])
		{
			continue;
		}

		if((E_RRH_LAYER3_RE_OPE == ltLayer3.layer3_re[lwReCnt]) || (E_RRH_LAYER3_RE_START == ltLayer3.layer3_re[lwReCnt]))
		{
			continue;
		}

		*lnkErrlfg = CMD_ON;
		break;
	}
	return;
}
/********************************************************************************************************************/
/**
 *  @brief    REキャリア状態報告
 *  @note     REキャリア状態報告
 *  @param    parm_p	[in]	parameter pointer
 *  @return   None
 *  @retval   -
 *  @date     2015/10/08 FPT)Yen create
 *  @date     2021/01/18 M&C)Tri.hn Update based on No.18 (sequence 3-27) - Add checking LTE-independent mode vs. LTE+NR mode
 */
/********************************************************************************************************************/
VOID m_rc_ReCarStaRep(VOID* parm_p)
{
	UINT		sigchk_rslt;					/*  result of checking signal */
	USHORT		sigsys_rslt;					/*  system by checking signal */
	T_RRH_LAYER3	layer3Sts;
	UINT			lwCnt;
	
	/*In case buffer is Null*/
	if( NULL == parm_p ){
		/*end processing*/
		return;
	}

	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind,&sigsys_rslt);
	if(sigchk_rslt!= CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt, 
							"validate signal NG");
	}
	
	if( CMD_SYS_3G == sigsys_rslt )
	{
		m_rc_ReCarStaRep_3G(parm_p);
	}
	else
	{
		/* If LTE-independent mode, forward message to re/rec */
		cmw_connectoperatemode = CMD_OP_LTE;	// Tri.hn FIXME Stub data
		if (cmw_connectoperatemode == CMD_OP_LTE)
		{
			f_cmn_com_layer3_get(CMD_SYS_S3G, &layer3Sts);
			/*  CPRIリンク数だけループループ */
			for(lwCnt = 0; lwCnt < D_RRH_CPRINO_RE_MAX; lwCnt++)
			{
				if( E_RRH_LAYER3_RE_OPE == layer3Sts.layer3_re[lwCnt] || E_RRH_LAYER3_RE_START == layer3Sts.layer3_re[lwCnt])
				{
					((CMT_TSKIF_CPRISND_CARLSTSREQ*)parm_p)->cprisnd_inf.link_num = lwCnt + CMD_NUM1;
					/* Send message between threads (l3/rct->re/rec) */
					l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0, parm_p, sizeof(CMT_TSKIF_CPRISND_CARLSTSREQ));	
				}
			}
		}
		/* Else (LTE+NR mode), collect subordinate RE information,
		   retain it in the intermediate device,
		   and send response to REC */ 
		else
		{
			m_rc_ReCarStaRep_S3G(parm_p);
		}
	}

	return;
}

/********************************************************************************************************************/
/**
 *  @brief    RE carrier status report process for S3G
 *  @note     RE carrier status report process for S3G
 *  @param    parm_p	[in]	parameter pointer
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2009/08/08 FFCS)Wangjuan modify for CR-00058-005(CPRI Spec V1.0.9) 
 *  @date     2010/01/12 QNET)Kabasima M-S3G-eNBPF-03094:[IT3](3017)キャリア状態監視開始マクロにて期待外動作
 *  @date     2011/11/02 COMAS)Uchida 11B-01-005：CeNB-F-079-008(DCM)(F-1326(DCM)：キャリア状態に関する動作について)
 *  @date     2012/07/02 FFCS)Xut CeNB-F-086-011
 *  @date     2012/10/19 FFCS)Xut Modify for CR-00070 
 *  @date     2015/10/08 FPT)Yen update(REから受信したREキャリア状態報告を集約する)
 *  @date     2015/10/29 FPT)Yen fix bug IT3 No39
 *  @date     2015/10/29 TDI)satou FHM技説102-003
 *  @date     2015/11/18 FPT)Yen fix bug IT3 No89
 *  @date     2016/02/26 tdips)enoki FHM技説1.0.5仕様変更
 *  @date     2016/03/25 TDI)satou M-RRU-ZSYS-02091 FHMにTRX設定があり、REからキャリアなしが報告された場合に、固定値を報告するように
 *  @date     2017/08/16 FJT)ohashi 3201-17A-TS69 仕様変更(S3C運用状態移行タイミング変更)
 */
/********************************************************************************************************************/
VOID m_rc_ReCarStaRep_S3G(VOID* parm_p)
{
	CMT_TSKIF_CPRISND_CARLSTSRES 	*rsp_p;								/* タスク間フレームポインタ		*/
	UINT   							bufget_rslt;						/* バッファ獲得結果				*/
	USHORT							setdata[(sizeof(T_RRH_CARST_S3G) - sizeof(USHORT) * CMD_NUM4 )];
	USHORT 							set_data_cnt = CMD_NUM0;			/* データ長						*/
	USHORT							lwReCnt;
	USHORT							lwCarrCnt;
	USHORT							lwCarrNo;
	USHORT							lwBrCnt;
	USHORT							lwBrNo;
	USHORT							lwBrNumIndex;
	T_RRH_CARST_S3G					ltCarrSts;
	T_RRH_CARST_S3G					ltCarrStsTotal;
	USHORT							lwBr0Idx=0xFF;
	USHORT							lwBr1Idx=0xFF;
	USHORT							lwCarrStsDL;
	USHORT							RecSeqComplete=D_RRH_OFF;
	UINT							rec_startup_cmp = D_RRH_OFF;
	INT								linkno;
	USHORT							cpr_idx;
	T_RRH_LAYER3 					layer3;

//16B スロット数制限機能追加 start
	T_RRH_LAYER3 shmLayer3[CMD_MAX_SYSNUM];
//16B スロット数制限機能追加 end
	UINT	cnt1;
	UINT	cnt2;

	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , start\n ", __LINE__, __FUNCTION__);
#endif	
	if(cmw_filedatsnd_flag !=0)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "carr state req Start:flag=%d", cmw_filedatsnd_flag);
	}

	cm_MemClr(setdata, sizeof(setdata));
	cm_MemClr(&ltCarrStsTotal, sizeof(ltCarrStsTotal));
	for( lwCarrCnt = CMD_NUM0; lwCarrCnt < CMD_MAX_CARNUM_S3G; lwCarrCnt++ )
	{
		ltCarrStsTotal.car_every[lwCarrCnt].car_num = 0xFFFF;
		for( lwBrCnt = CMD_NUM0; lwBrCnt < 8; lwBrCnt++ )
		{
			ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].br_num = 0xFFFF;
		}
	}

	/* ポインタ初期化 */
	rsp_p = NULL;

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 取りうる最大のデータ長でバッファを獲得する */
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_CARLSTSRES) + sizeof(setdata),
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
					"m_rc_ReCarStaRep_S3G cm_L2BfGet NG");
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}

	/************************************************************/
	/* CPRI信号データ部の信号種別(REキャリア状態報告応答)を設定 */
	/************************************************************/
	rsp_p->cpridat_carstsres.signal_kind = CMD_CPRID_CARLSTSRES + CMD_NUM1;
	
	/******************************************/
	/* CPRI信号データ部の応答結果(51:処理NG（RE）)を設定 */
	/******************************************/
	/* もし、対RECの立ち上げシーケンス未完了の場合、OKを設定 2015/11/25*/
	if( rec_Startup_First_flg[CMD_SYS_S3G] == CMD_NUM0 )
	{
		rsp_p->cpridat_carstsres.result = CMD_NML;
	}
	else
	{
		rsp_p->cpridat_carstsres.result = CMD_HDL_NG;
	}
	
	/*****************************************/
	/* CPRI信号データ部のキャリア数(0)を設定 */
	/*****************************************/
	rsp_p->cpridat_carstsres.car_num = CMD_NUM0;

	for( lwReCnt = D_RRH_CPRINO_RE1; lwReCnt <= D_RRH_CPRINO_RE_MAX ; lwReCnt++ )
	{
		f_cmn_com_carst_s3g_get(lwReCnt, &ltCarrSts);
		/* 信号種別が0: 未受信 or 応答結果: 異常の場合 */
		if(( CMD_NUM0 == ltCarrSts.signal_kind ) || ( CMD_NUM0 != ltCarrSts.result ))
		{
			continue;
		}

		/* TODO：キャリア番号が前詰めになって(異なるキャリア番号の状態を集約して)ないか確認が必要 */
		/* Loop all carrier */
		/* REから受信したREキャリア状態報告を集約する */
		for( lwCarrCnt = CMD_NUM0; lwCarrCnt < CMD_MAX_CARNUM_S3G; lwCarrCnt++ )
		{
			/* キャリア番号 */
			lwCarrNo = ltCarrSts.car_every[lwCarrCnt].car_num;
			if( CMD_NUM0 == lwCarrNo )
			{
				continue;
			}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s [lwReCnt:%d]lwCarrNo[%d]\n ", __LINE__, __FUNCTION__,lwReCnt,lwCarrNo);
#endif

			/* ------------------------------------------------------------------------------------ */
			/* キャリア単位のキャリア状態集約を行う													*/
			/* 使用中bitはOR条件、ALMbitはAND条件で設定する											*/
			/* ------------------------------------------------------------------------------------ */
			if( 0xFFFF == ltCarrStsTotal.car_every[lwCarrCnt].car_num )
			{
				/* 初回は代入 */
				ltCarrStsTotal.car_every[lwCarrCnt].car_st    = (ltCarrSts.car_every[lwCarrCnt].car_st    & CMD_CARSTBIT_VBIT);
				ltCarrStsTotal.car_every[lwCarrCnt].car_st_ul = (ltCarrSts.car_every[lwCarrCnt].car_st_ul & CMD_CARSTBIT_VBIT);
				ltCarrStsTotal.car_every[lwCarrCnt].car_st_dl = (ltCarrSts.car_every[lwCarrCnt].car_st_dl & CMD_CARSTBIT_VBIT);
			}
			else
			{
				/* 2回目以降は、USEはOR条件、ALMはAND条件で集約する */
				ltCarrStsTotal.car_every[lwCarrCnt].car_st    |= (ltCarrSts.car_every[lwCarrCnt].car_st    & CMD_CARSTBIT_USE);
				ltCarrStsTotal.car_every[lwCarrCnt].car_st_ul |= (ltCarrSts.car_every[lwCarrCnt].car_st_ul & CMD_CARSTBIT_USE);
				ltCarrStsTotal.car_every[lwCarrCnt].car_st_dl |= (ltCarrSts.car_every[lwCarrCnt].car_st_dl & CMD_CARSTBIT_USE);
				ltCarrStsTotal.car_every[lwCarrCnt].car_st    &= ((ltCarrSts.car_every[lwCarrCnt].car_st    & CMD_CARSTBIT_ALM)|(CMD_CARSTBIT_USE));
				ltCarrStsTotal.car_every[lwCarrCnt].car_st_ul &= ((ltCarrSts.car_every[lwCarrCnt].car_st_ul & CMD_CARSTBIT_ALM)|(CMD_CARSTBIT_USE));
				ltCarrStsTotal.car_every[lwCarrCnt].car_st_dl &= ((ltCarrSts.car_every[lwCarrCnt].car_st_dl & CMD_CARSTBIT_ALM)|(CMD_CARSTBIT_USE));
			}

			/* キャリア番号設定 */
			ltCarrStsTotal.car_every[lwCarrCnt].car_num 	 = lwCarrNo;

			/* Loop all branch */
			for( lwBrCnt = CMD_NUM0; lwBrCnt < ltCarrSts.car_every[lwCarrCnt].br_num; lwBrCnt++ )
			{
				lwBrNo = ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].br_num;
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s lwBrNo[%d]\n ", __LINE__, __FUNCTION__,lwBrNo);
#endif

				/* ------------------------------------------------------------------------------------ */
				/* Br単位のキャリア状態集約を行う														*/
				/* 使用中bitはOR条件、ALMbitはAND条件で設定する										*/
				/* ------------------------------------------------------------------------------------ */
				if( 0xFFFF == ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].br_num )
				{
					/* 初回は代入 */
					ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].car_st = 
											(ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].car_st    & CMD_CARSTBIT_VBIT);
					ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_ul = 
											(ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_ul & CMD_CARSTBIT_VBIT);
					ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_dl = 
											(ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_dl & CMD_CARSTBIT_VBIT);
				}
				else
				{
					/* 2回目以降は、USEはOR条件、ALMはAND条件で集約する */
					ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].car_st |= 
											(ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].car_st    & CMD_CARSTBIT_USE);
					ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_ul |= 
											(ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_ul & CMD_CARSTBIT_USE);
					ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_dl |= 
											(ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_dl & CMD_CARSTBIT_USE);
					ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].car_st &= 
											((ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].car_st    & CMD_CARSTBIT_ALM)|(CMD_CARSTBIT_USE));
					ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_ul &= 
											((ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_ul & CMD_CARSTBIT_ALM)|(CMD_CARSTBIT_USE));
					ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_dl &= 
											((ltCarrSts.car_every[lwCarrCnt].br_every[lwBrCnt].car_st_dl & CMD_CARSTBIT_ALM)|(CMD_CARSTBIT_USE));
				}

				/* Br番号設定 */
				ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].br_num = lwBrNo;
			}
		}

		rsp_p->cpridat_carstsres.result = CMD_NML;
	}

	/* ------------------------------------------------------------------------------------ */
	/* 送信系切り替えによるキャリア状態の入れ替え [Br0(DL)⇔Br1(DL)]を行う。				*/
	/* 入れ替え条件：Br0キャリア状態(DL)=USE+ALM [Br1キャリア状態(DL)は参照しない]			*/
	/* 入れ替える場合は、Br(DL)のみ入れ替える												*/
	/* ------------------------------------------------------------------------------------ */
	for( lwCarrCnt = CMD_NUM0; lwCarrCnt < CMD_MAX_CARNUM_S3G; lwCarrCnt++ )
	{
		/* キャリア番号 */
		if( 0xFFFF == ltCarrStsTotal.car_every[lwCarrCnt].car_num )
		{
			continue;
		}

		/* Br0/Br1のキャリア状態が存在しているかを調べる */
		lwBr0Idx=0xFF;
		lwBr1Idx=0xFF;
		for( lwBrCnt = CMD_NUM0; lwBrCnt < 8; lwBrCnt++ )
		{
			if( 0 == ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].br_num )
			{
				lwBr0Idx = lwBrCnt;
			}
			else if( 1 == ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].br_num )
			{
				lwBr1Idx = lwBrCnt;
			}
		}
		/* Br0/Br1のキャリア状態が存在している場合 */
		if(( lwBr0Idx != 0xFF )&&( lwBr0Idx != 0xFF ))
		{
			/* Br0系(DL)は「USE+ALM」の場合、Br0(DL)とBr1(DL)のキャリア状態を入れ替える */
			if( CMD_CARSTBIT_VBIT == ( ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBr0Idx].car_st_dl & CMD_CARSTBIT_VBIT ))
			{
				/* Br0状態一時退避 */
				lwCarrStsDL  = ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBr0Idx].car_st_dl;
				/* Br0更新 */
				ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBr0Idx].car_st_dl = ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBr1Idx].car_st_dl;
				/* Br1更新 */
				ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBr1Idx].car_st_dl = lwCarrStsDL;
			}
		}
	}

	if( CMD_NML == rsp_p->cpridat_carstsres.result )
	{
		/* Make response data */
		for( lwCarrCnt = CMD_NUM0; lwCarrCnt < CMD_MAX_CARNUM_S3G; lwCarrCnt++ )
		{
			m_rc_MakeCarDat_OnlyFHM_S3G(lwCarrCnt, &ltCarrStsTotal.car_every[lwCarrCnt]);
			if( 0xFFFF == ltCarrStsTotal.car_every[lwCarrCnt].car_num )
			{
				continue;
			}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s lwCarrNo[%d]\n ", __LINE__, __FUNCTION__,lwCarrCnt+1);
#endif
			/* キャリア数 */
			rsp_p->cpridat_carstsres.car_num++; 

			memcpy( &setdata[set_data_cnt], &ltCarrStsTotal.car_every[lwCarrCnt], sizeof(USHORT) * CMD_NUM4);
			set_data_cnt += CMD_NUM4;
			lwBrNumIndex = set_data_cnt;	/* ブランチ数n */
			
			set_data_cnt++;
			for( lwBrCnt = CMD_NUM0; lwBrCnt < 8; lwBrCnt++ )
			{
				if( 0xFFFF == ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt].br_num )
				{
					continue;
				}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s lwBrNo[%d]\n ", __LINE__, __FUNCTION__,lwBrCnt);
#endif
				/* ブランチ数n */
				setdata[lwBrNumIndex]++;
				memcpy( &setdata[set_data_cnt], &ltCarrStsTotal.car_every[lwCarrCnt].br_every[lwBrCnt], sizeof(USHORT) * CMD_NUM4);
				set_data_cnt += CMD_NUM4;
			}
			/* ALM情報数 (ALM情報数はRECに報告しない)*/
			setdata[set_data_cnt] = 0;
			set_data_cnt++;
			/* カード数:１固定 */
			setdata[set_data_cnt] = 1;
			set_data_cnt++;
			/* スロット番号:１固定 */
			setdata[set_data_cnt] = 1;
			set_data_cnt++;
		}
		/* 対REC起動シーケンスの正常終了とする */
		RecSeqComplete = D_RRH_ON;
	}

	/* 応答データに可変長データを設定 */
	cm_MemCpy(((UCHAR *)&(rsp_p->cpridat_carstsres)) + sizeof(CMT_CPRIF_CARLSTSRES),
			  &setdata[CMD_NUM0], 
			  sizeof(USHORT) * set_data_cnt);

	m_cm_CarStsResLogSave(&(rsp_p->cpridat_carstsres),
				sizeof(rsp_p->cpridat_carstsres) + sizeof(USHORT) * set_data_cnt );

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + 
							+	sizeof(CMT_CPRIF_CARLSTSRES)	/* 信号種別～キャリア情報	*/
							+	(set_data_cnt * CMD_NUM2);		/* キャリア番号		+		*/
																/* REキャリア状態	+		*/
																/* ALM情報数		+		*/
																/* カード数			+		*/
																/* スロット番号				*/
												/* キャリア状態詳細先頭情報とキャリア番号が	*/
												/* 同一エリアなので減算しておく 			*/

	/************/
	/* 応答送信 */
	/************/
	(VOID)m_cm_L3MsgSend_REC(rsp_p);
		
	/* 対REC起動シーケンスのREキャリア状態報告応答(正常)を送信時は、
	 * RECの起動シーケンスが完了したと判断して、立ち上げ応答(正常)を受信している配下REについて、
	 * Layer3状態を「④運用中状態」に設定する
	 */
	if( D_RRH_ON == RecSeqComplete )
	{
		if( D_RRH_OFF == rec_Startup_First_flg[CMD_SYS_S3G] )
		{
			rec_Startup_First_flg[CMD_SYS_S3G] = D_RRH_ON;
			cm_Assert( CMD_ASL_USELOW, CMD_SYS_S3G, "Set rec_Startup_First_flg(S3G)" );
			
//16B スロット数制限機能追加 start
//強制停止状態時はE_RRH_LAYER3_RE_OPEへの状態移行を実施しない
			f_cmn_com_layer3_get(CMD_SYS_S3G, &shmLayer3[CMD_SYS_S3G]);
			
			for (linkno = D_RRH_CPRINO_RE1; linkno <= D_RRH_CPRINO_RE_MAX; linkno++)
			{
				if (re_startupRSP[linkno][CMD_SYS_S3G] == D_RRH_ON)
				{
			        if (shmLayer3[CMD_SYS_S3G].layer3_re[linkno -1] != E_RRH_LAYER3_RE_STOP){
						(VOID)f_cmn_com_layer3_set_each(linkno, CMD_SYS_S3G, (USHORT)E_RRH_LAYER3_RE_OPE);
					}
//16B スロット数制限機能追加 end
				}
			}
		
			/* REC起動完了までに変更されたRE接続有無状態の内容を反映する */
			m_rc_ReUseApplySettingsInBoot();

			/* LED設定処理 */
			m_cm_WriteLEDReg();

			/****************************************************************************/
			/* キューイングされたCPRI接続通知があれば、キュー先頭から処理開始			*/
			/****************************************************************************/
			rec_startup_cmp = (cmw_validsysinfo == CMD_SYS_S3G) ? (rec_Startup_First_flg[CMD_SYS_S3G]) : (rec_Startup_First_flg[CMD_SYS_S3G] & rec_Startup_First_flg[CMD_SYS_3G]);

			/* REC-FHM間起動完了してれば、キューイングの先頭から処理開始 */
			if(rcw_buffcnt > CMD_NUM0 && rec_startup_cmp != D_RRH_OFF)
			{
				/* キューイング数が0となったら抜ける */
				while(rcw_buffcnt > CMD_NUM0)
				{
					/* 該当systemのL3状態取得 */
					f_cmn_com_layer3_get(rcw_system_type[0], &layer3);

					/* CPRI index算出 */
					cpr_idx = rcw_cpri[0] -1;

					/* 起動待ちアイドル状態のRE */
					if(layer3.layer3_re[cpr_idx] == E_RRH_LAYER3_RE_IDLE)
					{
						/* REの状態を全て確認 */
						for ( cnt1=CMD_NUM0; cnt1 < CMD_SYS_BOTH_3G_S3G	; cnt1++ )
						{
							f_cmn_com_layer3_get(cnt1, &layer3);

							for ( cnt2=CMD_NUM0; cnt2 < D_RRH_CPRINO_RE_MAX; cnt2++ )
							{
								/* RE起動中状態のREがあれば、LOOPを抜け、起動完了後に実施する */
								if(layer3.layer3_re[cnt2] == E_RRH_LAYER3_RE_START)
								{
									cm_Assert( CMD_ASL_USELOW, 0, "Skip RE Start [other RE start]" );

									/* 起動中REが存在していたら処理終了 */
									return;
								}
							}
						}

						/* キュー先頭のCPRI接続通知を処理 */
						m_rc_ReStr_Req(rcw_cpri[0], rcw_system_type[0]);
						cm_Assert( CMD_ASL_USELOW, 
							(((rcw_system_type[0] << 24 ) & 0xFF000000 ) | ((rcw_cpri[0] << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
							"RE Start" );

						/* 処理したCPRI接続通知を削除 */
						memcpy(&rcw_cpri[0], &rcw_cpri[1], sizeof(rcw_cpri[0]) * (rcw_buffcnt - 1));
						memcpy(&rcw_system_type[0], &rcw_system_type[1], sizeof(rcw_system_type[0]) * (rcw_buffcnt - 1));
						rcw_buffcnt--;

						/* 実施したらLOOPを抜ける */
						break;
					}
					else
					{
						/* 起動不要であるため、CPRI接続通知を削除し、次のキューを検索 */
						memcpy(&rcw_cpri[0], &rcw_cpri[1], sizeof(rcw_cpri[0]) * (rcw_buffcnt - 1));
						memcpy(&rcw_system_type[0], &rcw_system_type[1], sizeof(rcw_system_type[0]) * (rcw_buffcnt - 1));
						rcw_buffcnt--;
						cm_Assert( CMD_ASL_USELOW, 
							(((rcw_system_type[0] << 24 ) & 0xFF000000 ) | ((rcw_cpri[0] << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
							"Skip RE Start [NOT IDLE]" );
					}
				}
			}
		}
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , end\n ", __LINE__, __FUNCTION__);
#endif
	if(cmw_filedatsnd_flag !=0)
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "carr state req End:flag=%d", cmw_filedatsnd_flag);
		cmw_filedatsnd_flag --;
		if((cmw_filedatsnd_flag & 0x80000000) != 0)
		{
			cmw_filedatsnd_flag = 0;
		}
	}
	return ;
}

/********************************************************************************************************************/
/**
 *  @brief    REキャリア状態報告 (3G)
 *  @note     REキャリア状態報告
 *  @param    parm_p	[in]	parameter pointer
 *  @return   None
 *  @retval   -
 *  @date     2015/10/08 FPT)Yen create
 *  @date     2015/10/27 FPT)Yen fix bug IT3 No36
 *  @date     2015/10/29 TDI)satou FHM技説102-003
 *  @date     2015/11/18 FPT)Yen fix bug IT3 No89
 *  @date     2015/12/07 FPT)Lay fix bug IT3 No134
 *  @date     2016/02/05 TDI)enoki 3G時のREC起動シーケンス完了判断をMTアドレス設定からREキャリア状態報告に変更
 *  @date     2016/03/25 TDI)satou M-RRU-ZSYS-02091 FHMにTRX設定があり、REからキャリアなしが報告された場合に、固定値を報告するように
 *  @date     2016/11/16 FJT)ohashi 16B スロット数制限機能追加(強制停止状態でのOPE状態移行の抑止)
 *  @date     2017/01/26 FJT)ohashi 3201-16B-TS96対処 キューイングREのCPRI接続通知条件を追加
 *  @date     2017/02/21 FJT)ohashi 3201-16B-TS96対処 RE起動が並行で行われる不具合対処
 *  @date     2021/01/18 M&C)Tri.hn Update based on No.18 (sequence 3-27) - Return NG in case of 3G signal
 */
/********************************************************************************************************************/
VOID m_rc_ReCarStaRep_3G(VOID* parm_p)
{
	CMT_TSKIF_CPRISND_CARLSTSRES 	*rsp_p;								/* タスク間フレームポインタ		*/
	UINT   							bufget_rslt;						/* バッファ獲得結果				*/
	USHORT							setdata[(sizeof(T_RRH_CARST_3G) - sizeof(USHORT) * CMD_NUM4)];
// 	USHORT 							set_data_cnt = CMD_NUM0;			/* データ長						*/
// 	USHORT							lwReCnt;
// 	USHORT							lwCarrCnt;
// 	USHORT							lwCarrNo;
// 	USHORT							lwCopyErrNum;
// 	USHORT							allAlmInfo = CMD_CARSTBIT_ALM;
// 	T_RRH_CARST_3G					ltCarrSts;
// 	T_RRH_CARST_3G					ltCarrStsTotal;
// 	UINT                            chkbit_CarEqp_Repo[CMD_MAX_CARNUM_3G] = {0};
// 	UINT                            chkbit_CarEqp_ALM1[CMD_MAX_CARNUM_3G] = {0};
// 	UINT                            chkbit_CarEqp_ALM2[CMD_MAX_CARNUM_3G] = {0};
// 	USHORT							lnkErrlfg;
// 	USHORT							RecSeqComplete=D_RRH_OFF;
// 	UINT							rec_startup_cmp = D_RRH_OFF;
// 	INT								linkno;
// 	INT								isSet_CarDat_FhmOnly;
// 	USHORT							cpr_idx;
// 	T_RRH_LAYER3 					layer3;

//16B スロット数制限機能追加 start
//	T_RRH_LAYER3 shmLayer3[CMD_MAX_SYSNUM];
//16B スロット数制限機能追加 end
//	UINT	cnt1;
//	UINT	cnt2;

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , start\n ", __LINE__, __FUNCTION__);
#endif	

// 	cm_MemClr(setdata, sizeof(setdata));
// 	cm_MemClr(&ltCarrStsTotal, sizeof(ltCarrStsTotal));
// 	for( lwCarrCnt = CMD_NUM0; lwCarrCnt < CMD_MAX_CARNUM_3G; lwCarrCnt++ )
// 	{
// 		ltCarrStsTotal.car_every[lwCarrCnt].carrier_num = 0xFFFF;
// 	}

	/* ポインタ初期化 */
	rsp_p = NULL;

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 取りうる最大のデータ長でバッファを獲得する */
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_CARLSTSRES) + sizeof(setdata),
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
					"m_rc_ReCarStaRep_NR cm_L2BfGet NG");
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}

	/************************************************************/
	/* CPRI信号データ部の信号種別(REキャリア状態報告応答)を設定 */
	/************************************************************/
	rsp_p->cpridat_carstsres.signal_kind = CMD_CPRID_CARLSTSRES;
	
	/************************************************************/
	/* CPRI信号応答結果にNGを設定 */
	/************************************************************/
	rsp_p->cpridat_carstsres.result = CMD_RPT_NG;

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_CARLSTSRES);

	/************/
	/* 応答送信 */
	/************/
	(VOID)m_cm_L3MsgSend_REC(rsp_p);
	
// 	/******************************************/
// 	/* CPRI信号データ部の応答結果(2:報告NG)を設定 */
// 	/******************************************/
// 	/* もし、対RECの立ち上げシーケンス未完了の場合、OKを設定 2015/11/25*/
// 	if( rec_Startup_First_flg[CMD_SYS_3G] == CMD_NUM0 )
// 	{
// 		rsp_p->cpridat_carstsres.result = CMD_NML;
// 	}
// 	else
// 	{
// 		rsp_p->cpridat_carstsres.result = CMD_RPT_NG;
// 	}
	
// 	/*****************************************/
// 	/* CPRI信号データ部のキャリア数(0)を設定 */
// 	/*****************************************/
// 	rsp_p->cpridat_carstsres.car_num = CMD_NUM0;

// 	for( lwReCnt = D_RRH_CPRINO_RE1; lwReCnt <= D_RRH_CPRINO_RE_MAX ; lwReCnt++ )
// 	{
// 		f_cmn_com_carst_3g_get(lwReCnt, &ltCarrSts);
// 		/* 信号種別が0: 未受信 or 応答結果: 異常の場合 */
// 		if(( CMD_NUM0 == ltCarrSts.signal_kind ) || ( CMD_NUM0 != ltCarrSts.result ))
// 		{
// 			continue;
// 		}

// 		/* TODO：キャリア番号が前詰めになって(異なるキャリア番号の状態を集約して)ないか確認が必要 */
// 		/* Loop all carrier */
// 		/* REから受信したREキャリア状態報告を集約する */
// 		for( lwCarrCnt = CMD_NUM0; lwCarrCnt < CMD_MAX_CARNUM_3G; lwCarrCnt++ )
// 		{
// 			/* キャリア番号 */
// 			lwCarrNo = ltCarrSts.car_every[lwCarrCnt].carrier_num;
// 			if( CMD_NUM0 == lwCarrNo )
// 			{
// 				continue;
// 			}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s [lwReCnt:%d]lwCarrNo[%d]\n ", __LINE__, __FUNCTION__,lwReCnt,lwCarrNo);
// #endif
// 			ltCarrStsTotal.car_every[lwCarrNo - CMD_NUM1].carrier_num 	 = lwCarrNo;
// 			chkbit_CarEqp_Repo[lwCarrNo - CMD_NUM1] |= (UINT)(1 << (lwReCnt - 1));

// 			/*	使用中bitとALMbit以外は未使用であるためマスクして設定する	*/
// 			ltCarrStsTotal.car_every[lwCarrCnt].carrier_st |= (ltCarrSts.car_every[lwCarrCnt].carrier_st & CMD_CARSTBIT_VBIT);
// 			allAlmInfo &= (ltCarrSts.car_every[lwCarrCnt].carrier_st & CMD_CARSTBIT_ALM);

// 			if (1 == ltCarrSts.car_every[lwCarrCnt].carrier_eqp)
// 			{
// 			    /* 1:0系送信不可能 */
// 			    chkbit_CarEqp_ALM1[lwCarrNo - CMD_NUM1] |= (UINT)(1 << (lwReCnt - 1));
// 			}
// 			else if (2 == ltCarrSts.car_every[lwCarrCnt].carrier_eqp)
// 			{
// 			    /* 2:1系送信不可能 */
// 			    chkbit_CarEqp_ALM2[lwCarrNo - CMD_NUM1] |= (UINT)(1 << (lwReCnt - 1));
// 			}
// 			else {}

// 			if( ( 255 - ltCarrStsTotal.car_every[lwCarrNo - CMD_NUM1].err_num ) < ltCarrSts.car_every[lwCarrCnt].err_num )
// 			{
// 				lwCopyErrNum = 255 - ltCarrStsTotal.car_every[lwCarrNo - CMD_NUM1].err_num;
// 			}
// 			else
// 			{
// 				lwCopyErrNum = ltCarrSts.car_every[lwCarrCnt].err_num;
// 			}
// 			memcpy( &ltCarrStsTotal.car_every[lwCarrNo - CMD_NUM1].err_inf[ltCarrStsTotal.car_every[lwCarrNo - CMD_NUM1].err_num],
// 					&ltCarrSts.car_every[lwCarrCnt].err_inf[CMD_NUM0],
// 					sizeof(USHORT) * lwCopyErrNum );
// 			ltCarrStsTotal.car_every[lwCarrNo - CMD_NUM1].err_num += lwCopyErrNum;
// 		}
		
// 		rsp_p->cpridat_carstsres.result = CMD_NML;
// 	}

// 	if( CMD_NML == rsp_p->cpridat_carstsres.result )
// 	{
// 		/* Make response data */
// 		for( lwCarrCnt = CMD_NUM0; lwCarrCnt < CMD_MAX_CARNUM_3G; lwCarrCnt++ )
// 		{
// 			isSet_CarDat_FhmOnly = m_rc_MakeCarDat_OnlyFHM_3G(lwCarrCnt, &ltCarrStsTotal.car_every[lwCarrCnt]);

// 			if( 0xFFFF == ltCarrStsTotal.car_every[lwCarrCnt].carrier_num )
// 			{
// 				continue;
// 			}
// #ifdef FHM_RCT_DEBUG_FOR_IT1
// 	printf("[%d]%s lwCarrNo[%d]\n ", __LINE__, __FUNCTION__,lwCarrCnt+1);
// #endif
// 			/* キャリア数 */
// 			rsp_p->cpridat_carstsres.car_num++; 

// 			if (D_RRH_FALSE == isSet_CarDat_FhmOnly)
// 			{
// 				/*
// 				 * キャリア状態
// 				 * ------------
// 				 * 全ての配下REのREキャリア状態報告の当該応答でALMがない場合 : 使用中
// 				 * 1つ以上の配下REのREキャリア状態報告応答がALMである場合    : 使用中＋ERR
// 				 * 全ての配下REのREキャリア状態報告の当該応答がALMである場合 : ALM
// 				 */
// 				/*	全配下REがALMではない場合	*/
// 				if(allAlmInfo != CMD_CARSTBIT_ALM)
// 				{
// 					/*	1つ以上の全配下REにALMがある場合	*/
// 					if((ltCarrStsTotal.car_every[lwCarrCnt].carrier_st & CMD_CARSTBIT_ALM) != 0)
// 					{
// 						ltCarrStsTotal.car_every[lwCarrCnt].carrier_st &= ~CMD_CARSTBIT_ALM;
// 						ltCarrStsTotal.car_every[lwCarrCnt].carrier_st |= CMD_CARSTBIT_ERR;
// 					}
// 					/*	全配下REにALMでない場合	*/
// 					else
// 					{
// 						ltCarrStsTotal.car_every[lwCarrCnt].carrier_st &= ~(CMD_CARSTBIT_ALM | CMD_CARSTBIT_ERR);

// 						/************************************************************************/
// 						/*	当該配下REからREキャリア状態報告の当該応答がALMとして動作する		*/
// 						/*	既にALM/ERRになっている場合は、リンク断が存在しても報告値は変化無し	*/
// 						/*	であるため、全リンクがALM無しの場合のみ処理を行う					*/
// 						/************************************************************************/
// 						/*	配下REのリンク断が有無をチェックする	*/
// 						m_rc_chk_l3layer(CMD_SYS_3G, &lnkErrlfg);
// 						/*	FHM-配下RE間のCPRIリンク断をFHMが検出した場合						*/
// 						if(lnkErrlfg == CMD_ON)
// 						{
// 							/*	一部配下REがALMとなるため、キャリア状態にERRを付与する	*/
// 							ltCarrStsTotal.car_every[lwCarrCnt].carrier_st |= CMD_CARSTBIT_ERR;
// 						}
// 					}
// 				}
// 				/*
// 				 * キャリア構成状態
// 				 * ----------------
// 				 * - 全ての配下REのREキャリア状態報告の当該応答が「1系送信不可能」である場合 : 2:1系送信不可能
// 				 * - 全ての配下REのREキャリア状態報告の当該応答が                            : 1:0系送信不可能
// 				 *   「0系送信不可能」若しくは「1系送信不可能」である場合
// 				 * - 全ての配下REのREキャリア状態報告の当該応答が「0系送信不可能」である場合 : 1:0系送信不可能
// 				 * - 全ての配下REのREキャリア状態報告の当該応答が「正常送信」の場合          : 0:正常送信
// 				 * - 全てではないが1つ以上の配下REのREキャリア状態報告の当該応答が           : 0:正常送信
// 				 *   「0系送信不可能」若しくは「1系送信不可能」である場合
// 				 */
// 				if (chkbit_CarEqp_Repo[lwCarrCnt] == chkbit_CarEqp_ALM2[lwCarrCnt])
// 				{
// 					ltCarrStsTotal.car_every[lwCarrCnt].carrier_eqp = 2;
// 				}
// 				else if (chkbit_CarEqp_Repo[lwCarrCnt] == (chkbit_CarEqp_ALM1[lwCarrCnt] | chkbit_CarEqp_ALM2[lwCarrCnt]))
// 				{
// 					ltCarrStsTotal.car_every[lwCarrCnt].carrier_eqp = 1;
// 				}
// 				else if (chkbit_CarEqp_Repo[lwCarrCnt] == chkbit_CarEqp_ALM1[lwCarrCnt])
// 				{
// 					ltCarrStsTotal.car_every[lwCarrCnt].carrier_eqp = 1;
// 				}
// 				else
// 				{
// 					ltCarrStsTotal.car_every[lwCarrCnt].carrier_eqp = 0;
// 				}
// 			}

// 			/* キャリア番号, キャリア状態, キャリア構成状態 */
// 			memcpy( &setdata[set_data_cnt], &ltCarrStsTotal.car_every[lwCarrCnt], sizeof(USHORT) * CMD_NUM3);
// 			set_data_cnt += CMD_NUM3;
// 			/* ALM情報数:0固定 */
// 			setdata[set_data_cnt] = 0;
// 			set_data_cnt++;
			
// 			/* ERR情報数 */
// 			setdata[set_data_cnt] = ltCarrStsTotal.car_every[lwCarrCnt].err_num;
// 			set_data_cnt++;
// 			/* Sort data */
// 		    l3_com_ushortsort_asc(
// 		        ltCarrStsTotal.car_every[lwCarrCnt].err_num, 
// 		        sizeof(USHORT)/sizeof(USHORT),
// 		        &ltCarrStsTotal.car_every[lwCarrCnt].err_inf[CMD_NUM0]
// 		    );
// 			/* ERR情報 */
// 			memcpy( &setdata[set_data_cnt], &ltCarrStsTotal.car_every[lwCarrCnt].err_inf[CMD_NUM0],
// 					sizeof(USHORT) * ltCarrStsTotal.car_every[lwCarrCnt].err_num );
// 			set_data_cnt += ltCarrStsTotal.car_every[lwCarrCnt].err_num;	

// 			/* カード数:１固定 */
// 			setdata[set_data_cnt] = 1;
// 			set_data_cnt++;
// 			/* スロット番号:１固定 */
// 			setdata[set_data_cnt] = 1;
// 			set_data_cnt++;
// 		}

// 		/* 対REC起動シーケンスの正常終了とする */
// 		RecSeqComplete = D_RRH_ON;
// 	}

// 	/* 応答データに可変長データを設定 */
// 	cm_MemCpy(((UCHAR *)&(rsp_p->cpridat_carstsres)) + sizeof(CMT_CPRIF_CARLSTSRES),
// 			  &setdata[CMD_NUM0], 
// 			  sizeof(USHORT) * set_data_cnt);

// 	m_cm_CarStsResLogSave(&(rsp_p->cpridat_carstsres),
// 				sizeof(rsp_p->cpridat_carstsres) + sizeof(USHORT) * set_data_cnt );

// 	/**********************/
// 	/* タスク間ヘッダ作成 */
// 	/**********************/
// 	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + 
// 							+	sizeof(CMT_CPRIF_CARLSTSRES)	/* 信号種別～キャリア情報	*/
// 							+	(set_data_cnt * CMD_NUM2);		/* キャリア番号		+		*/
// 																/* REキャリア状態	+		*/
// 																/* ALM情報数		+		*/
// 																/* カード数			+		*/
// 																/* スロット番号				*/
// 												/* キャリア状態詳細先頭情報とキャリア番号が	*/
// 												/* 同一エリアなので減算しておく 			*/

// 	/************/
// 	/* 応答送信 */
// 	/************/
// 	(VOID)m_cm_L3MsgSend_REC(rsp_p);
	


// 	/* 対REC起動シーケンスのREキャリア状態報告応答(正常)を送信時は、
// 	 * RECの起動シーケンスが完了したと判断して、立ち上げ応答(正常)を受信している配下REについて、
// 	 * Layer3状態を「④運用中状態」に設定する
// 	 */
// 	if( D_RRH_ON == RecSeqComplete )
// 	{
// 		if( D_RRH_OFF == rec_Startup_First_flg[CMD_SYS_3G] )
// 		{
// 			rec_Startup_First_flg[CMD_SYS_3G] = D_RRH_ON;
// 			cm_Assert( CMD_ASL_USELOW, CMD_SYS_3G, "Set rec_Startup_First_flg(3G) " );
			
// //16B スロット数制限機能追加 start
// //強制停止状態時はE_RRH_LAYER3_RE_OPEへの状態移行を実施しない
// 			f_cmn_com_layer3_get(CMD_SYS_3G, &shmLayer3[CMD_SYS_3G]);
			
// 			for (linkno = D_RRH_CPRINO_RE1; linkno <= D_RRH_CPRINO_RE_MAX; linkno++)
// 			{
// 				if (re_startupRSP[linkno][CMD_SYS_3G] == D_RRH_ON)
// 				{
// 			        if (shmLayer3[CMD_SYS_3G].layer3_re[linkno -1] != E_RRH_LAYER3_RE_STOP){
// 						(VOID)f_cmn_com_layer3_set_each(linkno, CMD_SYS_3G, (USHORT)E_RRH_LAYER3_RE_OPE);
// 					}
// //16B スロット数制限機能追加 end
// 				}
// 			}
		
// 			/* REC起動完了までに変更されたRE接続有無状態の内容を反映する */
// 			m_rc_ReUseApplySettingsInBoot();

// 			/* LED設定処理 */
// 			m_cm_WriteLEDReg();

// 			/****************************************************************************/
// 			/* キューイングされたCPRI接続通知があれば、キュー先頭から処理開始			*/
// 			/****************************************************************************/
// 			rec_startup_cmp = (cmw_validsysinfo == CMD_SYS_S3G) ? (rec_Startup_First_flg[CMD_SYS_S3G]) : (rec_Startup_First_flg[CMD_SYS_S3G] & rec_Startup_First_flg[CMD_SYS_3G]);

// 			/* REC-FHM間起動完了してれば、キューイングの先頭から処理開始 */
// 			if(rcw_buffcnt > CMD_NUM0 && rec_startup_cmp != D_RRH_OFF)
// 			{
// 				/* キューイング数が0となったら抜ける */
// 				while(rcw_buffcnt > CMD_NUM0)
// 				{
// 					/* 該当systemのL3状態取得 */
// 					f_cmn_com_layer3_get(rcw_system_type[0], &layer3);

// 					/* CPRI index算出 */
// 					cpr_idx = rcw_cpri[0] -1;

// 					/* 起動待ちアイドル状態のRE */
// 					if(layer3.layer3_re[cpr_idx] == E_RRH_LAYER3_RE_IDLE)
// 					{
// 						/* REの状態を全て確認 */
// 						for ( cnt1=CMD_NUM0; cnt1 < CMD_SYS_BOTH_3G_S3G	; cnt1++ )
// 						{
// 							f_cmn_com_layer3_get(cnt1, &layer3);

// 							for ( cnt2=CMD_NUM0; cnt2 < D_RRH_CPRINO_RE_MAX; cnt2++ )
// 							{
// 								/* RE起動中状態のREがあれば、LOOPを抜け、起動完了後に実施する */
// 								if(layer3.layer3_re[cnt2] == E_RRH_LAYER3_RE_START)
// 								{
// 									cm_Assert( CMD_ASL_USELOW, 0, "Skip RE Start [other RE start]" );

// 									/* 起動中REが存在していたら処理終了 */
// 									return;
// 								}
// 							}
// 						}

// 						/* キュー先頭のCPRI接続通知を処理 */
// 						m_rc_ReStr_Req(rcw_cpri[0], rcw_system_type[0]);
// 						cm_Assert( CMD_ASL_USELOW, 
// 							(((rcw_system_type[0] << 24 ) & 0xFF000000 ) | ((rcw_cpri[0] << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
// 							"RE Start" );

// 						/* 処理したCPRI接続通知を削除 */
// 						memcpy(&rcw_cpri[0], &rcw_cpri[1], sizeof(rcw_cpri[0]) * (rcw_buffcnt - 1));
// 						memcpy(&rcw_system_type[0], &rcw_system_type[1], sizeof(rcw_system_type[0]) * (rcw_buffcnt - 1));
// 						rcw_buffcnt--;

// 						/* 実施したらLOOPを抜ける */
// 						break;
// 					}
// 					else
// 					{
// 						/* 起動不要であるため、CPRI接続通知を削除し、次のキューを検索 */
// 						memcpy(&rcw_cpri[0], &rcw_cpri[1], sizeof(rcw_cpri[0]) * (rcw_buffcnt - 1));
// 						memcpy(&rcw_system_type[0], &rcw_system_type[1], sizeof(rcw_system_type[0]) * (rcw_buffcnt - 1));
// 						rcw_buffcnt--;
// 						cm_Assert( CMD_ASL_USELOW, 
// 							(((rcw_system_type[0] << 24 ) & 0xFF000000 ) | ((rcw_cpri[0] << 16 ) & 0x00FF0000 ) | ((rec_startup_cmp << 8) & 0x0000FF00 ) | ( layer3.layer3_re[cpr_idx] & 0x000000FF )), 
// 							"Skip RE Start [NOT IDLE]" );
// 					}
// 				}
// 			}
// 		}
// 	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , end\n ", __LINE__, __FUNCTION__);
#endif
	return ;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RE carrier status report response
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @date   2021/02/22 M&C)Tri.hn Create
 */
/********************************************************************************************************************/
VOID m_rc_ReCarStaRepRes_S3G( VOID *parm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , start\n ", __LINE__, __FUNCTION__);
#endif
	UINT	sigchk_rslt;					/*  result of checking signal */
	USHORT	sigsys_rslt;					/*  system by checking signal */

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	sigchk_rslt = m_cm_chksig( ((CMT_TSKIF_CPRISND_CARLSTSRES *)parm_p)->cpridat_carstsres.signal_kind, &sigsys_rslt );
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW, sigsys_rslt, "m_rc_ReCarStaRepRes_S3G validate signal NG");
	}

	/* 3G の場合 */
	if( CMD_SYS_3G == sigsys_rslt )
	{
		cm_Assert(	CMD_ASL_USELOW, sigsys_rslt, "m_rc_ReCarStaRepRes_S3G 3G signal NG");
		return;
	}
	/* LTE の場合 */
	else
	{
		/************/
		/* 応答送信 */
		/************/
		(VOID)m_cm_L3MsgSend_REC(parm_p);
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , end\n ", __LINE__, __FUNCTION__);
#endif
	return;
}
/* @} */

/********************************************************************************************************************/
/**
 *  @brief    FHMのTRX設定状態のみでキャリア状態報告内容を作成する(LTE)
 *  @param    carIdx		[in ] キャリア情報作成先インデックス
 *  @param    carEvery		[out] キャリア状態報告内容作成先
 *  @return   None
 *  @date     2016/03/25 TDI)satou M-RRU-ZSYS-02091 create
 */
/********************************************************************************************************************/
static VOID m_rc_MakeCarDat_OnlyFHM_S3G(USHORT carIdx, T_RRH_CARST_S3G_CAR_EVERY *carEvery)
{
	USHORT carNo = carIdx + 1;
	T_TRXSET_PARA_LTE shmTrxPrm;

	/* 共有メモリからTRX設定パラメータを取得 */
	f_cmn_com_trxset_para_lte_get(carNo, &shmTrxPrm);

	if (E_RRH_TRXSTA_SETING == shmTrxPrm.wStatus)
	{	/* RECからのTRX設定処理中の場合、「キャリアなし」を報告する */
		m_rc_MakeCarDat_OnlyFHM_S3G_SetDisable(carEvery);
		return;
	}

	if (0 == shmTrxPrm.wSigKind)
	{	/* TRX設定されていない場合、「キャリアなし」を報告する */
		m_rc_MakeCarDat_OnlyFHM_S3G_SetDisable(carEvery);
		return;
	}

	if (0xFFFF != carEvery->car_num)
	{	/* 配下REから「キャリアあり」が報告されている場合、 「キャリアあり」を報告する(配下REから通知された情報で作成) */
		return;
	}

	/* TRX設定されている かつ 配下REからREキャリア状態報告応答(USE)を受信していない場合、「キャリアあり(固定値)」を報告する */
	m_rc_MakeCarDat_OnlyFHM_S3G_SetEnable(carNo, shmTrxPrm.wSend, carEvery);
}

/********************************************************************************************************************/
/**
 *  @brief    FHMのTRX設定状態のみでキャリア状態報告内容を作成(LTE) : キャリアなしの場合
 *  @param    carEvery		[out] キャリア状態報告内容作成先
 *  @return   None
 *  @date     2016/03/25 TDI)satou M-RRU-ZSYS-02091 create
 */
/********************************************************************************************************************/
static VOID m_rc_MakeCarDat_OnlyFHM_S3G_SetDisable(T_RRH_CARST_S3G_CAR_EVERY *carEvery)
{
	/* 0xFFFFを設定することで、該当キャリアは報告対象外になる */
	carEvery->car_num	= 0xFFFF;
}

/********************************************************************************************************************/
/**
 *  @brief    FHMのTRX設定状態のみでキャリア状態報告内容を作成(LTE) : キャリアありの場合
 *  @param    carEvery		[out] キャリア状態報告内容作成先
 *  @return   None
 *  @date     2016/03/25 TDI)satou M-RRU-ZSYS-02091 create
 */
/********************************************************************************************************************/
static VOID m_rc_MakeCarDat_OnlyFHM_S3G_SetEnable(USHORT carNo, USHORT wSend, T_RRH_CARST_S3G_CAR_EVERY *carEvery)
{
	USHORT brIdx, brMax;

	/*
	 * REキャリア状態 	= 0x8000、UL=0x8000、DL=0x8000
	 * ブランチ数		= TRX設定の送信系にて決定する(送信系が1bit:ON=1、2bit:ON=2、･･･、8bit:ON=8)
	 * 				-> RE装置構成のアンテナ数や変数名Dの送信系統数などは関係しない
	 * (CPRI技説より、)
	 * Br#nブランチ番号			= TRX設定の送信系にて決定する(送信系のD0:ON=0、D1:ON=1、･･･、D7:ON=7)
	 * Br#nREキャリア状態		= 0x8000
	 * Br#nREキャリア状態(UL)	= 0x8000
	 * Br#nREキャリア状態(DL)	= 0x8000
	 */

	carEvery->car_num	= carNo;
	carEvery->car_st	= CMD_CARSTBIT_USE;
	carEvery->car_st_ul	= CMD_CARSTBIT_USE;
	carEvery->car_st_dl	= CMD_CARSTBIT_USE;

	carEvery->br_num = 0;
	brMax = sizeof(carEvery->br_every) / sizeof(T_RRH_CARST_S3G_BR_EVERY);
	for (brIdx = 0; brIdx < brMax; brIdx++)
	{
		if (0 != (wSend & (1<<brIdx)))
		{
			carEvery->br_num++;
			carEvery->br_every[brIdx].br_num	= brIdx;
			carEvery->br_every[brIdx].car_st	= CMD_CARSTBIT_USE;
			carEvery->br_every[brIdx].car_st_ul	= CMD_CARSTBIT_USE;
			carEvery->br_every[brIdx].car_st_dl	= CMD_CARSTBIT_USE;
		}
	}

	carEvery->alm_num		= 0;
	carEvery->card_num		= 1;
	carEvery->slot_num[0]	= 1;
}

/********************************************************************************************************************/
/**
 *  @brief    FHMのTRX設定状態のみでキャリア状態報告内容を作成する(3G)
 *  @param    carIdx		[in ] キャリア情報作成先インデックス
 *  @param    carEvery		[out] キャリア状態報告内容作成先
 *  @return   キャリア状態報告内容を編集したかどうか
 *  @retval   D_RRH_TRUE	編集した
 *  @retval   D_RRH_FALSE   編集していない
 *  @date     2016/03/25 TDI)satou M-RRU-ZSYS-02091 create
 */
/********************************************************************************************************************/
static INT m_rc_MakeCarDat_OnlyFHM_3G(USHORT carIdx, T_RRH_CARST_3G_CAR_EVERY *carEvery)
{
	USHORT carNo;
	T_TRXSET_PARA_3G shmTrxPrm;

	carNo = carIdx + 1;

	/* 共有メモリからTRX設定パラメータを取得 */
	f_cmn_com_trxset_para_3g_get(carNo, &shmTrxPrm);

	if (E_RRH_TRXSTA_SETING == shmTrxPrm.wStatus)
	{	/* RECからのTRX設定処理中の場合、「キャリアなし」を報告する */
		m_rc_MakeCarDat_OnlyFHM_3G_SetDisable(carEvery);
		return D_RRH_FALSE;
	}

	if (0 == shmTrxPrm.wSigKind)
	{	/* TRX設定されていない場合、「キャリアなし」を報告する */
		m_rc_MakeCarDat_OnlyFHM_3G_SetDisable(carEvery);
		return D_RRH_FALSE;
	}

	if (0xFFFF != carEvery->carrier_num)
	{	/* 配下REから「キャリアあり」が報告されている場合、 「キャリアあり」を報告する(配下REから通知された情報で作成) */
		return D_RRH_FALSE;
	}

	/* TRX設定されている かつ 配下REからREキャリア状態報告応答(USE)を受信していない場合、「キャリアあり(固定値)」を報告する */
	m_rc_MakeCarDat_OnlyFHM_3G_SetEnable(carNo, carEvery);
	return D_RRH_TRUE;
}

/********************************************************************************************************************/
/**
 *  @brief    FHMのTRX設定状態のみでキャリア状態報告内容を作成(3G) : キャリアなしの場合
 *  @param    carEvery		[out] キャリア状態報告内容作成先
 *  @return   None
 *  @date     2016/03/25 TDI)satou M-RRU-ZSYS-02091 create
 */
/********************************************************************************************************************/
static VOID m_rc_MakeCarDat_OnlyFHM_3G_SetDisable(T_RRH_CARST_3G_CAR_EVERY *carEvery)
{
	/* 0xFFFFを設定することで、該当キャリアは報告対象外になる */
	carEvery->carrier_num	= 0xFFFF;
}

/********************************************************************************************************************/
/**
 *  @brief    FHMのTRX設定状態のみでキャリア状態報告内容を作成(3G) : キャリアありの場合
 *  @param    carEvery		[out] キャリア状態報告内容作成先
 *  @return   None
 *  @date     2016/03/25 TDI)satou M-RRU-ZSYS-02091 create
 */
/********************************************************************************************************************/
static VOID m_rc_MakeCarDat_OnlyFHM_3G_SetEnable(USHORT carNo, T_RRH_CARST_3G_CAR_EVERY *carEvery)
{
	/*
	 * [3G]:固定値
	 * REキャリア状態 = 0x8000、キャリア構成情報=0、ALM情報数=0、ERR情報数=0、カード数=1、スロット番号=1
	 */

	carEvery->carrier_num	= carNo;
	carEvery->carrier_st	= CMD_CARSTBIT_USE;
	carEvery->carrier_eqp	= 0;
	carEvery->alm_num		= 0;
	carEvery->err_num		= 0;
	carEvery->card_num		= 1;
	carEvery->slot_num[0]	= 1;
}

/* @} */


