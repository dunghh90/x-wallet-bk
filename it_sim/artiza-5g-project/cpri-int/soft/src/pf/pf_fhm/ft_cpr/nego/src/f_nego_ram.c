/*!
 * @skip  $ld:$
 * @file  f_nego_ram.c
 * @brief PF_nego thread ram table define
 * @date 2015/04/22 FFCS)fuxg create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-
 */

#include "f_sys_inc.h"
#include "f_nego_inc.h"
    
/*!
 * @addtogroup RRH_PF_NEGO
 * @{
 */
UINT f_nego_thrdState;					/*!< PF_nego threadstate	*/

UINT f_nego_extEventNo;					/*!< PF_nego threadexternal event number	*/

T_NEGO_RUNHIS_TBL f_nego_runhis_tbl;

UINT f_nego_hard_set_flag;				/*!< hard register set flag	*/

UINT f_nego_bitrate;					/*!< real CPRI Bit Rate	*/

UINT f_nego_4s_count;					/*!< 4S count	*/

UINT f_nego_hfn_count;					/*!< HFN count	*/

UINT f_nego_pll_holdover_value;			/*!< EXT PLL HOLD OVER value	*/

UINT f_nego_dbg_result_24g;				/*!< 4.9G nego result for DBG	*/

UINT f_nego_dbg_result_49g;				/*!< 4.9G nego result for DBG	*/

UINT f_nego_dbg_result_98g;				/*!< 9.8G nego result for DBG	*/

/* @} */

