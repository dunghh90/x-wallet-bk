/************************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_rfcp_sendCarrierOffSetting.c
 * @brief 共通関数 Carrier Off Setting Request送信処理
 * @date  2013/03/26 ALPHA)藤井 Create (base: f_com_vupu_sendTrxSettingReq_LTE.c)
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
 * @brief		f_rfcp_sendCarrierOffSetting
 * @note		Carrier Off Setting Request送信処理を行う
 *				-# Carrier Off Setting Request を作成し、DPDAスレッドへ送信する
 * @param		*pCarInfo		[in]T_SYS_CARRIER_OFF_SETTING_REQ_INFO*	Carrier Off Setting用の構造体
 * @param		uiSrcPQId		[in]UINT								送信元ProcessQID
 * @param 		uiSrcTQId		[in]UINT								送信元ThreadQID
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-002-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/04/26 ALPHA)藤井 Create  (base: f_com_vupu_sendTrxSettingReq_LTE)
 * @date		2013/11/22 ALPHA)藤井 Modify for docomo zynq
 * @date  		2014/12/01 ALPHA)中村 ppc対応
 * @date  		2015/04/22 ALPHA)藤原 TDD対応
 */
/************************************************************************************************************/
UINT f_rfcp_sendCarrierOffSetting(	T_SYS_CARRIER_OFF_SETTING_REQ_INFO	*pCarInfo,
									UINT								uiSrcPQId,
									UINT								uiSrcTQId )
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_rfcp_sendCarrierOffSetting called");

	T_SYS_CARRIER_OFF_SETTING_REQ*		sndCarOffReq;
	INT									ret;
	INT									errcd;

	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_CARRIER_OFF_SETTING_REQ),	/* Size								*/
										(VOID **)&sndCarOffReq,					/* msgP								*/
										&errcd );
	
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed get process message address. %d", errcd);
		return D_SYS_NG;
	}
	
	/* 送信MSG作成	*/
	sndCarOffReq->head.uiEventNo		= D_SYS_MSGID_CARRIER_OFF_SETTING_REQ;	/* キャリア設定応答					*/
	sndCarOffReq->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* 送信先Process Queue ID			*/
	sndCarOffReq->head.uiDstTQueueID	= D_SYS_THDQID_PF_CCA;					/* 送信先Thread Queue ID			*/
	sndCarOffReq->head.uiSrcPQueueID	= uiSrcPQId;							/* 送信元Process Queue ID			*/
	sndCarOffReq->head.uiSrcTQueueID	= uiSrcTQId;							/* 送信元Thread Queue ID			*/
	
	memcpy( &(sndCarOffReq->carrierOffSettingReqInfo), pCarInfo, sizeof(T_SYS_CARRIER_OFF_SETTING_REQ_INFO) );
	
	/* キャリア設定を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_CCA,						/* 送信先Thread Queue ID			*/
									(void *)sndCarOffReq );						/* 送信Message						*/
	
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", errcd);
		return D_SYS_NG;
	}
	
	return D_SYS_OK;
}

/* @} */
