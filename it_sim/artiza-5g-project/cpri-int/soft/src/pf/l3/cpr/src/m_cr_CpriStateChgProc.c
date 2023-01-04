/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_CpriStateChgProc.c
 *  @brief  CPRI state change interruption handle function
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00054-005(CPRI spec V1.0.8) 
 *  @date   2009/10/08 QNET)Kabasima MOD CR-00071-001(TRA-HS-V2R11 RE AUTO RESET and 3GRF-INF)
 *  @date   2009/10/14 QNET)Kabasima HISTORY ADD CR-00064-001(TRA-HS-V2R9 HDLC RATE CHANGE IRQ)
 *  @date   2009/12/27 QNET)Kabasima M-S3G-eNBPF-02862:[3G水平展開]OPTTX5とMDEIF直結にて
 *                          OPTTX5の光ON/OFFを繰り返すと、MDEIFに対してinbandリセット発行する
 *  @date   2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
 *  @date   2012/07/05  FFCS)niemsh modify for 1.5GVA
 *  @date   2015/07/30 TDIPS)ikeda 17link対応 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"			/* CPRI management task header file */
#include "f_rrh_def.h"
#include "rrhApi_Cpri.h"
#include "f_rrh_reg_eep.h"

/**
* @brief CPRI state change interruption handle function
* @note CPRI state change interruption handle function.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2009/12/27 QNET)Kabasima M-S3G-eNBPF-02862:[3G水平展開]OPTTX5とMDEIF直結にて
*                       OPTTX5の光ON/OFFを繰り返すと、MDEIFに対してinbandリセット発行する
* @date 2010/03/02 QNET)Kabasima MOD M-S3G-eNBPF-03221:CPRI共用からLTE単独構成変更時にREのレジスタに3G-L3ERRが残る場合がある
* @date 2012/07/05  FFCS)niemsh modify for 1.5GVA
* @date 2014/12/15  FFCS)fuxg modify for ppcEOL
* @date 2015/04/20  FFCS)fuxg modify for 3.5G TD-SRE
* @date 2015/07/27  TDIPS)ikeda 17link対応*
* @date 2015/09/03  TDIPS)ikeda 遷移追加
* @date 2015/09/04  TDI)satou LTE単独構成の場合、3G L3 ERRをクリアするように処理を追加
* @date 2015/10/22  TDI)satou その他未実装-007 LED設定を追加
* @date 2015/11/13  TDI)satou その他未実装-013 「RCTにCPRI再起動依頼」「L2 link disconnect request notice」追加
*/
VOID m_cr_CpriStateChgProc(UINT * buff_adr)
 {
	UINT						a_cpirLineBitrate;
	UINT						a_cpriHdlc;
	UINT						a_cpriEDown;
	UINT						a_cpriState;
	T_API_CPRILINK_STATE_NTC*	apiNtc_p;	/* input IF pointer */

	USHORT 						a_prioind;	/* CPRI state check priority indication */
	UINT                        svRegAdr;   /* SV制御レジスタアドレス */
	UINT                        svRegDat;   /* SV制御レジスタ設定値 */
	UINT                        linkno;

	/* get CPRI state from IF */
	apiNtc_p = (T_API_CPRILINK_STATE_NTC*)buff_adr;
 	if (apiNtc_p->link_num != D_RRH_CPRINO_REC)
	{
		m_cr_CpriStateChgProcRE(buff_adr);
		return;
	}
	a_cpirLineBitrate = apiNtc_p->cpriLineBitrate;
	a_cpriHdlc = apiNtc_p->cpriHdlc;
	a_cpriEDown = apiNtc_p->cpriEDown ;
	a_cpriState = apiNtc_p->cpriState  ;

	BPF_COM_PLOG_TRACE_CPRI(E_TRC_LV_INFO, 
						"cpirRate = %d, hdlcRate = 0x%x, cpriHdlc = %d, cpriEDown = %d, cpriState = %d", 
						a_cpirLineBitrate,
						apiNtc_p->cpriHDLCBitrate,
						a_cpriHdlc,
						a_cpriEDown,
						a_cpriState);
	
	/* if HDLC ACT or State E(HDLC IDLE & E Down off) */
	if( (a_cpriHdlc == D_RRH_ON) || 
		(a_cpriState == D_RRH_CPRISTAT_E) )
	{
		/* judge LINE BIT RATE value :0 : 4.9Gbps, 1: 9.8Gbps */
		if (a_cpirLineBitrate == D_RRH_LINE_BITRATE_24G)
		{
			cmw_linebitrate = CMD_LINE_BITRATE_24G;
		}
		else if (a_cpirLineBitrate == D_RRH_LINE_BITRATE_49G)
		{
			cmw_linebitrate = CMD_LINE_BITRATE_49G;
		}
		else if(a_cpirLineBitrate == D_RRH_LINE_BITRATE_98G)
		{
			cmw_linebitrate = CMD_LINE_BITRATE_98G;
		}
		else
		{
			cm_Assert(CMD_ASL_DBGLOW,
						a_cpirLineBitrate,
						(UCHAR*)"System Abnormal");
			return;
		}

		/* 有効システム */
		if (cmw_linebitrate == CMD_LINE_BITRATE_24G)
		{
			if ((apiNtc_p->cpriHDLCBitrate & D_RRH_HDLC_RATE_MASK) == D_RRH_HDLC_RATE_1920)
			{
				cm_Assert(CMD_ASL_DBGLOW,
						apiNtc_p->cpriHDLCBitrate,
						(UCHAR*)"Slave CPRI=2.4G/3G S3G COMMON(hdlcRate)");
				cmw_validsysinfo = CMD_SYS_BOTH_3G_S3G;
				cmw_connectinfo = CMD_CONN_CPRI_SHARED;
			}
			else
			{
				cm_Assert(CMD_ASL_DBGLOW,
						apiNtc_p->cpriHDLCBitrate,
						(UCHAR*)"Slave CPRI=2.4G/S3G SINGLE(hdlcRate)");
				cmw_validsysinfo = CMD_SYS_S3G;
				cmw_connectinfo = CMD_CONN_S3G_SINGLE;
			}
		}
		else
		{
			if ((apiNtc_p->cpriHDLCBitrate & D_RRH_HDLC_RATE_MASK) == D_RRH_HDLC_RATE_1920)
			{
				cm_Assert(CMD_ASL_DBGLOW,
							apiNtc_p->cpriHDLCBitrate,
							(UCHAR*)"System Abnormal");
			}
			cm_Assert(CMD_ASL_DBGLOW,
						a_cpirLineBitrate,
						(UCHAR*)"Slave CPRI=9.8Gor4.9G/S3G SINGLE(cpriRate)");
			cmw_validsysinfo = CMD_SYS_S3G;
			cmw_connectinfo = CMD_CONN_S3G_SINGLE;
		}

		/* LTE単独構成の場合、3G L3 ERRを解除する(3Gのヘルスチェックは来ない) */
		if (CMD_CONN_S3G_SINGLE == cmw_connectinfo)
		{
			svRegAdr = M_RRH_REG_CNT_3G_L3_ERR_ADR(D_RRH_CPRINO_REC);
			svRegDat = M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_REC);
			m_cm_ErrCntRegSet(CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_NCY);
			
		    for(linkno=D_RRH_CPRINO_RE_MIN; linkno<D_RRH_CPRINO_NUM; linkno++)
		    {
			    svRegAdr = M_RRH_REG_CNT_3G_L3_ERR_ADR(linkno);
			    svRegDat = M_RRH_REG_CNT_3G_L3_ERR(linkno);
			    
			    m_cm_ErrCntRegSetRE(linkno, CMD_CLR, svRegAdr, svRegDat, CMD_CHKALM_UNNCY);
			}
		}
	}

	/* CPRI State設定 */
	f_cmn_com_cpri_state_set_cprist(D_RRH_CPRINO_REC,
			a_cpriState,  a_cpriHdlc,  a_cpirLineBitrate, cmw_connectinfo);

    /* LED設定 */
    m_cm_WriteLEDReg();

	/* case 1: change to HDLC ACT state */
	if ( a_cpriHdlc == D_RRH_ON)  
	{ 
		/* check whether need to process this state change or not */
		 m_cr_CpriStaPriorityChk(D_RRH_CPRINO_REC, CRD_LINKEV_HDLCACT, CMD_NUM0, CMD_NUM0, &a_prioind);
		if (a_prioind == CRD_STAPRIO_HIGH)
		{
			/* RCTにPrepare応答メッセージ送信  */
			m_cr_CpriLinkPrepareRes();

			/* set task state to 3: re startup */
			crw_taskstate = CRD_TASKSTA_RE_STARTUP;
	        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "REC<->FHM L1 EST");
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
			m_cr_CpriStaPriorityChk(D_RRH_CPRINO_REC, CRD_LINKEV_STAEDWN,a_cpriState,CMD_NUM0,&a_prioind);
		}
		else
		{
			m_cr_CpriStaPriorityChk(D_RRH_CPRINO_REC, CRD_LINKEV_HDLCIDLE,CMD_NUM0,CMD_NUM0,&a_prioind);
		}

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

			if (D_RRH_ON == a_cpriEDown)
			{
                /* L2 link disconnect request notice */
                m_cr_DlinkStopReq(D_RRH_CPRINO_REC);

                /* RCTにCPRI再起動依頼 */
                m_cr_CpriRestartNtc();
			}

			/* set task state to RECStateE待ち */
			crw_taskstate = CRD_TASKSTA_REC_ACTIVE;
	        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, "REC<->FHM L1 DWN");
		}
		
		return;
	}

	/* case 3: change to stateE (from stateA / B/ C /D) */
	if ( a_cpriState == D_RRH_CPRISTAT_E )
	{
		/* update CPRI link state to CPRI state table */
		 m_cr_CpriStaPriorityChk(D_RRH_CPRINO_REC, CRD_LINKEV_STAEDWNTOE,CMD_NUM0,CMD_NUM0,&a_prioind);

		return;
	}
	else
	/* case 4: change among stateB / C / D*/
	{
		/* only update CPRI state*/	
		cmw_cprilnkstatbl[D_RRH_CPRINO_REC].cprista = a_cpriState; 
	}

	return;
 }

