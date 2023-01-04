/*!
 * @skip 		$Id:$
 * @file 		f_fhs_typ.h
 * @brief		FHS thread type header
 * @date		2018/11/23 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_FHS

 *  @{
 */
#ifndef F_FHS_TYP_H
#define F_FHS_TYP_H

/****************************************************************************************************************/
/*  Information of FHS Thread																					*/
/****************************************************************************************************************/
/*!
 * @name Information of FHS Thread
 * @note Information of FHS Thread
 * @{
 */
typedef struct{
	UINT							thdState;							/* Thread���			*/
}T_FHS_INFO;
/* @} */

/****************************************************************************************************************/
/*  Matrix function of FHS Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of FHS Thread
 * @note	Matrix function of FHS Thread
 * @{
 */
typedef struct{
	INT								(*eventProcess_addr)(VOID *msgPtr);	/* �}�g���N�X�֐�		*/
}T_FHS_CALLSETRELL_MTXTBL;
/* @} */

/****************************************************************************************************************/
/*  Time information for SAA response																			*/
/****************************************************************************************************************/
/*!
 * @name	Time information for SAA response
 * @note	Time information for SAA response
 * @{
 */
typedef struct{
	USHORT				year;								/**< �N  */
	UCHAR				month;								/**< ��  */
	UCHAR				day;								/**< ��  */
	UCHAR				hour;								/**< ��  */
	UCHAR				min;								/**< ��  */
	UCHAR				sec;								/**< �b  */
	UCHAR				dummy;								/**< dummy  */
}T_FHS_STARTUP_TIME;
/* @} */

#endif
/** @} */
