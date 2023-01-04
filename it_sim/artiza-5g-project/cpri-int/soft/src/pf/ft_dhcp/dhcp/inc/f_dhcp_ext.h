/*!
 * @skip 		$Id:$
 * @file 		f_dhcp_ext.h
 * @brief		DHCP thread extern header
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_DHC

 *  @{
 */
#ifndef F_DHCP_EXT_H
#define F_DHCP_EXT_H

/****************************************************************************************************************/
/*  Information of DHCP Thread																					*/
/****************************************************************************************************************/
/*!
 * @name Information of DHCP Thread
 * @note Information of DHCP Thread
 * @{
 */
																		/* Information of DHCP Thread			*/
extern	T_DHCP_INFO					f_dhcpw_thdInfo __attribute__ ((aligned (16)));
/* @} */

/****************************************************************************************************************/
/*  Matrix function of DHCP Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of DHCP Thread
 * @note	Matrix function of DHCP Thread
 * @{
 */
																		/* Matrix function of DHCP Thread		*/
extern	const	T_DHCP_CALLSETRELL_MTXTBL	f_dhcpr_callSetRell_mtxtbl[D_DHCP_STATENUM][D_DHCP_EVENTNUM];
/* @} */

#endif
/** @} */
