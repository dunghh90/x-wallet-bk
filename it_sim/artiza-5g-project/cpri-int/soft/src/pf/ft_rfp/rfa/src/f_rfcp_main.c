/*!
 * @skip  $ld:$
 * @file  f_rfcp_main.c
 * @brief pf_rfcpスレッド メイン処理
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
 * @brief		f_rfcp_main
 * @note		pf_rfcpスレッドメイン。スレッド起動からMSG受信待ちのLoop処理を実施
 *				-# BPFへスレッド情報登録する
 *				-# RFCPタスク状態を"初期状態"に設定する
 *				-# 初期化関数をCallする(f_rfcp_init())
 *				-# MSG待ち無限Loop処理へ移行する
 *					-# スレッド間通信キュー受信関数をCallする(BPF_RU_IPCM_MSGQ_RECV())
 *					-# 受信メッセージ解析関数をCallする(f_rfcp_anlz())
 *					-# 通信用共有メモリの解放(BPF_RU_IPCM_PROCMSG_ADDRFREE())
 * @param		N/A
 * @retval		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	PF-Rfp-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2013/03/07 ALPHA)松延 Create
 * @date		2013/11/22 ALPHA)藤井 Modify for docomo zynq
 */
/****************************************************************************************************************************/
VOID f_rfcp_main(void)
{
	VOID				*msgP;
	UINT				rcvmsgsz;
	INT					ret;
	
	/* スレッド名表示	*/
	prctl(PR_SET_NAME, "pf_rfa", 0, 0, 0);
	
	/* BPFへスレッド情報登録	*/
	BPF_RM_SVRM_THREAD_INFO_SET(	D_RRH_PROCID_PF,				/* ProcessID		*/
									D_SYS_THDID_PF_RFA,			/* ThreadID			*/
									D_SYS_THDQID_PF_RFA,			/* ThreadQueueID	*/
								 (unsigned long int)pthread_self());	/* pThreadID			*/
	
	/* タスク状態初期化		*/
	f_rfcpw_thdInfo.thdState = D_RFCP_THD_INIT;
	
	/* 初期化処理関数Call	*/
	(VOID)f_rfcp_init(NULL);
	
	/************************************************************************************************************************/
	/* メッセージ受信待ち																									*/
	/************************************************************************************************************************/
	while(1)
	{
		/* メッセージ受信	*/
		ret = BPF_RU_IPCM_MSGQ_RECV(D_SYS_THDQID_PF_RFA,				/* Thread Qid						*/
									0,									/* 応答待ち時間						*/
									(void **)&msgP,						/* 通信用共有メモリのポインタ		*/
									&rcvmsgsz );						/* Receiveしたメッセージのサイズ	*/
		
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}

		/* 受信メッセージ解析関数Call	*/
		(VOID)f_rfcp_anlz((VOID*)msgP);
		
		/* 通信用共有メモリの解放	*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)msgP );
		
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}
	}
	
	return;
}

/* @} */
