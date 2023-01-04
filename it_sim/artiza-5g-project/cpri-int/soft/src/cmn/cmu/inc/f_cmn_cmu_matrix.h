/*!
 * @skip		$ld:$
 * @file		f_cmn_cmu_matrix.h
 * @brief		cmu matrix table
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#ifndef	F_CMU_MATRIX_H
#define	F_CMU_MATRIX_H


/**
 * @brief	callSetRell
 * @note	CMU State Mng
 * 
 */
typedef struct {
	VOID (*eventProcess_addr)(VOID *msgPtr);
}T_CMU_CALLSETRELL_MTXTBL;

/** @} */

extern const 	T_CMU_CALLSETRELL_MTXTBL	f_cmur_callSetRell_recvTbl[D_CMU_STATENUM][D_CMU_STATENUM];
extern const 	T_CMU_CALLSETRELL_MTXTBL	f_cmur_callSetRell_sendTbl[D_CMU_STATENUM][D_CMU_STATENUM];

#endif	/* F_CMU_MATRIX_H */
/** @} */
