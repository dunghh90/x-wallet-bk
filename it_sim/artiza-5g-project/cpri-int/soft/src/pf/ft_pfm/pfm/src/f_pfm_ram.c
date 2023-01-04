/*!
 * @skip 		$ld:$
 * @file 		f_pfm_ram.c
 * @brief		pf_pfmスレッド RAMテーブル
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	RAM Table
 *  @note	
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

#include	"f_pfm_inc.h"

/** @addtogroup RRH_PF_PFM
 *  @{
 */

/****************************************************************************************************************/
/*  Information of PFM Thread																					*/
/****************************************************************************************************************/
/*!
 * @name	Information of PFM Thread
 * @note	Information of PFM Thread
 * @{
 */
T_PFM_INFO							f_pfmw_thdInfo;					/* Information of PFM Thread			*/
/** @} */

UINT						f_pfmw_rxwin_reg_addr_offset_antnum;
UINT						f_pfmw_rxwin_reg_addr_offset_ant;
UINT						f_pfmw_rxwin_reg_addr_offset_ccnum;
UINT						f_pfmw_rxwin_reg_addr_offset_cc;
T_PFM_REG_INFO				f_pfmw_rxwin_reg_addr[D_RRH_PFM_NUM_OF_REG];

/* @} */
