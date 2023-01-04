/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Definitions of state change matrix
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also StateTransitionTable(dpda).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2011-2013
 */
/******************************************************************************************************************************/

#ifndef	F_DPDA_MATRIX_H
#define	F_DPDA_MATRIX_H


/** @addtogroup RRH_PF_RFP
 *  @{
 */

/******************************************************************************************************************************/
/** DPDA State Mng
 *  @addtogroup RRH_PF_RFP
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	callSetRell
 * @note	DPDA State Mng
 * 
 */
typedef struct{
	INT (*eventProcess_addr)(VOID *msgPtr);
}T_DPDA_CALLSETRELL_MTXTBL;

/** @} */

#endif	/* F_DPDA_MATRIX_H */
/** @} */
