/*!
 * @skip 		$Id:$
 * @file 		f_eom_ext.h
 * @brief		EOM thread extern header
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_DHC

 *  @{
 */
#ifndef F_EOM_EXT_H
#define F_EOM_EXT_H

/****************************************************************************************************************/
/*  Information of EOM Thread																					*/
/****************************************************************************************************************/
/*!
 * @name Information of EOM Thread
 * @note Information of EOM Thread
 * @{
 */
																		/* Information of EOM Thread			*/
extern	T_EOM_INFO					f_eomw_thdInfo __attribute__ ((aligned (16)));
/* @} */

/****************************************************************************************************************/
/*  Matrix function of EOM Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of EOM Thread
 * @note	Matrix function of EOM Thread
 * @{
 */
																		/* Matrix function of EOM Thread		*/
extern	const	T_EOM_CALLSETRELL_MTXTBL	f_eomr_callSetRell_mtxtbl[E_EOM_STATE_MAX][E_EOM_EVT_MAX];
/* @} */

#endif
/** @} */
