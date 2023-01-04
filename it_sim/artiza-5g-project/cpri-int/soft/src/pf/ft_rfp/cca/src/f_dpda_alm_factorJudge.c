/*!
 * @skip  $ld:$
 * @file  f_dpda_alm_factorJudge.c
 * @brief �A���[���v�����菈��
 * @date  2011/07/19 ALP)���� Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2011-2013
 */

/*!
 * @addtogroup RRH_DPDA
 * @{
 */
#include "f_dpda_inc.h"

/*!
 * @brief �֐��@�\�T�v:�A���[���v�����菈��
 * @note  �֐��������e.
 *	-# �A���[�����艉�Z�q�ɏ]���āA�A���[�����茋�ʂ����o��
 * @param  almJdgInfo1		[in]UINT	�A���[��������1
 * @param  almJdgInfo2		[in]UINT	�A���[��������1
 * @param  almOperator		[in]UINT	�A���[�����艉�Z�q
 * @param  almResult		[out]UCHAR	�A���[�����茋��(ON,OFF)
 * @return VOID
 * @date 2011/07/19 ALPHA)���� Create
 * @date 2011/10/20 ALPHA)���� M-RRU-ZSYS-00693�Ώ�
 * @date 2011/11/15 ALPHA)���� M-RRU-ZSYS-00721 �Ώ�(�A���[���d�l�ύX)
 * @date 2012/04/02 ALPHA)���� M-RRU-ZSYS-00840�Ώ�(IT1�⏈No.2)
 * @date 2013/06/03 ALPHA)���� B25�Ή�
 */
VOID f_dpda_alm_factorJudge(	UINT	almJdgInfo1,
								UINT	almJdgInfo2,
								UINT	almOperator,
								UCHAR*	almResult)
{
	UINT		almFactorResult;
	
	almFactorResult=0;

	/* �A���[�����艉�Z�q�ɂ�蕪�� */
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

	/* �A���[�����A���Z�q��p���Ĕ��肵�����ʂ��A�E�g�p���ɐݒ� */
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
