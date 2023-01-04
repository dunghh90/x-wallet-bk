/*!
 * @skip  $ld:$
 * @file  f_dpda_abort.c
 * @brief DPDAタスク Abort処理
 * @date  2013/03/07 ALPHA)松延 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"

/*!
 * @brief 関数機能概要:Abort処理を行う。
 * @note  関数処理内容.
 *		-# Abort処理を実施。(f_com_abort() 関数Call)
 *		-# 固定で正常を上位に返却する。
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return INT
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @retval D_SYS_OK		0:正常終了
 * @date 2013/03/07 ALPHA)松延 Create
 * @date 2013/11/26 ALPHA)加藤 ZYNQ対応
 */
INT f_dpda_abort(VOID* rcvMsg_p)
{
	INT						rtn;			/*	処理結果格納変数		*/
	T_SYS_COMMON_THDIF*		msgPtr;			/*	メッセージ変数			*/

	rtn = D_SYS_OK;

	/*	メッセージを取得	*/
	msgPtr = (T_SYS_COMMON_THDIF*)rcvMsg_p;
	
	/* assert処理 */
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "dpda task error 0x%08x", msgPtr->uiEventNo );

	/*	abort処理	*/
	/* f_com_abort(D_SYS_THDID_PF_CCA, D_DU_ALMCD_TIF); */

	return rtn;
}

/* @} */
