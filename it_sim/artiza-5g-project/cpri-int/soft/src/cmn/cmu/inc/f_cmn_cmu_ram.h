/*!
 * @skip		$ld:$
 * @file		f_cmn_cmu_ram.h
 * @brief		cmu ram table define
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#ifndef	F_CMU_RAM_H
#define	F_CMU_RAM_H

/* recv */
extern USHORT					f_recvw_thdState;							/**< thread state management table						*/
extern T_CMU_ROUTE_MNG			f_recvw_route;								/**< route information of message table					*/
extern T_CMU_THREAD_MNG			f_recvw_ProcTbl;							/**< Process information table							*/

/* send */
extern USHORT					f_sendw_thdState;							/**< thread state management table						*/
extern T_CMU_ROUTE_MNG			f_sendw_route;								/**< route information of message table					*/
extern T_CMU_THREAD_MNG			f_sendw_ProcTbl;							/**< Process information table							*/

#endif	/* F_CMU_RAM_H */
/** @} */
