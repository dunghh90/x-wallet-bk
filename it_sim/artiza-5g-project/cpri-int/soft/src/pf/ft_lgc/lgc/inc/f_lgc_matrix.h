/******************************************************************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		f_lgc_matrix.h
 *  @brief		Definitions of state change matrix
 *  @note		NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 				See also StateTransitionTable(log).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010
 */
/******************************************************************************************************************************/

#ifndef	F_LGC_MATRIX_H
#define	F_LGC_MATRIX_H


/** @addtogroup RRH_PF_LGC
 *  @{
 */

/******************************************************************************************************************************/
/** LOG State Mng
 *  @addtogroup RRH_PF_LGC
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	callSetRell
 * @note	LOG State Mng
 * 
 */
typedef struct{
	INT (*eventProcess_addr)(VOID *msgPtr);
}T_LGC_CALLSETRELL_MTXTBL;

/** @} */



#endif	/* F_LGC_MATRIX_H */
/** @} */
