/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_UnderReResetCont.c
 *  @brief  配下REリセット
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
 *  @brief  外部装置データ送信要求(配下REリセット)処理
 *  @param  parm_p [in] the buffer address pointer of received message
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01738
 *  @Bug_No M-RRU-ZSYS-01747
 *  @date   2015/08/04 FPT)Viet Create new   
 *  @date   2015/09/30 TDI)satou M-RRU-ZSYS-01738 外部装置データ送信応答が必ずNG応答になる不具合の修正
 *  @date   2015/10/02 FPT)Hai fix bug IT1
 *  @date   2015/10/05 TDIPS)sasaki M-RRU-ZSYS-01747 リセットされて再起動した配下REの最新RE状態を取得していない不具合を修正
 *  @date   2015/10/17 TDI)satou FHM技説-QA-045 CPRI State C未満の場合は破棄するように
 *  @date   2015/10/19 TDI)satou ハソ-QA-036 強制リセットをPFプロセスに依頼するように
 *  @date   2015/10/23 TDI)satou FHM技説-設計変更-020
 */
/********************************************************************************************************************/
VOID m_rc_UnderReResetCont( CMT_TSKIF_CPRIRCV_EXTDATSNDREQ* parm_p )
{
	USHORT 						lwDevnum;
	UINT						lwSigchk_rslt;					/* result of checking signal  */
	USHORT						lwSigsys_rslt;					/* system by checking signal  */
	T_RRH_CPRI_STATE            shmCpriState;                   /* CPRI state (shared memory) */
	INT                         ret;

    #ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
    #endif
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
	
	/* Eフィールド(装置番号)にて対象REを指定する(1～16) */
	lwDevnum = D_L3_CNVBCD_DEC(parm_p->cpridat_extdatsndreq.snd_msg[CMD_NUM5]);

	/*
	 * 共有メモリからCPRI Stateを取得し、対象REのCPRI Stateを判定する.
	 * CPRI StateがC未満の場合はリセットせずに処理終了(リセット制御を破棄).
	 */
	ret = f_cmn_com_cpri_state_get(&shmCpriState);
	if (D_RRH_OK != ret)
	{
        /* アサート出力処理 */
        cm_Assert(CMD_ASL_USELOW, ret, "f_cmn_com_cpri_state_get NG");
        return;
	}

	if (D_RRH_CPRISTAT_C > shmCpriState.cprista_re[lwDevnum - 1])
	{
	    return;
	}

	/* 該当配下REを強制リセット */
	rrhApi_Svp_Mnt_ReL1Reset(D_RRH_PROCQUE_L3, lwDevnum);

	/* 該当配下REリセット状態：リセット中 */
	gw_rcw_reRstSta[lwSigsys_rslt][lwDevnum - 1] = CMD_ON;
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  配下RE起動完了した後、配下REリセット報告処理を行う
 *  @note 
 *  @param  awReNo [in] RE番号
 *  @param  awSysType [in] Systerm type
 *  @return None
 *  @Bug_No M-RRU-ZSYS-01747
 *  @date   2015/08/04 FPT)Viet Create new   
 *  @date   2015/10/05 TDIPS)sasaki M-RRU-ZSYS-01747 リセットされて再起動した配下REの最新RE状態を取得していない不具合を修正
 *  @date   2016/04/25 FJT)koshida 3001-15B-TS123対応(3G/LTE個別にTILT-INF信号の変化情報を作成する)
 */
/********************************************************************************************************************/
VOID m_rc_UnderReResetRep( USHORT awReNo, USHORT awSysType )
{
	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start(reNo:%d, Systype:%d) \n",__LINE__,__FUNCTION__,awReNo,awSysType);
	#endif
	
	CMT_TSKIF_CPRIRCV_EXTDATRCVREQ 		response_p;
	CMT_TSKIF_CPRIRCV_EXTDATSNDREQ		parm;

	/*  */
	parm.cpridat_extdatsndreq.snd_msg[CMD_NUM0] = CMD_NUM1;
	parm.cpridat_extdatsndreq.snd_msg[CMD_NUM3] = CMD_NUM0;
	parm.cpridat_extdatsndreq.snd_msg[CMD_NUM5] = D_L3_CNVBCD(awReNo);

	/* Build 外部装置データ受信要求送信 */
	m_rc_ExtDataRcvBuildData(&parm, &response_p, SUBRE_RESET_RES_SIG, awSysType);

	/* 該当配下REリセット状態：リセットなし */
	gw_rcw_reRstSta[awSysType][awReNo - DEV_NUM_MIN] = CMD_OFF;

	/* 外部装置データ受信要求送信(状態報告信号) */
	m_rc_ExtDataRcvReqExe(&response_p, RCD_EXTRCVREQ_RERST, awSysType);

	#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s end \n",__LINE__,__FUNCTION__);
	#endif
}

/* @} */

/* @} */

