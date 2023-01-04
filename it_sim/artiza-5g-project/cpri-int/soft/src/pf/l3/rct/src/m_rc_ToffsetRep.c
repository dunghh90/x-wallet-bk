/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ToffsetRep.c
 *  @brief  RE Toffset value report process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2013/11/27 FFCS)Xut modify for ZYNQ.
 *
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

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief     RE Toffset value report process
 *  @note     RE Toffset value report process
 *  @param    a_rcv_adr		[in]	receive message address
 *  @return   None
 *  @retval   -
 *  @Bug_No   M-RRU-ZSYS-01868
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2013/11/27 FFCS)Xut modify for ZYNQ.
 *  @date     2015/09/17 TDIPS)Lay M-RRU-ZSYS-01868 remove return code
 *  @date     2015/11/10 TDI)satou その他未実装-017 遅延補正処理を追加
 *  @date     2016/01/29 TDI)satou eNB-F-15B-3201-00115 APIの応答をメインルートで受信するように
 *  @date     2016/04/20 TDI)satou rrhApi_Cpri_Mnt_DelayAdjustStep10() の引数変更
 */
/********************************************************************************************************************/
VOID m_rc_ToffsetRep(VOID* a_rcv_adr)
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	CMT_TSKIF_CPRISND_TOFFSETINFRES *rsp_p;	/* タスク間フレームポインタ		*/
	UINT   bufget_rslt;						/* バッファ獲得結果				*/
	UINT	sigchk_rslt;						/*  result of checking signal */
	USHORT		sigsys_rslt;					/*  system by checking signal */
	T_RRH_DELAY_INF	ltDelayInf	= {};			/* 遅延情報*/
	T_RRH_LAYER3 shmLayer3[CMD_MAX_SYSNUM];
	USHORT cpr_num, cpr_idx;
	USHORT isStartup = D_RRH_OFF;
	USHORT forceStopEn;							/* 最大遅延量超過による強制停止への遷移有効/無効 */
	E_RRHAPI_RCODE rcode;
	
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("SIGNAL KIND: %d\n", ((CMT_TSKIF_CPRIRCV_SIGNALGET *)a_rcv_adr)->signal_kind);
#endif
	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)a_rcv_adr)->signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt, 
							"validate signal NG");
	}

	/* 最大遅延量超過による強制停止への遷移は、LTE-RECからの要求受信の場合に有効(D_RRH_ON)とする */
	forceStopEn = (CMD_SYS_S3G == sigsys_rslt) ? D_RRH_ON : D_RRH_OFF;

	/* ポインタ初期化 */
	rsp_p = NULL;

	/****************/
	/* バッファ獲得 */
	/****************/
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_TOFFSETINFRES),
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
					"m_rc_ToffsetRep cm_L2BfGet NG");
#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
#endif
		return;
	}

	/********************************************/
	/* 起動完了しているREについて遅延補正を実施 */
	/********************************************/
