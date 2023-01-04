/*!
 * @skip  $ld:$
 * @file  rrhApi_Rfa_com_ParamCheck.c
 * @brief API : RF操作関連
 * @date  2018/8/13 KCN)takagi Create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/** @addtogroup RRH_PF_RFP_API
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "f_sys_def.h"
#include "BPF_RU_IPCM.h"
#include "BPF_COM_LOG.h"
#include "rrhApi_Com.h"
#include "rrhApi_Rfp.h"

/********************************************************************************************************************/
/**
 *  @brief  API共通関数 : キャリア番号の範囲チェック
 *  @param  carrier     : 指定キャリアID
 *  @return E_RRHAPI_RCODE
 *  @retval E_API_RCD_EPARAM
 *  @retval E_API_RCD_OK
 *  @warning	N/A
 *  @FeatureID	PF-Rfp-002-001-001
 				PF-Rfp-003-002-001
 *  @Bug_No		N/A
 *  @CR_No		N/A
 *  @TBD_No		N/A
 *  @date   	2018/8/13  KCN)takagi Create
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Rfa_com_CarrierCheck(USHORT carrier)
{
	INT iRet = E_API_RCD_EPARAM;
	
	/* キャリア番号の範囲チェック */
	if( (D_API_RFP_CARRIER_NO_MIN <= carrier) && (carrier <= D_API_RFP_CARRIER_NO_MAX) )
	{
		iRet = E_API_RCD_OK;
	}
	else
	{
		/* キャリアチェック不正の場合 */
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "CarrierID NG %d", carrier);
	}

	return iRet;
}

/********************************************************************************************************************/
/**
 *  @brief  API共通関数 : ブランチ番号の範囲チェック
 *  @param  branch      : ブランチ番号
 *  @return E_RRHAPI_RCODE
 *  @retval E_API_RCD_EPARAM
 *  @retval E_API_RCD_OK
 *  @warning	N/A
 *  @FeatureID	PF-Rfp-002-001-001
 				PF-Rfp-003-002-001
 *  @Bug_No		N/A
 *  @CR_No		N/A
 *  @TBD_No		N/A
 *  @date   	2015/1/5 ALPHA)nakamura Create.
 *  @date   	2015/4/21 ALPHA)fujiwara TDD対応.
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Rfa_com_BranchCheck(USHORT branch)
{
	/* キャリア設定解放は Tx無し設定もあるため、この範囲チェックは使用しない */
	/* ブランチ番号の範囲チェック */
	if(E_API_RFP_NUM_BRANCH_ALL != branch){
		if( branch > E_API_RFP_NUM_BRANCH_MAX ){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Branch Number NG %d", branch);
			return E_API_RCD_EPARAM;
		}
	}
	return E_API_RCD_OK;
}

/** @} */