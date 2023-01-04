/*!
 *  @skip   $Id$
 *  @file   t_cr_forceStopProc.c
 *  @brief  RE強制停止通知 message reception processing
 *  @author FFCS WANGJUAN
 *  @date   2007/03/15 WANGJUAN create
 *  @date   2009/03/06 FFCS)Wangjuan modify for M-S3G-eNBPF-01474 
 *
 *  Copyright 2007 FUJITSU LIMITED
 */

  /** @addtogroup TRIF_CPC
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"			/* CPRI management task header file */
/*!
*  @brief  RE強制停止通知 message reception processing
*  @note   health check stop / active switch start for ¥n
*          the specified CPRI link
*  @param  buff_adr          [in] response info
*  @return none
*  @Bug_No M-RRU-ZSYS-01971  
*  @date   2007/03/15 FFCS WANGJUAN create
*  @date   2009/03/06 FFCS)Wangjuan modify for M-S3G-eNBPF-01474 
*  @date   2015/09/03 TDIPS)ikeda 安定化追加
*  @date   2015/10/09 TDI)satou 配下REの強制停止処理が常に処理されない不具合を修正
*  @date   2015/10/22 TDI)satou LED制御のためL3/rctに強制停止を通知するように
*  @date   2015/10/29 TDI)satou FHM技説-QA-113 強制停止の要因を通知するように
*  @date   2015/10/30 TDI)ikeda IT3 問処番号No.43対応
*  @date   2015/11/10 TDI)takeuchi M-RRU-ZSYS-01971 問処番号No.165対応 優先度をチェックしないよう修正
*/
VOID t_cr_forceStopProc(UINT* buff_adr)
{
	CMT_TSKIF_RECMPSTPNTC* a_msg ;
	CMT_TSKIF_RE_FORCESTOP_NTC forceStopNtc;
	USHORT linkno;
	USHORT system_type;

	a_msg = ( ( CMT_TSKIF_RECMPSTPNTC *) buff_adr);
	linkno = a_msg->link_num;
	system_type = a_msg->system_type;

	/* CPRI link disconnection notification sending function */
	m_cr_cm_lnkDisNtcRE(linkno, system_type);

	/* 強制停止をl3/rctに通知 */
	memset(&forceStopNtc, 0, sizeof forceStopNtc);
	forceStopNtc.head.uiEventNo = CMD_TSKIF_RE_FORCESTOP_NTC;
	forceStopNtc.link_num       = linkno;
	forceStopNtc.system_type    = system_type;
	forceStopNtc.stopFactor     = a_msg->stopFactor;
	l3_com_sendMsg(CMD_TSKID_CPR, D_RRH_PROCQUE_L3, CMD_TSKID_RCT, &forceStopNtc, sizeof forceStopNtc);

	cmw_cprilnkstatbl[linkno].cprilnkstano[system_type] = CRD_CPRILNKSTA_NO6;

	return;

}
/* @} */
