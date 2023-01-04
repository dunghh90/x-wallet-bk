/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_RePortStaRep.c
 *  @brief  RE card PORT status report process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2009/03/03 FFCS)Linlj (M-S3G-eNBPF-01470) ☆CPRI-L3信号受信におけるNegative判定処理
 *  @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9)
 *  @date   2013/11/05 FFCS)Xut modify for ZYNQ.
 *  @date   2014/12/22 FFCS)Xut Modify for ppcEOL
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

/********************************************************************************************************************/
/* prototype																										*/
/********************************************************************************************************************/
static VOID m_rc_RePortStaRep_3G(VOID);
static VOID m_rc_RePortStaRep_S3G(VOID);

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief     RE card PORT status report process
 *  @note     RE card PORT status report process
 *  @param    parm_p	[in]	parameter pointer
 *  @return   None
 *  @Bug_No   M-RRU-ZSYS-01864
 *  @date     2008/07/25 FFCS)linlj create
 *  @date     2009/03/03 FFCS)Linlj (M-S3G-eNBPF-01470) ☆CPRI-L3信号受信におけるNegative判定処理
 *  @date     2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) 
 *  @date     2011/01/13 FJT)Koshida modify for ECO-800M-RRE対応
 *  @date     2013/11/05 FFCS)Xut modify for ZYNQ.
 *  @date     2014/12/22 FFCS)Xut Modify for ppcEOL
 *  @date     2015/07/20 fpt)tan Modify
 *  @date     2015/09/17 TDIPS)Lay M-RRU-ZSYS-01864 判定誤りを修正
 *  @date     2015/11/17 FPT)Yen fix bug IT3 No85
 *  @date     2021/02/09 M&C)Huan.dh add RE PORT status report judge
*/
/********************************************************************************************************************/
VOID m_rc_RePortStaRep(VOID* parm_p)
{
	UINT	sigchk_rslt;						/*  result of checking signal */
	USHORT		sigsys_rslt;					/*  system by checking signal */

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s satrt\n ", __LINE__, __FUNCTION__);
#endif
	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		/*ASSERT*/
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt,
							"validate signal NG");
	}

	//NR independent mode or LTE + NR mode
	if (sigsys_rslt == CMD_SYS_LTE && cmw_connectoperatemode > CMD_OP_LTE)
	{
  	    m_rc_RePortStaRep_S3G();
	}
	else
	{
		m_rc_RePortStaRep_3G();
	}
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return;

}

/********************************************************************************************************************/
/**
 *  @brief		RE PORT状態報告(3G)
 *  @note		3Gの応答は、NG応答固定(2:報告NG)
 *  @return		None
 *  @Bug_No
 *  @date		2016/04/25 TDI)satou create
*/
/********************************************************************************************************************/
static VOID m_rc_RePortStaRep_3G(VOID)
{
	CMT_TSKIF_CPRISND_REPORTSTSRES	*rsp_p;			/* 応答作成先 */
	UINT							 bufget_rslt;	/* 応答作成用バッファの取得結果評価用 */

	bufget_rslt = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRISND_REPORTSTSRES), 1, (VOID**)&rsp_p);
	if(bufget_rslt != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_USELOW, bufget_rslt, "m_rc_RePortStaRep_3G cm_L2BfGet NG");
		return;
	}

	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_REPORTSTSRES);
	rsp_p->cpridat_reportstsres.signal_kind              = CMD_CPRID_REPORTSTSRES;
	rsp_p->cpridat_reportstsres.result                   = CMD_RPT_NG;
	rsp_p->cpridat_reportstsres.portsts_inf.port_out_sts = 0;
	rsp_p->cpridat_reportstsres.portsts_inf.port_in_sts  = 0;

	m_cm_L3MsgSend_REC(rsp_p);
}

