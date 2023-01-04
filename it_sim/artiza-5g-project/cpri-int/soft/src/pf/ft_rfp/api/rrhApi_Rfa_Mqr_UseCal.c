/*!
 * @skip  $ld:$
 * @file  rrhApi_Rfa_Mqr_UseCal.c
 * @brief API : RF操作関連
 * @date  2018/08/14 KCN)takagi Create
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
 *  @brief  API :運用中のTxCalRxCal要求
 *  @note   PF EVENT ID           : 0xA0010007
 *          Reponse  ID : -
 *          TYPE        : MNT
 *  @param  qid                   : response queue id (応答送信先QueueID)
 *  @param  wtime                 : wait time(msec) for response (応答待ち時間。即時応答型でのみ有効)
 *  @param  *data_p               : 取得データポインタ
 *  @param  txrx_flag             : TxCal or RxCal要求フラグ(0:TxCal 1:RxCal)
 *  @return E_RRHAPI_RCODE
 *  @retval E_API_RCD_ERR_BPF_CALL
 *  @retval E_API_RCD_EPARAM
 *  @retval E_API_RCD_ENOMSG
 *  @retval E_API_RCD_OK
 *  @warning	N/A
 *  @FeatureID	PF-Rfp-002-001-001
 *  @Bug_No		N/A
 *  @CR_No		N/A
 *  @TBD_No		N/A
 * @date  2018/08/14 KCN)takagi Create
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Rfa_Mqr_UseCal(INT qid, INT wtime, VOID *data_p,
												USHORT txrx_flag)
{

	
	return E_API_RCD_OK;
}

/** @} */