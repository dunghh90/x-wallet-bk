/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReTimSet.c
 *  @brief  RE time setting process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2013/11/05 FFCS)Xut modify for ZYNQ.
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
#include "rrhApi_Svp.h"


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief     RE time setting  process
 *  @note     RE time setting  process
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2013/11/05 FFCS)Xut modify for ZYNQ.
 *  @date   2021/02/08 M&C)Huan.dh intermediate return 3G RE time setting NG
 */
/********************************************************************************************************************/
VOID m_rc_ReTimSet(CMT_TSKIF_CPRISND_RETIMSETREQ *parm_p)
{
	DATETIME_T rtc_data;					/* RTC設定データ				*/
	UINT	sigchk_rslt;					/*  result of checking signal */
	USHORT		sigsys_rslt;				/*  system by checking signal */
	INT			wtime = CMD_NUM0;		/*  応答メッセージ待ち時間(msec単位)*/
	UINT   		rtn = E_API_RCD_OK;		/* 戻り値						*/
	T_RRH_SYSTEM_TIME		datetime;
	T_RRH_LAYER3			ltLayer3Sts;	/* Layer3状態 */
	USHORT		lwCpri;						/* */
	INT			ldwResult; 					/* */
	CMT_TSKIF_CPRISND_SLOTINFRES *rsp_p;	/* タスク間フレームポインタ		*/
	USHORT set_data[CMD_NUM1	+			/* スロット情報数N				*/
					CMD_NUM8	+			/* スロット番号					*/
					CMD_NUM8	];			/* REカード機能部情報			*/
	UINT   datalength;						/* データ長						*/

#ifdef FHM_RCT_DEBUG_FOR_IT1
    int i;
    printf( "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt, 
							"validate signal NG");
		return;
	}

	/* 設定年月日字分秒をRTC設定関数へ渡す形に編集する */

	/* 初期化 */
	cm_MemClr(&rtc_data, sizeof(rtc_data));
	cm_MemClr(&datetime, sizeof(datetime));

	/**********/
	/* 年設定 */
	/**********/
	/* 0010 0000 0000 0110 */ /* 2006年   */
	/*    2    0    0    6 */
	rtc_data.year  = parm_p->cpridat_retimsetreq.year;
	/* 年のBCD範囲チェック */
	if(	(((rtc_data.year & RCD_MSK_F000) >> CMD_BIT12) > CMD_NUM9 ) ||
		(((rtc_data.year & RCD_MSK_0F00) >> CMD_BIT8 ) > CMD_NUM9 ) ||
		(((rtc_data.year & RCD_MSK_00F0) >> CMD_BIT4 ) > CMD_NUM9 ) ||
		(((rtc_data.year & RCD_MSK_000F)             ) > CMD_NUM9 ))
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					rtc_data.year,
					"m_rc_ReTimSet year data NG");
		return;
	}

	/**********/
	/* 月設定 */
	/**********/
	/* 0000 0001 0010 0110 */ /* 1月26日  */
	/*    0    1    2    6 */
	rtc_data.month = (UCHAR)((parm_p->cpridat_retimsetreq.month_day & RCD_HI_MSK) >> CMD_BIT8);
	/* 月のBCD範囲チェック */
	if( (((rtc_data.month & RCD_MSK_00F0) >> CMD_BIT4 ) > CMD_NUM9 ) ||
		(((rtc_data.month & RCD_MSK_000F)             ) > CMD_NUM9 ))
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					rtc_data.month,
					"m_rc_ReTimSet month data NG");
		return;
	}

	/**********/
	/* 日設定 */
	/**********/
	/* 0000 0001 0010 0110 */ /* 1月26日  */
	/*    0    1    2    6 */
	rtc_data.day    = (CHAR)(parm_p->cpridat_retimsetreq.month_day & RCD_LW_MSK);
	/* 日のBCD範囲チェック */
	if(	(((rtc_data.day & RCD_MSK_00F0) >> CMD_BIT4 ) > CMD_NUM9 ) ||
		(((rtc_data.day & RCD_MSK_000F)             ) > CMD_NUM9 ))
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					rtc_data.day,
					"m_rc_ReTimSet day data NG");
		return;
	}

	/**********/
	/* 時設定 */
	/**********/
	/* ---- ---- 0001 0010 */ /* 12時     */
	/*   未   未    1    2 */
	rtc_data.hour  = (CHAR)(parm_p->cpridat_retimsetreq.hour & RCD_LW_MSK);
	/* 時のBCD範囲チェック */
	if( (((rtc_data.hour & RCD_MSK_00F0) >> CMD_BIT4 ) > CMD_NUM9 ) ||
		(((rtc_data.hour & RCD_MSK_000F)             ) > CMD_NUM9 ))
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					rtc_data.hour,
					"m_rc_ReTimSet hour data NG");
		return;
	}

	/**********/
	/* 分設定 */
	/**********/
	/* 0011 0100 0101 0110 */ /* 34分56秒 */
	/*    3    4    5    6 */
	rtc_data.min   = (CHAR)((parm_p->cpridat_retimsetreq.minute_sec & RCD_HI_MSK) >> CMD_BIT8);
	/* 分のBCD範囲チェック */
	if(	(((rtc_data.min & RCD_MSK_00F0) >> CMD_BIT4 ) > CMD_NUM9 ) ||
		(((rtc_data.min & RCD_MSK_000F)             ) > CMD_NUM9 ))
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					rtc_data.min,
					"m_rc_ReTimSet min data NG");
		return;
	}

	/**********/
	/* 秒設定 */
	/**********/
	/* 0011 0100 0101 0110 */ /* 34分56秒 */
	/*    3    4    5    6 */
	rtc_data.sec   = (CHAR)(parm_p->cpridat_retimsetreq.minute_sec & RCD_LW_MSK);
	/* 秒のBCD範囲チェック */
	if(	(((rtc_data.sec & RCD_MSK_00F0) >> CMD_BIT4 ) > CMD_NUM9 ) ||
		(((rtc_data.sec & RCD_MSK_000F)             ) > CMD_NUM9 ))
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					rtc_data.sec,
					"m_rc_ReTimSet sec data NG");
		return;
	}

	/**************/
	/* ミリ秒設定 */
	/**************/
	rtc_data.msec  = (CHAR)(CMD_NUM0);

	memcpy(&datetime, &rtc_data, sizeof(rtc_data));

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING,"Time Set %x-%x-%x %x:%x:%x (system=%d)",
					rtc_data.year,
					rtc_data.month,
					rtc_data.day,
					rtc_data.hour,
					rtc_data.min,
					rtc_data.sec,
					sigsys_rslt
					);

	/*******************************/
	/* API関数(時刻設定通知)コール */
	/*******************************/
	rtn = rrhApi_Svp_Mnt_SetDateTime(D_RRH_PROCQUE_L3, wtime, D_RRH_NULL, datetime);

	if(rtn != E_API_RCD_OK)
	{
		/* アサート情報出力 */
		cm_Assert(	CMD_ASL_USELOW,
					CMD_NUM0, 
					"rrhApi_Svp_Mnt_SetDateTime NG");
	}


	/*************************************/
	/* 配下REにRE時刻設定要求を送信する  */
	/*************************************/
	/* 共用メモリLayer3の呼び出し 取得用 */
	if( CMD_SYS_3G == sigsys_rslt)
	{
		//f_cmn_com_layer3_get(CMD_SYS_3G, &ltLayer3Sts);
		/* ポインタ初期化 */
		rsp_p = NULL;

		/* 可変長設定用データ	*/
		cm_MemClr(set_data, sizeof(set_data));

		/****************/
		/* バッファ獲得 */
		/****************/
		/* 取りうる最大のデータ長でバッファを獲得する */
		cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_SLOTINFRES)+	/* 応答結果まで	*/
								sizeof(set_data)					,
								CMD_NUM1,
								(VOID **)&rsp_p	);
		
		/* CPRI信号応答結果にOKを設定 */
		rsp_p->cpridat_slotinfres.result = CMD_HDL_NG;
		
		/* CPRI信号種別設定 */
		rsp_p->cpridat_slotinfres.signal_kind = CMD_CPRID_SLOTINFRES;

		/****************/
		/* データ長設定 */
		/****************/
		datalength = (USHORT)(	sizeof(CMT_CPRISIGSND_INF)
							+	sizeof(CMT_CPRIF_SLOTINFRES)	/* 信号種別～キャリア情報	*/
							+	(3 * CMD_NUM2));		/* スロット情報数 +			*/
																/* スロット情報   +			*/
																/* REカード機能部情報		*/

		/* 応答信号にレングス長設定 */
		rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + datalength;
		
		/************/
		/* 応答送信 */
		/************/
		m_cm_L3MsgSend_REC(rsp_p);
		return ;

	}
	else
	{
		f_cmn_com_layer3_get(CMD_SYS_LTE, &ltLayer3Sts);
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
    for( i = 0; i< D_RRH_CPRINO_RE_MAX; i++)
    {
        printf( "%02x ", ltLayer3Sts.layer3_re[i]);
    }
    printf( "\n" );
#endif

	/*  CPRIリンク数だけループ */
	for(lwCpri = CMD_NUM0; lwCpri < D_RRH_CPRINO_RE_MAX; lwCpri++)
	{
		/* 対RE Layer3状態が「②RE起動中状態」または「④運用中状態」の場合 */
		if(( D_L3_STA_2 == ltLayer3Sts.layer3_re[ lwCpri ] )||( D_L3_STA_4 == ltLayer3Sts.layer3_re[ lwCpri ] ))
		{
#ifdef FHM_RCT_DEBUG_FOR_IT1
    printf( "%s %s %d <lwCpri>%d\n", __FILE__, __FUNCTION__, __LINE__, lwCpri);
#endif
			parm_p->cprisnd_inf.link_num = lwCpri + CMD_NUM1;
			ldwResult = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, sizeof( CMT_TSKIF_CPRISND_RETIMSETREQ ) );
			if( D_RRH_OK != ldwResult)
			{
				cm_Assert(	CMD_ASL_USELOW, CMD_NUM0, "RE TimeSet Request Send NG");
			}
		}
	}

	return ;
}

/* @} */


/* @} */


