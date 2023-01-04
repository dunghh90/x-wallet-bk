/*!
 * @skip 		$Id:$
 * @file 		f_pfm_typ.h
 * @brief		PFM thread type header
 * @date		2018/11/23 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_PFM

 *  @{
 */
#ifndef F_PFM_TYP_H
#define F_PFM_TYP_H

/****************************************************************************************************************/
/*  Information of PFM Thread																					*/
/****************************************************************************************************************/
/*!
 * @name Information of PFM Thread
 * @note Information of PFM Thread
 * @{
 */
typedef struct{
	UINT							thdState;							/* Thread状態			*/
}T_PFM_INFO;
/* @} */

/****************************************************************************************************************/
/*  Matrix function of PFM Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of PFM Thread
 * @note	Matrix function of PFM Thread
 * @{
 */
typedef struct{
	INT								(*eventProcess_addr)(VOID *msgPtr);	/* マトリクス関数		*/
}T_PFM_CALLSETRELL_MTXTBL;
/* @} */


/*!
 * @name	Matrix function of PFM Thread
 * @note	Matrix function of PFM Thread
 * @{
 */
typedef struct{
	UINT				regAddr;
	UINT				pfmType;
}T_PFM_REG_INFO;
/* @} */


/* @} */

#endif
/** @} */
