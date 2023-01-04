/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_CpriStateChgProc.c
 *  @brief  CPRI state change interruption handle function
 *  @date   2015/07/30 TDIPS)ikeda create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"			/* CPRI management task header file */
#include "f_rrh_def.h"
#include "rrhApi_Cpri.h"
#include "f_rrh_reg.h"
#include "f_rrh_reg_eep.h"
#include "f_rrh_reg_cpri.h"
#include "f_rrh_reg_slcr.h"

/**
* @brief CPRI state change interruption handle function
* @note CPRI state change interruption handle function.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2015/07/30 TDIPS)ikeda create
* @date 2015/7/30 TDIPS)ikeda rev.27665 L3 17リンク対応
* @date 2015/10/22 TDI)satou その他未実装-007 LED設定を追加
*/
VOID m_cr_CpriStateChgProcRE(UINT * buff_adr)
 {
	UINT						a_cpirLineBitrate;
	UINT						a_cpriHdlc;
	UINT						a_cpriEDown;
	UINT						a_cpriState;
	T_API_CPRILINK_STATE_NTC*	apiNtc_p;	/* input IF pointer */
 	USHORT						a_link_num;

	USHORT 						a_prioind;	/* CPRI state check priority indication */

	/* get CPRI state from IF */
	apiNtc_p = (T_API_CPRILINK_STATE_NTC*)buff_adr;
	a_link_num = apiNtc_p->link_num;
 	a_cpirLineBitrate = apiNtc_p->cpriLineBitrate;;
	a_cpriHdlc = apiNtc_p->cpriHdlc;
	a_cpriEDown = apiNtc_p->cpriEDown ;
	a_cpriState = apiNtc_p->cpriState  ;

	BPF_COM_PLOG_TRACE_CPRI(E_TRC_LV_INFO,
						"a_link_num = %d,"
						"a_cpirLineBitrate = %d,"
						"a_cpriHdlc = %d, "
						"a_cpriEDown = %d, "
						"a_cpriState = %d",
						a_link_num,
						a_cpirLineBitrate,
						a_cpriHdlc,
						a_cpriEDown,
						a_cpriState);


	/* CPRI State設定 */
	f_cmn_com_cpri_state_set_cprist(a_link_num,
			a_cpriState,  a_cpriHdlc,  a_cpirLineBitrate, 0);

    /* LED設定 */
    m_cm_WriteLEDReg();

	/* CPRI State 送信 */
	m_cr_CpriStateChgNotice(a_link_num, a_cpriState);

	/* case 1: change to HDLC ACT state */
	if ( a_cpriHdlc == D_RRH_ON)
	{
		/* check whether need to process this state change or not */
		 m_cr_CpriStaPriorityChk(a_link_num, CRD_LINKEV_HDLCACT, CMD_NUM0, CMD_NUM0, &a_prioind);
		if (a_prioind == CRD_STAPRIO_HIGH)
		{
			/* edit event 「CPRI信号送信通知」(データリンク設定要求) and send to LAPB task  */
			m_cr_DlinkSettingReq(a_link_num);
	        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FHM<->RE#%d L1 EST", a_link_num);
		}

		return;
	}

	/*  case 2: change to HDLC IDLE or E down */
	if ((a_cpriHdlc == D_RRH_OFF)  ||
		( a_cpriEDown  == D_RRH_ON ) )
	{
		/* check whether need to process this state change or no */
		if (a_cpriEDown  == D_RRH_ON)
		{
			m_cr_CpriStaPriorityChk(a_link_num, CRD_LINKEV_STAEDWN,a_cpriState,CMD_NUM0,&a_prioind);
		}
		else
		{
			m_cr_CpriStaPriorityChk(a_link_num, CRD_LINKEV_HDLCIDLE,CMD_NUM0,CMD_NUM0,&a_prioind);
		}

		if (a_prioind == CRD_STAPRIO_HIGH)
		{
			/* 安定化タイマ停止 */
			m_cr_CpriDscstTimer(a_link_num, CMD_OFF);

			/* edit event 「L2 DEACT依頼」 and send it to LAPB task*/
			m_cr_L2DeactReq(a_link_num);

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
	        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "FHM<->RE#%d L1 DWN", a_link_num);

		}

		return;
	}

	/* case 3: change to stateE (from stateA / B/ C /D) */
	if ( a_cpriState == D_RRH_CPRISTAT_E )
	{
		/* update CPRI link state to CPRI state table */
		m_cr_CpriStaPriorityChk(a_link_num, CRD_LINKEV_STAEDWNTOE,CMD_NUM0,CMD_NUM0,&a_prioind);

		return;
	}
	else
	/* case 4: change among stateB / C / D*/
	{
		/* only update CPRI state*/
		cmw_cprilnkstatbl[a_link_num].cprista = a_cpriState;
	}

	return;
 }