#if 0
//    f_cmn_com_layer3_get(CMD_SYS_3G,  &shmLayer3[CMD_SYS_3G ]);
#endif
    f_cmn_com_layer3_get(CMD_SYS_S3G, &shmLayer3[CMD_SYS_S3G]);

    for (cpr_num = D_RRH_CPRINO_RE_MIN; cpr_num <= D_RRH_CPRINO_RE_MAX; cpr_num++)
    {
        cpr_idx = cpr_num - 1;
#if 0
//        if (CMD_SYS_S3G == cmw_validsysinfo)
//        {   /* LTE単独の場合、LTEのLayer3が運用中状態なら起動完了とみなす */
//            if (E_RRH_LAYER3_RE_OPE == shmLayer3[CMD_SYS_S3G].layer3_re[cpr_idx])
//            {
//                rcode = rrhApi_Cpri_Mnt_DelayAdjustStep10(D_RRH_PROCQUE_L3, cpr_num, isStartup, forceStopEn);
//                if (E_API_RCD_OK != rcode)
//                {
//                	cm_Assert(CMD_ASL_USEHIGH, rcode, "rrhApi_Cpri_Mnt_DelayAdjustStep10 NG");
//                }
//            }
//        }
//        else
//        {   /* 共用構成の場合、3GとLTEの両方のLayer3が運用中状態なら起動完了とみなす */
//            if ((E_RRH_LAYER3_RE_OPE == shmLayer3[CMD_SYS_3G ].layer3_re[cpr_idx])
//            &&  (E_RRH_LAYER3_RE_OPE == shmLayer3[CMD_SYS_S3G].layer3_re[cpr_idx]))
//            {
//                rcode = rrhApi_Cpri_Mnt_DelayAdjustStep10(D_RRH_PROCQUE_L3, cpr_num, isStartup, forceStopEn);
//                if (E_API_RCD_OK != rcode)
//                {
//                	cm_Assert(CMD_ASL_USEHIGH, rcode, "rrhApi_Cpri_Mnt_DelayAdjustStep10 NG");
//                }
//            }
//        }
#endif
        /*	LTEからのToffset要求の場合	*/
        if(forceStopEn == D_RRH_ON)
        {
            /*	L3レイヤが運用中のリンクを対象に遅延補正を行う	*/
            if (E_RRH_LAYER3_RE_OPE == shmLayer3[CMD_SYS_S3G].layer3_re[cpr_idx])
            {
                rcode = rrhApi_Cpri_Mnt_DelayAdjustStep10(D_RRH_PROCQUE_L3, cpr_num, isStartup, forceStopEn);
                if (E_API_RCD_OK != rcode)
                {
                	cm_Assert(CMD_ASL_USEHIGH, rcode, "rrhApi_Cpri_Mnt_DelayAdjustStep10 NG");
                }
            }
		}
    }

	/* 遅延量算出・設定 */
	m_rc_chkDelay();

	/* 共有メモリ遅延補正情報の呼び出し */
	f_cmn_com_delay_inf_get(&ltDelayInf);

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF)
									+	sizeof(CMT_CPRIF_TOFFSETINFRES);

	/**********************/
	/* CPRIデータ共通設定 */
	/**********************/
	/* 応答編集 */
	rsp_p->cpridat_toffsetinfres.result      = ltDelayInf.toffsetRslt[sigsys_rslt];
	
	/* CPRI信号種別設定 */
	rsp_p->cpridat_toffsetinfres.signal_kind = CMD_CPRID_TOFFSETINFRES + sigsys_rslt;

	/* TOFFSET値設定 */
	rsp_p->cpridat_toffsetinfres.toffset_inf.toffset = ltDelayInf.ToffsetReport;

	/* 上り遅延値設定 */
	rsp_p->cpridat_toffsetinfres.toffset_inf.updeleqp   = ltDelayInf.MaxUlDelayRe[sigsys_rslt];

	/* 下り遅延値設定 */
	rsp_p->cpridat_toffsetinfres.toffset_inf.downdeleqp = ltDelayInf.MaxDlDelayRe[sigsys_rslt];

	/************/
	/* 応答送信 */
	/************/
	m_cm_L3MsgSend_REC(rsp_p);

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \nSignalkind: [%x] \nResult: [%d] \nToffset: [%d] \n",__LINE__,__FUNCTION__, CMD_CPRID_TOFFSETINFRES + sigsys_rslt, ltDelayInf.toffsetRslt[sigsys_rslt], ltDelayInf.ToffsetReport);
#endif
	return ;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief    Get Toffset value from share memory
 *  @note     
 *            立ち上げ(対RE)時にて全ての配下REからの応答がNGの場合(タイムアウト含む): return NG
 *  @param    awSystermType		[in]	systerm type
 *  @return   result
 *  @retval   -
 *  @date     2015/07/24 FPT)Yen create
 *  @date     2015/11/18 TDI)satou 「RECから定期監視で行われるToffset値報告要求に対する応答がNGとなる」対処
 */
