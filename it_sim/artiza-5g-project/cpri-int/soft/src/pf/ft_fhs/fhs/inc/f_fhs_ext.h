/*!
 * @skip 		$Id:$
 * @file 		f_fhs_ext.h
 * @brief		FHS thread extern header
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_DHC

 *  @{
 */
#ifndef F_FHS_EXT_H
#define F_FHS_EXT_H

/****************************************************************************************************************/
/*  Information of FHS Thread																					*/
/****************************************************************************************************************/
/*!
 * @name Information of FHS Thread
 * @note Information of FHS Thread
 * @{
 */
																		/* Information of FHS Thread			*/
extern	T_FHS_INFO					f_fhsw_thdInfo __attribute__ ((aligned (16)));
/* @} */

/****************************************************************************************************************/
/*  Matrix function of FHS Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of FHS Thread
 * @note	Matrix function of FHS Thread
 * @{
 */
																		/* Matrix function of FHS Thread		*/
extern	const	T_FHS_CALLSETRELL_MTXTBL	f_fhsr_callSetRell_mtxtbl[D_FHS_STATENUM][D_FHS_EVENTNUM];
/* @} */

/****************************************************************************************************************/
/*  Judgement of Operational Status Table																		*/
/****************************************************************************************************************/
/*!
 * @name	Judgement of Operational Status Table
 * @note	Judgement of Operational Status Table
 * @{
 */
extern	const	INT		f_fhsr_reg_state_mask[D_RRH_SFP_MAXNUM][3];					/* Low Layer 障害レジスタマスク値		*/
/* @} */

/****************************************************************************************************************/
/*  Time information for SAA response																			*/
/****************************************************************************************************************/
/*!
 * @name	Time information for SAA response
 * @note	Time information for SAA response
 * @{
 */
extern	T_FHS_STARTUP_TIME	f_fhsw_startup_time;
/* @} */

#endif
/** @} */
