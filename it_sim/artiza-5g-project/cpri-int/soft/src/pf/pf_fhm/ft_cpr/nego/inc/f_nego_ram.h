/*!
* @skip $Id:$
* @file f_nego_ram.h
* @brief NEGO RAM table extern declaration
* @date 2015/04/22 FFCS)fuxg create.
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015-
*/

/** @addtogroup RRH_PF_NEGO
 *  @{
 */

#ifndef F_NEGO_RAM_H
#define F_NEGO_RAM_H

extern UINT f_nego_thrdState;			/*!< CPRI task state	*/

extern UINT f_nego_extEventNo;			/*!< CPRI task external event number	*/

extern T_NEGO_RUNHIS_TBL f_nego_runhis_tbl;

#ifndef OPT_RRH_ZYNQ_REC
extern UINT f_nego_hard_set_flag;		/*!< hard register set flag	*/

extern UINT f_nego_bitrate;				/*!< real CPRI Bit Rate	*/

extern UINT f_nego_4s_count;			/*!< 4S count	*/

extern UINT f_nego_hfn_count;			/*!< HFN count	*/
#else
extern UINT f_nego_hard_set_flag[D_RRH_CPRINO_RE_MAX];		/*!< hard register set flag	*/
extern UINT f_nego_bitrate[D_RRH_CPRINO_RE_MAX];				/*!< real CPRI Bit Rate	*/
extern UINT f_nego_4s_count[D_RRH_CPRINO_RE_MAX];			/*!< 4S count	*/
extern UINT f_nego_hfn_count[D_RRH_CPRINO_RE_MAX];			/*!< HFN count	*/
#endif
extern UINT f_nego_pll_holdover_value;	/*!< EXT PLL HOLD OVER value	*/

extern UINT f_nego_dbg_result_24g;		/*!< 2.4G nego result for DBG	*/

extern UINT f_nego_dbg_result_49g;		/*!< 4.9G nego result for DBG	*/

extern UINT f_nego_dbg_result_98g;		/*!< 9.8G nego result for DBG	*/

#endif

/** @} */

