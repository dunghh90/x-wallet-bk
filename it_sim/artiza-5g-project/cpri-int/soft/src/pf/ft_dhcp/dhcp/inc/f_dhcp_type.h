/*!
 * @skip 		$Id:$
 * @file 		f_dhcp_typ.h
 * @brief		DHCP thread type header
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_DHC

 *  @{
 */
#ifndef F_DHCP_TYP_H
#define F_DHCP_TYP_H

/****************************************************************************************************************/
/*  Information of DHCP Thread																					*/
/****************************************************************************************************************/
/*!
 * @name Information of DHCP Thread
 * @note Information of DHCP Thread
 * @{
 */
typedef struct{
	UINT							thdState;							/* Thread状態			*/
	UINT							dhcpState;							/* DHCP状態				*/
}T_DHCP_INFO;
/* @} */

/****************************************************************************************************************/
/*  Matrix function of DHCP Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of DHCP Thread
 * @note	Matrix function of DHCP Thread
 * @{
 */
typedef struct{
	INT								(*eventProcess_addr)(VOID *msgPtr);	/* マトリクス関数		*/
}T_DHCP_CALLSETRELL_MTXTBL;
/* @} */

#endif
/** @} */
