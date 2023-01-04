/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_ReCardInfRep.c
 *  @brief  response to RE card info report request, carry out RE card info report process
 *  @date   2008/07/25 FFCS)linlj create
 *  @date   2010/02/23 QNET)Kabasima M-S3G-eNBSYS-01338:☆ST_RE☆3G側MTよりLREのカード情報報告読み出し内容NG
 *  @date   2010/05/06 FJT)Otani  CR-00091-001:[DCM向け]REハード・ソフトインタフェース仕様書v2.18版リリース
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
 */
/********************************************************************************************************************/

/** @addtogroup TRA_RCT
 * @{ */

/********************************************************************************************************************/
/* include file																										*/
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "m_rc_header.h"
#include "l3_com_inc.h"

/********************************************************************************************************************/
/**
 *  @brief     Get RE card name process
 *  @note     Get RE card name process
 *  @param    in_cardname [in] the original card name
 *  @param    out_cardname [out] the transformed card name 
 *  @param    arynum [in] the length of return card name 
 *  @return   None
 *  @retval   -
 *  @date     2008/07/25 FFCS)linlj create
 */
/********************************************************************************************************************/
VOID m_rc_GetName(	USHORT*	in_cardname, 
					UCHAR*	out_cardname,
					UINT*	arynum)
{
	UINT   loopcnt;							/* ループカウンタ				*/
	UINT   setcnt;							/* カード名設定カウンタ			*/
	UINT   getname_rslt = CMD_NG;			/* 名称取得処理結果				*/

	/* カード名設定カウンタ初期化 */
	setcnt = CMD_NUM0;
	/* NULLを検索する		*/
	for (loopcnt = CMD_NUM0; loopcnt < RCD_CARDNAME_MAX; loopcnt++)
	{
		out_cardname[setcnt] = (UCHAR)(( in_cardname[loopcnt] & RCD_HI_MSK) >> CMD_BIT8);
		setcnt++;
		out_cardname[setcnt] = (UCHAR)( in_cardname[loopcnt] & RCD_LW_MSK);
		setcnt++;
	}

	/* カード品名の最後は、ハード側でNULLを設定しているため、NULLを検索 */
	for (loopcnt = CMD_NUM0; loopcnt < RCD_MAX_CARDNAME; loopcnt++)
	{
		/* NULLがあった場合、配列数を保持して	*/
		/* ループを抜ける						*/
		if(out_cardname[loopcnt] == 0)
		{
			*arynum  = loopcnt;
			getname_rslt = CMD_OK;
			break;
		}
	}
	/* 獲得結果がNGの場合カード名称最大数(8)に設定 */
	if (getname_rslt == CMD_NG)
	{
		*arynum  = RCD_MAX_CARDNAME;
	}
	return;
}



/* @} */
/* @} */
/* @} */

