/*!
 * @skip  $ld:$
 * @file  f_rfcp_com_ParamCheck.c
 * @brief RFCPタスク 共通処理
 * @date  2015/01/05 ALPHA) 中村 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_rfcp_inc.h"

/*!
 * @brief 関数機能概要:キャリアIDの範囲チェックを行う
 * @note  関数処理内容.
 *		-# キャリアIDの範囲チェックを行う
 * @param  carId	[in]	キャリアID
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date  2015/01/05 ALPHA) 中村 Create
 * @date  2015/04/22 ALPHA) 藤原 TDD対応
 */
INT f_rfcp_com_CarrierCheck(UINT carId)
{
	
	/* パラメータチェック */
	if( carId >= D_SYS_CAR_MAX )
	{
		/* assert log	*/
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "carrierId out of range. %d", carId);
		return D_SYS_NG;
	}
	
	return D_SYS_OK;
}

/*!
 * @brief 関数機能概要:キャリアIDの範囲チェックを行う
 * @note  関数処理内容.
 *		-# キャリアIDの範囲チェックを行う
 * @param  branch		[in]	ブランチID
 * @return INT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date  2015/01/05 ALPHA) 中村 Create
 * @date  2015/04/22 ALPHA) 藤原 TDD対応
 */
INT f_rfcp_com_BranchCheck(UINT branch)
{
	
	/* パラメータチェック */
	if( branch >= D_SYS_ANT_MAX )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "branch number out of range %d", branch);
		return D_SYS_NG;
	}
	
	return D_SYS_OK;
}


/* @} */
