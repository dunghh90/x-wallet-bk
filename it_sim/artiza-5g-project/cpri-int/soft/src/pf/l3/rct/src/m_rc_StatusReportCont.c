/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_StatusReportCont.c
 *  @brief  状態報告
 *  @date   2015/08/04 FPT)Viet Create new  
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
 *  @brief  外部装置データ送信要求(状態報告)処理
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01738
 *  @date  2015/08/04 FPT)Viet Create new   
 *  @date  2015/09/30 TDI)satou M-RRU-ZSYS-01738 外部装置データ送信応答が必ずNG応答になる不具合の修正
 *  @date  2015/10/23 TDI)satou FHM技説-設計変更-020
 *  @date  2016/04/25 FJT)koshida 3001-15B-TS123対応(3G/LTE個別にTILT-INF信号の変化情報を作成する)
 */
/********************************************************************************************************************/
VOID m_rc_StatusReportCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p )
{
	CMT_TSKIF_CPRIRCV_EXTDATRCVREQ ltReq;
	UINT						lwSigchk_rslt;					/*  result of checking signal */
	USHORT						lwSigsys_rslt;					/*  system by checking signal */

	/*In case buffer is Null*/
	if (NULL == parm_p)
	{
		/*end processing*/
		return;
	}
	
	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_extdatsndreq.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
		return;
	}
	
	/* Build 外部装置データ受信要求送信 */
	m_rc_ExtDataRcvBuildData(parm_p, &ltReq, STATE_RES_SIG, lwSigsys_rslt);
	
	/* 外部装置データ受信要求送信(状態報告信号) */
	m_rc_ExtDataRcvReqExe(&ltReq, RCD_EXTRCVREQ_STAREP, lwSigsys_rslt);
}

/* @} */


/* @} */

