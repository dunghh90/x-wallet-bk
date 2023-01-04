/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Table Define
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			...
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013
 */
/******************************************************************************************************************************/

#ifndef	F_MAIN_RAM_H
#define	F_MAIN_RAM_H

#include "f_rrh_def.h"

/** @addtogroup RRH_PF_MAIN
 *  @{
 */
#define	D_MAIN_DATA_MAX		8168														/**< main Data 最大値 				*/
/** @} */


/**
 * @brief	THREAD ID management Table
 * @note	THREAD ID management Table table is ..
 * 
 */
typedef struct{
	unsigned long int 	thdId[D_SYS_THDID_PF_MAX];										/**< THREAD ID						*/
}T_MAIN_THDID;

/** @} */

#endif	/* F_MAIN_RAM_H */
/** @} */
