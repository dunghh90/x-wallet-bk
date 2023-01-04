/*!
 * @skip		$ld:$
 * @file		f_main_abort.c
 * @brief		pf_main Abort処理
 * @author		alpha)中嶋
 * @date		2013/11/11 alpha)中嶋 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/*!
 * @brief		f_main_abort
 * @note		Abort処理.
 *					-# アボート処理実施(f_com_abort()).
 *					-# 正常終了を戻り値として上位に返却.
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @retval		D_SYS_OK	0:正常終了
 * @retval		D_SYS_NG	1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-MAIN-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/11 alpha)中嶋
 */
UINT f_main_abort(VOID *rcvMsg_p)
{
	INT						rtn;
	UINT					eventNo;
	T_SYS_COMMON_THDIF*		msgPtr;

	rtn = D_SYS_OK;

	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ENTER, "[f_main_abort] ENTER" );

	/* メッセージを取得			*/
	msgPtr = (T_SYS_COMMON_THDIF *)rcvMsg_p;
	
	/* イベント番号を取得		*/
	eventNo = msgPtr->uiEventNo;
	
	/* Write assert log			*/
	BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Thread IF error eventNo=%08x.", eventNo);

	/* abort処理(f_com_abort())	*/
	f_com_abort(D_SYS_THDID_PF_F_MAIN, D_RRH_ALMCD_DIA1);
	
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_RETURN, "[f_main_abort] RETURN" );

	return rtn;
}


/* @} */
