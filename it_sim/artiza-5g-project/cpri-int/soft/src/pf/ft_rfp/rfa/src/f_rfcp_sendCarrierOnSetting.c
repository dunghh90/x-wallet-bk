/************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_rfcp_sendCarrierOnSetting.c
 * @brief 共通関数 Carrier On Setting Request送信処理
 * @date  2013/03/26 ALPHA)藤井 Create (base f_com_vupu_sendTrxSettingReq_LTE.c)
 * @date  2014/12/01 ALPHA)中村 ppc対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2014
 */
/************************************************************************************************************/

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include "f_rfcp_inc.h"
#include "f_sys_type.h"

/************************************************************************************************************/
/*!
 * @brief		f_rfcp_sendCarrierOnSetting
 * @note		Carrier On Setting Request送信処理を行う
 *				-# Carrier On Setting Request を作成し、DPDAスレッドへ送信する
 * @param		*pCarSetting	[in]T_SYS_CARRIER_ON_SETTING_REQ_INFO*	Carrier On Setting用の構造体
 * @param		uiSrcPQId		[in]UINT								送信元ProcessQID
 * @param 		uiSrcTQId		[in]UINT								送信元ThreadQID
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-002-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/04/26 ALPHA)藤井 Create  (base: f_com_vupu_sendTrxSettingReq_LTE)
 * @date		2013/11/22 ALPHA)藤井 Modify for docomo zynq
 * @date		2014/12/01 ALPHA)中村 ppc対応
 * @date		2015/04/22 ALPHA)藤原 TDD対応
 */
/************************************************************************************************************/
UINT f_rfcp_sendCarrierOnSetting(	T_SYS_CARRIER_ON_SETTING_REQ_INFO	*pCarSetting,
									UINT								uiSrcPQId,
									UINT								uiSrcTQId )
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_rfcp_sendCarrierOnSetting called");

	T_SYS_CARRIER_ON_SETTING_REQ*		sndCarOnReq;
	INT									ret;
	INT									errcd;

	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_CARRIER_ON_SETTING_REQ),	/* Size								*/
										(VOID **)&sndCarOnReq,					/* msgP								*/
										&errcd );
	
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed get process message address. %d", errcd);
		return D_SYS_NG;
	}
	
	/* 送信MSG作成	*/
	sndCarOnReq->head.uiEventNo 	= D_SYS_MSGID_CARRIER_ON_SETTING_REQ;		/* キャリア設定要求					*/
	sndCarOnReq->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;							/* 送信先Process Queue ID			*/
	sndCarOnReq->head.uiDstTQueueID	= D_SYS_THDQID_PF_CCA;						/* 送信先Thread Queue ID			*/
	sndCarOnReq->head.uiSrcPQueueID	= uiSrcPQId;								/* 送信元Process Queue ID			*/
	sndCarOnReq->head.uiSrcTQueueID	= uiSrcTQId;								/* 送信元Thread Queue ID			*/
	
	memcpy( &(sndCarOnReq->carrierOnSettingReqInfo), pCarSetting, sizeof(T_SYS_CARRIER_ON_SETTING_REQ_INFO) );
	
	/* キャリア設定を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_CCA,						/* 送信先Thread Queue ID			*/
									(void *)sndCarOnReq );						/* 送信Message						*/
	
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", ret);
		return D_SYS_NG;
	}

	return D_SYS_OK;
}

/* @} */
