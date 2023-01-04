/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_L2LinkStateProc.c
*   @brief  l2 link state notice handle function
*   @date 2015/08/04 TDIPS)ikeda create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  /* CPRI管理 task header file */
#include "rrhApi_L2.h"
#include "f_rrh_reg_cpri.h"
#include "f_rrh_reg_cm.h"
/* @{ */
/**
* @brief  l2 link state notice handle function
* @note  perform CPRI link disconnection processing.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date   2015/7/30 TDIPS)ikeda rev.27665 L3 17リンク対応
* @date   2015/09/03 TDIPS)ikeda 安定化追加
* @date   2015/10/22 TDI)satou その他未実装-007 LED設定を追加
* @date 2015/10/30 TDI)ikeda IT3 問処番号No.43対応
*/

VOID m_cr_L2LinkStateProcRE(UINT * buff_adr)
 {
	USHORT			a_prioind;	/* CPRI state check priority indication */
	T_API_L2_LINKSTATE_CHG_NTC	*apiNtc_pt;
	USHORT			a_link_num;
	CRT_HC_CTL_MST_TBL* hctbl;
	UINT regVal;
	USHORT system_type;
	USHORT system[CMD_MAX_SYSNUM] = {CMD_SYS_S3G, CMD_SYS_3G};

	apiNtc_pt =  (T_API_L2_LINKSTATE_CHG_NTC*)buff_adr;

	a_link_num = apiNtc_pt->data.link_num;

	if( apiNtc_pt->data.state == D_API_L2_STACHG_EST )
	{
		cm_Assert( CMD_ASL_DBGLOW, (a_link_num << 16) | apiNtc_pt->data.state,
					"l2 state mng change est");

		/* check whether need to process this event or not  */
		m_cr_CpriStaPriorityChk(a_link_num, CRD_LINKEV_L2ESTIND, CMD_NUM0, CMD_NUM0, &a_prioind);

		if(a_prioind == CRD_STAPRIO_HIGH)
		{
			/* CPRI State設定 */
			f_cmn_com_cpri_state_set_cprist(a_link_num,
					D_RRH_CPRISTAT_F,  D_RRH_ON,  0, 0);

	        /* LED設定 */
	        m_cm_WriteLEDReg();

			/* 安定化OFF */
			m_cr_CpriDscstTimer(a_link_num, CMD_OFF);

			/* システム数分繰り返し(必ずS3Gから開始すること!!!) */
			for ( system_type = 0; system_type < CMD_MAX_SYSNUM; system_type++ )
			{
				/* LTE単独の場合は3Gはヘルスチェックを行わない */
				if((cmw_connectinfo == CMD_CONN_S3G_SINGLE)&&(system[system_type] == CMD_SYS_3G))
				{
					continue;
				}
				/* ヘルスチェックテーブル */
				hctbl = &crw_hc_ctl_mst_tbl[a_link_num][system[system_type]];

				if (hctbl->hlc_srtupsts == CRD_RECSTATE_STOP)
				{
					/* HDLC統計情報クリア ★ON設定をCPRI起動で実施している */
					BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
							M_RRH_REG_CM_LAERR1(a_link_num), &regVal);
					BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
							M_RRH_REG_CM_LAERR2(a_link_num), &regVal);
					BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ_CPRI,
							M_RRH_REG_CM_LAERR3(a_link_num), &regVal);

					/* ヘルチェック送信開始 */
					m_cr_L3HcSta(a_link_num, system[system_type]);
				}
			}
		}
	}
	else if(apiNtc_pt->data.state == D_API_L2_STACHG_REL)
	{
		cm_Assert( CMD_ASL_DBGLOW, (a_link_num << 16) | apiNtc_pt->data.state,
					"l2 state mng change rel");

		/* check whether need to process this event or not */
		m_cr_CpriStaPriorityChk (a_link_num, CRD_LINKEV_L2RELIND, CMD_NUM0, CMD_NUM0, &a_prioind);

		if(a_prioind == CRD_STAPRIO_HIGH)
		{
			if (cmw_cprilnkstatbl[a_link_num].cpridscstbflg == CMD_OFF)
			{
				/* 安定化ON */
				m_cr_CpriDscstTimer(a_link_num, CMD_ON);
			}
			if ( cmw_connectinfo == CMD_CONN_CPRI_SHARED )
			{
				/*stop HC and notice it to mnt processs */
				m_cr_cm_lnkDisNtcRE(a_link_num, CMD_SYS_3G);
			}
			/*stop HC and notice it to mnt processs */
			m_cr_cm_lnkDisNtcRE(a_link_num, CMD_SYS_S3G);

			/* retry L2 link establishment */
			m_cr_DlinkSettingReq(a_link_num);
		}
		else if(a_prioind == CRD_STAPRIO_SAME)
		{
			/* retry L2 link establishment */
			m_cr_DlinkSettingReq(a_link_num);
		}
		else
		{
			/*	nop	*/
		}
	}
	else
	{
		cm_Assert( CMD_ASL_DBGLOW, (a_link_num << 16) | apiNtc_pt->data.state,
					"l2 state mng change abn");

		/* check whether need to process this state change or no */
		m_cr_CpriStaPriorityChk (a_link_num, CRD_LINKEV_TXABNM, CMD_NUM0, CMD_NUM0, &a_prioind);
		if (a_prioind == CRD_STAPRIO_HIGH)
		{
			/* edit event 「L2 DEACT依頼」 and send it to LAPB task*/
			m_cr_L2DeactReq(a_link_num);

			/* 安定化OFF */
			m_cr_CpriDscstTimer(a_link_num, CMD_OFF);

			if ( cmw_connectinfo == CMD_CONN_CPRI_SHARED )
			{
				/*stop HC and notice it to mnt processs */
				m_cr_cm_lnkDisNtcRE(a_link_num, CMD_SYS_3G);
				/* 切断通知 */
				m_cr_CpriDisConnectNtc(a_link_num, CMD_SYS_3G, E_RRH_RAYER_KIND_1_2);
			}
			/*stop HC and notice it to mnt processs */
			m_cr_cm_lnkDisNtcRE(a_link_num, CMD_SYS_S3G);
			/* 切断通知 */
			m_cr_CpriDisConnectNtc(a_link_num, CMD_SYS_S3G, E_RRH_RAYER_KIND_1_2);
		}
	}

	return;	
 }


