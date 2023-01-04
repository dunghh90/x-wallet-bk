/*!
 * @skip  $ld:$
 * @file  f_dpda_act_txrxCal_inUse.c
 * @brief DPDAタスク The request of TxCal RxCal in use受信処理
 * @date 2018/08/13 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_dpda_inc.h"


/*!
 * @brief 関数機能概要:Tx/Rxキャリブレーション処理の入り口(sub6)
 * @note  関数処理内容.
 *		-# メッセージからTx/Rxキャリブレーションのパラメータを取得
 *		-# TxCAL/RxCALの判定をし、キャリブレーション処理を呼び出す
 * @param  msgP			[in]VOID*		The request of TxCal RxCal in use
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @warning		N/A
 * @FeatureID	N/A
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No
 * @date 2018/08/13 KCN)takagi Create
 */
INT f_dpda_act_txrxCal_inUse( VOID* msgP )
{
	INT                          ret                    = D_SYS_OK;

	return ret;
}
/* @} */

