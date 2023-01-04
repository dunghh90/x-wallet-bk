/******************************************************************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		f_log_ram_local.h
 *  @brief		External Definition
 *  @note		NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 				See also TableSpecifications(log)_ram.xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010-2014
 */
/******************************************************************************************************************************/

#ifndef	F_LOG_RAM_LOCAL_H
#define	F_LOG_RAM_LOCAL_H

/** @addtogroup RRH_PF_LOG
 *  @{
 */
#include "f_log_inc.h"
#include "f_log_ram.h"

extern USHORT f_logw_thdState[D_SYS_MAX_SYSNUM];
extern T_LOG_RSP_MNG_TBL f_logw_rspMngTbl[D_SYS_MAX_SYSNUM][E_LOG_RSP_MAX];
extern T_API_LOG_TROUBLELOG_GET_INFO f_logw_troublelog_get_info;
extern UINT f_logw_logShelterTimCount;
extern VOID* f_logw_assert_p;

#endif	/* F_LOG_RAM_LOCAL_H */
/** @} */
