/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	External Definition
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also TableSpecifications(dpda)_rom.xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2011-2013
 */
/******************************************************************************************************************************/

#ifndef	F_DPDA_ROM_LOCAL_H
#define	F_DPDA_ROM_LOCAL_H

#include "f_dpda_rom.h"

/** @addtogroup RRH_PF_RFP
 *  @{
 */

extern const UINT f_dpdar_reg_Tbl[E_RRH_REG_NUM_MAX];
extern const UINT f_dpdar_virtualRegAddr[D_DPDA_DBG_VIRTUAL_REG_CNT];
extern const UINT f_dpdar_virtualRegData[D_DPDA_DBG_VIRTUAL_REG_CNT];

/* アラーム関連 */
extern const T_DPDA_ALM_THRESHOLD_INFO f_dpdar_almThreshold[E_DPDA_ALM_THRESH_NUM_MAX];
extern const T_DPDA_ALM_INFO f_dpdar_almInfo[D_DPDA_ALM_NUM];
extern const T_DPDA_ALM_MSK_CHECK f_dpdar_almmsk_check[D_DPDA_ALM_NUM];

#endif	/* F_DPDA_ROM_LOCAL_H */
/** @} */
