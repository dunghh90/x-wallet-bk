/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_Init.c
 *  @brief  CPRI management task init function
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00054-005(CPRI spec V1.0.8) 
 *  @date   2012/07/05  FFCS)niemsh modify for 1.5GVA
 *  @date   2015/08/04 TDIPS)ikeda
 *  @date   2015/09/26 TDIPS)sasaki update
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */
#include "m_cm_header.h"	/* TRA共通ヘッダファイル		*/
#include "m_cr_header.h"	/* CPRI管理 task header file	*/
#include "f_rrh_def.h"
#include "f_rrh_reg.h"
#include "f_rrh_reg_slcr.h"

/* @{ */
/**
* @brief CPRI management task init function
* @note init CPRI management task related common table.\n
* @param buff_adr [in] the buffer address pointer of received message
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date   2012/07/05  FFCS)niemsh modify for 1.5GVA
* @date   2015/08/04 TDIPS)ikeda
* @date   2015/09/26 TDIPS)sasaki update
* @date   2016/02/05 TDIPS)enoki 配下REのInbandReset対象のFHM起動要因に自身のL1RSTとUnKnownを追加
* @date   2016/02/22 TDIPS)enoki 上記の配下REのInbandReset対象を全REを対象とする
*/

VOID m_cr_Init(UINT * buff_adr)
{
	CMT_TSKIF_HEAD		*head_pt;	/**<  タスク共通ヘッダ		 */
	USHORT		system_type;		/* use as system type index */
	INT			a_rtn;
    UINT regVal;
	
 	crw_hissetflag = CMD_ON;

	cm_MemClr( (void *)cmw_cprilnkstatbl, sizeof (cmw_cprilnkstatbl) );
	
	/* CPRI state common management table init */
	cm_MemClr( (void *)cmw_cprilnkstatbl, sizeof (cmw_cprilnkstatbl) );

	/* health check control table initial */
	cm_MemClr( crw_hc_ctl_tbl, sizeof( crw_hc_ctl_tbl) );
	cm_MemClr( crw_hc_ctl_mst_tbl, sizeof( crw_hc_ctl_mst_tbl) );

	cmw_validsysinfo = CMD_SYS_S3G;

	/* RE  common management table init */
	cm_MemClr( (void *)&cmw_reclsmngtbl, sizeof (cmw_reclsmngtbl) );

	/* read RE equipment info table to get RE type */
	cmw_reclsmngtbl.retype = cmw_re_dev_tbl.re_type;		

	 /* OFTRX case, always active setting */
	for( system_type = CMD_NUM0; system_type < CMD_MAX_SYSNUM; system_type++)
	{
		/* RRE/LRE(15G17G-LRE除く)はActive切替無効装置であるため、テーブルの初期状態をActiveで初期化 */
		cmw_reclsmngtbl.traclsinfo[system_type] = CMD_RECLS_ACTSET;
		/*	RRE/LRE(15G17G-LRE除く)はActive LEDがないため、消灯固定とする	*/
		cmw_reclsmngtbl.actneginfo[system_type] = CMD_RECLS_NEGSET;
	}
	
	/* set self TRA slot number =  1  */
	cmw_reclsmngtbl.selfslotno = CMD_NUM1 ;

	/* Initialize Slave Port L3-State	*/
	f_cmn_com_layer3_set_each(D_RRH_CPRINO_REC, CMD_SYS_3G, E_RRH_LAYER3_REC_IDLE);
	f_cmn_com_layer3_set_each(D_RRH_CPRINO_REC, CMD_SYS_S3G, E_RRH_LAYER3_REC_IDLE);

	/* 戻り値? */
	if( cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_HEAD ), (VOID**)&head_pt ) != CMD_RES_OK )
	{
			/* バッファ取得失敗時はアボート処理 */
		cm_MAbort( CMD_ALMCD_BUFGET, "m_cr_Init", 
							   "cm_BReq NG", 
							   CMD_NG, sizeof( CMT_TSKIF_HEAD ), CMD_NUM0 );
	}
	
	head_pt->uiEventNo = CMD_TSKIF_INIRSP;
	head_pt->uiSrcTQueueID = CMD_TSKID_CPR;
	head_pt->uiDstTQueueID = CMD_TSKID_INI;
	head_pt->uiLength = sizeof(CMT_TSKIF_HEAD);
	/* send 初期化応答to 初期化タスク */
	a_rtn = cm_Enter(CMD_TSKID_INI,CMD_QRB_NORMAL,CMD_TSKIF_INIRSP,head_pt);
	if(a_rtn != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, a_rtn, "cm_Enter NG" );
	}

	/* 起動要因が「POWERON(Bit22)」「L1Inband-RST/UnKnown(Bit31)」の場合、配下REを2次リセットの対象とする */
    crw_poweron_reset_target = 0;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_RRH_REG_SLCR_ZYNQ_REBOOT_STATUS, &regVal);
	if (0 != (((D_RRH_SLCR_RST_FACT_POR)|(D_RRH_SLCR_RST_FACT_CPYL1RST)) & regVal))
	{
		/* D0=RE#1～D15=RE#16  BitON=2次リセット対象 */
		crw_poweron_reset_target = 0xFFFF;
    }
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_RRH_REG_SLCR_ZYNQ_REBOOT_STATUS, (D_RRH_SLCR_RST_FACT_CPYL1RST));

	/************************************************************************/
	/* CPRI管理走行履歴書き込み												*/
	/************************************************************************/
	m_cr_cm_RunHisSet(CRD_RUNHIS_FLG_SND, CMD_TSKID_INI, CMD_TSKIF_INIRSP, (UCHAR*)head_pt);

	/* set task state to 1: rec startup */
	crw_taskstate = CRD_TASKSTA_REC_STARTUP;

	return;	
 }
/* @} */

/* @} */
