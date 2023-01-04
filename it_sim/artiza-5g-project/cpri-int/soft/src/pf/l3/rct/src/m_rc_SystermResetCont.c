/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_SystermResetCont.c
 *  @brief  局リセット
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
#include "f_rrh_reg_cnt.h"
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  外部装置データ送信要求(局リセット)処理
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01738
 *  @date   2015/08/04 FPT)Viet Create new   
 *  @date   2015/09/30 TDI)satou M-RRU-ZSYS-01738 外部装置データ送信応答が必ずNG応答になる不具合の修正
 *  @date   2015/10/19 TDI)satou ハソ-QA-036 全配下RE強制リセットの方法変更
 *  @date   2015/10/23 TDI)satou FHM技説-設計変更-020
 *  @date   2015/12/10 TDI)enoki 自身のリセットまでに50ms Wait
 *  @date   2016/04/25 FJT)koshida 3001-15B-TS123対応(3G/LTE個別にTILT-INF信号の変化情報を作成する)
 */
/********************************************************************************************************************/
VOID m_rc_SystermResetCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	CMT_TSKIF_CPRIRCV_EXTDATRCVREQ response_p;
	UINT						lwSigchk_rslt;					/*  result of checking signal */
	USHORT						lwSigsys_rslt;					/*  system by checking signal */
	
	/*In case buffer is Null*/
	if (NULL == parm_p)
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
	printf( "[%d]m_cm_chksig : %d \n",__LINE__,lwSigchk_rslt );
	#endif
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt, 
							"validate signal NG");
	}
	
	/* Build 外部装置データ受信要求送信 */
	m_rc_ExtDataRcvBuildData(parm_p, &response_p, RESET_RES_SIG, lwSigsys_rslt);
	
	/* 外部装置データ受信要求送信(状態報告信号) */
	m_rc_ExtDataRcvReqExe(&response_p, RCD_EXTRCVREQ_SYSRST, lwSigsys_rslt);

	/* 全配下REを強制リセット */
	rrhApi_Svp_Mnt_AllReL1Reset(D_RRH_PROCQUE_L3);

	/* Wait 50ms */
    cm_MWait(5);

	/* 自FHMリセット */
	m_rc_cm_TraRstExec(RCD_RMT_RST_RE_ALL, D_RRH_RSTFACT_RERST);
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
	return;

}

/* @} */


/* @} */

