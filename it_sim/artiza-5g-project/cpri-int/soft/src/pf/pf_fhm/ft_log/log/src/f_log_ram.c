/******************************************************************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		f_log_ram.c
 *  @brief		RAM Table
 * @author		ALPHA)yokoyama
 *  @note		NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 				See also TableSpecifications(log)_ram.xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010-2013
 */
/******************************************************************************************************************************/

#include	"f_log_inc.h"

/*!
 * @addtogroup RRH_PF_LOG_LOG
 * @{
 */

/**
 * @brief thread state management table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
USHORT f_logw_thdState[D_SYS_MAX_SYSNUM];

/**
 * @brief response manage table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_LOG_RSP_MNG_TBL f_logw_rspMngTbl[D_SYS_MAX_SYSNUM][E_LOG_RSP_MAX];

/**
 * @brief trouble log get info table
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
T_API_LOG_TROUBLELOG_GET_INFO f_logw_troublelog_get_info;

/**
 * @brief log shelter timer count
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
UINT f_logw_logShelterTimCount;

/**
 * @brief log thread assert pointer
 * @note  [ 値の説明 ]
 * @note  [ 用途     ]
 * @note  [ 注意事項 ]
 * @note  [ 更新契機 ]
 */
VOID* f_logw_assert_p;

/** @} */
