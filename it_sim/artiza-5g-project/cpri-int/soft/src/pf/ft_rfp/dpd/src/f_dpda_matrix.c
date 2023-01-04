/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Table definitions of state change matrix 
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also ...
 * @date 2013/03/07 ALPHA)松延 Create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013
 */
/******************************************************************************************************************************/

#include	"f_dpda_inc.h"

/** @addtogroup RRH_PF_RFP
 *  @{
 */

/* DPDA State Mng */
const T_DPDA_CALLSETRELL_MTXTBL f_dpdar_callSetRell_mtxtbl[D_DPDA_STATENUM][E_DPDA_EVENTNUM] __attribute__ ((aligned (16))) =
{
	{
		/*	init	*/
		{f_dpda_abort_fhm							} 			/**< 周期監視報告					*/
	},
	{
		/*	運用状態状態	*/
		{f_dpda_act_powReportPeriodic_fhm			}			/**< 周期監視報告					*/
	}
};

/** @} */

