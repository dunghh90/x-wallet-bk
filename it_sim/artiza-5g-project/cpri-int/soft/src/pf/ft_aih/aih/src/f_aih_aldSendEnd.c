/**
 *  @skip	$Id:$
 *  @file	f_aih_aldSendend.c
 *  @brief  5GDU-LLS ALD transmission completion function
 *  @date   2019/08/01 FJT)Takahashi create
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */

/** @addtogroup RRH_PF_AIH
* @{ */

#include "f_aih_inc.h"			/* 5GDU-LLS AIHタスクヘッダファイル */

/**
* @brief 5GDU-LLS ALD transmission completion function
* @note  ALD(AISG) interface function.\n
* @param  *pRcvMsg   [in]    received message buffer
* @return VOID
* @retval none
* @date 2019/08/01 FJT)Takahashi create
* @warning	N/A
* @FeatureID	5GDU-M-002
* @Bug_No	N/A
* @CR_No	N/A
* @TBD_No	N/A
*
*/
VOID f_aih_aldSendEnd( VOID *pRcvMsg )
{
	/* Update task state to ALD receive state */
	f_aih_thrdstate = E_AIH_STAT_ALDRCV;

	return ;
}

/* @} */

