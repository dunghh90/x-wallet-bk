/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	External Definition
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also TableSpecifications(dpda)_ram.xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2011-2015
 */
/******************************************************************************************************************************/

#include "f_dpda_inc.h"
#include "f_dpda_ram.h"

#ifndef	F_DPDA_RAM_LOCAL_H
#define	F_DPDA_RAM_LOCAL_H

/** @addtogroup RRH_PF_RFP
 *  @{
 */
extern T_DPDA_REGDATA           f_dpdaw_regdataReal __attribute__ ((aligned (32)));
extern T_DPDA_STATUS_INFO       f_dpdaw_statusInfo __attribute__ ((aligned (32)));
extern UINT                     f_dpdaw_almThreshold[E_DPDA_ALM_THRESH_NUM_MAX][D_DPDA_ANTMAX];
extern UINT                     f_dpdaw_occurAlmInstanceId[D_DPDA_ALM_NUM];
extern UINT                     f_dpdaw_clearAlmInstanceId[D_DPDA_ALM_NUM];
extern T_DPDA_ALM_STATE         f_dpdaw_almState[D_DPDA_ALM_NUM];
extern T_DPDA_INFO_ALL          *f_dpdaw_info_all;
extern T_DPDA_ALM_LOG_DEBUG     *f_dpdaw_almLog;
extern T_RRH_ALM_LOG_FOR_HIST   *f_dpdaw_almLogForHistory;
extern T_RRH_LOG_TEMP           *f_dpdaw_tempLog;

#endif	/* F_DPDA_RAM_LOCAL_H */
/** @} */
