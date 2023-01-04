/*!
 * @skip		$ld:$
 * @file		f_cmu_send_matrix.c
 * @brief		cmu_send matrix table
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

/* Recieve State Mng */
const T_CMU_CALLSETRELL_MTXTBL f_cmur_callSetRell_recvTbl[D_CMU_STATENUM][D_CMU_STATENUM] =
{
	{
		/*	Init								*/
		{f_cmu_com_abort		},						/**< Create from INIT process		*/
		{f_cmu_com_abort		},						/**< Thread Init completion notice	*/
	},
	{
		/*	Thread Initializing	waitng			*/
		{f_cmu_com_abort		},						/**< Create from INIT process		*/
		{f_recv_msgRecvSend		},						/**< Thread Init completion notice	*/
	}
};

/* Send State Mng */
const T_CMU_CALLSETRELL_MTXTBL f_cmur_callSetRell_sendTbl[D_CMU_STATENUM][D_CMU_STATENUM] =
{
	{
		/*	Init								*/
		{f_cmu_com_abort		},						/**< Create from INIT process		*/
		{f_cmu_com_abort		},						/**< Thread Init completion notice	*/
	},
	{
		/*	Thread Initializing	waitng			*/
		{f_cmu_com_abort		},						/**< Create from INIT process		*/
		{f_send_msgRecvSend		},						/**< Thread Init completion notice	*/
	}
};

/** @} */

