/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_ActiveChk.c
 *  @brief	TRA common function - acive link check function 
 *  @date   2008/07/18 FFCS)Tangj create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*****************************************************************************/

/** @addtogroup RRH_DCM
 *  @{
 */
 
/**************************************************************************/
/**
 *  @brief  RA common function - acive link check function
 *  @note   check whether the slef link is active or not \n
 *  @param  sysType			[in]		specified system type
 *  @param  file			[in]		point of file
 *  @param  line			[in]		line no
 *  @return Active status 
 *  @retval CMD_RECLS_ACTSET				0: 	active state
 *  @retval CMD_RECLS_NEGSET				1: 	negative state		
 *  @date   2008/07/22		FFCS)Tangj	new create
 */
/**************************************************************************/

 
#include "m_cm_header.h" /* TRA common header file*/

UINT	m_cmR_ActiveChk(USHORT sysType,VOID *file, UINT line)
{
	/* input parameter check 				*/
	if((sysType != CMD_SYS_3G) && (sysType != CMD_SYS_S3G))
	{
		cm_MAbort(CMD_ALMCD_DATA, 
					"m_cmR_ActiveChk", file, line,sysType , 0);
		
		return(CMD_RECLS_NEGSET);
	}

	return(CMD_RECLS_ACTSET);
}
/* @} */
