/******************************************************************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		f_lgc_ram_local.h
 *  @brief		External Definition
 *  @note		NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 				See also TableSpecifications(log)_ram.xls
 * @date		2018/08/24 KCN)若水 カスケードを削除 sysKindの3G/S3GをSUB6/MMWに変更
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010-2014
 */
/******************************************************************************************************************************/

#ifndef	F_LGC_RAM_LOCAL_H
#define	F_LGC_RAM_LOCAL_H

/** @addtogroup RRH_PF_LGC
 *  @{
 */
#include "f_lgc_inc.h"
#include "f_lgc_ram.h"

extern	USHORT				f_lgcw_thdState;		/**< thread state management table			*/
extern	T_LGC_RSP_MNG_TBL	f_lgcw_rspMngTbl[E_LGC_RSP_MAX];	/**< response manage table		*/
extern	T_API_LGC_TROUBLELOG_GET_INFO	f_lgcw_troublelog_get_info;	/**< trouble log get info table				*/
extern	VOID*				f_lgcw_assert_p;						/**< log assert pointer						*/

#endif	/* F_LGC_RAM_LOCAL_H */
/** @} */
