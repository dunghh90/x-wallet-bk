/*!
 * @skip  $ld:$
 * @file  f_eio_apiMsgRecv.c
 * @brief API message receive proc.
 * @date 2019/03/18 FJT)Koshida create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019-
 */
    
/*!
 * @addtogroup RRH_PF_EIO
 * @{
 */
#include "f_eio_header.h"			/* 5GDU-LLS外部IOタスクヘッダファイル			*/

/*!
 *  @brief  Setting FAN cntrol debug mode for API message(API)
 *  @note   This function set FAN cntrol debug mode in shared memory.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/03/18 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eio_apiFanDbgInd( VOID *pRcvMsg )
{
	/*	デバッグモードを設定する	*/
	f_eiow_svctrl_mngp->fanCtrl.fanDbgFlg = D_RRH_ON;
	
	/*	外部IO制御履歴ログ	*/
	f_eio_extIoSvCtrlLog("[API]FAN ctrl dbg mode ON");
	return ;
}


/*!
 *  @brief  Setting FAN cntrol debug mode for API message(API)
 *  @note   This function set FAN cntrol debug mode in shared memory.\n
 *  @param  *pRcvMsg   [in]    received message buffer
 *  @return VOID
 *  @retval none
 *  @date 2019/03/18 FJT)Koshida create
 *  @warning	N/A
 *  @FeatureID	5GDU-002-007
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_eio_apiFanDacInd( VOID *pRcvMsg )
{
	T_API_EXT_FAN_DAC_CHG_NTC	*apiMsgp;
	UINT						a_stsNo;
	UINT						a_dacVal;
	
	apiMsgp  = (T_API_EXT_FAN_DAC_CHG_NTC *)pRcvMsg;
	a_stsNo  = apiMsgp->fanDacInfo.dacNo;
	a_dacVal = apiMsgp->fanDacInfo.dacSetVal;
	
	/*	設定パラメータが範囲外の場合	*/
	if(a_stsNo >= E_DU_FANSPEEDSTS_MAXNO)
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "fan sts Error (0x%08x, 0x%08x)", a_stsNo, a_dacVal );
	}
	/*	指定した状態でのFAN DAC設定値を変更する	*/
	f_eiow_svctrl_mngp->fanCtrl.fanDacParam[a_stsNo].fanDacVal = a_dacVal;
	
	/*	外部IO制御履歴ログ	*/
	f_eio_extIoSvCtrlLog("[API]FAN Speed StsNo=%d FANDAC=0x%x", a_stsNo, a_dacVal);
}

/* @} */

