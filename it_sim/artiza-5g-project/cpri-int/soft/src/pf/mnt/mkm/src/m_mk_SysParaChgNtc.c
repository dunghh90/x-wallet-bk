/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_mk_sysParaChgNtc.c
 *  @brief  System parameter change notice process
 *  @date   2015/05/31 FFCS)wuqy create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/*  TRA common head file							*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/
#include "rrhApi_Svp.h"

/********************************************************************************************************************/
/**
 *  @brief  system parameter change notice process
 *  @note   system parameter change notice process
 *  @param  inf_p		[in]	Pointer of buffer
 *  @return   None
 *  @retval   -
 *  @date     2015/05/31 FFCS)wuqy modify for tdd
 */
/********************************************************************************************************************/
VOID m_mk_SysParaChgNtc(UINT *inf_p)
{
	CMT_TSKIF_SYSPARA_CHG_NTC 	    *ntc_p = (CMT_TSKIF_SYSPARA_CHG_NTC *)inf_p;

	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, 
		"MKM sys parameter change,old:%d,new:%d",
		ntc_p->inf.tx_sys_num_old,
		ntc_p->inf.tx_sys_num_new);

	cmw_sys_mng_tbl[CMD_SYS_S3G].tx_sys_num = ntc_p->inf.tx_sys_num_new;

}
/* @} */