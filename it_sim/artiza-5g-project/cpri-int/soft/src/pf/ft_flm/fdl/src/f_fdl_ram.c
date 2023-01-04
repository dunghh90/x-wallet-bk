/*!
 * @skip $Id:$
 * @file f_fdl_ram.c
 * @brief FDLタスク内部データ
 * @date  2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */

#include "f_fdl_inc.h"

T_FDL_INFO					f_fdl_thdInfo;											/**< fdl thrad information table		*/

UINT						f_fdl_swact_data[2];

T_FDL_INSTSTATE				f_fdl_inst_state_data ;

T_FDL_BUILD_DATA			f_fdl_inst_build_data ;

char						f_fdl_prdct_id[D_SYS_NUM16];

/** @} */

