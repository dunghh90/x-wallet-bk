/*!
 * @skip  $ld:$
 * @file  f_rfcp_act_systemParamUpdate.c
 * @brief pf_rfcpスレッド システムパラメータ更新
 * @date  2013/11/24 ALPHA)藤井 Create
 * @date  2014/12/01 ALPHA)中村 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2014
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_rfcp_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_rfcp_act_systemParamUpdate
 * @note		システムパラメータ更新受信時の処理を行う
 *				-# pf_dpdaに"System Parameter Update"を送信
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-008-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/11/24 ALPHA)藤井 Create for docomo zynq
 * @date  		2014/12/01 ALPHA)中村 ppc対応
 * @date  		2015/04/22 ALPHA)藤原 TDD対応
 */
/****************************************************************************************************************************/
INT f_rfcp_act_systemParamUpdate(VOID* msgP)
{
	
	return D_SYS_OK;
}

/* @} */
