/*!
 * @skip  $ld:$
 * @file  f_aih_abort.c
 * @brief AIHタスク Abort処理
 * @date  2019/08/01 FJ)Takahashi Create
 *
 * All Rights Reserved, Copyright FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_AIH
 * @{
 */

#include "f_aih_inc.h"

/*!
 * @brief 関数機能概要:Abort処理を行う。
 * @note  関数処理内容.
 *		-# Abort処理を実施。				f_com_abort() 関数Call
 * @param  *rcvMsg_p	[in]	受信メッセージ
 * @return VOID
 * @retval none
 * @date 2019/08/01 FJ)Takahashi Create
 */
VOID f_aih_abort(VOID* rcvMsg_p)
{
	UINT					eventNo;		/*	イベント番号格納変数	*/
	T_SYS_COMMON_THDIF*		msgPtr;			/*	メッセージ変数			*/

	/*	メッセージを取得	*/
	msgPtr = (T_SYS_COMMON_THDIF*)rcvMsg_p;
	
	/*	イベント番号を取得	*/
	eventNo = msgPtr->uiEventNo;
	
	/* assert処理 */
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_CRITICAL, "aih thread error %08x", eventNo);

	/*	abort処理 */
	f_com_abort(D_RRH_PROCID_PF, D_DU_ALMCD_TIF);
	
	return;
}

/* @} */
