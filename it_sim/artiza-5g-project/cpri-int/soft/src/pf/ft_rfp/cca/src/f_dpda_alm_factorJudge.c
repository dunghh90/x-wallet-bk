/*!
 * @skip  $ld:$
 * @file  f_dpda_alm_factorJudge.c
 * @brief アラーム要因判定処理
 * @date  2011/07/19 ALP)藤井 Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2011-2013
 */

/*!
 * @addtogroup RRH_DPDA
 * @{
 */
#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:アラーム要因判定処理
 * @note  関数処理内容.
 *	-# アラーム判定演算子に従って、アラーム判定結果を取り出す
 * @param  almJdgInfo1		[in]UINT	アラーム判定情報1
 * @param  almJdgInfo2		[in]UINT	アラーム判定情報1
 * @param  almOperator		[in]UINT	アラーム判定演算子
 * @param  almResult		[out]UCHAR	アラーム判定結果(ON,OFF)
 * @return VOID
 * @date 2011/07/19 ALPHA)藤井 Create
 * @date 2011/10/20 ALPHA)藤井 M-RRU-ZSYS-00693対処
 * @date 2011/11/15 ALPHA)藤井 M-RRU-ZSYS-00721 対処(アラーム仕様変更)
 * @date 2012/04/02 ALPHA)藤井 M-RRU-ZSYS-00840対処(IT1問処No.2)
 * @date 2013/06/03 ALPHA)中村 B25対応
 */
VOID f_dpda_alm_factorJudge(	UINT	almJdgInfo1,
								UINT	almJdgInfo2,
								UINT	almOperator,
								UCHAR*	almResult)
{
	UINT		almFactorResult;
	
	almFactorResult=0;

	/* アラーム判定演算子により分岐 */
	switch( almOperator )
	{

	/* AND */
	case E_DPDA_ALM_OPE_AND:
		almFactorResult = (almJdgInfo1 & almJdgInfo2);
		break;

	/* OR */
	case E_DPDA_ALM_OPE_OR:
		almFactorResult = (almJdgInfo1 | almJdgInfo2);
		break;

	/* greater than */
	case E_DPDA_ALM_OPE_GT:
		if( almJdgInfo1 > almJdgInfo2)
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* greater than or equal to */
	case E_DPDA_ALM_OPE_GE:
		if( almJdgInfo1 >= almJdgInfo2)
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* less than */
	case E_DPDA_ALM_OPE_LT:
		if( almJdgInfo1 < almJdgInfo2)
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* less than or equal to */
	case E_DPDA_ALM_OPE_LE:
		if( almJdgInfo1 <= almJdgInfo2)
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* equal to */
	case E_DPDA_ALM_OPE_EQ:
		if( almJdgInfo1 == almJdgInfo2 )
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* not equal to */
	case E_DPDA_ALM_OPE_NE:
		if( almJdgInfo1 != almJdgInfo2 )
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* bit off */
	case E_DPDA_ALM_OPE_BITOFF:
		if( (almJdgInfo1 & almJdgInfo2) == D_SYS_OFF )
		{
			almFactorResult=D_SYS_ON;
		}
		break;
		
	/* bit on */
	case E_DPDA_ALM_OPE_BITON:
		if( (almJdgInfo1 & almJdgInfo2) != 0 )
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* greater than or equal to(signed) */
	case E_DPDA_ALM_OPE_GE_SIGNED:
		if( (INT)almJdgInfo1 >= (INT)almJdgInfo2)
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* greater than(signed) */
	case E_DPDA_ALM_OPE_GT_SIGNED:
		if( (INT)almJdgInfo1 > (INT)almJdgInfo2)
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* less than or equal to(signed) */
	case E_DPDA_ALM_OPE_LE_SIGNED:
		if( (INT)almJdgInfo1 <= (INT)almJdgInfo2)
		{
			almFactorResult=D_SYS_ON;
		}
		break;

	/* less than(signed) */
	case E_DPDA_ALM_OPE_LT_SIGNED:
		if( (INT)almJdgInfo1 < (INT)almJdgInfo2)
		{
			almFactorResult=D_SYS_ON;
		}
		break;
		
	default:
		break;
	}

	/* アラーム情報、演算子を用いて判定した結果をアウトパラに設定 */
	if( almFactorResult )
	{
		*almResult = D_SYS_ON;
	}
	else
	{
		*almResult = D_SYS_OFF;
	}
	
	return;
}

/* @} */
