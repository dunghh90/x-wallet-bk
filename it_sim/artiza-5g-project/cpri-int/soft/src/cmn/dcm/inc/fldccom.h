/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	fldcCom.h
 *  @brief	fldc return value definition table
 *  @date   2005/09/27 平田	MB01-002-001-203/MB共通部品化
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*****************************************************************************/

#ifndef __INCL_FLDCCOM_H
#define	__INCL_FLDCCOM_H
	
/** @addtogroup TRA_COM
 *  @{
 */
 
/* for MB	return value */
#define PARA_NG		 9	/**< parameter value invalid						*/
/* for fldc return value */
#define NORMAL		 0	/**< normal (used in fldc1,2,3) 					*/
#define BADCODE 	 1	/**< corrupted encoded data (used in fldc1,2,3) 	*/
#define BUFFERFULL	 2	/**< buffuer full (used in fldc1,2) 				*/
#define BUFFEREMPTY  3	/**< buffer is empty (used in fldc3) 				*/
#define DetectEOF	 4	/**< detect EOF (used in fldc1,2) 				*/
#define ERR_FATAL	 5	/**< FATAL error(used in fldc1,2,3)  				*/

/* @} */
#endif
