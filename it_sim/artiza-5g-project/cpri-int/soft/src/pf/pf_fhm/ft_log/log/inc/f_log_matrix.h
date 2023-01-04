/******************************************************************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		f_log_matrix.h
 *  @brief		Definitions of state change matrix
 *  @note		NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 				See also StateTransitionTable(log).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010
 */
/******************************************************************************************************************************/

#ifndef	F_LOG_MATRIX_H
#define	F_LOG_MATRIX_H


/** @addtogroup RRH_PF_LOG
 *  @{
 */

/******************************************************************************************************************************/
/** LOG State Mng
 *  
 *  
 */
/******************************************************************************************************************************/
/**
 * @brief	callSetRell
 * @note	LOG State Mng
 * 
 */
typedef struct{
	INT (*eventProcess_addr)(VOID *msgPtr);
}T_LOG_CALLSETRELL_MTXTBL;





#endif	/* F_LOG_MATRIX_H */
/** @} */
