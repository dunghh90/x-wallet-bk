/*!
 * @skip		$ld:$
 * @file		f_cmu_recv_ram.c
 * @brief		cmu RAMテーブル
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#include "f_cmn_cmu_inc.h"

USHORT					f_recvw_thdState;						/**< thread state management table						*/
T_CMU_MSG_MNG			f_recvw_msg;							/**< receive msg manage table							*/
T_CMU_ROUTE_MNG			f_recvw_route;							/**< route information of message table					*/
T_CMU_THREAD_MNG		f_recvw_ProcTbl;						/**< Process information table							*/

USHORT					f_sendw_thdState;						/**< thread state management table						*/
T_CMU_MSG_MNG			f_sendw_msg;							/**< receive msg manage table							*/
T_CMU_ROUTE_MNG			f_sendw_route;							/**< route information of message table					*/
T_CMU_THREAD_MNG		f_sendw_ProcTbl;						/**< Process information table							*/

/** @} */

