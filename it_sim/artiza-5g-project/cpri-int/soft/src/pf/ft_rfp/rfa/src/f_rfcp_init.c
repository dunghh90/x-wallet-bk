/*!
 * @skip  $ld:$
 * @file  f_rfcp_init.c
 * @brief pf_rfcpスレッド 初期化処理
 * @date  2013/03/07 ALPHA)松延 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/*!
 * @addtogroup RRH_PF_RFP
 * @{
 */

#include "f_rfcp_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_rfcp_init
 * @note		pf_rfcpスレッドの初期化処理を行う
 *				-# pf_rfcpスレッド内部テーブル初期化
 *				-# pf_mainに初期化完了通知(Thread Init completion notice)を送信する(f_com_msgQSend())
 *				-# スレッド状態を"運用状態"に遷移させる
 * @param		*msgP			[in]VOID	受信メッセージ
 * @retval		D_SYS_OK		0:正常終了
 * @retval		D_SYS_NG		1:異常終了
 * @return		INT				実行結果
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/03/07 ALPHA)松延 Create
 * @date		2013/11/22 ALPHA)藤井 Modify for docomo zynq
 */
/****************************************************************************************************************************/
INT f_rfcp_init(VOID* msgP)
{
	T_SYS_INIT_COMP_NTC				*sendMsg;									/* 送信メッセージ(初期化完了通知)	*/
	INT								errcd;										/* 詳細NGコード						*/
	INT								ret;										/* 返り値							*/
	UINT							antCnt;
	UINT							carrloop_cnt;

	/****************************************************************************************************************/
	/* 内部テーブルの初期化																							*/
	/****************************************************************************************************************/
	f_rfcpw_carInfo = &f_rfcpw_carInfo_mem;
	memset( (VOID*)f_rfcpw_carInfo,			0,	sizeof(T_RFCP_CAR_MNG_INFO) );
	
	/* PA状態は初期状態をONとする(PA ON/OFF制御がないため内部的にはONとしておく */
	for( antCnt=0; antCnt<D_SYS_ANT_MAX; antCnt++ )
	{
		f_rfcpw_carInfo->antInfo[antCnt].paState = D_SYS_ON;
	}
	

	/* u-plane conf. state Getテーブル, 最新のedit MIS req idの初期化 */
	for ( antCnt = 0 ; antCnt < D_SYS_ANT_MAX ; antCnt++ )
	{
		for ( carrloop_cnt = 0 ; carrloop_cnt < D_SYS_CAR_MAX ; carrloop_cnt++ )
		{
			/* TXの状態設定 */
			f_upcnf_state[antCnt][carrloop_cnt][0] = E_RFCP_UPCNF_STS_DISABLE ;
			/* RXの状態設定 */
			f_upcnf_state[antCnt][carrloop_cnt][1] = E_RFCP_UPCNF_STS_DISABLE ;
			/* TX/RXのEdit MSIのrequest ID */
			f_upcnf_latest_msi_reqid[antCnt][carrloop_cnt][0] = 0;
			f_upcnf_latest_msi_reqid[antCnt][carrloop_cnt][1] = 0;
		}
	}

	/****************************************************************************************************************/
	/* 初期化完了通知送信																							*/
	/****************************************************************************************************************/
	/* 通信用共有メモリを取得する	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer種別						*/
										sizeof(T_SYS_INIT_COMP_NTC),			/* Size								*/
										(VOID **)&sendMsg,						/* msgP								*/
										&errcd );
	
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed get process message address %d", errcd);
		return D_SYS_NG;
	}
	
	/* 送信MSG作成					*/
	sendMsg->head.uiEventNo		= D_SYS_MSGID_INIT_COMP_NTC;					/* 初期化完了通知					*/
	sendMsg->head.uiDstPQueueID	= D_RRH_PROCQUE_PF;								/* 送信先Process Queue ID			*/
	sendMsg->head.uiDstTQueueID	= D_SYS_THDQID_PF_MAIN;							/* 送信先Thread Queue ID			*/
	sendMsg->head.uiSrcPQueueID	= D_RRH_PROCQUE_PF;								/* 送信元Process Queue ID			*/
	sendMsg->head.uiSrcTQueueID	= D_SYS_THDQID_PF_RFA;							/* 送信元Thread Queue ID			*/
	
	/* pf_mainへ初期化完了通知を送信する */
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_MAIN,						/* 送信先Thread Queue ID			*/
									(void *)sendMsg );							/* 送信Message						*/
	
	if( ret != D_SYS_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "failed send message. %d", ret);
		return D_SYS_NG;
	}
	
	/* スレッド状態遷移(運用状態)	*/
	f_rfcpw_thdInfo.thdState = D_RFCP_THD_ACT;
	
	return D_SYS_OK;
}

/* @} */
