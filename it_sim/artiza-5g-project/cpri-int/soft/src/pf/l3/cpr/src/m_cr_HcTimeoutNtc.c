/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cr_HcTimeoutNtc.c
 *  @brief  Health check T.O notice handle  function
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221
 *  @date   2013/10/07 FJT)Matsumoto 13B-03-005
 *  @date   2013/11/27 FFCS)niemsh modify for zynq
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"
#include "m_cr_header.h"
#include "rrhApi_L2.h"

/**
* @brief Health check confirm T.O notice handle  function
* @note T.O count vaule add up, perform L3 link disconnect if T.O count value over the limitation.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221
* @date 2013/11/27 FFCS)niemsh modify for zynq
* @date 2015/07/30 FFCS)ikeda 17link対応
* @date 2015/09/04 TDI)satou SV制御レジスタ設定値をFHM用に変更
* @date 2015/09/26 TDIPS)sasaki 共有メモリ上のL3状態更新処理を追加
*/

VOID m_cr_HcTimeoutNtc( UINT * buff_adr )
{
	UINT		a_rtn		= CMD_RES_OK;
	USHORT		a_prt_val	= CMD_NUM0;
	USHORT		a_tim_val	= CMD_NUM0;
	UINT		errcd;
	CMT_TSKIF_HEAD		*head_pt;					/**<  タスク共通ヘッダ		 */
	UINT		a_system_type;
	USHORT		linkno;
	UINT        svRegAdr;                           /* SV制御レジスタアドレス */
	UINT        svRegDat;                           /* SV制御レジスタ設定値 */

	head_pt = (CMT_TSKIF_HEAD*) (buff_adr);
	linkno = D_RRH_CPRINO_REC;
	if(head_pt->uiSrcPQueueID == CMD_TIMID_HCREQ_CNF_S3G) {
	    a_system_type = CMD_SYS_S3G;
	    svRegAdr = M_RRH_REG_CNT_S3G_L3_ERR_ADR(linkno);
	    svRegDat = M_RRH_REG_CNT_S3G_L3_ERR(linkno);
	} else {
	    a_system_type = CMD_SYS_3G;
	    svRegAdr = M_RRH_REG_CNT_3G_L3_ERR_ADR(linkno);
	    svRegDat = M_RRH_REG_CNT_3G_L3_ERR(linkno);
	}

	/* if crw_hc_ctl_tbl health check status is idle */
	if (crw_hc_ctl_tbl[a_system_type].re_state == CRD_RESTATE_IDLE)
	{
		return;
	}

	/* T.O count */
	crw_hc_ctl_tbl[a_system_type].to_count++;

	cm_Assert( CMD_ASL_DBGLOW, crw_hc_ctl_tbl[a_system_type].to_count,
				"Health check confirm timer T.O" );

	/* get system table health check unconf count */
	a_prt_val = cmw_sys_mng_tbl[a_system_type].hls_unconfcnt;

	/***************************************************/
	/* T.O count check */
	/***************************************************/
	if( crw_hc_ctl_tbl[a_system_type].to_count <= a_prt_val )
	{
		/* get health check timer value from system table */
		a_tim_val = cmw_sys_mng_tbl[a_system_type].hls_timer;

		/* health check timer start */
		a_rtn = cm_TStat( crr_hctimno[a_system_type], TIMVAL_GET( a_tim_val ), CMD_TIMMD_SINGLE,
					CMD_TSKIF_TIMOUTNTC,CMD_TSKID_CPR,(INT*)&errcd);
		if( a_rtn != CMD_RES_OK)
		{
			cm_Assert( CMD_ASL_DBGLOW, crr_hctimno[a_system_type],
						"Health check confirm timer start NG" );
		}
	}
	/***************************************************/
	/* T.O count over */
	/***************************************************/
	else
	{
		cm_Assert(CMD_ASL_USELOW,a_system_type,"L3 Health Check T.O");

		/* L3 CPRI ERR register Write */
		m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);

		/* set crw_hc_ctl_tbl RE status to idle */
		crw_hc_ctl_tbl[a_system_type].re_state = CRD_RESTATE_IDLE;
		f_cmn_com_layer3_set_each(D_RRH_CPRINO_REC, a_system_type, E_RRH_LAYER3_REC_IDLE);

		cmw_hcsta[linkno][a_system_type] = CMD_OFF;

		m_cr_HcStateChgNtc(linkno, a_system_type,CMD_OFF);

		/*	REC<->FHMのHC NGのみログを退避する	*/
		if(linkno == D_RRH_CPRINO_REC)
		{
			/* Soft Alarm発生をINITプロセスに通知	*/
#if 0
//			BPF_RM_SVRM_SIGNAL_SEND(	D_RRH_PROCID_INI,					/* 送信先プロセスID			*/
//										SIGUSR1,							/* 送信するSignal			*/
//										D_RRH_SIGUSR_OPT_RSET_HLT_DISC );	/* オプション				*/
#endif
		}
	}

	return ;
}

/**
 * @brief   ヘルスチェック状態の変化を通知する
 * @param   linkno        [in] CPRIリンク番号
 * @param   a_system_type [in] 3G/LTE
 * @param   onOff         [in] 確立/切断
 * @date    2015/10/21 TDI)satou その他未実装-004
 * @date    2015/10/22 TDI)satou その他未実装-007 LED設定処理を追加
 * @date    2015/10/24 TDIPS)sasaki Warning対処
 */
VOID m_cr_HcStateChgNtc(USHORT linkno, USHORT a_system_type,USHORT onOff )
{
	USHORT prioind;

	m_cr_cm_L3LnkStaNtc(linkno, a_system_type,onOff);

	if(onOff == CMD_ON)
	{
		m_cr_CpriStaPriorityChk(linkno, CRD_LINKEV_L3HCOK, 0, a_system_type, &prioind);

		f_cmn_com_cpri_state_set_hc(linkno, a_system_type, CMD_ON);
	}
	else
	{
		m_cr_CpriStaPriorityChk(linkno, CRD_LINKEV_L3HCDSC, 0, a_system_type, &prioind);

		f_cmn_com_cpri_state_set_hc(linkno, a_system_type, CMD_OFF);

		/*call API to request the switch of L3 on/off history */
		rrhApi_L2_Mnt_CpriMsgStatistic(linkno, a_system_type, D_RRH_PROCID_Q_L2,0,D_RRH_NULL);

		if (linkno < D_RRH_CPRINO_RE_MIN)
		{
            /* 切断通知 */
            m_cr_CpriDisConnectNtc(linkno, a_system_type, E_RRH_RAYER_KIND_3);
		}
	}

    /* LED設定 */
    m_cm_WriteLEDReg();

	return;
}
/* @} */