/**
* @brief CPRI link disconnection notification sending function
* @note edit CPRI link disconnection notification and send to RSV/MTM task.\n
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2015/07/30 TDIPS)ikeda 17link対応
* @date 2015/09/04 TDI)satou SV制御レジスタ設定値をFHM用に変更
* @date 2015/09/26 TDIPD)sasaki 共有メモリ上のL3状態更新処理を追加
* @date 2015/10/21 TDI)satou その他未実装-004 TRX解放処理を削除。リンク断通知の一環で処理するので不要になった
* @date 2015/10/24 TDIPS)sasaki Warning対処
*/

VOID m_cr_cm_lnkDisNtc(USHORT system_type)
{
/*	UINT						a_ret; */
/*	INT							buf_ret; */
	INT							errcd;
/*	CMT_TSKIF_CPRIDSCNTC*		a_cpridiscntc_p; */
	UINT                        svRegAdr;               /* SV制御レジスタアドレス */
	UINT                        svRegDat;               /* SV制御レジスタ設定値 */

	if (CMD_SYS_S3G == system_type) {
	    svRegAdr = M_RRH_REG_CNT_S3G_L3_ERR_ADR(D_RRH_CPRINO_REC);
	    svRegDat = M_RRH_REG_CNT_S3G_L3_ERR(D_RRH_CPRINO_REC);
	} else {
	    svRegAdr = M_RRH_REG_CNT_3G_L3_ERR_ADR(D_RRH_CPRINO_REC);
	    svRegDat = M_RRH_REG_CNT_3G_L3_ERR(D_RRH_CPRINO_REC);
	}

	/* if L3 Link set up, stop health check timer */
	if(crw_hc_ctl_tbl[system_type].re_state != CRD_RESTATE_IDLE)
	{
		/* ヘルスチェック要求確認タイマを停止する */
		if( cm_TStop( crr_hctimno[system_type],&errcd )  != CMD_RES_OK)
	 		cm_Assert( CMD_ASL_DBGLOW, crr_hctimno[system_type],
						 "Health check confirm timer stop NG" );
	 	
		/* 状態を"REアイドル状態"に遷移させる */
		crw_hc_ctl_tbl[system_type].re_state = CRD_RESTATE_IDLE;
		f_cmn_com_layer3_set_each(D_RRH_CPRINO_REC, system_type, E_RRH_LAYER3_REC_IDLE);

		/* タイムアウトカウント値をクリア */
		crw_hc_ctl_tbl[system_type].to_count = 0;
		
		cmw_hcsta[D_RRH_CPRINO_REC][system_type] = CMD_OFF;
	}
	
	/* L3リンク断処理を実施する */
	m_cr_cm_L3LnkStaNtc(D_RRH_CPRINO_REC, system_type, CMD_OFF);
	
	/* L3 CPRIリンク断をレジスタWrite */
	m_cm_ErrCntRegSet(CMD_SET, svRegAdr, svRegDat, CMD_CHKALM_NCY);
#if 0
	a_ret = cm_BReq(CMD_BUFCA_TSKIF, sizeof(CMT_TSKIF_CPRIDSCNTC),(void**)&a_cpridiscntc_p);
	if (a_ret  != CMD_RES_OK)
	{
		cm_MAbort(CMD_ALMCD_BUFGET, (CHAR *)"m_cr_cm_lnkDisNtc", 
			(CHAR *)"buffer hunt fail 1", a_ret, sizeof(CMT_TSKIF_CPRIDSCNTC), CMD_NUM0);
	}
	
	a_cpridiscntc_p->head.uiLength = sizeof(CMT_TSKIF_CPRIDSCNTC) ;
	a_cpridiscntc_p->head.uiEventNo = CMD_TSKIF_CPRIDSCNTC;
	a_cpridiscntc_p->head.uiSrcPQueueID = D_RRH_PROCQUE_L3;
	a_cpridiscntc_p->head.uiSrcTQueueID = CMD_TSKID_CPR;
	a_cpridiscntc_p->head.uiDstPQueueID = D_RRH_PROCQUE_MT;
	a_cpridiscntc_p->head.uiDstTQueueID = 0;
	a_cpridiscntc_p->cpri_no = D_RRH_CPRINO_REC;
	a_cpridiscntc_p->dislink_inf = CMD_DSCLINK_INF_L1DSC;
	a_cpridiscntc_p->system_type = system_type;

	/* Send Log */
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, D_RRH_PROCQUE_MT, CMD_TSKIF_CPRIDSCNTC, (UCHAR*)a_cpridiscntc_p);

	buf_ret = cm_Enter(D_RRH_PROCQUE_MT,CMD_QRB_NORMAL,CMD_TSKIF_CPRIDSCNTC,a_cpridiscntc_p);
	if(buf_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, buf_ret, "cm_Enter NG" );
	}
#endif
	return;
}
/* @} */