/**
* @brief CPRI link disconnection notification sending function
* @return None
* @Bug_No M-RRU-ZSYS-01965
* @date 2015/07/30 TDIPS)ikeda create trif.t_lc_L3hcStp
* @date 2015/09/04 TDI)satou SV制御レジスタ設定を追加
* @date 2015/10/07 TDI)satou 「L3 CPRIリンク断をレジスタWrite」はRE接続状態が接続の場合のみ実施する
* @date 2015/11/07 koshida M-RRU-ZSYS-01965 "ヘルスチェック要求未送信"設定先を変更
*/
VOID m_cr_cm_lnkDisNtcRE(USHORT linkno, USHORT a_system_type)
{
	INT							errcd;
	UINT                        svRegAdr;       /* SV制御レジスタアドレス */
	UINT                        svRegDat;       /* SV制御レジスタ設定値 */
	T_RRH_CONN_DISC             shmConnDisc;

	CRT_HC_CTL_MST_TBL* hctbl;

	if (CMD_SYS_S3G == a_system_type) {
	    svRegAdr = M_RRH_REG_CNT_S3G_L3_ERR_ADR(linkno);
	    svRegDat = M_RRH_REG_CNT_S3G_L3_ERR(linkno);
	} else {
	    svRegAdr = M_RRH_REG_CNT_3G_L3_ERR_ADR(linkno);
	    svRegDat = M_RRH_REG_CNT_3G_L3_ERR(linkno);
	}

	/* ヘルスチェックテーブル */
	hctbl = &crw_hc_ctl_mst_tbl[linkno][a_system_type];

	/* 該当CPRIリンクがヘルスチェック起動中の場合 */
	if(hctbl->hlc_srtupsts == CRD_RECSTATE_START)
	{
		/* タイマ停止 */
		if( cm_TStop( M_RRH_TIMID_HCSND_CNF(linkno, a_system_type),&errcd )  != CMD_RES_OK)
	 		cm_Assert( CMD_ASL_DBGLOW, M_RRH_TIMID_HCSND_CNF(linkno, a_system_type),
						 "Health check send timer stop NG" );

		/* ヘルスチェック要求送信を送信中の場合 */
		if(hctbl->hlc_sndsts == CRD_RECSTATE_SENDING)
		{
			/* タイマ停止 */
			if( cm_TStop( M_RRH_TIMID_HCRSP_CNF(linkno, a_system_type),&errcd )  != CMD_RES_OK)
		 		cm_Assert( CMD_ASL_DBGLOW, M_RRH_TIMID_HCRSP_CNF(linkno, a_system_type),
							 "Health check confirm timer stop NG" );
		}
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_INFO, "FHM<->RE#%d L3 Health Check Stop(system=%d)", linkno, a_system_type);

		/* ヘルスチェック起動状態に"ヘルスチェック停止中"を設定 */
		hctbl->hlc_srtupsts = CRD_RECSTATE_STOP;

		/* ヘルスチェック確立状態に"ヘルスチェック未確立(初期値)"を設定 */
		hctbl->hlc_eststs = CRD_RECSTATE_INI;

		/* ヘルスチェック送信状態に"ヘルスチェック要求未送信"を設定 */
		hctbl->hlc_sndsts = CRD_RECSTATE_NOSEND;
		
		/* ヘルスチェック停止(REC/RE共用) */
		cmw_hcsta[linkno][a_system_type] = CMD_OFF;

		/* HC停止共有メモリ設定 */
		f_cmn_com_cpri_state_set_hc(linkno, a_system_type, CMD_OFF);

		/* L3 CPRIリンク断をレジスタWrite */
		f_cmn_com_conn_disk_get(&shmConnDisc);
		if (D_RRH_ON == shmConnDisc.conn_disc[linkno - 1]) {
		    m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
		}
	}
	else
	{
		/* ヘルスチェック停止中の場合、無処理 */
	}
	
	return;
}
/* @} */



