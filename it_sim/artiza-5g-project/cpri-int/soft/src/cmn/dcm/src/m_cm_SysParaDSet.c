/*!
 * @skip   $Id$
 * @file   m_cm_SysParaDSet.c
 * @brief  the processing for systemparameter's part D setting .
 * @date   2009/07/20 FFCS)Tangj Create for eNB-009-001 
 *                    S3G PRC 7th release function system parameter mechanism change
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2009
 */

/*!
 * @addtogroup RRH_DCM
 * @{
 */

#include "m_cm_header.h"			/* TRA common head file			*/
#include "rrhApi_Cpri.h"
/*!
 *  @brief  the processing for systemparameter's part D setting .
 *  @note   This function is processed as follows.
 *          1) set sector chip revision
 *          2) set CPRI state stability timer
 *  @param  a_sys_typ	[in]	system type
 *  @return Void.
 *  @retval -
 *  @date   2008/09/08 FFCS)Wuh Create.
 *  @date   2008/12/26 FFCS)Wuh M-S3G-eNBPF-01078												
 *				            don't set branch1 if card type is LRE 1TX
 *  @date   2009/07/20 FFCS)Tangj modify for S3G PRC 7th release function system parameter mechanism change
 *                          move the m_dl_SysParaDSet to m_cmR_SysParaDSet and add the comparation processing
 */

VOID m_cm_SysParaDSet( UINT a_sys_typ)
{
	INT		ret;
	/****************************************************************/
	/* Sector n CHIP revision setting*/
	/****************************************************************/
	/*  Branch #0 setting */
	cmw_hwsetsys_tbl.sec_chip0 = cmw_sys_mng_tbl[a_sys_typ].sec_chip0;
	cmw_hwsetsys_tbl.sec_chip1 = cmw_sys_mng_tbl[a_sys_typ].sec_chip1;
	
	/* CPRI ステート安定時間設定レジスタに、									*/
	/* システムパラメータ情報管理テーブル「CPRI State安定化時間（RE）」の値		*/
	/* を設定する。																*/
	if(cmw_sys_mng_tbl[a_sys_typ].cpri_sttimer != cmw_hwsetsys_tbl.cpri_sttimer)
	{
/*		cm_WriteReg( CMD_REGADR_CST_TIMER, cmw_sys_mng_tbl[a_sys_typ].cpri_sttimer );*/
		ret = rrhApi_Cpri_Mnt_UpdateSystemPara(D_RRH_PROCQUE_L3,0,NULL,cmw_sys_mng_tbl[a_sys_typ].cpri_sttimer);
		if(ret != E_API_RCD_OK)
			cm_Assert( CMD_ASL_DBGHIGH, ret,	"rrhApi_Cpri_Mnt_UpdateSystemPara error" );		
		else 
			cm_Assert( CMD_ASL_DBGLOW, cmw_sys_mng_tbl[a_sys_typ].cpri_sttimer,	
						"rrhApi_Cpri_Mnt_UpdateSystemPara OK" );		
			
		
		/* update the cmw_hwsetsys_tbl table for saving the hardware setting system parameter value */
		cmw_hwsetsys_tbl.cpri_sttimer = cmw_sys_mng_tbl[a_sys_typ].cpri_sttimer;
	}

	return;
}

/* @} */
