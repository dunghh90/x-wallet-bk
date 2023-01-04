/*!
 * @skip 		$ld:$
 * @file 		f_fhs_ram.c
 * @brief		pf_fhsスレッド RAMテーブル
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

#include	"f_fhs_inc.h"

/** @addtogroup RRH_PF_FHS
 *  @{
 */

/****************************************************************************************************************/
/*  Information of FHS Thread																					*/
/****************************************************************************************************************/
/*!
 * @name	Information of FHS Thread
 * @note	Information of FHS Thread
 * @{
 */
T_FHS_INFO							f_fhsw_thdInfo;					/* Information of FHS Thread			*/
/** @} */

/****************************************************************************************************************/
/*  Time information for SAA response																			*/
/****************************************************************************************************************/
/*!
 * @name	Time information for SAA response
 * @note	Time information for SAA response
 * @{
 */
T_FHS_STARTUP_TIME					f_fhsw_startup_time;
/* @} */
