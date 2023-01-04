/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_DlinkSettingReq.c
 *  @brief  dala link setting request sending function
 *  @date   2008/07/18 FFCS)Wangjuan create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_CPR
* @{ */

#include "m_cm_header.h"	/* TRA共通ヘッダファイル				*/
#include "m_cr_header.h"	/* CPRI管理 task header file */
#include "f_rrh_def.h"
#include "rrhApi_L2.h"

/* @{ */
/**
* @brief dala link setting request sending function
* @note edit data link establish request and send to LAPB task.\n
* @param linkno CPRIリンク番号
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2015/07/30 TDI)ikeda 17リンク対応
*/

VOID m_cr_DlinkSettingReq(USHORT linkno)
 {
 	INT ret;
 	T_API_COM_RSP_DATA	result;
	
	ret = rrhApi_L2_Mqr_SetupLink(linkno, D_RRH_PROCQUE_API_SETUPLNK,100,&result);
	if(ret != E_API_RCD_OK)
	cm_Assert(CMD_ASL_DBGLOW,ret,"rrhApi_L2_Mnr_SetupLink NG");		
	
	return;
 }
/* @} */

/* @} */ 	
