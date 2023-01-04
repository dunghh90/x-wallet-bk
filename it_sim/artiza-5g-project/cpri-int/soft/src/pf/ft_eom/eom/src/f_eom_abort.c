/*!
 * @skip 		$ld:$
 * @file 		f_eom_abort.c
 * @brief		pf_eomスレッド Abort処理
 * @date		2018/11/23 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_EOM
 * @{
 */

#include "f_eom_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_eom_abort
 * @note		Abort処理を実行する。
 *		-# Abort処理を実施。				f_com_abort() 関数Call
 *		-# 固定で正常を上位に返却する。
 * @param		*rcvMsg_p	[in]	受信メッセージ
 * @return		INT
 * @retval		D_SYS_OK		0:正常終了
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
INT	f_eom_abort(VOID	*msgP)
{
	INT								rtn = D_SYS_OK;						/* 復帰値				*/

	/* Assertログ出力	*/
	BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_WARNING, 						/* Assertレベル			*/
						"Matrix Function Call NG, sta=%d, event=%08x",	/* message				*/
						f_eomw_thdInfo.thdState,						/* Information			*/
						((T_SYS_COMMON_THDIF *)msgP)->uiEventNo );		/* Information			*/

	/* Abort処理 : タスク間メッセージ異常	*/
	f_com_abort( D_RRH_PROCID_PF, D_DU_ALMCD_TIF );
	return	rtn;
}

/* @} */
