/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file     m_mk_RENgcodeTrans.c
 *  @brief   Thansform NG code of common function to NG code of  RE
 *  @date   2008/07/25 FFCS)zhengmh create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup RRH_MNT_MKM
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/

#include "m_cm_header.h"			/*  TRA common head file							*/
#include "m_mk_header.h"			/*	Maker MT management task header 	*/

/********************************************************************************************************************/
/**
 *  @brief     Thansform NG code of common function to NG code of  RE 
 *  @note     Thansform NG code of common function to NG code of  RE
 *  @param  a_ret		[in]	Common function return code
 *  @return   return NG code
 *  @retval   MKD_RES_KINDERR			0x14 : kind error
 *  @retval   MKD_RES_TIMEOUT			0x15 : time out		
 *  @retval   MKD_RES_PARITYERR		0x16 : parity error
 *  @retval   MKD_RES_RWCHKERR		0x17 : R/W check		
 *  @retval   MKD_RES_TRASTA_NG		0x1D : TRA status error
 *  @retval   MKD_RES_TRAUNINS		0x13 : TRA uninstall
 *  @retval   MKD_RES_PARAMERR		0x11 : parameter error
 *  @retval   MKD_RES_FAILED 			0x12 : failed process
 *  @date     2008/07/25 FFCS)zhengmh create
 */
/********************************************************************************************************************/



UINT	m_mk_REMTNgcodeTrans(UINT a_ret)
{
	UINT	a_ngcode;

	if((a_ret & 0xF0000000)  != 0xF0000000)
	{
		a_ngcode = a_ret;
		return(a_ngcode);
	}
	switch(a_ret)
	{
		/* 指定種別誤り					*/
		case CMD_RES_KINDERR:
			a_ngcode = MKD_RES_KINDERR;
			break;
		/* 処理タイムアウト				*/
		case CMD_RES_TIMEOUT:
			a_ngcode = MKD_RES_TIMEOUT;
			break;
		/* パリティエラー				*/
		case CMD_RES_PARITYERR:
			a_ngcode = MKD_RES_PARITYERR;
			break;
		/* R/WチェックNG				*/
		case CMD_RES_RWCHKERR:
			a_ngcode = MKD_RES_RWCHKERR;
			break;
		/* RE status error */
		case CMD_RES_RE_TRASTAERR:
			a_ngcode = MKD_RES_TRASTA_NG;
			break;
		/* RE not install */
		case CMD_RES_RE_TRAUNINS:
			a_ngcode = MKD_RES_TRAUNINS;
			break;
		/* パラメータ指定誤り			*/
		case CMD_RES_PARAMERR:
			a_ngcode = MKD_RES_PARAMERR;
			break;
		/* 処理失敗						*/
		default:
			a_ngcode = MKD_RES_FAILED;
			break;
	}
	return(a_ngcode);
}
/* @} */
