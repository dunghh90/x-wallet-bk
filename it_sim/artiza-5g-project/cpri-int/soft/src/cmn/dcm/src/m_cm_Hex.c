/*******************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	m_cm_Hex.c
 *  @brief  Hex and string operation
 *  @date   2008/10/1	FFCS)Niemsh	 create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/*****************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */
/****************************************************************************/
/* ヘッダインクルード                                                       */
/****************************************************************************/
#include "m_cm_header.h"			/* TRA共通ヘッダファイル			*/

/**************************************************************************/
/**
 *  @brief  Convert string from hex
 *  @note  Convert string to hex \n
 *
 *  @param  str_p			[in]		string 
 *  @param  hex_p		[out]		hex 
 *  @retval   CMD_RES_OK 				0:	success
 *  @retval   CMD_RES_NG 				0xF0000001:	timeout error
 *  @return   Result code is return 
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2008/10/1	FFCS)Niemsh		new editly
 *  @date   2013/09/17	alpha)Nakajima	zynq化対応
 */
/**************************************************************************/

UINT cm_Str2Hex( CHAR *str_p, UINT *hex_p		)			
{
	CHAR			*endptr = "\0";

	if(str_p == NULL || hex_p == NULL)
		return(CMD_RES_NG);
	
	*hex_p = CMD_NUM0;					/* clear hex-data					*/
	if(str_p[CMD_NUM0] == 0)
		return(CMD_RES_NG);
	
	*hex_p = strtol(str_p, &endptr, 16);
	if((errno != 0) || (strcmp(endptr,"\0")!=0) )
	{
		return(CMD_RES_NG);
	}
	return(CMD_RES_OK);
}
/**************************************************************************/
/**
 *  @brief  Convert hex from string
 *  @note  Convert hex to string \n
 *
 *  @param  hex_p			[in]		hex 
 *  @param  str_p			[out]		string 
 *  @param  len				[in]		length  
 *  @retval   CMD_RES_OK 				0:	success
 *  @return   Result code is return 
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2008/10/1	FFCS)Niemsh		new editly
 *  @date   2013/09/17	alpha)Nakajima	zynq化対応
 */
/**************************************************************************/

UINT cm_Hex2Str( CHAR *hex_p, CHAR *str_p,int len)			
{
	if(hex_p == NULL || str_p == NULL || len < CMD_NUM1)
		return(CMD_RES_NG);
	
	if((snprintf(str_p, len, "%X", *hex_p)) < 0)
	{
		return(CMD_RES_NG);
	}
	
	return(CMD_RES_OK);
}
/* @} */
