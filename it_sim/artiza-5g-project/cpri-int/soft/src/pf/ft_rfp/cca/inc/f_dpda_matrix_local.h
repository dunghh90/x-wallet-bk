/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	External Definition of state change matrix 
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also StateTransitionTable(dpda).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2011-2015
 */
/******************************************************************************************************************************/

#ifndef	F_DPDA_MATRIX_LOCAL_H
#define	F_DPDA_MATRIX_LOCAL_H

/** @addtogroup RRH_PF_RFP
 *  @{
 */
#include "f_dpda_matrix.h"

extern	const	T_DPDA_CALLSETRELL_MTXTBL	f_dpdar_callSetRell_mtxtbl_sub6[D_DPDA_STATENUM][E_DPDA_EVENTNUM];
															/**< DPDA State MngSTM  */

extern	const	T_DPDA_CALLSETRELL_MTXTBL	f_dpdar_callSetRell_mtxtbl_mmW[D_DPDA_STATENUM][E_DPDA_EVENTNUM];

#endif	/* F_DPDA_MATRIX_LOCAL_H */
/** @} */
