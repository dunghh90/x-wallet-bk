/*!
 * @skip		$ld:$
 * @file		f_fmg_typ.h
 * @brief		ログスレッド 構造体宣言
 * @date		2010/05/24 ALPHA)横山 Create
 * @date		2013/03/28 ALPHA)松延 B25 対処
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010
 */

/*!
 * @addtogroup RRH_PF_LGC
 * @{
 */
#ifndef F_LGC_TYP_H
#define F_LGC_TYP_H

/*!
 * @brief Response manage table
 * @note  Response manage table
 * @{
 */
typedef struct
{
	UINT					srcPID;				/* 送信元ProcessID		*/
	UINT					srcTID;				/* 送信元ThreadID		*/
}T_LGC_RSP_MNG_TBL;
/* @} */

#endif
/* @} */