/* @} */

/* @{ */
/**
* @brief  安定化タイマー設定
* @note  安定化タイマーをON/OFFする
* @param linkno [in] CPRIリンク番号
* @param flg [in] CMD_ON/CMD_OFF
* @return None
* @date 2015/9/3 TDIPS)ikeda create
  *
*/
VOID m_cr_CpriDscstTimer(USHORT linkno, UINT flg)
{
	USHORT		a_tim_val	= CMD_NUM0;

	int errcd;
	if (flg == CMD_OFF)
	{
		if(cmw_cprilnkstatbl[linkno].cpridscstbflg == CMD_ON)
		{
			/* 安定化タイマーOFF */
			if( cm_TStop( M_RRH_TIMID_CPRIDSCST_FLG(linkno), &errcd )  != CMD_RES_OK)
				cm_Assert( CMD_ASL_DBGLOW, M_RRH_TIMID_CPRIDSCST_FLG(linkno),
							 "dscst timer stop NG" );
		}
		cmw_cprilnkstatbl[linkno].cpridscstbflg = CMD_OFF;
	}
	else
	{
		/* L2 なのでS3G のCPRI安定化時間 */
		a_tim_val = cmw_sys_mng_tbl[CMD_SYS_S3G].cpri_sttimer;

		/* 安定化タイマーON */
		if( cm_TStat( M_RRH_TIMID_CPRIDSCST_FLG(linkno), TIMVAL_GET( a_tim_val ), CMD_TIMMD_SINGLE,
				CMD_TSKIF_CPRIDSCSTNTC,CMD_TSKID_CPR,(INT*)&errcd))
			cm_Assert( CMD_ASL_DBGLOW, M_RRH_TIMID_CPRIDSCST_FLG(linkno),
						"dscst timer start NG" );
		cmw_cprilnkstatbl[linkno].cpridscstbflg = CMD_ON;
	}

}

/* @} */


/* @{ */
/**
* @brief  安定化タイマーT.O
* @note  安定化タイマータイムアウト処理
* @param buff_adr [IN] 受信メッセージ
* @return None
* @date 2015/9/3 TDIPS)ikeda create
  *
*/
VOID m_cr_CpriDscstTimeoutNtc( UINT * buff_adr )
{
	CMT_TSKIF_HEAD		*head_pt;					/**<  タスク共通ヘッダ		 */
	USHORT linkno;

	/* タイマーIDからリンク番号を計算 */
	head_pt = (CMT_TSKIF_HEAD*) (buff_adr);
	linkno = (head_pt->uiSignalkind - D_RRH_TIMID_CPRIDSCST_FLG) + D_RRH_CPRINO_RE_MIN;

	cm_Assert( CMD_ASL_DBGLOW, (linkno << 16),
				"l2 state mng timeout");

	if ( cmw_connectinfo == CMD_CONN_CPRI_SHARED )
	{
		/* 切断通知 */
		m_cr_CpriDisConnectNtc(linkno, CMD_SYS_3G, E_RRH_RAYER_KIND_1_2);
	}
	/* 切断通知 */
	m_cr_CpriDisConnectNtc(linkno, CMD_SYS_S3G, E_RRH_RAYER_KIND_1_2);

	cmw_cprilnkstatbl[linkno].cpridscstbflg = CMD_OFF;
}

/* @} */

/* @} */
