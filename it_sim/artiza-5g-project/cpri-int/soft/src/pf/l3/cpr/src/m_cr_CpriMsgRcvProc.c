/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_CpriMsgRcvProc.c
 *  @brief  CPRI message receive handle function
 *  @date   2008/07/18 FFCS)Wangjuan create
 *  @date   2009/08/08 FFCS)Wangjuan modify for 
 *							  CR-00058-005(CPRI spec V1.0.9) 
 *  @date   2009/10/28 QNET)Kabasima modify for 
 *							  M-S3G-eNBSYS-00664:☆BRE☆2003形にBRE装置を接続し、
 *							  3本確立しているCPRIリンクの1本のみ断したところ、
 *							  Activeリンクがいなくなった。
 *  @date   2010/04/02 FJT)Taniguchi CR-XXXXX-XXX:F-971(DCM)対応
 *  @date   2012/07/05  FFCS)niemsh modify for 1.5GVA
 *  @date   2015/08/03 TDIPS)ikeda modify
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"  /* CPRI管理 task header file */

/* @{ */
/**
* @brief CPRI message receive handle function
* @note Judge the input CPRI message and dispatch it to the corresponding handle function.\n
* @param buff_adr [in] the buffer address pointer of received CPRI message
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date   2009/10/28 QNET)Kabasima modify for 
*						  M-S3G-eNBSYS-00664:☆BRE☆2003形にBRE装置を接続し、
*						  3本確立しているCPRIリンクの1本のみ断したところ、
*						  Activeリンクがいなくなった。
* @date   2015/08/04 TDIPS)ikeda 対REヘルスチェック応答呼び出し追加
*/
VOID m_cr_CpriMsgRcvProc(UINT * buff_adr)
{
	USHORT			a_signalkind;	/* signal kind */
	USHORT			a_signalkind_org;	/* signal kind original*/
	CMT_TSKIF_CPRIRCV_LINKSTSREQ*	a_reclsntc_p;	/* input IF pointer */
	USHORT							a_settype;	/* Act/Negative seting type */
	USHORT			a_system_type;	/* system type */
	UINT		a_sigchk;			/* result of checking signal */
	USHORT			a_link_num;

	/*Get act/negative setting type from input IF */
	a_reclsntc_p = (CMT_TSKIF_CPRIRCV_LINKSTSREQ*)buff_adr;
	a_settype = a_reclsntc_p->cpridat_linkstsreq.link; 

	a_link_num = a_reclsntc_p->cprircv_inf.link_num;

	/* 信号種別を取得 */
	a_signalkind = (a_reclsntc_p->cpridat_linkstsreq.signal_kind) & CMD_SYS_MSK_CPRID;
	a_signalkind_org = a_reclsntc_p->cpridat_linkstsreq.signal_kind;

	if(a_signalkind == CMD_CPRID_HCREQ)
	{
		m_cr_HcReq( a_reclsntc_p->cpridat_linkstsreq.signal_kind & CMD_SYS_CHK_CPRID);
	}
	else if(a_signalkind == CMD_CPRID_HCRES)
	{
		/* 対RE HC応答受信 */
		m_cr_HcRsp(a_link_num, a_reclsntc_p->cpridat_linkstsreq.signal_kind & CMD_SYS_CHK_CPRID);
	}
	else
	{
		a_sigchk = m_cm_chksig(a_signalkind_org,&a_system_type);

		if(a_sigchk != CMD_RES_OK)
		{
			cm_Assert(	CMD_ASL_USELOW,
						a_system_type, "validate signal NG");

			m_cr_ComLinkSettingL3Rsp(a_link_num, a_system_type, CMD_MAKER_RENG);

			return;
		}
		
		switch( a_signalkind )
		{
			case CMD_CPRID_LINKSTSREQ:
				/* handle RE common link set request */
				m_cr_ReComLnkSetProc(a_link_num, a_system_type, a_settype);
				
				break;

			default:
				cm_Assert( CMD_ASL_DBGLOW, (UINT)a_signalkind, 
						 	"Exception Event Rcv" );

				break;
		
		}
	}

	return;
}
/* @} */

