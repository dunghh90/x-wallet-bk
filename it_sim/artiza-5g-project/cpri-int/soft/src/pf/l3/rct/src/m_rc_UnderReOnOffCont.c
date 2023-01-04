/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_UnderReOnOffCont.c
 *  @brief  配下REON OFF
 *  @date   2015/08/04 FPT)Viet Create new
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
 *  @brief  外部装置データ送信要求(配下REON OFF)処理
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01738
 *  @date 2015/08/04 FPT)Viet Create new
 *  @date 2015/09/30 TDI)satou M-RRU-ZSYS-01738 外部装置データ送信応答が必ずNG応答になる不具合の修正 
 *  @date 2015/10/23 TDI)satou FHM技説-設計変更-020
 *  @date 2015/10/30 TDI)satou その他未実装-002
 *  @date 2016/04/25 FJT)koshida 3001-15B-TS123対応(3G/LTE個別にTILT-INF信号の変化情報を作成する)
 */
/********************************************************************************************************************/
VOID m_rc_UnderReOnOffCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
	#endif
	
	CMT_TSKIF_CPRIRCV_EXTDATRCVREQ response_p;
	UINT						lwSigchk_rslt;					/*  result of checking signal */
	USHORT						lwSigsys_rslt;					/*  system by checking signal */
	USHORT 						lwCpriNo;
	UCHAR						lcReOnOffSta;
	INT							ldwRtn;
	T_RRH_TX_ONOFF				ltTxOnOff;
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
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	
	/* Eフィールド(装置番号)にて対象REを指定する(1～16) */
	lwCpriNo = D_L3_CNVBCD_DEC(parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM5]);
	/* 対象REの下りIQデータの送信／停止制御　実施 */
	/* TBD */

	/* 対象REのON/OFF制御状態更新（EEPROM） */
	ldwRtn = BPF_HM_DEVC_EEPROM_READ(M_RRH_EEP_RE_ONOFF(lwCpriNo), &lcReOnOffSta);
	if( BPF_HM_DEVC_COMPLETE != ldwRtn )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "[Re On/Off control sts] EEPROM Read Access Fail.");
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}
	/* ON/OFF制御状態更新 */
	lcReOnOffSta = (CMD_ON == lcReOnOffSta)?CMD_OFF:CMD_ON;

	/* FHMのON/OFF制御状態更新（EEPROM） */
	ldwRtn = BPF_HM_DEVC_EEPROM_WRITE(M_RRH_EEP_RE_ONOFF(lwCpriNo), &lcReOnOffSta);
	if( BPF_HM_DEVC_COMPLETE != ldwRtn )
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_WARNING, "[Re On/Off control sts] EEPROM Write Access Fail.");
		#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
		#endif
		return;
	}

	/* 対象REのON/OFF制御状態更新（共有メモリ） */
	f_cmn_com_tx_onoff_get(&ltTxOnOff);
	
	/*
	 * update on/off value for ltTxOnOff struct.
	 * EEPROMは「1:OFF, その他:ON」であり、ソフト内部では「1:ON, 0:OFF」であることに注意
	 */
	ltTxOnOff.onoff_re[lwCpriNo - 1] = (lcReOnOffSta == 1) ? CMD_OFF : CMD_ON;
	
	f_cmn_com_tx_onoff_set(&ltTxOnOff);
	
    /* 停波設定/解除 */
    m_rc_SetWave();

	/* Build 外部装置データ受信要求送信 */
	m_rc_ExtDataRcvBuildData(parm_p, &response_p, SUBRE_ONOFF_RES_SIG, lwSigsys_rslt);
	
	/* 外部装置データ受信要求送信(状態報告信号) */
	m_rc_ExtDataRcvReqExe(&response_p, RCD_EXTRCVREQ_REONOFF, lwSigsys_rslt);

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
}

/* @} */


/* @} */

