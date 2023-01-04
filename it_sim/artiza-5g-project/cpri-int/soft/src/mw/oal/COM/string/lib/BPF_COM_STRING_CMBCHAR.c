/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_COM_STRING_CMBCHAR.c
 *	@brief	Common String Function.
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_COM_STRING /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "BPF_COM_STRING.h"

/** @addtogroup BPF_COM_STRING
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  指定された文字列を結合する。
 *  @note   API function. 
 *  @param  orgdata        [in]  追加元文字列
 *  @param  adddata        [in]  追加文字列
 *  @return 変換文字列
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
char* BPF_COM_STRING_CMBCHAR(char *orgdata, char *adddata)
{	
	return strcat(orgdata,adddata);
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
