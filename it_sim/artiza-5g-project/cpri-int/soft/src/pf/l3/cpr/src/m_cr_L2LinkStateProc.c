/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_L2LinkStateProc.c
*   @brief  l2 link state notice handle function
*   @date 2013/11/18 FFCS)niemsh create
*   @date 2015/08/05 TDIPS)ikeda
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  /* CPRI管理 task header file */
#include "rrhApi_L2.h"

/**
* @brief  l2 link state notice handle function
* @note  perform CPRI link disconnection processing.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2013/11/18 FFCS)niemsh create
* @date 2015/08/05 TDIPS)ikeda
* @date 2015/10/11 TDI)satou 共有メモリのState F設定を追加
* @date 2015/10/22 TDI)satou その他未実装-007 LED設定を追加
*/

VOID m_cr_L2LinkStateProc(UINT * buff_adr)
 {
	USHORT			a_prioind;	/* CPRI state check priority indication */
	T_API_L2_LINKSTATE_CHG_NTC	*apiNtc_pt;
	T_RRH_CPRI_STATE shmCpriSt;

	apiNtc_pt =  (T_API_L2_LINKSTATE_CHG_NTC*)buff_adr;
 	if (apiNtc_pt->data.link_num != D_RRH_CPRINO_REC)
 	{
 		m_cr_L2LinkStateProcRE(buff_adr);
		return;
 	}
	cm_Assert( CMD_ASL_DBGLOW, apiNtc_pt->data.state, 
				"l2 state mng change" );

	if(apiNtc_pt->data.state == D_API_L2_STACHG_EST)
	{
		/* check whether need to process this event or not  */
		m_cr_CpriStaPriorityChk(D_RRH_CPRINO_REC, CRD_LINKEV_L2ESTIND, CMD_NUM0, CMD_NUM0, &a_prioind);

		/* set state F */
		f_cmn_com_cpri_state_get(&shmCpriSt);
        f_cmn_com_cpri_state_set_cprist(D_RRH_CPRINO_REC, D_RRH_CPRISTAT_F, shmCpriSt.hdlcst_rec, shmCpriSt.cpri_bitrate, shmCpriSt.cpri_linktype);

        /* LED設定 */
        m_cm_WriteLEDReg();
	}
	else if(apiNtc_pt->data.state == D_API_L2_STACHG_REL)
	{
		/* check whether need to process this event or not */	
		m_cr_CpriStaPriorityChk (D_RRH_CPRINO_REC, CRD_LINKEV_L2RELIND, CMD_NUM0, CMD_NUM0, &a_prioind);

		if(a_prioind == CRD_STAPRIO_HIGH)
		{
			if ( cmw_connectinfo == CMD_CONN_CPRI_SHARED )
			{
				/*stop HC and notice it to mnt processs */
				m_cr_cm_lnkDisNtc(CMD_SYS_3G);
				/* RCT にCPRI断通知 */
				m_cr_CpriDisConnectNtc(D_RRH_CPRINO_REC, CMD_SYS_3G, E_RRH_RAYER_KIND_1_2);
			}
			/*stop HC and notice it to mnt processs */
			m_cr_cm_lnkDisNtc(CMD_SYS_S3G);
			/* RCT にCPRI断通知 */
			m_cr_CpriDisConnectNtc(D_RRH_CPRINO_REC, CMD_SYS_S3G, E_RRH_RAYER_KIND_1_2);

			/* retry L2 link establishment */				
			m_cr_DlinkSettingReq(D_RRH_CPRINO_REC);
		}
		else if(a_prioind == CRD_STAPRIO_SAME)
		{
			/* retry L2 link establishment */				
			m_cr_DlinkSettingReq(D_RRH_CPRINO_REC);
		}
		else
		{
			/*	nop	*/
		}
	}
	else
	{
		/* check whether need to process this state change or no */
		m_cr_CpriStaPriorityChk (D_RRH_CPRINO_REC, CRD_LINKEV_TXABNM, CMD_NUM0, CMD_NUM0, &a_prioind);
		if (a_prioind == CRD_STAPRIO_HIGH)
		{
			/* edit event 「L2 DEACT依頼」 and send it to LAPB task*/
			m_cr_L2DeactReq(D_RRH_CPRINO_REC);

			if ( cmw_connectinfo == CMD_CONN_CPRI_SHARED )
			{
				/*stop HC and notice it to mnt processs */
				m_cr_cm_lnkDisNtc(CMD_SYS_3G);
				/* 切断通知 */
				m_cr_CpriDisConnectNtc(D_RRH_CPRINO_REC, CMD_SYS_3G, E_RRH_RAYER_KIND_1_2);
			}
			/*stop HC and notice it to mnt processs */
			m_cr_cm_lnkDisNtc(CMD_SYS_S3G);
			/* 切断通知 */
			m_cr_CpriDisConnectNtc(D_RRH_CPRINO_REC, CMD_SYS_S3G, E_RRH_RAYER_KIND_1_2);
		}
	}

	return;	
 }

VOID m_cr_L2LinkStateProc2(UINT * buff_adr)
 {
	T_API_L2_LINKSTATE_CHG_NTC	*apiNtc_pt;

	apiNtc_pt =  (T_API_L2_LINKSTATE_CHG_NTC*)buff_adr;
 	if (apiNtc_pt->data.link_num != D_RRH_CPRINO_REC)
 	{
 		m_cr_L2LinkStateProcRE(buff_adr);
		return;
 	}
	/*	REC側は既にStateE ACT未満(L1断通知済み)であり、		*/
	/*	配下REが起動完了していないので、REC側はStateE保持	*/
	cm_Assert( CMD_ASL_DBGHIGH, apiNtc_pt->data.state, 
				"l2 state mng change at RE startUp wait" );
	return;	
 }
/* @} */