/********************************************************************************************************************/
/**
 *  @brief		RE PORT状態報告(S3G)
 *  @note
 *  @return		None
 *  @Bug_No
 *  @date		2016/04/25 TDI)satou create
 *  @date		2016/04/26 TDI)enoki 割り当て対象REについて全配下REがリンク断/NG応答/T.O時処理追加
*/
/********************************************************************************************************************/
static VOID m_rc_RePortStaRep_S3G(VOID)
{
	CMT_TSKIF_CPRISND_REPORTSTSRES	*rsp_p;				/* RE PORT状態報告応答作成先 */
	UINT							 bufget_rslt;		/* 応答作成用バッファ取得結果評価用 */
	T_RRH_REPORTSTA_S3G				 shmRePortSta;		/* 共有メモリ(RE PORT状態報告) */
	T_RRH_CONN_DISC					 shmConnDisc;		/* 共有メモリ(RE使用/未使用を決定するON/OFF) */
	T_RRH_LAYER3					 shmLayer3;			/* 共有メモリ(Layer3) */
	USHORT							 port_in_sts = 0;	/* RECに通知するPORT入力状態 */
	USHORT							 re_idx;			/* インデックス(RE) */
	USHORT                           bit_no_rec;		/* RECに通知するBit位置 */
	USHORT							 mask;				/* REからの報告値で特定のBitが立っているか確認するためのマスクデータ */
	USHORT							 totalchk = 0x0020;	/* D0 ON=いずれかの割り当てあり
														*  D1 ON=対REのPORT状態報告にてOK応答あり
														*  D2 ON=対REのPORT状態報告にてNG応答あり
														*  D3 ON=対REのPORT状態報告にてT.Oあり
														*  D4 ON=対REのリンク断あり
														*  D5 ON=使用/未使用を決定するON/OFFが全てOFF
														*/

	/********************************************
	 * 共有メモリから状態取得
	 ********************************************/
	f_cmn_com_RePortSta_s3g_allget(&shmRePortSta);
	f_cmn_com_conn_disk_get(&shmConnDisc);
	f_cmn_com_layer3_get(CMD_SYS_S3G, &shmLayer3);

	/********************************************
	 * 割り当て対象REについて、全配下REがリンク断/NG応答/T.Oかを判定
	 ********************************************/
	for (re_idx = 0; re_idx < D_RRH_CPRINO_RE_MAX; re_idx++)
	{
		/* 該当REへのPORT入力割り当て有無を判定 */
		if( D_RRH_OFF == shmRePortSta.alloc_en[re_idx] )
		{
			continue;				/* 割り当てなし */
		}
		totalchk |= 0x0001;			/* 割り当てあり */

		if( D_RRH_ON == shmConnDisc.conn_disc[re_idx] )
		{
			totalchk &= ~0x0020;
		}

		/* 該当REのリンク断を判定 */
		if(( E_RRH_LAYER3_RE_START != shmLayer3.layer3_re[re_idx] )&&( E_RRH_LAYER3_RE_OPE != shmLayer3.layer3_re[re_idx] ))
		{
			totalchk |= 0x0010;		/* リンク断あり */
			continue;
		}
		
		/* 該当REのPORT状態報告にOK応答有無判定 */
		if( D_RRH_REPORTSTA_RSP_NORMAL == shmRePortSta.received_rsp[re_idx] )
		{
			totalchk |= 0x0002;		/* OK応答あり */
			continue;
		}
		
		/* 該当REのPORT状態報告にてNG応答有無判定 */
		if( D_RRH_REPORTSTA_RSP_NG == shmRePortSta.received_rsp[re_idx] )
		{
			totalchk |= 0x0004;		/* NG応答あり */
			continue;
		}
		
		/* 該当REのPORT状態報告にてT.O有無判定 */
		if( D_RRH_REPORTSTA_RSP_TO == shmRePortSta.received_rsp[re_idx] )
		{
			totalchk |= 0x0008;		/* T.Oあり */
			continue;
		}
	}

	/********************************************
	 * 応答内容作成
	 ********************************************/
	bufget_rslt = cm_L2BfGet(CMD_BUFCA_LAPSND, sizeof(CMT_TSKIF_CPRISND_REPORTSTSRES), 1, (VOID**)&rsp_p);
	if(bufget_rslt != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_USELOW, bufget_rslt, "m_rc_RePortStaRep_3G cm_L2BfGet NG");
		return;
	}

	for (re_idx = 0; re_idx < D_RRH_CPRINO_RE_MAX; re_idx++)
	{
		if (D_RRH_OFF == shmConnDisc.conn_disc[re_idx])
		{	/* RE使用/未使用を決定するON/OFFがOFFのREは、集約対象外 */
			continue;
		}

		if (D_RRH_OFF == shmRePortSta.alloc_en[re_idx])
		{
			continue;
		}

		for (bit_no_rec = 0; bit_no_rec < 16; bit_no_rec++)
		{
			if ((re_idx+1) == shmRePortSta.cpri_no[bit_no_rec])
			{
				mask = 1 << shmRePortSta.bit_no[bit_no_rec];
				if (0 != (shmRePortSta.rsp_portin[re_idx] & mask))
				{
					port_in_sts |= (USHORT)(1 << bit_no_rec);
				}
			}
		}
	}

	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + sizeof(CMT_CPRIF_REPORTSTSRES);
	rsp_p->cpridat_reportstsres.signal_kind              = CMD_CPRID_REPORTSTSRES + CMD_SYS_S3G;
	rsp_p->cpridat_reportstsres.portsts_inf.port_out_sts = 0;
	rsp_p->cpridat_reportstsres.portsts_inf.port_in_sts  = port_in_sts;
	
	/*	RECへの応答を正常応答(0)で初期化する	*/
	rsp_p->cpridat_reportstsres.result = CMD_NML;
	
	if(0x0000 == ( totalchk & 0x0001 ))
	{	/*	FHM-MTからのPort割当なしの場合	*/
		
		/*	最初のFHM-MTからのPort割当なしによるOK応答の場合	*/
		if(gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[3] == 0)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "RE PORT STS RSP OK for FHMMT NON SET. totalchk=%x", totalchk);
		}
		/*	FHM-MTからのPort割当なしによるOK応答数をインクリメントする	*/
		gw_rcw_shmRePortSta_logp->rsp_result_rec[3]++;
		gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[3]++;
	}
	else if( 0x0020 == ( totalchk & 0x0020 ))
	{	/* 割り当て対象の全REについて、使用/未使用を決定するON/OFFがOFFの場合、応答結果は正常(0) */
		
		/*	最初の全配下REの未使用によるOK応答の場合	*/
		if(gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[2] == 0)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "RE PORT STS RSP OK for ALL RE NOUSE. totalchk=%x", totalchk);
		}
		/*	全配下REの未使用によるOK応答数をインクリメントする	*/
		gw_rcw_shmRePortSta_logp->rsp_result_rec[2]++;
		gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[2]++;
	}
	else if(0x0004 == ( totalchk & 0x0006 ))
	{	/* 正常応答なしかつNG応答の場合 */
		
		/* 応答結果をNG応答(51)にする */
		rsp_p->cpridat_reportstsres.result = CMD_HDL_NG;
		
		/*	NG応答の場合	*/
		if(gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[1] == 0)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "RE PORT STS RSP NG. totalchk=%x", totalchk);
		}
		/*	NG応答数をインクリメントする	*/
		gw_rcw_shmRePortSta_logp->rsp_result_rec[1]++;
		gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[1]++;
	}
	else if(0x0008 == ( totalchk & 0x000A ))
	{	/*	正常応答なしかつ応答TOありの場合	*/
		
		/* 応答結果をNG応答(51)にする */
		rsp_p->cpridat_reportstsres.result = CMD_HDL_NG;
		
		/*	最初の応答TOの場合	*/
		if(gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[0] == 0)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "RE PORT STS RSP TO. totalchk=%x", totalchk);
		}
		/*	応答TO数をインクリメントする	*/
		gw_rcw_shmRePortSta_logp->rsp_result_rec[0]++;
		gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[0]++;
	}
	else if(0x0010 == ( totalchk & 0x0012 ))
	{	/*	正常応答なしかつリンク断の場合	*/
		
		/* 応答結果をNG応答(51)にする */
		rsp_p->cpridat_reportstsres.result = CMD_HDL_NG;
		
		/*	応答TO数(リンク断)をインクリメントする	*/
		gw_rcw_shmRePortSta_logp->rsp_result_rec[0]++;
		gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[0]++;
	}
	else
	{	/*	正常応答ありもしくは、応答未受信の場合	*/
		
		/*	正常応答ありの場合	*/
		if(0x0002 == ( totalchk & 0x0002 ))
		{
			if((gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[0] != 0) ||
					(gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[1] != 0) ||
					(gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[2] != 0) ||
					(gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[3] != 0))
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "RE PORT STS RSP ABNORMAL->OK. totalchk=%x:NON=%d:NG=%d:OK=%d", 
									totalchk, 
									gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[0], gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[1], 
									(gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[2]+gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[3]) );
				/*	各NON OK応答統計情報を初期化 	*/
				gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[0] = 0;
				gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[1] = 0;
				gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[2] = 0;
				gw_rcw_shmRePortSta_logp->rsp_result_rec_nonok[3] = 0;
			}
		}
	}
	m_cm_L3MsgSend_REC(rsp_p);
}

