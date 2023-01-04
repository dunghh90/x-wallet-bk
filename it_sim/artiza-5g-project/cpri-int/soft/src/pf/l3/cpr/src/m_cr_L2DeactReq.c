/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_cr_L2DeactReq.c
 *  @brief  L2 DEACT request sending function
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

/**
* @brief L2 DEACT request sending function
* @note edit L2 DEACT request and send to LAPB task.\n
* @param linkno [in] CPRIリンク番号
* @return None
* @date 2008/07/18 FFCS)Wangjuan create
* @date 2015/08/07 TDI)ikeda CPRIリンク番号を引数に追加
*/
VOID m_cr_L2DeactReq(USHORT linkno)
{
	INT ret = CMD_NUM0;

	ret = rrhApi_L2_Mnt_DeactLapbLink(linkno, D_RRH_PROCQUE_L3,0,D_RRH_NULL);
	if(ret != E_API_RCD_OK)
	cm_Assert(CMD_ASL_DBGLOW,ret,"rrhApi_L2_Mnt_DeactLapbLink NG");		

	return;
}

/* @} */
