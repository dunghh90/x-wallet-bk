/*!
 * @skip  $ld:$
 * @file  f_rfcp_txrxcal_inuse.c
 * @brief pf_rfcpスレッド TxCal RxCal要求処理
 * @date		2018/08/14 KCN)takagi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_rfcp_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_rfcp_txrxcal_inuse
 * @note		TxCal RxCal要求受信時の処理を行う
 *				-# パラメータチェック実施
 *				-# pf_dpdaに"The request of TxCal RxCal in use"を送信
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/08/14 KCN)takagi Create
 */
/****************************************************************************************************************************/
INT f_rfcp_txrxcal_inuse(VOID* msgP)
{
	
	return D_SYS_OK;
}

/* @} */