/********************************************************************************************************************/
/**
 *  @brief    RE card PORT forward request to re
 *  @note     RE card PORT forward request to re
 *  @param    parm_p	[in]	parameter pointer
 *  @return   None
 *  @date     2021/02/09 M&C)Huan.dh create

*/
/********************************************************************************************************************/
VOID m_rc_RePortStaReq_LTE(VOID* parm_p)
{
	UINT		sigchk_rslt;					/*  result of checking signal */
	USHORT		sigsys_rslt;					/*  system by checking signal */
	USHORT		re_idx;							/* インデックス(RE) */

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s satrt\n ", __LINE__, __FUNCTION__);
#endif
	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		/*ASSERT*/
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt,
							"validate signal NG");
	}

for (re_idx = 0; re_idx < D_RRH_CPRINO_RE_MAX; re_idx++)
	{
		/* l3/rctからの正常メッセージをre/recに送信する(RE PORT状態報告要求) */
		l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, sizeof( CMT_TSKIF_CPRIRCV_REPORTSTSREQ ) );
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return;

}

/********************************************************************************************************************/
/**
 *  @brief    RE card PORT forward reponse to rec
 *  @note     RE card PORT forward reponse to rec
 *  @param    parm_p	[in]	parameter pointer
 *  @return   None
 *  @date     2021/02/09 M&C)Huan.dh create

*/
/********************************************************************************************************************/
VOID m_rc_RePortStaRep_LTE(VOID* parm_p)
{
	UINT		sigchk_rslt;					/*  result of checking signal */
	USHORT		sigsys_rslt;					/*  system by checking signal */
	USHORT		re_idx;							/* インデックス(RE) */

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s satrt\n ", __LINE__, __FUNCTION__);
#endif
	/* validate signal */
	sigchk_rslt = m_cm_chksig(((CMT_TSKIF_CPRIRCV_SIGNALGET *)parm_p)->signal_kind,&sigsys_rslt);
	if(sigchk_rslt != CMD_RES_OK)
	{
		/*ASSERT*/
		cm_Assert(	CMD_ASL_USELOW,
							sigsys_rslt,
							"validate signal NG");
	}

	m_cm_L3MsgSend_REC(parm_p);

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end\n ", __LINE__, __FUNCTION__);
#endif
	return;

}

/* @} */


/* @} */


