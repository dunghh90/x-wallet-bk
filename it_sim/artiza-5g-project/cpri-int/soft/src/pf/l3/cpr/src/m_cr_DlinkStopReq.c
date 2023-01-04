/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file   m_cr_DlinkStopReq.c
 *  @brief  L2 link disconnect request notice handle function
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"	/* CPRI管理 task header file */
#include "rrhApi_L2.h"

/* @{ */
/**
* @brief L2 link disconnect request notice handle function
* @note edit L2 STOP request and send to LAPB task.\n
* @param linkno [in] CPRIリンク番号
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
 *  @date    2015.08.11 TDIPS)ikeda 17リンク対応
  *
*/

VOID m_cr_DlinkStopReq(USHORT linkno)
 {
 	UINT ret;
 	T_API_COM_RSP_DATA	result;

	ret = rrhApi_L2_Mqr_ReleaseLink(linkno, D_RRH_PROCQUE_API_RELEASELNK,0,&result);
	if(ret != E_API_RCD_OK)
	cm_Assert(CMD_ASL_DBGLOW,ret,"rrhApi_L2_Mqr_ReleaseLink NG");		

	return;
 }
/* @} */

/* @} */
