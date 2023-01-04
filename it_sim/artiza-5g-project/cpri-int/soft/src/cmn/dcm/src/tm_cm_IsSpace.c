/****************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	tm_cm_IsSpace.c
 *  @brief	空白文字かどうかを調べる
 *  @date	2010/02/28	QNET)Nagasima create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010-
 */
/****************************************************************************/

/** @addtogroup TRA_COM
 * @{ */

/****************************************************************************/
/* include file																*/
/****************************************************************************/

#include "m_cm_header.h" /* TRA common head file */


/****************************************************************************/
/**
 *  @brief	空白文字かどうかを調べる
 *  @note	空白文字かどうかを調べる
 *  @param	c			[in]	文字
 *  @param	FileNamep	[in]	ファイル名
 *  @param	LineNo		[in]	行番号
 *  @retval	CMD_TRUE	1 : 真
 *  @retval	CMD_FALSE	0 : 偽
 *  @return	結果
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date	2010/02/28	QNET)Nagasima create
 *  @date   2013/11/26 alpha)Kondou	zynq化対応
 */
/****************************************************************************/

INT cmR_IsSpace(		/* R:結果 */
	INT			c,		/* I:文字 */
	const VOID* FileNamep,
	UINT		LineNo
) {
	INT own_ret;

	/* 空白文字の場合 */
	if (((CHAR)c == ' ' ) ||
	    ((CHAR)c == '\t') ||
	    ((CHAR)c == '\n') ||
	    ((CHAR)c == '\v') ||
	    ((CHAR)c == '\f') ||
	    ((CHAR)c == '\r')) {
		own_ret = CMD_TRUE;
	}
	/* 空白文字じゃない場合 */
	else {
		own_ret = CMD_FALSE;
	}

	return own_ret;
}
/* @} */
