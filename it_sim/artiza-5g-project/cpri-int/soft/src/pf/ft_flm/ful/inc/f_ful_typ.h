/*!
 * @skip  $ld:$
 * @file  f_ful_typ.h
 * @brief FULタスク 構造体宣言
 * @date 2019/01/28 KCN)上本 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_FLD
 * @{
 */
#ifndef F_FUL_TYP_H
#define F_FUL_TYP_H

/*!
 * @addtogroup RRH_PF_FLM
 * @{
 */
typedef struct{
	UINT					thdState;												/* Thread State						*/
}T_FUL_INFO;

typedef struct{
	INT (*eventProcess_addr)(VOID *msgPtr);
}T_FUL_CALLSETRELL_MTXTBL;





/* @} */


#endif
/* @} */
