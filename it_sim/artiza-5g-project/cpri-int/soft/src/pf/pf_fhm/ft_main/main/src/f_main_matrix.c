/*!
 * @skip		$ld:$
 * @file		f_main_matrix.c
 * @brief		pf_main matrix table
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/* MAIN State Mng */
const T_MAIN_CALLSETRELL_MTXTBL f_mainr_callSetRell_mtxtbl[D_MAIN_STATENUM][D_MAIN_EVENTNUM] __attribute__ ((aligned (16))) =
{
	{
		/*	Init								*/
		{f_main_init			},														/**< Create from INIT process		*/
		{f_main_abort			},														/**< Thread Init completion notice	*/
	},
	{
		/*	Thread Initializing	waitng			*/
		{f_main_abort			},														/**< Create from INIT process		*/
		{f_main_iniWait			},														/**< Thread Init completion notice	*/
	}
};

/** @} */

