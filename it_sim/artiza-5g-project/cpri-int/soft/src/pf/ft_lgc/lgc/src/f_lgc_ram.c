/******************************************************************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		f_lgc_ram.c
 *  @brief		RAM Table
 * @author		ALPHA)yokoyama
 *  @note		NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 				See also TableSpecifications(log)_ram.xls
 * @date 		2018/08/24 KCN)若水 カスケードを削除 sysKindの3G/S3GをSUB6/MMWに変更
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010-2013
 */
/******************************************************************************************************************************/

#include	"f_lgc_inc.h"

/*!
 * @addtogroup RRH_PF_LGC_LGC
 * @{
 */

USHORT					f_lgcw_thdState;	/**< thread state management table	*/

T_LGC_RSP_MNG_TBL		f_lgcw_rspMngTbl[E_LGC_RSP_MAX];	/**< response manage table	*/

VOID*					f_lgcw_assert_p;					/**< log thread assert pointer		*/

/** @} */