/********************************************************************************************************************/
USHORT m_rc_GetToffset( USHORT awSystermType )
{
	T_RRH_TOFFSET_3G		ltToffset3g;
	T_RRH_TOFFSET_S3G		ltToffsetLte;
	USHORT					lwRst;
	USHORT					lwReCnt;

	if( CMD_SYS_3G == awSystermType )
	{
		/*  CPRI信号応答結果にその他エラー(2)を設定 */
		lwRst = CMD_RPT_NG;
		for( lwReCnt = CMD_NUM0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++ )
		{
			cm_MemClr(&ltToffset3g, sizeof(ltToffset3g));
			/* 共用メモリ(3G)Toffset値報告の呼び出し 取得用 */
			f_cmn_com_toffset_3g_get( lwReCnt + CMD_NUM1, &ltToffset3g );
			
			if( CMD_NUM0 != ltToffset3g.signal_kind )
			{
				lwRst = CMD_NML;
				break;
			}
		}
	}
	else
	{
		/*  CPRI信号応答結果に処理NG(RE)(51)を設定  */
		lwRst = CMD_HDL_NG;
		for( lwReCnt = CMD_NUM0; lwReCnt < D_RRH_CPRINO_RE_MAX; lwReCnt++ )
		{
			cm_MemClr(&ltToffsetLte, sizeof(ltToffset3g));
			/* 共用メモリ(LTE)Toffset値報告の呼び出し 取得用 */
			f_cmn_com_toffset_s3g_get( lwReCnt + CMD_NUM1, &ltToffsetLte );
			
			if( CMD_NUM0 != ltToffsetLte.signal_kind )
			{
				lwRst = CMD_NML;
				break;
			}
		}
	}

	return lwRst;
}
/* @} */

/********************************************************************************************************************/
/**
 *  @brief    遅延補正応答(Step10)
 *  @param    buff_adr [in] received message
 *  @return   none
 *  @date     2016/01/29 TDI)satou eNB-F-15B-3201-00115 create
 *  @date     2017/04/07 FJT)koshida 17A(3201-16B-TS105)対応 CPRIリンク異常時の機能部故障状態の集約誤り修正
 */
/********************************************************************************************************************/
VOID m_rc_DelayAdjStep10_Rsp(VOID *buff_adr)
{
	T_API_CPRILINK_ADJUST_DELAY_STEP10_RSP *parm_p;
	T_RRH_LAYER3 shmLayer3[CMD_MAX_SYSNUM];
	USHORT sysType;
	USHORT cpr_num, cpr_idx;
	UINT userCode;

	parm_p  = (T_API_CPRILINK_ADJUST_DELAY_STEP10_RSP*)buff_adr;
	cpr_num = parm_p->cpr_num;
	cpr_idx = parm_p->cpr_num - 1;

	/*
	 * 遅延補正に失敗した場合は強制停止.
	 * ただし、Layer3が起動待ちアイドル、または強制停止の場合は対象外.
	 *
	 * (1) 起動待ちアイドルの場合を除外する理由
	 * Step10は運用中状態の時にしか行わないが、要求～応答までの短い区間で
	 * CPRIリンク断(L1/L2)が発生した場合を考慮
	 *
	 * (2) 強制停止の場合を除外する理由
	 * すでに強制停止処理を実施したあとであり、無駄な処理であるため
	 */
	if (E_API_RCD_OVER_MAX_DELAY == parm_p->result)
	{
		for (sysType = 0; sysType < CMD_MAX_SYSNUM; sysType++)
		{
			if ((CMD_SYS_S3G == cmw_validsysinfo) && (CMD_SYS_3G == sysType))
			{	/* LTE単独構成の場合、3Gの処理は除外 */
				continue;
			}

			f_cmn_com_layer3_get(sysType, &shmLayer3[sysType]);

			switch (shmLayer3[sysType].layer3_re[cpr_idx])
			{
			case E_RRH_LAYER3_RE_IDLE:
			case E_RRH_LAYER3_RE_STOP:
				userCode = (sysType << 16) | cpr_num;
				cm_Assert(CMD_ASL_DBGLOW, userCode, "exempt force stop");
				break;
			default:
				m_rc_ReHCStop(cpr_num, sysType, E_RRH_RESTOP_FACTOR_DELAY_NG);
				break;
			}
		}
		/*	CPRI使用が確定したため、対象REのCPRI異常をREカード状態報告の故障状態に反映する	*/
		re_funcFailValid[cpr_num] = D_RRH_ON;
	}
	/*	CPRI使用中になって最初のStep10応答の場合	*/
	else if(re_funcFailValid[cpr_num] == D_RRH_OFF)
	{
		/*	Step10実行中(2)を設定する	*/
		re_funcFailValid[cpr_num] = 2;
	}
}

/* @} */


