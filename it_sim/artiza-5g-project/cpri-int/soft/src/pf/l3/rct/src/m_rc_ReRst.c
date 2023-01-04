/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReRst.c
 *  @brief  response to RE reset request, carry out RE reset process according to reset type
 *  @date   2008/07/25 FFCS)linlj create
 *  @date	2021/01/21 M&C)Tri.hn Modify based on No.29 (seq 3-2)
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup TRA_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief     RE remote reset process
 *  @note     RE remote reset process
 *  @param    parm_p [in] the buffer address pointer of received message
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)linlj create
 *  @date	2015/07/15 fpt)tan Modify
 *  @date	2015/07/18 fpt)tan Add Self reset
 *  @date   2015/10/06 TDI)satou 「REリセット要求後にstateBに落ちる時間が遅い（4秒かかっている） IT3」の問題対処
 *  @date	2021/01/21 M&C)Tri.hn Modify based on No.29 (seq 3-2)
 */
/********************************************************************************************************************/
VOID m_rc_ReRst(CMT_TSKIF_CPRIRCV_RERSTREQ *parm_p)
{
	UINT		sigchk_rslt;				/*  result of checking signal */
	USHORT		sigsys_rslt;				/*  system by checking signal */

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif
	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
		/*end processing*/
		return;
	}
	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind,&sigsys_rslt);
	/* validate signal NG */
	if(sigchk_rslt!= CMD_RES_OK)
	{
		cm_Assert(	CMD_ASL_USELOW,
					sigsys_rslt, 
					"validate signal NG");
	}

	if (CMD_SYS_3G == sigsys_rslt)
	{
		m_rc_ReRst_3G();
	}
	else
	{
		m_rc_ReRst_S3G(parm_p);
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif

	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief	RE remote reset process (3G)
 *  @note	RE remote reset process (3G)
 *  @return	None
 *  @retval	-
 *  @date	2021/01/21 M&C)Tri.hn Create based on No.29 (seq 3-2)
 */
/********************************************************************************************************************/
VOID m_rc_ReRst_3G()
{
	CMT_TSKIF_CPRIRCV_RERSTRES 		*rsp_p;								/* タスク間フレームポインタ		*/
	UINT   							bufget_rslt;						/* バッファ獲得結果				*/
	USHORT							setdata[(sizeof(T_RRH_CARST_3G) - sizeof(USHORT) * CMD_NUM4)];

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , start\n ", __LINE__, __FUNCTION__);
#endif	

	/* ポインタ初期化 */
	rsp_p = NULL;

	/****************/
	/* バッファ獲得 */
	/****************/
	/* 取りうる最大のデータ長でバッファを獲得する */
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRIRCV_RERSTRES) + sizeof(setdata),
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
					"m_rc_ReRst_3G cm_L2BfGet NG");
		
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s , end\n ", __LINE__, __FUNCTION__);
#endif
		return;
	}

	/************************************************************/
	/* CPRI信号データ部の信号種別(中間装置のリセット制御)を設定 */
	/************************************************************/
	rsp_p->cpridat_rerstres.signal_kind = CMD_CPRID_RERSTRES;

	/************************************************************/
	/* CPRI信号応答結果にOTHER_ERRORを設定 */
	/************************************************************/
	rsp_p->cpridat_rerstres.result = CMD_OTHR_ERR_RE;

	/**********************/
	/* タスク間ヘッダ作成 */
	/**********************/
	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_RERSTRES);

	/************/
	/* 応答送信 */
	/************/
	(VOID)m_cm_L3MsgSend_REC(rsp_p);


#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif

	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief	RE remote reset process (LTE)
 *  @note	RE remote reset process (LTE)
 *  @param	parm_p [in] the buffer address pointer of received message
 *  @return	None
 *  @retval	-
 *  @date	2021/01/21 M&C)Tri.hn Create based on No.29 (seq 3-2)
 */
/********************************************************************************************************************/
VOID m_rc_ReRst_S3G(CMT_TSKIF_CPRIRCV_RERSTREQ *parm_p)
{
	T_RRH_CPRI_STATE	ltCpriSta;			/* CPRIState */
	UINT				lwLoop;				/* loop	*/

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s start\n ", __LINE__, __FUNCTION__);
#endif

	/* Tri.hn FIXME 
		Destination determination for intermediate unit or RE or LTE-FHM
	*/

	/* For RE 	*/
	if (parm_p->cpridat_rerstreq.kind == CMD_RE)
	{
		/* 共用メモリCPRI状態の呼び出し 取得用 */
		f_cmn_com_cpri_state_get( &ltCpriSta );

		for (lwLoop = CMD_NUM0; lwLoop < D_RRH_CPRINO_RE_MAX ; lwLoop ++)
		{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf("[%d]%s lwLoop= %d, CpriState=%d\n ", __LINE__, __FUNCTION__, lwLoop,ltCpriSta.cprista_re[lwLoop] );
	#endif
			/* check CPRIリンク確立済の全RE */
			/* 配下CPRIリンクがstate Fの場合 */
			if ( D_RRH_CPRISTAT_F == ltCpriSta.cprista_re[lwLoop] )
			{
				/* set CPRI番号 */
				parm_p->cprircv_inf.link_num = lwLoop + CMD_NUM1;
	#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf("[%d]%s signal_kind= %x\n ", __LINE__, __FUNCTION__, parm_p->cpridat_rerstreq.signal_kind);
		printf("[%d]%s link_num= %d\n ", __LINE__, __FUNCTION__, parm_p->cprircv_inf.link_num);
	#endif
				/*	Send [REリセット要求] (l3/rct->re/rec) */
				l3_com_sendMsg(CMD_TSKID_RCT,
							D_RRH_PROCQUE_RE,
							0,
							parm_p, sizeof(CMT_TSKIF_CPRIRCV_RERSTREQ)
							);
			}
		}
		
		/* wait 200ms */
		cm_MWait(2*10);
	}
	/* For LTE-FHM	*/
	else if (parm_p->cpridat_rerstreq.kind == CMD_LNKALL)
	{
		/* Tri.hn TODO: Not sure if this API is correct */
		/* 自カードリセット実施 */
		m_rc_cm_TraRstExec(RCD_RMT_RST_RE_ALL, D_RRH_RSTFACT_RERST);

	}
	/* For intermediate device */
	else
	{
		/* Tri.hn FIXME 
			Confirm that the entire intermediate device (all units) can be reset
			When reset is not possible (RTN is 0x01)
			Intermediate device reset request signal (0x0F)
			Reset RE
			Reset the entire intermediate device (all units)
		*/	
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif

	return;
}

/* @} */

/* @} */

