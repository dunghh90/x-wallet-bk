/*!
 * @skip		$ld:$
 * @file		f_cmu_recv_abort.c
 * @brief		cmu Abort処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_CMU
 * @{
 */

#include "f_cmn_cmu_inc.h"

/*!
 * @brief		f_recv_abort
 * @note		Abort処理.
 *					-# アサートログ取得関数Call(BPF_COM_LOG_ASSERT()).
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @return		N/A
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
VOID f_cmu_com_abort(VOID *rcvMsg_p)
{
	UINT					eventNo;
	T_CMU_MSG_MNG*			msgPtr;

	/* メッセージを取得			*/
	msgPtr = (T_CMU_MSG_MNG*)rcvMsg_p;
	
	/* イベント番号を取得		*/
	eventNo = msgPtr->thdif.uiEventNo;
	
	/* Write assert log			*/
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, (const char*)"Thread IF error EVENT = %08x", eventNo);
}


/* @} */
