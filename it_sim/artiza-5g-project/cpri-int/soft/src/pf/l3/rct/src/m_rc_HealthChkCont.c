/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_HealthChkCont.c
 *  @brief  ヘルスチェック
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
 *  @brief  外部装置データ送信要求(ヘルスチェック)処理
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01738 
 *  @date 2015/08/04 FPT)Viet Create new
 *  @date 2015/09/30 TDI)satou M-RRU-ZSYS-01738 外部装置データ送信応答が必ずNG応答になる不具合の修正
 *  @date 2015/10/23 TDI)satou FHM技説-設計変更-020
 *  @date 2016/04/25 FJT)koshida 3001-15B-TS123対応(3G/LTE個別にTILT-INF信号の変化情報を作成する)
 */
/********************************************************************************************************************/

VOID m_rc_HealthChkCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p )
{
	CMT_TSKIF_CPRIRCV_EXTDATRCVREQ response_p;
	UINT						lwSigchk_rslt;					/*  result of checking signal */
	USHORT						lwSigsys_rslt;					/*  system by checking signal */
	USHORT						lwStatusChange;
	UCHAR						lw_device_number;
	USHORT						lw_almA;
	UCHAR						lc_almB;
	T_RRH_ALM_ABC 				lt_alm_abc;
	
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
	
	/* 最新の装置状態構築 */

	/* 「別紙A 装置状態更新」参照 */

	/* TILTヘルスチェック状態チェック */
	if (gw_rcw_tiltHcSta[lwSigsys_rslt] == RCD_TILTHC_NONE)
	{
		/* Build 外部装置データ受信要求送信(状態報告信号) */
		m_rc_ExtDataRcvBuildData(parm_p, &response_p, FULL_STATE_RES_SIG, lwSigsys_rslt);
		
		gw_rcw_tiltHcSta[lwSigsys_rslt] = RCD_TILTHC_RCV;
		
		/* 外部装置データ受信要求送信(状態報告信号) */
		m_rc_ExtDataRcvReqExe(&response_p, RCD_EXTRCVREQ_STACHG, lwSigsys_rslt);
	}
	else
	{
		lwStatusChange = CMD_OFF;
		
		/* 共有メモリALM_ABCの呼び出し 取得用 */
		f_cmn_com_alm_abc_get(lwSigsys_rslt, &lt_alm_abc);
		/* Make Alm-A */
		lw_almA = m_rc_get_AlmA();
		if (lt_alm_abc.alm_a != lw_almA)
		{
			lwStatusChange = CMD_ON;
		}
		else
		{
			for (lw_device_number = DEV_NUM_MIN; lw_device_number <= DEV_NUM_MAX; lw_device_number++)
			{
				/* Make Alm-B */
				lc_almB = m_rc_get_AlmB(lw_device_number);
				if (lt_alm_abc.alm_b[lw_device_number-DEV_NUM_MIN] != lc_almB)
				{
					lwStatusChange = CMD_ON;
					break;
				}
			}
		}
		
		/*  there is no change, create response with signal "health check" */
		if ( CMD_ON != lwStatusChange )
		{
			/* Build 外部装置データ受信要求送信(ヘルスチェック) */
			m_rc_ExtDataRcvBuildData(parm_p, &response_p, HEAL_CHK_RES_SIG, lwSigsys_rslt);
			
			/* 外部装置データ受信要求送信(ヘルスチェック報告信号) */
			m_rc_ExtDataRcvReqExe(&response_p, RCD_EXTRCVREQ_HCRSP, lwSigsys_rslt);
		}
		else
		{
			/* Build 外部装置データ受信要求送信(状態報告信号) */
			m_rc_ExtDataRcvBuildData(parm_p, &response_p, CHANGE_STATE_RES_SIG, lwSigsys_rslt);
			
			/* 外部装置データ受信要求送信(状態報告信号) */
			m_rc_ExtDataRcvReqExe(&response_p, RCD_EXTRCVREQ_STACHG, lwSigsys_rslt);
		}
	}

}

/* @} */


/* @} */