/* @{ */
VOID m_cr_CpriMsgRcvProc2(UINT * buff_adr)
{
	USHORT			a_signalkind;	/* signal kind */
	CMT_TSKIF_CPRIRCV_LINKSTSREQ*	a_reclsntc_p;	/* input IF pointer */
	USHORT			a_link_num;

	/*Get act/negative setting type from input IF */
	a_reclsntc_p = (CMT_TSKIF_CPRIRCV_LINKSTSREQ*)buff_adr;

	a_link_num = a_reclsntc_p->cprircv_inf.link_num;

	/* 信号種別を取得 */
	a_signalkind = (a_reclsntc_p->cpridat_linkstsreq.signal_kind) & CMD_SYS_MSK_CPRID;

	if(a_signalkind == CMD_CPRID_HCRES)
	{
		/* 対RE HC応答受信 */
		m_cr_HcRsp(a_link_num, a_reclsntc_p->cpridat_linkstsreq.signal_kind & CMD_SYS_CHK_CPRID);
	}
	else
	{
		cm_Assert(	CMD_ASL_USELOW,
						a_signalkind, "cpri signal NG to REC Message");

	}

	return;
}
/* @} */

/* @{ */
/**
* @brief RE common link set request handle function
* @note perform active setting according to the set type.\n
* @param linkno [in] CPRIリンク番号
* @param system_type [in] the system that request RE common link set
* @param set_type [in] the RE common link set type
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2009/08/08 FFCS)Wangjuan modify for 
*						CR-00058-005(CPRI spec V1.0.9) 
* @date   2009/10/28 QNET)Kabasima modify for 
*						M-S3G-eNBSYS-00664:☆BRE☆2003形にBRE装置を接続し、
*						3本確立しているCPRIリンクの1本のみ断したところ、
*						Activeリンクがいなくなった。
* @date   2010/04/02 FJT)Taniguchi CR-XXXXX-XXX:F-971(DCM)対応
* @date   2012/07/05  FFCS)niemsh modify for 1.5GVA
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
*
*/

VOID m_cr_ReComLnkSetProc(USHORT linkno, USHORT system_type, USHORT set_type)
 {
	/*================================================================ */
	/* below processing is for RE type = OFTRX 								     */
	/*================================================================ */
	if(set_type == CMD_RECLS_ACTSET)
	{  /* in case of active set request */
		
		/*send  OK response */
		m_cr_ComLinkSettingL3Rsp(linkno, system_type, CMD_OK);
	}
	else
	{  /* in case of  negative set request */

		/*send  NG response */
		if(system_type == CMD_SYS_3G)
		{
			m_cr_ComLinkSettingL3Rsp(linkno, system_type, CMD_NG);
		}
		else
		{
			if (cmw_connectoperatemode > CMD_OP_LTE)
			{
				m_cr_ComLinkSettingL3Rsp(linkno, system_type, CMD_HDL_NG);
			}
		}
	}

	cmw_reclsmngtbl.retype = (UINT)((system_type<<16))+cmw_reclsmngtbl.retype;

	for( system_type = CMD_NUM0; system_type < CMD_MAX_SYSNUM; system_type++)
	{
		/* RRE/LRE(15G17G-LRE除く)はActive切替無効装置であるため、テーブルの初期状態をActiveで初期化 */
		cmw_reclsmngtbl.traclsinfo[system_type] = set_type;
		/*	RRE/LRE(15G17G-LRE除く)はActive LEDがないため、消灯固定とする	*/
		cmw_reclsmngtbl.actneginfo[system_type] = CMD_RECLS_NEGSET;
	}
	
	/* set self TRA slot number =  1  */
	cmw_reclsmngtbl.selfslotno = CMD_NUM1 ;

	/*	RRE/LER(15G-17G-LRE除く)は、REC主導での共通部リンク確立要求は受信しないため	*/
	cm_Assert(	CMD_ASL_DBGLOW,(UINT)((system_type<<16)+cmw_reclsmngtbl.retype), 
					"m_cr_ReComLnkSetProc for OF-TRX");

	return;
 }
/* @} */

/* @} */
